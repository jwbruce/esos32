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


// Documentation for this file. If the \file tag isn't present,
// this file won't be documented.
/** \file
 * \brief Central code for Embedded Systems Operating System 32-bit (ESOS32)
 *
 */

/*! \mainpage Embedded Systems Operating System 32-bit (ESOS32)
 *
 * \section intro_sec Introduction
 *
 * Embedded Systems Operating Systems 32-bit (ESOS32) is a continuation
 * of the line of ESOS operating systems which have include ports for
 * 8-bit and 16-bit microcontrollers, predominantly the Microchip 
 * architectures.
 * 
 * The initial port of ESOS32 is aimed at the ARM-Cortex M4 chips from
 * STMicroelectronics. The first specific target processor is the
 * STM32L452RE processor hosted on the Nucleo64 development board.
 * 
 * This tree contains the foundations for two separate hardware
 * implmentations for the STM32L452RE processor. The first is found
 * in under the *stm32l4_HAL* folders and is built upon the very
 * feature-rich [hardware abstraction library available from
 * STMicroelectronics (https://github.com/STMicroelectronics/STM32CubeL4)].  
 * The second is found
 * in under the *stm32l4_ocm3* folders and is built upon the very
 * lower-level and community developed [libOpenCM3 hardware abstraction
 * library (http://libopencm3.org)].  
 * 
 * \section install_sec Installation
 * 
 * The public repository for ESOS32 is hosted by
 * [github](http://github.com/).
 * Source code can be found at 
 * [ESOS32 public repo](https://github.com/jwbruce/esos32).
 *
 * \section using_dox Using this documentation
 *
 * A good jumping off point in this documentation is to visit
 * the "Modules" page via the link above.  This page will give the
 * reader a high-level overview of the major systems and services
 * provided by ESOS32.
 * 
 * As the reader becomes more familiar with ESOS32, then venturing
 * into the structures, macros, and source code is the next step.
 * 
 * Of course, the provided example ESOS32 applications should give
 * great insight into the structure and use of ESOS32.
 * 
 * **Enjoy!**
 */

#include    "esos.h"

//**********************************************************
// GLOBAL variables for ESOS to use/maintain
//**********************************************************

// Tasks management variables
struct stTask       __astUserTaskPool[MAX_NUM_USER_TASKS];
uint8_t               __au8UserTaskStructIndex[MAX_NUM_USER_TASKS];
struct stTask       __astChildTaskPool[MAX_NUM_CHILD_TASKS];
uint8_t               __u8UserTasksRegistered;
uint8_t               __u8ChildTasksRegistered;
uint16_t              __u16NumTasksEverCreated;

// ESOS timer managmentment variables
struct stTimer        __astTmrSvcs[MAX_NUM_TMRS];
uint8_t                 __esos_u8TmrSvcsRegistered;
uint32_t                __esos_u32TmrActiveFlags;

#ifdef ESOS_USE_BULK_CDC_USB
static struct stTask        __stUsbCommSystem;
#endif

// ESOS task mail services
MAILBOX     __astMailbox[MAX_NUM_USER_TASKS];
uint8_t       __au8_MBData[MAX_NUM_USER_TASKS][MAX_SIZE_TASK_MAILBOX];
CBUFFER     __astCircularBuffers[MAX_NUM_USER_TASKS];


// misc ESOS variables
uint32_t      __esos_u32UserFlags, __esos_u32SystemFlags;

