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
 
#include <esos.h>
#include "esos_lcd44780wo.h"

// ******** G L O B A L S ***************
volatile struct    stTask   __stLCDChildTask;

// Main data structure for updating lcd44780
struct {
	BOOL b_cursorPositionNeedsUpdate;
	uint8_t u8_cursorRow;
	uint8_t u8_cursorCol;

	BOOL b_cursorShownNeedsUpdate;
	BOOL b_cursorShown;

	BOOL b_cursorBlinkNeedsUpdate;
	BOOL b_cursorBlink;

	BOOL b_displayVisibleNeedsUpdate;
	BOOL b_displayVisible;

	BOOL ab_lcdBufferNeedsUpdate[ESOS_LCD44780_MEM_HEIGHT][ESOS_LCD44780_MEM_WIDTH];
	char aac_lcdBuffer[ESOS_LCD44780_MEM_HEIGHT][ESOS_LCD44780_MEM_WIDTH];

	BOOL ab_customCharNeedsUpdate[ESOS_LCD44780_NUM_CUSTOM_CHARS];
	esos_lcd44780_char_t ast_customChar[ESOS_LCD44780_NUM_CUSTOM_CHARS];
} esos_lcd44780_vars;

// Hidden LCD character moduel service/housekeeping task
ESOS_USER_TASK( __esos_lcd44780_service )
{
	// Can first initialized the LCD module hardware and setup the LCD service software structures
	// Also manages the LCD character module got ESOS
	// The LCD service hidden task will need to maintain a buffer containing the LCD character display
	ESOS_TASK_BEGIN();
    ESOS_TASK_WAIT_TICKS(500);			// Wait > 40 msec after power is applied
    
#ifdef ESOS_USE_LCD_8BIT
	// TODO: remove the magic numbers in this section
	ESOS_TASK_WAIT_LCD44780_WRITE_COMMAND_NOWAIT(	ESOS_LCD44780_CMD_FUNCTION_SET +    \
											ESOS_LCD44780_CMD_FUNCTION_SET_8BIT_MODE + \
											ESOS_LCD44780_CMD_FUNCTION_SET_2LINE_MODE + \
											ESOS_LCD44780_CMD_FUNCTION_SET_LOWFONT_MODE );		// 0x30
	ESOS_TASK_WAIT_TICKS(10);			// must wait 5ms, busy flag not available
	ESOS_TASK_WAIT_LCD44780_WRITE_COMMAND_NOWAIT(	ESOS_LCD44780_CMD_FUNCTION_SET +    \
											ESOS_LCD44780_CMD_FUNCTION_SET_8BIT_MODE + \
											ESOS_LCD44780_CMD_FUNCTION_SET_2LINE_MODE + \
											ESOS_LCD44780_CMD_FUNCTION_SET_LOWFONT_MODE );		// 0x30

	ESOS_TASK_WAIT_TICKS(1);			// must wait 160us, busy flag not available
	ESOS_TASK_WAIT_LCD44780_WRITE_COMMAND_NOWAIT(	ESOS_LCD44780_CMD_FUNCTION_SET +    \
											ESOS_LCD44780_CMD_FUNCTION_SET_8BIT_MODE + \
											ESOS_LCD44780_CMD_FUNCTION_SET_2LINE_MODE + \
											ESOS_LCD44780_CMD_FUNCTION_SET_LOWFONT_MODE );		// 0x30

	ESOS_TASK_WAIT_TICKS(1);			// must wait 160us, busy flag not available
	ESOS_TASK_WAIT_LCD44780_WRITE_COMMAND(ESOS_LCD44780_CMD_FUNCTION_SET +    \
											ESOS_LCD44780_CMD_FUNCTION_SET_8BIT_MODE + \
											ESOS_LCD44780_CMD_FUNCTION_SET_2LINE_MODE + \
											ESOS_LCD44780_CMD_FUNCTION_SET_LOWFONT_MODE );		// 0x38
#endif

#ifdef ESOS_USE_LCD_4BIT
	// TODO: remove the magic numbers in this section
	// see p.45 figure 23 in hitachi HD44780 datasheet
	__esos_unsafe_lcd44780_hw_write_u4( 0x30, FALSE, FALSE) ;	// write a command (RS=0) nibble 0x30 with E low
	ESOS_TASK_WAIT_TICKS(5);			// must wait 5ms, busy flag not available
	// Now "clock" in command 0x30
	__esos_unsafe_lcd44780_hw_write_u4( 0x30, TRUE, FALSE) ;	// write a command (RS=0) nibble 0x30 with E HIGH
	__esos_unsafe_lcd44780_hw_write_u4( 0x30, FALSE, FALSE) ;	// write a command (RS=0) nibble 0x30 with E LOW
	ESOS_TASK_WAIT_TICKS(5);			// must wait 5ms, busy flag not available
	// Now "clock" in command 0x30 a second time
	__esos_unsafe_lcd44780_hw_write_u4( 0x30, TRUE, FALSE) ;	// write a command (RS=0) nibble 0x30 with E HIGH
	__esos_unsafe_lcd44780_hw_write_u4( 0x30, FALSE, FALSE) ;	// write a command (RS=0) nibble 0x30 with E LOW
	ESOS_TASK_WAIT_TICKS(5);			// must wait 5ms, busy flag not available
	// Now "clock" in command 0x30 a third
	__esos_unsafe_lcd44780_hw_write_u4( 0x30, TRUE, FALSE) ;	// write a command (RS=0) nibble 0x30 with E HIGH
	__esos_unsafe_lcd44780_hw_write_u4( 0x30, FALSE, FALSE) ;	// write a command (RS=0) nibble 0x30 with E LOW
	ESOS_TASK_WAIT_TICKS(5);			// must wait 5ms, busy flag not available
	// Now "clock" in command 0x20 to command LCD into 4-bit mode
	__esos_unsafe_lcd44780_hw_write_u4( 0x20, TRUE, FALSE) ;	// write a command (RS=0) nibble 0x30 with E HIGH
	__esos_unsafe_lcd44780_hw_write_u4( 0x20, FALSE, FALSE) ;	// write a command (RS=0) nibble 0x30 with E LOW
	ESOS_TASK_WAIT_TICKS(1);			// wait a smidge
	// send it 4-bit mode with two lines and font selection
	ESOS_TASK_WAIT_LCD44780_WRITE_COMMAND(ESOS_LCD44780_CMD_FUNCTION_SET +    \
											ESOS_LCD44780_CMD_FUNCTION_SET_4BIT_MODE + \
											ESOS_LCD44780_CMD_FUNCTION_SET_2LINE_MODE + \
											ESOS_LCD44780_CMD_FUNCTION_SET_LOWFONT_MODE );		// 0x28
	
#endif

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




	ESOS_TASK_WAIT_LCD44780_WRITE_COMMAND(ESOS_LCD44780_CMD_CUR_DISP_CURSOR_SHIFT_R);												// 0x10
	//ESOS_TASK_WAIT_LCD44780_WRITE_COMMAND(ESOS_LCD44780_CMD_DISPLAY_ON_OFF + ESOS_LCD44780_CMD_DISPLAY_ON_OFF_DISP);		// 0x0C
	//ESOS_TASK_WAIT_LCD44780_WRITE_COMMAND(ESOS_LCD44780_CMD_ENTRY_MODE_SET+ESOS_LCD44780_CMD_ENTRY_MODE_SET_INC);			// 0x06

	// Send startup sequence from datasheet
	
	ESOS_TASK_WAIT_LCD44780_WRITE_COMMAND(  ESOS_LCD44780_CMD_DISPLAY_ON_OFF + \
											ESOS_LCD44780_CMD_DISPLAY_ON_OFF_DISPLAYON );

	
	//ESOS_TASK_WAIT_LCD44780_WRITE_COMMAND(  ESOS_LCD44780_CMD_DISPLAY_ON_OFF + \
    //                                        ESOS_LCD44780_CMD_DISPLAY_ON_OFF_DISPLAYON + \
    //                                        ESOS_LCD44780_CMD_DISPLAY_ON_OFF_CURSORON + \
    //                                        ESOS_LCD44780_CMD_DISPLAY_ON_OFF_BLINKCURSOR);
	
	ESOS_TASK_WAIT_LCD44780_WRITE_COMMAND(  ESOS_LCD44780_CMD_ENTRY_MODE_SET |
                                            ESOS_LCD44780_CMD_ENTRY_MODE_SET_INC);

	while(TRUE) {
		static uint8_t i, u8_col, u8_row;

		if(esos_lcd44780_vars.b_cursorPositionNeedsUpdate) {
			esos_lcd44780_vars.b_cursorPositionNeedsUpdate = FALSE;
			ESOS_TASK_WAIT_LCD44780_SET_DATA_ADDRESS(esos_lcd44780_vars.u8_cursorRow > 0 ? 0x40 : 0x00 |
			                                    esos_lcd44780_vars.u8_cursorCol);
		}

		if(esos_lcd44780_vars.b_cursorShownNeedsUpdate ||
		   esos_lcd44780_vars.b_cursorBlinkNeedsUpdate ||
		   esos_lcd44780_vars.b_displayVisibleNeedsUpdate) {
			esos_lcd44780_vars.b_cursorShownNeedsUpdate = FALSE;
			esos_lcd44780_vars.b_cursorBlinkNeedsUpdate = FALSE;
			esos_lcd44780_vars.b_displayVisibleNeedsUpdate = FALSE;
			ESOS_TASK_WAIT_LCD44780_WRITE_COMMAND( ESOS_LCD44780_CMD_DISPLAY_ON_OFF |
				(esos_lcd44780_vars.b_cursorShown ? ESOS_LCD44780_CMD_DISPLAY_ON_OFF_CURSORON : 0) |
				(esos_lcd44780_vars.b_cursorBlink ? ESOS_LCD44780_CMD_DISPLAY_ON_OFF_BLINKCURSOR : 0) |
				(esos_lcd44780_vars.b_displayVisible ? ESOS_LCD44780_CMD_DISPLAY_ON_OFF_DISPLAYON : 0));
		}

		for(u8_row = 0; u8_row < ESOS_LCD44780_MEM_HEIGHT; ++u8_row) {
			for(u8_col = 0; u8_col < ESOS_LCD44780_MEM_WIDTH; ++u8_col) {
				// Update all consecutive characters that need it, but only send the address once.
				if(esos_lcd44780_vars.ab_lcdBufferNeedsUpdate[u8_row][u8_col]) {
					esos_lcd44780_vars.b_cursorPositionNeedsUpdate = TRUE;
					ESOS_TASK_WAIT_LCD44780_SET_DATA_ADDRESS((u8_row > 0 ? 0x40 : 0x00) | u8_col);
					do {
						esos_lcd44780_vars.ab_lcdBufferNeedsUpdate[u8_row][u8_col] = FALSE;
						ESOS_TASK_WAIT_LCD44780_WRITE_DATA(esos_lcd44780_vars.aac_lcdBuffer[u8_row][u8_col]);
						++u8_col;
						ESOS_TASK_YIELD();
					} while(esos_lcd44780_vars.ab_lcdBufferNeedsUpdate[u8_row][u8_col]
					        && u8_col < ESOS_LCD44780_MEM_WIDTH);
				}
				ESOS_TASK_YIELD();
			}
		}

		for(i = 0; i < ESOS_LCD44780_NUM_CUSTOM_CHARS; ++i) {
			// Update all characters that need updating, but only send the CG address once.
			if(esos_lcd44780_vars.ab_customCharNeedsUpdate[i]) {
				// We destroy the hardware's cursor position, so fix it later.
				esos_lcd44780_vars.b_cursorPositionNeedsUpdate = TRUE;
				ESOS_TASK_WAIT_LCD44780_SET_CG_ADDRESS(8 * i);

				do {
					static int n;

					esos_lcd44780_vars.ab_customCharNeedsUpdate[i] = FALSE;

					for(n = 0; n < 8; ++n) {
						ESOS_TASK_WAIT_LCD44780_WRITE_DATA(esos_lcd44780_vars.ast_customChar[i].au8_data[n]);
					}
					++i;
					ESOS_TASK_YIELD();
				} while(esos_lcd44780_vars.ab_customCharNeedsUpdate[i] && i < ESOS_LCD44780_NUM_CUSTOM_CHARS);
			}
			ESOS_TASK_YIELD();
		}

		ESOS_TASK_YIELD();
	}

	ESOS_TASK_END();
}

