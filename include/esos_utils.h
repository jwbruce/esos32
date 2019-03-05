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
 * \addtogroup ESOS32_Utility_Functions
 * @{
 */


/** \file
 *  This file contains macros, prototypes, and definitions for
 *  some numerical and other utility functions for ESOS tasks.
 *
 */

#ifndef   ESOS_UTILS_H
#define ESOS_UTILS_H

/* I N C L U D E S **********************************************************/
#include    "esos.h"

/* S T R U C T U R E S ******************************************************/


/* D E F I N E S ************************************************************/

/* M A C R O S ************************************************************/


/* E X T E R N S ************************************************************/
extern uint32_t       __esos_u32FNVHash;

/* P U B L I C  P R O T O T Y P E S *****************************************/
// ESOS-provided pseudo-random number generator
uint32_t    esos_GetRandomUint32();
// ESOS-provided hash functions
uint16_t 	esos_hash_u32_to_u16(uint32_t u32_hash);
uint32_t 	esos_string_hash_u32(char *psz_str);
uint32_t 	esos_buffer_hash_u32(void *buf, uint16_t len);
// ESOS-provided CRC functions go here

/* P R I V A T E    P R O T O T Y P E S ***********************************/
void 		__esos_set_PRNG_U32Seed(uint32_t u32_seed);
uint32_t    __esos_get_PRNG_RandomUint32(void);

/** @} */

#endif    // ESOS_UTILS_H
