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

#ifndef _ESOS_STM32L4_UTILS_H
#define _ESOS_STM32L4_UTILS_H


/**
 * \addtogroup ESOS32_Utility_Functions
 * @{
 */

// Documentation for this file. If the \file tag isn't present,
// this file won't be documented.
/** \file
 *  \brief This file contains routines which configure and
 *  use hardware-version of the ESOS32 utility functions
 */


/* I N C L U D E S **********************************************************/
#include	"esos.h"
#include 	"esos_stm32l4.h"

/* D E F I N I T I O N S ****************************************************/

/* E X T E R N S ************************************************************/
#ifdef STM32L452xx
extern RNG_HandleTypeDef hrng;
#endif
/* M A C R O S **************************************************************/


/* P U B L I C  P R O T O T Y P E S *****************************************/
uint32_t	__esos_hw_PRNG_u32(void);
void		__esos_hw_config_PRNG(void);
void 		__esos_hw_set_PRNG_Seed(uint32_t u32_seed);

/** @} */
#endif // end _ESOS_STM32L4_UTILS_H
