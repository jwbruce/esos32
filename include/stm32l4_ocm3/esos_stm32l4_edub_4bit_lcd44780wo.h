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

#ifndef ESOS_STM32L4_EDUB_4BIT_LCDWO_H
#define ESOS_STM32L4_EDUB_4BIT_LCDWO_H

/**
 * \addtogroup ESOS_Task_LCD_Service_WO
 * @{
 */

/** \file
*  \brief This file contains routines which define, configure,
* and allow use of the write-only "LCD character display" service on
* the STMicroelectronics ARM-Cortex hardware targeting the Edubase-v2
* Communication with LCD display is via 6 GPIO pins (for RS,E, D4-D7)
* to implement the LCD nibble-wide data interface per the
* HD44780 informal specification
* 
*/

/* I N C L U D E S **********************************************************/
#include "esos.h"
#include "esos_lcd44780wo.h"
#include "esos_stm32l4.h"

/* D E F I N I T I O N S ****************************************************/
#define LCD_RS_PIN	GPIO12
#define LCD_RS_PORT	GPIOA
#define LCD_E_PIN	GPIO10
#define LCD_E_PORT	GPIOA
#define LCD_D4_PIN	GPIO1
#define LCD_D4_PORT	GPIOB
#define LCD_D5_PIN	GPIO2
#define LCD_D5_PORT	GPIOB
#define LCD_D6_PIN	GPIO3
#define LCD_D6_PORT	GPIOB
#define LCD_D7_PIN	GPIO4
#define LCD_D7_PORT	GPIOB



/* E X T E R N S ************************************************************/
extern volatile struct    stTask   __stLCDChildTask;

/* M A C R O S **************************************************************/

/* P U B L I C  P R O T O T Y P E S *****************************************/


/* HW-SPECIFIC MACROS *******************************************************/

/* P R I V A T E   P R O T O T Y P E S *****************************************/
void __esos_lcd44780_hw_config ( void );
ESOS_CHILD_TASK( __esos_lcd44780_hw_write_u8, uint8_t u8_data, BOOL b_isData);
#ifdef ESOS_USE_LCD_4BIT
void __esos_unsafe_lcd44780_hw_write_u4(uint8_t	u8_u4data, BOOL b_isEnable, BOOL b_isData) ;
#endif



#endif   //  ESOS_STM32L4_LCD_H
