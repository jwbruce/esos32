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
 * \brief Hardware independent code for ESOS32 hardware interrupts support
 *
 */



/************************************************************************
 * esos_irq.c
 ************************************************************************
 * ES_OS file for (hardware independent) IRQ routines
 */

#include    "esos_irq.h"

void    (*__esos_IsrFcns[NUM_USER_IRQS])(void);

void    esos_InitUserInterrupts(void) {
  uint8_t        i;

  // initialize the os ISR fcn ptrs to point to a benign ISR
  for (i=0; i<NUM_USER_IRQS; i++) {
    esos_UnregisterUserInterrupt(i);
  }
  _esos_hw_InitUserInterrupts();
} // end osInitInterrupts()

void  esos_UnregisterUserInterrupt( uint8_t u8IrqIndex ) {
  // disassociate and unregister the p2f function with the IRQ given
  // in u8IrqIndex
  if (u8IrqIndex < NUM_USER_IRQS) {
    esos_DisableUserInterrupt( u8IrqIndex );
    __esos_IsrFcns[u8IrqIndex] = _esos_DoNothingIsr;
  }  // end if
} // end osUnregisterUserIsr()

void  esos_RegisterUserInterrupt( uint8_t u8IrqIndex, void (*p2f)(void) ) {
  // associate and register the p2f function with the IRQ given
  // in u8IrqIndex
  if (u8IrqIndex < NUM_USER_IRQS) {
    __esos_IsrFcns[u8IrqIndex] = p2f;
  }
} // end osRegisterUserIsr()

void  esos_EnableVerifiedUserInterrupt( uint8_t u8IrqIndex ) {
  // do ESOS checks on UserInterrupt and if all is well
  // call the user-provided HW specific routine
  if ((__esos_IsrFcns[u8IrqIndex] != NULLPTR) && ( __esos_IsrFcns[u8IrqIndex] != _esos_DoNothingIsr))
    _esos_hw_EnableUserInterrupt( u8IrqIndex );
} // end esos_EnableUserInterrupt()

void    esos_ExecuteUserIsr( uint8_t u8IrqIndex ) {
  __esos_IsrFcns[u8IrqIndex]();
} // esos_ExecuteUserIsr

void _esos_DoNothingIsr(void) {
  _esos_hw_DoNothingIsr();
} // end _esos_DoNothingIsr()