void esos_lcd44780_configDisplay( void )
{
	unsigned int u8_column;

	esos_lcd44780_vars.b_displayVisible = TRUE;
	esos_lcd44780_vars.b_displayVisibleNeedsUpdate = TRUE;
	esos_lcd44780_vars.b_cursorShown = FALSE;
	esos_lcd44780_vars.b_cursorShownNeedsUpdate = TRUE;
	esos_lcd44780_vars.b_cursorBlink = FALSE;
	esos_lcd44780_vars.b_cursorBlinkNeedsUpdate = TRUE;

	esos_lcd44780_clearScreen();

	for(u8_column = 0; u8_column < ESOS_LCD44780_NUM_CUSTOM_CHARS; ++u8_column) {
		esos_lcd44780_vars.ab_customCharNeedsUpdate[u8_column] = FALSE;
	}
}

void __esos_lcd44780_init( void )
{
	// give HW specific code a chance to do anything else to init/config
	__esos_lcd44780_hw_config();

	// install our LCD housekeeping task into the scheduler
	esos_RegisterTask( __esos_lcd44780_service );
}

void esos_lcd44780_clearScreen( void )
{
	uint8_t u8_height;
	uint8_t u8_width;
	// Clears the buffer
	for(u8_height = 0; u8_height < ESOS_LCD44780_MEM_HEIGHT; u8_height++){
		for(u8_width = 0; u8_width < ESOS_LCD44780_MEM_WIDTH; u8_width++){
			esos_lcd44780_vars.aac_lcdBuffer[u8_height][u8_width] = ' ';
			esos_lcd44780_vars.ab_lcdBufferNeedsUpdate[u8_height][u8_width] = TRUE;
		}
	}

	esos_lcd44780_setCursor(0,0);
	esos_lcd44780_vars.b_cursorPositionNeedsUpdate = TRUE; 
}

