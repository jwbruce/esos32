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


#ifndef _ESOS_SPI_H
#define _ESOS_SPI_H


/**
 * \addtogroup ESOS_SPI_Service
 * @{
 */


// Documentation for this file. If the \file tag isn't present,
// this file won't be documented.
/** \file
 *  This file contains routines which SPI service on ESOS
 */


/* I N C L U D E S **********************************************************/
#include "esos.h"

/* D E F I N I T I O N S ****************************************************/

/* E X T E R N S ************************************************************/
extern struct stTask     __stChildTaskSPI;         // req'd child task for hw SPI functions
extern uint16_t            __esos_spi_u16s[2];     // used to store arguments

/* M A C R O S **************************************************************/

#define ESOS_TASK_WAIT_ON_AVAILABLE_SPI()                                           \
        do {                                                                        \
        ESOS_TASK_WAIT_WHILE(__esos_IsSystemFlagSet(__ESOS_SYS_SPI_IS_BUSY));   \
        __esos_SetSystemFlag(__ESOS_SYS_SPI_IS_BUSY);                           \
      }while(0)

#define ESOS_TASK_SIGNAL_AVAILABLE_SPI() __esos_ClearSystemFlag(__ESOS_SYS_SPI_IS_BUSY)

/**
Transaction: Write 1 (ONE) "word" stored in variable \em u16_d1 to SPI device.
\param u16_d1   Variable containing word (byte or 16-bits) to write
\note Assumes SPI peripheral has been properly configured.
\note SPI peripheral configuration determines whether 8 or 16 bits are written.
\sa ESOS_TASK_WAIT_ON_WRITE2SPI1
\sa ESOS_TASK_WAIT_ON_WRITENSPI1
\sa ESOS_TASK_WAIT_ON_XFERNSPI1
\hideinitializer
*/
#define   ESOS_TASK_WAIT_ON_WRITE1SPI1(u16_d1 )              \
    do{                                                                        \
      __esos_spi_u16s[0] = (uint16_t) (u16_d1);                                      \
      ESOS_TASK_SPAWN_AND_WAIT( (ESOS_TASK_HANDLE)&__stChildTaskSPI, __esos_spi_hw_xferNSPI1, &__esos_spi_u16s[0], NULLPTR, 1 );    \
    }while(0)


// We need a 2-byte array to be able to use the buffer.
// Otherwise, we'd need two writeN calls, which require
// child tasks, i.e. code size grows

/**
Transaction: Write 2 (TWO) "words" (bytes or 16-bits) stored in variables \em u16_d1 and \em u16_d2 to SPI device.
\param u16_d1   Variable containing first byte to write
\param u16_d2   Variable containing second byte to write
\note Assumes SPI peripheral has been properly configured.
\note SPI peripheral configuration determines whether 8 or 16 bits are written.
\sa ESOS_TASK_WAIT_ON_WRITE1SPI1
\sa ESOS_TASK_WAIT_ON_WRITENSPI1
\sa ESOS_TASK_WAIT_ON_XFERNSPI1
\hideinitializer
*/
#define   ESOS_TASK_WAIT_ON_WRITE2SPI1(u16_d1, u16_d2 )              \
    do{                                                                        \
      __esos_spi_u16s[0] = (uint16_t) (u16_d1);                                      \
      __esos_spi_u16s[1] = (uint16_t) (u16_d2);                                      \
      ESOS_TASK_SPAWN_AND_WAIT( (ESOS_TASK_HANDLE)&__stChildTaskSPI, __esos_spi_hw_xferNSPI1, &__esos_spi_u16s[0], NULLPTR, 2 );    \
    }while(0)

/**
Transaction: Write \em u16_cnt "words" (bytes or 16-bits) to SPI device
\param pu16_out Pointer to buffer containing words to send
\param u16_cnt Number of bytes to send
\note Assumes SPI peripheral has been properly configured.
\note SPI peripheral configuration determines whether 8 or 16 bits are written.
\sa ESOS_TASK_WAIT_ON_WRITE1SPI1
\sa ESOS_TASK_WAIT_ON_WRITE2SPI1
\sa ESOS_TASK_WAIT_ON_XFERNSPI1
\hideinitializer
*/
#define   ESOS_TASK_WAIT_ON_WRITENSPI1( pu16_out, u16_cnt )              \
            ESOS_TASK_SPAWN_AND_WAIT( (ESOS_TASK_HANDLE)&__stChildTaskSPI, __esos_spi_hw_xferNSPI1, (pu16_out), NULLPTR, (u16_cnt) )

