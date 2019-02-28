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

#define ESOS_USE_IRQS

// INCLUDEs go here  (First include the main esos.h file)
//      After that, the user can include what they need
#include    "esos.h"
#include    "esos_pc.h"
#include    "esos_pc_stdio.h"

#include <stdio.h>
#include <sys/select.h>
#include <termios.h>
#include <unistd.h>



// DEFINEs go here

/*
 * PROTOTYPEs go here
 *
 */
uint32_t    randomNumInRange(uint32_t u32_lo, uint32_t u32_hi);


// GLOBALs go here
//  Generally, the user-created semaphores will be defined/allocated here
volatile uint32_t   u32_T2Count;
volatile uint32_t   u32_T3Count;
static uint8_t psz_T2Is[]="T2 is ";
static uint8_t psz_T3Is[]="T3 is ";
static uint8_t psz_Enabled[]="enabled.";
static uint8_t psz_Disabled[]="disabled.";
static uint8_t psz_CRNL[3]= {0x0D, 0x0A, 0};


// timer globals
uint32_t    u32_myT1Count = 0;
uint8_t     LED1 = TRUE;
uint8_t     LED2 = TRUE;

struct stTask*    pst_MyTasks[3];

/************************************************************************
 * User supplied functions
 ************************************************************************
 */

// Simulate the timer ISR on a MCU
//  The PC doesn't have a timer ISR, so this task will periodically
//  call the timer services callback instead.
//  USED ONLY FOR DEVELOPMENT AND TESTING!
ESOS_USER_TASK( __simulated_isr ) {
  static uint32_t    u32_count, u32_tick;

  ESOS_TASK_BEGIN(pstTask);
  u32_count = 0;
  while (TRUE) {
    // call the ESOS timer services callback like the ISR would
    __esos_tmrSvcsExecute();
    ESOS_TASK_WAIT_TICKS( pstTask, 1 );

  } // endof while(TRUE)
  ESOS_TASK_END(pstTask);
} // end child_task

uint32_t    randomNumInRange(uint32_t u32_lo, uint32_t u32_hi) {
  uint32_t  u32_d1, u32_d2, u32_d4, u32_ret;
  UINT32  U32_temp;

  while (TRUE) {
    u32_d4 = esos_GetRandomUint32();
    u32_ret = u32_lo + u32_d4;
    if (u32_ret <= u32_hi) return u32_ret;

    U32_temp._uint32_t = u32_d4;
    u32_d2 = U32_temp.u16LoWord ^ U32_temp.u16HiWord;
    u32_ret = u32_lo + u32_d2;
    if (u32_ret <= u32_hi) return u32_ret;

    u32_d1 = U32_temp.u8LoLsb ^ U32_temp.u8LoMsb ^ U32_temp.u8HiLsb ^ U32_temp.u8HiMsb;
    u32_ret = u32_lo + u32_d1;
    if (u32_ret <= u32_hi) return u32_ret;
  } //endwhile
} //end randomNumInRange


// user-created timer callback
ESOS_USER_TIMER( swTimerCounter ) {
  u32_myT1Count++;
} //endof swTimerCounter

// user-created timer callback
ESOS_USER_TIMER( swTimerLED ) {
  // LED2 = !LED2;
  if (LED2)
    printf("\a");
  else
    printf("\a");
  fflush(stdout);
  LED2 = !LED2;
} //endof sw_Timer_LED

uint32_t    u32_cnt1, u32_cnt2, u32_cnt3;

// user-created timer callback
ESOS_USER_TIMER( swTimerPrintA ) {

  printf("A:%d\n", u32_cnt1++);
  fflush(stdout);
} //endof sw_Timer_LED

ESOS_USER_TIMER( swTimerPrintB ) {

  printf("B:%d\n", u32_cnt2++);
  fflush(stdout);
} //endof sw_Timer_LED

ESOS_USER_TIMER( swTimerPrintC ) {

  printf("C:%d\n", u32_cnt3++);
  fflush(stdout);
} //endof sw_Timer_LED

ESOS_USER_TASK( task1 ) {
  uint32_t     u32_rnd;

  ESOS_TASK_BEGIN(pstTask);
  while (TRUE) {
    u32_rnd = 100*randomNumInRange(1, 30);
    printf("T1 (%d)\n", u32_rnd);
    ESOS_TASK_WAIT_TICKS(pstTask, u32_rnd);
  } // endof while(TRUE)
  ESOS_TASK_END(pstTask);
} // end task1()

