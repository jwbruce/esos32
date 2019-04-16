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

#ifndef _ESOS_HWXXX_I2C_H
#define _ESOS_HWXXX_I2C_H


/**
 * \addtogroup ESOS_I2C_Service
 * @{
 */

// Documentation for this file. If the \file tag isn't present,
// this file won't be documented.
/** \file
*  \brief This file contains template routines which define,
* configure and allow use of the I<sup>2</sup>C service on
* some specific hardware
*/


/* I N C L U D E S **********************************************************/
#include "esos.h"
#include "esos_i2c.h"
#include "esos_hwxxx.h"

/* D E F I N I T I O N S ****************************************************/

/* E X T E R N S ************************************************************/
extern struct stTask    __stChildTaskI2C, __stGrandChildTaskI2C;
extern uint8_t            __esos_i2c_dataBytes[2];                    // used to store arguments

/* M A C R O S **************************************************************/
#define	CLEAR_REGISTER_BITS(reg,mask)			reg &= (~(mask))
#define	SET_REGISTER_BITS(reg,mask)				reg |= (mask)
#define	MAKE_I2C_WR_ADDR(bits)					bits &= 0xFFE
#define	MAKE_I2C_RD_ADDR(bits)					bits &= 0x001

/**
 * Transaction: Write 2 (TWO) bytes stored in variables \em u8_d1 and \em u8_d2 to I2C slave at address \em u8_addr.
 * \param u8_addr  Slave I2C address
 * \param u8_d1   Variable containing first byte to write
 * \param u8_d2   Variable containing second byte to write
 * \sa ESOS_TASK_WAIT_ON_WRITE1I2C
 * \sa ESOS_TASK_WAIT_ON_WRITENI2C
 * \hideinitializer
 * 
 * */
#define   ESOS_TASK_WAIT_ON_WRITE2I2C( u8_addr, u8_d1, u8_d2 )              \
    do{                                                                        \
      __esos_i2c_dataBytes[0] = (u8_d1);                                      \
      __esos_i2c_dataBytes[1] = (u8_d2);                                      \
      ESOS_TASK_SPAWN_AND_WAIT( (ESOS_TASK_HANDLE)&__stChildTaskI2C, __esos_i2c_hw_writeN, (u8_addr), &__esos_i2c_dataBytes[0], 2 );    \
    }while(0)

/**
 * Transaction: Write \em u8_cnt bytes stored in buffer \em *pu8_d to I2C slave at address \em u8_addr.
 * \param u8_addr  Slave I2C address
 * \param pu8_d Pointer to buffer containing bytes to send
 * \param u8_cnt Number of bytes to send
 * \sa ESOS_TASK_WAIT_ON_WRITE1I2C
 * \sa ESOS_TASK_WAIT_ON_WRITE2I2C
 * \hideinitializer
 * 
 * */
#define   ESOS_TASK_WAIT_ON_WRITENI2C( u8_addr, pu8_d, u8_cnt )              \
            ESOS_TASK_SPAWN_AND_WAIT( (ESOS_TASK_HANDLE)&__stChildTaskI2C, __esos_i2c_hw_writeN, (u8_addr), (pu8_d), (u8_cnt) )

/**
 * Transaction: Read 1 (ONE) byte from I2C slave at address \em u8_addr, and save to variable \em u8_d1
 * As per the I2C standard, a NAK is returned for the last byte read from the slave, ACKs are returned for the other bytes.
 * \param u8_addr  Slave I2C address
 * \param u8_d1   variable to hold the read byte
 * \hideinitializer
 * \sa ESOS_TASK_WAIT_ON_READ2I2C
 * \sa ESOS_TASK_WAIT_ON_READNI2C
 * \sa esos_hwxxx_configI2C1
 * 
 * */
#define   ESOS_TASK_WAIT_ON_READ1I2C( u8_addr, u8_d1 )                     \
            ESOS_TASK_SPAWN_AND_WAIT( (ESOS_TASK_HANDLE)&__stChildTaskI2C, __esos_i2c_hw_readN, (u8_addr), &(u8_d1), 1 )

// While READ1 still worked fine, READ2 needs a new trick, since u8_d1 and u8_d2 are
// not in consecutive memory locations.  Can't do two READN calls, because that (again)
// requires another child task.  Instead, just re-use our 2-byte array as a read buffer
/**
 * Transaction: Read 2 (TWO) bytes from I2C slave at address \em u8_addr, save to variables \em u8_d1 and \em u8_d2
 * As per the I2C standard, a NAK is returned for the last byte read from the slave, ACKs are returned for the other bytes.
 * \param u8_addr  Slave I2C address
 * \param u8_d1   variable to hold first read byte
 * \param u8_d2   variable to hold second read byte
 * \hideinitializer
 * \sa ESOS_TASK_WAIT_ON_READ1I2C
 * \sa ESOS_TASK_WAIT_ON_READNI2C
 * \sa esos_hwxxx_configI2C1
 */
#define   ESOS_TASK_WAIT_ON_READ2I2C( u8_addr, u8_d1, u8_d2 )                     \
     do{                                                                              \
       ESOS_TASK_SPAWN_AND_WAIT( (ESOS_TASK_HANDLE)&__stChildTaskI2C, __esos_i2c_hw_readN, (u8_addr), &__esos_i2c_dataBytes[0], 2 );      \
       (u8_d1) = __esos_i2c_dataBytes[0];                                             \
       (u8_d2) = __esos_i2c_dataBytes[1];                                             \
     }while(0)

/**
 * Transaction: Read \em u8_cnt bytes from I2C slave at address \em u8_addr, save to buffer \em *pu8_d.
 * As per the I2C standard, a NAK is returned for the last byte read from the slave, ACKs are returned for the other bytes.
 * \param u8_addr  Slave I2C address
 * \param pu8_d Pointer to buffer for storing bytes read from slave
 * \param u8_cnt Number of bytes read from slave.
 * \hideinitializer
 * \sa ESOS_TASK_WAIT_ON_READ1I2C
 * \sa ESOS_TASK_WAIT_ON_READ2I2C
 * \sa __esos_i2c_hw_configI2C1
 *
 **/
#define   ESOS_TASK_WAIT_ON_READNI2C( u8_addr, pu8_d, u8_cnt )                     \
            ESOS_TASK_SPAWN_AND_WAIT( (ESOS_TASK_HANDLE)&__stChildTaskI2C, __esos_i2c_hw_readNI2C, (u8_addr), (pu8_d), (u8_cnt) )
 
/* P U B L I C  P R O T O T Y P E S *****************************************/

//I2C Operations
void __esos_i2c_hw_config(uint32_t u32_i2cbps);

//I2C Transactions
ESOS_CHILD_TASK( __esos_i2c_hw_writeN, uint8_t u8_addr, uint8_t* pu8_d, uint_t u8_cnt);
ESOS_CHILD_TASK( __esos_i2c_hw_readN, uint8_t u8_addr, uint8_t* pu8_d, uint8_t u8_cnt);
ESOS_CHILD_TASK( __esos_i2c_hw_writeNReadM, uint8_t u8_addr, uint8_t* pu8_wd, uint8_t u8_wcnt, uint8_t* pu8_rd, uint8_t u8_rcnt);


/** @} */
#endif // end ESOS_HWXXX_I2C_H
