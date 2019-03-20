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

/** \file
 * \brief Hardware independent code for ESOS32 to support hardware
 * watchdog timers
 *
 */

/**
 * \addtogroup ESOS_Watchdog_Service
 * @{
 */



/************************************************************************
 * esos_wdog.c
 ************************************************************************
 * ES_OS file for (hardware independent) watchdog timer routines
 */

#include    "esos_wdog.h"

// ******** G L O B A L S ***************
volatile uint32_t		__esos_wdog_ticksWatchdogPeriod;
volatile uint32_t		__esos_wdog_ticksWatchdogLastFed;

/**
* Initializes ESOS watchdog timer
*
* \param number of *milliseconds* between watchdog resets
* \note This function will call _esos_hw_wdog_initWatchdog() routine so
* that the hardware-specific code is execute to setup and enable the
* watchdog timer.  Each hardware port of ESOS32 should provide such
* a function
*
* \sa __esos_hw_wdog_initWatchdog()
* \sa ESOS_USES_WATCHDOG
* \hideinitializer
*/
void    _esos_wdog_initWatchdog(uint32_t	u32_msBetweenWatchdogResets ) {
  // record the watchdog period
  __esos_wdog_ticksWatchdogPeriod = u32_msBetweenWatchdogResets * MILLISECONDS;
  // call the HW-specific code to setup the watchdog timer
  _esos_hw_wdog_initWatchdog( u32_msBetweenWatchdogResets );				
  // record the time we last "fed" the watchdog
  __esos_wdog_ticksWatchdogLastFed = esos_GetSystemTick();
} // end esos_initWatchdog()

/**
* ESOS hardware-indepednet call to "feed" the watchdog timer
*
* \note This function will call _esos_hw_feedWatchdog() routine so
* that the hardware-specific code is execute to "feed" (reset) the
* watchdog timer.  Each hardware port of ESOS32 should provide such
* a function
*
* \sa __esos_hw_wdog_feedWatchdog()
* \sa ESOS_USES_WATCHDOG
* \hideinitializer
*/
void  esos_wdog_feedWatchdog( void ) {
  // record the time we last "fed" the watchdog
  __esos_wdog_ticksWatchdogLastFed = esos_GetSystemTick();	
  // call the HW-specific code to setup the watchdog timer
  _esos_hw_wdog_feedWatchdog();				
} // end esos_feedWatchdog()

/**
* Get the ESOS systems ticks value of when the watchdog was last fed (reset)
*
* \retval The uint32_t value ESOS system ticks value of when the
* watchdog was last "fed" (reset)
*
* \sa esos_wdog_feedWatchdog()
* \sa esos_wdog_getTicksSinceWatchdogFed() 
* \sa ESOS_USES_WATCHDOG
* \hideinitializer
*/
inline uint32_t esos_wdog_getWatchdogFedTicks(void) {
	return __esos_wdog_ticksWatchdogLastFed;
} // end esos_getWatchdogFedTicks(void)

/**
* Get the number of system ticks since the watchdog was last fed (reset)
*
* \retval The uint32_t value of the number of ESOS system ticks value
* since the watchdog was last "fed" (reset)
*
* \sa esos_wdog_feedWatchdog()
* \sa esos_wdog_getWatchdogFedTicks()
* \sa ESOS_USES_WATCHDOG
* \hideinitializer
*/
inline uint32_t esos_wdog_getTicksSinceWatchdogFed(void) {
	return esos_GetSystemTick() - __esos_wdog_ticksWatchdogLastFed;
} // end esos_getTicksSinceWatchdogFed(void)

/**
* Get the number of system ticks until next watchdog reset
*
* \retval The uint32_t value of the number of ESOS system ticks until
* the watchdog will reset the system
*
* \note	This value will only be approximately correct as the variance
* of execution times through the ESOS scheduler loop and watchdog
* watchdog hardware is often driven by a less-than-accurate timer.
* 
* \sa esos_wdog_feedWatchdog()
* \sa esos_wdog_getWatchdogFedTicks()
* \sa ESOS_USES_WATCHDOG
* \hideinitializer
*/
inline uint32_t esos_wdog_getTicksUntilWatchdogReset(void) {
	return __esos_wdog_ticksWatchdogLastFed + __esos_wdog_ticksWatchdogPeriod - esos_GetSystemTick();
}

/**
* Get the programmed watchdog period in ESOS system ticks
*
* \retval The uint32_t value of the number of ESOS system ticks that
* is the full watchdog timeout period
*
* \note	ESOS user applications should strive to maximize time between
* watchdog "feedings", but do not push all the way to the limit in
* case some user tasks is slow to wait/yield control to the scheduler,
* or the user app has many tasks.
* 
* \sa esos_wdog_feedWatchdog()
* \sa esos_wdog_getWatchdogFedTicks()
* \sa ESOS_USES_WATCHDOG
* \hideinitializer
*/
inline uint32_t esos_wdog_getTicksWatchdogPeriod(void) {
	return __esos_wdog_ticksWatchdogPeriod;
}