void esos_lcd44780_setCursorHome( void )
{
	esos_lcd44780_setCursor(0,0);
	esos_lcd44780_vars.b_cursorPositionNeedsUpdate = TRUE; 
}

void esos_lcd44780_setCursor( uint8_t u8_row, uint8_t u8_column )
{
    // Move cursor to (u8_row,u8_column) without changing memory buffer or the display
	// TODO:  Write hardware-independent code here
}

void esos_lcd44780_writeChar( uint8_t u8_row, uint8_t u8_column, uint8_t u8_data )
{
	esos_lcd44780_vars.aac_lcdBuffer[u8_row][u8_column] = u8_data;
	esos_lcd44780_vars.ab_lcdBufferNeedsUpdate[u8_row][u8_column] = TRUE;
}

uint8_t esos_lcd44780_getChar( uint8_t u8_row, uint8_t u8_column )
{
	return esos_lcd44780_vars.aac_lcdBuffer[u8_row][u8_column];
}

void esos_lcd44780_writeBuffer( uint8_t u8_row, uint8_t u8_column, uint8_t *pu8_data, uint8_t u8_bufflen ) {
	// Write u8_bufflen characters from pu8_data to (u8_row,u8_column)
	//This should ensure that the buffer is written in sequentially left to right
	// and that the buffer is never written to past its length or width.
	uint8_t i;
    for(i = 0; i < u8_bufflen; i++){
        if(u8_column + i >= 8){
            u8_row = !u8_row;
        }
        esos_lcd44780_vars.aac_lcdBuffer[(u8_row)][(u8_column+i)%ESOS_LCD44780_MEM_WIDTH] = pu8_data[i];
        esos_lcd44780_vars.ab_lcdBufferNeedsUpdate[(u8_row)][(u8_column+i)%ESOS_LCD44780_MEM_WIDTH] = TRUE;
    }
    return;
} 

