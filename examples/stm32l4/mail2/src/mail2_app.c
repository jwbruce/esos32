/*
 * "Copyright (c) 2019 J. W. Bruce ("AUTHOR(S)")"
 * All rights reserved.
 * (J. W. Bruce, jwbruce_AT_tntech.edu, Tennessee Tech University)
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose, without fee, and without written agreement is
 * hereby granted, provided that the above copyright notice, the following
 * two paragraphs and the authors appear in all copies of this software.
 *
 * IN NO EVENT SHALL THE "AUTHORS" BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
 * OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE "AUTHORS"
 * HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * THE "AUTHORS" SPECIFICALLY DISCLAIMS ANY WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE "AUTHORS" HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS."
 *
 * Please maintain this header in its entirety when copying/modifying
 * these files.
 *
 *
 */

// #define ESOS_USE_IRQS

// INCLUDEs go here  (First include the main esos.h file)
//      After that, the user can include what they need
#include    "esos.h"
#include  "user_app.h"
#ifdef __linux
#include    "esos_pc.h"
#include    "esos_pc_stdio.h"
// INCLUDE these so our printf and other PC hacks work
#include <stdio.h>
#include <sys/select.h>
#include <termios.h>
#include <unistd.h>
#else
#include    "esos_stm32l4.h"
#include    "esos_stm32l4_rs232.h"
#endif


// DEFINEs go here

/*
 * PROTOTYPEs go here
 *
 */
void __user_init_hw( void );
uint32_t getRandomDelay(void);
ESOS_USER_TASK(sender_B0);
ESOS_USER_TASK(recipient_B);


// GLOBALs go here
//  Generally, the user-created semaphores will be defined/allocated here
char psz_CRNL[3]= {0x0D, 0x0A, 0};

extern  UART_HandleTypeDef    st_huart2;

// timer globals

#ifdef _linux
/*
 * Simulate the timer ISR found on a MCU
 *   The PC doesn't have a timer ISR, so this task will periodically
 *   call the timer services callback instead.
 *   USED ONLY FOR DEVELOPMENT AND TESTING ON PC.
 *   Real MCU hardware doesn't need this task
 */
ESOS_USER_TASK( __simulated_isr ) {
  ESOS_TASK_BEGIN();
  while (TRUE) {
    // call the ESOS timer services callback just like a real H/W ISR would
    __esos_tmrSvcsExecute();
    ESOS_TASK_WAIT_TICKS(  1 );

  } // endof while(TRUE)
  ESOS_TASK_END();
} // end child_task
#endif

ESOS_USER_TASK( task_flash_LED ) {
  ESOS_TASK_BEGIN();
  while (TRUE) {
    HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
    if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET) {
      ESOS_TASK_WAIT_TICKS( 125 );
    } else {
      ESOS_TASK_WAIT_TICKS( 600 );
    }
  } // endof while(TRUE)
  ESOS_TASK_END();
} // end task_flash_LED()

ESOS_USER_TASK( sender_B0 ) {
  uint32_t                u32_rnd;
  static  uint8_t         u8_cnt;
  static  ESOS_TASK_HANDLE    hTask;
  static  MAILMESSAGE       st_Message;
  static  char				psz_tempStr[80];

  ESOS_TASK_BEGIN();
  u8_cnt=0;
  // store handle to our recipient ESOS task
  hTask = esos_GetTaskHandle( recipient_B );

  while (TRUE) {

    // create/fill in the local copy of the message
    // we will send a single uint8 as data payload
    ESOS_TASK_MAKE_MSG_UINT8(st_Message, u8_cnt);

    // simulate a task that does other things if the
    // receipient task mailbox is full

    if (ESOS_TASK_MAILBOX_GOT_AT_LEAST_DATA_BYTES( hTask, sizeof(uint8_t) ) ) {
      // ESOS task mailboxes are stored locally in each task.
      // but we already now the recipient task has enough space
      // in their mailbox, so send message
      ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
      sprintf(psz_tempStr, "B0 sending MESSAGE %d\n", u8_cnt);
      ESOS_TASK_WAIT_ON_SEND_STRING( psz_tempStr );
      ESOS_TASK_SEND_MESSAGE(hTask, &st_Message);
    } else {
      sprintf(psz_tempStr,"B0 doing useful work instead of mailing. Discarding MESSAGE %d.\n", u8_cnt );
      ESOS_TASK_WAIT_ON_SEND_STRING( psz_tempStr );
    }
    ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();

    u8_cnt++;
    if (u8_cnt>50) u8_cnt=0;

    // wait a random amount of time between sending mail messages
    u32_rnd = 1+(0x0F & esos_GetRandomUint32());
    u32_rnd <<= 6;
    ESOS_TASK_WAIT_TICKS( u32_rnd);

  } // endof while(TRUE)
  ESOS_TASK_END();
} // end sender_B0()

