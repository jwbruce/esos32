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
/**
* /** \file
* \brief Template for creating hardware-specific support for the
* ESOS32 watchdog service
*/

/**
 * \addtogroup ESOS_Watchdog_Service
 * @{
 */


/*** I N C L U D E S *************************************************/
#include "esos_hwxxx_wdog.h"

/*** G L O B A L S *************************************************/

/*** T H E   C O D E *************************************************/

/*********************************************************
 * Public functions intended to be called by other files *
 *********************************************************/
/**
* Initialize the hardware watchdog timers
*
* \param u32_ticksBetweenWatchdogResets desired number of milliseconds betweeen
*  watchdog resets
*
* \sa _esos_wdog_initWatchdog()
* \sa _esos_hw_wdog_feedWatchdog()
* \hideinitializer
*/ 
void    _esos_hw_wdog_initWatchdog(uint32_t	u32_ticksBetweenWatchdogResets );

}

/**
* "Feeds" the hardware watchdog
* \note This function <em>ASSUMES</em> that the hardware watchdog has been
* configured properly
*
* \sa _esos_hw_wdog_initWatchdog()
* \sa esos_wdog_feedWatchdog()
* \hideinitializer
*/ 
void  	_esos_hw_wdog_feedWatchdog( void );
  // HW code to feed/reset the watchdog timer goes here
}


/** @} */
