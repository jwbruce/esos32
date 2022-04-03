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
 * Embedded Systems Operating System 32-bit (ESOS32)
 *
 */

#ifndef ESOS_H
#define ESOS_H

#ifndef BUILT_ON_ESOS
#define BUILT_ON_ESOS
#endif

// Place this define here because many of the the following INCLUDE files need this macro variable
#define     MAX_NUM_USER_TASKS      32


// Include all the files we need
#include "all_generic.h"
#include "esos_utils.h"
#include "esos_task.h"          // defines ESOS tasks and semaphores
#include "esos_mail.h"          // defines ESOS task mailboxes (eventually make MAILBOXes optional)

// PUT THESE HERE FOR NOW.  They belong somewhere else
// in the long-run.
//
// PC versions do NOT have IRQs (yet) so do NOT define the variables
#if 0
// #ifndef __linux
#define ESOS_USE_IRQS
#endif

#define ESOS_USE_SERIAL_PORT

/**
* Define the constants and structures required for our little
* embedded systems operating system
*/
#if defined(ESOS_USE_SERIAL_PORT)
#include    "esos_comm.h"
#endif      // USE_USB or USE_SERIAL

#ifdef  ESOS_USE_IRQS
/*
*  user wants IRQ support, so prototype the required IRQ functions
*/
#include    "esos_irq.h"
#endif      //ESOS_USE_IRQS

#ifdef  ESOS_USE_WATCHDOG
/*
*  user wants to include watchdog timer support
*/
#include    "esos_wdog.h"
#endif      //ESOS_USE_WATCHDOG

#ifdef  ESOS_USE_SUI
/*
*  user wants to include the simple user inteface service support
*/
#include    "esos_sui.h"
#endif      //ESOS_USE_SUI

#ifdef ESOS_USE_LCD
/*
*  user wants to use the LCD interface
*/
#include    "esos_lcd44780wo.h"
#endif      //ESOS_USE_LCD

#if defined (ESOS_USE_I2C_100KBPS) || defined (ESOS_USE_I2C) || defined (ESOS_USE_I2C_400KBPS)
/*
*  user wants to use the I2C interface
*/
#include	"esos_i2c.h"
#endif		// ESOS_USE_I2C


#if defined (ESOS_USE_SPI)
/*
*  user wants to use the SPI interface
*/
#include	"esos_spi.h"
#endif		// ESOS_USE_SPI


#ifdef  ESOS_RUNS_ON_REAL_OS
// computers running a host OS (windoze/Linux/etc) expect
//      their applications to return and return a value
//      so define a return type for main and create a return statement
//      also, multitasking OS-es will want some CPU time for other
//      applications, so sleep some (10ms = 10000us) should be enough
typedef             int                 main_t;
#define             OS_END              return(1)
#define             OS_ITERATE          usleep(10000)
#else
// hardware w/o a hosting OS will never return. So main_t
//      must be void return type and hang-up when done!
//      (Of course, real hardware apps should never be done.)
typedef             int                 main_t;
#define             OS_END              while(1)
#define             OS_ITERATE
#endif

//*********************************************************************
// P R I V A T E    D E F I N I T I O N S
//*********************************************************************
/**
 * Define the maximum number of user tasks in the system
 *    \note Technically, this is actually the maximum number of
 *          tasks that will be running "concurrently".  Usually,
 *          this number is the maximum number of tasks that the
 *          user has defined, UNLESS they are absolutely sure that
 *          two (or more) tasks are mutually exclusive in execution.
 *
 *    \note BOTH "parent" and "child" tasks use this NUMBER to allocate
 *      their pool of tasks.  So this number should be equal to or greater
 *          than the MAXIMUM number of concurrently running child --OR--
 *          parent tasks.
 */
#define     MAX_NUM_CHILD_TASKS     MAX_NUM_USER_TASKS
#define     REMOVE_IDX              0xFE


/* S T R U C T U R E S ******************************************************/
/**
* structure to contain a set of descriptors about the buffers used
* to implement ESOS task mailboxes
**/
struct stMailBoxDesc {
  volatile uint8_t* pau8_Data;
  int16_t                 u16_Head;
  int16_t                 u16_Tail;
  uint16_t                u16_Length;
  struct stTask   pst_Task;
};

/**
* structure to contain a mail message "envelope" -- a set of descriptors
* about a mail message and the contents within
**/
struct stMailEnvelope {
  uint16_t    u16_Header;
  uint32_t    u32_Postmark;
};

struct stTimer {
  void    (*pfn)(void);
  uint32_t  u32_period;
  uint32_t  u32_cntDown;
};

