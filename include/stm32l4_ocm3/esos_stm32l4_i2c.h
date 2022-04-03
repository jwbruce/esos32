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

#ifndef _ESOS_STM32L4_I2C_H
#define _ESOS_STM32L4_I2C_H


/**
 * \addtogroup ESOS_I2C_Service
 * @{
 */

// Documentation for this file. If the \file tag isn't present,
// this file won't be documented.
/** \file
*  \brief This file contains routines which define, configure and
*  allow use of the I<sup>2</sup>C service on the ST Microelectronics
* STM32L452RE MCU
*/


/* I N C L U D E S **********************************************************/
#include "esos.h"
#include "esos_i2c.h"
#include "esos_stm32l4.h"
#include <libopencm3/stm32/i2c.h>

/* D E F I N I T I O N S ****************************************************/

/* E X T E R N S ************************************************************/
extern struct stTask    __stChildTaskI2C, __stGrandChildTaskI2C;
extern uint8_t            __esos_i2c_dataBytes[2];                    // used to store arguments

/* M A C R O S **************************************************************/
#define	CLEAR_REGISTER_BITS(reg,mask)			reg &= (~(mask))
#define	SET_REGISTER_BITS(reg,mask)				reg |= (mask)
#define	MAKE_I2C_WR_ADDR(bits)					bits &= 0xFFF
#define	MAKE_I2C_RD_ADDR(bits)					bits &= 0x001

#define	__ESOS_I2C_STM32L4_SET_WRITE_DIR()		I2C1_CR2 &= (~I2C_CR2_RD_WRN)
#define	__ESOS_I2C_STM32L4_SET_READ_DIR()		I2C1_CR2 |= I2C_CR2_RD_WRN
#define	__ESOS_I2C_STM32L4_SET_NUM_BYTES(x)		I2C1_CR2 = (I2C1_CR2 & ~I2C_CR2_NBYTES_MASK) | (x << I2C_CR2_NBYTES_SHIFT) // & ~I2C_CR2_START )|((x) << I2C_CR2_NBYTES_SHIFT)
#define	__ESOS_I2C_STM32L4_SET_ADDR7_MODE()		I2C1_CR2 &= (~I2C_CR2_ADD10)
#define	__ESOS_I2C_STM32L4_SET_ADDR10_MODE()	I2C1_CR2 |= I2C_CR2_ADD10
#define	__ESOS_I2C_STM32L4_SET_ADDR7(x)			I2C1_CR2 = (I2C1_CR2 & ~I2C_CR2_SADD_7BIT_MASK) | ((x & 0x7F) << I2C_CR2_SADD_7BIT_SHIFT) //& ~I2C_CR2_START)|((x) << I2C_CR2_SADD_7BIT_SHIFT)
#define	__ESOS_I2C_STM32L4_SET_AUTOEND()		I2C1_CR2 |= I2C_CR2_AUTOEND
#define	__ESOS_I2C_STM32L4_CLEAR_AUTOEND()		I2C1_CR2 &= (~I2C_CR2_AUTOEND)
#define	__ESOS_I2C_STM32L4_SET_RELOAD()			I2C1_CR2 |= I2C_CR2_RELOAD
#define	__ESOS_I2C_STM32L4_CLEAR_RELOAD()		I2C1_CR2 &= (~I2C_CR2_RELOAD)
#define	__ESOS_I2C_STM32L4_IS_NACK_RECEIVED()	(I2C1_ISR & I2C_ISR_NACKF)

#define	__ESOS_I2C_STM32L4_CLEAR_STOP_FLAG()	I2C1_ICR |= I2C_ICR_STOPCF
#define	__ESOS_I2C_STM32L4_RESET_CR2()			I2C1_CR2 &= (uint32_t)~((uint32_t)(I2C_CR2_SADD_7BIT_MASK | I2C_CR2_HEAD10R | I2C_CR2_NBYTES_MASK | I2C_CR2_RELOAD | I2C_CR2_RD_WRN))


#define	__ESOS_I2C_STM32L4_IS_BUSY()			(I2C1_ISR & I2C_ISR_BUSY)
// FROM Ref Manual:
// This bit is set by hardware when the I2C_TXDR register is empty and the data to be
// transmitted must be written in the I2C_TXDR register. It is cleared when the next data to be
// sent is written in the I2C_TXDR register.
#define	__ESOS_I2C_STM32L4_IS_TX_EMPTY()		(I2C1_ISR & I2C_ISR_TXIS)

// FROM Ref Manual:
// This bit is set by hardware when the received data is copied into the I2C_RXDR register, and
// is ready to be read. It is cleared when I2C_RXDR is read.
#define	__ESOS_I2C_STM32L4_IS_RX_NOT_EMPTY()	(I2C1_ISR & I2C_ISR_RXNE)
 
// Macros to perform hardware I2C fundamentals onwithin the ESOS
// I2C child tasks
#define __ESOS_I2C_HW_START()                   \
   do{                                         \
      I2C1_CR2 |= I2C_CR2_START;           \
      ESOS_TASK_WAIT_WHILE(  I2C1_CR2 & I2C_CR2_START);  \
   }while(0)

#define __ESOS_I2C_HW_RSTART()                \
  do{                                        \
    I2C1CONbits.RSEN = 1;                    \
    ESOS_TASK_WAIT_WHILE( I2C1CONbits.RSEN); \
  }while(0)

#define __ESOS_I2C_HW_STOP()                  \
  do{                                        \
    I2C1_CR2 |= I2C_CR2_STOP;                     \
    ESOS_TASK_WAIT_WHILE(  I2C1_CR2 & I2C_CR2_STOP);  \
  }while(0)

#define __ESOS_I2C_HW_PUT(byte)                   \
  do{                                            \
    I2C1_TXDR = (byte);                            \
    ESOS_TASK_WAIT_UNTIL( __esos_i2c_hw_IsTransferComplete() );  \
  }while(0)

/* P U B L I C  P R O T O T Y P E S *****************************************/

//I2C Operations
void __esos_i2c_hw_config(uint32_t u32_i2cbps);

//I2C Transactions
ESOS_CHILD_TASK( __esos_hw_getI2C1, uint8_t* pu8_x, uint8_t u8_ack2Send);
ESOS_CHILD_TASK( __esos_hw_writeNI2C1, uint8_t u8_addr, uint8_t* pu8_d, uint16_t u16_cnt);
ESOS_CHILD_TASK( __esos_hw_readNI2C1, uint8_t u8_addr, uint8_t* pu8_d, uint16_t u16_cnt);

/** @} */
#endif // end ESOS_STM32L4_I2C_H
