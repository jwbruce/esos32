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
 * \addtogroup ESOS_Circular_Buffers
 * @{
 */


/** \file
 *  This file contains macros, prototypes, and definitions for
 *  circular buffers used by many ESOS services.
 *
 *   \todo Reconcile circular buffer routines to be used in tasks and ISRs!
 */


#ifndef   ESOS_CB_H
#define ESOS_CB_H

/* I N C L U D E S **********************************************************/
#include    "esos.h"

/* D E F I N E S ************************************************************/

/* S T R U C T U R E S ******************************************************/

/**
* structure to contain a set of descriptors about the circular buffers
* used to implement ESOS task mailboxes, communication buffers, etc.
**/
typedef struct __stCIRCBUFF {
  uint16_t          u16_Length;                       // maximum number of elements
  uint16_t          u16_Start;                        // index of oldest element
  uint16_t          u16_Count;                        // number of elements in use
  uint8_t*          pau8_Data;                        // ptr to data area (SHOULD THIS BE A void* ?)
} CBUFFER;

typedef CBUFFER*    CBUFF_HANDLE;

/* M A C R O S ************************************************************/
#define __ESOS_CB_FLUSH(pstCB)                              (pstCB)->u16_Count = 0
#define __ESOS_CB_IS_EMPTY(pstCB)                           ((pstCB)->u16_Count == 0)
#define __ESOS_CB_IS_NOT_EMPTY(pstCB)                       ((pstCB)->u16_Count != 0)
#define __ESOS_CB_IS_FULL(pstCB)                            ((pstCB)->u16_Length == (pstCB)->u16_Count)
#define __ESOS_CB_GET_LENGTH(pstCB)                         ((pstCB)->u16_Length)
#define __ESOS_CB_GET_COUNT(pstCB)                          ((pstCB)->u16_Count)
#define __ESOS_CB_GET_AVAILABLE(pstCB)                      (__ESOS_CB_GET_LENGTH(pstCB)-__ESOS_CB_GET_COUNT(pstCB))
#define __ESOS_CB_IS_AVAILABLE_AT_LEAST(pstCB, x)           (__ESOS_CB_GET_AVAILABLE((pstCB))>=(x))
#define __ESOS_CB_IS_AVAILABLE_EXACTLY(pstCB, x)            (__ESOS_CB_GET_AVAILABLE((pstCB))==(x))

#define ESOS_TASK_WAIT_WHILE_CB_IS_EMPTY(pstCB)                   ESOS_TASK_WAIT_WHILE(__ESOS_CB_IS_EMPTY((pstCB)))
#define ESOS_TASK_WAIT_WHILE_CB_IS_FULL(pstCB)                    ESOS_TASK_WAIT_WHILE(__ESOS_CB_IS_FULL((pstCB)))
#define ESOS_TASK_WAIT_UNTIL_CB_HAS_AVAILABLE_AT_LEAST(pstCB,x)   ESOS_TASK_WAIT_UNTIL(__ESOS_CB_IS_AVAILABLE_AT_LEAST((pstCB),(x)))


/* E X T E R N S ************************************************************/

/* P U B L I C  P R O T O T Y P E S *****************************************/


/* P R I V A T E    P R O T O T Y P E S ***********************************/
void __esos_CB_Init(CBUFFER* pst_CBuffer, uint8_t* pau8_ptr, uint16_t u16_Length);
void __esos_CB_WriteUINT8(CBUFFER* pst_CBuffer, uint8_t u8_x);
void __esos_CB_WriteUINT16(CBUFFER* pst_CBuffer, uint16_t u16_x);
void __esos_CB_WriteUINT32(CBUFFER* pst_CBuffer, uint32_t u32_x);
void __esos_CB_WriteUINT8Buffer(CBUFFER* pst_CBuffer, uint8_t* pu8_x, uint16_t u16_size );
void __esos_CB_OverwriteUINT8(CBUFFER *pst_CBuffer, uint8_t u8_x);


uint8_t __esos_CB_ReadUINT8(CBUFFER* pst_CBuffer);
uint16_t __esos_CB_ReadUINT16(CBUFFER* pst_CBuffer);
uint32_t __esos_CB_ReadUINT32(CBUFFER* pst_CBuffer);
void __esos_CB_ReadUINT8Buffer(CBUFFER* pst_CBuffer, uint8_t* pu8_x, uint16_t u16_size );

/**
void __esos_CB_Init(MAILBOX* pst_Mailbox);
void __esos_WriteMailboxUINT8(MAILBOX* pst_Mailbox, uint8_t u8_x );
void __esos_WriteMailboxUINT16(MAILBOX* pst_Mailbox, uint16_t u16_x );
void __esos_WriteMailboxUINT32(MAILBOX* pst_Mailbox, uint32_t u32_x );
uint8_t __esos_ReadMailboxUINT8(MAILBOX* pst_Mailbox );
uint16_t __esos_ReadMailboxUINT16(MAILBOX* pst_Mailbox );
uint32_t __esos_ReadMailboxUINT32(MAILBOX* pst_Mailbox );
void __esos_SendMailUint8(ESOS_TASK_HANDLE pst_Task, MAILBOX* pst_Mailbox, uint8_t* pau8_data, uint8_t u8_len );
**/

/** @} */

#endif    // ESOS_CB_H
