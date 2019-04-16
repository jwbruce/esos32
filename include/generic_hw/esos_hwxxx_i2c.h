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
 
/* P U B L I C  P R O T O T Y P E S *****************************************/

//I2C Operations
void __esos_i2c_hw_config(uint32_t u32_i2cbps);

//I2C Transactions
ESOS_CHILD_TASK( __esos_i2c_hw_writeN, uint8_t u8_addr, uint8_t* pu8_d, uint_t u8_cnt);
ESOS_CHILD_TASK( __esos_i2c_hw_readN, uint8_t u8_addr, uint8_t* pu8_d, uint8_t u8_cnt);
ESOS_CHILD_TASK( __esos_i2c_hw_writeNReadM, uint8_t u8_addr, uint8_t* pu8_wd, uint8_t u8_wcnt, uint8_t* pu8_rd, uint8_t u8_rcnt);


/** @} */
#endif // end ESOS_HWXXX_I2C_H
