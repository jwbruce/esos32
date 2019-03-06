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
 * \addtogroup ESOS_Task_ECAN_Service
 * @{
 */

/** \file
* \brief Template for creating hardware-specific support for the ESOS32
* Controller Area Network (CAN) service 
*/

/*** I N C L U D E S *************************************************/
#include    "esos.h"
#include    "esos_hwxxx.h"
#include    "esos_hwxxx_ecan.h"
#include    "pic24_all.h"
#include    "pic24_ecan.h"

/*** T H E   C O D E *************************************************/
extern __eds__ ECANMSG __esos_ecan_msgBuf[__ESOS_ECAN_HW_NUM_BUFS] __attribute__((space(dma),aligned(__ESOS_ECAN_HW_NUM_BUFS*16),eds));

void __esos_ecan_hw_config_ecan ( void ) {
  uint8_t u8_i;

  CHANGE_MODE_ECAN1(ECAN_MODE_CONFIGURE);
  configBaudECAN1();

  C1CFG1 = 0x00c1;
  C1CFG2 = 0x45da;

  C1FCTRL = ECAN_FIFO_START_AREA_1 | ECAN_DMA_BUF_SIZE_8;
  configRxFilterECAN1( 0, 0x7a0, 0, 15, 0 );
  configRxMaskECAN1( 0, 0x000, 0, 0 );
  clrRxFullOvfFlagsECAN1();

  for ( u8_i = 0; u8_i < 8; u8_i++ ) {
    if ( u8_i < __ESOS_ECAN_HW_NUM_TX_BUFS ) {
      configTxRxBufferECAN1( u8_i, ECAN_TX_BUFF, 3 );
    } else {
      configTxRxBufferECAN1( u8_i, ECAN_RX_BUFF, 3 );
    }
  }

  __esos_ecan_hw_config_dma_tx();
  __esos_ecan_hw_config_dma_rx();

  CONFIG_C1TX_TO_RP(102);
  RPINR26bits.C1RXR = 101;

  return;
}

void __esos_ecan_hw_config_dma_tx ( void ) {
  DMAPWC = 0;
  _DMA0IF = 0;
  DMA0PAD = (unsigned int) &C1TXD;
  DMA0REQ = DMA_IRQ_ECAN1TX;
  DMA0STAL = (unsigned int) &__esos_ecan_msgBuf;
  DMA0STAH = (unsigned int) &__esos_ecan_msgBuf;
  DMA0CNT =   7;
  DMA0CON =
    (   DMA_MODULE_ON |
        DMA_SIZE_WORD |
        DMA_DIR_WRITE_PERIPHERAL |
        DMA_INTERRUPT_FULL |
        DMA_NULLW_OFF |
        DMA_AMODE_PERIPHERAL_INDIRECT |
        DMA_MODE_CONTINUOUS
    );

  return;
}

void __esos_ecan_hw_config_dma_rx ( void ) {
  _DMA1IF = 0;
  DMA1PAD = (unsigned int) &C1RXD;
  DMA1REQ = DMA_IRQ_ECAN1RX;
  DMA1STAL = (unsigned int) &__esos_ecan_msgBuf;
  DMA1STAH = (unsigned int) &__esos_ecan_msgBuf;
  DMA1CNT =  7;
  DMA1CON =
    (   DMA_MODULE_ON |
        DMA_SIZE_WORD |
        DMA_DIR_READ_PERIPHERAL |
        DMA_INTERRUPT_FULL |
        DMA_NULLW_OFF |
        DMA_AMODE_PERIPHERAL_INDIRECT |
        DMA_MODE_CONTINUOUS
    );

  return;
}

void __esos_ecan_hw_mark_message_received ( uint16_t u16_rx_buff_id ) {
  clrRxFullFlagECAN1( u16_rx_buff_id );
  clrRxFullOvfFlagsECAN1();

  return;
}

void __esos_ecan_hw_format_and_send ( ECANMSG *p_temp_Msg, uint16_t u16_can_id, uint8_t u8_payload_len ) {
  formatStandardDataFrameECAN( p_temp_Msg, u16_can_id, u8_payload_len );
  __esos_ecan_msgBuf[0] = *p_temp_Msg;
  startTxECAN1(0);

  return;
}