/****************************************************************
** Embedded Systems Operating System (ESOS) code
****************************************************************/
/**
 * Adds a task to the scheduler.  Task will start executing at the
 * next opportunity. (almost immediately)
 * \param taskname name of task (argument to \ref ESOS_USER_TASK declaration
 * \retval NULLPTR   if no more tasks can execute at this time (scheduler is full)
 * \retval TaskHandle the handle of the just registered and scheduled task
 *  \sa ESOS_USER_TASK
 *  \sa esos_UnregisterTask
*/
ESOS_TASK_HANDLE    esos_RegisterTask( uint8_t (*taskname)(ESOS_TASK_HANDLE pstTask) ) {
  uint8_t     u8_i;
  uint8_t     u8_FoundFcn = FALSE;
  uint8_t     u8_IndexFcn;
  uint8_t     u8_IndexFree=0;
  uint8_t     u8_FoundFree = FALSE;

  if (__u8UserTasksRegistered < MAX_NUM_USER_TASKS) {
    /* First, we will look to see if the request task
       has already been allocated to a task from the pool.
       If so, then let's just reactivate/reset/etc the task.
    */
    for (u8_i=0; u8_i<MAX_NUM_USER_TASKS; u8_i++) {
      if (__astUserTaskPool[u8_i].pfn == taskname) {
        u8_FoundFcn = TRUE;
        u8_IndexFcn = u8_i;
        break;
      } // endof if()
      /* While we are looping through, take note of the first unused task
         we find in the pool.  We will assign this slot to the new task if
         we can't find it already allocated a (dead) slot
      */
      if ((!u8_FoundFree) && (__astUserTaskPool[u8_i].pfn == NULLPTR)) {
        u8_FoundFree = TRUE;
        u8_IndexFree = u8_i;
      } // endof if()
    } // endof for()

    /* OK. We looked at all the tasks in the pool.  We either
       * found the new task already allocated (u8_FoundFcn),
       OR
       * we did not.  In this case, there are two cases:
          # We found a dead/free task slot in our search (u8_FoundFree),
          OR
          # we did not.  (All slots are already being used.  WE ARE IN TROUBLE!)

       If we found the new task already (dead but allocated) in the pool, then we need to
         1) initialize the task, its flags, and its mailbox, and
         2) add the task to the task rotation
    */
    if (u8_FoundFcn) {
      __ESOS_INIT_TASK( &__astUserTaskPool[u8_IndexFcn]);                 // reset the task state
      __astUserTaskPool[u8_IndexFcn].flags = 0;                           // reset the task flags
      ESOS_TASK_FLUSH_TASK_MAILBOX(&__astUserTaskPool[u8_IndexFcn]);      // reset the task mailbox
      __au8UserTaskStructIndex[__u8UserTasksRegistered] = u8_IndexFcn;
      __u8UserTasksRegistered++;
      // make sure this task has a non-zero task identifier
      if ( __astUserTaskPool[u8_IndexFree].u16_taskID == 0 ) {
        // we will simply assign a sequential task ID number (sorta like unix does for PIDs)
        __u16NumTasksEverCreated++;
        __astUserTaskPool[u8_IndexFree].u16_taskID = __u16NumTasksEverCreated;
      } // endif
      return &__astUserTaskPool[u8_IndexFcn];
    } // endof if
    /* We did NOT find our task already in the pool, so allocate a new struct
       It has never been registered before, or it's location was garbage collected at some
       point.

       If we found a free task slot in the pool, then give this free slot to the new task.
    */
    if (u8_FoundFree) {
      __astUserTaskPool[u8_IndexFree].pfn = taskname;                   // attach task to the free slot
      __ESOS_INIT_TASK(&__astUserTaskPool[u8_IndexFree]);               // reset the task state
      __astUserTaskPool[u8_IndexFree].flags = 0;                        // reset the task flags
      ESOS_TASK_FLUSH_TASK_MAILBOX(&__astUserTaskPool[u8_IndexFree]);   // reset the task mailbox
      __au8UserTaskStructIndex[__u8UserTasksRegistered] = u8_IndexFree;
      __u8UserTasksRegistered++;
      // Since this is a "new" task, give it a new task ID number
      __u16NumTasksEverCreated++;
      __astUserTaskPool[u8_IndexFree].u16_taskID = __u16NumTasksEverCreated;
      return &__astUserTaskPool[u8_IndexFree];
    } // endof if
    /*  we did NOT find our function in the pool OR a free struct to use, so
        we will return a NULLPTR for now.  In the future, maybe the garbage can be called here
        to find some space by looking for killed/dead tasks.  (Current dead tasks are garbaged
        at the end of a complete pool rotation.  Possibly we can do an early garbage collection
        here, but it will only help in very rare circumstances.)
    */
    return NULLPTR;
  } else {
    return NULLPTR;
  } //end of if-else
}// end esos_RegisterTask()

