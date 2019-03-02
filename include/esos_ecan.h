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
* \addtogroup ESOS_Task_ECAN_Service
* @{
*/

/** \file
*  This file contains macros, prototypes, and definitions for
*  ECAN services for ESOS tasks.
*/

#ifndef ESOS_ECAN_H
#define ESOS_ECAN_H

/* I N C L U D E S **********************************************************/
#include    "esos.h"

/* D E F I N E S ************************************************************/
#define MAX_CANFACTORY_CLIENTS  32
#define DEFAULT_MSG_ID      0x7a0
#define DEBUG_MODE        ESOS_USER_FLAG_F

#define ENABLE_DEBUG_MODE()   esos_SetUserFlag(DEBUG_MODE)
#define DISABLE_DEBUG_MODE()  esos_ClearUserFlag(DEBUG_MODE)
#define CHECK_DEBUG_MODE_ENABLED()    esos_IsUserFlagSet(DEBUG_MODE)

#define TRUE                    1
#define FALSE                   0

/* S T R U C T U R E S ******************************************************/
typedef enum {
  MASKCONTROL_FIELD_NONZERO,
  MASKCONTROL_EQUAL
} maskcontrol_t;

typedef struct {
  uint16_t        u16_canID;
  uint16_t        u16_idMask;
  maskcontrol_t   m_idMaskControl;
  uint8_t (*pf_task) (ESOS_TASK_HANDLE pst_Task);
  //uint8_t(*pfn) (struct stTask *pst_Task);
} client_t;

/* P U B L I C  P R O T O T Y P E S *****************************************/
/**
 * esos_ecan_send():
 *
 * Send a message using the CANFactory user task.
 *
 * u16_can_id:  CAN ID to be attached to the message. This should follow the
 *              assumed convention of your system. For embedded systems at
 *              MSU, this should be populated with the appropriate data from
 *              the supplied header file.
 * pu8_msg:     Pointer to the byte that is the beginning of u8_len bytes
 *              of sequential data to be sent.
 * u8_len:      Length of message pointed to by pu8_msg in bytes.
 */
//void esos_ecan_send ( uint16_t u16_can_id, uint8_t *pu8_msg, uint8_t u8_len );

#define ESOS_ECAN_SEND(u16_can_id,pu8_msg,u8_len)   { \
                                                        do { \
                                                            ESOS_TASK_HANDLE __esos_ecan_hTask; \
                                                            MAILMESSAGE __esos_ecan_mailMsg; \
                                                            uint8_t __esos_mail_msgBuf[ sizeof( uint16_t ) + 8 * sizeof( uint8_t ) ]; \
                                                            __esos_ecan_hTask = esos_GetTaskHandle( CANFactory ); \
                                                            if ( ESOS_TASK_MAILBOX_GOT_AT_LEAST_DATA_BYTES( __esos_ecan_hTask, __MAIL_MSG_HEADER_LEN + sizeof( uint8_t ) * u8_len + sizeof( uint16_t ) ) ) { \
                                                                *( ( uint16_t* ) &__esos_mail_msgBuf[0] ) = u16_can_id; \
                                                                memcpy( &__esos_mail_msgBuf[ sizeof( uint16_t ) ], pu8_msg, u8_len ); \
                                                                ESOS_TASK_MAKE_MSG_AUINT8 ( __esos_ecan_mailMsg, __esos_mail_msgBuf, sizeof( uint8_t ) * u8_len + sizeof( uint16_t ) ); \
                                                                ESOS_TASK_SEND_MESSAGE( __esos_ecan_hTask, &__esos_ecan_mailMsg ); \
                                                            } \
                                                        } while ( 0 ); \
                                                    }

/**
 * esos_ecan_canfactory_subscribe():
 *
 * Enroll a task/mask combination in the CANFactory system.
 *
 * pst_Task:        ESOS user task to be subscribed in the CANFactory. This is
 *                  the task that is responsible for receiving relevant
 *                  messages through the ESOS mail system.
 * u16_can_id:      CAN ID of the specified task pointed to by pst_Task.
 * u16_mask:        Mask to be used in accordance to the convention specified
 *                  by m_mask_control.
 * m_mask_control:  Can be either value of the enumeration maskcontrol_t as
 *                  shown below.
 *                  MASKCONTROL_FIELD_NONZERO:  u16_mask represents a mask that
 *                                              covers all message headers to
 *                                              which pst_Task should listen. If
 *                                              a message header has a bit
 *                                              lifted underneath the mask, the
 *                                              message will be sent to the
 *                                              pst_Task mailbox.
 *                  MASKCONTROL_EQUAL:          u16_mask represents a mask that
 *                                              is the exact message header to
 *                                              which pst_Task should listen. If
 *                                              a message header does not match
 *                                              this mask exactly, it will not
 *                                              be sent to the pst_Task mailbox.
 */
void esos_ecan_canfactory_subscribe(ESOS_TASK_HANDLE pst_Task, uint16_t u16_can_id, uint16_t u16_mask, maskcontrol_t m_mask_control);

/**
 * esos_ecan_canfactory_unsubscribe():
 *
 * Unsubscribe a task/mask combination in the CANFactory system.
 *
 * pst_Task:        Task handle of the task that should be removed from
 *                  CANFactory's data structure.
 * u16_can_id:      CAN ID of the task that should be removed from
 *                  CANFactory's data structure.
 * u16_mask:        Mask of the task/mask combination that should be
 *                  removed from the CANFactory's data structure.
 * m_mask_control:  Mask control specifier of the task/mask combination that
 *                  should be removed from the CANFactory's data structure.
 */
void esos_ecan_canfactory_unsubscribe(uint8_t(*pst_Task) (ESOS_TASK_HANDLE), uint16_t u16_can_id, uint16_t u16_mask, maskcontrol_t m_mask_control);

/**
 * esos_ecan_mask_check():
 *
 * Check subscribed messages against recieved messages according to the mask.
 *
 * u16_subscribed:  ID of the subscribed task.
 *
 * u16_recieved:    CAN ID of the message recieved.
 *
 * u16_mask:        Mask ID to be checked against.
 *
 */
BOOL esos_ecan_mask_check (uint16_t u16_subscribed, uint16_t u16_recieved, uint16_t u16_mask);

/**
 * CANFactory user task:
 *
 * Handles the management of the ECAN peripheral so that the API can be used.
 *
 * Note: the user should register this task from user_init in addition to using the API in this file.
 */
ESOS_USER_TASK ( CANFactory );

#endif  /* ESOS_ECAN_H */