void esos_lcd44780_getBuffer( uint8_t u8_row, uint8_t u8_column, uint8_t *pu8_data, uint8_t u8_bufflen ) {
    // Return pu8_data with u8_bufflen characters currently displayed beginning at (u8_row,u8_column)
	// This should ensure that the buffer is read out sequentially left to right
	// and that the buffer is never read past its length or width.
	uint8_t i;
    for(i = 0; i < u8_bufflen; i++){
        if(u8_row + i >= ESOS_LCD44780_MEM_WIDTH){
            u8_column = (u8_column + 1) % ESOS_LCD44780_MEM_WIDTH;
        }
        pu8_data[i] = esos_lcd44780_vars.aac_lcdBuffer[(u8_row+i)%ESOS_LCD44780_MEM_HEIGHT][u8_column];
    }
    return;
}

void esos_lcd44780_writeString( uint8_t u8_row, uint8_t u8_column, char *psz_data ) {
    // Write zero-terminated string psz_data to location starting at (u8_row,u8_column)
	// This should ensure that the zero-terminated string is written sequentially left to right
	// and that the buffer is never written to past its length or width.
   
    // Find length of zero-terminated string so that we can implement like buffer
    uint8_t u8_pszbufflen = 0;
    uint8_t i;
    while(psz_data[u8_pszbufflen] != '\0'){
        u8_pszbufflen++;
    }
    
    //replica code from esos_lcd44780_writeBuffer with updated variable names
    for(i = 0; i < u8_pszbufflen; i++){
        if(u8_row + i >= ESOS_LCD44780_MEM_WIDTH){
            u8_column = (u8_column + 1) % ESOS_LCD44780_MEM_WIDTH;
        }
        esos_lcd44780_vars.aac_lcdBuffer[(u8_row)][(u8_column+i)%ESOS_LCD44780_MEM_WIDTH] = psz_data[i];
        esos_lcd44780_vars.ab_lcdBufferNeedsUpdate[(u8_row)][(u8_column+i)%ESOS_LCD44780_MEM_WIDTH] = TRUE;
    }
    return;
}

