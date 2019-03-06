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

/** \file
 * \brief Circular buffer routines for ESOS32
 *
 */


#include    "esos.h"
#include    "esos_cb.h"

// ******** G L O B A L S ***************

/****************************************************************
** F U N C T I O N S
****************************************************************/
void __esos_CB_Init(CBUFFER* pst_CBuffer, uint8_t* pau8_ptr, uint16_t u16_Len) {
  // setup (or reset) the circular buffer descriptors
  pst_CBuffer->u16_Start = 0;
  pst_CBuffer->u16_Count = 0;
  pst_CBuffer->u16_Length = u16_Len;
  pst_CBuffer->pau8_Data = pau8_ptr;
} // endof __esos_CB_Init()


/******************************************
*** LOCAL HELPER MACROS
******************************************/
#define __WRITE_CB_UINT8(pstB,u8x,u16e)                                       \
  do {                                                                        \
    if ((pstB)->u16_Count < (pstB)->u16_Length) {                             \
      (u16e) = ((pstB)->u16_Start + (pstB)->u16_Count) % (pstB)->u16_Length;  \
      (pstB)->pau8_Data[(u16e)] = (u8x);                                      \
      ++(pstB)->u16_Count;                                                    \
    }                                                                         \
  }while(0)

#define __OVERWRITE_CB_UINT8(pstB,u8x,u16e)                                   \
  do {                                                                        \
    (u16e) = ((pstB)->u16_Start + (pstB)->u16_Count) % (pstB)->u16_Length;    \
    (pstB)->pau8_Data[(u16e)] = (u8x);                                        \
    if ((pstB)->u16_Count == (pstB)->u16_Length)                              \
      (pstB)->u16_Start = ((pstB)->u16_Start + 1) % (pstB)->u16_Length;       \
    else                                                                      \
      ++(pstB)->u16_Count;                                                    \
  }while(0)

#define __READ_CB_UINT8(pstB,u8x)                                             \
  do {                                                                        \
    (u8x) = (pstB)->pau8_Data[(pstB)->u16_Start];                             \
    (pstB)->u16_Start = ((pstB)->u16_Start + 1) % (pstB)->u16_Length;         \
    --(pstB)->u16_Count;                                                      \
  } while(0)


/***************************************************************
**** WRITERs
***************************************************************/

/**
* Writes byte data to a circular buffer
*
* \param pst_CBuffer  pointer to structure (CBUFFER) describing the circular buffer
* \param u8_x     data to write to the circular buffer
* \note This function <em>ASSUMES</em> that there is ample free
* space available in specified circular buffer.
*
* \hideinitializer
*/
void __esos_CB_WriteUINT8(CBUFFER* pst_CBuffer, uint8_t u8_x ) {
  uint16_t    u16_end;

  __WRITE_CB_UINT8(pst_CBuffer,u8_x,u16_end);
} // end __esos_CB_WriteUINT8()

void __esos_CB_OverwriteUINT8(CBUFFER* pst_CBuffer, uint8_t u8_x ) {
  uint16_t    u16_end;

  __OVERWRITE_CB_UINT8(pst_CBuffer,u8_x,u16_end);
} // end __esos_CB_OverwriteUINT8()

void __esos_CB_WriteUINT16(CBUFFER* pst_CBuffer, uint16_t u16_x ) {
  uint16_t    u16_end;
  uint8_t     u8_temp;

  u8_temp = (uint8_t) u16_x & 0xFF;
  __WRITE_CB_UINT8(pst_CBuffer,u8_temp,u16_end);
  u8_temp = (uint8_t) (u16_x>>8);
  __WRITE_CB_UINT8(pst_CBuffer,u8_temp,u16_end);
} // end __esos_CB_WriteUINT16()

void __esos_CB_WriteUINT32(CBUFFER* pst_CBuffer, uint32_t u32_x ) {
  uint16_t    u16_end;
  uint8_t     u8_temp;

  u8_temp = (uint8_t) u32_x & 0xFF;
  __WRITE_CB_UINT8(pst_CBuffer,u8_temp,u16_end);
  u8_temp = (uint8_t) (u32_x>>8);
  __WRITE_CB_UINT8(pst_CBuffer,u8_temp,u16_end);
  u8_temp = (uint8_t) (u32_x>>16);
  __WRITE_CB_UINT8(pst_CBuffer,u8_temp,u16_end);
  u8_temp = (uint8_t) (u32_x>>24);
  __WRITE_CB_UINT8(pst_CBuffer,u8_temp,u16_end);
} // end __esos_CB_WriteUINT32()

void __esos_CB_WriteUINT8Buffer(CBUFFER* pst_CBuffer, uint8_t* pu8_x, uint16_t u16_size ) {
  uint16_t    u16_i, u16_end;

  for (u16_i=0; u16_i<u16_size; u16_i++) {
    __WRITE_CB_UINT8(pst_CBuffer, pu8_x[u16_i], u16_end);
  }
} // end __esos_CB_WriteUINT8Buffer()

/***************************************************************
**** READERs
***************************************************************/
uint8_t __esos_CB_ReadUINT8(CBUFFER* pst_CBuffer ) {
  uint8_t     u8_retval;

  __READ_CB_UINT8(pst_CBuffer, u8_retval);
  return(u8_retval);
} // __esos_CB_ReadUINT8()

uint16_t __esos_CB_ReadUINT16(CBUFFER* pst_CBuffer ) {
  uint8_t     u8_temp;
  uint16_t    u16_retval;

  __READ_CB_UINT8(pst_CBuffer, u8_temp);
  u16_retval = (uint16_t) u8_temp;
  __READ_CB_UINT8(pst_CBuffer, u8_temp);
  u16_retval += ((uint16_t) u8_temp)<<8;
  return (u16_retval);
} // __esos_CB_ReadUINT16()

uint32_t __esos_CB_ReadUINT32(CBUFFER* pst_CBuffer ) {
  uint8_t     u8_temp;
  uint32_t    u32_retval;

  __READ_CB_UINT8(pst_CBuffer, u8_temp);
  u32_retval = (uint32_t) u8_temp;
  __READ_CB_UINT8(pst_CBuffer, u8_temp);
  u32_retval += (((uint32_t) u8_temp)<<8);
  __READ_CB_UINT8(pst_CBuffer, u8_temp);
  u32_retval += (((uint32_t) u8_temp)<<16);
  __READ_CB_UINT8(pst_CBuffer, u8_temp);
  u32_retval += (((uint32_t) u8_temp)<<24);
  return (u32_retval);
} // __esos_CB_ReadUINT32()


void __esos_CB_ReadUINT8Buffer(CBUFFER* pst_CBuffer, uint8_t* pu8_x, uint16_t u16_size ) {
  uint16_t    u16_i;

  for (u16_i=0; u16_i<u16_size; u16_i++) {
    __READ_CB_UINT8(pst_CBuffer, pu8_x[u16_i]);
  }
} // end __esos_CB_ReadUINT8Buffer()
