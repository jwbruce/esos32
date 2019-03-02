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


#ifndef _ESOS_PIC24_I2C_H
#define _ESOS_PIC24_I2C_H


/**
 * \addtogroup ESOS_I2C_Service
 * @{
 */

// Documentation for this file. If the \file tag isn't present,
// this file won't be documented.
/** \file
 *  \brief This file contains routines which configure and
 *  use I<sup>2</sup>C on the Microchip PIC24 MCUs.
 */


/* I N C L U D E S **********************************************************/
#include "esos.h"
#include "esos_pic24.h"


/* D E F I N I T I O N S ****************************************************/
#ifndef I2C_ACK
#define I2C_ACK 0
#endif
#ifndef I2C_NAK
#define I2C_NAK 1
#endif

/* E X T E R N S ************************************************************/
extern struct stTask    __stChildTaskI2C, __stGrandChildTaskI2C;
extern uint8_t            __esos_i2c_dataBytes[2];                    // used to store arguments

/* M A C R O S **************************************************************/
#define I2C_WADDR(x) (x & 0xFE) //clear R/W bit of I2C addr
#define I2C_RADDR(x) (x | 0x01) //set R/W bit of I2C addr

#define ESOS_TASK_WAIT_ON_AVAILABLE_I2C()                                           \
        do {                                                                        \
        ESOS_TASK_WAIT_WHILE(__esos_IsSystemFlagSet(__ESOS_SYS_I2C_IS_BUSY));   \
        __esos_SetSystemFlag(__ESOS_SYS_I2C_IS_BUSY);                           \
    } while(0)

#define ESOS_TASK_SIGNAL_AVAILABLE_I2C() __esos_ClearSystemFlag(__ESOS_SYS_I2C_IS_BUSY)

// Macros to perform I2C operations within a child task
#define __PIC24_I2C1_START()                   \
   do{                                         \
      I2C1CONbits.SEN = 1;                     \
      ESOS_TASK_WAIT_WHILE( I2C1CONbits.SEN);  \
   }while(0)

#define __PIC24_I2C1_RSTART()                \
  do{                                        \
    I2C1CONbits.RSEN = 1;                    \
    ESOS_TASK_WAIT_WHILE( I2C1CONbits.RSEN); \
  }while(0)

#define __PIC24_I2C1_STOP()                  \
  do{                                        \
    I2C1CONbits.PEN = 1;                     \
    ESOS_TASK_WAIT_WHILE( I2C1CONbits.PEN);  \
  }while(0)

#define __PIC24_I2C1_PUT(byte)                   \
  do{                                            \
    I2C1TRN = (byte);                            \
    ESOS_TASK_WAIT_WHILE( I2C1STATbits.TRSTAT);  \
  }while(0)

#define   ESOS_TASK_WAIT_ON_GETI2C1( pu8_get, u8_ack2Send )                     \
            ESOS_TASK_SPAWN_AND_WAIT( (ESOS_TASK_HANDLE)&__stGrandChildTaskI2C, __esos_pic24_getI2C1, (pu8_get), (u8_ack2Send) )

// Must copy value to a var, send &var, just in case u8_d1 is a literal
/**
Transaction: Write 1 (ONE) byte stored in variable \em u8_d1 to I2C slave at address \em u8_addr.
\param u8_addr  Slave I2C address
\param u8_d1   Variable containing first byte to write
\sa ESOS_TASK_WAIT_ON_WRITE2I2C1
\sa ESOS_TASK_WAIT_ON_WRITENI2C1
\sa esos_pic24_configI2C1
\hideinitializer
*/
#define   ESOS_TASK_WAIT_ON_WRITE1I2C1( u8_addr, u8_d1 )                     \
    do{                                                                        \
      __esos_i2c_dataBytes[0] = (u8_d1);                                      \
      ESOS_TASK_SPAWN_AND_WAIT( (ESOS_TASK_HANDLE)&__stChildTaskI2C, __esos_pic24_writeNI2C1, (u8_addr), &__esos_i2c_dataBytes[0], 1 );  \
    }while(0)

// We needed a 2-byte array to be able to use the buffer.
// Otherwise, we'd need two writeN calls, which require
// child tasks, i.e. code size grows