/**
 * Removes the task from the scheduler
 * \param taskname name of task (argument to \ref ESOS_USER_TASK declaration
 * \retval TRUE if task was found in scheduler and removed
 * \retval FALSE  otherwise
 * \sa ESOS_USER_TASK
 * \sa esos_RegisterTask
*/
uint8_t    esos_UnregisterTask( uint8_t (*taskname)(ESOS_TASK_HANDLE pstTask) ) {
  uint8_t                 u8Status=FALSE;
  uint8_t                 u8_i, u8_z;
  ESOS_TASK_HANDLE      pstNowTask;

  /* Search through the pool and find out where the task needing unregistering
     is residing in the pool.  Then, we will mark this slot as needing removal
     and setting a flag for task pool repacking at the end of the current
     rotation through the pool.
  */
  for (u8_i=0; u8_i<__u8UserTasksRegistered; u8_i++) {
    // get next index from array so we can get the task handle
    u8_z = __au8UserTaskStructIndex[u8_i];
    /* check tasks that have been allocated (not a NULLIDX) and
       not been garbage collected (not REMOVE_IDX) yet.  If our
       task is among them, the mark it to be garbaged collected
       (REMOVE_IDX) at the next opportunity
    */
    if ((u8_z != NULLIDX) & (u8_z != REMOVE_IDX)) {
      pstNowTask = &__astUserTaskPool[u8_z];
      // If we find our task, mark it and signal ESOS to repack task pool
      if (pstNowTask->pfn == taskname) {
        __au8UserTaskStructIndex[u8_i] = REMOVE_IDX;
        __esos_SetSystemFlag( __ESOS_SYS_FLAG_PACK_TASKS );
        u8Status=TRUE;
        break;
      } // end if (pfn == taskname)
    } // end if (!NULLIDX)
  } // end for
  return  u8Status;
}// end esos_UnregisterTask()

/**
 * Find the (active) task handle for a given task function
 * \param taskname name of task (argument to \ref ESOS_USER_TASK declaration
 * \retval NULLPTR   if task is not found among the active tasks
 * \retval TaskHandle the handle to the task function requested
 *  \sa ESOS_USER_TASK
 *  \sa esos_RegisterTask
 *  \sa esos_UnregisterTask
*/
ESOS_TASK_HANDLE    esos_GetTaskHandle( uint8_t (*taskname)(ESOS_TASK_HANDLE pstTask) ) {
  uint8_t                 u8_i, u8_z;
  ESOS_TASK_HANDLE      pst_NowTask;
  ESOS_TASK_HANDLE      pst_ReturnTask = (ESOS_TASK_HANDLE) NULLPTR;

  /* Scan through the pool of "registered" tasks and see
     if we can find the task function name requested
  */
  for (u8_i=0; u8_i<__u8UserTasksRegistered; u8_i++) {
    // get next index from array so we can get the task handle
    u8_z = __au8UserTaskStructIndex[u8_i];
    /* check tasks that have been allocated (not a NULLIDX) and
       not been garbage collected (not REMOVE_IDX) yet.  If our
       task is among them, then return the handle to the caller
    */
    if ((u8_z != NULLIDX) & (u8_z != REMOVE_IDX)) {
      pst_NowTask = &__astUserTaskPool[u8_z];
      // If we find our task, save the pstXXX so we can return it
      if (pst_NowTask->pfn == taskname) {
        pst_ReturnTask = pst_NowTask;
        break;
      } // end if (pfn == taskname)
    } // end if (!NULLIDX)
  } //end for
  return pst_ReturnTask;
} //end esos_GetTaskHandle()

