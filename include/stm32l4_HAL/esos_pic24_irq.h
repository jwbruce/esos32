/*
 * "Copyright (c) 2008 Robert B. Reese, Bryan A. Jones, J. W. Bruce ("AUTHORS")"
 * All rights reserved.
 * (R. Reese, reese_AT_ece.msstate.edu, Mississippi State University)
 * (B. A. Jones, bjones_AT_ece.msstate.edu, Mississippi State University)
 * (J. W. Bruce, jwbruce_AT_ece.msstate.edu, Mississippi State University)
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
 *  Microchip PIC24 family specific interrupts on ESOS
 */

/************************************************************************
 * esos_pic24_irq.h
 ************************************************************************
 * User-supplied include file which defines the IRQ which are supported
 *
 * NOTE:  the file must be consistent with esos_pic24_irq.c which uses
 *          many of these constant to manipulate IRQ registers
 */

#ifndef     ESOS_PIC24_IRQ_H
#define ESOS_PIC24_IRQ_H

#include "esos.h"
#include "esos_irq.h"
#include "esos_pic24.h"


/**
 * Declaration of an ESOS interrupt.
 *
 * This macro is used to declare an ESOS user inerrupt. All ESOS user
 * interrupt tasks must be declared with this macro.
 *
 * \param desc Descriptor of the hardware interrupt
 *
 * \note ESOS user interrupts have no arguments passed in and cannot return values.
 * \note Hardware interrupt descriptors are found in the file <b>esos_<i>hw</i>_irq.h</b>
 * where <i>hw</i> is the name of the chip for the ESOS port has been written.
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 *
 * \hideinitializer
 */
#define ESOS_USER_INTERRUPT(desc)    __xESOS_USER_ISR(desc)
#define __xESOS_USER_ISR(attrib, ivt, ifsr, ifsb, ipcr, ipcb)      void _ISRFAST attrib (void)



/*
 * Define the ESOS user IRQ levels here
 *  ESOS-based IRQs will run at IRQ priority levels
 *  7 and 5.
 *
 * NOTE: Any user IRQ with its IRQ priority level at 0 signifies
 *  that the IRQ is not registered with ESOS.
 */
/**
 * Highest priority user interrupt level on PIC24 Family
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \hideinitializer
 */
#define ESOS_USER_IRQ_LEVEL1        6
/**
 * Second-highest priority user interrupt level on PIC24 Family
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \hideinitializer
 */
#define ESOS_USER_IRQ_LEVEL2        4
/**
 * Third-highest priority user interrupt level on PIC24 Family
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \hideinitializer
 */
#define ESOS_USER_IRQ_LEVEL3        3
/**
 * Lowest priority user interrupt level on PIC24 Family
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \hideinitializer
 */
#define ESOS_USER_IRQ_LEVEL4        2

#define __ESOS_USER_IRQ_UNREGISTERED    0
// this #define is the IPL that will disabled all user IRQs at once
#define __ESOS_DISABLE_USER_IRQS_LEVEL    ESOS_USER_IRQ_LEVEL2
// this #define is the IPL that will enabled all user IRQs at once
#define __ESOS_ENABLE_USER_IRQS_LEVEL   __ESOS_USER_IRQ_UNREGISTERED

/********************************************************
***  IRQ masks for the PIC24/dsPIC33 chips
***
***  The old verion (<= rev 511) defined thes interrupt
***  based on the compiler target device.  This rewrite
***  (6 SEPT 2014) will attempt to diagnose whether the
***  target devices possesses each interrupt by checking
***  for the existence of the appropriate interrupt flag
***
***  TODO: make a thorough search through the datasheets
***        for all device families, models, etc to make
***        sure Microchip did NOT change the name of these
***        interrupt flags over the years.  (They have
***        a nasty habit of doing that quietly when new
***        chips come out.)
***
*** MNEMONIC  C30 ISR ATTRIB, IVT addr, IFS register, IFS bit, IPC register, IPC bit
***  see Interrupt Vector Details table in FRM or datasheets
*********************************************************/

