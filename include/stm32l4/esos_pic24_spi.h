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


#ifndef _ESOS_PIC24_SPI_H
#define _ESOS_PIC24_SPI_H


/**
 * \addtogroup ESOS_SPI_Service
 * @{
 */


// Documentation for this file. If the \file tag isn't present,
// this file won't be documented.
/** \file
 *  This file contains routines which se SPI on the Microchip PIC24 MCU.
 */


/* I N C L U D E S **********************************************************/
#include "esos.h"
#include "esos_pic24.h"

/* D E F I N I T I O N S ****************************************************/
/*
The following macros are a subset of the AND/OR macros found in the
the MPLAB C30/src/peripheral_24F/spi.h file and placed
in this header for use solely and exclusively on Microchip
PICmicro Microcontroller products as
specified in the MPLAB C30/src/peripheral_24F/spi.h header.
*/

/* SPIXCON1: SPIx CONTROL REGISTER 1 */
#define  DISABLE_SCK_PIN        0x1000 /* SCK pin is not used by module */
#define  ENABLE_SCK_PIN         0x0000 /* SCK pin is  used by module */
#define  SCK_PIN_MASK           (~DISABLE_SCK_PIN)

#define  DISABLE_SDO_PIN        0x0800 /* SDO pin is not used by module */
#define  ENABLE_SDO_PIN         0x0000 /* SDO pin is  used by module */
#define  SDO_PIN_MASK           (~DISABLE_SDO_PIN)

#define  SPI_MODE16_ON          0x0400 /* Communication is word wide */
#define  SPI_MODE8_ON           0x0000 /* Communication is byte wide */
#define  SPI_MODE_MASK          (~SPI_MODE16_ON)

#define  SPI_SMP_ON             0x0200 /* Input data sampled at end of data output time */
#define  SPI_SMP_OFF            0x0000 /* Input data sampled at middle of data output time */
#define  SPI_SMP_MASK           (~SPI_SMP_ON)

#define  SPI_CKE_ON             0x0100 /* Transmit happens from active clock state to idle clock state*/
#define  SPI_CKE_OFF            0x0000 /* Transmit happens on transition from idle clock state to active clock state */
#define  SPI_CKE_MASK           (~SPI_CKE_ON)

#define  SLAVE_ENABLE_ON        0x0080 /* Slave Select enbale */
#define  SLAVE_ENABLE_OFF       0x0000 /* Slave Select not used by module */
#define  SLAVE_ENABLE_MASK      (~SLAVE_ENABLE_ON)

#define  CLK_POL_ACTIVE_LOW     0x0040 /* Idle state for clock is high, active is low */
#define  CLK_POL_ACTIVE_HIGH    0x0000 /* Idle state for clock is low, active is high */
#define  CLK_POL_ACTIVE_MASK    (~CLK_POL_ACTIVE_LOW)

#define  MASTER_ENABLE_ON       0x0020 /* Master Mode */
#define  MASTER_ENABLE_OFF      0x0000 /* Slave Mode */
#define  MASTER_ENABLE_MASK     (~MASTER_ENABLE_ON)

#define  SEC_PRESCAL_1_1        0x001c /* Secondary Prescale 1:1   */
#define  SEC_PRESCAL_2_1        0x0018 /* Secondary Prescale 2:1   */
#define  SEC_PRESCAL_3_1        0x0014 /* Secondary Prescale 3:1   */
#define  SEC_PRESCAL_4_1        0x0010/* Secondary Prescale 4:1   */
#define  SEC_PRESCAL_5_1        0x000c /* Secondary Prescale 5:1   */
#define  SEC_PRESCAL_6_1        0x0008/* Secondary Prescale 6:1   */
#define  SEC_PRESCAL_7_1        0x0004 /* Secondary Prescale 7:1   */
#define  SEC_PRESCAL_8_1        0x0000 /* Secondary Prescale 8:1   */
#define  SEC_PRESCAL_MASK       (~SEC_PRESCAL_1_1)

