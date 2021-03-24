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


/*
 *  This file defines the macros and structures needed to implement the
 *  ESOS cooperative mulitasking.  It relies heavily on the PROTOTHREADS
 *  library created by Adam Dunkels at SICS.  The SICS notices are
 *  found at the EOF.
 */

/**
 * \addtogroup ESOS_Tasks
 * @{
 */

/** \file
 *  This file contains macros for creating and managing
 *  ESOS tasks and semaphores.  User need not include
 *  this file, but rather should include  \ref esos.h
 *  instead.
 */

#ifndef __ESOS_TASK_H__
#define __ESOS_TASK_H__

#include "lc.h"
#include "esos_mail.h"

struct stTask {
  lc_t                  lc;
  uint8_t                 flags;
  uint8_t                               (*pfn) (struct stTask *pst_Task);
  uint32_t                u32_savedTick;
  uint32_t                u32_waitLen;
  uint16_t                u16_taskID;
  MAILBOX*              pst_Mailbox;
};

/** \struct ESOS_TASK_HANDLE
 * Handle to an ESOS user task.  This handle is used whenever a
 * task needs to refer to another task, or whenever a task needs
 * to retreive its own handle.
 *
 * \sa ESOS_TASK_GET_TASK_HANDLE
 * \sa ESOS_ALLOCATE_CHILD_TASK
 *
 */
typedef   struct stTask*                   ESOS_TASK_HANDLE;

/******************************
** create a typedef to represent pointers to ESOS
** user task functions
*******************************/
// typedef   uint8_t (*pfn_ESOS_USER_TASK)(struct stTask *pst_Task);
//#define   __MAKE_UINT16_TASK_ID(pfn)            esos_taskname_hash_u16( (pfn), sizeof((pfn)) )
//#define   ESOS_DOES_TASK_HAVE_ID(pfn,taskID)        ((taskID)==__MAKE_UINT16_TASK_ID((pfn)))

/* Task "return" values.  Used by scheduler to determine what to do with task **/
#define ESOS_TASK_WAITING 0
#define ESOS_TASK_ENDED  3


/* Task flag : task is waiting on ACK from other task reading mail */
#define __TASK_MAILNACK_MASK     ESOS_BIT5
/* Task flag : task has mail needing to be read */
#define __TASK_HASMAIL_MASK      ESOS_BIT4

/* Task flag : task has ended (hit ESOS_TASK_END) */
#define __TASK_ENDED_MASK        ESOS_BIT3
/* Task flag : task has been flagged to be killed (will exit on next block/wait/yield) */
#define __TASK_KILLED_MASK       ESOS_BIT2
/* Task flag : task is sleeping */
#define __TASK_SLEEPING_MASK     ESOS_BIT1
/* Task flag : task is waiting */
#define __TASK_WAITING_MASK      ESOS_BIT0
/* Task flag : task has been called by scheduler  */
#define __TASK_CALLED_MASK       ESOS_BIT7

/* some helper macros to manage Task states **/
#define   __ESOS_SET_TASK_SLEEPING_FLAG(TaskHandle)    BIT_SET_MASK((TaskHandle)->flags, __TASK_SLEEPING_MASK)
#define   __ESOS_CLEAR_TASK_SLEEPING_FLAG(TaskHandle)  BIT_CLEAR_MASK((TaskHandle)->flags, __TASK_SLEEPING_MASK)
#define   __ESOS_SET_TASK_KILLED_FLAG(TaskHandle)      BIT_SET_MASK((TaskHandle)->flags, __TASK_KILLED_MASK)
#define   __ESOS_CLEAR_TASK_KILLED_FLAG(TaskHandle)    BIT_CLEAR_MASK((TaskHandle)->flags, __TASK_KILLED_MASK)
#define   __ESOS_SET_TASK_WAITING_FLAG(TaskHandle)     BIT_SET_MASK((TaskHandle)->flags, __TASK_WAITING_MASK)
#define   __ESOS_CLEAR_TASK_WAITING_FLAG(TaskHandle)   BIT_CLEAR_MASK((TaskHandle)->flags, __TASK_WAITING_MASK)
#define   __ESOS_SET_TASK_CALLED_FLAG(TaskHandle)      BIT_SET_MASK((TaskHandle)->flags, __TASK_CALLED_MASK)
#define   __ESOS_CLEAR_TASK_CALLED_FLAG(TaskHandle)    BIT_CLEAR_MASK((TaskHandle)->flags, __TASK_CALLED_MASK)
#define   __ESOS_IS_TASK_CALLED(TaskHandle)            IS_BIT_SET_MASK((TaskHandle)->flags, __TASK_CALLED_MASK)
#define   __ESOS_SET_TASK_ENDED_FLAG(TaskHandle)       BIT_SET_MASK((TaskHandle)->flags, __TASK_ENDED_MASK)
#define   __ESOS_CLEAR_TASK_ENDED_FLAG(TaskHandle)     BIT_CLEAR_MASK((TaskHandle)->flags, __TASK_ENDED_MASK)