/**
Transaction: Transfer (Read and write SPI simultaneously) \em u16_cnt "words" (bytes or 16-bits) to SPI device
\param pu16_out Pointer to buffer containing words to send
\param pu16_in Pointer to buffer to catch incoming words
\param u16_cnt Number of bytes to send
\note Assumes SPI peripheral has been properly configured.
\note SPI peripheral configuration determines whether 8 or 16 bits are written.
\note Assumes that both incoming and outgoing buffers are properly sized and available.
\sa ESOS_TASK_WAIT_ON_WRITE1SPI1
\sa ESOS_TASK_WAIT_ON_WRITE2SPI1
\sa ESOS_TASK_WAIT_ON_WRITENSPI1
\sa ESOS_TASK_WAIT_ON_READ1SPI1
\sa ESOS_TASK_WAIT_ON_READ2SPI1
\sa ESOS_TASK_WAIT_ON_READNSPI1
\hideinitializer
*/
#define   ESOS_TASK_WAIT_ON_XFERNSPI1( pu16_out, pu16_in, u16_cnt )              \
            ESOS_TASK_SPAWN_AND_WAIT( (ESOS_TASK_HANDLE)&__stChildTaskSPI, __esos_spi_hw_xferNSPI1, (pu16_out), (pu16_in), (u16_cnt) )

/**
Transaction: Read 1 (ONE) "word" from SPI device and stores result in variable \em u16_d1
\param u16_d1   Variable containing word (byte or 16-bits) to hold read results
\note Assumes SPI peripheral has been properly configured.
\note SPI peripheral configuration determines whether 8 or 16 bits are written.
\note This routine writes ZEROES to SPI device during reads
\sa ESOS_TASK_WAIT_ON_READ2SPI1
\sa ESOS_TASK_WAIT_ON_READNSPI1
\sa ESOS_TASK_WAIT_ON_XFERNSPI1
\hideinitializer
*/
#define   ESOS_TASK_WAIT_ON_READ1SPI1(u16_d1 )              \
    do{                                                                        \
      ESOS_TASK_SPAWN_AND_WAIT( (ESOS_TASK_HANDLE)&__stChildTaskSPI, __esos_spi_hw_xferNSPI1, NULLPTR, &__esos_spi_u16s[0], 1 );    \
      (u16_d1) = __esos_spi_u16s[0];        \
    }while(0)

/**
Transaction: Read 2 (TWO) "words" (bytes or 16-bits) from SPI device.  Stores results in variables \em u16_d1 and \em u16_d2
\param u16_d1   Variable containing first word/byte read
\param u16_d2   Variable containing second word/byte read
\note Assumes SPI peripheral has been properly configured.
\note SPI peripheral configuration determines whether 8 or 16 bits are written.
\note This routine writes ZEROES to SPI device during reads
\sa ESOS_TASK_WAIT_ON_READ1SPI1
\sa ESOS_TASK_WAIT_ON_READNSPI1
\sa ESOS_TASK_WAIT_ON_XFERNSPI1
\hideinitializer
*/
#define   ESOS_TASK_WAIT_ON_READ2SPI1(u16_d1, u16_d2)              \
    do{                                                            \
      ESOS_TASK_SPAWN_AND_WAIT( (ESOS_TASK_HANDLE)&__stChildTaskSPI, __esos_spi_hw_xferNSPI1, NULLPTR, &__esos_spi_u16s[0], 2 );    \
      (u16_d1) = __esos_spi_u16s[0];        \
      (u16_d2) = __esos_spi_u16s[1];        \
    }while(0)

/**
Transaction: Reads \em u16_cnt "words" (bytes or 16-bits) from SPI device.  Results are written to buffer pointed to by \em pu16_d
\param pu16_in Pointer to buffer to catch read results
\param u16_cnt Number of bytes to read
\note Assumes SPI peripheral has been properly configured.
\note SPI peripheral configuration determines whether 8 or 16 bits are written.
\note This routine writes ZEROES to SPI device during reads
\sa ESOS_TASK_WAIT_ON_READ1SPI1
\sa ESOS_TASK_WAIT_ON_READ2SPI1
\sa ESOS_TASK_WAIT_ON_XFERNSPI1
\hideinitializer
*/
#define   ESOS_TASK_WAIT_ON_READNSPI1( pu16_in, u16_cnt )              \
            ESOS_TASK_SPAWN_AND_WAIT( (ESOS_TASK_HANDLE)&__stChildTaskSPI, __esos_spi_hw_xferNSPI1, NULLPTR, (pu16_in), (u16_cnt) )


/* P U B L I C  P R O T O T Y P E S *****************************************/
void __esos_spi_config(uint32_t u32_spibps);

/* P R O T O T Y P E S  HARDWARE-SPECIFIC ********************************/
extern void __esos_spi_hw_config(uint32_t u32_spibps);
ESOS_CHILD_TASK( __esos_spi_hw_xferNSPI1, uint16_t* pu16_out, uint16_t* pu16_in, uint16_t u16_cnt);

/** @} */
#endif // end ESOS_SPI_H
