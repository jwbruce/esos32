/*
 * "Copyright (c) 2020 J. W. Bruce ("AUTHOR(S)")"
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

#ifndef _ESOS_STM32L4_SPI_H
#define _ESOS_STM32L4_SPI_H


/**
 * \addtogroup ESOS_SPI_Service
 * @{
 */

// Documentation for this file. If the \file tag isn't present,
// this file won't be documented.
/** \file
*  \brief This file contains routines which define, configure and
*  allow use of the SPI service on the ST Microelectronics
* STM32L452RE MCU
*/


/* I N C L U D E S **********************************************************/
#include "esos.h"
#include "esos_spi.h"
#include "esos_stm32l4.h"
#include <libopencm3/stm32/spi.h>

/* D E F I N I T I O N S ****************************************************/
#define	__ESOS_SPI_STM32L4_IS_BUSY()			( SPI_SR(SPI1) & SPI_SR_BSY )
#define	__ESOS_SPI_STM32L4_IS_OVR()				( SPI_SR(SPI1) & SPI_SR_OVR )
#define	__ESOS_SPI_STM32L4_IS_TXE()				( SPI_SR(SPI1) & SPI_SR_TXE )
#define	__ESOS_SPI_STM32L4_IS_RXNE()			( SPI_SR(SPI1) & SPI_SR_RXNE )



/* E X T E R N S ************************************************************/


/* M A C R O S **************************************************************/

/* P U B L I C  P R O T O T Y P E S *****************************************/
//I2C Operations
void __esos_spi_hw_config(uint32_t u32_spibps);

//I2C Transactions
ESOS_CHILD_TASK( __esos_spi_hw_xferNSPI1, uint16_t* pu16_out, uint16_t* pu16_in, uint16_t u16_cnt);
/** @} */
#endif // end ESOS_STM32L4_SPI_H