ESOS_USER_TASK( task2 ) {
  uint32_t     u32_rnd;

  ESOS_TASK_BEGIN(pstTask);
  while (TRUE) {
    u32_rnd = 100*randomNumInRange(1, 30);
    printf("T2 (%d)\n", u32_rnd);
    ESOS_TASK_WAIT_TICKS(pstTask, u32_rnd);
  } // endof while(TRUE)
  ESOS_TASK_END(pstTask);
} // end task1()

ESOS_USER_TASK( task3 ) {
  uint32_t    u32_rnd;

  ESOS_TASK_BEGIN(pstTask);
  while (TRUE) {
    u32_rnd = 100*randomNumInRange(1, 30);
    printf("T3 (%d)\n", u32_rnd);
    ESOS_TASK_WAIT_TICKS(pstTask, u32_rnd);
  } // endof while(TRUE)
  ESOS_TASK_END(pstTask);
} // end task1()

ESOS_USER_TASK( task_LED ) {
  ESOS_TASK_BEGIN(pstTask);
  while (TRUE) {
    // LED2 = !LED2;
    ESOS_TASK_WAIT_TICKS(pstTask, 1000);
    printf("\a\a");
    fflush(stdout);
  } // endof while(TRUE)
  ESOS_TASK_END(pstTask);
} // end upper_case()

// user task to randomly turn on and off some timer service
//
ESOS_USER_TASK( random_tmr ) {
  static uint32_t       u32_RandomNumber;
  static uint8_t        u8_RandomNumber;
  static uint8_t        u8_Count;
  static ESOS_TMR_HANDLE    tmrhnd_t1;
  UINT32              U32_Temp;

  ESOS_TASK_BEGIN(pstTask);
  while (TRUE) {
    ESOS_TASK_WAIT_TICKS(pstTask, randomNumInRange( 5000, 15000 ) );
    ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
    ESOS_TASK_SIGNAL_BUSY_OUT_COMM();
    ESOS_TASK_WAIT_ON_SEND_STRING( "starting timer 1 (");
    ESOS_TASK_WAIT_ON_SEND_UINT32_AS_HEX_STRING( u32_myT1Count );
    ESOS_TASK_WAIT_ON_SEND_STRING( ")");
    ESOS_TASK_WAIT_ON_SEND_STRING( psz_CRNL );
    ESOS_TASK_RELEASE_OUT_COMM();
    tmrhnd_t1 = esos_RegisterTimer( swTimerCounter, 500 );
    ESOS_TASK_WAIT_TICKS(pstTask, randomNumInRange( 5000, 15000 ) );
    ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
    ESOS_TASK_SIGNAL_BUSY_OUT_COMM();
    ESOS_TASK_WAIT_ON_SEND_STRING( "stopping timer 1 by handle (");
    ESOS_TASK_WAIT_ON_SEND_UINT32_AS_HEX_STRING( u32_myT1Count );
    ESOS_TASK_WAIT_ON_SEND_STRING( ")");
    ESOS_TASK_WAIT_ON_SEND_STRING( psz_CRNL );
    ESOS_TASK_RELEASE_OUT_COMM();
    esos_UnregisterTimer( tmrhnd_t1 );

    ESOS_TASK_WAIT_TICKS(pstTask, randomNumInRange( 5000, 15000 ) );
    ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
    ESOS_TASK_SIGNAL_BUSY_OUT_COMM();
    ESOS_TASK_WAIT_ON_SEND_STRING( "starting timer 1 (");
    ESOS_TASK_WAIT_ON_SEND_UINT32_AS_HEX_STRING( u32_myT1Count );
    ESOS_TASK_WAIT_ON_SEND_STRING( ")");
    ESOS_TASK_WAIT_ON_SEND_STRING( psz_CRNL );
    ESOS_TASK_RELEASE_OUT_COMM();
    tmrhnd_t1 = esos_RegisterTimer( swTimerCounter, 500 );
    ESOS_TASK_WAIT_TICKS(pstTask, randomNumInRange( 5000, 15000 ) );
    ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
    ESOS_TASK_SIGNAL_BUSY_OUT_COMM();
    ESOS_TASK_WAIT_ON_SEND_STRING( "stopping timer 1 by function (");
    ESOS_TASK_WAIT_ON_SEND_UINT32_AS_HEX_STRING( u32_myT1Count );
    ESOS_TASK_WAIT_ON_SEND_STRING( ")");
    ESOS_TASK_WAIT_ON_SEND_STRING( psz_CRNL );
    ESOS_TASK_RELEASE_OUT_COMM();
    esos_UnregisterTimer( esos_GetTimerHandle( swTimerCounter) );

  } // endof while(TRUE)
  ESOS_TASK_END(pstTask);
} // end child_task