/* mailbox task flags */
#define   __ESOS_SET_TASK_HASMAIL_FLAG(TaskHandle)     BIT_SET_MASK((TaskHandle)->flags, __TASK_HASMAIL_MASK)
#define   __ESOS_CLEAR_TASK_HASMAIL_FLAG(TaskHandle)   BIT_CLEAR_MASK((TaskHandle)->flags, __TASK_HASMAIL_MASK)
#define   __ESOS_SET_TASK_MAILNACK_FLAG(TaskHandle)    BIT_SET_MASK((TaskHandle)->flags, __TASK_MAILNACK_MASK)
#define   __ESOS_CLEAR_TASK_MAILNACK_FLAG(TaskHandle)  BIT_CLEAR_MASK((TaskHandle)->flags, __TASK_MAILNACK_MASK)


/**
 * \name Initialization
 * @{
 */

/**
 * Initialize an ESOS task.
 *
 * Initializes an ESOS task. Initialization must be done prior to
 * starting to execute the task.
 *
 * \note Called by ESOS internally.  Users have no real reason to call this function
 * themselves.
 * \param TaskHandle The \ref ESOS_TASK_HANDLE of the task to be initialized
 * \sa ESOS_TASK_SPAWN_AND_WAIT
 * \sa ESOS_TASK_GET_TASK_HANDLE
 * \hideinitializer
 */
#define __ESOS_INIT_TASK(TaskHandle)                 \
    LC_INIT((TaskHandle)->lc);

/** @} */

/**
 * \name Querying the state of a task
 * @{
 */

/**
 * Is ESOS task structure initialized?.
 *
 * Checks to see of the ESOS task structure is initialized. Initialization must be done prior to
 * starting to execute the task.  In reality, this checks to see if a task structure is available
 * since a 'running' task structure will appear to be uninitialized since its state will not be
 * NULL, ZERO, or whatever our implementation of uses as the initial state.
 *
 * \param TaskHandle The \ref ESOS_TASK_HANDLE of the task being queried
 *
 * \sa __ESOS_INIT_TASK
 *
 * \hideinitializer
 */
#define ESOS_IS_TASK_INITED(TaskHandle)   LC_IS_INITED((TaskHandle)->lc)

/**
 * Determines if a task is currently sleeping
 *
 * \param TaskHandle The \ref ESOS_TASK_HANDLE of the task being queried
 * \sa ESOS_TASK_GET_TASK_HANDLE
 * \hideinitializer
 */
#define   ESOS_IS_TASK_SLEEPING(TaskHandle)          IS_BIT_SET_MASK((TaskHandle)->flags, __TASK_SLEEPING_MASK)

/**
 * Determines if a task is slated to be killed at its next execution.
 *
 * \param TaskHandle The \ref ESOS_TASK_HANDLE of the task being queried
 * \sa ESOS_TASK_GET_TASK_HANDLE
 * \hideinitializer
 */
#define   ESOS_IS_TASK_KILLED(TaskHandle)            IS_BIT_SET_MASK((TaskHandle)->flags, __TASK_KILLED_MASK)

/**
 * Determines if a task is waiting to run/blocked by some condition.
 *
 * \param TaskHandle The \ref ESOS_TASK_HANDLE of the task being queried
 * \sa ESOS_TASK_GET_TASK_HANDLE
 * \hideinitializer
 */
#define   ESOS_IS_TASK_WAITING(TaskHandle)           IS_BIT_SET_MASK((TaskHandle)->flags, __TASK_WAITING_MASK)

/**
 * Determines if a task is inactive/ended.    Tasks in this state
 * are subject to culling by the ESOS scheduler at some point in
 * future.
 *
 * \param TaskHandle The \ref ESOS_TASK_HANDLE of the task being queried
 * \sa ESOS_TASK_GET_TASK_HANDLE
 * \hideinitializer
 */
