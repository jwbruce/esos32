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

#ifndef _ESOS_PC_STDIO_H
#define _ESOS_PC_STDIO_H

/* I N C L U D E S **********************************************************/
#include "esos.h"
#include "esos_comm.h"
#include "esos_pc.h"

/* D E F I N I T I O N S ****************************************************/

/* E X T E R N S ************************************************************/
extern struct termios stored_settings;

/* M A C R O S **************************************************************/

/* P U B L I C  P R O T O T Y P E S *****************************************/
void    __esos_hw_signal_start_tx(void);
void    __esos_hw_signal_stop_tx(void);
uint8_t   kbhit(void);
void set_keypress(void);

// Documentation for this file. If the \file tag isn't present,
// this file won't be documented.
/*
 *  This file contains routines which configure and
 *  use STDIO on the PC for communications.
 */

// communications commands used outside of ESOS tasks (like user_init routine)
// these routines/macros should almost never be used.
void __esos_hw_PutUint8(uint8_t u8_c);
void __esos_hw_PutString(uint8_t* psz_in);
uint8_t __esos_hw_GetUint8(void);

void __esos_hw_InitCommSystem(void);

#endif // end ESOS_PC_STDIO_H
