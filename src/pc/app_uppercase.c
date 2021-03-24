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


// DEFINEs go here

/*
 * PROTOTYPEs go here
 *
 */

// GLOBALs go here
//  Generally, the user-created semaphores will be defined/allocated here
volatile uint32_t   u32_T2Count;
volatile uint32_t   u32_T3Count;
static uint8_t psz_T2Is[]="T2 is ";
static uint8_t psz_T3Is[]="T3 is ";
static uint8_t psz_Enabled[]="enabled.";
static uint8_t psz_Disabled[]="disabled.";
static uint8_t psz_CRNL[3]= {0x0D, 0x0A, 0};

struct stTask*    pst_MyTasks[3];

/************************************************************************
 * User supplied functions
 ************************************************************************
 */

/******************************************************************************
 * Function:        upper_case
 *
 * PreConditions:   This ESOS task has been registered with ESOS properly
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:
 *
 * Overview:        A task to read the data buffers, convert them to upper case
 *                  and send the data right back out.
 *
 * Notes:
 *
 *****************************************************************************/
ESOS_USER_TASK( upper_case ) {
  static uint8_t      u8_Char;

  ESOS_TASK_BEGIN(pstTask);
  while (TRUE) {
    ESOS_TASK_WAIT_ON_AVAILABLE_IN_COMM();
    ESOS_TASK_SIGNAL_BUSY_IN_COMM();
    ESOS_TASK_WAIT_ON_GET_UINT8( u8_Char );
    ESOS_TASK_RELEASE_IN_COMM();
    if ((u8_Char >= 'a') && (u8_Char <= 'z') )
      u8_Char = u8_Char - 'a' + 'A';
    ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
    ESOS_TASK_SIGNAL_BUSY_OUT_COMM();
    ESOS_TASK_WAIT_ON_SEND_UINT8( u8_Char);
    ESOS_TASK_RELEASE_OUT_COMM();
  } // endof while(TRUE)
  ESOS_TASK_END(pstTask);
}

/******************************************************************************
 * Function:        echo
 *
 * PreConditions:   This ESOS task has been registered with ESOS properly
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:
 *
 * Overview:        A task to read the data buffers, and echo it write back out
 *
 * Notes:
 *
 *****************************************************************************/
ESOS_USER_TASK( echo ) {
  static uint8_t      u8_Char;

  ESOS_TASK_BEGIN(pstTask);
  while (TRUE) {
    ESOS_TASK_WAIT_ON_AVAILABLE_IN_COMM();
    ESOS_TASK_SIGNAL_BUSY_IN_COMM();
    ESOS_TASK_WAIT_ON_GET_UINT8( u8_Char );
    ESOS_TASK_RELEASE_IN_COMM();
    ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
    ESOS_TASK_SIGNAL_BUSY_OUT_COMM();
    ESOS_TASK_WAIT_ON_SEND_UINT8( u8_Char);
    ESOS_TASK_RELEASE_OUT_COMM();
  } // endof while(TRUE)
  ESOS_TASK_END(pstTask);
}

// user task to talk to child task RX/TX tasks --- bypasses the
//   ESOS comm system.  Requires that you "disable" the
//   ESOS comm system task in the main ESOS code.
//
//  USED ONLY FOR DEVELOPMENT AND TESTING!
ESOS_USER_TASK( child_echo_buffers ) {

#define   LOCAL_BUFFER_LEN    16

  static uint8_t            u8_i;
  static uint8_t            au8_Char[LOCAL_BUFFER_LEN+4];

  ESOS_TASK_BEGIN(pstTask);
  while (TRUE) {
    ESOS_TASK_WAIT_ON_AVAILABLE_IN_COMM();
    ESOS_TASK_SIGNAL_BUSY_IN_COMM();
    ESOS_TASK_WAIT_ON_GET_U8BUFFER( &au8_Char[0], LOCAL_BUFFER_LEN);
    ESOS_TASK_RELEASE_IN_COMM();
    for (u8_i=0; u8_i<LOCAL_BUFFER_LEN; u8_i++) {
      if ((au8_Char[u8_i] >= 'A') && (au8_Char[u8_i] <= 'Z') )
        au8_Char[u8_i] = au8_Char[u8_i] - 'A' + 'a';
    } // end for

    ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
    ESOS_TASK_SIGNAL_BUSY_OUT_COMM();
    ESOS_TASK_WAIT_ON_SEND_U8BUFFER( &au8_Char[0], LOCAL_BUFFER_LEN );
    ESOS_TASK_RELEASE_OUT_COMM();
  } // endof while(TRUE)
  ESOS_TASK_END(pstTask);
} // end child_task