/**
 * Find the (active) task handle for a given task function
 * \param u16_TaskID name of task (argument to \ref ESOS_USER_TASK declaration)
 * \retval NULLPTR   if task is not found among the active tasks
 * \retval TaskHandle the handle to the task function requested
 *  \sa ESOS_USER_TASK
 *  \sa esos_RegisterTask
 *  \sa esos_UnregisterTask
*/
ESOS_TASK_HANDLE    esos_GetTaskHandleFromID( uint16_t u16_TaskID ) {
  uint8_t             u8_i, u8_z;
  ESOS_TASK_HANDLE      pst_NowTask;
  ESOS_TASK_HANDLE      pst_ReturnTask = (ESOS_TASK_HANDLE) NULLPTR;

  /* Scan through the pool of "registered" tasks and see
     if we can find the task function name requested
  */
  for (u8_i=0; u8_i<__u8UserTasksRegistered; u8_i++) {
    // get next index from array so we can get the task handle
    u8_z = __au8UserTaskStructIndex[u8_i];
    /* check tasks that have been allocated (not a NULLIDX) and
       not been garbage collected (not REMOVE_IDX) yet.  If our
       task is among them, then return the handle to the caller
    */
    if ((u8_z != NULLIDX) & (u8_z != REMOVE_IDX)) {
      pst_NowTask = &__astUserTaskPool[u8_z];
      // If we find our task, save the pstXXX so we can return it
      if (pst_NowTask->u16_taskID == u16_TaskID) {
        pst_ReturnTask = pst_NowTask;
        break;
      } // end if (pfn == taskname)
    } // end if (!NULLIDX)
  } //end for
  return pst_ReturnTask;
} //end esos_GetTaskHandleFromID()



// TODO: I DONT THINK I NEED TO STORE THE ACTUAL PFNs SINCE THE PARENT'S
// WAIT FUNCTION WILL DECOMPOSE INTO A CALL TO THE CHILD TASK AUTOMATICALLY.
//
// INVESTIGATE MORE!
//
// TODO:  make sure childs get restarted if they yield and some other task
//        executes in the meantime!

/**
* Searches child task pool to find a free child task structure and returns
*    a handle (pst) back to the caller
* \retval TaskHandle if a child task structure is available
* \retval ESOS_BAD_CHILD_TASK_HANDLE  if no structures are available at this time
*/
ESOS_TASK_HANDLE  esos_GetFreeChildTaskStruct() {
  uint16_t    u16_i = 0;

  while (u16_i < MAX_NUM_CHILD_TASKS) {
    if (ESOS_IS_TASK_INITED( &__astChildTaskPool[u16_i]) )
      return &__astChildTaskPool[u16_i];
    u16_i++;
  }
  return NULLPTR;
}// end esos_u16GetFreeChildTaskStruct()

/********************************************************************************/

/**
* Returns the number of tasks we can execute
* \retval N the number of tasks this version of ESOS can execute
*/
uint8_t   esos_GetMaxNumberTasks(void) {
  return MAX_NUM_USER_TASKS;
} // end osGetMaxNumberTasks()

/*
* Determine whether a period of time has elapsed.  Users
* have no need to call this function.  It is used by ESOS
* internally.
* \param u32_startTick system tick count when timer was created
* \param u32_period duration of period in system ticks
* \retval TRUE if the period of time has elapsed
* \retval FALSE if the period of time has not yet elapsed
*/
uint16_t  __esos_hasTickDurationPassed(uint32_t u32_startTick, uint32_t u32_period) {
  uint32_t    u32_delta, u32_current;

  u32_current = esos_GetSystemTick();
  u32_delta = u32_current - u32_startTick;
  if (u32_current < u32_startTick)
    u32_delta += 0xFFFFFFFF;    // account for rollover (DELTA=0xFFFFFFF-start+current)
  if (u32_delta > u32_period)
    return TRUE;
  else
    return FALSE;
} // end __esos_hasSystemTickDurationPassed()

/*
* ESOS timer services callback function.  HW-specific code
* that creates the system tick must call this function at
* every ESOS system tick.
*/
void __esos_tmrSvcsExecute(void) {
  uint8_t     u8_cnt, u8_index;

  u8_cnt = __esos_u8TmrSvcsRegistered;
  u8_index = 0;
  while (u8_cnt) {
    // if timer is running, update its structure and call it if necessary
    if (esos_IsTimerRunning(u8_index)) {
      __astTmrSvcs[u8_index].u32_cntDown--;
      if (__astTmrSvcs[u8_index].u32_cntDown == 0 ) {
        __astTmrSvcs[u8_index].u32_cntDown = __astTmrSvcs[u8_index].u32_period;
        __astTmrSvcs[u8_index].pfn();
      } //endif timer has expired
      u8_cnt--;                         // denote we've serviced one of the active timers
    } // endif IsTimerRunning
    u8_index++;       // move index to next timer in array
  } // end while(u8_cnt)
} //end __esos_tmrSvcsExecute()