#define  PRI_PRESCAL_1_1        0x0003 /* Primary Prescale 1:1     */
#define  PRI_PRESCAL_4_1        0x0002 /* Primary Prescale 4:1     */
#define  PRI_PRESCAL_16_1       0x0001 /* Primary Prescale 16:1    */
#define  PRI_PRESCAL_64_1       0x0000 /* Primary Prescale 64:1    */
#define  PRI_PRESCAL_MASK       (~PRI_PRESCAL_1_1)

/* SPIxSTAT REGISTER */
#define  SPI_ENABLE             0x8000 /* Enable module */
#define  SPI_DISABLE            0x0000 /* Disable module */
#define  SPI_ENBL_DSBL_MASK     (~SPI_ENABLE)

#define  SPI_IDLE_STOP          0x2000 /* Discontinue module operation in idle mode */
#define  SPI_IDLE_CON           0x0000 /* Continue module operation in idle mode */
#define  SPI_IDLE_MASK          (~SPI_IDLE_STOP)

#define  SPI_RX_OVFLOW          0x0040
#define  SPI_RX_OVFLOW_CLR      0x0000 /* Clear receive overflow bit */

#define  SPI_TX_BUFFER_FULL     0x0002 /* TX not started yet; SPIxTXB is full */
#define  SPI_TX_BUFFER_FULL_CLR (~SPI_TX_BUFFER_FULL) /* TX underway; SPIxTXB is empty */

#define  SPI_RX_BUFFER_FULL     0x0001 /* RX complete; SPIxRXB is full */
#define  SPI_RX_BUFFER_FULL_CLR (~SPI_RX_BUFFER_FULL) /* RX complete; SPIxRXB is full */

/* SPIxCON2: SPIx CONTROL REGISTER 2 */
#define  FRAME_ENABLE_ON        0x8000 /* Frame SPI support enable */
#define  FRAME_ENABLE_OFF       0x0000 /* Frame SPI support Disable */
#define  FRAME_ENABLE_MASK      (~FRAME_ENABLE_ON)

#define  FRAME_SYNC_INPUT       0x4000 /* Frame sync pulse Input (slave)  */
#define  FRAME_SYNC_OUTPUT      0x0000 /* Frame sync pulse Output (master)*/
#define  FRAME_SYNC_MASK        (~FRAME_SYNC_INPUT)

#define  FRAME_SYNC_ACTIVE_HIGH 0x2000 /* Frame sync pulse Input (slave)  */
#define  FRAME_SYNC_ACTIVE_LOW  0x0000 /* Frame sync pulse Output (master)*/
#define  FRAME_SYNC_POL_MASK    (~FRAME_SYNC_ACTIVE_HIGH)

#define  SPI_FRM_PULSE_FIRST_CLK 0x0002 /* frame pulse coincides with the first bit clock */
#define  SPI_FRM_PULSE_PREV_CLK  0x0000 /* frame pulse precedes the first bit clock */
#define  SPI_FRM_PULSE_MASK     (~SPI_FRM_PULSE_FIRST_CLK)

#define  SPI_ENH_BUFF_ENABLE    0x0001 /* enable enhanced buffer */
#define  SPI_ENH_BUFF_DISABLE   0x0000 /* disable enhanced buffer */
#define  SPI_ENH_BUFF_MASK      (~SPI_ENH_BUFF_ENABLE)


/* E X T E R N S ************************************************************/
extern struct stTask     __stChildTaskSPI;
extern uint16_t            __esos_spi_u16s[2];     // used to store arguments

/* M A C R O S **************************************************************/

#define ESOS_TASK_WAIT_ON_AVAILABLE_SPI()                                           \
        do {                                                                        \
        ESOS_TASK_WAIT_WHILE(__esos_IsSystemFlagSet(__ESOS_SYS_SPI_IS_BUSY));   \
        __esos_SetSystemFlag(__ESOS_SYS_SPI_IS_BUSY);                           \
      }while(0)

#define ESOS_TASK_SIGNAL_AVAILABLE_SPI() __esos_ClearSystemFlag(__ESOS_SYS_SPI_IS_BUSY)

