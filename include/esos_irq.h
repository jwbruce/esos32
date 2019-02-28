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
 *  This file contains macros, prototypes, and definitions for
 *  relatively hardware-independent interrupts on ESOS
 */


#ifndef ESOS_IRQ_H
#define ESOS_IRQ_H

//#include    "user_config.h"
#include    "all_generic.h"

#define NUM_USER_IRQS       16

void    _esos_hw_InitUserInterrupts(void);

#if 0
// a helpful macro used to declare and prototype user ISRs
#define ESOS_USER_ISR(fcn)              void fcn(void)

// Prototypes for all the hardware specific routines we expect
/**
* Enables the already-registered user interrupt denoted by the interrupt descriptor
* \param u16_desc   Descriptor of the hardware interrupt
* \note Hardware interrupt descriptors are found in the file <b>esos_<i>hw</i>_irq.h</b>
* where <i>hw</i> is the name of the chip for the ESOS port has been written.
* \hideinitializer
*/
void  esos_EnableVerifiedUserInterrupt( uint16_t u16_desc );
void    _esos_hw_DisableAllUserInterrupts(void);
void    _esos_hw_EnableAllUserInterrupts(void);
void    _esos_hw_InitUserInterrupts(void);
uint8_t   _esos_hw_IsUserInterruptEnabled(uint16_t  u16_desc );
uint8_t   _esos_hw_IsUserInterruptFlagSet(uint16_t  u16_desc );

/**
* Unregisters the user interrupt denoted by the interrupt descriptor
* \param u16_desc   Descriptor of the hardware interrupt
* \note Hardware interrupt descriptors are found in the file <b>esos_<i>hw</i>_irq.h</b>
* where <i>hw</i> is the name of the chip for the ESOS port has been written.
* \hideinitializer
* \sa esos_RegisterUserInterrupt
* \sa esos_EnableUserInterrupt
* \sa esos_DisableUserInterrupt
*/
void    esos_UnregisterUserInterrupt( uint16_t u16_desc );

/**
* Registers the user interrupt denoted by the interrupt descriptor
* \param u16_desc   Descriptor of the hardware interrupt
* \param u8_IPL     Interrupt Priority Level (IPL) for the interrupt being registered
* \param p2f        Pointer to function to serve as the ISR. Function must have <em>void</em>
* arguments and return <em>void</em>
* \note Hardware interrupt descriptors are found in the file <b>esos_<i>hw</i>_irq.h</b>
* where <i>hw</i> is the name of the chip for the ESOS port has been written.
* \hideinitializer
* \sa esos_RegisterUserInterrupt
* \sa esos_EnableUserInterrupt
* \sa esos_DisableUserInterrupt
*/
void    esos_RegisterUserInterrupt( uint16_t u16_desc, uint8_t u8_IPL, void (*p2f)(void) );

void    _esos_DoNothingIsr(void);
void    _esos_hw_DoNothingIsr(void);

// Map the HW-specific routine names into osXXXXXX ones
//#define   esos_InitUserInterrupts         _esos_hw_InitUserInterrupts

/**
* Disables all registered user interrupts.
* \note This function does <em>NOT</em> disable all interrupts.  Interrupts used by
* ESOS will continue to function.
* \hideinitializer
*/
#define     esos_DisableAllUserInterrupts          _esos_hw_DisableAllUserInterrupts

/**
* Enables all registered user interrupts.
* \note Does not affect interrupts used by ESOS
* \hideinitializer
*/
#define     esos_EnableAllUserInterrupts           _esos_hw_EnableAllUserInterrupts

/**
* Queries the enabled state of user interrupt denoted by the interrupt descriptor
* \param u16_desc   Descriptor of the hardware interrupt
* \retval TRUE    If the user interrupt is currently enabled
* \retval FALSE    If the user interrupt is currently disabled
* \note Hardware interrupt descriptors are found in the file <b>esos_<i>hw</i>_irq.h</b>
* where <i>hw</i> is the name of the chip for the ESOS port has been written.
* \hideinitializer
* \sa esos_RegisterUserInterrupt
* \sa esos_UnregisterUserInterrupt
* \sa esos_EnableUserInterrupt
* \sa esos_DisableUserInterrupt
*/
#define     esos_IsUserInterruptEnabled            _esos_hw_IsUserInterruptEnabled

/**
* Queries the "flag" state of user interrupt denoted by the interrupt descriptor
* \param u16_desc   Descriptor of the hardware interrupt
* \retval TRUE    If the user interrupt is needs to be serviced
* \retval FALSE    Otherwise
* \note Hardware interrupt descriptors are found in the file <b>esos_<i>hw</i>_irq.h</b>
* where <i>hw</i> is the name of the chip for the ESOS port has been written.
* \hideinitializer
* \sa esos_MarkUserInterruptServiced
*/
#define     esos_DoesUserInterruptNeedServicing            _esos_hw_IsUserInterruptFlagSet

/**
* Tells ESOS to mark the user interrupt denoted by the interrupt descriptor as being serviced
* \param u16_desc   Descriptor of the hardware interrupt
* \note Hardware interrupt descriptors are found in the file <b>esos_<i>hw</i>_irq.h</b>
* where <i>hw</i> is the name of the chip for the ESOS port has been written.
* \hideinitializer
* \sa esos_DoesUserInterruptNeedServicing
*/
#define     esos_MarkUserInterruptServiced            _esos_hw_ClearUserInterruptFlag

/**
* Enables the user interrupt denoted by the interrupt descriptor
* \param u16_desc   Descriptor of the hardware interrupt
* \note Hardware interrupt descriptors are found in the file <b>esos_<i>hw</i>_irq.h</b>
* where <i>hw</i> is the name of the chip for the ESOS port has been written.
* \hideinitializer
* \sa esos_RegisterUserInterrupt
* \sa esos_UnregisterUserInterrupt
* \sa esos_IsUserInterruptEnabled
* \sa esos_DisableUserInterrupt
*/
#define     esos_EnableUserInterrupt                _esos_hw_EnableUserInterrupt

/**
* Disables the user interrupt denoted by the interrupt descriptor
* \param u16_desc   Descriptor of the hardware interrupt
* \note Hardware interrupt descriptors are found in the file <b>esos_<i>hw</i>_irq.h</b>
* where <i>hw</i> is the name of the chip for the ESOS port has been written.
* \hideinitializer
* \sa esos_RegisterUserInterrupt
* \sa esos_UnregisterUserInterrupt
* \sa esos_IsUserInterruptEnabled
* \sa esos_EnableUserInterrupt
*/
#define     esos_DisableUserInterrupt                _esos_hw_DisableUserInterrupt

/*
 * And define the private data storage we need
 */
//extern void             (*__esos_IsrFcns[NUM_USER_IRQS])(void);

#endif  // IF 0

#endif              // ESOS_IRQ_H
