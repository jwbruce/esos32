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
 * \addtogroup ESOS_Simple_User_Interface_Service
 * @{
 */



#ifndef ESOS_SUI_H
#define ESOS_SUI_H

#include "esos.h"

// STRUCTURES
typedef struct {
    BOOL        b_pressed;
    BOOL        b_doublePressed;
    uint8_t     u8_lastEvent;
    uint32_t    u32_lastEventTime;
    uint8_t     _u8_state;
    uint32_t    _u32_nextTickTime;    
    uint32_t    u32_userData1;
    uint32_t    u32_userData2;   
} _st_esos_sui_Switch;

typedef struct {
    BOOL        b_LEDOn;
    uint8_t		_u8_state;
    uint16_t    u16_LEDFlashPeriod;    
    uint8_t     u8_lastEvent;
    uint32_t    u32_lastEventTime;    
    uint32_t    u32_userData1;
    uint32_t    u32_userData2;    
} _st_esos_sui_LED;

typedef		uint8_t		ESOS_SUI_LED_HANDLE;
typedef		uint8_t		ESOS_SUI_SWITCH_HANDLE;

// ENUMERATIONS
enum esos_sui_switchStates {
  ESOS_SUI_SWSTATE_IDLE,
  ESOS_SUI_SWSTATE_WAIT_SW_UP,
  ESOS_SUI_SWSTATE_WAIT_CLICK_TIMEOUT,
  ESOS_SUI_SWSTATE_WAIT_DCLICK_TIMEOUT,
};

enum esos_sui_switchEvents {
  ESOS_SUI_SWITCH_EVENT_NULL,
  ESOS_SUI_SWITCH_EVENT_HOLD,
  ESOS_SUI_SWITCH_EVENT_CLICK,  
  ESOS_SUI_SWITCH_EVENT_DOUBLE_CLICK
};

enum esos_sui_LEDStates {
  ESOS_SUI_LED_STATE_ALL_OFF = 0,
  ESOS_SUI_LED_STATE_ON = 1,
  ESOS_SUI_LED_STATE_FLASHING = 2,
  ESOS_SUI_LED_STATE_ONE_SHOT = 4
};


enum esos_sui_LEDEvents {
  ESOS_SUI_LED_EVENT_OFF = 1,
  ESOS_SUI_LED_EVENT_ON = 2
};

// DEFINEs and CONSTANTs
#define   ESOS_SUI_NUM_MAX_SWITCHES         32
#define   ESOS_SUI_NUM_MAX_LEDS             32

#define CLICK_TICK          200	    //
#define DOUBLE_CLICK_TICK   200	    //
#define HOLD_TICK           500

#define   SWITCH_SINGLE_PRESS_LATENCY       100 // Time before single click is recognized, double click must first release before this time.
#define   SWITCH_DOUBLE_PRESS_MAX_LATENCY   200 // Time before double click must be pressed after releasing the switch from first press

// The time duration #DEFINES should be used from  esos/include/all_generic.h
//#define __ESOS_TICKS_TO_MS(x)           (x/1)
//#define __ESOS_MS_TO_TICKS(x)           (x*1)
#define __ESOS_SUI_TASK_PERIOD         	(25*MILLISECONDS)

// PRIVATE DATA 

// PRIVATE FUNCTION PROTOTYPES
ESOS_USER_TASK( __esos_sui_task );

// PUBLIC API FUNCTION PROTOTYPES
void __esos_InitSUI(void);
ESOS_SUI_SWITCH_HANDLE   esos_sui_registerSwitch(uint32_t u32_d1, uint32_t u32_d2);
uint8_t esos_sui_getNumberOfSwitches(void);

BOOL esos_sui_isSWPressed ( ESOS_SUI_SWITCH_HANDLE h_sw );
BOOL esos_sui_isSWReleased ( ESOS_SUI_SWITCH_HANDLE h_sw );
BOOL esos_sui_isSWDoublePressed ( ESOS_SUI_SWITCH_HANDLE h_sw );
uint8_t esos_sui_getSwitchLastEvent (ESOS_SUI_SWITCH_HANDLE h_sw);
uint32_t esos_sui_getSwitchLastEventTime (ESOS_SUI_SWITCH_HANDLE h_sw);
uint32_t esos_sui_getSwitchUserData1 (ESOS_SUI_SWITCH_HANDLE h_sw); 
uint32_t esos_sui_getSwitchUserData2 (ESOS_SUI_SWITCH_HANDLE h_sw); 

ESOS_SUI_LED_HANDLE esos_sui_registerLED(uint32_t u32_d1, uint32_t u32_d2);
uint8_t esos_sui_getNumberOfLEDs(void);
BOOL esos_sui_isLEDOn ( ESOS_SUI_LED_HANDLE h_sw );
BOOL esos_sui_isLEDOff ( ESOS_SUI_LED_HANDLE h_sw );
BOOL esos_sui_isLEDOneShot ( ESOS_SUI_LED_HANDLE h_sw );
BOOL esos_sui_isLEDFlashing ( ESOS_SUI_LED_HANDLE h_sw );
void esos_sui_turnLEDOn ( ESOS_SUI_LED_HANDLE h_sw );
void esos_sui_turnLEDOff ( ESOS_SUI_LED_HANDLE h_sw );
void esos_sui_toggleLED ( ESOS_SUI_LED_HANDLE h_sw );
void esos_sui_oneshotLED (ESOS_SUI_LED_HANDLE   h_led, uint16_t u16_period);
void esos_sui_flashLED ( ESOS_SUI_LED_HANDLE h_sw, uint16_t u16_period);
void esos_sui_stopFlashLED (ESOS_SUI_LED_HANDLE   h_led);

uint16_t esos_sui_getLEDFlashPeriod (ESOS_SUI_LED_HANDLE   h_led);
uint32_t esos_sui_getLEDLastEventTime (ESOS_SUI_LED_HANDLE   h_led);
uint32_t esos_sui_getLEDUserData1 (ESOS_SUI_LED_HANDLE   h_led);
uint32_t esos_sui_getLEDUserData2 (ESOS_SUI_LED_HANDLE   h_led);

// PUBLIC HARDWARE FUNCTION PROTOTYPES
extern void  esos_hw_sui_turnLEDOn( ESOS_SUI_LED_HANDLE h_led );
extern void  esos_hw_sui_turnLEDOff( ESOS_SUI_LED_HANDLE h_led );
extern BOOL  esos_hw_sui_isSwitchPressed( ESOS_SUI_LED_HANDLE h_led );
extern BOOL  esos_hw_sui_isSwitchReleased( ESOS_SUI_LED_HANDLE h_led );
extern void  esos_hw_sui_configLED( ESOS_SUI_LED_HANDLE h_led );
extern void  esos_hw_sui_configSwitch( ESOS_SUI_LED_HANDLE h_led );

// PUBLIC API ESOS TASK MACROS
#define ESOS_TASK_WAIT_UNTIL_SUI_SW_PRESSED(x)                    ESOS_TASK_WAIT_UNTIL( esos_sui_isSWPressed((x)) )
#define ESOS_TASK_WAIT_UNTIL_SUI_SW_RELEASED(x)                   ESOS_TASK_WAIT_UNTIL( esos_sui_isSWReleased((x)) )
#define ESOS_TASK_WAIT_UNTIL_SUI_SW_PRESSED_AND_RELEASED(x)       do {                                                      \
                                                                    ESOS_TASK_WAIT_UNTIL_SUI_SW_PRESSED((x));           \
                                                                    ESOS_TASK_WAIT_UNTIL_SUI_SW_RELEASED((x));          \
                                                                  } while (0) 