/**
Transaction: Write 1 (ONE) "word" stored in variable \em u16_d1 to SPI device.
\param u16_d1   Variable containing word (byte or 16-bits) to write
\note Assumes SPI peripheral has been properly configured.
\note SPI peripheral configuration determines whether 8 or 16 bits are written.
\sa ESOS_TASK_WAIT_ON_WRITE2SPI1
\sa ESOS_TASK_WAIT_ON_WRITENSPI1
\sa ESOS_TASK_WAIT_ON_XFERNSPI1
\hideinitializer
*/
#define   ESOS_TASK_WAIT_ON_WRITE1SPI1(u16_d1 )              \
    do{                                                                        \
      __esos_spi_u16s[0] = (uint16_t) (u16_d1);                                      \
      ESOS_TASK_SPAWN_AND_WAIT( (ESOS_TASK_HANDLE)&__stChildTaskSPI, __esos_pic24_xferNSPI1, &__esos_spi_u16s[0], NULLPTR, 1 );    \
    }while(0)


// We need a 2-byte array to be able to use the buffer.
// Otherwise, we'd need two writeN calls, which require
// child tasks, i.e. code size grows

/**
Transaction: Write 2 (TWO) "words" (bytes or 16-bits) stored in variables \em u16_d1 and \em u16_d2 to SPI device.
\param u16_d1   Variable containing first byte to write
\param u16_d2   Variable containing second byte to write
\note Assumes SPI peripheral has been properly configured.
\note SPI peripheral configuration determines whether 8 or 16 bits are written.
\sa ESOS_TASK_WAIT_ON_WRITE1SPI1
\sa ESOS_TASK_WAIT_ON_WRITENSPI1
\sa ESOS_TASK_WAIT_ON_XFERNSPI1
\hideinitializer
*/
#define   ESOS_TASK_WAIT_ON_WRITE2SPI1(u16_d1, u16_d2 )              \
    do{                                                                        \
      __esos_spi_u16s[0] = (uint16_t) (u16_d1);                                      \
      __esos_spi_u16s[1] = (uint16_t) (u16_d2);                                      \
      ESOS_TASK_SPAWN_AND_WAIT( (ESOS_TASK_HANDLE)&__stChildTaskSPI, __esos_pic24_xferNSPI1, &__esos_spi_u16s[0], NULLPTR, 2 );    \
    }while(0)

/**
Transaction: Write \em u16_cnt "words" (bytes or 16-bits) to SPI device
\param pu16_out Pointer to buffer containing words to send
\param u16_cnt Number of bytes to send
\note Assumes SPI peripheral has been properly configured.
\note SPI peripheral configuration determines whether 8 or 16 bits are written.
\sa ESOS_TASK_WAIT_ON_WRITE1SPI1
\sa ESOS_TASK_WAIT_ON_WRITE2SPI1
\sa ESOS_TASK_WAIT_ON_XFERNSPI1
\hideinitializer
*/
#define   ESOS_TASK_WAIT_ON_WRITENSPI1( pu16_out, u16_cnt )              \
            ESOS_TASK_SPAWN_AND_WAIT( (ESOS_TASK_HANDLE)&__stChildTaskSPI, __esos_pic24_xferNSPI1, (pu16_out), NULLPTR, (u16_cnt) )

/**
Transaction: Transfer (Read and write SPI simultaneously) \em u16_cnt "words" (bytes or 16-bits) to SPI device
\param pu16_out Pointer to buffer containing words to send
\param pu16_in Pointer to buffer to catch incoming words
\param u16_cnt Number of bytes to send
\note Assumes SPI peripheral has been properly configured.
\note SPI peripheral configuration determines whether 8 or 16 bits are written.
\note Assumes that both incoming and outgoing buffers are properly sized and available.
\sa ESOS_TASK_WAIT_ON_WRITE1SPI1
\sa ESOS_TASK_WAIT_ON_WRITE2SPI1
\sa ESOS_TASK_WAIT_ON_WRITENSPI1
\sa ESOS_TASK_WAIT_ON_READ1SPI1
\sa ESOS_TASK_WAIT_ON_READ2SPI1
\sa ESOS_TASK_WAIT_ON_READNSPI1
\hideinitializer
*/
#define   ESOS_TASK_WAIT_ON_XFERNSPI1( pu16_out, pu16_in, u16_cnt )              \
            ESOS_TASK_SPAWN_AND_WAIT( (ESOS_TASK_HANDLE)&__stChildTaskSPI, __esos_pic24_xferNSPI1, (pu16_out), (pu16_in), (u16_cnt) )