// Define masks for the user to use for their flags
/** Mask for a global user flag provided by ESOS
 * \hideinitializer
 */
#define   ESOS_USER_FLAG_0          ESOS_BIT0
/** Mask for a global user flag provided by ESOS
 * \hideinitializer
 */
#define   ESOS_USER_FLAG_1          ESOS_BIT1
/** Mask for a global user flag provided by ESOS
 * \hideinitializer
 */
#define   ESOS_USER_FLAG_2          ESOS_BIT2
/** Mask for a global user flag provided by ESOS
 * \hideinitializer
 */
#define   ESOS_USER_FLAG_3          ESOS_BIT3
/** Mask for a global user flag provided by ESOS
 * \hideinitializer
 */
#define   ESOS_USER_FLAG_4          ESOS_BIT4
/** Mask for a global user flag provided by ESOS
 * \hideinitializer
 */
#define   ESOS_USER_FLAG_5          ESOS_BIT5
/** Mask for a global user flag provided by ESOS
 * \hideinitializer
 */
#define   ESOS_USER_FLAG_6          ESOS_BIT6
/** Mask for a global user flag provided by ESOS
 * \hideinitializer
 */
#define   ESOS_USER_FLAG_7          ESOS_BIT7
/** Mask for a global user flag provided by ESOS
 * \hideinitializer
 */
#define   ESOS_USER_FLAG_8          ESOS_BIT8
/** Mask for a global user flag provided by ESOS
 * \hideinitializer
 */
#define   ESOS_USER_FLAG_9          ESOS_BIT9
/** Mask for a global user flag provided by ESOS
 * \hideinitializer
 */
#define   ESOS_USER_FLAG_10          ESOS_BIT10
/** Mask for a global user flag provided by ESOS
 * \hideinitializer
 */
#define   ESOS_USER_FLAG_11          ESOS_BIT11
/** Mask for a global user flag provided by ESOS
 * \hideinitializer
 */
#define   ESOS_USER_FLAG_12          ESOS_BIT12
/** Mask for a global user flag provided by ESOS
 * \hideinitializer
 */
#define   ESOS_USER_FLAG_13          ESOS_BIT13
/** Mask for a global user flag provided by ESOS
 * \hideinitializer
 */
#define   ESOS_USER_FLAG_14          ESOS_BIT14
/** Mask for a global user flag provided by ESOS
 * \hideinitializer
 */
#define   ESOS_USER_FLAG_15          ESOS_BIT15
/** Mask for a global user flag provided by ESOS
 * \hideinitializer
 */
#define   ESOS_USER_FLAG_16          ESOS_BIT16
/** Mask for a global user flag provided by ESOS
 * \hideinitializer
 */
#define   ESOS_USER_FLAG_17          ESOS_BIT17
/** Mask for a global user flag provided by ESOS
 * \hideinitializer
 */
#define   ESOS_USER_FLAG_18          ESOS_BIT18
/** Mask for a global user flag provided by ESOS
 * \hideinitializer
 */
#define   ESOS_USER_FLAG_19          ESOS_BIT19
/** Mask for a global user flag provided by ESOS
 * \hideinitializer
 */
#define   ESOS_USER_FLAG_20          ESOS_BIT20
/** Mask for a global user flag provided by ESOS
 * \hideinitializer
 */
#define   ESOS_USER_FLAG_21          ESOS_BIT21
/** Mask for a global user flag provided by ESOS
 * \hideinitializer
 */
#define   ESOS_USER_FLAG_22          ESOS_BIT22
/** Mask for a global user flag provided by ESOS
 * \hideinitializer
 */
#define   ESOS_USER_FLAG_23          ESOS_BIT23
/** Mask for a global user flag provided by ESOS
 * \hideinitializer
 */
#define   ESOS_USER_FLAG_24          ESOS_BIT24
/** Mask for a global user flag provided by ESOS
 * \hideinitializer
 */
#define   ESOS_USER_FLAG_25         ESOS_BIT25
/** Mask for a global user flag provided by ESOS
 * \hideinitializer
 */
#define   ESOS_USER_FLAG_26          ESOS_BIT26
/** Mask for a global user flag provided by ESOS
 * \hideinitializer
 */
#define   ESOS_USER_FLAG_27          ESOS_BIT27
/** Mask for a global user flag provided by ESOS
 * \hideinitializer
 */
#define   ESOS_USER_FLAG_28          ESOS_BIT28
/** Mask for a global user flag provided by ESOS
 * \hideinitializer
 */
