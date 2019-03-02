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
 * \addtogroup ESOS_UART_Service
 * @{
 */


/** \file
 *  This file contains macros, prototypes, and definitions for
 *  HARDWARE INDEPENDENT communications on ESOS
 */


#ifndef   ESOS_COMM_H
#define ESOS_COMM_H

/* I N C L U D E S **********************************************************/
#include    "esos.h"

/* P R O T O T Y P E S ******************************************************/
void __esos_InitCommSystem(void);
uint8_t __esos_u8_GetMSBHexCharFromUint8(uint8_t u8_x);
uint8_t __esos_u8_GetLSBHexCharFromUint8(uint8_t u8_x);
ESOS_CHILD_TASK( __esos_OutChar, uint8_t u8_c);
ESOS_CHILD_TASK( __esos_OutUint8AsDecString, uint8_t u8_x);
ESOS_CHILD_TASK( __esos_OutUint8AsHexString, uint8_t u8_x);
ESOS_CHILD_TASK( __esos_OutUint32AsHexString, uint32_t u32_x);
ESOS_CHILD_TASK( __esos_OutCharBuffer, uint8_t* pu8_out, uint8_t u8_len);
ESOS_CHILD_TASK( __esos_getBuffer, uint8_t* pau8_buff, uint8_t u8_size);
ESOS_CHILD_TASK( __esos_getString, char* pau8_buff);
ESOS_CHILD_TASK( __esos_OutString, char* psz_out );
void __esos_unsafe_PutUint8(uint8_t u8_c);
void __esos_unsafe_PutString(char* psz_in);
uint8_t __esos_unsafe_GetUint8(void);

/* D E F I N E S ************************************************************/
#define ESOS_COMM_SYS_USB       0x80
#define ESOS_COMM_SYS_SERIAL    0x00
#define ESOS_COMM_SYS_SERIAL_REV  (ESOS_COMM_SYS_SERIAL + 0x01)
// size of buffer to catch data incoming to MCU (based on USB terminology)
#define ESOS_SERIAL_OUT_EP_SIZE    64
// size of buffer to hold data leaving the MCU (based on USB terminology)
#define ESOS_SERIAL_IN_EP_SIZE     64
#define DEFAULT_BAUDRATE      57600

/***
 *** A few defines to help make data transfer easier
 ***/
#define   __ESOS_COMM_TXFIFO_PREP()                     \
  do{u16_tmp = __st_TxBuffer.u16_Head;                     \
  u16_tmp++;                                            \
  if (u16_tmp == ESOS_SERIAL_IN_EP_SIZE) u16_tmp = 0;   \
  } while (0)

#define   __ESOS_COMM_WRITE_TXFIFO( u8_out )            \
    do{__st_TxBuffer.pau8_Data[u16_tmp] = (u8_out);        \
    __st_TxBuffer.u16_Head = u16_tmp;} while (0)



/* M A C R O S ************************************************************/

/*
 * Evaluates to the number of bytes in the ESOS "in" communications buffer
 *
 * \param pstTask A pointer to the ESOS task control structure specific to this task.
 *
 * \hideinitializer
 */

/*
* Evaluates to the number of bytes in the ESOS "in" communications buffer
*
* \param pfnTaskFcn pointer to task function
* \retval NULLPTR   if no more tasks can execute at this time (scheduler is full)
* \retval pstTask   pointer to the scheduled task's structure
*
* \hideinitializer
*/

/**
* Evaluates to the number of bytes in the ESOS "in" communications buffer
* \retval N   number of bytes current contained in the "in" buffer
* \hideinitializer
*/
#define GET_ESOS_COMM_IN_DATA_LEN()   ((__st_RxBuffer.u16_Head>=__st_RxBuffer.u16_Tail)?(__st_RxBuffer.u16_Head-__st_RxBuffer.u16_Tail):(__st_RxBuffer.u16_Length-__st_RxBuffer.u16_Tail+__st_RxBuffer.u16_Head))

/**
* Evaluates to the booelan to determine if "in" communications buffer has
* <em>exactly</em> x bytes
*
* \param x    number of bytes to check for
* \retval TRUE   if "in" buffer has <em>exactly</em> x bytes
* \retval FALSE   otherwise
*
* \hideinitializer
*/
#define IS_ESOS_COMM_GOT_EXACTLY_DATA_BYTES(x)    (GET_ESOS_COMM_IN_DATA_LEN() == x)

/**
* Evaluates to the booelan to determine if "in" communications buffer has
* <em>at least</em> x bytes
*
* \param x    number of bytes to check for
* \retval TRUE   if "in" buffer has  x bytes <em>or more</em>
* \retval FALSE   otherwise
*
* \hideinitializer
*/
#define IS_ESOS_COMM_GOT_AT_LEAST_DATA_BYTES(x)   (GET_ESOS_COMM_IN_DATA_LEN() >= x)

/**
* Flushes the "in" communications buffer.  All unread data in the "in" communications
* buffer will be lost.
*
* \note Use this function only if you want to reset the "in" communications buffer back
* to its empty state.
*
* \hideinitializer
*/
#define FLUSH_ESOS_COMM_IN_DATA()             (__st_RxBuffer.u16_Head = __st_RxBuffer.u16_Tail)

/**
* Evaluates to the booelan to determine if "in" communications buffer
* <em>any</em> readable bytes
*
* \retval TRUE   if "in" buffer has <em>some</em> data to read
* \retval FALSE   otherwise
*
* \hideinitializer
*/
#define IS_ESOS_COMM_GOT_IN_DATA()            (__st_RxBuffer.u16_Head != __st_RxBuffer.u16_Tail)

// should use PEEK... It is unsafe since IRQs can occur at anytime....
/**
* Evaluates to a "peek" of the x-th data byte in the "in" communications buffer
*
* \note This macro does <em>NOT</em> move the "in" buffer pointers.  The data is
* still by the communcations subsystem GET_xxx routines.  This macro simply does
* a peek or look-ahead into the buffer.
*
* \note Use sparingly. This macro may be deprecated at some point.
*
* \note ESOS communication systems built upon interrupts (most of them) will make
*  this macro unsafe since the macro could be interrupted.
*
* \param x    byte in FIFO to "peek"
* \retval data   peeked data byte
*
* \hideinitializer
*/
#define PEEK_ESOS_COMM_IN_DATA(x)             ( __st_RxBuffer.pau8_Data[((__st_RxBuffer.u16_Tail+1+x)% ESOS_SERIAL_OUT_EP_SIZE)] )

/**
* Evaluates to a "peek" of the most recent data byte written to the "in" communications buffer
*
* \note This macro does <em>NOT</em> move the "in" buffer pointers.  The data is
* still by the communcations subsystem GET_xxx routines.  This macro simply does
* a peek or look-ahead into the buffer.
*
* \note Use sparingly. This macro may be deprecated at some point.
*
* \note ESOS communication systems built upon interrupts (most of them) will make
*  this macro unsafe since the macro could be interrupted.
*
* \retval data   peeked data byte
*
* \hideinitializer
*/
#define PEEK_ESOS_COMM_IN_LATEST_DATA()       ( __st_RxBuffer.pau8_Data[__st_RxBuffer.u16_Head] )

/**
* Evaluates to boolean to that determines whether the "out" system can accept anymore
* data
*
* \retval TRUE   if "out" communication system buffer has room for 1+ bytes
* \retval FALSE  otherwise
*
* \hideinitializer
*/
#define IS_ESOS_COMM_READY_OUT_DATA()             (__st_TxBuffer.u16_Head != __st_TxBuffer.u16_Tail)

// communications commands used by ESOS tasks
/**
 * Cause the current task to wait (block) until the ESOS "in" stream is available for
 * use.
 * \sa ESOS_TASK_SIGNAL_AVAILABLE_IN_COMM()
 * \hideinitializer
 */
#define   ESOS_TASK_WAIT_ON_AVAILABLE_IN_COMM()                                                \
            ESOS_TASK_WAIT_WHILE( __esos_IsSystemFlagSet( __ESOS_SYS_COMM_RX_IS_BUSY ) );      \
            __esos_SetSystemFlag( __ESOS_SYS_COMM_RX_IS_BUSY )

/**
 * Causes the current task to wait (block) until the ESOS "out" stream is available for
 * use.  Code will resume once the stream is available for our use.  This routine marks
 * the "in" stream in use by the current task.
 * \sa ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM()
 * \hideinitializer
 */
#define   ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM()                                                \
            ESOS_TASK_WAIT_WHILE( __esos_IsSystemFlagSet( __ESOS_SYS_COMM_TX_IS_BUSY ) );       \
            __esos_SetSystemFlag( __ESOS_SYS_COMM_TX_IS_BUSY )

/**
 * Signals to other requesting tasks that the current task is making the ESOS "in" stream
 * available again.
 *
 * \sa ESOS_TASK_WAIT_ON_AVAILABLE_IN_COMM()
 * \hideinitializer
 */
#define   ESOS_TASK_SIGNAL_AVAILABLE_IN_COMM()         __esos_ClearSystemFlag( __ESOS_SYS_COMM_RX_IS_BUSY )

/**
 * Signals to other requesting tasks that the current task is making the ESOS "out" stream
 * available again.
 * \sa ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM()
 * \hideinitializer
 */
#define   ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM()         __esos_ClearSystemFlag( __ESOS_SYS_COMM_TX_IS_BUSY )

/**
 * Signals to other requesting tasks that the ESOS "in" stream is being released or
 * made available again by the current task.
 * \sa ESOS_TASK_SIGNAL_AVAILABLE_IN_COMM()
 * \sa ESOS_TASK_WAIT_ON_AVAILABLE_IN_COMM()
 * \deprecated Use \ref ESOS_TASK_SIGNAL_AVAILABLE_IN_COMM instead.
 *
 * \hideinitializer
 */
#define   ESOS_TASK_RELEASE_IN_COMM()     ESOS_TASK_SIGNAL_AVAILABLE_IN_COMM()

/**
 * Signals to other requesting tasks that the ESOS "out" stream is being released or
 * made available again by the current task.
 * \sa ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM()
 * \sa ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM()
 * \deprecated Use \ref ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM instead.
 * \hideinitializer
 */
#define   ESOS_TASK_RELEASE_OUT_COMM()    ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM()


// **************** spawn child tasks to do getting and putting to comm streams ***********************
/**
* Create, spawn and wait on a child task to get a byte (uint8) from the ESOS "in" communications buffer
* <em>Results are written into the variable which is passed in</em>
*
* \note This call will block the current task until the 8-bit data is read
*
* \note This macro does not evaluate to anything.  Data is returned in the argument variable because
*  of the way ESOS tasks, child tasks, and macros are used.
*
* \param u8_in    variable <em>in which</em> data should be returned
* \sa ESOS_TASK_SPAWN_AND_WAIT
* \hideinitializer
*/
#define   ESOS_TASK_WAIT_ON_GET_UINT8( u8_in )                                    \
            ESOS_TASK_SPAWN_AND_WAIT( (ESOS_TASK_HANDLE)&__stChildTaskRx, __esos_getBuffer, &(u8_in), 1 )

/**
* Create, spawn and wait on a child task to get an array of bytes (uint8s) from the ESOS "in" communications buffer
* <em>Results are written into the array whose address is passed in</em>
*
* \note This call will block the current task until the data is read
*
* \note This macro does not evaluate to anything.  Data is returned in the argument variable because
*  of the way ESOS tasks, child tasks, and macros are used.
*
* \param pau8_in    pointer to array <em>in which</em> bytes should be returned
* \param u8_size    number of bytes to read from "in" stream
* \sa ESOS_TASK_SPAWN_AND_WAIT
* \hideinitializer
*/
#define   ESOS_TASK_WAIT_ON_GET_U8BUFFER( pau8_in, u8_size)                                                \
            ESOS_TASK_SPAWN_AND_WAIT( (ESOS_TASK_HANDLE) &__stChildTaskRx, __esos_getBuffer, (pau8_in), (u8_size) )

/**
* Create, spawn and wait on a child task to get a double-byte value (uint16) from the ESOS "in" communications buffer
* <em>Results are written into the variable which is passed in</em>
*
* \note This call will block the current task until the 16-bit data is read
*
* \note This macro does not evaluate to anything.  Data is returned in the argument variable because
*  of the way ESOS tasks, child tasks, and macros are used.
*
* \param u16_in    variable <em>in which</em> data should be returned
* \sa ESOS_TASK_SPAWN_AND_WAIT
* \hideinitializer
*/
#define   ESOS_TASK_WAIT_ON_GET_UINT16( u16_in )                                    \
            ESOS_TASK_SPAWN_AND_WAIT( (ESOS_TASK_HANDLE)&__stChildTaskRx, __esos_getBuffer, (uint8_t*) &(u16_in), 2 )

/**
* Create, spawn and wait on a child task to get a quad-byte value (uint32) from the ESOS "in" communications buffer
* <em>Results are written into the variable which is passed in</em>
*
* \note This call will block the current task until the 32-bit data is read
*
* \note This macro does not evaluate to anything.  Data is returned in the argument variable because
*  of the way ESOS tasks, child tasks, and macros are used.
*
* \param u32_in    variable <em>in which</em> data should be returned
* \sa ESOS_TASK_SPAWN_AND_WAIT
* \hideinitializer
*/
#define   ESOS_TASK_WAIT_ON_GET_UINT32( u32_in )                                    \
            ESOS_TASK_SPAWN_AND_WAIT( (ESOS_TASK_HANDLE)&__stChildTaskRx, __esos_getBuffer, (uint8_t*) &(u32_in), 4 )



/**
* Create, spawn and wait on a child task to get a string from the ESOS "in" communications buffer.
* The incoming string must be zero-terminated (useful when reading from other streaming data devices), or
* terminated with a newline/return character (useful when reading from terminals/keyboards/etc.)
* <em>Results are written into the array whose address is passed in</em>
*
* \note This call assumes that the data array target is large enough to hold the incoming string,
* which is limited to a length equal to the buffer used internally for the ESOS communications "in" buffer
*
* \note This call will block the current task until the data is read
*
* \note This macro does not evaluate to anything.  Data is returned in the argument variable because
*  of the way ESOS tasks, child tasks, and macros are used.
*
* \param pau8_in    pointer to array <em>in which</em> bytes should be returned
* \sa ESOS_TASK_SPAWN_AND_WAIT
* \hideinitializer
*/
#define   ESOS_TASK_WAIT_ON_GET_STRING( pau8_in )                                                \
            ESOS_TASK_SPAWN_AND_WAIT( (ESOS_TASK_HANDLE)&__stChildTaskRx, __esos_getString, (pau8_in) )

/**
* Create, spawn and wait on a child task to put a byte (uint8) to the ESOS "out" communications buffer
*
* \note This call will block the current task until the data is absorbed by the ESOS communications subsystem
* \param u8_out    data to write to "out" stream
* \sa ESOS_TASK_SPAWN_AND_WAIT
* \hideinitializer
*/
#define   ESOS_TASK_WAIT_ON_SEND_UINT8( u8_out)         \
            ESOS_TASK_SPAWN_AND_WAIT( (ESOS_TASK_HANDLE) &__stChildTaskTx, __esos_OutChar, (u8_out) )

/**
* Create, spawn and wait on a child task to put a byte (uint8) to the ESOS "out" communications buffer as a human-readable
* hexadecimal string.  Results will look like "0x4C"
*
* \note This call will block the current task until the data is absorbed by the ESOS communications subsystem
* \param u8_out    data to write to "out" stream
* \sa ESOS_TASK_SPAWN_AND_WAIT
* \hideinitializer
*/
#define   ESOS_TASK_WAIT_ON_SEND_UINT8_AS_HEX_STRING( u8_out)         \
            ESOS_TASK_SPAWN_AND_WAIT( (ESOS_TASK_HANDLE)&__stChildTaskTx, __esos_OutUint8AsHexString, (u8_out) )


/**
* Create, spawn and wait on a child task to put a byte (uint8) to the ESOS "out" communications buffer as a human-readable
* decimal string.  Results will look like "253"
*
* \note This call will block the current task until the data is absorbed by the ESOS communications subsystem
* \param u8_out    data to write to "out" stream
* \sa ESOS_TASK_SPAWN_AND_WAIT
* \hideinitializer
*/
#define   ESOS_TASK_WAIT_ON_SEND_UINT8_AS_DEC_STRING( u8_out)           \
            ESOS_TASK_SPAWN_AND_WAIT( (ESOS_TASK_HANDLE)&__stChildTaskTx, __esos_OutUint8AsDecString,(u8_out))
/**
* Create, spawn and wait on a child task to put a 32-bit value (uint32) to the ESOS "out" communications buffer as a human-readable
* hexadecimal string.  Results will look like "0x0123BEEF"
*
* \note This call will block the current task until the data is absorbed by the ESOS communications subsystem
* \param u32_out    data to write to "out" stream
* \sa ESOS_TASK_SPAWN_AND_WAIT
* \hideinitializer
*/
#define   ESOS_TASK_WAIT_ON_SEND_UINT32_AS_HEX_STRING( u32_out)         \
            ESOS_TASK_SPAWN_AND_WAIT( (ESOS_TASK_HANDLE)&__stChildTaskTx, __esos_OutUint32AsHexString, (u32_out) )

/**
* Create, spawn and wait on a child task to put a zero-terminated string to the ESOS "out" communications buffer.
*
* \note This call will block the current task until the data is absorbed by the ESOS communications subsystem
* \param psz_out    pointer to the zero-terminated string
* \sa ESOS_TASK_SPAWN_AND_WAIT
* \hideinitializer
*/
#define   ESOS_TASK_WAIT_ON_SEND_STRING( psz_out)         \
            ESOS_TASK_SPAWN_AND_WAIT( (ESOS_TASK_HANDLE)&__stChildTaskTx, __esos_OutString, (psz_out) )

/**
* Create, spawn and wait on a child task to put an array of bytes (uint8s) to the ESOS "out" communications buffer
*
* \note This call will block the current task until the data is absorbed by the ESOS communications subsystem
* \param pau8_out   pointer to beginning of array of bytes to send to "out" stream
* \param u8_size   number of bytes to send
* \sa ESOS_TASK_SPAWN_AND_WAIT
* \hideinitializer
*/
#define   ESOS_TASK_WAIT_ON_SEND_U8BUFFER( pau8_out, u8_size)         \
            ESOS_TASK_SPAWN_AND_WAIT( (ESOS_TASK_HANDLE)&__stChildTaskTx, __esos_OutCharBuffer, (pau8_out), (u8_size) )


/* S T R U C T U R E S ******************************************************/


/* E X T E R N S ************************************************************/
extern volatile uint8_t                 __esos_comm_tx_buff[ESOS_SERIAL_IN_EP_SIZE];
extern volatile uint8_t                 __esos_comm_rx_buff[ESOS_SERIAL_OUT_EP_SIZE];
extern volatile struct stTask           __stChildTaskTx, __stChildTaskRx;

/* P U B L I C  P R O T O T Y P E S *****************************************/
/**
* Returns the version number of the ESOS communication systems
* \retval verNum   Version number.  Exact value and meaning depends on hardware
* \hideinitializer
*/
uint8_t esos_GetCommSystemVersion(void);

/**
* Returns the size of the ESOS communication systems "out" buffers
* \retval uint8_t   Number of bytes
* \hideinitializer
*/
uint8_t esos_GetCommSystemMaxOutDataLen(void);

/**
* Returns the size of the ESOS communication systems "in" buffers
* \retval uint8_t   Number of bytes
* \hideinitializer
*/
uint8_t esos_GetCommSystemMaxInDataLen(void);

void  __esos_InitCommSystem(void);

/* prototypes of the unsafe comm functions provided by ESOS */
void __esos_unsafe_PutUint8(uint8_t u8_c);
void __esos_unsafe_PutString(char* psz_in);
uint8_t __esos_unsafe_GetUint8(void);

/* prototypes of external functions provided by hardware */
void __esos_hw_signal_start_tx(void);
void __esos_hw_signal_stop_tx(void);
void  __esos_hw_InitCommSystem(void);

/** @} */

#endif    // ESOS_COMM_H