/**
 * Adds a timer to the ESOS timer service.  Timer function will execute at its
 * next opportunity.  Timer functions must have \em void arguments and \em void
 * returns.
 * \param timername name under which timer was declared in \ref ESOS_USER_TIMER.
 * and contains the code to run when software timer expires
 * \param u32_period period of timer in system ticks (currently, milliseconds)
 * \retval ESOS_TMR_FAILURE   if no more timers can added at this time
 * \retval timerhandle   if timer service was registered
 * \sa ESOS_USER_TIMER
 * \sa esos_UnregisterTimer
 * \sa esos_GetTimerHandle
 * \sa esos_ChangeTimerPeriod
 * \sa esos_IsTimerRunning
 * \sa
 *
 */
ESOS_TMR_HANDLE    esos_RegisterTimer( void (*timername)(void), uint32_t u32_period ) {
  uint8_t   u8_i;

  if ( esos_GetNumberRunningTimers() < MAX_NUM_TMRS) {
    for (u8_i=0; u8_i<MAX_NUM_TMRS; u8_i++ ) {
      if (!esos_IsTimerRunning(u8_i)) {
        __astTmrSvcs[u8_i].pfn = timername;
        __astTmrSvcs[u8_i].u32_period = u32_period;
        __astTmrSvcs[u8_i].u32_cntDown = u32_period;
        __esos_u8TmrSvcsRegistered++;
        __esos_MarkTimerRunning( u8_i );
        return u8_i;
      } // endif IsTimerRunning
    } // endfor
    return ESOS_TMR_FAILURE;
  } // endif
  else
    return ESOS_TMR_FAILURE;
} // end esos_RegisterTimer

/**
 * Removes a timer from the ESOS timer service.
 * \param hnd_timer handle to timer to remove
 * \retval FALSE  if timer wasn't active in the first place
 * \retval TRUE   if timer was stopped and removed
 * \sa esos_RegisterTimer
 * \sa esos_GetTimerHandle
 * \sa esos_ChangeTimerPeriod
 */
uint8_t    esos_UnregisterTimer( ESOS_TMR_HANDLE hnd_timer ) {

  if ( esos_IsTimerRunning(hnd_timer) ) {
    __astTmrSvcs[hnd_timer].pfn = NULLPTR;
    __esos_u8TmrSvcsRegistered--;
    __esos_MarkTimerStopped(hnd_timer);
    return TRUE;
  } else
    return FALSE;
} //end esos_UnregisterTimer()

/**
 * Finds the timer handle to the provided and ACTIVE timer function
 * \param pfnTmrFcn pointer to timer function (will execute each time timer expires)
 * \retval ESOS_TMR_FAILURE    could not find the function in the active timer list
 * \retval timerHandle       handle to timer
 * \sa esos_RegisterTimer
 * \sa esos_UnregisterTimer
 * \sa esos_ChangeTimerPeriod
 * \sa esos_IsTimerRunning
 */
ESOS_TMR_HANDLE    esos_GetTimerHandle( void (*pfnTmrFcn)(void) ) {
  uint8_t   u8_i=0;
  uint8_t   u8_cnt;

  u8_cnt = esos_GetNumberRunningTimers();
  while (u8_cnt) {
    if (esos_IsTimerRunning(u8_i) ) {
      if ( __astTmrSvcs[u8_i].pfn == pfnTmrFcn ) return u8_i;
      u8_cnt--;
    } //endif
    u8_i++;
  } // endwhile
  return ESOS_TMR_FAILURE;
} //end esos_GetTimerHandle()