/**
Transaction: Read 1 (ONE) "word" from SPI device and stores result in variable \em u16_d1
\param u16_d1   Variable containing word (byte or 16-bits) to hold read results
\note Assumes SPI peripheral has been properly configured.
\note SPI peripheral configuration determines whether 8 or 16 bits are written.
\note This routine writes ZEROES to SPI device during reads
\sa ESOS_TASK_WAIT_ON_READ2SPI1
\sa ESOS_TASK_WAIT_ON_READNSPI1
\sa ESOS_TASK_WAIT_ON_XFERNSPI1
\hideinitializer
*/
#define   ESOS_TASK_WAIT_ON_READ1SPI1(u16_d1 )              \
    do{                                                                        \
      ESOS_TASK_SPAWN_AND_WAIT( (ESOS_TASK_HANDLE)&__stChildTaskSPI, __esos_pic24_xferNSPI1, NULLPTR, &__esos_spi_u16s[0], 1 );    \
      (u16_d1) = __esos_spi_u16s[0];        \
    }while(0)

/**
Transaction: Read 2 (TWO) "words" (bytes or 16-bits) from SPI device.  Stores results in variables \em u16_d1 and \em u16_d2
\param u16_d1   Variable containing first word/byte read
\param u16_d2   Variable containing second word/byte read
\note Assumes SPI peripheral has been properly configured.
\note SPI peripheral configuration determines whether 8 or 16 bits are written.
\note This routine writes ZEROES to SPI device during reads
\sa ESOS_TASK_WAIT_ON_READ1SPI1
\sa ESOS_TASK_WAIT_ON_READNSPI1
\sa ESOS_TASK_WAIT_ON_XFERNSPI1
\hideinitializer
*/
#define   ESOS_TASK_WAIT_ON_READ2SPI1(u16_d1, u16_d2)              \
    do{                                                            \
      ESOS_TASK_SPAWN_AND_WAIT( (ESOS_TASK_HANDLE)&__stChildTaskSPI, __esos_pic24_xferNSPI1, NULLPTR, &__esos_spi_u16s[0], 2 );    \
      (u16_d1) = __esos_spi_u16s[0];        \
      (u16_d2) = __esos_spi_u16s[1];        \
    }while(0)

/**
Transaction: Reads \em u16_cnt "words" (bytes or 16-bits) from SPI device.  Results are written to buffer pointed to by \em pu16_d
\param pu16_in Pointer to buffer to catch read results
\param u16_cnt Number of bytes to read
\note Assumes SPI peripheral has been properly configured.
\note SPI peripheral configuration determines whether 8 or 16 bits are written.
\note This routine writes ZEROES to SPI device during reads
\sa ESOS_TASK_WAIT_ON_READ1SPI1
\sa ESOS_TASK_WAIT_ON_READ2SPI1
\sa ESOS_TASK_WAIT_ON_XFERNSPI1
\hideinitializer
*/
#define   ESOS_TASK_WAIT_ON_READNSPI1( pu16_in, u16_cnt )              \
            ESOS_TASK_SPAWN_AND_WAIT( (ESOS_TASK_HANDLE)&__stChildTaskSPI, __esos_pic24_xferNSPI1, NULLPTR, (pu16_in), (u16_cnt) )


/* P U B L I C  P R O T O T Y P E S *****************************************/
ESOS_CHILD_TASK( __esos_pic24_xferNSPI1, uint16_t* pu16_out, uint16_t* pu16_in, uint16_t u16_cnt);

/** @} */
#endif // end ESOS_PIC24_SPI_H