// user task to transmit a 32bit random number
//
//  USED ONLY FOR DEVELOPMENT AND TESTING!
ESOS_USER_TASK( random_xmit ) {
  static uint32_t    u32_RandomNumber;
  static uint32_t    u32_count;
  UINT32    U32_Temp;

  ESOS_TASK_BEGIN(pstTask);
  u32_RandomNumber = 0;
  while (TRUE) {
    u32_RandomNumber = 245 + (esos_GetRandomUint32() & 0xFF);
    U32_Temp._uint32_t = u32_RandomNumber;
    ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
    ESOS_TASK_SIGNAL_BUSY_OUT_COMM();
#if FALSE
    ESOS_TASK_WAIT_ON_SEND_UINT8( U32_Temp.u8LoLsb );
    ESOS_TASK_WAIT_ON_SEND_UINT8( U32_Temp.u8LoMsb );
    ESOS_TASK_WAIT_ON_SEND_UINT8( U32_Temp.u8HiLsb );
    ESOS_TASK_WAIT_ON_SEND_UINT8( U32_Temp.u8HiMsb );
#else
    ESOS_TASK_WAIT_ON_SEND_UINT32_AS_HEX_STRING( u32_RandomNumber );
#endif
    ESOS_TASK_WAIT_ON_SEND_STRING( psz_CRNL );
    ESOS_TASK_RELEASE_OUT_COMM();

    ESOS_TASK_WAIT_TICKS( pstTask, u32_RandomNumber );
  } // endof while(TRUE)
  ESOS_TASK_END(pstTask);
} // end child_task


// user task to transmit a 32bit random number
//
//  USED ONLY FOR DEVELOPMENT AND TESTING!
ESOS_USER_TASK( random_1 ) {
  static uint32_t       u32_RandomNumber;
  static uint8_t        u8_RandomNumber;
  static uint8_t        u8_Count;
  UINT32              U32_Temp;

  ESOS_TASK_BEGIN(pstTask);
  while (TRUE) {
    u8_Count = 0x13;
    while (u8_Count--) {
      u32_RandomNumber = esos_GetRandomUint32();
      U32_Temp._uint32_t = u32_RandomNumber;
      u8_RandomNumber = U32_Temp.u8LoLsb ^ U32_Temp.u8LoMsb ^ U32_Temp.u8HiLsb ^ U32_Temp.u8HiMsb;
      ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
      ESOS_TASK_SIGNAL_BUSY_OUT_COMM();
      ESOS_TASK_WAIT_ON_SEND_STRING( "R1: ");
      ESOS_TASK_WAIT_ON_SEND_UINT8_AS_HEX_STRING( u8_RandomNumber );
      ESOS_TASK_WAIT_ON_SEND_UINT8( ' ');
      ESOS_TASK_WAIT_ON_SEND_UINT8_AS_HEX_STRING( u8_Count );
      ESOS_TASK_WAIT_ON_SEND_STRING( psz_CRNL );
      ESOS_TASK_RELEASE_OUT_COMM();
      ESOS_TASK_WAIT_TICKS(pstTask, 10+4*u8_RandomNumber );
    } // endof while (u8_Count)
    ESOS_TASK_SLEEP( pstTask );
  } // endof while(TRUE)
  ESOS_TASK_END(pstTask);
} // end child_task

