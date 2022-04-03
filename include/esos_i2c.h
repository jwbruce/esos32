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
* \addtogroup ESOS_I2C_Service
* @{
*/

/** \file
*  This file contains macros, prototypes, and definitions for the
*  I2C services for ESOS tasks.
*
*/

#ifndef ESOS_I2C_H
#define ESOS_I2C_H

/* I N C L U D E S **********************************************************/
#include <esos.h>

/* D E F I N E S ************************************************************/
#ifndef ESOS_I2C_ACK
#define ESOS_I2C_ACK 0
#endif
#ifndef ESOS_I2C_NAK
#define ESOS_I2C_NAK 1
#endif

/* M A C R O S **************************************************************/
#define I2C_WADDR(x) (x & 0xFE) //clear R/W bit of I2C addr
#define I2C_RADDR(x) (x | 0x01) //set R/W bit of I2C addr

/**
Current task waits until the ESOS I2C resource becomes available for use.

\sa ESOS_SIGNAL_AVAILABLE_I2C
\sa ESOS_IS_I2C_AVAILABLE
\sa ESOS_TASK_WAIT_ON_WRITE2I2C
\sa ESOS_TASK_WAIT_ON_WRITENI2C
\sa __esos_i2c_hw_config
\hideinitializer
*/
#define ESOS_TASK_WAIT_ON_AVAILABLE_I2C()                    \
        do {                                                 \
        ESOS_TASK_WAIT_UNTIL(ESOS_IS_I2C_AVAILABLE());       \
        __esos_SetSystemFlag(__ESOS_SYS_I2C_IN_USE);         \
    } while(0)

/**
Release ESOS I2C resource for use by other task.

\sa ESOS_TASK_WAIT_ON_AVAILABLE_I2C
\sa ESOS_IS_I2C_AVAILABLE
\sa ESOS_TASK_WAIT_ON_WRITE2I2C
\sa ESOS_TASK_WAIT_ON_WRITENI2C
\sa __esos_i2c_hw_config
\hideinitializer
*/
#define ESOS_SIGNAL_AVAILABLE_I2C() __esos_ClearSystemFlag(__ESOS_SYS_I2C_IN_USE)

/**
Returns TRUE if the ESOS I2C resource is available, else returns FALSE.
\retval  TRUE   ESOS I2C is not in use; available
\sa ESOS_TASK_WAIT_ON_AVAILABLE_I2C
\sa ESOS_SIGNAL_AVAILABLE_I2C 
\sa ESOS_TASK_WAIT_ON_WRITE2I2C
\sa ESOS_TASK_WAIT_ON_WRITENI2C
\sa __esos_i2c_hw_config
\hideinitializer
*/
#define ESOS_IS_I2C_AVAILABLE()     (__esos_IsSystemFlagClear(__ESOS_SYS_I2C_IN_USE))

/**
 *  \todo Should this be deprecated?
 * #define   ESOS_TASK_WAIT_ON_GETI2C( pu8_get, u8_ack2Send )                     \
 *            ESOS_TASK_SPAWN_AND_WAIT( (ESOS_TASK_HANDLE)&__stGrandChildTaskI2C, __esos_hw_getI2C, (pu8_get), (u8_ack2Send) )
 **/

// Must copy value to a var, send &var, just in case u8_d1 is a literal
/**
Transaction: Write 1 (ONE) byte stored in variable \em u8_d1 to I2C slave at address \em u8_addr.
\param u8_addr  Slave I2C address
\param u8_d1   Variable containing first byte to write
\sa ESOS_TASK_WAIT_ON_AVAILABLE_I2C
\sa ESOS_SIGNAL_AVAILABLE_I2C
\sa ESOS_IS_I2C_AVAILABLE 
\sa ESOS_TASK_WAIT_ON_WRITE2I2C
\sa ESOS_TASK_WAIT_ON_WRITENI2C
\sa __esos_i2c_hw_config
\hideinitializer
*/
#define   ESOS_TASK_WAIT_ON_WRITE1I2C( u8_addr, u8_d1 )                     \
    do{                                                                        \
      __esos_i2c_dataBytes[0] = (u8_d1);                                      \
      ESOS_TASK_SPAWN_AND_WAIT( (ESOS_TASK_HANDLE)&__stChildTaskI2C, __esos_i2c_hw_writeN, (u8_addr), &__esos_i2c_dataBytes[0], 1 );  \
    }while(0)

// We needed a 2-byte array to be able to use the buffer.
// Otherwise, we'd need two writeN calls, which require
// child tasks, i.e. code size grows

/**
Transaction: Write 2 (TWO) bytes stored in variables \em u8_d1 and \em u8_d2 to I2C slave at address \em u8_addr.
\param u8_addr  Slave I2C address
\param u8_d1   Variable containing first byte to write
\param u8_d2   Variable containing second byte to write
\sa ESOS_TASK_WAIT_ON_AVAILABLE_I2C
\sa ESOS_SIGNAL_AVAILABLE_I2C
\sa ESOS_IS_I2C_AVAILABLE 
\sa ESOS_TASK_WAIT_ON_WRITE1I2C
\sa ESOS_TASK_WAIT_ON_WRITENI2C
\sa __esos_i2c_hw_config
\hideinitializer
*/
#define   ESOS_TASK_WAIT_ON_WRITE2I2C( u8_addr, u8_d1, u8_d2 )              \
    do{                                                                        \
      __esos_i2c_dataBytes[0] = (u8_d1);                                      \
      __esos_i2c_dataBytes[1] = (u8_d2);                                      \
      ESOS_TASK_SPAWN_AND_WAIT( (ESOS_TASK_HANDLE)&__stChildTaskI2C,  __esos_i2c_hw_writeN, (u8_addr), &__esos_i2c_dataBytes[0], 2 );    \
    }while(0)

/**
Transaction: Write \em u16_cnt bytes stored in buffer \em *pu8_d to I2C slave at address \em u8_addr.
\param u8_addr  Slave I2C address
\param pu8_d Pointer to buffer containing bytes to send
\param u16_cnt Number of bytes to send
\sa ESOS_TASK_WAIT_ON_AVAILABLE_I2C
\sa ESOS_SIGNAL_AVAILABLE_I2C
\sa ESOS_IS_I2C_AVAILABLE 
\sa ESOS_TASK_WAIT_ON_WRITE1I2C
\sa ESOS_TASK_WAIT_ON_WRITE2I2C
\sa __esos_i2c_hw_config
\hideinitializer
*/
#define   ESOS_TASK_WAIT_ON_WRITENI2C( u8_addr, pu8_d, u8_cnt )              \
		do {																\
            ESOS_TASK_SPAWN_AND_WAIT( (ESOS_TASK_HANDLE)&__stChildTaskI2C, __esos_i2c_hw_writeN, (u8_addr), (pu8_d), (u8_cnt) );  \
		} while(0)

/**
Transaction: Read 1 (ONE) byte from I2C slave at address \em u8_addr, and save to variable \em u8_d1
As per the I2C standard, a NAK is returned for the last byte read from the slave, ACKs are returned for the other bytes.
\param u8_addr  Slave I2C address
\param u8_d1   variable to hold the read byte
\hideinitializer
\sa ESOS_TASK_WAIT_ON_AVAILABLE_I2C
\sa ESOS_SIGNAL_AVAILABLE_I2C
\sa ESOS_IS_I2C_AVAILABLE 
\sa ESOS_TASK_WAIT_ON_READ2I2C
\sa ESOS_TASK_WAIT_ON_READNI2C
\sa __esos_i2c_hw_config
*/
#define   ESOS_TASK_WAIT_ON_READ1I2C( u8_addr, u8_d1 )                     \
            ESOS_TASK_SPAWN_AND_WAIT( (ESOS_TASK_HANDLE)&__stChildTaskI2C,  __esos_i2c_hw_readN, (u8_addr), &(u8_d1), 1 )

// While READ1 still worked fine, READ2 needs a new trick, since u8_d1 and u8_d2 are
// not in consecutive memory locations.  Can't do two READN calls, because that (again)
// requires another child task.  Instead, just re-use our 2-byte array as a read buffer

/**
Transaction: Read 2 (TWO) bytes from I2C slave at address \em u8_addr, save to variables \em u8_d1 and \em u8_d2
As per the I2C standard, a NAK is returned for the last byte read from the slave, ACKs are returned for the other bytes.
\param u8_addr  Slave I2C address
\param u8_d1   variable to hold first read byte
\param u8_d2   variable to hold second read byte
\hideinitializer
\sa ESOS_TASK_WAIT_ON_AVAILABLE_I2C
\sa ESOS_SIGNAL_AVAILABLE_I2C
\sa ESOS_IS_I2C_AVAILABLE 
\sa ESOS_TASK_WAIT_ON_READ1I2C
\sa ESOS_TASK_WAIT_ON_READNI2C
\sa __esos_i2c_hw_config
*/
#define   ESOS_TASK_WAIT_ON_READ2I2C( u8_addr, u8_d1, u8_d2 )                     \
     do{                                                                              \
       ESOS_TASK_SPAWN_AND_WAIT( (ESOS_TASK_HANDLE)&__stChildTaskI2C, __esos_i2c_hw_readN, (u8_addr), &__esos_i2c_dataBytes[0], 2 );      \
       (u8_d1) = __esos_i2c_dataBytes[0];                                             \
       (u8_d2) = __esos_i2c_dataBytes[1];                                             \
     }while(0)

/**
Transaction: Read \em u16_cnt bytes from I2C slave at address \em u8_addr, save to buffer \em *pu8_d.
As per the I2C standard, a NAK is returned for the last byte read from the slave, ACKs are returned for the other bytes.
\param u8_addr  Slave I2C address
\param pu8_d Pointer to buffer for storing bytes read from slave
\param u16_cnt Number of bytes read from slave.
\hideinitializer
\sa ESOS_TASK_WAIT_ON_AVIALABLE_I2C
\sa ESOS_SIGNAL_AVIALABLE_I2C
\sa ESOS_TASK_WAIT_ON_READ1I2C
\sa ESOS_TASK_WAIT_ON_READ2I2C
\sa __esos_i2c_hw_config
*/
#define   ESOS_TASK_WAIT_ON_READNI2C( u8_addr, pu8_d, u8_cnt )                     \
		do {																\
            ESOS_TASK_SPAWN_AND_WAIT( (ESOS_TASK_HANDLE)&__stChildTaskI2C,  __esos_i2c_hw_readN, (u8_addr), (pu8_d), (u8_cnt) );	\
		} while (0)

/* S T R U C T U R E S ******************************************************/

/* P U B L I C  P R O T O T Y P E S *****************************************/
void __esos_i2c_config(uint32_t u32_i2cbps);

/* P R O T O T Y P E S  HARDWARE-SPECIFIC ********************************/
extern void __esos_i2c_hw_config(uint32_t u32_i2cbps);
extern ESOS_CHILD_TASK( __esos_hw_getI2C, uint8_t* pu8_x, uint8_t u8_ack2Send);
extern ESOS_CHILD_TASK( __esos_i2c_hw_writeN, uint8_t u8_addr, uint8_t* pu8_d, uint8_t u8_cnt);
extern ESOS_CHILD_TASK( __esos_i2c_hw_readN, uint8_t u8_addr, uint8_t* pu8_d, uint8_t u8_cnt);
// extern ESOS_CHILD_TASK( __esos_i2c_hw_writeMreadN, uint8_t u8_addr, uint8_t* pu8_wd, uint8_t u8_wcnt, uint8_t* pu8_rd, uint8_t u8_rcnt);

#endif  // ESOS_I2C_H