#define   ESOS_IS_TASK_ENDED(TaskHandle)             IS_BIT_SET_MASK((TaskHandle)->flags, __TASK_ENDED_MASK)

/** @} */

/**
 * \name Declaration and definition
 * @{
 */

/**
 * Declaration of an ESOS task.
 *
 * This macro is used to declare an ESOS task. All ESOS tasks must
 * be declared with this macro.
 *
 * \note ESOS tasks have no arguments passed in and cannot return values.
 * \param taskname The name by which you wish for the user task to be known.
 * In reality, this name is the name of the C function implementing the ESOS
 * task.  Therefore, your task names must be unique and adhere to C language
 * naming restrictions.
 *
 * \hideinitializer
 */
#define ESOS_USER_TASK(taskname)    uint8_t taskname(ESOS_TASK_HANDLE __pstSelf)

/**
 * Declaration of an ESOS child task -- a task spawned by another
 * ESOS task (a.k.a. the parent task)
 *
 * This macro is used to declare an ESOS child task. All ESOS child
 * tasks must be declared with this macro.  This macro relies on the
 * compiler's ability to handle variadic arguments.  GCC does this
 * just fine.  Other compilers may not work.
 *
 *  \note ESOS child tasks can have zero or more input arguments passed in.
 * However, ESOS child tasks <em>cannot</em> return values.
 * \param taskname The name by which you wish for the child task to be known.  In
 * reality, this name is the name of a C function implementing the ESOS child task.
 * \param ... (OPTIONAL) Any arguments to pass to the the child task
 *
 * \hideinitializer
 */
#define ESOS_CHILD_TASK(taskname, ...)    uint8_t taskname(ESOS_TASK_HANDLE __pstSelf, ##__VA_ARGS__)

/**
 * Declare the start of an ESOS task inside the C function
 * implementing the ESOS task.
 *
 * This macro is used to declare the starting point of a
 * ESOS task. It should be placed at the start of the function in
 * which the ESOS task runs. All C statements above the ESOS_TASK_BEGIN()
 * invokation will be executed each time the ESOS task is scheduled.
 * \sa ESOS_TASK_END
 * \hideinitializer
 */
#define ESOS_TASK_BEGIN()                 \
      { __ESOS_SET_TASK_CALLED_FLAG(__pstSelf);    \
      LC_RESUME(__pstSelf->lc)

/**
 * Declare the end of an ESOS task.
 *
 * This macro is used for declaring that an ESOS task ends. It must
 * always be used together with a matching ESOS_TASK_BEGIN() macro.
 * \sa ESOS_TASK_BEGIN
 * \hideinitializer
 */
#define ESOS_TASK_END()          \
    LC_END(__pstSelf->lc);              \
    __pstSelf->flags = __TASK_ENDED_MASK;      \
    __ESOS_INIT_TASK(__pstSelf);          \
    return ESOS_TASK_ENDED; }


/**
 * Retrieve the task handle for the current task.
 *
 * This macro gets the task handle for the current task. Useful
 * if the current task wishes to give its handle to some other task
 * so that the other task can manipulate the current task externally.
 *
 * \sa ESOS_TASK_WAKE
 * \sa ESOS_TASK_KILL
 * \hideinitializer
 */
#define ESOS_TASK_GET_TASK_HANDLE()     __pstSelf

/** @} */

/**
 * \name Calling an ESOS task
 * @{
 */

/**
 * Schedule an ESOS task.
 *
 * This function schedules an ESOS task. The return value of the
 * function is non-zero if the ESOS task is running or zero if the
 * ESOS task has exited.
 *
 * \note Typically, this macro is only called by other ESOS task macros and
 * the ESOS task scheduler. I can't think of any reasonable reason why an
 * user would need to call this macro!
 * \param pfnThread The call to the C function implementing the ESOS task to
 * be scheduled
 *
 * \hideinitializer
 */
#define ESOS_SCHEDULE_TASK(pfnThread) ( (pfnThread) < ESOS_TASK_ENDED )

/** @} */


/**
 * \name Blocked waits
 * @{
 */

/**
 * Block and wait until condition is true.
 *
 * This macro blocks the ESOS task until the specified condition is
 * true.
 *
 * \param condition The condition.
 *
 * \sa ESOS_TASK_WAIT_WHILE
 *
 * \hideinitializer
 */
