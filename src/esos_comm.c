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
 * \brief Hardware independent code for ESOS32 Communications service
 *
 */


#include    "esos.h"
#include  	"esos_cb.h"
#include    "esos_comm.h"

// ******** G L O B A L S ***************
volatile uint8_t                __esos_comm_tx_buff[ESOS_SERIAL_IN_EP_SIZE];
volatile uint8_t                __esos_comm_rx_buff[ESOS_SERIAL_OUT_EP_SIZE];
CBUFFER                   __st_CB_Tx, __st_CB_Rx;
CBUFFER*                    __pst_CB_Tx;
CBUFFER*              __pst_CB_Rx;
volatile struct    stTask   __stChildTaskTx, __stChildTaskRx;

/****************************************************************
** F U N C T I O N S
****************************************************************/
void __esos_InitCommSystem(void) {
  // setup the circular buffers & their descriptors
  __pst_CB_Rx = &__st_CB_Rx;
  __pst_CB_Tx = &__st_CB_Tx;
  __esos_CB_Init( __pst_CB_Tx, __esos_comm_tx_buff, ESOS_SERIAL_IN_EP_SIZE);
  __esos_CB_Init( __pst_CB_Rx, __esos_comm_rx_buff, ESOS_SERIAL_OUT_EP_SIZE);

  __esos_hw_InitCommSystem();

} // endof esos_Init_CommSystem()

uint8_t __esos_u8_GetMSBHexCharFromUint8(uint8_t u8_x) {
  uint8_t u8_c;

  u8_c = (u8_x>>4)& 0xf;
  if (u8_c > 9) return('A'+u8_c-10);
  else return('0'+u8_c);
}

uint8_t __esos_u8_GetLSBHexCharFromUint8(uint8_t u8_x) {
  uint8_t u8_c;

  u8_c = u8_x & 0xf;
  if (u8_c > 9) return('A'+u8_c-10);
  else return('0'+u8_c);
}


ESOS_CHILD_TASK( __esos_OutChar, uint8_t u8_c) {
  static uint8_t        u8_localChar;

  ESOS_TASK_BEGIN();
  u8_localChar = u8_c;

  // wait for room in the TX CB to appear
  ESOS_TASK_WAIT_WHILE_CB_IS_FULL( __pst_CB_Tx );
  // write the data in the TX CB
  __esos_CB_WriteUINT8( __pst_CB_Tx, u8_localChar);
  // signal the hardware that a xfer should be started if
  // not already ongoing
  __esos_hw_signal_start_tx();

  ESOS_TASK_END();
} // end __esos_OutChar


ESOS_CHILD_TASK( __esos_OutUint8AsHexString, uint8_t u8_x) {
  static uint8_t        au8_String[5];

  ESOS_TASK_BEGIN();
  au8_String[0] = '0';
  au8_String[1] = 'x';
  au8_String[2] = __esos_u8_GetMSBHexCharFromUint8(u8_x);
  au8_String[3] = __esos_u8_GetLSBHexCharFromUint8(u8_x);
  au8_String[4] = 0;

  ESOS_TASK_WAIT_UNTIL_CB_HAS_AVAILABLE_AT_LEAST( __pst_CB_Tx, 4);
  __esos_CB_WriteUINT8Buffer( __pst_CB_Tx, &au8_String[0], 4 );
  // signal the hardware that a xfer should be started if
  // not already ongoing
  __esos_hw_signal_start_tx();
  ESOS_TASK_END();

} // end __esos_OutUint8AsHexString()

ESOS_CHILD_TASK( __esos_OutUint8AsDecString, uint8_t u8_x) {
  // code provided by Gary Weasel
  static uint8_t      au8_String[5];
  static uint8_t      u8_c;
  static uint8_t      u8_digit;


  ESOS_TASK_BEGIN();
  u8_digit = 0;
  if (u8_x > 99)
    au8_String[u8_digit++] = '0' + u8_x / 100;
  if (u8_x > 9)
    au8_String[u8_digit++] = '0' + (u8_x % 100) / 10;
  au8_String[u8_digit++] = '0' + (u8_x % 10);
  au8_String[u8_digit] = 0;
  u8_c = 0;
  while (u8_c < u8_digit) {
    // wait for room in the TX CB to appear
    ESOS_TASK_WAIT_WHILE_CB_IS_FULL( __pst_CB_Tx );
    // write the data in the TX CB
    __esos_CB_WriteUINT8( __pst_CB_Tx, au8_String[u8_c++] );
  } //end while()
  // signal the hardware that a xfer should be started if
  // not already ongoing
  __esos_hw_signal_start_tx();
  ESOS_TASK_END();
} // end __esos_OutUint8AsDecString

ESOS_CHILD_TASK( __esos_OutUint32AsHexString, uint32_t u32_x) {
  static uint8_t        au8_String[11];
  static uint8_t        u8_c;

  ESOS_TASK_BEGIN();
  au8_String[0] = '0';
  au8_String[1] = 'x';
  u8_c = (u32_x >> 24);
  au8_String[2] = __esos_u8_GetMSBHexCharFromUint8(u8_c);
  au8_String[3] = __esos_u8_GetLSBHexCharFromUint8(u8_c);
  u8_c = (u32_x >> 16);
  au8_String[4] = __esos_u8_GetMSBHexCharFromUint8(u8_c);
  au8_String[5] = __esos_u8_GetLSBHexCharFromUint8(u8_c);
  u8_c = (u32_x >> 8);
  au8_String[6] = __esos_u8_GetMSBHexCharFromUint8(u8_c);
  au8_String[7] = __esos_u8_GetLSBHexCharFromUint8(u8_c);
  u8_c = u32_x;
  au8_String[8] = __esos_u8_GetMSBHexCharFromUint8(u8_c);
  au8_String[9] = __esos_u8_GetLSBHexCharFromUint8(u8_c);
  au8_String[10] = 0;
  u8_c = 0;

  ESOS_TASK_WAIT_UNTIL_CB_HAS_AVAILABLE_AT_LEAST( __pst_CB_Tx, 10);
  __esos_CB_WriteUINT8Buffer( __pst_CB_Tx, &au8_String[0], 10 );
  // signal the hardware that a xfer should be started if
  // not already ongoing
  __esos_hw_signal_start_tx();
  ESOS_TASK_END();

} // end __esos_OutUint32AsHexString()