/**********************************************************************
*** EXTERNAL ASYNCHRONOUS INTERRUPTS
**********************************************************************/
#if (defined(_INT0IF))
/**
 * External INT0 Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_INT0  _INT0Interrupt, 0x0014, 0, 0, 0, 0             // external interrupt 0
#endif

#if (defined(_INT1IF))
/**
 * External INT1 Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_INT1  _INT1Interrupt, 0x003C, 1, 4, 5, 0             // external interrupt 1
#endif

#if (defined(_INT2IF))
/**
 * External INT2 Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_INT2  _INT2Interrupt, 0x004E, 1, 13, 7, 4            // external interrupt 2
#endif

#if (defined(_INT3IF))
/**
 * External INT3 Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_INT3  _INT3Interrupt, 0x007E, 3, 5, 13, 4            // external interrupt 3
#endif

#if (defined(_INT4IF))
/**
 * External INT4 Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_INT4  _INT4Interrupt, 0x0080, 3, 6, 13, 8            // external interrupt 4
#endif

/**********************************************************************
*** INPUT CAPTURES
**********************************************************************/
#if (defined(_IC1IF))
/**
 * Input Capture 1 Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_IC1 _IC1Interrupt, 0x0016, 0, 1, 0, 4              // Input capture channel 1
#endif

#if (defined(_IC2IF))
/**
 * Input Capture 2 Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_IC2 _IC2Interrupt, 0x001E, 0, 5, 1, 4              // Input capture channel 2
#endif


#if (defined(_IC3IF))
/**
 * Input Capture 3 Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_IC3   _IC3Interrupt,    0x005E, 2,  5,  9,  4 // Input Capture 3
#endif

#if (defined(_IC4IF))
/**
 * Input Capture 4 Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_IC4   _IC4Interrupt,    0x0060, 2,  6,  9,  8 // Input Capture 4
#endif

#if (defined(_IC5IF))
/**
 * Input Capture 5 Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_IC5 _IC5Interrupt, 0x0062, 2, 7, 9, 12              // Input capture channel 5
#endif

#if (defined(_IC6IF))
/**
 * Input Capture 6 Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_IC6 _IC6Interrupt, 0x0064, 2, 8, 10, 0              // Input capture channel 6
#endif

#if (defined(_IC7IF))
/**
 * Input Capture 7 Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_IC7 _IC7Interrupt, 0x0040, 1, 6, 5, 8              // Input capture channel 7
#endif

#if (defined(_IC8IF))
/**
 * Input Capture 8 Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_IC8 _IC8Interrupt, 0x0042, 1, 7, 5, 12             // Input capture channel 8
#endif

#if (defined(_IC9IF))
/**
 * Input Capture 9 Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_IC9 _IC9Interrupt, 0x00CE, 5, 13, 23, 4              // Input capture channel 9
#endif

#if (defined(_IC10IF))
/**
 * Input Capture 10 Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_IC10 _IC10Interrupt, 0x0010E, 7, 13, 31, 4              // Input capture channel 10
#endif

#if (defined(_IC11IF))
/**
 * Input Capture 11 Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_IC11 _IC11Interrupt, 0x0112, 7, 15, 31, 12              // Input capture channel 11
#endif

#if (defined(_IC12IF))
/**
 * Input Capture 12 Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_IC12 _IC12Interrupt, 0x0116, 8, 1, 32, 4              // Input capture channel 12
#endif

#if (defined(_IC13IF))
/**
 * Input Capture 13 Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_IC13 _IC13Interrupt, 0x0122, 8, 7, 33, 12              // Input capture channel 13
#endif

#if (defined(_IC14IF))
/**
 * Input Capture 14 Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_IC14 _IC14Interrupt, 0x0126, 8, 9, 34, 4              // Input capture channel 14
#endif

#if (defined(_IC15IF))
/**
 * Input Capture 15 Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_IC15 _IC15Interrupt, 0x012A, 8, 11, 34, 12              // Input capture channel 15
#endif

#if (defined(_IC16IF))
/**
 * Input Capture 16 Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_IC16 _IC16Interrupt, 0x012E, 8, 13, 35, 4              // Input capture channel 16
#endif

/**********************************************************************
*** OUTPUT COMPARES
**********************************************************************/
#if (defined(_OC1IF))
/**
 * Output Compare 1 Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_OC1 _OC1Interrupt, 0x0018, 0, 2, 0, 8              // Output compare channel 1
#endif

#if (defined(_OC2IF))
/**
 * Output Compare 2 Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_OC2 _OC2Interrupt, 0x0020, 0, 6, 1, 8              // Output compare channel 2
#endif

#if (defined(_OC3IF))
/**
 * Output Compare 3 Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_OC3   _OC3Interrupt,    0x0046, 1,  9,  6,  4 // Output Compare 3
#endif

#if (defined(_OC4IF))
/**
 * Output Compare 4 Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_OC4   _OC4Interrupt,    0x0048, 1,  10, 6,  8 // Output Compare 4
#endif

#if (defined(_OC5IF))
/**
 * Output Compare 5 Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_OC5 _OC5Interrupt, 0x0066, 2, 9, 10, 4              // Output compare channel 5
#endif

#if (defined(_OC6IF))
/**
 * Output Compare 6 Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_OC6 _OC6Interrupt, 0x0068, 2, 10, 10, 8              // Output compare channel 6
#endif

#if (defined(_OC7IF))
/**
 * Output Compare 7 Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_OC7 _OC7Interrupt, 0x006A, 2, 11, 10, 12              // Output compare channel 7
#endif

#if (defined(_OC8IF))
/**
 * Output Compare 8 Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_OC8 _OC8Interrupt, 0x006C, 2, 12, 11, 0              // Output compare channel 8
#endif

#if (defined(_OC9IF))
/**
 * Output Compare 9 Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_OC9 _OC9Interrupt, 0x00CC, 5, 12, 23, 0              // Output compare channel 9
#endif

#if (defined(_OC10IF))
/**
 * Output Compare 10 Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_OC10 _OC10Interrupt, 0x010C, 7, 12, 31, 0              // Output compare channel 10
#endif

#if (defined(_OC11IF))
/**
 * Output Compare 11 Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_OC11 _OC11Interrupt, 0x0110, 7, 14, 31, 8              // Output compare channel 11
#endif

#if (defined(_OC12IF))
/**
 * Output Compare 12 Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_OC12 _OC12Interrupt, 0x0114, 8, 0, 32, 0              // Output compare channel 12
#endif

#if (defined(_OC13IF))
/**
 * Output Compare 13 Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_OC13 _OC13Interrupt, 0x0120, 8, 6, 33, 8              // Output compare channel 13
#endif

#if (defined(_OC14IF))
/**
 * Output Compare 14 Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_OC14 _OC14Interrupt, 0x0124, 8, 8, 34, 0              // Output compare channel 14
#endif

#if (defined(_OC15IF))
/**
 * Output Compare 15 Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_OC15 _OC15Interrupt, 0x0128, 8, 10, 34, 8              // Output compare channel 15
#endif

#if (defined(_OC16IF))
/**
 * Output Compare 16 Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_OC16 _OC16Interrupt, 0x012C, 8, 12, 35, 0              // Output compare channel 16
#endif

/**********************************************************************
*** TIMERS
**********************************************************************/
// NOTE: TIMER 1 interrupt descriptor is NOT defined as ESOS
//       uses this timer for the system tick!