#define   ESOS_USER_FLAG_29          ESOS_BIT29
/** Mask for a global user flag provided by ESOS
 * \hideinitializer
 */
#define   ESOS_USER_FLAG_30          ESOS_BIT30
/** Mask for a global user flag provided by ESOS
 * \hideinitializer
 */
#define   ESOS_USER_FLAG_31          ESOS_BIT31


/**
 * Declaration of an user-defined timer callback (for ESOS timer services)
 *
 * This macro is used to declare a user-timer. All timers using ESOS timer
 * services  must be declared with this macro.
 *
 * \param timername The name of the software timer to create.
 * \note You are really creating a C function implementing the user timer callback.
 *   Since ESOS timer serivces calls this callback function at the appropriate time,
 *  this function cannot be passed arguments and cannot return values
 * \hideinitializer
 */
#define   ESOS_USER_TIMER(timername)    void timername(void)

/**
 * Handle to a software timer in the ESOS timer service.
 *
 * \sa ESOS_USER_TIMER
 * \sa esos_RegisterTimer
 * \sa esos_UnregisterTimer
 * \sa esos_GetTimerHandle
 * \sa esos_ChangeTimerPeriod
 *
 * \hideinitializer
 */
typedef   uint8_t                   ESOS_TMR_HANDLE;

/*
 * Now define the public user function prototypes that are
 *   always available regardless of the user_config settings
 */
/**
 * User-provided function to initialize user's hardware configuration
 * register user tasks.
 *
 * \note All ESOS applications <em>MUST</em> provide this function.
 * \note This function  <em>MUST</em>  register at least one ESOS task.
 *
 * \note user_init() is a centralized initialization routine where
 *                  the user can setup their application.   It is called
 *                  automagically by ES_OS during the operating system
 *                  initialization.
 *
 * \note User should set up any state machines and init
 *                  all application variables.  They can also turn on
 *                  any needed peripherals here.
 *
 * \note User <em>shall not</em> mess with the interrupt hardware
 *                  directly!!!  The ESOS must be aware of the interrupts
 *                  and provides esos_XXXXXXX functions for the user to use.
 *                  Using these ESOS-provided functions, the user may
 *                  (and probably should) initialize, register, and enable
 *                  interrupts in this routine.
 *
 * \note Furthermore, the user should register AT LEAST one
 *                  user application task here via \ref esos_RegisterTask
 *                  or the ES_OS scheduler will have nothing to schedule
 *                  to run when this function returns.
 * \hideinitializer
 */

void    user_init( void );
ESOS_TASK_HANDLE   esos_RegisterTask( uint8_t (*pfn_TaskFcn)(struct stTask *pst_Task) );
uint8_t   esos_UnregisterTask( uint8_t (*pfn_TaskFcn)(struct stTask *pst_Task) ) ;
ESOS_TASK_HANDLE  esos_GetFreeChildTaskStruct();
ESOS_TASK_HANDLE    esos_GetTaskHandle( uint8_t (*taskname)(ESOS_TASK_HANDLE pstTask) );
ESOS_TASK_HANDLE    esos_GetTaskHandleFromID( uint16_t u16_TaskID );


// prototypes for ESOS software timers
ESOS_TMR_HANDLE    esos_RegisterTimer( void (*pfnTmrFcn)(void), uint32_t u32_period );
uint8_t    esos_UnregisterTimer( ESOS_TMR_HANDLE hnd_timer );
ESOS_TMR_HANDLE    esos_GetTimerHandle( void (*pfnTmrFcn)(void) );
uint8_t    esos_ChangeTimerPeriod( ESOS_TMR_HANDLE hnd_timer, uint32_t u32_period );

/* ********************************************************************
*  ESOS requires that the hardware-specific code provide certain basic
*  functions, even for a stripped down ESOS app.
* ********************************************************************/

// The user must provide the HW-specific way of getting a 32bit 1.0ms tick
void    	__esos_hw_InitSystemTick(void);
uint32_t 	__esos_hw_GetSystemTickCount(void);
// pseudo random number generation routines
uint32_t	__esos_hw_PRNG_u32(void);
void		__esos_hw_config_PRNG(void);
void 		__esos_hw_set_PRNG_Seed(uint32_t u32_seed);


