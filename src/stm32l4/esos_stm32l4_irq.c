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

/************************************************************************
 * esos_pic24_irq.c
 ************************************************************************
 * User provided (HW-specific) ES_OS routines for individual IRQs
 *
 * NOTE:  The actual handler ISR (the one that the CPU will vector to
 *        does NOT go in this file.  The real HW handler for the user
 *        IRQs goes in the file esos_p18_uisr.c
 *
 *        This allows us to emulate the p18 hardware on PCs since
 *        goofy INTERRUPT keywords are kept away from this code.
 */

#include    "esos_pic24_irq.h"

/*
 * User must provide a routine to setup the user (low-priority) IRQs
 *   This routine must only init the user IRQs.... It can not disable
 *   or other mess with IRQs beyond its purview because they may be
 *   being used to generate the system tick.
 */
void    _esos_hw_InitUserInterrupts(void) {
  // Set all possible user IRQs to the lowest priority (0)
  // However, ESOS may use a few IRQs for its own purposes, so
  // preserve them.
  //
  // At this time, ESOS for PIC24 family uses T1, U1TX, U1RX
  uint8_t   u8_T1IP, u8_U1TXIP, u8_U1RXIP;
  uint8_t   u8_i;
  uint16_t*   pu16_IpcPtr;

  u8_T1IP = _T1IP;
  u8_U1RXIP = _U1RXIP;
  u8_U1TXIP = _U1TXIP;

  // Set all IRQs to 0 (ZERO) IP level
  pu16_IpcPtr = (uint16_t*) &IPC0;
  for (u8_i=0; u8_i<=17; u8_i++) {
    *pu16_IpcPtr=0;
    pu16_IpcPtr++;
  }

  // restore the ESOS IRQ IP levels
  _T1IP = u8_T1IP;
  _U1RXIP = u8_U1RXIP;
  _U1TXIP = u8_U1TXIP;

} // end _esos_hw_InitUserInterrupts()

void _esos_hw_DoNothingIsr(void) {
  /*
  DO NOTHING.... DON'T CALL PRINTF.
  It makes the image HUGE!!!
  printf("Doing nothing, of course!\n\r");
  printf("PIR1 register is:    %x\n\r", PIR1);
  printf("PIR2 register is:    %x\n\r", PIR2);
  printf("INTCON3 register is: %x\n\r", INTCON3);
  */
  //BIT_CLEAR( INTCON3, 0);     // INTCON3bits.INT1IF = 0;

}  // end _esos_hw_DoNothingIsr()
