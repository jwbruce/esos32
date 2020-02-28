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
/** \file
* \brief STM32L4xx hardware support for various ESOS utilities
* random numbers, CRC, hashes, etc.  executed in hardware, *if available*
*/

// Include any HW-specific header files to pick up the HW register
//  definitions, macros, etc.
#include    "esos_stm32l4.h"
#include	"esos_stm32l4_utils.h"

// VARIABLES
#ifdef STM32L452xx
RNG_HandleTypeDef hrng;
#endif

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
#ifdef STM32L452xx
  uint32_t		u32_randomNum;
  
  #if 1
    HAL_RNG_GenerateRandomNumber( &hrng, &u32_randomNum);
  #else
    // not sure why this doesn't work...... should be
    // faster than above as we will read last RNG and then
    //  kick off the RNG hardware to make a new RNG before
    //  our next read.   Need to study HAL more to figure this out.
    u32_randomNum = HAL_RNG_ReadLastRandomNumber( &hrng ); 
    HAL_RNG_GenerateRandomNumber_IT( &hrng );
  #endif
  return u32_randomNum;
#else
  return ( __esos_get_PRNG_RandomUint32() );
#endif
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
#ifdef STM32L452xx	
  hrng.Instance = RNG;
  if (HAL_RNG_Init(&hrng) != HAL_OK)
  {
    Error_Handler();
  }
#endif
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
#ifdef STM32L452xx
   // RNG hardware doesn't let us set the seed
#else
   __esos_set_PRNG_U32Seed(u32_seed);
#endif
  
} // end __esos_hw_setPRNG_Seed(uint32)