//TASK that doesn't check mail very often
ESOS_USER_TASK( recipient_B ) {
  uint32_t            u32_rnd;
  static uint8_t      u8_cnt;
  static MAILMESSAGE    st_Msg;
  static ESOS_TASK_HANDLE   h_SenderB0;
  static char				psz_tempStr[80];

  ESOS_TASK_BEGIN();
  h_SenderB0 = esos_GetTaskHandle (sender_B0);
  while (TRUE) {

    // create a random delay to simulate being "busy"
    u32_rnd = 1+(0x0F & esos_GetRandomUint32());
    u32_rnd <<= 10;
    ESOS_TASK_WAIT_TICKS( u32_rnd );

    // check for incoming mail
    ESOS_TASK_WAIT_FOR_MAIL();
    u8_cnt=0;

    // keep reading mail messages until they are all processed
    while ( ESOS_TASK_IVE_GOT_MAIL() ) {
      // make local copy of message (frees up mailbox space)
      ESOS_TASK_GET_NEXT_MESSAGE( &st_Msg);
      ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
      sprintf(psz_tempStr,"Got a message from ");
      ESOS_TASK_WAIT_ON_SEND_STRING( psz_tempStr );
      if ( ESOS_IS_TASK_SENDER( h_SenderB0, st_Msg) ) {
        sprintf(psz_tempStr,"sender_B0");
      } else {
        printf(psz_tempStr,"UNKNOWN");
      }
      ESOS_TASK_WAIT_ON_SEND_STRING( psz_tempStr );
      sprintf (psz_tempStr," containing %d \t\tenroute time = %ld ms\n", st_Msg.au8_Contents[0], esos_GetSystemTick()-ESOS_GET_MSG_POSTMARK(st_Msg));
      ESOS_TASK_WAIT_ON_SEND_STRING( psz_tempStr );
      ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
    } //endof while()
  } // endof while(TRUE)
  ESOS_TASK_END();
} // end recipient_B()


/************************************************************************
 * User supplied functions
 ************************************************************************
 */
 
 /*
 * return an uint32_t that can be used for a reasonable delay
 * should not be too short (~255 ticks) and not too long (~4096 ticks)
 */
inline uint32_t getRandomDelay() {
  return ((esos_GetRandomUint32() & 0x0FFF)|0x100);
}

 
void __user_init_hw(void) {
  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __GPIOC_CLK_ENABLE();
  __GPIOA_CLK_ENABLE();

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : LED2_Pin */
  GPIO_InitStruct.Pin = LED2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(LED2_GPIO_Port, &GPIO_InitStruct);

}



/******************************************************************************
 * Function:        void user_init(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        user_init is a centralized initialization routine where
 *                  the user can setup their application.   It is called
 *                  automagically by ES_OS during the operating system
 *                  initialization.
 *
 * Note:            The user should set up any state machines and init
 *                  all application variables.  They can also turn on
 *                  any needed peripherals here.
 *
 *                  The user SHALL NOT mess with the interrupt hardware
 *                  directly!!!  The ES_OS must be aware of the interrupts
 *                  and provides osXXXXXXX functions for the user to use.
 *                  Using these ES_OS-provided functions, the user may
 *                  (and probably should) initialize, register, and enable
 *                  interrupts in this routine.
 *
 *                  Furthermore, the user should register AT LEAST one
 *                  user application task here (via esos_RegisterTask) or
 *                  the ES_OS scheduler will have nothing to schedule
 *                  to run when this function returns.
 *
 *****************************************************************************/
void user_init(void) {

  // setup our software structures here
  
  // call our function to setup the hardware we like it.
  __user_init_hw();

  // Drive the UART directly.....  to print the HELLO_MSG
  __esos_unsafe_PutString( HELLO_MSG );

  /*
   * Now, let's get down and dirty with ESOS and our user tasks
   *
   *   Once tasks are registered, they will start executing in
   *   the ESOS scheduler.
   */
  esos_RegisterTask( task_flash_LED );
  esos_RegisterTask( sender_B0 );
  esos_RegisterTask( recipient_B );

} // end user_init()

