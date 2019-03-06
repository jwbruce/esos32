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
 *  \brief This is the master include file for implementing ESOS32
 * the STMicroelectronics STM32L4xx MCUs
 */

#ifndef ESOS_STM32L4_H
#define ESOS_STM32L4_H

//#include    "user_config.h"

#ifdef      __linux
/*
* Oh! We're running on a real computer (PC/Linux/etc.)
*   So let's emulate the MCU instead
*/

#define     pcrlf()       printf("\n")

// Emulate the STM32L4's registers in RAM variable
// The user must provide these variables in the user file
// extern uint8_t        PIE1;  etc. etc. etc.

#else
/*
 * OK!  We have a real microprocessor, so start including the real
 *   hardware files for the compiler
 */
 
#include    "stm32l4xx_hal.h"

#endif      // __linux

// include the IRQ mask definitions
#ifdef      ESOS_USE_IRQS
#include    "esos_stm32l4_irq.h"
#endif
#ifdef 		ESOS_USE_SERIAL_PORT
#include    "esos_stm32l4_rs232.h"
#endif

#endif      // ESOS_STM32L4_H