#if (defined(_T2IF))
/**
 * Timer 2 Expiration Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_T2  _T2Interrupt, 0x0022, 0, 7, 1, 12              // Timer 2
#endif

#if (defined(_T3IF))
/**
 * Timer 3 Expiration Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_T3     _T3Interrupt, 0x0024, 0, 8, 2, 0               // Timer 3
#endif

#if (defined(_T4IF))
/**
 * Timer 4 Expiration Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_T4    _T4Interrupt,   0x004A, 1,  11, 6,  12  // Timer4
#endif

#if (defined(_T5IF))
/**
 * Timer 5 Expiration Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_T5    _T5Interrupt,   0x004C, 1,  12, 7,  0 // Timer5
#endif

#if (defined(_T6IF))
/**
 * Timer 6 Expiration Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_T6     _T6Interrupt, 0x0072, 2, 15, 11, 12               // Timer 6
#endif

#if (defined(_T7IF))
/**
 * Timer 7 Expiration Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_T7     _T7Interrupt, 0x0074, 3, 0, 12, 0               // Timer 7
#endif

#if (defined(_T8IF))
/**
 * Timer 8 Expiration Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_T8     _T8Interrupt, 0x007A, 3, 3, 12, 12               // Timer 8
#endif

#if (defined(_T9IF))
/**
 * Timer 9 Expiration Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_T9     _T9Interrupt, 0x007C, 3, 4, 13, 0               // Timer 9
#endif

/**********************************************************************
*** DMA CHANNELS
**********************************************************************/
#if (defined(_DMA0IF))
/**
 * Direct Memory Access (DMA) 0 Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_DMA0  _DMA0Interrupt,   0x001A, 0,  4,  1,  0 // DMA Channel 0
#endif

#if (defined(_DMA1IF))
/**
 * Direct Memory Access (DMA) 1 Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_DMA1  _DMA1Interrupt,   0x0030, 0,  14, 3,  8 // DMA Channel 1
#endif

#if (defined(_DMA2IF))
/**
 * Direct Memory Access (DMA) 2 Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_DMA2  _DMA2Interrupt,   0x0044, 1,  8,  6,  0 // DMA Channel 2
#endif

#if (defined(_DMA3IF))
/**
 * Direct Memory Access (DMA) 3 Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_DMA3  _DMA3Interrupt,   0x005C, 2,  4,  9,  0 // DMA Channel 3
#endif

#if (defined(_DMA4IF))
/**
 * Direct Memory Access (DMA) 4 Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_DMA4  _DMA4Interrupt,   0x0070, 2,  14,  11,  8 // DMA Channel 4
#endif

#if (defined(_DMA5IF))
/**
 * Direct Memory Access (DMA) 5 Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_DMA5  _DMA5Interrupt,   0x008E, 3,  13,  15,  4 // DMA Channel 5
#endif

#if (defined(_DMA6IF))
/**
 * Direct Memory Access (DMA) 6 Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_DMA6  _DMA6Interrupt,   0x009C, 4,  4,  17,  0 // DMA Channel 6
#endif

#if (defined(_DMA7IF))
/**
 * Direct Memory Access (DMA) 7 Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_DMA7  _DMA7Interrupt,   0x009E, 4,  5,  17,  4 // DMA Channel 7
#endif

#if (defined(_DMA8IF))
/**
 * Direct Memory Access (DMA) 8 Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_DMA8  _DMA8Interrupt,   0x0100, 7,  6,  29,  8 // DMA Channel 8
#endif

#if (defined(_DMA9IF))
/**
 * Direct Memory Access (DMA) 9 Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_DMA9  _DMA9Interrupt,   0x0102, 7,  7,  29,  12 // DMA Channel 9
#endif

#if (defined(_DMA10IF))
/**
 * Direct Memory Access (DMA) 10 Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_DMA10  _DMA10Interrupt,   0x0104, 7,  8,  30,  0 // DMA Channel 10
#endif

#if (defined(_DMA11IF))
/**
 * Direct Memory Access (DMA) 11 Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_DMA11  _DMA11Interrupt,   0x0106, 7,  9,  30,  4 // DMA Channel 11
#endif

#if (defined(_DMA12IF))
/**
 * Direct Memory Access (DMA) 12 Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_DMA12  _DMA12Interrupt,   0x0118, 8,  2,  32,  8 // DMA Channel 12
#endif

#if (defined(_DMA13IF))
/**
 * Direct Memory Access (DMA) 13 Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_DMA13  _DMA13Interrupt,   0x011A, 8,  3,  32,  12 // DMA Channel 13
#endif

#if (defined(_DMA14IF))
/**
 * Direct Memory Access (DMA) 14 Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_DMA14  _DMA14Interrupt,   0x011C, 8,  4,  33,  0 // DMA Channel 14
#endif

/**********************************************************************
*** SERIAL PERIPHERAL INTERFACE (SPI)
**********************************************************************/
#if (defined(_SPI1IF))
/**
 * Serial Peripheral Interface 1 Transfer Done Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_SPI1  _SPI1Interrupt, 0x0028, 0, 10, 2, 8            // SPI1 event

/**
 * Serial Peripheral Interface 1 Error Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_SPI1E _SPI1ErrInterrupt, 0x0026, 0, 9, 2, 4          // SPI1 (exception) fault event
#endif

#if (defined(_SPI2F))
/**
 * Serial Peripheral Interface 2 Transfer Done Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_SPI2  _SPI2Interrupt,   0x0056, 2,  1,  8,  4 // SPI2 Transfer Done
/**
 * Serial Peripheral Interface 2 Error Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_SPI2E _SPI2ErrInterrupt,  0x0054, 2,  0,  8,  0 // SPI2 Error
#endif

#if (defined(_SPI3F))
/**
 * Serial Peripheral Interface 3 Transfer Done Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_SPI3  _SPI3Interrupt,   0x00CA, 5,  11,  22,  12 // SPI3 Transfer Done
/**
 * Serial Peripheral Interface 3 Error Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_SPI3E _SPI3ErrInterrupt,  0x00C8, 5,  10,  22,  8 // SPI3 Error
#endif

#if (defined(_SPI4F))
/**
 * Serial Peripheral Interface 4 Transfer Done Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_SPI4  _SPI4Interrupt,   0x010A, 7,  11,  30,  12 // SPI4 Transfer Done
/**
 * Serial Peripheral Interface 4 Error Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_SPI24 _SPI4ErrInterrupt,  0x0108, 7,  10,  30,  8 // SPI4 Error
#endif

/**********************************************************************
*** UNIVERSAL ASYNCHRONOUS RECEIVER TRANSMITTER (UART)
**********************************************************************/
// if the user is __NOT__  using the built-in ESOS comm system
// but __IS__ using user IRQs, then we should define the UART
// IRQs for their use
#if !defined(_ESOS_PIC24_RS232_H)  || defined(__DOXYGEN__)
/**
 * UART1 TX Event Interrupt (User interrupt descriptor)
 * \note Only defined if ESOS is built <em>without</em> the communications subsystem.
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_U1TX  _U1TXInterrupt, 0x002C, 0, 12, 3, 0            // UART1 TX event
/**
 * UART1 RX Event Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \note Only defined if ESOS is built <em>without</em> the communications subsystem.
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_U1RX  _U1RXInterrupt, 0x002A, 0, 11, 2, 12           // UART1 RX event

/**
 * UART1 Error Event Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \note Only defined if ESOS is built <em>without</em> the communications subsystem.
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_U1E _U1ErrInterrupt, 0x0096, 4, 1, 16, 4           // UART1 Error event
#endif      // end of UART1 constants

#if (defined(_U2TXIF))
/**
 * UART 2 Transmit Done Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_U2TX  _U2TXInterrupt,   0x0052, 1,  15, 7,  12  // UART2 Transmitter
/**
 * UART 2 Receive Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_U2RX  _U2RXInterrupt,   0x0050, 1,  14, 7,  8 // UART2 Receiver
/**
 * UART 2 Error Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_U2E   _U2ErrInterrupt,  0x0098, 4,  2,  16, 8 // UART2 Error Interrupt
#endif

#if (defined(_U3TXIF))
/**
 * UART 3 Transmit Done Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_U3TX  _U3TXInterrupt,   0x00BA, 5,  3, 20,  12  // UART3 Transmitter
/**
 * UART 3 Receive Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_U3RX  _U3RXInterrupt,   0x00B8, 5,  2, 20,  8 // UART3 Receiver
/**
 * UART 3 Error Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_U3E   _U3ErrInterrupt,  0x00B6, 5,  1,  20, 4 // UART3 Error Interrupt
#endif

#if (defined(_U4TXIF))
/**
 * UART 4 Transmit Done Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_U4TX  _U4TXInterrupt,   0x00C6, 5,  9, 22, 4  // UART4 Transmitter
/**
 * UART 4 Receive Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_U4RX  _U4RXInterrupt,   0x00C4, 5, 8, 22, 0 // UART4 Receiver
/**
 * UART 4 Error Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_U4E   _U4ErrInterrupt,  0x00C2, 5, 7, 21, 12 // UART4 Error Interrupt
#endif

/**********************************************************************
*** ANALOG-TO-DIGITAL CONVERTER (A/D)
**********************************************************************/
#if (defined(_AD1IF))
/**
 * A/D 1 Conversion Complete Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_AD1 _ADC1Interrupt, 0x002E, 0, 13, 3, 4            // AD1 Conversion complete
#endif
#if (defined(_AD2IF))
/**
 * A/D 2 Conversion Complete Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_AD2 _ADC2Interrupt, 0x003E, 1, 5, 5, 4            // AD2 Conversion complete
#endif

/**********************************************************************
*** NON-VOLATILE MEMORY (NVM) WRITE COMPLETE
**********************************************************************/
// TODO:  create this user interrupt descriptor

