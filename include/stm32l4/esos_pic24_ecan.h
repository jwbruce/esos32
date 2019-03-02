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

/**
 * \addtogroup ESOS_Task_ECAN_Service
 * @{
 */

/** \file
*  \brief This file contains routines which configure and
*  use an ECAN interface on the Microchip PIC24 MCUs.
*/

#ifndef ESOS_PIC24_ECAN_H
#define ESOS_PIC24_ECAN_H

#include    "esos.h"
#include    "esos_pic24.h"

#define __ESOS_ECAN_HW_NUM_TX_BUFS              1
#define __ESOS_ECAN_HW_NUM_BUFS                 8

#define __ESOS_ECAN_HW_ARE_MESSAGES_WAITING()   ( _DMA1IF )
#define __ESOS_ECAN_HW_GET_BUFFER_ID()          ( C1FIFO & 0x1F )
#define __ESOS_ECAN_HW_TX_IN_PROGRESS()         ( C1TR01CONbits.TXREQ0 )

void __esos_ecan_hw_config_ecan ( void );
void __esos_ecan_hw_config_baud ( void );
void __esos_ecan_hw_config_rxFilter (  );
void __esos_ecan_hw_config_dma_tx ( void );
void __esos_ecan_hw_config_dma_rx ( void );
void __esos_ecan_hw_mark_message_received ( uint16_t u16_rx_buff_id );
void __esos_ecan_hw_format_and_send ( ECANMSG *p_temp_Msg, uint16_t u16_can_id, uint8_t u8_payload_len );

#endif  /* ESOS_PIC24_ECAN_H */