// user task to randomly turn on and off some timer service
//
ESOS_USER_TASK( random_tmr_dbg ) {
  static uint32_t       u32_RandomNumber;
  static uint8_t        u8_RandomNumber;
  static uint8_t        u8_Count;
  static ESOS_TMR_HANDLE    tmrhnd_t1;
  static ESOS_TMR_HANDLE    tmrhnd_ret;
  UINT32              U32_Temp;

  ESOS_TASK_BEGIN(pstTask);
  while (TRUE) {
    ESOS_TASK_WAIT_TICKS(pstTask, 1*1000 );
    u32_myT1Count = 0;
    ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
    ESOS_TASK_SIGNAL_BUSY_OUT_COMM();
    ESOS_TASK_WAIT_ON_SEND_STRING( "starting timer 10s/0.1 (");
    ESOS_TASK_WAIT_ON_SEND_UINT32_AS_HEX_STRING( u32_myT1Count );
    ESOS_TASK_WAIT_ON_SEND_STRING( ")");
    ESOS_TASK_WAIT_ON_SEND_STRING( psz_CRNL );
    ESOS_TASK_RELEASE_OUT_COMM();
    // our simulated ISR only runs at 100ms intervals
    tmrhnd_t1 = esos_RegisterTimer( swTimerCounter, 1 );
    ESOS_TASK_WAIT_TICKS(pstTask, 10*1000 );
    ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
    ESOS_TASK_SIGNAL_BUSY_OUT_COMM();
    ESOS_TASK_WAIT_ON_SEND_STRING( "stopping timer 1 by handle (");
    ESOS_TASK_WAIT_ON_SEND_UINT32_AS_HEX_STRING( u32_myT1Count );
    ESOS_TASK_WAIT_ON_SEND_STRING( ")");
    ESOS_TASK_WAIT_ON_SEND_STRING( psz_CRNL );
    ESOS_TASK_RELEASE_OUT_COMM();
    esos_UnregisterTimer( tmrhnd_t1 );

    ESOS_TASK_WAIT_TICKS(pstTask, randomNumInRange( 1*1000, 10*1000 ) );
    u32_myT1Count = 0;
    ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
    ESOS_TASK_SIGNAL_BUSY_OUT_COMM();
    ESOS_TASK_WAIT_ON_SEND_STRING( "starting timer 10s/0.2 (");
    ESOS_TASK_WAIT_ON_SEND_UINT32_AS_HEX_STRING( u32_myT1Count );
    ESOS_TASK_WAIT_ON_SEND_STRING( ")");
    ESOS_TASK_WAIT_ON_SEND_STRING( psz_CRNL );
    ESOS_TASK_RELEASE_OUT_COMM();
    // our simulated ISR only runs at 100ms interval
    tmrhnd_t1 = esos_RegisterTimer( swTimerCounter, 2 );
    ESOS_TASK_WAIT_TICKS(pstTask, 10*1000 );
    ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
    ESOS_TASK_SIGNAL_BUSY_OUT_COMM();
    ESOS_TASK_WAIT_ON_SEND_STRING( "stopping timer 1 by function (");
    ESOS_TASK_WAIT_ON_SEND_UINT32_AS_HEX_STRING( u32_myT1Count );
    ESOS_TASK_WAIT_ON_SEND_STRING( ")");
    ESOS_TASK_WAIT_ON_SEND_STRING( psz_CRNL );
    ESOS_TASK_RELEASE_OUT_COMM();
    esos_UnregisterTimer( esos_GetTimerHandle( swTimerCounter) );

    ESOS_TASK_WAIT_TICKS(pstTask, randomNumInRange( 1*1000, 10*1000 ) );
    u32_myT1Count = 0;
    ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
    ESOS_TASK_SIGNAL_BUSY_OUT_COMM();
    ESOS_TASK_WAIT_ON_SEND_STRING( "starting timer 10s/0.15 (");
    ESOS_TASK_WAIT_ON_SEND_UINT32_AS_HEX_STRING( u32_myT1Count );
    ESOS_TASK_WAIT_ON_SEND_STRING( ")");
    ESOS_TASK_WAIT_ON_SEND_STRING( psz_CRNL );
    ESOS_TASK_RELEASE_OUT_COMM();
    // our simulated ISR only runs at 100ms intervals
    tmrhnd_t1 = esos_RegisterTimer( swTimerCounter, 1 );
    ESOS_TASK_WAIT_TICKS(pstTask, 5*1000 );
    tmrhnd_ret = esos_ChangeTimerPeriod( tmrhnd_t1, 2 );
    if (tmrhnd_ret == ESOS_TMR_FAILURE) {
      ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
      ESOS_TASK_SIGNAL_BUSY_OUT_COMM();
      ESOS_TASK_WAIT_ON_SEND_STRING( "change period failed");
      ESOS_TASK_WAIT_ON_SEND_STRING( psz_CRNL );
      ESOS_TASK_RELEASE_OUT_COMM();
    } //endif
    ESOS_TASK_WAIT_TICKS(pstTask, 5*1000 );
    ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
    ESOS_TASK_SIGNAL_BUSY_OUT_COMM();
    ESOS_TASK_WAIT_ON_SEND_STRING( "stopping timer 1 by function (");
    ESOS_TASK_WAIT_ON_SEND_UINT32_AS_HEX_STRING( u32_myT1Count );
    ESOS_TASK_WAIT_ON_SEND_STRING( ")");
    ESOS_TASK_WAIT_ON_SEND_STRING( psz_CRNL );
    ESOS_TASK_RELEASE_OUT_COMM();
    esos_UnregisterTimer( esos_GetTimerHandle( swTimerCounter) );


  } // endof while(TRUE)
  ESOS_TASK_END(pstTask);
} // end child_task