/**********************************************************************
*** INTER-INTEGRATED CIRCUIT (I2C)
**********************************************************************/
#if (defined(_I2C1IF))
/**
 * I2C1 Master Event Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_MI2C1 _MI2C1Interrupt, 0x0036, 1, 1, 4, 4            // I2C1 Master event
/**
 * I2C1 Slave Event Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_SI2C1 _SI2C1Interrupt, 0x0034, 1, 0, 4, 0            // I2C1 slave event
#endif

#if (defined(_I2C2IF))
/**
 * I2C2 Master Event Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_MI2C2 _MI2C2Interrupt, 0x0078, 3, 2, 12, 8            // I2C2 Master event
/**
 * I2C2 Slave Event Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_SI2C2 _SI2C2Interrupt, 0x0076, 3, 1, 12, 4            // I2C2 slave event
#endif

/**********************************************************************
*** COMPARATOR
**********************************************************************/
#if (defined(_CMIF))
/**
 * Analog comparator Event Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_CM    _CMInterrupt,   0x0038, 1,  2,  4,  8 // Comparator Combined Event
#endif

/**********************************************************************
*** CHANGE NOTIFICATION
**********************************************************************/
#if (defined(_CNIF))
/**
 * Change Notification Event Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_CN    _CNInterrupt,   0x003A, 1,  3,  4,  12  // Input Change Interrupt
#endif

/**********************************************************************
*** ENHANCED CONTROLLER AREA NETWORK (ECAN)
**********************************************************************/
#if (defined(_C1IF))
/**
 * ECAN 1 Event Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_C1    _C1Interrupt,   0x005A, 2,  3,  8,  12  // CAN1 Event
/**
 * ECAN 1 Transmit Date Request Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_C1TX  _C1TXInterrupt,   0x00A0, 4,  6,  17, 8 // CAN1 TX Data Request
/**
 * ECAN 1 Receive Data Ready Event Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_C1RX  _C1RXInterrupt,   0x0058, 2,  2,  8,  8 // CAN1 RX Data Ready
#endif

#if (defined(_C2IF))
/**
 * ECAN 2 Event Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_C2    _C2Interrupt,   0x0084, 3,  8,  14,  0  // CAN2 Event
/**
 * ECAN 2 Transmit Date Request Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_C2TX  _C2TXInterrupt,   0x00A2, 4,  7,  17, 12 // CAN2 TX Data Request
/**
 * ECAN 2 Receive Data Ready Event Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_C2RX  _C2RXInterrupt,   0x0082, 3,  7,  13,  12 // CAN2 RX Data Ready
#endif

/**********************************************************************
*** PARALLEL MASTER PORT (PMP)
**********************************************************************/
// TODO:  add this user interrupt descriptor


