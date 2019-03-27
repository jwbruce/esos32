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
* \addtogroup ESOS_Task_LCD_Service_WO
* @{
*/

/** \file
*  This file contains macros, prototypes, and definitions for the
*  simple (write-only) LCD services for ESOS tasks.
*
*/

#ifndef ESOS_LCDWO_H
#define ESOS_LCDWO_H

/* I N C L U D E S **********************************************************/
#include <esos.h>

/* D E F I N E S ************************************************************/
#define ESOS_LCD44780_CMD_CLEAR_DISPLAY          		0b00000001
#define ESOS_LCD44780_CMD_RETURN_HOME            		0b00000010
#define ESOS_LCD44780_CMD_ENTRY_MODE_SET         		0b00000100
#define ESOS_LCD44780_CMD_ENTRY_MODE_SET_INC     		0b00000010
#define ESOS_LCD44780_CMD_ENTRY_MODE_SET_SHIFT   		0b00000001
#define ESOS_LCD44780_CMD_DISPLAY_ON_OFF         		0b00001000
#define ESOS_LCD44780_CMD_DISPLAY_ON_OFF_DISPLAYON    	0b00000100
#define ESOS_LCD44780_CMD_DISPLAY_ON_OFF_CURSORON		0b00000010
#define ESOS_LCD44780_CMD_DISPLAY_ON_OFF_BLINKCURSOR	0b00000001
#define ESOS_LCD44780_CMD_CUR_DISP_CURSOR_SHIFT_L  		0b00010000
#define ESOS_LCD44780_CMD_CUR_DISP_CURSOR_SHIFT_R  		0b00010100
#define ESOS_LCD44780_CMD_CUR_DISP_DISPLAY_SHIFT_L 		0b00011000
#define ESOS_LCD44780_CMD_CUR_DISP_DISPLAY_SHIFT_R 		0b00011100
#define ESOS_LCD44780_CMD_FUNCTION_SET           		0b00100000
#define ESOS_LCD44780_CMD_FUNCTION_SET_8BIT_MODE   		0b00010000
#define ESOS_LCD44780_CMD_FUNCTION_SET_4BIT_MODE   		0b00000000
#define ESOS_LCD44780_CMD_FUNCTION_SET_1LINE_MODE   	0b00000000
#define ESOS_LCD44780_CMD_FUNCTION_SET_2LINE_MODE  		0b00001000
#define ESOS_LCD44780_CMD_FUNCTION_SET_LOWFONT_MODE   	0b00000000
#define ESOS_LCD44780_CMD_FUNCTION_SET_HIFONT_MODE 		0b00000100
#define ESOS_LCD44780_CMD_SET_CGRAM_ADDR         		0b01000000
#define ESOS_LCD44780_CMD_SET_DDRAM_ADDR         		0b10000000
// HD44780 chipsets implement 40 characters in the IC because 40 wide
// displays are available.  The physically connected LCD may be (much)
// smaller in width, but the memory is still there.  Some applications
// exploit this fact for "off-screen" rendering or buffering.
// Furthermore, the display "shift" commands facilitate this behavior.
// The ESOS LCD character service will implement LCD memory as 40
// characters wide to allow apps to work this way.
#define ESOS_LCD44780_MEM_WIDTH                  40
#define ESOS_LCD44780_MEM_HEIGHT                 2
#define ESOS_LCD44780_NUM_CUSTOM_CHARS           8

#define LCD44780_READ                       1
#define LCD44780_WRITE                      0
#define LCD44780_DATA                       TRUE
#define LCD44780_COMMANDS                   FALSE


#define ESOS_TASK_WAIT_ON_LCD44780_REFRESH()		ESOS_TASK_WAIT_UNTIL(esos_lcd44780_isCurrent())

#define ESOS_TASK_WAIT_LCD44780_WRITE_COMMAND(u8_cmd) do { \
    ESOS_TASK_SPAWN_AND_WAIT( (ESOS_TASK_HANDLE) &__stLCDChildTask, __esos_lcd44780_hw_write_u8, u8_cmd, LCD44780_COMMANDS ); \
} while(0)

