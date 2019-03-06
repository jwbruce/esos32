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
 * \addtogroup ESOS_SPI_Service
 * @{
 */



// Documentation for this file. If the \file tag isn't present,
// this file won't be documented.
/** \file
* \brief Template for creating hardware-specific support for ESOS32 
* SPI communications service
*/


/*** I N C L U D E S *************************************************/
#include "esos_hwxxx_spi.h"

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
 *  SPI support functions. \see pic24_spi.h for details.
 */

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
ESOS_CHILD_TASK( __esos_hwxxx_xferNSPI1, uint16_t* pu16_out, uint16_t* pu16_in, uint16_t u16_cnt) {
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

  // clear the overflow flag, just in case it is set
  if (SPI1STATbits.SPIROV) SPI1STATbits.SPIROV = 0;
  //clear SPI interrupt flag since we are about to write new value to SPI
  _SPI1IF = 0;
  /* read SPI1BUF to clear SPI_RX_BUFFER_FULL bit just in case previous
     SPI use did not read the SPI1BUF that last time!
  */
  u16_scratch = SPI1BUF;
  for (u16_i=0; u16_i < u16_tempCnt; u16_i++) {
    if (u8_isWriting) {
      SPI1BUF = *pu16_tempPtrOut;
      pu16_tempPtrOut++;
    } else {
      SPI1BUF = 0;
    } // end isWriting

    /* Seen some strange behavior checking _SPI1IF like the
     * hardware support library.  The following method is valid
     * and appears to work in all cases.
     */
    // wait for TX word to be copied to SPI1SR
    ESOS_TASK_WAIT_WHILE( SPI1STAT & SPI_TX_BUFFER_FULL );
    // wait for RX word to be copied from SPI1SR
    ESOS_TASK_WAIT_UNTIL( SPI1STAT & SPI_RX_BUFFER_FULL );
    // read the word from SPI (clears SPI_RX_BUFFER_FULL bit)
    u16_scratch = SPI1BUF;
    if (u8_isReading) {
      *pu16_tempPtrIn = u16_scratch;
      pu16_tempPtrIn++;
    } // end isReading
  } // end for()
  ESOS_TASK_END();
} // end __esos_hwxxx_xferNSPI1

/**
 * @}
 */
