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
 
/*** I N C L U D E S *************************************************/
#include "esos_hwxxx_lcd44780wo.h"

/*** T H E   C O D E *************************************************/
void __esos_lcd44780_hw_config ( void )
{
    // Set up the hardware aspects of the HWxxx interface of the LCD module service
    //    direction, thresholds, etc beyond what is already done in esos_lcd44780_config()


}


/**
* ESOS child task to write data and commands to 44780-class LCD
* character modules
*
* \param u8_data data/command byte to write to display
* \param b_isData is TRUE if the u8_data is "data" (RS is set), 
* FALSE if u8_data is "command" (RS is cleared)
*
* \note This child task assumes that the associated hardware data
* pins are already configured, and the pins are in the "idle" state
* 
* \hideinitializer
*/

ESOS_CHILD_TASK(__esos_lcd44780_hw_write_u8, uint8_t u8_data, BOOL b_isData)
{
	ESOS_TASK_BEGIN();

	// hardware-specific code for writing to display goes here.

	ESOS_TASK_END();
}

/**
* unsafe hidden hardware function to write only four bits to display
*
* \param u8_u4data data/command byte to write to display
* \param b_isData is TRUE if the u8_data is "data" (RS is set), 
* FALSE if u8_data is "command" (RS is cleared)
*
* \note the least significant nibble is written to the hardware 
* \note This function must be provided by the hardware port only
* if the compile process defines the compiler variable
* ESOS_USE_LCD_4BIT
* \note This function is potentially unsafe
* 
* \hideinitializer
*/
#ifdef ESOS_USE_LCD_4BIT
void __esos_unsafe_lcd44780_hw_write_u4(uint8_t	u8_u4data, BOOL b_isData)
{
// hardware-specific code for writing a single nibble to display goes here
}
#endif