// user task to transmit a 32bit random number
//
//  USED ONLY FOR DEVELOPMENT AND TESTING!
ESOS_USER_TASK( random_2 ) {
  static uint32_t       u32_RandomNumber;
  static uint8_t        u8_RandomNumber;
  static uint8_t        u8_Count;
  UINT32              U32_Temp;

  ESOS_TASK_BEGIN(pstTask);
  //while(TRUE) {
  u8_Count = 0x16 ;
  while (u8_Count--) {
    u32_RandomNumber = esos_GetRandomUint32();
    U32_Temp._uint32_t = u32_RandomNumber;
    u8_RandomNumber = U32_Temp.u8LoLsb ^ U32_Temp.u8LoMsb ^ U32_Temp.u8HiLsb ^ U32_Temp.u8HiMsb;
    ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
    ESOS_TASK_SIGNAL_BUSY_OUT_COMM();
    ESOS_TASK_WAIT_ON_SEND_STRING( "R2: ");
    ESOS_TASK_WAIT_ON_SEND_UINT8_AS_HEX_STRING( u8_RandomNumber );
    ESOS_TASK_WAIT_ON_SEND_UINT8( ' ');
    ESOS_TASK_WAIT_ON_SEND_UINT8_AS_HEX_STRING( u8_Count );
    ESOS_TASK_WAIT_ON_SEND_STRING( psz_CRNL );
    ESOS_TASK_RELEASE_OUT_COMM();
    ESOS_TASK_WAIT_TICKS(pstTask, 10+4*u8_RandomNumber );

  } // endof while (u8_Count)
  // uncomment the line below to have the task SLEEP when finished,
  //  else, it will just die
  //ESOS_TASK_SLEEP( pstTask );
  //ESOS_TASK_SLEEP( pstTask );
  //} // endof while(TRUE)
  ESOS_TASK_END(pstTask);
} // end child_task

// user task to transmit a 32bit random number
//
//  USED ONLY FOR DEVELOPMENT AND TESTING!
ESOS_USER_TASK( random_3 ) {
  static uint32_t       u32_RandomNumber;
  static uint8_t        u8_RandomNumber;
  static uint8_t        u8_Count;
  UINT32              U32_Temp;

  ESOS_TASK_BEGIN(pstTask);
  while (TRUE) {
    u8_Count = 0x11;
    while (u8_Count--) {
      u32_RandomNumber = esos_GetRandomUint32();
      U32_Temp._uint32_t = u32_RandomNumber;
      u8_RandomNumber = U32_Temp.u8LoLsb;
      ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
      ESOS_TASK_SIGNAL_BUSY_OUT_COMM();
      ESOS_TASK_WAIT_ON_SEND_STRING( "R3: ");
      ESOS_TASK_WAIT_ON_SEND_UINT8_AS_HEX_STRING( u8_RandomNumber );
      ESOS_TASK_WAIT_ON_SEND_UINT8( ' ');
      ESOS_TASK_WAIT_ON_SEND_UINT8_AS_HEX_STRING( u8_Count );
      ESOS_TASK_WAIT_ON_SEND_STRING( psz_CRNL );
      ESOS_TASK_RELEASE_OUT_COMM();
      ESOS_TASK_WAIT_TICKS(pstTask, 10+4*u8_RandomNumber );
    } // endof while (u8_Count)
    ESOS_TASK_SLEEP( pstTask );
  } // endof while(TRUE)
  ESOS_TASK_END(pstTask);
} // end child_task


/******************************************************************************
 * Function:        task_ctrl
 *
 * PreConditions:   This ESOS task has been registered with ESOS properly
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:
 *
 * Overview:        A task to sleep, wake, kill, restart, and re-register
 *                  a few other tasks
 *
 * Notes:
 *
 *****************************************************************************/
