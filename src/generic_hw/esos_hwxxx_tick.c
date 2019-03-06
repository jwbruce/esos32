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
 * \addtogroup ESOS_Tick
 * @{
 */

// Documentation for this file. If the \file tag isn't present,
// this file won't be documented.
/** \file
* \brief Template for creating hardware-specific support for ESOS32 
* system tick
*/



// Documentation for this file. If the \file tag isn't present,
// this file won't be documented.
/**
* \file
* Hardware-specific file to generate the ESOS system tick functionality
*/

// Include any HW-specific header files to pick up the HW register
//  definitions, macros, etc.
#include    "esos_hwxxx.h"

// create a "global" variable for the esos_tick count
//  It either contains the actual tick (in the case of a using the
//  software variable) or the system tick of the last time the tick
//  was accessed (in the case of a hardware timer register
//  containing the real clock tick)
volatile  uint32_t        esos_tick_count;
volatile  uint8_t         sub_tick;

// prototype for the ESOS timer service function
extern void __esos_tmrSvcsExecute(void);

/****************************************************/
/*
* \brief Increments the ESOS system tick  (PIC24 hardware)
*
* \pre  TMR1 is initialized with __esos_hw_InitSystemTick
*
* This ISR provides the mechanism for incrementing ESOS's
* 1.0ms system tick.  Using Timer 1, we can call this ISR
* every 1.0ms and increment esos_tick_count.  Or, we can
* have T1 call this ISR every X ms, and increment the
* value esos_tick_count by X
*
* \note The ESOS system tick will overflow in 2^32 msec
********************************************************/
void _ISRFAST _T1Interrupt (void) {
  esos_tick_count++;            // increment the ESOS system tick by
  _T1IF = 0;                    // clear the timer interrupt bit
  __esos_tmrSvcsExecute();      // let ESOS implement the S/w tmr service
}

/****************************************************/
/*
* \brief Initializes the ESOS system tick.
*
* \pre None assumed
*
* \post Sets up any hardware required to generate the 1.0ms tick
* required by ESOS.
*
* The (platform-independent) ESOS initialization code will
* call this function to setup and init the hardware (HWXXX
* MCU, in this case) to create the required timers,IRQs,etc
* to generate the 1.0ms ESOS system tick.
*
* \note We can either generate an IRQ every 1.0ms or longer period,
* we just need to make sure that ISR that increments the tick
* count is consistent.
********************************************************/
void    __esos_hw_InitSystemTick(void) {
  // FOR NOW, we will init our usual PIC24 development setup here.
  //  THIS REALLY DOESN'T BELONG HERE!!!!!!!
  configClock();                //config clock

  // TODO: testing an experimental tick using an external watch xtal
  //       on the PIC24 secondary oscillator (SOSC) pins.
  /**********************************************
   * USE FCY (instruction clock) to drive T1
   *      operate during IDLE, use a 64x prescaler,
   *      and the internal clock
   ********************************************************/
  T1CON = T1_IDLE_CON + T1_PS_1_64 + T1_SOURCE_INT;
  PR1 = MS_TO_TICKS(1, 64);        // 1 ms interrupt interval

  TMR1  = 0;                       // clear T1's count
  _T1IF = 0;                       // clear interrupt flag

  /* set T1's priority to be the highest possible for a PIC24 user IRQ
  ** User IRQs (if used) will be a lower IRQ priority, so the tick will
  ** get serviced in a timely manner
  */
  _T1IP = 7;
  _T1IE = 1;                       // enable the interrupt
  T1CONbits.TON = 1;               // turn on the timer

} // end __esos_hw_InitSystemTick()

/****************************************************/
/*
* \brief Returns the ESOS system tick count.
*
* \pre ESOS system tick is running/working.
*
* \return A 32-bit value of the number of ESOS system ticks
* since the system has booted.
*
********************************************************/
uint32_t   __esos_hw_GetSystemTickCount(void) {
  // if a hardware timer register contains the free-running tick
  // value, be sure to save the register value to esos_tick_count
  // before returning.  This is done to ensure that any rogue app
  // that tries to read esos_tick_count directly will get a tick
  // value that is reasonable..... maybe.
  //
  // esos_tick_count = HWXXX_TIMER_REGISTER;

  return  esos_tick_count;
}  // end __esos_hw_GetSystemTickCount()
