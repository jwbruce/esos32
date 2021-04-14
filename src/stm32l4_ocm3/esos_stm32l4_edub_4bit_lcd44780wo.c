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
/**
 * \addtogroup ESOS_Task_LCD_Service_WO
 * @{
 */
 
/*** I N C L U D E S *************************************************/
#include 	"esos_stm32l4_edub.h"
#include 	"esos_stm32l4_edub_4bit_lcd44780wo.h"

/*** G L O B A L S ***************************************************/

/*** T H E   C O D E *************************************************/
void __esos_lcd44780_hw_config ( void )
{	
  	/*Configure the GPIO pins that drive the LCD 4bit interface */
	gpio_mode_setup(EDUB_LCDE_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, EDUB_LCDE_PIN);

	gpio_mode_setup(EDUB_LCDRS_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, EDUB_LCDRS_PIN);

	gpio_mode_setup(EDUB_LCDD4_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, EDUB_LCDD4_PIN);
    gpio_set_output_options(EDUB_LCDD4_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_VERYHIGH, EDUB_LCDD4_PIN);

	gpio_mode_setup(EDUB_LCDD5_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, EDUB_LCDD5_PIN);
    gpio_set_output_options(EDUB_LCDD5_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_VERYHIGH, EDUB_LCDD5_PIN);
    
	gpio_mode_setup(EDUB_LCDD6_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, EDUB_LCDD6_PIN);
    gpio_set_output_options(EDUB_LCDD6_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_VERYHIGH, EDUB_LCDD6_PIN);

	gpio_mode_setup(EDUB_LCDD7_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, EDUB_LCDD7_PIN);			
    gpio_set_output_options(EDUB_LCDD7_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_VERYHIGH, EDUB_LCDD7_PIN);

	gpio_set(EDUB_LCDE_PORT, EDUB_LCDE_PIN);
	
	gpio_clear(EDUB_LCDRS_PORT, EDUB_LCDRS_PIN);
	gpio_clear(EDUB_LCDD4_PORT, EDUB_LCDD4_PIN);
	gpio_clear(EDUB_LCDD5_PORT, EDUB_LCDD5_PIN);		
	gpio_clear(EDUB_LCDD6_PORT, EDUB_LCDD6_PIN);
	gpio_clear(EDUB_LCDD7_PORT, EDUB_LCDD7_PIN);		
    
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
ESOS_CHILD_TASK( __esos_lcd44780_hw_write_u8, uint8_t u8_data, BOOL b_isData)
{	// hardware-specific code for writing to display goes here.
	ESOS_TASK_BEGIN();
	static		uint8_t		u8_localData;
	static		BOOL		b_localIsData;
	static		uint8_t		u8_temp;

	// TODO: verify this.... since the parallel-connected
	//   4-bit LCD I/F shares pins with COLx (used by the
	//   scanned keypad, I think we need to re-initialize
	//   the pin directions each time in the keypad
	//   changed them
	// IT LOOKS LIKE COLx will always be OUTPUTs, so lets
	//   no initialize the pins again here.....
	//
	// __esos_lcd44780_hw_config ();
	
	u8_localData = u8_data;			// make local copies 
	b_localIsData = b_isData;
	
	// We send MSNibble first. (p.22 on H44780 datasheet)
	// H44780 4-bit transfer: Data sheet implies
	//    1. set RS appropriately
	//    2. raise E  (Newhaven controller/Systronix data sheet
	//       says the RS->E time can be zero.
	//    3. hold E high, while setting data bits
	//    4. hold data stable >= 40ns  (T data-setup)
	//    5. lower E
	//    6. hold data stable >= 10ns  (T hold)
	//    7. release data lines
	//
	// SO, our approach is
	//   0.  ???? might need to drive data out first with E still low
	//       VERIFY THIS WITH DISPLAY
	//   1.  drive out appropriate RS, E high, with data
	//   2.  drive out appropriate RS, E low,  with data

	u8_temp = u8_localData & 0xF0;	// send MSNibble first
	// send data with E high
    __esos_unsafe_lcd44780_hw_write_u4(u8_temp, TRUE, b_localIsData);
    // then send data with E low to make the falling-edge "strobing" the LCD
    __esos_unsafe_lcd44780_hw_write_u4(u8_temp, FALSE, b_localIsData);
	// send data with E high   THIS SHOULD NOT BE NEEDED!!!!!!
    __esos_unsafe_lcd44780_hw_write_u4(u8_temp, TRUE, b_localIsData);


    // Now, repeat the process with the LSNibble (moved up into the MSNibble)
	u8_temp = u8_localData << 4;		// send LSNibble now
	// send data with E high
    __esos_unsafe_lcd44780_hw_write_u4(u8_temp, TRUE, b_localIsData);
	// then send data with E low to make the falling-edge "strobing" the LCD
    __esos_unsafe_lcd44780_hw_write_u4(u8_temp, FALSE, b_localIsData);
	// send data with E high   THIS SHOULD NOT BE NEEDED!!!!!!
    __esos_unsafe_lcd44780_hw_write_u4(u8_temp, TRUE, b_localIsData);
    
    // according to HD44780 data sheet, operations take at least
    //    37us (most) and up to 1.53ms (RETURN HOME cmd)
    // With our parallel connected interface, the STM32L4 (running at
    //    80MHz) is sending commands every 5us or so......
    ESOS_TASK_WAIT_TICKS(1);

	ESOS_TASK_END();
}

/**
* unsafe hidden hardware function to write only four bits to display
*
* \param u8_u4data data/command byte to write to display
* \param b_isData is TRUE if the u8_data is "data" (RS is set), 
* FALSE if u8_data is "command" (RS is cleared)
*
* \note the most significant nibble is written to the hardware 
* \note This function must be provided by the hardware port only
* if the compile process defines the compiler variable
* ESOS_USE_LCD_4BIT
* \note This function is potentially unsafe
* 
* \hideinitializer
*/
#ifdef ESOS_USE_LCD_4BIT
void __esos_unsafe_lcd44780_hw_write_u4(uint8_t	u8_u4data, BOOL b_isEnable, BOOL b_isData) {

	// WRITE THE PINS DIRECTLY BECAUSE WE ARE 4-BIT PARALLEL CONNECTED!
	// NOTE:  BE SURE THE JUMPER J23 IS IN THE CORRECT (LEFT) POSITION

	// set or clear RS as requested		
	if (b_isData) {
		gpio_set(EDUB_LCDRS_PORT, EDUB_LCDRS_PIN);		
	} else {
		gpio_clear(EDUB_LCDRS_PORT, EDUB_LCDRS_PIN);
	}
	// The LCDD4-LCDD7 pin are adjacent (PB1-PB4) on
	// the Nucleo 452 + EduBaseV2 hardware,
	// but we will set/clear them individually anyway
	// as if there were not adjacent
	
	// if (u8_u4data & 0x10) {
	if((u8_u4data & 0x10) != 0) {
		gpio_set(EDUB_LCDD4_PORT, EDUB_LCDD4_PIN);
	} else {
		gpio_clear(EDUB_LCDD4_PORT, EDUB_LCDD4_PIN);
	}
	//if (u8_u4data & 0x20) {
	if((u8_u4data & 0x20) != 0) {
		gpio_set(EDUB_LCDD5_PORT, EDUB_LCDD5_PIN);
	} else {
		gpio_clear(EDUB_LCDD5_PORT, EDUB_LCDD5_PIN);		
	}
	//if (u8_u4data & 0x40) {
	if((u8_u4data & 0x40) != 0) {		
		gpio_set(EDUB_LCDD6_PORT, EDUB_LCDD6_PIN);
	} else {
		gpio_clear(EDUB_LCDD6_PORT, EDUB_LCDD6_PIN);
	}
	//if (u8_u4data & 0x80) {
	if((u8_u4data & 0x80) != 0) {		
		gpio_set(EDUB_LCDD7_PORT, EDUB_LCDD7_PIN);
	} else {
		gpio_clear(EDUB_LCDD7_PORT, EDUB_LCDD7_PIN);		
	}
	// JWB: I THINK WE NEED TO DO THIS LAST AS THE
	//   FCN IS CALLED REPEATED WITH THE SAME DATA
	//   BUT CHANGING LCDE_PIN TO EFFECT A "STROBE"
	//
	// set or clear E as requested with the "new" data
	// already out on the pins.
	if (b_isEnable) {
		gpio_set(EDUB_LCDE_PORT, EDUB_LCDE_PIN);
	} else {
		gpio_clear(EDUB_LCDE_PORT, EDUB_LCDE_PIN);
	}
}
#endif

