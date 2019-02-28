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

#ifndef ESOS_hwxxx_LCD_H
#define ESOS_hwxxx_LCD_H

/**
 * \addtogroup ESOS_Task_LCD_Service
 * @{
 */
 
 /** \file
 *  \brief This file contains routines which define, configure,
 * and allow use of the "LCD character display" service on
 * the target hardware (hwxxxx)
 */

/* I N C L U D E S **********************************************************/
#include <esos_lcd.h>

/* HW-SPECIFIC MACROS *******************************************************/
#define  __esos_lcd44780_hw_config		__esos_lcd44780_hwxxx_config

#define  __esos_lcd44780_hw_setDataPins \
    __esos_lcd44780_hwxxx_setDataPins
#define  __esos_lcd44780_hw_getDataPins \
    __esos_lcd44780_hwxxx_getDataPins
#define  __esos_lcd44780_hw_configDataPinsAsInput \
    __esos_lcd44780_hwxxx_configDataPinsAsInput
#define  __esos_lcd44780_hw_configDataPinsAsOutput \
    __esos_lcd44780_hwxxx_configDataPinsAsOutput

#define  __ESOS_LCD44780_HW_SET_E_HIGH          __ESOS_LCD44780_HWXXX_SET_E_HIGH       // E=1
#define  __ESOS_LCD44780_HW_SET_E_LOW           __ESOS_LCD44780_HWXXX_SET_E_LOW        // E=0
#define  __ESOS_LCD44780_HW_SET_RW_READ         __ESOS_LCD44780_HWXXX_SET_RW_READ      // RW=1
#define  __ESOS_LCD44780_HW_SET_RW_WRITE        __ESOS_LCD44780_HWXXX_SET_RW_WRITE     // RW=0
#define  __ESOS_LCD44780_HW_SET_RS_REGISTERS    __ESOS_LCD44780_HWXXX_SET_RS_REGISTERS // RS=0
#define  __ESOS_LCD44780_HW_SET_RS_DATA         __ESOS_LCD44780_HWXXX_SET_RS_DATA      // RS=1

#define  __ESOS_LCD44780_HW_SET_D0              __ESOS_LCD44780_HWXXX_SET_D0
#define  __ESOS_LCD44780_HW_CLEAR_D0            __ESOS_LCD44780_HWXXX_CLEAR_D0
#define  __ESOS_LCD44780_HW_GET_D0              __ESOS_LCD44780_HWXXX_GET_D0
#define  __ESOS_LCD44780_HW_SET_D1              __ESOS_LCD44780_HWXXX_SET_D1
#define  __ESOS_LCD44780_HW_CLEAR_D1            __ESOS_LCD44780_HWXXX_CLEAR_D1
#define  __ESOS_LCD44780_HW_GET_D1              __ESOS_LCD44780_HWXXX_GET_D1
#define  __ESOS_LCD44780_HW_SET_D2              __ESOS_LCD44780_HWXXX_SET_D2
#define  __ESOS_LCD44780_HW_CLEAR_D2            __ESOS_LCD44780_HWXXX_CLEAR_D2
#define  __ESOS_LCD44780_HW_GET_D2              __ESOS_LCD44780_HWXXX_GET_D2
#define  __ESOS_LCD44780_HW_SET_D3              __ESOS_LCD44780_HWXXX_SET_D3
#define  __ESOS_LCD44780_HW_CLEAR_D3            __ESOS_LCD44780_HWXXX_CLEAR_D3
#define  __ESOS_LCD44780_HW_GET_D3              __ESOS_LCD44780_HWXXX_GET_D3
#define  __ESOS_LCD44780_HW_SET_D4              __ESOS_LCD44780_HWXXX_SET_D4
#define  __ESOS_LCD44780_HW_CLEAR_D4            __ESOS_LCD44780_HWXXX_CLEAR_D4
#define  __ESOS_LCD44780_HW_GET_D4              __ESOS_LCD44780_HWXXX_GET_D4
#define  __ESOS_LCD44780_HW_SET_D5              __ESOS_LCD44780_HWXXX_SET_D5
#define  __ESOS_LCD44780_HW_CLEAR_D5            __ESOS_LCD44780_HWXXX_CLEAR_D5
#define  __ESOS_LCD44780_HW_GET_D5              __ESOS_LCD44780_HWXXX_GET_D5
#define  __ESOS_LCD44780_HW_SET_D6              __ESOS_LCD44780_HWXXX_SET_D6
#define  __ESOS_LCD44780_HW_CLEAR_D6            __ESOS_LCD44780_HWXXX_CLEAR_D6
#define  __ESOS_LCD44780_HW_GET_D6              __ESOS_LCD44780_HWXXX_GET_D6
#define  __ESOS_LCD44780_HW_SET_D7              __ESOS_LCD44780_HWXXX_SET_D7
#define  __ESOS_LCD44780_HW_CLEAR_D7            __ESOS_LCD44780_HWXXX_CLEAR_D7
#define  __ESOS_LCD44780_HW_GET_D7              __ESOS_LCD44780_HWXXX_GET_D7

/* P R I V A T E   P R O T O T Y P E S *****************************************/
void __esos_lcd44780_hwxxx_config ( void );
void __esos_lcd44780_hwxxx_setDataPins( uint8_t u8_data);
uint8_t __esos_lcd44780_hwxxx_getDataPins( void );
void __esos_lcd44780_hwxxx_configDataPinsAsInput( void );
void __esos_lcd44780_hwxxx_configDataPinsAsOutput( void );

#endif
