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
 *  \brief This file contains macros, prototypes, and definitions for
 *  a generic hardware implementation for watchdog service on ESOS32
 */

#ifndef _ESOS_hwxxx_WDOG_H
#define _ESOS_hwxxx_WDOG_H

/* I N C L U D E S **********************************************************/
#include "esos.h"
#include "esos_wdog.h"
#include "esos_hwxxx.h"

/* D E F I N I T I O N S ****************************************************/

/* E X T E R N S ************************************************************/

/* M A C R O S **************************************************************/

/* P U B L I C  P R O T O T Y P E S *****************************************/
extern void    _esos_hw_wdog_initWatchdog(uint32_t	u32_msBetweenWdogResets );
extern void  	_esos_hw_wdog_feedWatchdog( void ) ;

// Documentation for this file. If the \file tag isn't present,
// this file won't be documented.

/* ########################################################################### */


#endif // end ESOS_hwxxx_WDOG_H
