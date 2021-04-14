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
 * \addtogroup ESOS_SPI_Service
 * @{
 */



// Documentation for this file. If the \file tag isn't present,
// this file won't be documented.
/** \file
* \brief Hardware-specific support for ESOS32 SPI services
*        on the STM32L4xx MCUs (tested on STM32L452RE) ESOS32 
*/


/*** I N C L U D E S *************************************************/
#include "esos_spi.h"
#include "esos_stm32l4_edub.h"
#include "esos_stm32l4_spi.h"

/*** G L O B A L S *************************************************/
struct stTask   __stChildTaskSPI;
uint16_t           __esos_spi_u16s[2];

/*** T H E   C O D E *************************************************/

/*********************************************************
 * Public functions intended to be called by other files *
 *********************************************************/

// Documentation for this file. If the \file tag is not present,
// this file will not be documented.
// Note: place this comment below the #if NUM_I2C_MODS so Doxygen
// will only see it once.
/** \file
 *  SPI support functions. \see esos_stm32l4_spi.h for details.
 */


/*
Configure and enable the SPI module for operation at \em u32_spibps
bits/sec clock speed.
	param		u32_spibps specifies clock speed in bps
	\sa ESOS_TASK_WAIT_ON_WRITE1SPI1
	\sa ESOS_TASK_WAIT_ON_WRITE2SPI1
	\sa ESOS_TASK_WAIT_ON_WRITENSPI1
	\sa ESOS_TASK_WAIT_ON_XFERNSPI1
	\sa ESOS_TASK_WAIT_ON_READ1SPI1
	\sa ESOS_TASK_WAIT_ON_READ2SPI1
	\sa ESOS_TASK_WAIT_ON_READNSPI1
	\hideinitializer
*/
void __esos_spi_hw_config( uint32_t u32_spibps ) {
		
	// enable the clocking for the SPI1 peripheral
	rcc_periph_clock_enable(RCC_SPI1);
	spi_reset(SPI1);	
	
	// configure the SPI (output) pins:  SPI CLK and SPI MOSI
	gpio_mode_setup(EDUB_SPICLK_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, EDUB_SPICLK_PIN);	
	
	gpio_mode_setup(EDUB_SPIMOSI_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, EDUB_SPIMOSI_PIN);	

	// configure the SPI (input) pins:  SPI MISO
	gpio_mode_setup(EDUB_SPIMISO_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, EDUB_SPIMISO_PIN);	

	// configure the SPI pins to alternate function (AF)
	//    set Table 17 in the STM32L453RE datasheet 
	gpio_set_af(EDUB_SPICLK_PORT, GPIO_AF5, EDUB_SPICLK_PIN);
	gpio_set_af(EDUB_SPIMOSI_PORT, GPIO_AF5, EDUB_SPIMOSI_PIN);
	gpio_set_af(EDUB_SPIMISO_PORT, GPIO_AF5, EDUB_SPIMISO_PIN);

	//spi_reset(SPI1);
	// have libOpenCM3 initialize the SPI1 peripheral to
	//   the requested bitrate,  (NEED TO FIX THIS!)
	//   with 0 PHASE and 0 POLARITY (aka SPI mode 0)
	//   8bit transfers shifting MSb out first
	// There is a single API call: spi_init_master() but we
	//   will initialize via separate calls to make it more
	//   readable.
	
	spi_set_master_mode( SPI1 );				// we are the SPI MASTER
	
	// in the libOpenCM3 call spi_set_baudrate_prescaler, the prescaler is
	//   calculated by 2^(x+1)
    spi_set_baudrate_prescaler( SPI1, SPI_CR1_BR_FPCLK_DIV_8);		// Baud prescaler DIV_BY_8
	spi_set_standard_mode( SPI1, 0 );			// MODE 0:  CPOL=0 CPHA=0
	spi_set_full_duplex_mode( SPI1 );			// use 3-wire (SCL, MOSI, MISO) SPI
	spi_disable_crc( SPI1 );					// not using CRC
	spi_set_data_size ( SPI1, SPI_CR2_DS_8BIT);	// byte transfers 
	spi_send_msb_first( SPI1 );					// see EduBase schematic

	/*
	* Set NSS management to software.
	*
	* Note:
	* Setting nss high is very important, even if we are controlling the GPIO
	* ourselves this bit needs to be at least set to 1, otherwise the spi
	* peripheral will not send any data out.
	*/
	spi_enable_software_slave_management(SPI1);
	spi_set_nss_high(SPI1);

	/* Enable SPI1 periph. */
	spi_enable(SPI1);

} // end __esos_spi_hw_config() function


/*
Transaction: Writes \em u16_cnt words stored in
buffer \em *pu16_out to SPI device, while reading \em u16_cnt words from
SPI device placing results into buffer \em *pu16_in
\note Assumes that SPI peripheral has been properly configured.
\note The SPI peripheral setup determines whether 8-bit or 16-bit data
is written.
\param pu16_out Pointer to buffer containing data to send.  If \em pu16_out is \em NULLPTR this function will send zeroes to the SPI device and only "read"
\param pu16_in Pointer to buffer to catch incoming data.  If \em pu16_in is \em NULLPTR this function will only "write" the SPI device
\param u16_cnt Number of words to send
 */
ESOS_CHILD_TASK( __esos_spi_hw_xferNSPI1, uint16_t* pu16_out, uint16_t* pu16_in, uint16_t u16_cnt) {
  static uint16_t*      pu16_tempPtrIn;
  static uint16_t*      pu16_tempPtrOut;
  static uint16_t       u16_tempCnt, u16_i;
  static uint8_t        u8_isReading, u8_isWriting;
  uint16_t              u16_scratch;

  ESOS_TASK_BEGIN();

  pu16_tempPtrOut=pu16_out;
  pu16_tempPtrIn=pu16_in;
  u16_tempCnt=u16_cnt;
  if (pu16_tempPtrOut == NULLPTR)
    u8_isWriting = FALSE;
  else
    u8_isWriting = TRUE;

  if (pu16_tempPtrIn == NULLPTR)
    u8_isReading = FALSE;
  else
    u8_isReading = TRUE;

  // wait for any previous SPI transactions to finish
  ESOS_TASK_WAIT_WHILE( __ESOS_SPI_STM32L4_IS_BUSY() );

  // clear the overflow flag, just in case it is set
  //  the OVR flag is cleared by reading SPI_SR then reading SPI_DR
  //  see section 40.4.11 in the STM32L4xx reference manual
  if ( __ESOS_SPI_STM32L4_IS_OVR() ) {
	u16_scratch = SPI_SR(SPI1);
	u16_scratch = SPI_DR(SPI1);
  }
  
  for (u16_i=0; u16_i < u16_tempCnt; u16_i++) {
    if (u8_isWriting) {
      SPI_DR8(SPI1) = (uint8_t) (*pu16_tempPtrOut & 0x0FF);
      pu16_tempPtrOut++;
    } else {
      SPI_DR(SPI1) = 0;
    } // end isWriting

    // wait for TX word to be transfered
    //    the libOpenCM3 example omits this step.....
    ESOS_TASK_WAIT_UNTIL( __ESOS_SPI_STM32L4_IS_TXE() );
    
    // wait for complete RX word to come in...
    //ESOS_TASK_WAIT_UNTIL( __ESOS_SPI_STM32L4_IS_RXNE() );
    
    // read the incoming word from SPI
    u16_scratch = SPI_DR(SPI1);
    if (u8_isReading) {
      *pu16_tempPtrIn = u16_scratch;
      pu16_tempPtrIn++;
    } // end isReading
  } // end for()

  ESOS_TASK_END();
} // end __esos_spi_hw_xferNSPI1

/**
 * @}
 */