ESOS_CHILD_TASK( __esos_OutCharBuffer, uint8_t* pu8_out, uint8_t u8_len) {
  ESOS_TASK_BEGIN();
  ESOS_TASK_WAIT_UNTIL_CB_HAS_AVAILABLE_AT_LEAST( __pst_CB_Tx, u8_len);
  __esos_CB_WriteUINT8Buffer( __pst_CB_Tx, pu8_out, u8_len );
  // signal the hardware that a xfer should be started if
  // not already ongoing
  __esos_hw_signal_start_tx();
  ESOS_TASK_END();
} // end __esos_OutCharBuffer


ESOS_CHILD_TASK( __esos_OutString, char* psz_out ) {
  static char*        psz_local;

  ESOS_TASK_BEGIN();
  psz_local = psz_out;
  while ( *psz_local ) {
    // wait for room in the TX CB to appear
    ESOS_TASK_WAIT_WHILE_CB_IS_FULL( __pst_CB_Tx );
    // write the data in the TX CB
    __esos_CB_WriteUINT8( __pst_CB_Tx, *psz_local++ );
    __esos_hw_signal_start_tx();
  } //end while()
  ESOS_TASK_END();
} // end __esos_OutString


/* ***********************************
 *  INCOMING COMM CHILD TASKS
 *************************************
*/
ESOS_CHILD_TASK( __esos_getBuffer, uint8_t* pau8_buff, uint8_t u8_size) {
  static uint8_t        u8_i;
  static uint8_t*       pau8_LocalPtr;
  static uint8_t        u8_LocalSize;

  ESOS_TASK_BEGIN();
  u8_LocalSize = u8_size;
  pau8_LocalPtr = pau8_buff;

  for (u8_i=0; u8_i<u8_LocalSize; u8_i++) {
    //wait for the RX character to arrive
    ESOS_TASK_WAIT_WHILE ( __ESOS_CB_IS_EMPTY( __pst_CB_Rx ) );
    pau8_LocalPtr[u8_i] = __esos_CB_ReadUINT8( __pst_CB_Rx );
  } // end for(...)
  ESOS_TASK_END();
} // end __esos_getBuffer

// ************************************************************
// THIS FUNCTION IS POTENTIALLY VERY DANGEROUS!!!!!!
// ************************************************************
// The return data buffer pau8_buff must be at least as big as
// the ESOS coming incoming buffer -- ESOS_SERIAL_OUT_EP_SIZE
//
// Will suck down data until we get a zero-terminator, or
// carriage return/newline type character.  Will also return when
// the number of characters received is the size of the ESOS
// incoming data buffer -- ESOS_SERIAL_OUT_EP_SIZE
//
ESOS_CHILD_TASK( __esos_getString, char* pau8_buff) {
  static uint8_t        u8_i;

  ESOS_TASK_BEGIN();
  for (u8_i=0; u8_i<(ESOS_SERIAL_OUT_EP_SIZE-1); u8_i++) {
    //wait for the RX character to arrive
    ESOS_TASK_WAIT_WHILE ( __ESOS_CB_IS_EMPTY( __pst_CB_Rx ) );
    pau8_buff[u8_i] = __esos_CB_ReadUINT8( __pst_CB_Rx );
    if ((pau8_buff[u8_i] == '\n') || (pau8_buff[u8_i] == '\r') || (pau8_buff[u8_i] == 0)) break;
  } // end for(...)
  pau8_buff[u8_i] = 0;
  ESOS_TASK_END();
} // end __esos_getString


// ================================================================
// UNSAFE I/O functions.  Only use when you can be absolutely sure
//  that they will not hang the communications or ESOS scheduler.
// Usually used during init of ESOS before the comm system or scheduler
//  are even running.
// ================================================================

// This routine is UNSAFE.  It can HANG the system!!!!
void __esos_unsafe_PutUint8(uint8_t u8_c) {
  // hang while CB is full
  while (__ESOS_CB_IS_FULL( __pst_CB_Tx ));
  // write the data in the TX CB
  __esos_CB_WriteUINT8( __pst_CB_Tx, u8_c);
  // signal the hardware that a xfer should be started if
  // not already ongoing
  __esos_hw_signal_start_tx();
} //end of __esos_unsafe_PutUint8()

// This routine is UNSAFE.  It can HANG the system!!!!
void __esos_unsafe_PutString(char* psz_in) {
  while ( *psz_in ) {
    // hang while CB is full
    while (__ESOS_CB_IS_FULL( __pst_CB_Tx ));
    // write the data in the TX CB
    __esos_CB_WriteUINT8( __pst_CB_Tx, *psz_in++);
    // signal the hardware that a xfer should be started if
    // not already ongoing
    __esos_hw_signal_start_tx();
  } //end while
} // end __esos_unsafe_PutString()

// This routine is UNSAFE.  It can HANG the system!!!!
uint8_t __esos_unsafe_GetUint8(void) {
  //wait for the RX character to arrive -- CAN HANG HERE!
  while (__ESOS_CB_IS_EMPTY( __pst_CB_Rx ));
  return __esos_CB_ReadUINT8( __pst_CB_Rx );  //return the character
}

