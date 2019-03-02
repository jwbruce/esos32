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
#ifdef __linux
#include    "esos_pc.h"
#include    "esos_pc_stdio.h"
#else
#include    "esos_hwxxx.h"
#include    "esos_hwxxx_rs232.h"
#endif

// INCLUDE these so our printf and other PC hacks work
#include <stdio.h>
#include <sys/select.h>
#include <termios.h>
#include <unistd.h>

// DEFINEs go here

/*
 * PROTOTYPEs go here
 *
 */
void reverseString(char *psz_s1, char *psz_s2);
uint32_t    randomNumInRange(uint32_t u32_lo, uint32_t u32_hi);

ESOS_USER_TASK(mailtaskA);
ESOS_USER_TASK(mailtaskAA);
ESOS_USER_TASK(mailtaskB);
ESOS_USER_TASK(mailtaskC);
ESOS_USER_TASK(mailtask0);
ESOS_USER_TASK(mailtask1);
ESOS_USER_TASK(mailtask2);

ESOS_USER_TASK(mailtaskMSGA);
ESOS_USER_TASK(mailtaskMSG0);
ESOS_USER_TASK(mailtaskMSG1);

// GLOBALs go here
//  Generally, the user-created semaphores will be defined/allocated here
static uint8_t psz_CRNL[3]= {0x0D, 0x0A, 0};


// timer globals
uint32_t    u32_myT1Count = 0;
uint8_t     LED1 = TRUE;
uint8_t     LED2 = TRUE;

ESOS_SEMAPHORE(sem_BCanRun);
ESOS_SEMAPHORE(sem_CCanRun);
ESOS_SEMAPHORE(sem_mutex);

struct stTask*    pst_MyTasks[3];

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


/************************************************************************
 * User supplied functions
 ************************************************************************
 */

/*
 * Returns a random number with a value between the two arguments.
 *
 * /todo  Yes, I know this routine is cheesy.  But, it works and
 * I am in a really big hurry right now.
 */

