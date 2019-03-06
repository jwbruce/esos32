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

// Documentation for this file. If the \file tag isn't present,
// this file won't be documented.
/**
* /** \file
* \brief Template for creating hardware-specific support for the
* various ESOS32 utilities: random numbers, CRC, hashes, etc.
* in hardware, *if such hardware is available*.
*/

// Include any HW-specific header files to pick up the HW register
//  definitions, macros, etc.
#include    "esos_hwxxx.h"
#include	"esos_hwxxx_utils.h"

/***************************************************
 * 
 * HARDWARE-specific ESOS utility functions
 * 
 * *************************************************/

/* ******************************************************
* \brief HW pseudo-random number generation...
*
* \note All hardware must implement this function.
* 
* If the underlying hardware has PRNG on-board, it can
* implement it here and return the uint32 "random" number.
* 
* If the underlying HW does not have PRNG on-board, it
* can call the hidden ESOS function for software RNG
*         uint32 __esos_get_PRNG_RandomUint32(void)
* and return that value.
*
********************************************************/
uint32_t   __esos_hw_PRNG_u32(void) {
  return	__esos_get_PRNG_RandomUint32();
} // end __esos_hw_PRNG_u32(void)


/* *******************************************************
* \brief user-provided function to config the HW RNG
*
* \note All hardware must implement this function.
* 
* If the underlying hardware has PRNG on-board, HW-specific code
* for intializing the PRNG hardware goes here.
* 
* If the underlying HW does not have PRNG on-board, this
* function should just return.
*
*******************************************************  */
void   __esos_hw_config_PRNG(void) {

}  // end __esos_hw_config_PRNG(void)

/* *******************************************************
* \brief seed the HW pseudo-random number generator...
*
* \note All hardware must implement this function.
* 
* If the underlying hardware has PRNG on-board, it can
* do what it needs to "seed" the PRNG.
* 
* If the underlying HW does not have PRNG on-board, it
* can call the hidden ESOS function for seeding the
* ESOS software RNG function
*         void __esos_setSWRNGU32Seed(uint32)
*
*  \sa __esos_get_PRNG_RandomUint32
*
********************************************************** */
void __esos_hw_set_PRNG_Seed(uint32_t u32_seed) {
  __esos_set_PRNG_U32Seed(u32_seed);
} // end __esos_hw_setPRNG_Seed(uint32)
