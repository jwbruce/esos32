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
 * \brief Mailboxes and messaging code for ESOS32 tasks
 *
 */


#include    "esos.h"
#include    "esos_mail.h"

// ******** G L O B A L S ***************
//MAILBOX     __astMailbox[MAX_NUM_USER_TASKS];
//uint8_t       __au8_MBData[MAX_NUM_USER_TASKS][MAX_SIZE_TASK_MAILBOX];
uint8_t       __u8_esos_mail_routines_dummy_uint8;

/****************************************************************
** F U N C T I O N S
****************************************************************/
void __esos_InitMailbox(MAILBOX* pst_Mailbox, uint8_t* pau8_ptr) {
  __esos_CB_Init( pst_Mailbox->pst_CBuffer, pau8_ptr, MAX_SIZE_TASK_MAILBOX);
} // endof esos_InitMailbox()

/**
* Writes message data to a task's mailbox.
*
* \param pst_RcvrTask  pointer to task structure (ESOS_TASK_HANDLE) whose mailbox will be written
* \param pst_Msg        pointer to mailbox message structure that contains data to write to the task's mailbox
* \note This function <em>ASSUMES</em> that there is ample free space available in specified
* mailbox.
*
* \sa ESOS_TASK_WAIT_ON_TASKS_MAILBOX_HAS_SPACE
* \hideinitializer
*/
void __esos_SendMailMessage(ESOS_TASK_HANDLE pst_RcvrTask, MAILMESSAGE* pst_Msg ) {
  uint8_t               u8_i;

  // first message btye:  flags in upper nibble, payload length in lower
  u8_i = ((pst_Msg->u8_flags)<<4) + (pst_Msg->u8_DataLength & 0x0F);
  __esos_CB_WriteUINT8( pst_RcvrTask->pst_Mailbox->pst_CBuffer, u8_i );
  // second message word: Task ID of sending task
  __esos_CB_WriteUINT16( pst_RcvrTask->pst_Mailbox->pst_CBuffer, ESOS_GET_PMSG_FROMTASK(pst_Msg) );
  // Now, timestamp the message with double word
  __esos_CB_WriteUINT32( pst_RcvrTask->pst_Mailbox->pst_CBuffer, esos_GetSystemTick() );
  // Now write the data depending on what type and how many
  if ( ESOS_GET_PMSG_FLAGS(pst_Msg) & ESOS_MAILMESSAGE_UINT8) {
    for (u8_i=0; u8_i<ESOS_GET_PMSG_DATA_LENGTH(pst_Msg); u8_i++) {
      __esos_CB_WriteUINT8( pst_RcvrTask->pst_Mailbox->pst_CBuffer, pst_Msg->au8_Contents[u8_i] );
    } // end for UINT8s
  } else if ( ESOS_GET_PMSG_FLAGS(pst_Msg) & ESOS_MAILMESSAGE_UINT16) {
    for (u8_i=0; u8_i<pst_Msg->u8_DataLength; u8_i++) {
      __esos_CB_WriteUINT16( pst_RcvrTask->pst_Mailbox->pst_CBuffer, pst_Msg->au16_Contents[u8_i] );
    } // end for UINT16s
  } else if ( ESOS_GET_PMSG_FLAGS(pst_Msg) & ESOS_MAILMESSAGE_UINT32) {
    for (u8_i=0; u8_i<pst_Msg->u8_DataLength; u8_i++) {
      __esos_CB_WriteUINT32( pst_RcvrTask->pst_Mailbox->pst_CBuffer, pst_Msg->au32_Contents[u8_i] );
    } // end for UINT32s
  } else {
  } // end for STRINGs
} // endof __esos_SendMailMessage()

/**
* Reads the next (oldest) waiting message from a task's mailbox.
*
* \param pstTask  pointer to task structure (ESOS_TASK_HANDLE) whose mailbox will be written
* \param pstMsg   pointer to mailbox message structure that contains data to write to the task's mailbox
* \note This function <em>ASSUMES</em> that there is ample free space available in specified
* mailbox.
*
* \sa ESOS_TASK_WAIT_ON_TASKS_MAILBOX_HAS_SPACE
* \hideinitializer
*/
void __esos_ReadMailMessage(ESOS_TASK_HANDLE pst_Task, MAILMESSAGE* pst_Message ) {
  uint8_t               u8_i;
  ESOS_TASK_HANDLE      pst_From;

  /* first message btye:  flags in upper nibble, payload length in lower */
  u8_i = __esos_CB_ReadUINT8( pst_Task->pst_Mailbox->pst_CBuffer );
  pst_Message->u8_flags = u8_i>>4;
  pst_Message->u8_DataLength = u8_i & 0x0F;
  /* second message byte: Task ID of sending task */
  pst_Message->u16_FromTaskID = __esos_CB_ReadUINT16( pst_Task->pst_Mailbox->pst_CBuffer );
  /* Now, timestamp the message */
  pst_Message->u32_Postmark = __esos_CB_ReadUINT32( pst_Task->pst_Mailbox->pst_CBuffer );
  /* Now write the data depending on what type and how many */
  if ( ESOS_GET_PMSG_FLAGS(pst_Message) & ESOS_MAILMESSAGE_UINT8) {
    for (u8_i=0; u8_i<ESOS_GET_PMSG_DATA_LENGTH(pst_Message); u8_i++) {
      pst_Message->au8_Contents[u8_i] = __esos_CB_ReadUINT8( pst_Task->pst_Mailbox->pst_CBuffer );
    } // end for UINT8s
  } else if ( ESOS_GET_PMSG_FLAGS(pst_Message) & ESOS_MAILMESSAGE_UINT16) {
    for (u8_i=0; u8_i<pst_Message->u8_DataLength; u8_i++) {
      pst_Message->au16_Contents[u8_i] = __esos_CB_ReadUINT16( pst_Task->pst_Mailbox->pst_CBuffer );
    } // end for UINT16s
  } else if ( ESOS_GET_PMSG_FLAGS(pst_Message) & ESOS_MAILMESSAGE_UINT32) {
    for (u8_i=0; u8_i<pst_Message->u8_DataLength; u8_i++) {
      pst_Message->au32_Contents[u8_i] = __esos_CB_ReadUINT32( pst_Task->pst_Mailbox->pst_CBuffer );
    } // end for UINT32s
  } else {
  } // end strings
  /* If sending task requests ACK a.k.a. "delivery confirmation, then do it */
  if ( ESOS_GET_PMSG_FLAGS(pst_Message) & ESOS_MAILMESSAGE_REQUEST_ACK) {
    pst_From = esos_GetTaskHandleFromID( ESOS_GET_PMSG_FROMTASK(pst_Message) );
    if (pst_From != NULLPTR) {
      __ESOS_CLEAR_TASK_MAILNACK_FLAG( pst_From );
    } // end if ! NULLPTR
  } //end if
} // __esos_ReadMailMessage()



