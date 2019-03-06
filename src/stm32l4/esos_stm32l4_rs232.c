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
/** \file
 * \brief STM32L4xx hardware support for RS-232 serial communications
 *
 */

/*** I N C L U D E S *************************************************/
#include "esos_stm32l4_rs232.h"

/*** G L O B A L S *************************************************/
uint8_t         u8_uartRXbuf;
uint8_t         u8_uartTXbuf;
UART_HandleTypeDef    st_huart2;

/*** T H E   C O D E *************************************************/
// probably need to define a flag for this so ISRs can start/stop
//  transfers
#define   __ESOS_HW_SIGNAL_START_TX()     __esos_SetSystemFlag(__ESOS_SYS_COMM_TX_ONGOING)
#define   __ESOS_HW_SIGNAL_STOP_TX()        __esos_ClearSystemFlag(__ESOS_SYS_COMM_TX_ONGOING)

/*********************************************************
 * Public functions intended to be called by other files *
 *********************************************************/
inline void    __esos_hw_signal_start_tx(void) {
  if __esos_IsSystemFlagClear(__ESOS_SYS_COMM_TX_ONGOING) {
    // no TX is currently ongoing, so kick it off.
    __esos_SetSystemFlag(__ESOS_SYS_COMM_TX_ONGOING);
    // HAL_UART_Transmit_IT requires a pointer to the data, so
    //  have CB return the data to a local variable and pass that
    //  address to the HAL_UART_Transmit_IT.  The ESOS CB don't
    //  have a routine that returns a pointer to data, and I
    //  don't trust the HAL enough to pass an address into my CB.
    u8_uartTXbuf = __esos_CB_ReadUINT8(  __pst_CB_Tx );
    HAL_UART_Transmit_IT(&st_huart2, &u8_uartTXbuf, 1);
    //HAL_UART_Transmit_IT(&st_huart2, &__st_TxBuffer.pau8_Data[__st_TxBuffer.u16_Tail], 1);
  }
}

inline void    __esos_hw_signal_stop_tx(void) {
  __esos_ClearSystemFlag(__ESOS_SYS_COMM_TX_ONGOING);
}

/** *******************************************
 * UART ISR callbacks
 *
 * HAL will call these functions at the conclusion
 * of TX and RX operations
 *
 * TODO:  STM32L4 ISRs below are using the standard
 *   STM32 cube priorities.  These ISRs have not
 *   been incorporated into ESOS's interrupt priority
 *   scheme.
 *
 * ********************************************
 */


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle) {
  /* If this function is called, we have received a
   * byte into the UART.  The HAL UART handler has
   * called us, and the received data is in u8_uartRXbuf.
   * Do something with it (i.e. copy it over into our
   * ciruclar buffers) and initiate a new transfer.
   */

  //__st_RxBuffer.u16_Head++;     //increment head pointer
  //if (__st_RxBuffer.u16_Head == ESOS_SERIAL_OUT_EP_SIZE)
  //__st_RxBuffer.u16_Head = 0; //wrap if needed
  //// place our new data into buffer
  //__st_RxBuffer.pau8_Data[__st_RxBuffer.u16_Head] = u8_uartRXbuf;

  // call CB overwrite.... we can't block here in the ISR so
  // if data comes in too fast, we will just overwrite existing
  // data in the CBs
  __esos_CB_OverwriteUINT8( __pst_CB_Rx, u8_uartRXbuf );
  // request HAL UART handler to get ONE more byte
  HAL_UART_Receive_IT(UartHandle, &u8_uartRXbuf, 1);
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle) {
  if (__ESOS_CB_IS_EMPTY( __pst_CB_Tx)) {
    //empty TX buffer, disable the interrupt, do not clear the flag
    __esos_hw_signal_stop_tx();
  } else {
    // HAL_UART_Transmit_IT requires a pointer to the data, so
    //  have CB return the data to a local variable and pass that
    //  address to the HAL_UART_Transmit_IT.  The ESOS CB don't
    //  have a routine that returns a pointer to data, and I
    //  don't trust the HAL enough to pass an address into my CB.
    //
    // TODO: Once this gets working... change to have the HAL
    //       transmit all remining data in the circular buffer
    //       This will improve performance a little bit.
    u8_uartTXbuf = __esos_CB_ReadUINT8(  __pst_CB_Tx );
    HAL_UART_Transmit_IT(&st_huart2, &u8_uartTXbuf, 1);
  }
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle) {
  if(UartHandle->ErrorCode == HAL_UART_ERROR_ORE)
    HAL_UART_Receive_IT(UartHandle, &u8_uartRXbuf, 1);
}

/** Configure the UART. Settings chosen:
 *  - Format is 8 data bits, no parity, 1 stop bit
 *  - CTS, RTS, and BCLK not used
 *
 *  \param u32_baudRate The baud rate to use.
 */
void __esos_configUART1(uint32_t u32_baudRate) {
  /*************************  UART config ********************/


#if (HARDWARE_PLATFORM == NUCLEO64)
# warning Building configUART1() for Nucleo 64 development board
  // we are using USART2.  On the STM32L452RE chip, this means
  // that TX is PA2, and RX is PA3.
  st_huart2.Instance = USART2;
  st_huart2.Init.BaudRate = u32_baudRate;
  st_huart2.Init.WordLength = UART_WORDLENGTH_8B;
  st_huart2.Init.StopBits = UART_STOPBITS_1;
  st_huart2.Init.Parity = UART_PARITY_NONE;
  st_huart2.Init.Mode = UART_MODE_TX_RX;
  st_huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  st_huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  st_huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  st_huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&st_huart2) != HAL_OK) {
    Error_Handler();
  }
#else
#error Can not configUART1() since target hardware has not been defined.
#endif

  // Now that UART is configured, let's enable the USART2 interrupts
  HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(USART2_IRQn);

  // Kick off RX on UART via call to HAL.  Upon receipt of data
  //  the call back should kick off another one.  In the current
  //  specificaiton of ESOS, we always are "receiving", so RX is
  //  never disabled.
  HAL_UART_Receive_IT(&st_huart2, &u8_uartRXbuf, 1);

}

/* ########################################################################### */


/******************************************************************************
 * Function:        void _esos_hw_InitSerialUart( void )
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          ptr to ESOS_COMM_BUFF_DSC structure with initialized ptrs
 *
 * Side Effects:    Turns on USART hardware
 *
 *****************************************************************************/
void __esos_hw_InitCommSystem(void) {
  // use the HAL-inspired code to init the RS232 comm subsystem
  // 8N1 @ 56k7 baud (DEFAULT_BAUDRATE) for now
  __esos_configUART1(DEFAULT_BAUDRATE) ;

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