/**
Transaction: Write 2 (TWO) bytes stored in variables \em u8_d1 and \em u8_d2 to I2C slave at address \em u8_addr.
\param u8_addr  Slave I2C address
\param u8_d1   Variable containing first byte to write
\param u8_d2   Variable containing second byte to write
\sa ESOS_TASK_WAIT_ON_WRITE1I2C1
\sa ESOS_TASK_WAIT_ON_WRITENI2C1
\hideinitializer
*/
#define   ESOS_TASK_WAIT_ON_WRITE2I2C1( u8_addr, u8_d1, u8_d2 )              \
    do{                                                                        \
      __esos_i2c_dataBytes[0] = (u8_d1);                                      \
      __esos_i2c_dataBytes[1] = (u8_d2);                                      \
      ESOS_TASK_SPAWN_AND_WAIT( (ESOS_TASK_HANDLE)&__stChildTaskI2C, __esos_pic24_writeNI2C1, (u8_addr), &__esos_i2c_dataBytes[0], 2 );    \
    }while(0)

/**
Transaction: Write \em u16_cnt bytes stored in buffer \em *pu8_d to I2C slave at address \em u8_addr.
\param u8_addr  Slave I2C address
\param pu8_d Pointer to buffer containing bytes to send
\param u16_cnt Number of bytes to send
\sa ESOS_TASK_WAIT_ON_WRITE1I2C1
\sa ESOS_TASK_WAIT_ON_WRITE2I2C1
\hideinitializer
*/
#define   ESOS_TASK_WAIT_ON_WRITENI2C1( u8_addr, pu8_d, u16_cnt )              \
            ESOS_TASK_SPAWN_AND_WAIT( (ESOS_TASK_HANDLE)&__stChildTaskI2C, __esos_pic24_writeNI2C1, (u8_addr), (pu8_d), (u16_cnt) )

/**
Transaction: Read 1 (ONE) byte from I2C slave at address \em u8_addr, and save to variable \em u8_d1
As per the I2C standard, a NAK is returned for the last byte read from the slave, ACKs are returned for the other bytes.
\param u8_addr  Slave I2C address
\param u8_d1   variable to hold the read byte
\hideinitializer
\sa ESOS_TASK_WAIT_ON_READ2I2C1
\sa ESOS_TASK_WAIT_ON_READNI2C1
\sa esos_pic24_configI2C1
*/
#define   ESOS_TASK_WAIT_ON_READ1I2C1( u8_addr, u8_d1 )                     \
            ESOS_TASK_SPAWN_AND_WAIT( (ESOS_TASK_HANDLE)&__stChildTaskI2C, __esos_pic24_readNI2C1, (u8_addr), &(u8_d1), 1 )

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
\sa ESOS_TASK_WAIT_ON_READ1I2C1
\sa ESOS_TASK_WAIT_ON_READNI2C1
\sa esos_pic24_configI2C1
*/
#define   ESOS_TASK_WAIT_ON_READ2I2C1( u8_addr, u8_d1, u8_d2 )                     \
     do{                                                                              \
       ESOS_TASK_SPAWN_AND_WAIT( (ESOS_TASK_HANDLE)&__stChildTaskI2C, __esos_pic24_readNI2C1, (u8_addr), &__esos_i2c_dataBytes[0], 2 );      \
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
\sa ESOS_TASK_WAIT_ON_READ1I2C1
\sa ESOS_TASK_WAIT_ON_READ2I2C1
\sa esos_pic24_configI2C1
*/
#define   ESOS_TASK_WAIT_ON_READNI2C1( u8_addr, pu8_d, u16_cnt )                     \
            ESOS_TASK_SPAWN_AND_WAIT( (ESOS_TASK_HANDLE)&__stChildTaskI2C, __esos_pic24_readNI2C1, (u8_addr), (pu8_d), (u16_cnt) )


/* P U B L I C  P R O T O T Y P E S *****************************************/

//I2C Operations
void esos_pic24_configI2C1(uint16_t u16_FkHZ);

//I2C Transactions
ESOS_CHILD_TASK( __esos_pic24_getI2C1, uint8_t* pu8_x, uint8_t u8_ack2Send);
ESOS_CHILD_TASK( __esos_pic24_writeNI2C1, uint8_t u8_addr, uint8_t* pu8_d, uint16_t u16_cnt);
ESOS_CHILD_TASK( __esos_pic24_readNI2C1, uint8_t u8_addr, uint8_t* pu8_d, uint16_t u16_cnt);

/** @} */
#endif // end ESOS_PIC24_I2C_H