/**********************************************************************
*** PULSE-WIDTH MODULATION (PWM)
**********************************************************************/
#if (defined(_PSEMIF))
/**
 * Pulse-width modulation (PWM) special event Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_PSEM  _PSEMInterrupt,   0x0086, 3,  9,  14, 4 // PWM Special Event Match
#endif

// TODO: write PSESM (PWM Secondary Special Event) interrupt descriptor

#if (defined(_PWM1IF))
/**
 * Pulse-width modulation (PWM) Generator 1 Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_PWM1  _PWM1Interrupt,   0x00D0, 5,  14, 23, 8
#endif

#if (defined(_PWM2IF))
/**
 * Pulse-width modulation (PWM) Generator 2 Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_PWM2  _PWM2Interrupt,   0x00D2, 5,  15, 23, 12
#endif

#if (defined(_PWM3IF))
/**
 * Pulse-width modulation (PWM) Generator 3 Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_PWM3  _PWM3Interrupt,   0x00D4, 6,  0,  24, 0
#endif

#if (defined(_PWM4IF))
/**
 * Pulse-width modulation (PWM) Generator 4 Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_PWM4  _PWM4Interrupt,   0x00D6, 6,  1,  24, 4
#endif

#if (defined(_PWM5IF))
/**
 * Pulse-width modulation (PWM) Generator 5 Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_PWM5  _PWM5Interrupt,   0x00D8, 6,  2,  24, 8
#endif

#if (defined(_PWM6IF))
/**
 * Pulse-width modulation (PWM) Generator 6 Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_PWM6  _PWM6Interrupt,   0x00DA, 6,  3,  24, 12
#endif

#if (defined(_PWM7IF))
/**
 * Pulse-width modulation (PWM) Generator 7 Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_PWM7  _PWM7Interrupt,   0x00DC, 6,  4,  25, 0
#endif

/**********************************************************************
*** QUADRATURE ENCODER INTERFACE (QEI)
**********************************************************************/
#if (defined(_QEI1IF))
/**
 * Quadrature encoder interface module counter compare event Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_QEI1  _QEI1Interrupt,   0x0088, 3,  10, 14, 8
#endif

#if (defined(_QEI2IF))
/**
 * Quadrature encoder interface module counter compare event Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_QEI2  _QEI2Interrupt,   0x00AA, 4,  11, 18, 12
#endif

/**********************************************************************
*** UNIVERSAL SERIAL BUS (USB) ON-THE-GO (OTG)
**********************************************************************/
// TODO: add this user interrupt descriptor
#if (defined(_USB1IF))
#endif

