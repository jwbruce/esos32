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

/*** I N C L U D E S *************************************************/
#include "esos_hwxxx_i2c.h"

/*** G L O B A L S *************************************************/
struct stTask   __stChildTaskI2C, __stGrandChildTaskI2C;
uint8_t           __esos_i2c_dataBytes[2];

/*** T H E   C O D E *************************************************/

/*********************************************************
 * Public functions intended to be called by other files *
 *********************************************************/

// Documentation for this file. If the \file tag is not present,
// this file will not be documented.
// Note: place this comment below the #if NUM_I2C_MODS so Doxygen
// will only see it once.
/** \file
 *  I2C support functions. \see pic24_i2c.h for details.
 */


/**
Configure and enable the I2C1 module for operation at \em u16_fKHz KHz clock speed.
\param u16_fKHz specifies clock speed in KHz
\sa ESOS_TASK_WAIT_ON_WRITENI2C1
\sa ESOS_TASK_WAIT_ON_READNI2C1
\hideinitializer
 */
void esos_hwxxx_configI2C1(uint16_t u16_fKHz) {
  uint32_t u32_temp;

  u32_temp = (FCY/1000L)/((uint32_t) u16_fKHz);
  u32_temp = u32_temp - FCY/10000000L - 1;
  I2C1BRG = u32_temp;
  I2C1CONbits.I2CEN = 1;
}

ESOS_CHILD_TASK( __esos_hwxxx_getI2C1, uint8_t* pu8_x, uint8_t u8_ack2Send) {
  static uint8_t        u8_tempAck;

  ESOS_TASK_BEGIN();
  u8_tempAck = u8_ack2Send;

  ESOS_TASK_WAIT_WHILE(I2C1CON & 0x1F);           //wait for idle condition
  I2C1CONbits.RCEN = 1;                           //enable receive
  ESOS_TASK_WAIT_UNTIL(I2C1STATbits.RBF);         //wait for receive byte
  *pu8_x = I2C1RCV;                               //read byte;
  //wait for idle condition before attempting ACK
  ESOS_TASK_WAIT_WHILE(I2C1CON & 0x1F);           //lower 5 bits must be 0
  I2C1CONbits.ACKDT = u8_tempAck;                 //ACK bit to send back on receive
  I2C1CONbits.ACKEN = 1;                          //enable ACKbit transmittion
  ESOS_TASK_WAIT_WHILE(I2C1CONbits.ACKEN);        //wait for completion
  ESOS_TASK_END();
} // end __esos_hwxxx_getI2C1

/**
Transaction: Write \em u16_cnt bytes stored in buffer \em *pu8_d to I2C slave at address \em u8_addr.
\param u8_addr  Slave I2C address
\param pu8_d Pointer to buffer containing bytes to send
\param u16_cnt Number of bytes to send
 */
ESOS_CHILD_TASK( __esos_hwxxx_writeNI2C1, uint8_t u8_addr, uint8_t* pu8_d, uint16_t u16_cnt) {
  static uint8_t        u8_tempAddr;
  static uint8_t*       pu8_tempPtr;
  static uint16_t       u16_tempCnt, u16_i;

  ESOS_TASK_BEGIN();
  u8_tempAddr=u8_addr;
  pu8_tempPtr=pu8_d;
  u16_tempCnt=u16_cnt;
  __HWXXX_I2C1_START();
  __HWXXX_I2C1_PUT(I2C_WADDR(u8_tempAddr));
  for (u16_i=0; u16_i < u16_tempCnt; u16_i++) {
    __HWXXX_I2C1_PUT(*pu8_tempPtr);
    pu8_tempPtr++;
  }
  __HWXXX_I2C1_STOP();
  ESOS_TASK_END();
} // end __esos_hwxxx_writeNI2C1

/**
Transaction: Read \em u16_cnt bytes from I2C slave at address \em u8_addr, save to buffer \em *pu8_d.
As per the I2C standard, a NAK is returned for the last byte read from the slave, ACKs are returned for the other bytes.
\param u8_addr  Slave I2C address
\param pu8_d Pointer to buffer for storing bytes read from slave
\param u16_cnt Number of bytes read from slave.
 */
ESOS_CHILD_TASK( __esos_hwxxx_readNI2C1, uint8_t u8_addr, uint8_t* pu8_d, uint16_t u16_cnt) {
  static uint8_t        u8_tempAddr;
  static uint8_t*       pu8_tempD;
  static uint16_t       u16_tempCnt, u16_i;

  ESOS_TASK_BEGIN();
  u8_tempAddr=u8_addr;
  pu8_tempD=pu8_d;
  u16_tempCnt=u16_cnt;
  __HWXXX_I2C1_START();
  __HWXXX_I2C1_PUT(I2C_RADDR(u8_tempAddr));
  for (u16_i=0; u16_i < u16_tempCnt-1; u16_i++) {
    ESOS_TASK_WAIT_ON_GETI2C1(pu8_tempD, I2C_ACK);
    pu8_tempD++;
  }
  ESOS_TASK_WAIT_ON_GETI2C1(pu8_tempD, I2C_NAK);
  __HWXXX_I2C1_STOP();
  ESOS_TASK_END();
  /** @} */
} // end __esos_hwxxx_readNI2C1