ESOS_USER_TASK( task_ctrl ) {
  static  uint8_t         u8_Char;
  static  uint8_t         u8_i;

  ESOS_TASK_BEGIN(pstTask);
  while (TRUE) {
    ESOS_TASK_WAIT_ON_AVAILABLE_IN_COMM();
    ESOS_TASK_SIGNAL_BUSY_IN_COMM();
    ESOS_TASK_WAIT_ON_GET_UINT8( u8_Char );
    ESOS_TASK_RELEASE_IN_COMM();

    // do NOT wait for permission... Just seize control of
    // the output communications system.  Release it when
    // we are done so an errant task can be overcome.
    //ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
    ESOS_TASK_SIGNAL_BUSY_OUT_COMM();

    if ( u8_Char == '1') {
      ESOS_TASK_WAIT_ON_SEND_STRING( "Registering Task 1" );
      ESOS_TASK_WAIT_ON_SEND_STRING( psz_CRNL );
      pst_MyTasks[0] = esos_RegisterTask( random_1 );
    } else if (u8_Char == '2') {
      ESOS_TASK_WAIT_ON_SEND_STRING( "Registering Task 2" );
      ESOS_TASK_WAIT_ON_SEND_STRING( psz_CRNL );
      pst_MyTasks[1] = esos_RegisterTask( random_2 );
    } else if (u8_Char == '3') {
      ESOS_TASK_WAIT_ON_SEND_STRING( "Registering Task 3" );
      ESOS_TASK_WAIT_ON_SEND_STRING( psz_CRNL );
      pst_MyTasks[2] = esos_RegisterTask( random_3 );
    } else if (u8_Char == 'Q') {
      ESOS_TASK_WAIT_ON_SEND_STRING( "Killing Task 1" );
      ESOS_TASK_WAIT_ON_SEND_STRING( psz_CRNL );
      ESOS_TASK_KILL(pst_MyTasks[0] );
    } else if (u8_Char == 'W') {
      ESOS_TASK_WAIT_ON_SEND_STRING( "Killing Task 2" );
      ESOS_TASK_WAIT_ON_SEND_STRING( psz_CRNL );
      ESOS_TASK_KILL(pst_MyTasks[1] );
    } else if (u8_Char == 'E') {
      ESOS_TASK_WAIT_ON_SEND_STRING( "Killing Task 3" );
      ESOS_TASK_WAIT_ON_SEND_STRING( psz_CRNL );
      ESOS_TASK_KILL(pst_MyTasks[2] );
    } else if (u8_Char == 'A') {
      ESOS_TASK_WAIT_ON_SEND_STRING( "Restarting Task 1" );
      ESOS_TASK_WAIT_ON_SEND_STRING( psz_CRNL );
      ESOS_TASK_INIT( pst_MyTasks[0] );
    } else if (u8_Char == 'S') {
      ESOS_TASK_WAIT_ON_SEND_STRING( "Restarting Task 2" );
      ESOS_TASK_WAIT_ON_SEND_STRING( psz_CRNL );
      ESOS_TASK_INIT( pst_MyTasks[1] );
    } else if (u8_Char == 'D') {
      ESOS_TASK_WAIT_ON_SEND_STRING( "Restarting Task 3" );
      ESOS_TASK_WAIT_ON_SEND_STRING( psz_CRNL );
      ESOS_TASK_INIT( pst_MyTasks[2] );
    } else if (u8_Char == 'Z') {
      ESOS_TASK_WAIT_ON_SEND_STRING( "Waking Task 1" );
      ESOS_TASK_WAIT_ON_SEND_STRING( psz_CRNL );
      ESOS_TASK_WAKE(pst_MyTasks[0] );
    } else if (u8_Char == 'X') {
      ESOS_TASK_WAIT_ON_SEND_STRING( "Waking Task 2" );
      ESOS_TASK_WAIT_ON_SEND_STRING( psz_CRNL );
      ESOS_TASK_WAKE(pst_MyTasks[1] );
    } else if (u8_Char == 'C') {
      ESOS_TASK_WAIT_ON_SEND_STRING( "Waking Task 3" );
      ESOS_TASK_WAIT_ON_SEND_STRING( psz_CRNL );
      ESOS_TASK_WAKE(pst_MyTasks[2] );
    } else if (u8_Char == ' ') {
      ESOS_TASK_WAIT_ON_SEND_STRING( psz_CRNL );
      ESOS_TASK_WAIT_ON_SEND_STRING( "Status:" );
      ESOS_TASK_WAIT_ON_SEND_STRING( psz_CRNL );
      ESOS_TASK_WAIT_ON_SEND_STRING( "=======" );
      ESOS_TASK_WAIT_ON_SEND_STRING( psz_CRNL );
      for (u8_i=0; u8_i<3; u8_i++) {
        ESOS_TASK_WAIT_ON_SEND_STRING( "Task#" );
        ESOS_TASK_WAIT_ON_SEND_UINT8_AS_HEX_STRING( 1+u8_i );
        ESOS_TASK_WAIT_ON_SEND_STRING( " is " );
        if (ESOS_TASK_IS_SLEEPING(pst_MyTasks[u8_i])) {
          ESOS_TASK_WAIT_ON_SEND_STRING( "sleeping." );
        }
        if (ESOS_TASK_IS_KILLED(pst_MyTasks[u8_i])) {
          ESOS_TASK_WAIT_ON_SEND_STRING( "killed." );
        }
        if (__ESOS_TASK_IS_CALLED(pst_MyTasks[u8_i])) {
          ESOS_TASK_WAIT_ON_SEND_STRING( "called." );
        }
        if (ESOS_TASK_IS_ENDED(pst_MyTasks[u8_i])) {
          ESOS_TASK_WAIT_ON_SEND_STRING( "ended." );
        }
        if (ESOS_TASK_IS_WAITING(pst_MyTasks[u8_i])) {
          ESOS_TASK_WAIT_ON_SEND_STRING( "waiting." );
        }
        ESOS_TASK_WAIT_ON_SEND_STRING( psz_CRNL );
      } // endof for()
      ESOS_TASK_WAIT_ON_SEND_STRING( psz_CRNL );
    } else {
      ESOS_TASK_WAIT_ON_SEND_UINT8( u8_Char );
    }
    ESOS_TASK_RELEASE_OUT_COMM();
  }
  ESOS_TASK_END(pstTask);
}