/**********************************************************************
*** REAL-TIME CLOCK CALENDAR
**********************************************************************/
#if (defined(_RTCIF))
/**
 * Cyclic redundancy check generator Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_RTC   _RTCInterrupt,    0x0090, 3,  14,  15,  8  // RTCC Interrupt
#endif


/**********************************************************************
*** CYCLIC REDUNDANCY CHECKER (CRC)
**********************************************************************/
#if (defined(_CRCIF))
/**
 * Cyclic redundancy check generator Interrupt (User interrupt descriptor)
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 * \hideinitializer
 */
#define  ESOS_IRQ_PIC24_CRC   _CRCInterrupt,    0x009A, 4,  3,  16, 12  // CRC Generator Interrupt
#endif

#if 0
// some missing interrupts
DCIE
DCI

// some leftover interrupt descriptors from the change (JWB -- 6 SEPT 2014)
#define  ESOS_IRQ_PIC24_CRC   _CRCInterrupt,    0x009A, 4,  3,  16, 12  // CRC Generator Interrupt
#define  ESOS_IRQ_PIC24_CTMU  _CTMUInterrupt,   0x00AE, 4,  13, 19, 4 // CTMU Interrupt
#define  ESOS_IRQ_PIC24_ICD   _ICDInterrupt,    0x0142, 8,  14, 35, 8 // ICD Application
#define  ESOS_IRQ_PIC24_JTAG  _JTAGInterrupt,   0x0130, 8,  15, 35, 12  // JTAG Programming
#define  ESOS_IRQ_PIC24_PTGSTEP _PTGSTEPInterrupt,  0x0136, 9,  1,  36, 4 // PTG Step
#define  ESOS_IRQ_PIC24_PTGWDT  _PTGWDTInterrupt, 0x0138, 9,  2,  36, 8 // PTG Watchdog Time-out
#define  ESOS_IRQ_PIC24_PTG0  _PTG0Interrupt,   0x013A, 9,  3,  36, 12  // PTG Interupt 0
#define  ESOS_IRQ_PIC24_PTG1  _PTG1Interrupt,   0x013C, 9,  4,  37, 0 // PTG Interrupt 1
#define  ESOS_IRQ_PIC24_PTG2  _PTG2Interrupt,   0x013E, 9,  5,  37, 4 // PTG Interrupt 2
#define  ESOS_IRQ_PIC24_PTG3  _PTG3Interrupt,   0x0140, 9,  6,  37, 8 // PTG Interrupt 3
#endif

