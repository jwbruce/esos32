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
 *  hwxxx Family specific communications on ESOS
 */


#ifndef _ESOS_HWXXX_RS232_H
#define _ESOS_HWXXX_RS232_H

/* I N C L U D E S **********************************************************/
#include "esos.h"
#include "esos_comm.h"
#include "esos_hwxxx.h"

/* D E F I N I T I O N S ****************************************************/

/* E X T E R N S ************************************************************/

/* M A C R O S **************************************************************/

/* P U B L I C  P R O T O T Y P E S *****************************************/
void    __esos_hw_signal_start_tx(void);
void    __esos_hw_signal_stop_tx(void);

// Documentation for this file. If the \file tag isn't present,
// this file won't be documented.
/** \file
 *  This file contains routines which configure and
 *  use the UARTs on the HWXXX processor. .
 */

/* ########################################################################### */
/** Determine if a character is available in the UART's
 *  receive buffer.
 *  \return True (1) if character is available, 0 if not.
 */
#define IS_CHAR_READY_UART1()   U1STAbits.URXDA

/** Determine if a the transmit buffer is full.
 *  \return True (1) if the transmit buffer if full,
 *          false (0) if not.
 */
#define IS_TRANSMIT_BUFFER_FULL_UART1()   U1STAbits.UTXBF

/** Determines if all characters placed in the UART have been sent.
 *  Returns 1 if the last transmission has completed, or 0 if a transmission
 *  is in progress or queued in the transmit buffer.
 *  \return True (1) if the last transmission has completed, 0 if not.
 */
#define IS_TRANSMIT_COMPLETE_UART1() U1STAbits.TRMT

// communications commands used outside of ESOS tasks (like user_init routine)
// these routines/macros should almost never be used.
void __esos_hw_PutUint8(uint8_t u8_c);
void __esos_hw_PutString(uint8_t* psz_in);
uint8_t __esos_hw_GetUint8(void);

void __esos_configUART1(uint32_t u32_baudRate);
void __esos_hw_InitCommSystem(void);


/** Waits until all characters placed in the UART have been sent. */
inline static void WAIT_UNTIL_TRANSMIT_COMPLETE_UART1() {
  while (!IS_TRANSMIT_COMPLETE_UART1())
    doHeartbeat();
}



#endif // end ESOS_HWXXX_RS232_H