#define ESOS_TASK_WAIT_UNTIL_SUI_SW_PRESSED_TWICE(x)              do {                                                    \
                                                                    ESOS_TASK_WAIT_UNTIL_SUI_SW_PRESSED((x));           \
                                                                    ESOS_TASK_WAIT_UNTIL_SUI_SW_RELEASED((x));          \
                                                                    ESOS_TASK_WAIT_UNTIL_SUI_SW_PRESSED((x));           \
                                                                    ESOS_TASK_WAIT_UNTIL_SUI_SW_RELEASED((x));          \
                                                                  } while (0)
#define ESOS_TASK_WAIT_UNTIL_SUI_SW_EVENT(x)               		 ESOS_TASK_WAIT_WHILE( esos_sui_getSwitchLastEvent((x)) == ESOS_SUI_SWITCH_EVENT_NULL )
#define ESOS_TASK_WAIT_UNTIL_SUI_SW_HOLD_EVENT(x)                ESOS_TASK_WAIT_UNTIL( esos_sui_getSwitchLastEvent((x)) == ESOS_SUI_SWITCH_EVENT_HOLD )
#define ESOS_TASK_WAIT_UNTIL_SUI_SW_CLICK_EVENT(x)               ESOS_TASK_WAIT_UNTIL( esos_sui_getSwitchLastEvent((x)) == ESOS_SUI_SWITCH_EVENT_CLICK )
#define ESOS_TASK_WAIT_UNTIL_SUI_SW_DOUBLE_CLICK_EVENT(x)        ESOS_TASK_WAIT_UNTIL( esos_sui_getSwitchLastEvent((x)) == ESOS_SUI_SWITCH_EVENT_DOUBLE_CLICK )


#endif    // ESOS_SUI