void esos_lcd44780_setCursorDisplay( BOOL u8_state )
{
    // Set cursor display state to u8_state
	// TODO:  Write hardware-independent code here

    esos_lcd44780_vars.b_cursorShown = u8_state;
    esos_lcd44780_vars.b_cursorShownNeedsUpdate = TRUE;
}

BOOL esos_lcd44780_getCursorDisplay( void )
{
    // Return cursor display state
	return esos_lcd44780_vars.b_cursorShown;
}

void esos_lcd44780_setCursorBlink( BOOL u8_state )
{
    // Set cursor blink state to u8_state
    esos_lcd44780_vars.b_cursorBlink = u8_state;
    esos_lcd44780_vars.b_cursorBlinkNeedsUpdate = TRUE;
}

BOOL esos_lcd44780_getCursorBlink( void )
{
    // Return cursor blink state
	return esos_lcd44780_vars.b_cursorBlink;
}

void esos_lcd44780_setDisplayVisible( BOOL u8_state )
{
    // Set display visible state to u8_state
    esos_lcd44780_vars.b_displayVisible = u8_state;
    esos_lcd44780_vars.b_displayVisibleNeedsUpdate = TRUE;
}

BOOL esos_lcd44780_getDisplayVisible( void )
{
    // Return display visible state
	return esos_lcd44780_vars.b_displayVisible;
}

void esos_lcd44780_setCustomChar( uint8_t u8_charSlot, uint8_t *pu8_charData )
{
    // Set custom character memory for u8_charSlot to data in pu8_charData
    // TODO:  this code is untested.
	uint8_t i;
	esos_lcd44780_vars.ab_customCharNeedsUpdate[u8_charSlot] = TRUE;
	for(i = 0; i < ESOS_LCD44780_NUM_CUSTOM_CHARS; i++) {
		esos_lcd44780_vars.ast_customChar[u8_charSlot].au8_data[i] = pu8_charData[i];
	}
}

void esos_lcd44780_getCustomChar( uint8_t u8_charSlot, uint8_t *pu8_charData )
{
    // Return pu8_charData with custom character memory for u8_charSlot
	// TODO:  Write hardware-independent code here
}

BOOL esos_lcd44780_isCurrent( void )
{
	uint8_t u8_row, u8_column;

	if(esos_lcd44780_vars.b_cursorPositionNeedsUpdate ||
	   esos_lcd44780_vars.b_cursorBlinkNeedsUpdate ||
	   esos_lcd44780_vars.b_displayVisibleNeedsUpdate) {
		return FALSE;
	}

	for(u8_row = 0; u8_row < ESOS_LCD44780_MEM_HEIGHT; ++u8_row) {
		for(u8_column = 0; u8_column < ESOS_LCD44780_MEM_WIDTH; ++u8_column) {
			if(esos_lcd44780_vars.ab_lcdBufferNeedsUpdate[u8_row][u8_column]) {
				return FALSE;
			}
		}
	}

	for(u8_column = 0; u8_column < ESOS_LCD44780_NUM_CUSTOM_CHARS; ++u8_column) {
		if(esos_lcd44780_vars.ab_customCharNeedsUpdate[u8_column]) {
			return FALSE;
		}
	}

	return TRUE;
}