#define ESOS_TASK_WAIT_UNTIL(condition)             \
  do {                       \
    LC_SET(__pstSelf->lc);                 \
    if(ESOS_IS_TASK_KILLED(__pstSelf)) {             \
      __pstSelf->flags = __TASK_KILLED_MASK;          \
      return ESOS_TASK_ENDED;                                \
    }                                                         \
    if((condition)) {                                         \
      __ESOS_CLEAR_TASK_WAITING_FLAG(__pstSelf);                \
    }                                                         \
    else {                                                    \
      __ESOS_SET_TASK_WAITING_FLAG(__pstSelf);                  \
    }                                                         \
    if(ESOS_IS_TASK_WAITING(__pstSelf)) {                     \
      return ESOS_TASK_WAITING;                               \
    }                                                         \
  } while(0)

/**
 * Block and wait while condition is true.
 *
 * This function blocks and waits while the specified condition is true.
 * \param cond The condition.
 *
 * \sa ESOS_TASK_WAIT_UNTIL
 *
 * \hideinitializer
 */
#define ESOS_TASK_WAIT_WHILE(cond)         ESOS_TASK_WAIT_UNTIL(!(cond))

/**
 * Block and wait for a period of time/ticks
 *
 * This function blocks and waits for the duration of time requested.
 *
 * \param u32_duration Number of system ticks (currently milliseconds)
 * to block
 *
 * \hideinitializer
 */
#define ESOS_TASK_WAIT_TICKS(u32_duration)        \
do {                                                    \
   __pstSelf->u32_savedTick = esos_GetSystemTick();     \
   __pstSelf->u32_waitLen = (u32_duration);             \
   ESOS_TASK_WAIT_UNTIL(__esos_hasTickDurationPassed(__pstSelf->u32_savedTick, __pstSelf->u32_waitLen) ); \
} while(0);

/** @} */

/* helper function to spawn child tasks */
#define __ESOS_TASK_SPAWN(pstChild, fcnCallWithArgs)    \
  do {            \
    __ESOS_INIT_TASK((pstChild));       \
    ESOS_TASK_WAIT_THREAD((fcnCallWithArgs));   \
  } while(0)

/**
 * \name Tasks and child tasks
 * @{
 */

/**
 * Block and wait until a child ESOS task completes.
 *
 * This macro schedules a child ESOS task. The current ESOS task
 * will block until the child ESOS task completes.
 *
 * \todo I THINK THIS SHOULD BE REWRITTEN TO USE THE ESOS TASK STATE
 * VARIABLES INSTEAD OF A CALL TO ESOS_SCHEDULE_TASK.  DOESN'T WORK
 * FOR BOTH PARENT AND CHILD TASKS AS WRITTEN!
 *
 * \note The child ESOS task must be manually initialized with the
 * \ref __ESOS_INIT_TASK function before this function is used.
 * \note Child task should have been defined with \ref ESOS_CHILD_TASK
 *
 * \param pfnChild Pointer to the child task function
 * \param ... Arguments to the child task (if they exist)
 *
 * \sa ESOS_TASK_SPAWN_AND_WAIT
 * \sa ESOS_CHILD_TASK
 *
 * \hideinitializer
 */
#define ESOS_TASK_WAIT_THREAD(pfnChild, ...)    ESOS_TASK_WAIT_WHILE(ESOS_SCHEDULE_TASK(pfnChild,##__VA_ARGS__ ))

/**
 * This macro initializes an ESOS child task structure, calls the child task and blocks
 * the parent task until the child exits exits. The macro can only be used within an ESOS task.
 *
 * \param pstChild Pointer to the child ESOS task's control structure.
 * \param pfnChild Pointer to the child task function
 * \param ... Arguments to the child task (if they exist)
 *
 * \sa ESOS_TASK_WAIT_THREAD
 * \sa ESOS_CHILD_TASK
 *
 * \note Child task should have been defined with \ref ESOS_CHILD_TASK
 * \note Child task structure should have been obtained with \ref ESOS_ALLOCATE_CHILD_TASK
 * \hideinitializer
 */
