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
#include  	"user_app.h"
#include	"stdio.h"
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


/**********************************************************
**
**  create a few tasks to generate and print random numbers
**
************************************************************/
ESOS_USER_TASK( periodic_rand ) {
  static uint32_t		u32_rnd;
  static  char			psz_tempStr[80];

  ESOS_TASK_BEGIN();
  while (TRUE) {

    ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
    sprintf(psz_tempStr,"periodic: 0x%08lx\n", esos_GetRandomUint32() );
	ESOS_TASK_WAIT_ON_SEND_STRING( psz_tempStr );
	ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
	
    // wait a random amount of time between sending mail messages
    //  use the hidden ESOS software RNG so as not to disturb
    //  the hardware RNG we are testing
    u32_rnd = 1+(0x0F & __esos_get_PRNG_RandomUint32());
    u32_rnd <<= 7;
    ESOS_TASK_WAIT_TICKS( u32_rnd);
  } // endof while(TRUE)
  ESOS_TASK_END();
} // end periodic_rand()

ESOS_USER_TASK( burst_rand ) {
  static uint32_t		u32_rnd;
  static uint32_t		u32_r1, u32_r2, u32_r3, u32_r4, u32_r5, u32_r6;
  static  char			psz_tempStr[80];

  ESOS_TASK_BEGIN();
  while (TRUE) {
    ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
    u32_r1=esos_GetRandomUint32();       u32_r2=esos_GetRandomUint32();
    u32_r3=esos_GetRandomUint32();       u32_r4=esos_GetRandomUint32();
    u32_r5=esos_GetRandomUint32();       u32_r6=esos_GetRandomUint32();
    sprintf(psz_tempStr,"burst: 0x%08lx\n", u32_r1 );
	ESOS_TASK_WAIT_ON_SEND_STRING( psz_tempStr );
    sprintf(psz_tempStr,"burst: 0x%08lx\n", u32_r2 );
	ESOS_TASK_WAIT_ON_SEND_STRING( psz_tempStr );
    sprintf(psz_tempStr,"burst: 0x%08lx\n", u32_r3 );
	ESOS_TASK_WAIT_ON_SEND_STRING( psz_tempStr );
    sprintf(psz_tempStr,"burst: 0x%08lx\n", u32_r4 );
	ESOS_TASK_WAIT_ON_SEND_STRING( psz_tempStr );
    sprintf(psz_tempStr,"burst: 0x%08lx\n", u32_r5 );
	ESOS_TASK_WAIT_ON_SEND_STRING( psz_tempStr );
    sprintf(psz_tempStr,"burst: 0x%08lx\n", u32_r6 );
	ESOS_TASK_WAIT_ON_SEND_STRING( psz_tempStr );
	ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
	
    // wait a random amount of time between sending mail messages
    //  use the hidden ESOS software RNG so as not to disturb
    //  the hardware RNG we are testing
    u32_rnd = 1+(0x0F & __esos_get_PRNG_RandomUint32());
    u32_rnd <<= 10;
    ESOS_TASK_WAIT_TICKS( u32_rnd);
  } // endof while(TRUE)
  ESOS_TASK_END();
} // end periodic_rand()

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
  esos_RegisterTask( periodic_rand );
  esos_RegisterTask( burst_rand );

} // end user_init()