/*
 * MACROs COMMON TO ALL PIC24 PROCESSORS
 */

#define __GET_IRQ_ATTRIB(Q)     __xGET_IRQ_ATTRIB(Q)
#define __GET_IVTQ(Q)           __xGET_IVT(Q)
#define __GET_IFS_NUM(Q)        __xGET_IFS_NUM(Q)
#define __GET_IFS_BITNUM(Q)     __xGET_IFS_BITNUM(Q)
#define __GET_IEC_NUM(Q)        __xGET_IFS_NUM(Q)
#define __GET_IEC_BITNUM(Q)     __xGET_IFS_BITNUM(Q)
#define __GET_IPC_NUM(Q)        __xGET_IPC_NUM(Q)
#define __GET_IPC_BITNUM(Q)     __xGET_IPC_BITNUM(Q)

#define __xGET_IRQ_ATTRIB(attrib, ivt, ifsr, ifsb, ipcr, ipcb)            attrib
#define __xGET_IVT(attrib, ivt, ifsr, ifsb, ipcr, ipcb)               ivt
#define __xGET_IFS_NUM(attrib, ivt, ifsr, ifsb, ipcr, ipcb)           ifsr
#define __xGET_IFS_BITNUM(attrib, ivt, ifsr, ifsb, ipcr, ipcb)        ifsb
#define __xGET_IEC_NUM(attrib, ivt, ifsr, ifsb, ipcr, ipcb)           ifsr
#define __xGET_IEC_BITNUM(attrib, ivt, ifsr, ifsb, ipcr, ipcb)        ifsb
#define __xGET_IPC_NUM(attrib, ivt, ifsr, ifsb, ipcr, ipcb)           ipcr
#define __xGET_IPC_BITNUM(attrib, ivt, ifsr, ifsb, ipcr, ipcb)        ipcb


// this macro gets the IRQ's IPL number from its corresponding IPC register
#define __GET_IPL_FROM_IPCX(Q)      ((*(&IPC0+(__xGET_IPC_NUM(Q))) >> __xGET_IPC_BITNUM(Q))&0x7)

// this macro tests the IRQ's IPL number from its corresponding IPC register
// against a value
#define __IS_IPL_FROM_IPCX(Q, val)    ((__GET_IPL_FROM_IPCX(Q))==(val))

// this macro sets the IRQ's IPL number in its corresponding IPC register
#define __PUT_IPL_INTO_IPCX(ipc,ipcb,ipl)                             \
    do {                                                            \
    BIT_CLEAR_MASK( (*((&IPC0)+(ipc))), ((0x07<<ipcb) + (ipl<<ipcb)));  \
    (*((&IPC0)+ipc)) += (ipl<<ipcb); \
      }while(0)

/**
* Unregisters the user interrupt denoted by the interrupt descriptor
* \param desc   Descriptor of the hardware interrupt
* \note Hardware interrupt descriptors are found in the file <b>esos_<i>hw</i>_irq.h</b>
* where <i>hw</i> is the name of the chip for the ESOS port has been written.
* \hideinitializer
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 */
#define  ESOS_UNREGISTER_PIC24_USER_INTERRUPT(desc)    __xUNREGISTER_PIC24_USER_INTERRUPT(desc)
#define  __xUNREGISTER_PIC24_USER_INTERRUPT(attrib, ivt, ifsr, ifsb, ipcr, ipcb)   \
  do{                                               \
  __xDISABLE_PIC24_USER_INTERRUPT(attrib, ivt, ifsr, ifsb, ipcr, ipcb);               \
  __PUT_IPL_INTO_IPCX(ipcr,ipcb, __ESOS_USER_IRQ_UNREGISTERED);      \
  }while(0)


/**
* Registers the user interrupt denoted by the interrupt descriptor
* \param desc   Descriptor of the hardware interrupt
* \param ipl     Interrupt Priority Level (IPL) for the interrupt being registered
* \param p2f        Pointer to function to serve as the ISR. Function must have <em>void</em>
* arguments and return <em>void</em>
* \note Hardware interrupt descriptors are found in the file <b>esos_<i>hw</i>_irq.h</b>
* where <i>hw</i> is the name of the chip for the ESOS port has been written.
* \hideinitializer
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 */
#define ESOS_REGISTER_PIC24_USER_INTERRUPT(desc, ipl, p2f)    __xREGISTER_PIC24_USER_INTERRUPT(desc, ipl, p2f)
#define __xREGISTER_PIC24_USER_INTERRUPT(attrib, ivt, ifsr, ifsb, ipcr, ipcb, ipl, p2f)    \
   do {                                               \
      __xDISABLE_PIC24_USER_INTERRUPT(attrib, ivt, ifsr, ifsb, ipcr, ipcb);   \
      __PUT_IPL_INTO_IPCX(ipcr, ipcb, ipl);   \
   }while(0)