/******************************************************************************
 * Function:        watchdog
 *
 * PreConditions:   This ESOS task has been registered with ESOS properly
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:
 *
 * Overview:        A task to alert me when tasks are no longer located where
 *                  they should be
 *
 * Notes:
 *
 *****************************************************************************/
ESOS_USER_TASK( watchdog ) {
  static    uint8_t           u8_i;

  u8_i = FALSE;
  ESOS_TASK_BEGIN(pstTask);
  while (TRUE) {
    if (pst_MyTasks[0]->pfn != random_1 ) u8_i |= ESOS_BIT0;
    if (pst_MyTasks[1]->pfn != random_2 ) u8_i |= ESOS_BIT1;
    if (pst_MyTasks[2]->pfn != random_3 ) u8_i |= ESOS_BIT2;
    if (u8_i) {
      ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
      ESOS_TASK_SIGNAL_BUSY_OUT_COMM();

      if ( u8_i && ESOS_BIT0 ) {
        ESOS_TASK_WAIT_ON_SEND_STRING( "pstTasks[0] is now " );
        ESOS_TASK_WAIT_ON_SEND_UINT32_AS_HEX_STRING( (uint32_t) pst_MyTasks[0]->pfn );
        ESOS_TASK_WAIT_ON_SEND_STRING( psz_CRNL );
      }
      if ( u8_i && ESOS_BIT1 ) {
        ESOS_TASK_WAIT_ON_SEND_STRING( "pstTasks[1] is now" );
        ESOS_TASK_WAIT_ON_SEND_UINT32_AS_HEX_STRING( (uint32_t) pst_MyTasks[1]->pfn );
        ESOS_TASK_WAIT_ON_SEND_STRING( psz_CRNL );
      }
      if ( u8_i && ESOS_BIT2 ) {
        ESOS_TASK_WAIT_ON_SEND_STRING( "pstTasks[2] is now" );
        ESOS_TASK_WAIT_ON_SEND_UINT32_AS_HEX_STRING( (uint32_t) pst_MyTasks[2]->pfn );
        ESOS_TASK_WAIT_ON_SEND_STRING( psz_CRNL );
      }
      ESOS_TASK_RELEASE_OUT_COMM();
    } //end of if(u8_i)
    ESOS_TASK_YIELD( pstTask );
  } //endof while(TRUE)
  ESOS_TASK_END(pstTask);
} // endof watchdog() task