#define ESOS_TASK_SPAWN_AND_WAIT(pstChild, pfnChild, ...)  \
    __ESOS_TASK_SPAWN((pstChild), (pfnChild)( (pstChild), ##__VA_ARGS__) )

/**
 * Allocates a child task storage structure in the local stack frame.
 *
 * This macro spawns a child ESOS task and waits until it exits. The
 * macro can only be used within an ESOS task.
 *
 * \param pstName Name of variable to represent the allocated child task structure
 * \hideinitializer
 */
#define ESOS_ALLOCATE_CHILD_TASK(pstName)    (pstName)=esos_GetFreeChildTaskStruct()

/*
 do {                                            \
 static ESOS_TASK_HANDLE pstName;            \
  pstName=esos_GetFreeChildTaskStruct();   \
} while(0)
*/

/** @} */

/**
 * \name Sleeping, killing, exiting and restarting tasks
 * @{
 */


/**
 * Put the current task to sleep.
 *
 * This macro will cause the current task to "sleep" (block)
 * until the task is awakened.  The current task will not
 * execute until it is explicitly wakened by some task caliing
 * \ref ESOS_WAKE_TASK with the target task's identifier.
 * The sleeping task will resume execution at the instruction
 * following the ESOS_TASK_SLEEP().
 *
 * \sa ESOS_WAKE_TASK
 *
 * \hideinitializer
 */
#define ESOS_TASK_SLEEP()                        \
  do {                                      \
    __ESOS_SET_TASK_SLEEPING_FLAG(__pstSelf);                             \
    ESOS_TASK_WAIT_WHILE(ESOS_IS_TASK_SLEEPING(__pstSelf));   \
  } while(0)

/**
 * Restart the current ESOS task.
 *
 * This macro will block the current and cause the task to restart
 * its execution at the place of the ESOS_TASK_BEGIN() call at its
 * next scheduled execution time.
 *
 * \hideinitializer
 */
#define ESOS_TASK_RESTART()             \
  do {                                       \
    __pstSelf->flags = 0;                    \
    __ESOS_INIT_TASK(__pstSelf);              \
    return ESOS_TASK_WAITING;                 \
  } while(0)

/**
 * Exit the current ESOS task.
 *
 * This macro causes the current ESOS task to exit. If the ESOS task was
 * spawned by another ESOS task, the parent ESOS task will become
 * unblocked and can continue to run.
 *
 * \sa  ESOS_TASK_SPAWN
 * \sa  ESOS_TASK_WAIT_THREAD
 *
 * \hideinitializer
 */
#define ESOS_TASK_EXIT()                          \
  do {                                            \
    __pstSelf->flags = __TASK_ENDED_MASK;         \
    return ESOS_TASK_ENDED;                       \
  } while(0)


/**
 * Wake up a sleeping ESOS task.
 *
 * This macro will cause the target task to "wake" (resume)
 * at its next opportunity. The sleeping task will resume
 * execution at the instruction following the \ref ESOS_TASK_SLEEP
 * that initially put the task to sleep.
 *
 * \param TaskHandle The \ref ESOS_TASK_HANDLE of the task to wake up.
 * \sa ESOS_TASK_GET_TASK_HANDLE
 * \sa ESOS_TASK_SLEEP
 *
 * \hideinitializer
 */
#define ESOS_WAKE_TASK(TaskHandle)        __ESOS_TASK_CLEAR_SLEEPING_FLAG((TaskHandle))

/**
 * Kill an scheduled ESOS task.
 *
 * This macro will cause the target task to "die" (exit)
 * at its next scheduled execution.
 * The target task will not execute any more instructions.
 *
 * \param TaskHandle The \ref ESOS_TASK_HANDLE of the task to kill
 * \sa ESOS_TASK_GET_TASK_HANDLE
 * \hideinitializer
 */
#define ESOS_KILL_TASK(TaskHandle)    __ESOS_TASK_SET_KILLED_FLAG((TaskHandle))


/**
 * Restart a scheduled ESOS task
 *
 * This macro will cause the target task to "restart"
 * (run from the beginning as if it were just created)
 * at its next scheduled execution.
 * \note Anything the target task has done and all of its
 * local data variables and states will likely be lost.
 * Do <em>NOT</em> restart another task unless you are very
 * sure of how it will respond.
 *
 * \param TaskHandle The \ref ESOS_TASK_HANDLE of the task to kill
 * \sa ESOS_TASK_GET_TASK_HANDLE
 * \hideinitializer
 */
#define ESOS_RESTART_TASK(TaskHandle)             \
  do {                                       \
    (TaskHandle)->flags = 0;                    \
    __ESOS_INIT_TASK((TaskHandle));              \
} while(0)


/** @} */

/**
 * \name Yielding from an ESOS task
 * @{
 */

/**
 * Yield the current ESOS task.
 *
 * This function will yield the ESOS task IMMEDIATELY, thereby
 * allowing other processing to take place in the system.  The
 * task will resume at the next instruction at its next invocation
 * by the scheduler.  (Of course, another task may "kill" it in
 * the meantime and the task will not run again.)
 *
 * \hideinitializer
 */
#define ESOS_TASK_YIELD()               \
  do {                            \
    __ESOS_CLEAR_TASK_CALLED_FLAG(__pstSelf);                   \
    ESOS_TASK_WAIT_UNTIL(__ESOS_IS_TASK_CALLED(__pstSelf));     \
  } while(0)

/** @} */

/**
 * ESOS semaphore structure
 *
 * \sa ESOS_INIT_SEMAPHORE
 * \sa ESOS_TASK_WAIT_SEMAPHORE
 * \sa ESOS_SIGNAL_SEMAPHORE
 */
struct stSemaphore {
  int16_t i16_cnt;
};


/* Not using these with this definition of semaphores */
// #define _SEMNAME_CAT( name, line ) name##line
// #define _SEMNAME( name, line ) _SEMNAME_CAT( name, line )
// #define SEMNAME( name ) _SEMNAME( name, __LINE__ )

/**
 * \name Task semaphores
 * @{
 */

/**
 * Declare (and create storage for) an ESOS counting semaphore
 * \param semaphoreName The name by which the semaphore is to be known
 * \note Declares the memory storage space along with the
 * ESOS_SEMAPHORE "object" that is used by user code.
 * \note Since semaphores are typically used for synchronization,
 * this macro to allocate storage for a semaphore should almost
 * always be used in the "global" variable section of your application.
 * \todo Make sure semaphores are safe in ISRs.  Probably need to make
 * semaphore storage and associated macros <em>volatile</em>
 * \hideinitializer
 */
#define ESOS_SEMAPHORE(semaphoreName)   struct stSemaphore (semaphoreName)

/**
 * Initialize a semaphore
 *
 * This macro initializes a semaphore with a value for the
 * counter. Internally, the semaphores use an "signed 16 bit integer" to
 * represent the counter, and therefore the "count" argument should be
 * between -32768 and +32767
 *
 * \param semaphoreName An ESOS semaphore created by \ref ESOS_SEMAPHORE
 * \param i16_val (int16_t) The initial count of the semaphore.
 * \sa ESOS_SEMAPHORE
 * \sa ESOS_TASK_WAIT_SEMAPHORE
 * \sa ESOS_SIGNAL_SEMAPHORE
 *
 * \hideinitializer
 */
#define ESOS_INIT_SEMAPHORE(semaphoreName, i16_val)     (semaphoreName).i16_cnt=(i16_val)

/**
 * Wait for a semaphore
 *
 * This macro carries out the "wait" operation on the semaphore. The
 * wait operation causes the current ESOS task to block while the counter is
 * zero. When the counter reaches a value larger than zero, the
 * task will continue.
 * \param semaphoreName An ESOS semaphore created by \ref ESOS_SEMAPHORE
 * \param i16_val (int16_t) number to decrement semaphore value
 * \sa ESOS_SEMAPHORE
 * \sa ESOS_INIT_SEMAPHORE
 * \sa ESOS_SIGNAL_SEMAPHORE
 *
 * \hideinitializer
 */
#define ESOS_TASK_WAIT_SEMAPHORE(semaphoreName, i16_val)            \
  do {                                                              \
    ESOS_TASK_WAIT_UNTIL((semaphoreName).i16_cnt >= (i16_val) );    \
    (semaphoreName).i16_cnt -= (i16_val);                            \
   } while(0)

/**
 * Signal a semaphore
 *
 * This macro carries out the "signal" operation on the semaphore. The
 * signal operation increments the counter inside the semaphore, which
 * eventually will cause waiting protothreads to continue executing.
 * \param semaphoreName An ESOS semaphore created by \ref ESOS_SEMAPHORE
 * \param i16_val (int16_t) number to decrement semaphore value
 * \sa ESOS_SEMAPHORE
 * \sa ESOS_TASK_WAIT_SEMAPHORE
 * \sa ESOS_INIT_SEMAPHORE
 *
 * \hideinitializer
 */
#define ESOS_SIGNAL_SEMAPHORE(semaphoreName, i16_val)   (semaphoreName).i16_cnt+=(i16_val)

/* @} */


#endif /* __ESOS_TASK_H__ */

/** @} */

/***********************************************************************
 *
 * MUCH OF THIS CODE IS BASED ON PROTOTHREADS BY ADAM DUNKELS
 * AT THE SWEDISH INSTITUTE OF COMPUTER SCIENCE
 *
 ***********************************************************************
 *
 * Copyright (c) 2004-2005, Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 */