/**
 * Change a timer period.
 * \param hnd_timer handle to timer whose period is to be changed
 * \param u32_period new period for timer selected by mask
 * \retval FALSE if timer is not currently running
 * \retval TRUE if timer period was changed
 * \sa esos_RegisterTimer
 * \sa esos_UnregisterTimer
 * \sa esos_GetTimerHandle
 * \sa esos_IsTimerRunning
 */
uint8_t    esos_ChangeTimerPeriod( ESOS_TMR_HANDLE hnd_timer, uint32_t u32_period ) {

  if (esos_IsTimerRunning(hnd_timer) ) {
    __astTmrSvcs[hnd_timer].u32_period = u32_period;
    return TRUE;
  } else return FALSE;
} //end esos_geTimerHandle()

void __esosInit(void) {
  uint8_t     u8_i;

  // initialize the pool of available user tasks
  for (u8_i=0; u8_i<MAX_NUM_USER_TASKS; u8_i++) {
    __astUserTaskPool[u8_i].pfn = NULLPTR;
    __au8UserTaskStructIndex[u8_i] = NULLIDX;
    __astChildTaskPool[u8_i].pfn = NULLPTR;
    // assign each possible user task a mailbox and initialize it
    __astUserTaskPool[u8_i].pst_Mailbox = &__astMailbox[u8_i];
    (__astUserTaskPool[u8_i].pst_Mailbox)->pst_CBuffer = &__astCircularBuffers[u8_i];
    __esos_InitMailbox(__astUserTaskPool[u8_i].pst_Mailbox, &__au8_MBData[u8_i][0]);
  }
  __esos_u32TmrActiveFlags = 0;
  for (u8_i=0; u8_i<MAX_NUM_TMRS; u8_i++) {
    __astTmrSvcs[u8_i].pfn = NULLPTR;
  }

  // no user tasks are currently registered
  __u8UserTasksRegistered = 0;
  // no child tasks are active
  __u8ChildTasksRegistered = 0;
  // no timer services are active
  __esos_u8TmrSvcsRegistered = 0;

  // initialize the ESOS pseudo-random number generator
  // see value, in case the hardware-functions don't..
  // Also, some internal ESOS functions may use the SW PRNG
  // in ESOS via the hidden function, so let's make sure it 
  // is seed.  We will use MAX_NUM_USER_TASKS for now.
  __esos_set_PRNG_U32Seed( MAX_NUM_USER_TASKS );
  // call the config routine for the hardware PRNG.
  __esos_hw_config_PRNG();

  /* Call the user provided function to initialize the
   *    and start the ESOS system tick..
   */
  __esos_hw_InitSystemTick();

  // initialize the HW interrupts
  //   (This routine is considered HW specific, because
  //      we don't know the number of HW interrupts on the
  //      CPU at this point....)
#ifdef  ESOS_USE_IRQS
  _esos_hw_InitUserInterrupts();
#endif
  /*
   * Now, initialize one of the communication systems if
   * the user has requested it in user_config.h
   *
   * TODO: At some point, I want to be able to run both comm
   * systems independently so we can use USB and debug via
   * RS232 or vice-versa.  I'll worry about that later.
   */
#ifdef  ESOS_USE_BULK_CDC_USB
  __esos_InitCommSystem();
#endif
#ifdef ESOS_USE_SERIAL_PORT
  __esos_InitCommSystem();
#endif

#ifdef ESOS_USE_LCD
  // Initialize LCD services
  __esos_lcd44780_init();
#endif

  
  user_init();

#ifdef ESOS_USE_SUI
  // must be called **AFTER** user_init() because we expect the user
  // to "register" their SUI elements before we try to initialize the
  // devices
  __esos_InitSUI();
#endif

#if defined (ESOS_USE_I2C_100KBPS) || defined (ESOS_USE_I2C) || defined (ESOS_USE_I2C_400KBPS)
  // Cal **AFTER** user_init() because the user might have set
  // pin directions and peripheral functions in their user_init()
  // ESOS will call the specific __esos_i2c_hw_config( u32_i2cbps)
  //  as well.
#ifdef ESOS_USE_I2C_400KBPS
  #warning Using 400kbps I2C service.... Can your I2C slaves handle it?
  __esos_i2c_config(400000UL);
#else
  __esos_i2c_config(100000UL);
#endif
#endif

// Call **AFTER** user_init() because the user might have set
// pin directions and peripheral functions in their user_init()
// ESOS will call the specific __esos_spi_hw_config( u32_spibps)
//  as well.
#ifdef ESOS_USE_SPI
  __esos_spi_config(1000000UL);
#endif

#ifdef ESOS_USE_WATCHDOG
  // must be called at the very end so that watchdog doesnt reset
  // us before the user gets around to feeding the watchdog
  // We will use a 1000 ms = 1 second watchdog period for now....
  _esos_wdog_initWatchdog( 1000 );
#endif

} // end osInit()