#define		__MILLISECONDS_PER_TICK				1
#define		MILLISECONDS						__MILLISECONDS_PER_TICK
#define		SECONDS								MILLISECONDS*1000
#define		MINUTES								SECONDS*60
#define		HOURS								MINUTES*60
#define		DAYS								HOURS*24
/**
 * Get the current value of the ESOS system tick counter
 * In the current implementation of ESOS, a tick equal 1.0ms.
 * Therefore, the value returned by this function is approximately
 * equal to the number of milliseconds since the since was last
 * reset.
 * \return The uint32 value of current value of the ESOS
 * system tick counter
 * \note This counter value will roll-over every 49.7 days.
 * \hideinitializer
 */
#define   esos_GetSystemTick()          __esos_hw_GetSystemTickCount()


uint16_t  __esos_hasTickDurationPassed(uint32_t u32_startTick, uint32_t u32_period);
void    __esos_tmrSvcsExecute(void);

void    __esos_InitCommSystem(void);


/*
 * expose these ESOS system variables to allow macro access
 * intead of fcn access
 */
extern uint8_t        __esos_u8UserTasksRegistered;
extern uint32_t       __esos_u32UserFlags, __esos_u32SystemFlags;

/**
 * Get the current number of user task registered with the
 * ESOS scheduler.
 * \return The uint8_t number of currently registered user tasks
 * \note This value does not include the number of child tasks
 * (tasks of the type \ref ESOS_CHILD_TASK ), just the tasks
 * of the type \ref ESOS_USER_TASK
 * \hideinitializer
 */
#define esos_GetNumberRegisteredTasks()        (__esos_u8UserTasksRegistered)

/**
 * Returns the system tick value of a future time
 * \param deltaT the number of ticks in the future you'd like the
 * system tick value for
 * \return The uint32 number corresponding to the system tick
 * value of that future time
 * \sa esos_GetSystemTick
 * \hideinitializer
 */
#define esos_GetFutureSystemTick(deltaT)    ((uint32_t)(deltaT) + __esos_hw_GetSystemTickCount());

/**
 * Sets bits in the global user flags provided by ESOS
 * \param mask An uint16 value composed of the OR-ed user
 * mask flag masks, where each flag in the OR will be set
 * \note User should use the provided bits masks like \ref ESOS_USER_FLAG_0
 *  to create their own readable constants
 * \code
 * #define HEADLIGHTS_ARE_ON         ESOS_USER_FLAG_3
 * #define MY_USER_FRIENDLY_FLAG     ESOS_USER_FLAG_7
 * esos_SetUserFlag( HEADLIGHTS_ARE_ON | MY_USER_FRIENDLY_FLAG);
 * \endcode
 * \sa esos_ClearUserFlag
 * \sa esos_IsUserFlagSet
 * \sa esos_IsUserFlagClear
 * \hideinitializer
 */
#define esos_SetUserFlag(mask)              BIT_SET_MASK(__esos_u32UserFlags, (mask))

/**
 * Clears bits in the global user flags provided by ESOS
 * \param mask An uint16 value composed of the OR-ed user
 * mask flag masks, where each flag in the OR will be cleared
 * \note User should use the provided bits masks like \ref ESOS_USER_FLAG_0
 * and \ref ESOS_USER_FLAG_1 and ... \ref ESOS_USER_FLAG_F
 *  to create their own readable constants
 * \code
 * #define HEADLIGHTS_ARE_ON         ESOS_USER_FLAG_3
 * #define MY_USER_FRIENDLY_FLAG     ESOS_USER_FLAG_7
 * esos_ClearUserFlag( HEADLIGHTS_ARE_ON | MY_USER_FRIENDLY_FLAG);
 * \endcode
 * \sa esos_SetUserFlag
 * \sa esos_IsUserFlagSet
 * \sa esos_IsUserFlagClear
 * \hideinitializer
 */

#define esos_ClearUserFlag(mask)            BIT_CLEAR_MASK(__esos_u32UserFlags, (mask))

/**
 * Queries whether the global user flags provided by ESOS are set
 * \param mask An uint16 value composed of the OR-ed user
 * mask flag masks, where each flag in the OR will be checked for being set
 * \retval TRUE if <em>at least</em> one of the flags is set
 * \retval FALSE if none of the flags are set
 * \note User should use the provided bits masks like \ref ESOS_USER_FLAG_0
 * and \ref ESOS_USER_FLAG_1 and ... \ref ESOS_USER_FLAG_F
 *  to create their own readable constants
 * \code
 * #define HEADLIGHTS_ARE_ON         ESOS_USER_FLAG_3
 * #define MY_USER_FRIENDLY_FLAG     ESOS_USER_FLAG_7
 * esos_ClearUserFlag( HEADLIGHTS_ARE_ON | MY_USER_FRIENDLY_FLAG);
 * while(esos_IsUserFlagSet( HEADLIGHTS_ARE_ON));  // falls through
 * \endcode
 * \sa esos_SetUserFlag
 * \sa esos_ClearUserFlag
 * \sa esos_IsUserFlagClear
 * \hideinitializer
 */
