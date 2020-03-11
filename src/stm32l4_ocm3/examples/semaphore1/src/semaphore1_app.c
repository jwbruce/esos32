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
#include	"esos.h"
#include	"user_app.h"
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
char psz_T1[] = "Task 1: ";
char psz_T2[] = "Task 2: ";

ESOS_SEMAPHORE( sem_T2CanRun );

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
    gpio_toggle(LED2_GPIO_Port,LED2_Pin);
    if(gpio_get(SW1_GPIO_Port,SW1_Pin) == 0)
    {
      ESOS_TASK_WAIT_TICKS( 125 );
    } else {
      ESOS_TASK_WAIT_TICKS( 600 );
    }
  } // endof while(TRUE)
  ESOS_TASK_END();
} // end task_flash_LED()


/* == task1 ==
 * Print out a counter value local to this task, then delay
 * for a random period of time.  (Delays range from approx.
 * 0.25 - 4.1 seconds)
 */
ESOS_USER_TASK(task1) {
  static  uint8_t           u8_cnt=0;

  ESOS_TASK_BEGIN();
  while (TRUE) {
    ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
    ESOS_TASK_WAIT_ON_SEND_STRING( psz_T1 );
    ESOS_TASK_WAIT_ON_SEND_UINT8_AS_DEC_STRING( u8_cnt );
    ESOS_TASK_WAIT_ON_SEND_STRING( psz_CRNL );
    ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
    ESOS_SIGNAL_SEMAPHORE(sem_T2CanRun, 1);
    u8_cnt++;
    ESOS_TASK_WAIT_TICKS( getRandomDelay() );
  } // endof while(TRUE)
  ESOS_TASK_END();
} // end task1()

/* == task2 ==
 * Wait for a signal from task1, then
 * Print out a counter value local to this task
 */
ESOS_USER_TASK(task2) {
  static  uint8_t           u8_cnt=0;

  ESOS_TASK_BEGIN();
  while (TRUE) {
    ESOS_TASK_WAIT_SEMAPHORE(sem_T2CanRun, 5);
    ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
    ESOS_TASK_WAIT_ON_SEND_STRING( psz_T2 );
    ESOS_TASK_WAIT_ON_SEND_UINT8_AS_HEX_STRING( u8_cnt );
    ESOS_TASK_WAIT_ON_SEND_STRING( psz_CRNL );
    ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
    u8_cnt++;
  } // endof while(TRUE)
  ESOS_TASK_END();
} // end task1()


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
  /* GPIO Ports Clock Enable */
  rcc_periph_clock_enable(GPIOA);
  rcc_periph_clock_enable(GPIOC);

  /*Configure GPIO pin : PC13 */
  gpio_mode_setup(SW1_GPIO_Port, GPIO_MODE_INPUT, GPIO_PUPD_NONE, SW1_Pin);

  /*Configure GPIO pin : LED2_Pin */
  gpio_mode_setup(LED2_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED2_Pin);

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
  esos_RegisterTask( task1 );
  esos_RegisterTask( task2 );

} // end user_init()