uint32_t    randomNumInRange(uint32_t u32_lo, uint32_t u32_hi) {
  uint32_t  u32_d1, u32_d2, u32_d4, u32_ret;
  UINT32  U32_temp;

  while (TRUE) {
    u32_d4 = esos_GetRandomUint32();
    u32_ret = u32_lo + u32_d4;
    if (u32_ret <= u32_hi) return u32_ret;

    U32_temp._uint32 = u32_d4;
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
  LED2 = !LED2;
  printf("\a");
  fflush(stdout);
} //endof swTimerLED

// user-created timer callback
ESOS_USER_TIMER( swTimerPrintA ) {
  static uint32_t    u32_cnt;

  printf("A:%d\n", u32_cnt++);
  fflush(stdout);
} //endof swTimerPrintA

ESOS_USER_TIMER( swTimerPrintB ) {
  static uint32_t  u32_cnt;

  printf("B:%d\n", u32_cnt++);
  fflush(stdout);
} //endof swTimerPrintB

ESOS_USER_TIMER( swTimerPrintC ) {
  static uint32_t    u32_cnt;

  printf("C:%d\n", u32_cnt++);
  fflush(stdout);
} //endof swTimerPrintC

/* ======================================
 *  Three tasks to run cooperatively.
 *  They used printf() since I want the routines
 *  to fit a screen.  Plus, this is for the PC
 *  so we can tolerate printf()'s hugeness.
 * ======================================
 */
ESOS_USER_TASK( task1 ) {
  uint32_t     u32_rnd;

  ESOS_TASK_BEGIN();
  while (TRUE) {
    u32_rnd = 1+(0x0F & esos_GetRandomUint32());
    u32_rnd <<= 8;
    printf("T1 (%d)\n", u32_rnd);
    ESOS_TASK_WAIT_TICKS( u32_rnd);
  } // endof while(TRUE)
  ESOS_TASK_END();
} // end task1()

ESOS_USER_TASK( task2 ) {
  uint32_t     u32_rnd;

  ESOS_TASK_BEGIN();
  while (TRUE) {
    u32_rnd = 1+(0x0F & esos_GetRandomUint32());
    u32_rnd <<= 9;
    printf("T2 (%d)\n", u32_rnd);
    ESOS_TASK_WAIT_TICKS( u32_rnd);
  } // endof while(TRUE)
  ESOS_TASK_END();
} // end task2()

ESOS_USER_TASK( task3 ) {
  uint32_t    u32_rnd;

  ESOS_TASK_BEGIN();
  while (TRUE) {
    u32_rnd = 1+(0x0F & esos_GetRandomUint32());
    u32_rnd <<= 10;
    printf("T3 (%d)\n", u32_rnd);
    ESOS_TASK_WAIT_TICKS( u32_rnd);
  } // endof while(TRUE)
  ESOS_TASK_END();
} // end task3()

/* ======================================
 *  Three tasks to run cooperatively.
 *  They used printf() since I want the routines
 *  to fit a screen.  Plus, this is for the PC
 *  so we can tolerate printf()'s hugeness.
 * ======================================
 */
ESOS_USER_TASK( taskSemA ) {
  uint32_t         u32_rnd;
  static uint8_t     u8_cnt;

  ESOS_TASK_BEGIN();
  u8_cnt = 0;
  while (TRUE) {
    u8_cnt++;
    if (u8_cnt == 10) {
      ESOS_SIGNAL_SEMAPHORE( sem_BCanRun, 1 );
    }
    if (u8_cnt == 20) {
      ESOS_SIGNAL_SEMAPHORE( sem_CCanRun, 1 );
    }
    u32_rnd = 1+(0x0F & esos_GetRandomUint32());
    u32_rnd <<= 7;
    printf("taskSemA %d (%d)\n", u8_cnt, u32_rnd);
    ESOS_TASK_WAIT_TICKS( u32_rnd);
  } // endof while(TRUE)
  ESOS_TASK_END();
} // end taskSemA()

ESOS_USER_TASK( taskSemB ) {
  uint32_t     u32_rnd;
  static  uint8_t     u8_cnt;

  ESOS_TASK_BEGIN();
  u8_cnt = 0;
  ESOS_TASK_WAIT_SEMAPHORE( sem_BCanRun, 1 );
  while (TRUE) {
    u8_cnt++;
    u32_rnd = 1+(0x0F & esos_GetRandomUint32());
    u32_rnd <<= 8;
    printf("taskSemB %d (%d)\n", u8_cnt, u32_rnd);
    ESOS_TASK_WAIT_TICKS( u32_rnd);
  } // endof while(TRUE)
  ESOS_TASK_END();
} // end taskSemB()

ESOS_USER_TASK( taskSemC ) {
  uint32_t    u32_rnd;
  static     uint8_t  u8_cnt;

  ESOS_TASK_BEGIN();
  u8_cnt = 0;
  ESOS_TASK_WAIT_SEMAPHORE( sem_CCanRun, 1 );
  while (TRUE) {
    u8_cnt++;
    u32_rnd = 1+(0x0F & esos_GetRandomUint32());
    u32_rnd <<= 8;
    printf("taskSemC %d (%d)\n", u8_cnt, u32_rnd);
    ESOS_TASK_WAIT_TICKS( u32_rnd);
  } // endof while(TRUE)
  ESOS_TASK_END();
} // end taskSemC()


ESOS_USER_TASK( taskMutexA ) {
  uint32_t    u32_rnd;
  static    uint8_t  u8_cnt;

  ESOS_TASK_BEGIN();
  u8_cnt = 0;
  while (TRUE) {
    ESOS_TASK_WAIT_SEMAPHORE( sem_mutex, 1);
    u8_cnt++;
    u32_rnd = 1+(0x0F & esos_GetRandomUint32());
    u32_rnd <<= 8;
    printf("taskMutexA %d (%d)\n", u8_cnt, u32_rnd);
    ESOS_SIGNAL_SEMAPHORE( sem_mutex, 1);
    ESOS_TASK_WAIT_TICKS( u32_rnd);
  } // endof while(TRUE)
  ESOS_TASK_END();
} // end taskMutexA()


ESOS_USER_TASK( taskMutexB ) {
  uint32_t    u32_rnd;
  static     uint8_t  u8_cnt;

  ESOS_TASK_BEGIN();
  u8_cnt = 0;
  while (TRUE) {
    ESOS_TASK_WAIT_SEMAPHORE( sem_mutex, 1);
    u8_cnt++;
    u32_rnd = 1+(0x0F & esos_GetRandomUint32());
    u32_rnd <<= 8;
    printf("taskMutexB %d (%d)\n", u8_cnt, u32_rnd);
    ESOS_SIGNAL_SEMAPHORE( sem_mutex, 1);
    ESOS_TASK_WAIT_TICKS( u32_rnd );
  } // endof while(TRUE)
  ESOS_TASK_END();
} // end taskMutexB()


/*
 * A beeping/blinking ESOS task
 *
 * Downers to using a task..... Can't easily
 * change the timer period.  A S/W timer is
 * a bit more flexible and more responsive.
 * But, a S/W timer can wreak much more havoc
 * than its task counterpart.
 */
ESOS_USER_TASK( task_LED ) {
  ESOS_TASK_BEGIN();
  while (TRUE) {
    LED2 = !LED2;
    ESOS_TASK_WAIT_TICKS( 1000);
    printf("\a");
    fflush(stdout);
  } // endof while(TRUE)
  ESOS_TASK_END();
} // end upper_case()

/*
 * task to access and print the global variable
 * incremented by SW/ timer.  If the task were
 * modify this variable, we'd need a semaphore.
 * As it stands, this task only reads it, so we're
 * safe.
 */
ESOS_USER_TASK( query_swTmrCnt ) {
  static uint8_t           u8_char;

  ESOS_TASK_BEGIN();
  while (TRUE) {
    ESOS_TASK_WAIT_ON_AVAILABLE_IN_COMM();
    ESOS_TASK_SIGNAL_BUSY_IN_COMM();
    ESOS_TASK_WAIT_ON_GET_UINT8( u8_char );
    ESOS_TASK_RELEASE_IN_COMM();
    if (u8_char == ' ') {
      ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
      ESOS_TASK_SIGNAL_BUSY_OUT_COMM();
      ESOS_TASK_WAIT_ON_SEND_UINT32_AS_HEX_STRING( u32_myT1Count );
      ESOS_TASK_WAIT_ON_SEND_UINT8( '\n' );
      ESOS_TASK_RELEASE_OUT_COMM();
    } //endif()
  } // endof while(TRUE)
  ESOS_TASK_END();
} // end upper_case()

/*
 * Read "in" stream and make it upper-case one
 * character at a time.
 */
ESOS_USER_TASK( upper_case ) {
  static uint8_t           u8_char;

  ESOS_TASK_BEGIN();
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
  ESOS_TASK_END();
} // end upper_case()

/*
 * Read "in" stream and make it upper-case the whole
 * string at a time.  The ESOS comm system will return
 * an in-string to us when it hits a zero-terminator or
 * a suitable return/linefeed character.
 */
ESOS_USER_TASK( upper_case2 ) {
  static uint8_t           u8_i;
  static uint8_t           au8_x[257];
  static uint8_t           au8_y[257];

  ESOS_TASK_BEGIN();
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
  ESOS_TASK_END();
} // end upper_case()

/*
 * Read "in" stream and reverses it
 */
ESOS_USER_TASK( reverse_string ) {
  static uint8_t           u8_char;
  static char            sz_in[257];
  static char            sz_out[257];

  ESOS_TASK_BEGIN();
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
  ESOS_TASK_END();
} // end upper_case()

/*
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

/**********************************************************
**
**     a few tasks to send mail messages to each other
**
************************************************************
*/
ESOS_USER_TASK( mailtask0 ) {
  uint32_t                      u32_rnd;
  static  uint8_t               u8_cnt;
  static  ESOS_TASK_HANDLE    hTask, hTask16, hTask32;

  ESOS_TASK_BEGIN();
  u8_cnt=0;
  hTask = esos_GetTaskHandle( mailtaskA );
  hTask16 = esos_GetTaskHandle( mailtaskB );
  hTask32 = esos_GetTaskHandle( mailtaskC );

  while (TRUE) {
    u32_rnd = 1+(0x0F & esos_GetRandomUint32());
    u32_rnd <<= 8;
    ESOS_TASK_WAIT_ON_TASKS_MAILBOX_HAS_AT_LEAST(hTask, 1);
    printf("T0 sending %d					%d\n", u8_cnt, __pstSelf->u16_taskID);

    __esos_CB_WriteUINT8( hTask->pst_Mailbox->pst_CBuffer, u8_cnt );
    // ESOS_TASK_WRITE_MAILBOX_BYTE(hTask, u8_cnt );

    ESOS_TASK_WAIT_ON_TASKS_MAILBOX_HAS_AT_LEAST(hTask16, sizeof(uint16_t));
    printf("T0 sending %d					%d\n", 10000+u8_cnt, __pstSelf->u16_taskID);
    __esos_CB_WriteUINT16( hTask16->pst_Mailbox->pst_CBuffer, 10000+u8_cnt );

    ESOS_TASK_WAIT_ON_TASKS_MAILBOX_HAS_AT_LEAST(hTask32, sizeof(uint32_t));
    printf("T0 sending %d					%d\n", 1000000+u8_cnt, __pstSelf->u16_taskID);
    __esos_CB_WriteUINT32( hTask32->pst_Mailbox->pst_CBuffer, 1000000+u8_cnt );

    u8_cnt++;
    if (u8_cnt>50) u8_cnt=0;
    ESOS_TASK_WAIT_TICKS( u32_rnd);
  } // endof while(TRUE)
  ESOS_TASK_END();
} // end mailtask0()


ESOS_USER_TASK( mailtask1 ) {
  uint32_t            u32_rnd;
  static uint8_t      u8_cnt;
  static ESOS_TASK_HANDLE   hTask, hTask16, hTask32;

  ESOS_TASK_BEGIN();
  hTask = esos_GetTaskHandle( mailtaskA );
  hTask16 = esos_GetTaskHandle( mailtaskB );
  hTask32 = esos_GetTaskHandle( mailtaskC );

  u8_cnt = 0;
  while (TRUE) {
    u32_rnd = 1+(0x0F & esos_GetRandomUint32());
    u32_rnd <<= 8;
    ESOS_TASK_WAIT_ON_TASKS_MAILBOX_HAS_AT_LEAST(hTask, 1);
    printf("T1 sending %d					%d\n", 100+u8_cnt, __pstSelf->u16_taskID);

    __esos_CB_WriteUINT8(hTask->pst_Mailbox->pst_CBuffer, 100+u8_cnt );
    // ESOS_TASK_WRITE_MAILBOX_BYTE(hTask, 100+u8_cnt );

    ESOS_TASK_WAIT_ON_TASKS_MAILBOX_HAS_AT_LEAST(hTask16, sizeof(uint16_t));
    printf("T1 sending %d					%d\n", 20000+u8_cnt, __pstSelf->u16_taskID);
    __esos_CB_WriteUINT16( hTask16->pst_Mailbox->pst_CBuffer, 20000+u8_cnt );

    ESOS_TASK_WAIT_ON_TASKS_MAILBOX_HAS_AT_LEAST(hTask32, sizeof(uint32_t));
    printf("T1 sending %d					%d\n", 2000000+u8_cnt, __pstSelf->u16_taskID);
    __esos_CB_WriteUINT32( hTask32->pst_Mailbox->pst_CBuffer, 2000000+u8_cnt );

    u8_cnt++;
    if (u8_cnt>50) u8_cnt=0;
    ESOS_TASK_WAIT_TICKS( u32_rnd);
  } // endof while(TRUE)
  ESOS_TASK_END();
} // end mailtask1()

ESOS_USER_TASK( mailtask2 ) {
  uint32_t    u32_rnd;
  static uint8_t      u8_cnt;
  static  ESOS_TASK_HANDLE    hTask, hTask16;


  ESOS_TASK_BEGIN();
  hTask = esos_GetTaskHandle( mailtaskA );
  hTask16 = esos_GetTaskHandle (mailtaskB );
  u8_cnt = 0;
  while (TRUE) {
    u32_rnd = 1+(0x0F & esos_GetRandomUint32());
    u32_rnd <<= 8;
    ESOS_TASK_WAIT_ON_TASKS_MAILBOX_HAS_AT_LEAST(hTask, 1);
    printf("T2 sending %d					%d\n", 200+u8_cnt, __pstSelf->u16_taskID);

    __esos_CB_WriteUINT8(hTask->pst_Mailbox->pst_CBuffer, 200+u8_cnt );
    // ESOS_TASK_WRITE_MAILBOX_BYTE(hTask, 200+u8_cnt );

    ESOS_TASK_WAIT_ON_TASKS_MAILBOX_HAS_AT_LEAST(hTask16, sizeof(uint16_t));
    printf("T2 sending %d					%d\n", 30000+u8_cnt, __pstSelf->u16_taskID);
    __esos_CB_WriteUINT16( hTask16->pst_Mailbox->pst_CBuffer, 30000+u8_cnt );

    //ESOS_TASK_WAIT_ON_TASKS_MAILBOX_HAS_AT_LEAST(esos_GetTaskHandle(mailtaskC), sizeof(uint32_t));
    //printf("T2 sending %d         %d\n", 3000000+u8_cnt, __pstSelf->u16_taskID);
    //__esos_CB_WriteUINT32( esos_GetTaskHandle(mailtaskC)->pst_Mailbox->pst_CBuffer, 3000000+u8_cnt );

    u8_cnt++;
    if (u8_cnt>50) u8_cnt=0;
    ESOS_TASK_WAIT_TICKS( u32_rnd);
  } // endof while(TRUE)
  ESOS_TASK_END();
} // end mailtask2()

//TASK that doesn't check mail very often
ESOS_USER_TASK( mailtaskA ) {
  uint32_t          u32_rnd;
  uint8_t           u8_x;
  static uint8_t    u8_cnt;

  ESOS_TASK_BEGIN();
  while (TRUE) {
    u32_rnd = 1+(0x0F & esos_GetRandomUint32());
    u32_rnd <<= 10;
    ESOS_TASK_WAIT_TICKS( u32_rnd );
    ESOS_TASK_WAIT_FOR_MAIL();
    u8_cnt=0;
    while ( ESOS_TASK_IVE_GOT_MAIL() ) {
      u8_cnt++;

      u8_x = __esos_CB_ReadUINT8( __pstSelf->pst_Mailbox->pst_CBuffer );
      //u8_x = __esos_ReadMailboxUINT8( __pstSelf->pst_Mailbox );

      if (TRUE) {
        printf("mailtaskA got mail.... %3d             %d\n", u8_x, __pstSelf->pst_Mailbox->pst_CBuffer->u16_Count);
      } else {
        printf("mailtaskA got mail.... %3d             %d\n", u8_x, u8_cnt);
      } //endif
    } //endof while()
  } // endof while(TRUE)
  ESOS_TASK_END();
} // end mailtaskA()

//TASK that does nothing but repeatedly check its mailbox
ESOS_USER_TASK( mailtaskAA ) {
  uint32_t      u32_rnd;
  uint8_t       u8_x;

  ESOS_TASK_BEGIN();
  while (TRUE) {
    ESOS_TASK_WAIT_FOR_MAIL();

    u8_x = __esos_CB_ReadUINT8( __pstSelf->pst_Mailbox->pst_CBuffer );
    // u8_x = __esos_ReadMailboxUINT8( __pstSelf->pst_Mailbox );

    printf("mailtaskAA got mail.... %d\n", u8_x);
  } // endof while(TRUE)
  ESOS_TASK_END();
} // end mailtaskAA()

//TASK that does nothing but repeatedly check its mailbox
ESOS_USER_TASK( mailtaskB ) {
  uint32_t      u32_rnd;
  uint8_t       u8_x;
  uint16_t      u16_x;

  ESOS_TASK_BEGIN();
  while (TRUE) {
    ESOS_TASK_WAIT_FOR_MAIL();

    u16_x = __esos_CB_ReadUINT16( __pstSelf->pst_Mailbox->pst_CBuffer );
    printf("mailtaskB got mail.... %d\n", u16_x);
  } // endof while(TRUE)
  ESOS_TASK_END();
} //  end mailtaskB()

//TASK that does nothing but repeatedly check its mailbox
ESOS_USER_TASK( mailtaskC ) {
  uint32_t      u32_x;
  uint8_t       u8_x;

  ESOS_TASK_BEGIN();
  while (TRUE) {
    ESOS_TASK_WAIT_FOR_MAIL();

    u32_x = __esos_CB_ReadUINT32( __pstSelf->pst_Mailbox->pst_CBuffer );
    printf("mailtaskC got mail.... %d\n", u32_x);
    ESOS_TASK_WAIT_TICKS( 1000 );

  } // endof while(TRUE)
  ESOS_TASK_END();
} // end mailtaskC()

ESOS_USER_TASK( mailtaskMSG0 ) {
  uint32_t                      u32_rnd;
  static  uint8_t               u8_cnt;
  static  ESOS_TASK_HANDLE    hTask, hTask16, hTask32;
  static  MAILMESSAGE         st_Message;

  ESOS_TASK_BEGIN();
  u8_cnt=0;
  hTask = esos_GetTaskHandle( mailtaskMSGA );

  while (TRUE) {
    u32_rnd = 1+(0x0F & esos_GetRandomUint32());
    u32_rnd <<= 8;

    ESOS_TASK_MAKE_MSG_UINT8(st_Message, u8_cnt);
    /* create a random (50-50) message that wants ACK */
    if ( (u8_cnt % 5) ==0 ) {
      st_Message.u8_flags |= ESOS_MAILMESSAGE_REQUEST_ACK;
    }
    ESOS_TASK_WAIT_ON_TASKS_MAILBOX_HAS_AT_LEAST(hTask, 9);
    if (st_Message.u8_flags & ESOS_MAILMESSAGE_REQUEST_ACK) {
      printf("T0 sending MESSAGE with ACK request %3d\n", u8_cnt);
      ESOS_TASK_SEND_MESSAGE_WAIT_DELIVERY(hTask, &st_Message);
    } else {
      printf("T0 sending MESSAGE %3d\n", u8_cnt);
      ESOS_TASK_SEND_MESSAGE(hTask, &st_Message);
      ESOS_TASK_WAIT_TICKS( u32_rnd);
    }
    u8_cnt++;
    if (u8_cnt>=100) u8_cnt=0;
  } // endof while(TRUE)
  ESOS_TASK_END();
} // end mailtaskMSG0()

ESOS_USER_TASK( mailtaskMSG1 ) {
  uint32_t                      u32_rnd;
  static  uint8_t               u8_cnt;
  static  ESOS_TASK_HANDLE    hTask, hTask16, hTask32;
  static  MAILMESSAGE         st_Message;

  ESOS_TASK_BEGIN();
  u8_cnt=100;
  hTask = esos_GetTaskHandle( mailtaskMSGA );

  while (TRUE) {
    u32_rnd = 1+(0x0F & esos_GetRandomUint32());
    u32_rnd <<= 7;

    ESOS_TASK_MAKE_MSG_UINT8(st_Message, u8_cnt);
    /* create a random (50-50) message that wants ACK */
    if ( (u8_cnt % 3) ==0 ) {
      st_Message.u8_flags |= ESOS_MAILMESSAGE_REQUEST_ACK;
    }
    ESOS_TASK_WAIT_ON_TASKS_MAILBOX_HAS_AT_LEAST(hTask, 9);
    if (st_Message.u8_flags & ESOS_MAILMESSAGE_REQUEST_ACK) {
      printf("T1 sending MESSAGE with ACK request %3d\n", u8_cnt);
      ESOS_TASK_SEND_MESSAGE_WAIT_DELIVERY(hTask, &st_Message);
    } else {
      printf("T1 sending MESSAGE %3d\n", u8_cnt );
      ESOS_TASK_SEND_MESSAGE(hTask, &st_Message);
      ESOS_TASK_WAIT_TICKS( u32_rnd);
    }
    u8_cnt++;
    if (u8_cnt>=200) u8_cnt=100;
  } // endof while(TRUE)
  ESOS_TASK_END();
} // end mailtaskMSG1()


//TASK that doesn't check mail very often
ESOS_USER_TASK( mailtaskMSGA ) {
  uint32_t                u32_rnd;
  uint8_t                 u8_x;
  static uint8_t          u8_cnt;
  static MAILMESSAGE            stMsg;
  static ESOS_TASK_HANDLE           hMSG0, hMSG1;

  ESOS_TASK_BEGIN();
  hMSG0 = esos_GetTaskHandle(mailtaskMSG0);
  hMSG1 = esos_GetTaskHandle(mailtaskMSG1);
  while (TRUE) {
    u32_rnd = 1+(0x0F & esos_GetRandomUint32());
    u32_rnd <<= 10;
    ESOS_TASK_WAIT_TICKS( u32_rnd );
    ESOS_TASK_WAIT_FOR_MAIL();
    u8_cnt=0;
    while ( ESOS_TASK_IVE_GOT_MAIL() ) {
      __esos_ReadMailMessage(__pstSelf, &stMsg );
      //PRINTF_MESSAGE( stMsg);
      printf("Got a message from ");
      if ( ESOS_IS_TASK_SENDER( hMSG0, stMsg) ) {
        printf("mailtaskMSG0");
      } else if ( ESOS_IS_TASK_SENDER(hMSG1, stMsg) ) {
        printf("mailtaskMSG1");
      } else {
        printf("UNKNOWN");
      }
      printf (" containing %3d          delivery time = %d ms\n", stMsg.au8_Contents[0], esos_GetSystemTick()-stMsg.u32_Postmark );
    } //endof while()
  } // endof while(TRUE)
  ESOS_TASK_END();
} // end mailtaskMSGA()



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

  __esos_unsafe_PutString( HELLO_MSG );

  /*
   * Now, let's get down and dirty with ESOS and our user tasks
   *
   *   Once tasks are registered, they will start executing in
   *   the ESOS scheduler.
   */

  // register our little ESOS task to mimic MCU's TIMER T1 IRQ which kicks off
  // the ESOS S/W timers when they expire
  esos_RegisterTask( __simulated_isr );

  /* ====================================================================
   * REGISTER SOME USER TASKS
   * ====================================================================
   */


// here are several combinations of tasks that should work together
#if 0
  esos_RegisterTask( upper_case );
#endif
#if 0
  tmrhnd_t1 = esos_RegisterTimer( swTimerLED, 500 );
#endif
#if 0
  esos_RegisterTask( upper_case );
  tmrhnd_t1 = esos_RegisterTimer( swTimerLED, 500 );
#endif
#if 0
  esos_RegisterTask( query_swTmrCnt  );
  tmrhnd_t1 = esos_RegisterTimer( swTimerLED, 500 );
  tmrhnd_t2 = esos_RegisterTimer( swTimerCounter, 10 );
#endif
#if 0
  esos_RegisterTask( upper_case2 );
  tmrhnd_t1 = esos_RegisterTimer( swTimerLED, 1000 );
#endif
#if 0
  esos_RegisterTask( reverse_string );
  tmrhnd_t1 = esos_RegisterTimer( swTimerLED, 1000 );
#endif
#if 0
  esos_RegisterTimer( swTimerLED, 1000 );
  esos_RegisterTask( task1 );
  esos_RegisterTask( task2 );
  esos_RegisterTask( task3 );
#endif
#if 0
  esos_RegisterTask( reverse_string );
  esos_RegisterTask( task_LED );
#endif
#if 0
  esos_RegisterTask( upper_case2 );
  esos_RegisterTask( task_LED );
  tmrhnd_t1 = esos_RegisterTimer( swTimerPrintA, 400 );
  tmrhnd_t2 = esos_RegisterTimer( swTimerPrintB, 500 );
  tmrhnd_t3 = esos_RegisterTimer( swTimerPrintC, 750 );
#endif

// The whole enchilada!  Well, maybe not a whole one, but a big one!
#if 0
  esos_RegisterTask( query_swTmrCnt  );
  esos_RegisterTimer( swTimerCounter, 10 );
  esos_RegisterTimer( swTimerLED, 1000 );
  tmrhnd_t1 = esos_RegisterTimer( swTimerPrintA, 400 );
  tmrhnd_t2 = esos_RegisterTimer( swTimerPrintB, 500 );
  tmrhnd_t3 = esos_RegisterTimer( swTimerPrintC, 750 );
  esos_RegisterTask( task1 );
  esos_RegisterTask( task2 );
  esos_RegisterTask( task3 );
#endif

// Another big enchilada
#if 0
  esos_RegisterTask( reverse_string );
  esos_RegisterTimer( swTimerLED, 1000 );
  tmrhnd_t1 = esos_RegisterTimer( swTimerPrintA, 400 );
  tmrhnd_t2 = esos_RegisterTimer( swTimerPrintB, 500 );
  tmrhnd_t3 = esos_RegisterTimer( swTimerPrintC, 750 );
  esos_RegisterTask( task1 );
  esos_RegisterTask( task2 );
  esos_RegisterTask( task3 );
#endif

#if 0
  ESOS_INIT_SEMAPHORE(sem_BCanRun, 0);
  ESOS_INIT_SEMAPHORE(sem_CCanRun, 0);
  esos_RegisterTask( taskSemA );
  esos_RegisterTask( taskSemB );
  esos_RegisterTask( taskSemC );
#endif

#if 0
  // test code based on mutex semaphore problem
  // described by Jeff Brantley
  ESOS_INIT_SEMAPHORE(sem_mutex, 1);
  esos_RegisterTask( taskMutexA );
  esos_RegisterTask( taskMutexB );
#endif

#if 0
  esos_RegisterTimer( swTimerLED, 1000 );
  esos_RegisterTask( mailtaskA );
  esos_RegisterTask( mailtaskAA );
  esos_RegisterTask( mailtaskB );
  esos_RegisterTask( mailtaskC );
  esos_RegisterTask( mailtask0 );
  esos_RegisterTask( mailtask1 );
  esos_RegisterTask( mailtask2 );
#endif

#if 1
  //esos_RegisterTimer( swTimerLED, 1000 );
  esos_RegisterTask( mailtaskMSGA );
  esos_RegisterTask( mailtaskMSG0 );
  esos_RegisterTask( mailtaskMSG1 );
#endif


} // end user_init()