#define esos_IsUserFlagSet(mask)            IS_BIT_SET_MASK(__esos_u32UserFlags, (mask))

/**
 * Queries whether the global user flags provided by ESOS are clear
 * \param mask An uint16 value composed of the OR-ed user
 * mask flag masks, where each flag in the OR will be checked for being clear
 * \retval TRUE if <em>at least</em> one of the flags is clear
 * \retval FALSE if none of the flags are clear
 * \note User should use the provided bits masks like \ref ESOS_USER_FLAG_0
 * and \ref ESOS_USER_FLAG_1 and ... \ref ESOS_USER_FLAG_F
 *  to create their own readable constants
 * \code
 * #define HEADLIGHTS_ARE_ON         ESOS_USER_FLAG_3
 * #define MY_USER_FRIENDLY_FLAG     ESOS_USER_FLAG_7
 * esos_ClearUserFlag( HEADLIGHTS_ARE_ON | MY_USER_FRIENDLY_FLAG);
 * while(esos_IsUserFlagClear( HEADLIGHTS_ARE_ON));  // infinite loop
 * \endcode
 * \sa esos_SetUserFlag
 * \sa esos_ClearUserFlag
 * \sa esos_IsUserFlagSet
 * \hideinitializer
 */
#define esos_IsUserFlagClear(mask)          IS_BIT_CLEAR_MASK(__esos_u32UserFlags, (mask))

// define macros for ESOS system flags
//    these flags are NOT to be manipulated directly by the user!
#define __esos_SetSystemFlag(mask)              BIT_SET_MASK(__esos_u32SystemFlags, (mask))
#define __esos_ClearSystemFlag(mask)            BIT_CLEAR_MASK(__esos_u32SystemFlags, (mask))
#define __esos_IsSystemFlagSet(mask)            IS_BIT_SET_MASK(__esos_u32SystemFlags, (mask))
#define __esos_IsSystemFlagClear(mask)          IS_BIT_CLEAR_MASK(__esos_u32SystemFlags, (mask))

// Defines for ESOS timer services
#define   ESOS_TMR_FAILURE    0xFF
#define   MAX_NUM_TMRS        16

/**
 * Get the current number of user software timers registers (running)
 * in the ESOS timer services
 * \return The uint8_t number of currently registered user tasks
 * \hideinitializer
 */
#define esos_GetNumberRunningTimers()          (__esos_u8TmrSvcsRegistered)

/**
 * Determines if the software timer  represented by the handle is currently running
 * \param hndl The \ref ESOS_TMR_HANDLE of a software timer
 * \retval TRUE if the timer is currently running
 * \retval FALSE if the timer is not currently running
 * \sa ESOS_USER_TIMER
 * \sa esos_UnregisterTimer
 * \sa esos_GetTimerHandle
 * \sa esos_ChangeTimerPeriod
 * \hideinitializer
 */
#define   esos_IsTimerRunning(hndl)            IS_BIT_SET_MASK(__esos_u32TmrActiveFlags, (ESOS_BIT0<<(hndl)))
#define   __esos_MarkTimerRunning(hndl)        BIT_SET_MASK(__esos_u32TmrActiveFlags, (ESOS_BIT0<<(hndl)))
#define   __esos_MarkTimerStopped(hndl)        BIT_CLEAR_MASK(__esos_u32TmrActiveFlags, (ESOS_BIT0<<(hndl)))


// System flag definitions... only ESOS needs to use these
#define     __ESOS_SYS_FLAG_PACK_TASKS        ESOS_BIT0
#define     __ESOS_SYS_FLAG_NULL_LAST_TASK      ESOS_BIT1
#define     __ESOS_SYS_COMM_TX_IS_BUSY        ESOS_BIT2
#define     __ESOS_SYS_COMM_RX_IS_BUSY        ESOS_BIT3
#define   __ESOS_SYS_COMM_TX_ONGOING    ESOS_BIT4
#define   __ESOS_SYS_I2C_IN_USE           ESOS_BIT8
#define   __ESOS_SYS_SPI_IN_USE           ESOS_BIT9
#define   __ESOS_SYS_ADC_IN_USE           ESOS_BIT10

// Other useful macros for the user
#define   __abs(x)    (((x) < 0) ? -(x) : (x))

#endif          // ESOS_H