ESOS_USER_TASK( upper_case ) {
  static uint8_t           u8_char;

  ESOS_TASK_BEGIN(pstTask);
  while (TRUE) {
    ESOS_TASK_WAIT_ON_AVAILABLE_IN_COMM();
    ESOS_TASK_SIGNAL_BUSY_IN_COMM();
    ESOS_TASK_WAIT_ON_GET_UINT8( u8_char );
    ESOS_TASK_RELEASE_IN_COMM();
    if ((u8_char >= 'a') && (u8_char <= 'z') )
      u8_char = u8_char - 'a' + 'A';
    ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
    ESOS_TASK_SIGNAL_BUSY_OUT_COMM();
    ESOS_TASK_WAIT_ON_SEND_UINT8( u8_char);
    ESOS_TASK_RELEASE_OUT_COMM();
  } // endof while(TRUE)
  ESOS_TASK_END(pstTask);
} // end upper_case()

ESOS_USER_TASK( upper_case2 ) {
  static uint8_t           u8_i;
  static uint8_t           au8_x[257];
  static uint8_t           au8_y[257];

  ESOS_TASK_BEGIN(pstTask);
  while (TRUE) {
    ESOS_TASK_WAIT_ON_AVAILABLE_IN_COMM();
    ESOS_TASK_SIGNAL_BUSY_IN_COMM();
    ESOS_TASK_WAIT_ON_GET_STRING( au8_x );
    ESOS_TASK_RELEASE_IN_COMM();
    u8_i = 0;
    while (TRUE) {
      if ((au8_x[u8_i] >= 'a') && (au8_x[u8_i] <= 'z') )
        au8_y[u8_i] = au8_x[u8_i] - 'a' + 'A';
      else
        au8_y[u8_i] = au8_x[u8_i];
      if (au8_x[u8_i] == 0) break;
      u8_i++;
    }
    ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
    ESOS_TASK_SIGNAL_BUSY_OUT_COMM();
    ESOS_TASK_WAIT_ON_SEND_STRING( au8_y );
    ESOS_TASK_RELEASE_OUT_COMM();
  } // endof while(TRUE)
  ESOS_TASK_END(pstTask);
} // end upper_case()