/******************************************************************************
 * Function:        watchdog
 *
 * PreConditions:   This ESOS task has been registered with ESOS properly
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:
 *
 * Overview:        A task to alert me when tasks are no longer located where
 *                  they should be
 *
 * Notes:
 *
 *****************************************************************************/
ESOS_USER_TASK( init_print ) {

  ESOS_TASK_BEGIN(pstTask);
  ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
  ESOS_TASK_SIGNAL_BUSY_OUT_COMM();
  ESOS_TASK_WAIT_ON_SEND_STRING( "pstTasks[0] is " );
  ESOS_TASK_WAIT_ON_SEND_UINT32_AS_HEX_STRING( (uint32_t) pst_MyTasks[0]->pfn );
  ESOS_TASK_WAIT_ON_SEND_STRING( psz_CRNL );
  ESOS_TASK_WAIT_ON_SEND_STRING( "pstTasks[1] is " );
  ESOS_TASK_WAIT_ON_SEND_UINT32_AS_HEX_STRING( (uint32_t) pst_MyTasks[1]->pfn );
  ESOS_TASK_WAIT_ON_SEND_STRING( psz_CRNL );
  ESOS_TASK_WAIT_ON_SEND_STRING( "pstTasks[2] is " );
  ESOS_TASK_WAIT_ON_SEND_UINT32_AS_HEX_STRING( (uint32_t) pst_MyTasks[2]->pfn );
  ESOS_TASK_WAIT_ON_SEND_STRING( psz_CRNL );
  ESOS_TASK_RELEASE_OUT_COMM();
  ESOS_TASK_END(pstTask);

} // endof init_print() task




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

  __esos_hw_PutString( HELLO_MSG );

  /*
   * Now, let's get down and dirty with ESOS and our user tasks
   *
   *   Once tasks are registered, they will start executing in
   *   the ESOS scheduler.
   */


  // here are several combinations of tasks that should work together
#if 0
  esos_RegisterTask( upper_case );
#endif

#if 0
  esos_RegisterTask( child_echo_buffers );
#endif

#if 1
  esos_RegisterTask( random_xmit );
#endif

#if 0
  esos_RegisterTask( check_timers );
#endif

#if 0
  esos_RegisterTask( child_echo_buffers );
  esos_RegisterTask( random_xmit );
#endif

#if 0
  esos_RegisterTask( random_1 );
  esos_RegisterTask( random_2 );
  esos_RegisterTask( random_3 );
#endif

#if 0
  esos_RegisterTask( echo );
  esos_RegisterTask( random_1 );
  esos_RegisterTask( random_2 );
  esos_RegisterTask( random_3 );
#endif

#if 0
  esos_RegisterTask( child_echo_buffers );
  esos_RegisterTask( random_1 );
  esos_RegisterTask( random_2 );
  esos_RegisterTask( random_3 );
#endif

#if 0
  esos_RegisterTask( upper_case );
  esos_RegisterTask( random_1 );
  esos_RegisterTask( random_2 );
  esos_RegisterTask( random_3 );
#endif

#if 0
  pst_MyTasks[0] = esos_RegisterTask( random_1 );
  pst_MyTasks[1] = esos_RegisterTask( random_2 );
  pst_MyTasks[2] = esos_RegisterTask( random_3 );
  esos_RegisterTask( task_ctrl );
#endif

#if 0
  pst_MyTasks[0] = esos_RegisterTask( random_1 );
  pst_MyTasks[1] = esos_RegisterTask( random_2 );
  pst_MyTasks[2] = esos_RegisterTask( random_3 );
  esos_RegisterTask( task_ctrl );
  esos_RegisterTask( init_print );
  esos_RegisterTask( watchdog );
#endif


} // end user_init()
