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

#ifndef ESOS_hwxxx_SENSOR_H
#define ESOS_hwxxx_SENSOR_H

/**
 * \addtogroup ESOS_Task_Sensor_Service
 * @{
 */

/** \file
*  \brief This file contains routines which define, configure and
*  allow use of the "sensors" service (A/D converter) on the target
* hardware (hwxxxx)
*/

/* I N C L U D E S **********************************************************/
#include <esos_hwxxx.h>
#include <esos_sensor.h>

/* P U B L I C  P R O T O T Y P E S *****************************************/
void esos_sensor_config_hw (esos_sensor_ch_t, esos_sensor_vref_t);
void esos_sensor_initiate_conversion_hw (void);
uint16_t esos_sensor_getvalue_u16_hw (void);
void esos_sensor_release_hw (void);
BOOL esos_sensor_is_converting_hw (void);

#endif
