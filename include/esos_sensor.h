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
* \addtogroup ESOS_Task_Sensor_Service
* @{
*/

/** \file
*  This file contains macros, prototypes, and definitions for
*  sensor services for ESOS tasks.
*
*/

#ifndef ESOS_SENSOR_H
#define ESOS_SENSOR_H

/* I N C L U D E S **********************************************************/
#include <esos.h>

/* E N U M S ****************************************************************/

/**
* enumeration to select sensor channel
**/
typedef enum {
  ESOS_SENSOR_CH00 = 0x00,
  ESOS_SENSOR_CH01,
  ESOS_SENSOR_CH02,
  ESOS_SENSOR_CH03,
  ESOS_SENSOR_CH04,
  ESOS_SENSOR_CH05,
  ESOS_SENSOR_CH06,
  ESOS_SENSOR_CH07,
  ESOS_SENSOR_CH08,
  ESOS_SENSOR_CH09,
  ESOS_SENSOR_CH0A,
  ESOS_SENSOR_CH0B,
  ESOS_SENSOR_CH0C,
  ESOS_SENSOR_CH0D,
  ESOS_SENSOR_CH0E,
  ESOS_SENSOR_CH0F,
} esos_sensor_ch_t;

/**
* enumeration to select sensor voltage reference
**/
typedef enum {
  ESOS_SENSOR_VREF_1V0 = 0x00,
  ESOS_SENSOR_VREF_1V024,
  ESOS_SENSOR_VREF_2V0,
  ESOS_SENSOR_VREF_2V048,
  ESOS_SENSOR_VREF_3V0,
  ESOS_SENSOR_VREF_3V3,
  ESOS_SENSOR_VREF_4V0,
  ESOS_SENSOR_VREF_4V096,
  ESOS_SENSOR_VREF_5V0,
} esos_sensor_vref_t;

/**
* enumeration to select sensor processing
**/
typedef enum {
  ESOS_SENSOR_ONE_SHOT = 0x00,
  ESOS_SENSOR_AVG2 = 0x01,      // arithmetic average
  ESOS_SENSOR_AVG4 = 0x02,
  ESOS_SENSOR_AVG8 = 0x03,
  ESOS_SENSOR_AVG16 = 0x04,
  ESOS_SENSOR_AVG32 = 0x05,
  ESOS_SENSOR_AVG64 = 0x06,
  ESOS_SENSOR_MIN2 = 0x21,      // minimum
  ESOS_SENSOR_MIN4 = 0x22,
  ESOS_SENSOR_MIN8 = 0x23,
  ESOS_SENSOR_MIN16 = 0x24,
  ESOS_SENSOR_MIN32 = 0x25,
  ESOS_SENSOR_MIN64 = 0x26,
  ESOS_SENSOR_MAX2 = 0x41,      // maximum
  ESOS_SENSOR_MAX4 = 0x42,
  ESOS_SENSOR_MAX8 = 0x43,
  ESOS_SENSOR_MAX16 = 0x44,
  ESOS_SENSOR_MAX32 = 0x45,
  ESOS_SENSOR_MAX64 = 0x46,
  ESOS_SENSOR_MEDIAN2 = 0x81,   // median
  ESOS_SENSOR_MEDIAN4 = 0x82,
  ESOS_SENSOR_MEDIAN8 = 0x83,
  ESOS_SENSOR_MEDIAN16 = 0x84,
  ESOS_SENSOR_MEDIAN32 = 0x85,
  ESOS_SENSOR_MEDIAN64 = 0x86,
} esos_sensor_process_t;

/**
* enumeration to select sensor return value format
**/
typedef enum {
  ESOS_SENSOR_FORMAT_BITS = 0x00,    // Return the ADC "raw" bitset
  ESOS_SENSOR_FORMAT_VOLTAGE = 0x80, // Return the ADC value as an integer representating the number of 0.1mV
  ESOS_SENSOR_FORMAT_PERCENT = 0x40, // Return the ADC value as an integer percentage 0-100% of full scale
} esos_sensor_format_t;

/* C H I L D   T A S K S ****************************************************/

ESOS_CHILD_TASK(_WAIT_ON_AVAILABLE_SENSOR, esos_sensor_ch_t, esos_sensor_vref_t);
ESOS_CHILD_TASK(_WAIT_SENSOR_QUICK_READ, uint16_t *u16_data);
ESOS_CHILD_TASK(_WAIT_SENSOR_READ, uint16_t *u16_data, uint8_t, esos_sensor_format_t);
BOOL ESOS_SENSOR_CLOSE(void);

/* D E F I N E S ************************************************************/

#define SIGNAL_ADC_BUSY           esos_SetSystemFlag(__ESOS_SYS_ADC_IS_BUSY)

#define ESOS_TASK_WAIT_WHILE_ADC_BUSY do { \
  esos_ClearSystemFlag(__ESOS_SYS_ADC_IS_BUSY); \
  ESOS_TASK_WAIT_UNTIL(esos_IsSystemFlagSet(__ESOS_SYS_ADC_IS_BUSY)); \
} while(0)

static ESOS_TASK_HANDLE th_child;

#define ESOS_TASK_WAIT_ON_AVAILABLE_SENSOR(CHCONST, VREFCONST) do { \
  ESOS_ALLOCATE_CHILD_TASK(th_child); \
  ESOS_TASK_SPAWN_AND_WAIT(th_child, _WAIT_ON_AVAILABLE_SENSOR, CHCONST, VREFCONST); \
} while(0)

#define ESOS_TASK_WAIT_SENSOR_QUICK_READ(u16_data) do { \
  ESOS_ALLOCATE_CHILD_TASK(th_child); \
  ESOS_TASK_SPAWN_AND_WAIT(th_child, _WAIT_SENSOR_QUICK_READ, &u16_data); \
} while(0)

#define ESOS_TASK_WAIT_SENSOR_READ(u16_data, PROCESSCONST, FMTCONST) do { \
  ESOS_ALLOCATE_CHILD_TASK(th_child); \
  ESOS_TASK_SPAWN_AND_WAIT(th_child, _WAIT_SENSOR_READ, &u16_data, PROCESSCONST, FMTCONST); \
} while(0)

#endif