main_t main(void) {
  uint8_t             u8TaskReturnedVal=0;
  uint8_t             u8i,u8j, u8NumRegdTasksTemp;
  ESOS_TASK_HANDLE  pstNowTask;

  __esosInit();
  /* Keep a running counter of number of tasks we've created
  ** to serve as stupid/simple task identifier
  */
  __u16NumTasksEverCreated = 0;
  while (TRUE) {
    /* First, let ESOS get something done.....
     *      service communications, garbage collection, etc.
     *      Whatever a nice little OS needs to do.
     */

    u8i = 0;
    /* get the number of currently registered tasks.... we must make
     * a local copy, because the tasks themselves may unregister and
     * change the variable __u8UserTasksRegistered as they go!
     */
    u8NumRegdTasksTemp = __u8UserTasksRegistered;

    // if there are registered tasks, let them run (call them)
    while ( u8i < u8NumRegdTasksTemp  ) {
      /* Get the next task up for execution.  Call it and catch
         its state (returned value) when it gives focus back.
         We may need to do something depending on its new state,
         e.g. if it has ended, we need to remove it from the rotation
      */
      pstNowTask = &__astUserTaskPool[__au8UserTaskStructIndex[u8i]];
      u8TaskReturnedVal = pstNowTask->pfn( pstNowTask );
      if (u8TaskReturnedVal == ESOS_TASK_ENDED) {
        //printf ("Unregistering an ENDED protothread\n");
        esos_UnregisterTask( pstNowTask->pfn );
      } // endif
      u8i++;
      OS_ITERATE;
    } //end while()

    /* we have completed a rotation through the set of active tasks
       Now repack the pool (if necessary) to keep everything nice and
       tight.
    */
    if (__esos_IsSystemFlagSet( __ESOS_SYS_FLAG_PACK_TASKS) ) {
      /* Now, loop over the UserTasks and pack them into
         the beginning of our arrays.  We loop through the list
         backwards.....

          Loop over the original number of registered tasks and
          look for NULLPTRs.  If you find one, scoot all the following
          tasks down by one and make the last one a NULLPTR.  Reduce
          our count of tasks by one to make the next search shorter.

          NOTE: we can't "break" the for-loop search because there
                may be more than one unregistered tasks in the pool
      */
      u8i = __u8UserTasksRegistered;
      // TODO:  CHANGE THIS TO A   do{}while(u8i)
      while(TRUE) {
        pstNowTask = &__astUserTaskPool[u8i];
        if (__au8UserTaskStructIndex[u8i] == REMOVE_IDX) {
          for ( u8j=u8i+1; u8j<u8NumRegdTasksTemp; u8j++) {
            __au8UserTaskStructIndex[u8j-1] = __au8UserTaskStructIndex[u8j];
          } // end for
          __au8UserTaskStructIndex[u8NumRegdTasksTemp-1] = NULLIDX;
          u8NumRegdTasksTemp--;
        } // end if
        /* Have we hit the beginning of the task pool yet?
           If not, decrement (do another).
           If so, we are done (break)
        */
        if (u8i)
          u8i--;
        else
          break;
      } // end while
      /* We have repacked the task pool (possibly multiple times), so update
         the new number of registered tasks and clear the task PACK flag
      */
      __u8UserTasksRegistered=u8NumRegdTasksTemp;   // set record the new number of registered tasks
      __esos_ClearSystemFlag( __ESOS_SYS_FLAG_PACK_TASKS );
    } // end if
  } //end while

  OS_END;

} //end main()
