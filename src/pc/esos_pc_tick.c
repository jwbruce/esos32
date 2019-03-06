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

/**
 * \addtogroup ESOS_Tick
 * @{
 */

// Documentation for this file. If the \file tag isn't present,
// this file won't be documented.
/**
* \file
* \brief PC (linux) code to emulate the ESOS32 system tick functionality.
* 
* \note Windows version has not been tested in many years.
*/


/************************************************************************
 * esos_pc_tick.c
 ************************************************************************
 * User provided (HW-specific) ES_OS routines for IRQs
 */

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/time.h>
#endif

#include    "all_generic.h"

// PROTOTYPE our private little helper functions
uint32_t  clock_time(void);

// create a variable to save the initial clock time in
static uint32_t     initClockCount;

/*
 * User must provide the HW-specific routine to setup a system
 *   tick for ES_OS.
 *

 */
void    __esos_hw_InitSystemTick(void) {
  /*
   * save the time.h clock() count when the system inits
   *   so we can compute elapsed time when the user apps
   *   and the ES_OS request it later via esos_GetSystemTick()
   */
  initClockCount = clock_time();

} // end _esos_hw_InitSystemTick()

/*
 * User must provide the HW-specific routine to return the 32 bit
 *   system tick to ES_OS and user tasks.
 *
 * get the current time and compute delta back to init time
 */
uint32_t   __esos_hw_GetSystemTickCount(void) {
  return (clock_time()-initClockCount);
}  // end _esos_hw_GetSystemTickCount()

uint32_t clock_time(void) {
  struct timeval tv;
  struct timezone tz;

  // gettimeofday() returns time since the epoch.....
  //    in sec and usec members
  gettimeofday(&tv, &tz);
  // give the user time count in milliseconds
  return ((tv.tv_sec * 1000) +(tv.tv_usec / 1000));
}