ESOS_USER_TASK( reverse_string ) {
  static uint8_t           u8_char;
  \
  static char*           sz_in[257];
  static char*           sz_out[257];

  ESOS_TASK_BEGIN(pstTask);
  while (TRUE) {
    ESOS_TASK_WAIT_ON_AVAILABLE_IN_COMM();
    ESOS_TASK_SIGNAL_BUSY_IN_COMM();
    ESOS_TASK_WAIT_ON_GET_STRING( sz_in );
    ESOS_TASK_RELEASE_IN_COMM();
    reverseString( sz_in, sz_out );
    ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
    ESOS_TASK_SIGNAL_BUSY_OUT_COMM();
    ESOS_TASK_WAIT_ON_SEND_STRING( sz_out );
    ESOS_TASK_WAIT_ON_SEND_UINT8('\n');
    ESOS_TASK_RELEASE_OUT_COMM();
  } // endof while(TRUE)
  ESOS_TASK_END(pstTask);
} // end upper_case()

/** \file
 *  Inputs a string, outputs the reverse. This file is used
 *  in three MPLAB projects:
 *   reverse_string.mcp - polled RX, TX I/O
 *   uartrx_fifo.mcp - interrupt RX, polled TX I/O
 *   uartrxtx_fifo.mcp - interrupt RX, interrupt TX I/O
 * Interrupt RX inChar1() is selected by defining  UART1_RX_INTERRUPT macro
 * Interrupt TX outChar1() is selected by defining UART1_TX_INTERRUPT macro
 * These macros are defined in their respective MPLAB projects.
*/
void reverseString(char *psz_s1, char *psz_s2) {
  char *psz_s1end;
  if (!(*psz_s1)) {
    *psz_s2 = 0;  //psz_s1 is empty, return.
    return;
  }
  psz_s1end = psz_s1;
  //find end of first string
  while (*psz_s1end) psz_s1end++;
  psz_s1end--;      //backup one to first non-zero byte
  //now copy to S2 in reverse order
  while (psz_s1end != psz_s1) {
    *psz_s2 = *psz_s1end;
    psz_s1end--;
    psz_s2++;
  }
  //copy last byte
  *psz_s2 = *psz_s1end;
  psz_s2++;
  //mark end of string
  *psz_s2 = 0;
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
  uint16_t*   pu16_ptr;
  uint16_t    u16_junk;
  ESOS_TMR_HANDLE    tmrhnd_t1,tmrhnd_t2,tmrhnd_t3;

  __esos_hw_PutString( HELLO_MSG );

  /*
   * Now, let's get down and dirty with ESOS and our user tasks
   *
   *   Once tasks are registered, they will start executing in
   *   the ESOS scheduler.
   */

  // register our ESOS task to mimic MCU's TIMER T1 IRQ which kicks off
  // the ESOS S/W timers
  esos_RegisterTask( __simulated_isr );


  // here are several combinations of tasks that should work together
#if 0
  esos_RegisterTask( random_tmr);
#endif
#if 0
  esos_RegisterTask( random_tmr_dbg);
#endif
#if 0
  esos_RegisterTask( upper_case );
  tmrhnd_t2 = esos_RegisterTimer( swTimerCounter, 500 );
#endif
#if 0
  esos_RegisterTask( upper_case2 );
  tmrhnd_t2 = esos_RegisterTimer( swTimerLED, 1000 );
#endif
#if 0
  esos_RegisterTask( reverse_string );
  esos_RegisterTask( task_LED );
#endif
#if 0
  esos_RegisterTask( reverse_string );
  tmrhnd_t2 = esos_RegisterTimer( sw_Timer_LED, 1000 );
#endif
#if 0
  esos_RegisterTask( upper_case2 );
  esos_RegisterTimer( swTimerLED);
  tmrhnd_t1 = esos_RegisterTimer( swTimerPrintA, 400 );
  tmrhnd_t2 = esos_RegisterTimer( swTimerPrintB, 500 );
  tmrhnd_t3 = esos_RegisterTimer( swTimerPrintC, 750 );
#endif
#if 1
  esos_RegisterTimer( swTimerLED);
  esos_RegisterTask( task1 );
  esos_RegisterTask( task2 );
  esos_RegisterTask( task3 );
#endif





} // end user_init()