/**
* Disables all registered user interrupts.
* \note This function does <em>NOT</em> disable all interrupts.  Interrupts used by
* ESOS will continue to function.
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
* \hideinitializer
*/
#define  ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS()     SET_CPU_IPL(__ESOS_DISABLE_USER_IRQS_LEVEL)

/**
* Enables all registered user interrupts.
* \note Does not affect interrupts used by ESOS
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
* \hideinitializer
*/
#define  ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS()      SET_CPU_IPL(__ESOS_ENABLE_USER_IRQS_LEVEL)

/**
* Queries the enabled state of user interrupt denoted by the interrupt descriptor
* \param desc   Descriptor of the hardware interrupt
* \retval TRUE    If the user interrupt is currently enabled
* \retval FALSE    If the user interrupt is currently disabled
* \note Hardware interrupt descriptors are found in the file <b>esos_<i>hw</i>_irq.h</b>
* where <i>hw</i> is the name of the chip for the ESOS port has been written.
* \hideinitializer
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 */
#define  ESOS_IS_PIC24_USER_INTERRUPT_ENABLED(desc)     __xIS_PIC24_USER_INTERRUPT_ENABLED(desc)
#define  __xIS_PIC24_USER_INTERRUPT_ENABLED(attrib, ivt, ifsr, ifsb, ipcr, ipcb)    IS_BIT_SET(*(&IEC0+ifsr),ifsb)

/**
* Queries the "flag" state of user interrupt denoted by the interrupt descriptor
* \param desc   Descriptor of the hardware interrupt
* \retval TRUE    If the user interrupt is needs to be serviced
* \retval FALSE    Otherwise
* \note Hardware interrupt descriptors are found in the file <b>esos_<i>hw</i>_irq.h</b>
* where <i>hw</i> is the name of the chip for the ESOS port has been written.
* \hideinitializer
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
*/
#define  ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING(desc)    __xDOES_PIC24_USER_INTERRUPT_NEED_SERVICING(desc)
#define  __xDOES_PIC24_USER_INTERRUPT_NEED_SERVICING(attrib, ivt, ifsr, ifsb, ipcr, ipcb)     IS_BIT_SET(*(&IFS0+ifsr), ifsb)

/**
* Tells ESOS to mark the user interrupt denoted by the interrupt descriptor as being serviced
* \param desc   Descriptor of the hardware interrupt
* \note Hardware interrupt descriptors are found in the file <b>esos_<i>hw</i>_irq.h</b>
* where <i>hw</i> is the name of the chip for the ESOS port has been written.
* \hideinitializer
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
*/
#define  ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED(desc)   __xMARK_PIC24_USER_INTERRUPT_SERVICED(desc)
#define  __xMARK_PIC24_USER_INTERRUPT_SERVICED(attrib, ivt, ifsr, ifsb, ipcr, ipcb)            BIT_CLEAR(*(&IFS0+ifsr),ifsb)

/**
* Enables the user interrupt denoted by the interrupt descriptor
* \param desc   Descriptor of the hardware interrupt
* \note Hardware interrupt descriptors are found in the file <b>esos_<i>hw</i>_irq.h</b>
* where <i>hw</i> is the name of the chip for the ESOS port has been written.
* \hideinitializer
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_DISABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 */
#define   ESOS_ENABLE_PIC24_USER_INTERRUPT(desc)      __xENABLE_PIC24_USER_INTERRUPT(desc)
#define  __xENABLE_PIC24_USER_INTERRUPT(attrib, ivt, ifsr, ifsb, ipcr, ipcb)     BIT_SET(*(&IEC0+ifsr), ifsb)

/**
* Disables the user interrupt denoted by the interrupt descriptor
* \param desc   Descriptor of the hardware interrupt
* \note Hardware interrupt descriptors are found in the file <b>esos_<i>hw</i>_irq.h</b>
* where <i>hw</i> is the name of the chip for the ESOS port has been written.
* \hideinitializer
 * \sa ESOS_USER_INTERRUPT
 * \sa ESOS_REGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_UNREGISTER_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_PIC24_USER_INTERRUPT
 * \sa ESOS_ENABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_DISABLE_ALL_PIC24_USER_INTERRUPTS
 * \sa ESOS_IS_PIC24_USER_INTERRUPT_ENABLED
 * \sa ESOS_DOES_PIC24_USER_INTERRUPT_NEED_SERVICING
 * \sa ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED
 */
#define  ESOS_DISABLE_PIC24_USER_INTERRUPT(desc)       __xDISABLE_PIC24_USER_INTERRUPT(desc)
#define  __xDISABLE_PIC24_USER_INTERRUPT(attrib, ivt, ifsr, ifsb, ipcr, ipcb)       BIT_CLEAR(*((&IEC0)+ifsr), ifsb)

#endif          // ESOS_PIC24_IRQ_H