#define ESOS_TASK_WAIT_LCD44780_WRITE_COMMAND_NOWAIT(u8_cmd) do { \
    ESOS_TASK_SPAWN_AND_WAIT( (ESOS_TASK_HANDLE) &__stLCDChildTask, __esos_lcd44780_hw_write_u8, u8_cmd, LCD44780_COMMANDS ); \
} while(0)

#define ESOS_TASK_WAIT_LCD44780_SET_CG_ADDRESS(u8_addr) \
    ESOS_TASK_WAIT_LCD44780_WRITE_COMMAND( u8_addr | ESOS_LCD44780_CMD_SET_CGRAM_ADDR )

#define ESOS_TASK_WAIT_LCD44780_SET_DATA_ADDRESS(u8_addr) \
    ESOS_TASK_WAIT_LCD44780_WRITE_COMMAND( u8_addr | ESOS_LCD44780_CMD_SET_DDRAM_ADDR )

#define ESOS_TASK_WAIT_LCD44780_WRITE_DATA(u8_data) do { \
    ESOS_TASK_SPAWN_AND_WAIT( (ESOS_TASK_HANDLE) &__stLCDChildTask, __esos_lcd44780_hw_write_u8, u8_data, LCD44780_DATA ); \
} while(0)


/* S T R U C T U R E S ******************************************************/
typedef struct {
  uint8_t au8_data[8];
} esos_lcd44780_char_t;

/* P U B L I C  P R O T O T Y P E S *****************************************/
void esos_lcd44780_configDisplay( void );
void __esos_lcd44780_init( void );
void esos_lcd44780_clearScreen( void );
void esos_lcd44780_setCursorHome( void );
void esos_lcd44780_setCursor( uint8_t u8_row, uint8_t u8_column );
void esos_lcd44780_writeChar( uint8_t u8_row, uint8_t u8_column, uint8_t u8_data );
uint8_t esos_lcd44780_getChar( uint8_t u8_row, uint8_t u8_column );
void esos_lcd44780_writeBuffer( uint8_t u8_row, uint8_t u8_column, uint8_t *pu8_data, uint8_t u8_bufflen );
void esos_lcd44780_getBuffer( uint8_t u8_row, uint8_t u8_column, uint8_t *pu8_data, uint8_t u8_bufflen );
void esos_lcd44780_writeString( uint8_t u8_row, uint8_t u8_column, char *psz_data );
void esos_lcd44780_setCursorDisplay( BOOL u8_state );
BOOL esos_lcd44780_getCursorDisplay( void );
void esos_lcd44780_setCursorBlink( BOOL u8_state );
BOOL esos_lcd44780_getCursorBlink( void );
void esos_lcd44780_setDisplayVisible( BOOL u8_state );
BOOL esos_lcd44780_getDisplayVisible( void );
void esos_lcd44780_setCustomChar( uint8_t u8_charSlot, uint8_t *pu8_charData );
void esos_lcd44780_getCustomChar( uint8_t u8_charSlot, uint8_t *pu8_charData );
BOOL esos_lcd44780_isCurrent( void );

ESOS_USER_TASK( __esos_lcd44780_service );

/* P R O T O T Y P E S  HARDWARE-SPECIFIC ********************************/
extern ESOS_CHILD_TASK( __esos_lcd44780_hw_write_u8, uint8_t u8_data, BOOL b_isData);
extern void __esos_lcd44780_hw_config(void);

#ifdef ESOS_USE_LCD_4BIT
extern void __esos_unsafe_lcd44780_hw_write_u4(uint8_t	u8_u4data, BOOL b_isEnable, BOOL b_isData);
#endif

#endif  // ESOS_LCDWO_H
