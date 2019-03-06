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


/**
 * \addtogroup ESOS_UART_Service
 * @{
 */


// Documentation for this file. If the \file tag isn't present,
// this file won't be documented.
/**
* /** \file
* \brief Template for creating hardware-specific support for the
* ESOS32 serial communications service
*/

/*** I N C L U D E S *************************************************/
#include "esos_hwxxx_rs232.h"

/*** G L O B A L S *************************************************/

/*** T H E   C O D E *************************************************/
#define   __ESOS_HW_SIGNAL_START_TX()         _U1TXIE = 1
#define   __ESOS_HW_SIGNAL_STOP_TX()          _U1TXIE = 0

/*********************************************************
 * Public functions intended to be called by other files *
 *********************************************************/
// The hwxxx specific code to start or "kick-off" a transmit
//   of data out the UART
inline void    __esos_hw_signal_start_tx(void) {
  __ESOS_HW_SIGNAL_START_TX();
}

// The hwxxx specific code to stop or "cancel" a transmit
//   of data out the UART
inline void    __esos_hw_signal_stop_tx(void) {
  __ESOS_HW_SIGNAL_STOP_TX();
}



/* ########################################################################### */
void _ISRFAST _U1TXInterrupt (void) {
  if (__st_TxBuffer.u16_Head == __st_TxBuffer.u16_Tail) {
    //empty TX buffer, disable the interrupt, do not clear the flag
    __ESOS_HW_SIGNAL_STOP_TX();
  } else {
    //at least one free spot in the TX buffer!
    __st_TxBuffer.u16_Tail++;      //increment tail pointer
    if (__st_TxBuffer.u16_Tail == ESOS_SERIAL_IN_EP_SIZE)
      __st_TxBuffer.u16_Tail = 0;  //wrap if needed
    _U1TXIF = 0;                  //clear the interrupt flag
    //transfer character from software buffer to transmit buffer
    U1TXREG =  __st_TxBuffer.pau8_Data[__st_TxBuffer.u16_Tail];
  }
}

void _ISRFAST _U1RXInterrupt (void) {
  int8_t u8_c;

  _U1RXIF = 0;          //clear the UART RX interrupt bit

  // This fcn is found in pic24_uart.c which we've replaced with
  //   our own ESOS versions.
  u8_c = U1RXREG;       //read character
  __st_RxBuffer.u16_Head++;     //increment head pointer
  if (__st_RxBuffer.u16_Head == ESOS_SERIAL_OUT_EP_SIZE)
    __st_RxBuffer.u16_Head = 0; //wrap if needed

  __st_RxBuffer.pau8_Data[__st_RxBuffer.u16_Head] = u8_c;   //place in buffer
}


/******************************************************************************
 * Function:        void _esos_hw_enableCommSystem( void )
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    Enables USART operations
 *            hwxxx specific code to enable (start using) the
 *                  communications systems that has already been
 *                  configured/initialized.
 *
 *****************************************************************************/
void __esos_hw_enableCommSystem( ) {
  // code goes here to enable the UART
}

/* ########################################################################### */

/******************************************************************************
 * Function:        void _esos_hw_disableCommSystem( void )
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    Diables USART operations
 *            hwxxx specific code to disable (stop using) the
 *                  communications systems that has already been
 *                  configured/initialized.
 *
 *****************************************************************************/
void __esos_hw_disableCommSystem( ) {
  // code goes here to disable the UART
}

/* ########################################################################### */


/******************************************************************************
 * Function:        void _esos_hw_initCommSystem( uint32_t u32_baudRate )
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    Turns on USART hardware.
 *            hwxxx specific code to configure the UART to a the
 *                  specified baud rate.     The package pins used for
 *                  TX and RX are hard-coded in here for the specific
 *                  processor and PCB being used, as there is not
 *                  generalized way to do this across processor
 *                  architectures. Furthermore, such things do not
 *                  need to be configure at run-time, but at compile-time.
 *
 *                  NOTE:  The UART is simply configured.  It
 *                         is NOT enabled in this function.
 *
 *****************************************************************************/
void __esos_hw_initCommSystem( uint32_t u32_baudRate ) {
// code goes here

}  // end __esos_hw_InitCommSystem()


/******************************************************************************
 * Function:        uint8_t esos_GetCommSystemMaxInDataLen(void)
 *
 * PreCondition:    None.
 *
 * Input:           None
 *
 * Output:          the maximum number of uint8_ts that the comm system will
 *                  receive in a single buffer transfer from the host -- OR --
 *          in the case of single uint8_t xfers (like RS232), the maximum
 *          number of uint8_ts that can be RX-ed before the buffers
 *          overflow
 *
 * Side Effects:    None
 *
 * Overview:        A way for a run-time determination of the maximum buffer
 *                  size that the user can can expect.  This number is
 *                  actually hard-coded in the USB CDC header file, but this
 *                  method will allow the user code to be more generic, if
 *                  it chooses to be.
 *
 *****************************************************************************/
uint8_t esos_GetCommSystemMaxInDataLen(void) {
  return ESOS_SERIAL_OUT_EP_SIZE;
} //end esos_GetCommSystemMaxInDataLen()

/******************************************************************************
 * Function:        uint8_t esos_GetCommSystemMaxOutDataLen(void)
 *
 * PreCondition:    None.
 *
 * Input:           None
 *
 * Output:          the maximum number of uint8_ts that the comm system will
 *                  transfer back to the host in a single buffer  -- OR --
 *          in the case of singe uint8_t xfers (like RS232), the maximum
 *          number of uint8_ts in the output buffer before overflow
 *
 * Side Effects:    None
 *
 * Overview:        A way for a run-time determination of the maximum buffer
 *                  size that the user can can send efficiently.  The USB system
 *                  will send a bigger buffer than getUSBCdcTxMax() size, but
 *                  will do so in several smaller getUSBCdcTxMax()-sized chunks.
 *
 *                  This number is actually hard-coded in the USB CDC header file,
 *                  but this method will allow the user code to be more generic,
 *                  if it chooses to be.
 *
 *****************************************************************************/
uint8_t esos_GetCommSystemMaxOutDataLen(void) {
  return  ESOS_SERIAL_IN_EP_SIZE;
} //end esos_GetCommSystemMaxOutDataLen()

/******************************************************************************
 * Function:        uint8_t _esos_hw_GetUartVersion(void)
 *
 * PreCondition:    None.
 *
 * Input:           None
 *
 * Output:          Return the version number of the MSU Bulk CDC driver firmware
 *                  currently running.
 *                  The most-significant bit denotes we're running USB
 *                  The most-significant nibble is the major revision number
 *                  The least-significant nibble is the minor revision number
 *
 * Side Effects:    None
 *
 *****************************************************************************/
uint8_t _esos_hw_GetSerialUartVersion(void) {
  return  ESOS_COMM_SYS_SERIAL_REV;
} //end _esos_hw_GetUartVersion()

/** @} */
