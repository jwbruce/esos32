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
 * \brief Hardware indepdendent code for ESOS32's Simple User Interface
 * (LEDs and pushbuttons) service
 *
 */


/**
 * \addtogroup ESOS_Simple_User_Interface_Service
 * @{
 */

// INCLUDES
#include "esos_sui.h"
#include "esos_stm32l4.h"

// PRIVATE ESOS SUI SERVICE VARIABLES
volatile uint8_t    __u8_esosSuiNumLEDs = 0;
volatile uint8_t    __u8_esosSuiNumSWs = 0;
volatile _st_esos_sui_Switch    __ast_esosSuiSwitches[ ESOS_SUI_NUM_MAX_SWITCHES];
volatile _st_esos_sui_LED       __ast_esosSuiLEDs[ ESOS_SUI_NUM_MAX_LEDS];

// PRIVATE FUNCTIONS
/**
* Returns the number of switches being managed by ESOS SUI service
*
* \retval uint8_t  number of switches currently registered in the ESOS
* SUI service
* 
* \sa esos_sui_registerSwitch()
* \sa esos_hw_sui_configSwitches()
* \hideinitializer
*/
inline uint8_t esos_sui_getNumberOfSwitches(void){
  return( __u8_esosSuiNumSWs);
}

/**
* Returns the number of LEDs being managed by ESOS SUI service
*
* \retval uint8_t  number of LEDs currently registered in the ESOS
* SUI service
* 
* \sa esos_sui_registerLED()
* \sa esos_hw_sui_configLED()
* \hideinitializer
*/
inline uint8_t esos_sui_getNumberOfLEDs(void){
  return( __u8_esosSuiNumLEDs);
}

// PUBLIC SWITCH QUERIES (also GETTERs and SETTERs) ******************
/**
* Returns the state of the swtich
* \param h_sw  a "switch handle" to switch that was returned to user 	
* when the switch was registered
* \retval BOOL TRUE if switch is pressed, FALSE otherwise
* 
* \sa esos_sui_registerSwitch()
* \sa esos_hw_sui_isSwitchPressed()
* \sa esos_hw_sui_isSwitchReleased()
* \hideinitializer
*/
inline BOOL esos_sui_isSWPressed (ESOS_SUI_SWITCH_HANDLE   h_sw) {
  return (__ast_esosSuiSwitches[h_sw].b_pressed==TRUE);
}

/**
* Returns the state of the swtich
* \param h_sw  "switch handle" to switch that was returned to user
* when the switch was registered
* \retval BOOL  TRUE if switch is released, FALSE otherwise
* 
* \sa esos_sui_registerSwitch()
* \sa esos_hw_sui_isSwitchPressed()
* \sa esos_hw_sui_isSwitchReleased()
* \hideinitializer
*/
inline BOOL esos_sui_isSWReleased (ESOS_SUI_SWITCH_HANDLE   h_sw) {
  return (__ast_esosSuiSwitches[h_sw].b_pressed==FALSE && __ast_esosSuiSwitches[h_sw].b_doublePressed==FALSE);
}

/**
* Returns the state of the swtich
* \param h_sw  "switch handle" to switch that was returned to user
* when the switch was registered
* \retval BOOL TRUE if switch is double-pressed, FALSE otherwise
* 
* \sa esos_sui_registerSwitch()
* \sa esos_hw_sui_isSwitchPressed()
* \sa esos_hw_sui_isSwitchReleased()
* \hideinitializer
*/
inline BOOL esos_sui_isSWDoublePressed (ESOS_SUI_SWITCH_HANDLE   h_sw) {
  return (__ast_esosSuiSwitches[h_sw].b_doublePressed==TRUE);
}

/**
* Returns the most recent event (as opposed to the current state) 
* of a given switch
* \param h_sw "switch handle" to switch that was returned to user
* when the switch was registered
* \retval ESOS_SUI_SWITCH_STATE   the most recent switch event to occur
* based on h_sw actions
* 
* \note this call will "clear" the switch state event to ESOS_SUI_SWITCH_EVENT_NULL
* 
* \sa esos_sui_registerSwitch()
* \sa esos_sui_getSwitchLastEventTime()
* \hideinitializer
*/
inline uint8_t esos_sui_getSwitchLastEvent (ESOS_SUI_SWITCH_HANDLE   h_sw) {
  uint8_t    u8_retval;
  u8_retval = __ast_esosSuiSwitches[h_sw].u8_lastEvent;
  __ast_esosSuiSwitches[h_sw].u8_lastEvent = ESOS_SUI_SWITCH_EVENT_NULL;
  return u8_retval;
}

/**
* Returns the system tick value that corresponds to the most recent event
* (as opposed to the current state)  of a given switch
* \param h_sw "switch handle" to switch that was returned to user
* when the switch was registered
* \retval uint32_t system tick value when the last event occured
* 
* \note The value returned by this function is meaningless if the
* last event of the switch was ESOS_SUI_SWITCH_EVENT_NULL
* 
* \sa esos_sui_registerSwitch()
* \sa esos_sui_getSwitchLastEvent()

* \hideinitializer
*/
inline uint32_t esos_sui_getSwitchLastEventTime (ESOS_SUI_SWITCH_HANDLE   h_sw) {
  return (__ast_esosSuiSwitches[h_sw].u32_lastEventTime);
}

/**
* Returns the first user data word associated with this switch
* \param h_sw "switch handle" to switch that was returned to user
* when the switch was registered
* \retval uint32 the value of user data word #1
* 
* \note The user data value is set by the user when the switch is
* registerd in user_init().  User data can not be modified after the
* switch is created.
* 
* \sa esos_sui_registerSwitch()
* \hideinitializer
*/
inline uint32_t esos_sui_getSwitchUserData1 (ESOS_SUI_SWITCH_HANDLE   h_sw) {
  return (__ast_esosSuiSwitches[h_sw].u32_userData1);
}

/**
* Returns the second user data word associated with this switch
* \param h_sw "switch handle" to switch that was returned to user
* when the switch was registered
* \retval uint32 the value of user data word #2
* 
* \note The user data value is set by the user when the switch is
* registerd in user_init().  User data can not be modified after the
* switch is created.
* 
* \sa esos_sui_registerSwitch()
* \hideinitializer
*/
inline uint32_t esos_sui_getSwitchUserData2 (ESOS_SUI_SWITCH_HANDLE   h_sw) {
  return (__ast_esosSuiSwitches[h_sw].u32_userData2);
}

// PUBLIC LED FUNCTIONS

//LED QUERIES (also GETTERs and SETTERs)  ***************************
/**
* Returns the current state of the associated LED
* \param h_led "LED handle" to switch that was returned to user
* when the LED was registered
* \retval BOOL TRUE if LED is ON, FALSE otherwise
* 
* \sa esos_sui_registerLED()
* \sa esos_sui_turnLEDOn()
* \sa esos_sui_turnLEDOff()
* \hideinitializer
*/
inline BOOL esos_sui_isLEDOn (ESOS_SUI_LED_HANDLE   h_led) {
    return (__ast_esosSuiLEDs[h_led]._u8_state & ESOS_SUI_LED_STATE_ON);
}

/**
* Returns the current state of the associated LED
* \param h_led "LED handle" to switch that was returned to user
* when the LED was registered
* \retval BOOL TRUE if LED is OFF, FALSE otherwise
* 
* 
* \sa esos_sui_registerLED()
* \sa esos_sui_turnLEDOn()
* \sa esos_sui_turnLEDOff()
* \hideinitializer
*/
inline BOOL esos_sui_isLEDOff (ESOS_SUI_LED_HANDLE   h_led) {
    return (!esos_sui_isLEDOn(h_led));
}

/**
* Returns whether the associated LED is "flashing"
* \param h_led "LED handle" to switch that was returned to user
* when the LED was registered
* \retval BOOL TRUE if LED is currently flashing, FALSE otherwise
*  
* \sa esos_sui_registerLED()
* \sa esos_sui_turnLEDOn()
* \sa esos_sui_turnLEDOff()
* \hideinitializer
*/
inline BOOL esos_sui_isLEDFlashing (ESOS_SUI_LED_HANDLE   h_led) {
    if (__ast_esosSuiLEDs[h_led]._u8_state & ESOS_SUI_LED_STATE_FLASHING) {
		if (__ast_esosSuiLEDs[h_led].u16_LEDFlashPeriod != 0) {
			return TRUE;
		}
		else return FALSE;
	} else return FALSE;
}

/**
* Returns whether the associated LED is in a one-shot state
* \param h_led "LED handle" to switch that was returned to user
* when the LED was registered
* \retval BOOL TRUE if LED is currently flashing, FALSE otherwise
*  
* \sa esos_sui_registerLED()
* \sa esos_sui_turnLEDOn()
* \sa esos_sui_turnLEDOff()
* \hideinitializer
*/
inline BOOL esos_sui_isLEDOneShot (ESOS_SUI_LED_HANDLE   h_led) {
    if (__ast_esosSuiLEDs[h_led]._u8_state & ESOS_SUI_LED_STATE_ONE_SHOT) {
		if (__ast_esosSuiLEDs[h_led].u16_LEDFlashPeriod != 0) {
			return TRUE;
		}
	}
	return FALSE;
}

/**
* Returns the LED flashing period (in ticks)  as uint16
* associated with a given LED
* \param h_led "LED handle" to switch that was returned to user
* when the LED was registered
* \retval uint16_t  number of ticks = h_LED's flashing period
* 
* \note a return value of 0 denotes that the LED is not flashing but
* constantly ON or OFF
* 
* \sa esos_sui_registerLED()
* \sa esos_sui_flashLED()
* \sa esos_sui_turnLEDOn()
* \sa esos_sui_turnLEDOff()
* \hideinitializer
*/
inline uint16_t esos_sui_getLEDFlashPeriod (ESOS_SUI_LED_HANDLE   h_led) {
    return (__ast_esosSuiLEDs[h_led].u16_LEDFlashPeriod);
}

/**
* Returns the system tick value that corresponds to the most recent event
* associated with a given LED
* \param h_LED "LED handle" to switch that was returned to user
* when the LED was registered
* \retval uint32_t  system tick value when the last LED event occured
* 
* \sa esos_sui_registerLED()
* \hideinitializer
*/
inline uint32_t esos_sui_getLEDLastEventTime (ESOS_SUI_LED_HANDLE   h_led) {
    return (__ast_esosSuiLEDs[h_led].u32_lastEventTime);
}

/**
* Returns the first user data word associated with this LED
* \param h_led "LED handle" to switch that was returned to user
* when the LED was registered
* \retval uint32 the LED's user data word #1
* 
* \note The user data value is set by the user when the LED is
* registerd in user_init().  User data can not be modified after the
* LED is registered.
* 
* \sa esos_sui_registerLED()
* \hideinitializer
*/
inline uint32_t esos_sui_getLEDUserData1 (ESOS_SUI_LED_HANDLE   h_led) {
    return (__ast_esosSuiLEDs[h_led].u32_userData1);
}

/**
* Returns the second user data word associated with this LED
* \param h_led "LED handle" to switch that was returned to user
* when the LED was registered
* \retval uint32 the LED's user data word #2
* 
* \note The user data value is set by the user when the LED is
* registerd in user_init().  User data can not be modified after the
* LED is registered.
* 
* \sa esos_sui_registerLED()
* \hideinitializer
*/
inline uint32_t esos_sui_getLEDUserData2 (ESOS_SUI_LED_HANDLE   h_led) {
    return (__ast_esosSuiLEDs[h_led].u32_userData2);
}

//END LED QUERIES ***************************

//LED ACTUATION *****************************
/**
* called by user application to turn a given LED on
* \param h_led "LED handle" to switch that was returned to user
* when the LED was registered
* 
* \sa esos_sui_registerLED()
* \sa esos_sui_flashLED()
* \sa esos_sui_turnLEDOff()
* \sa esos_sui_toggleLED()
* \hideinitializer
*/
inline void esos_sui_turnLEDOn (ESOS_SUI_LED_HANDLE   h_led) {
    __ast_esosSuiLEDs[h_led]._u8_state |= ESOS_SUI_LED_STATE_ON;
    return;
}

/**
* called by user application to turn a given LED off
* \param h_led "LED handle" to switch that was returned to user
* when the LED was registered
* 
* \sa esos_sui_registerLED()
* \sa esos_sui_flashLED()
* \sa esos_sui_turnLEDOn()
* \sa esos_sui_toggleLED()
* \hideinitializer
*/
inline void esos_sui_turnLEDOff (ESOS_SUI_LED_HANDLE   h_led) {
    __ast_esosSuiLEDs[h_led]._u8_state &= (~ESOS_SUI_LED_STATE_ON);
    return;
}

/**
* called by user application to toggle a given LED
  ESOS_TASK_BEGIN();
* \param h_led "LED handle" to switch that was returned to user
* when the LED was registered
* 
* \sa esos_sui_registerLED()
* \sa esos_sui_flashLED()
* \sa esos_sui_turnLEDOn()
* \sa esos_sui_turnLEDOff()
* \hideinitializer
*/
inline void esos_sui_toggleLED (ESOS_SUI_LED_HANDLE   h_led) {
    __ast_esosSuiLEDs[h_led]._u8_state ^= ESOS_SUI_LED_STATE_ON;
    return;
}

/**
* called by user application to start a one-shot flash (illuminate)
* the given LED for the next u16_period ticks
* \param h_led "LED handle" to switch that was returned to user
* when the LED was registered
* \param u16_period value (in ticks) that represents how long the
* single illumination of the LED
* \note if u16_period is zero, the LED will not illuminate
* \note to cancel a ONE-SHOT in progress, set the LED's "flash period
* to zero.
* \sa esos_sui_registerLED()
* \sa esos_sui_toggleLED()
* \sa esos_sui_turnLEDOn()
* \sa esos_sui_turnLEDOff()
* \hideinitializer
*/
inline void esos_sui_oneshotLED (ESOS_SUI_LED_HANDLE   h_led, uint16_t u16_period) {
	// ONE-SHOT and FLASHING are MUTEX (as of right now)
    __ast_esosSuiLEDs[h_led]._u8_state &= (~ESOS_SUI_LED_STATE_FLASHING);
    __ast_esosSuiLEDs[h_led]._u8_state |= ESOS_SUI_LED_STATE_ONE_SHOT;
    __ast_esosSuiLEDs[h_led].u16_LEDFlashPeriod = u16_period;
    return;
}


/**
* called by user application to flash/toggle a given LED
* every u16_period ticks
* \param h_led "LED handle" to switch that was returned to user
* when the LED was registered
* \param u16_period  uint16 value (in ticks) that represents the flash
* period of the LED
* \note if u16_period is zero, the LED will not flash
* \sa esos_sui_registerLED()
* \sa esos_sui_toggleLED()
* \sa esos_sui_turnLEDOn()
* \sa esos_sui_turnLEDOff()
* \hideinitializer
*/
inline void esos_sui_flashLED (ESOS_SUI_LED_HANDLE   h_led, uint16_t u16_period) {
	// ONE-SHOT and FLASHING are MUTEX (as of right now)
    __ast_esosSuiLEDs[h_led]._u8_state &= (~ESOS_SUI_LED_STATE_ONE_SHOT);
    __ast_esosSuiLEDs[h_led]._u8_state |= ESOS_SUI_LED_STATE_FLASHING;
    __ast_esosSuiLEDs[h_led].u16_LEDFlashPeriod = u16_period;
    return;
}

/**
* called by user application to toggle a given LED
* \param h_led "LED handle" to switch that was returned to user
* when the LED was registered
* \sa esos_sui_registerLED()
* \sa esos_sui_toggleLED()
* \sa esos_sui_turnLEDOn()
* \sa esos_sui_turnLEDOff()
* \hideinitializer
*/
inline void esos_sui_stopFlashLED (ESOS_SUI_LED_HANDLE   h_led) {
    __ast_esosSuiLEDs[h_led]._u8_state = ESOS_SUI_LED_STATE_ALL_OFF;
    __ast_esosSuiLEDs[h_led].u16_LEDFlashPeriod = 0;
    return;
}

//END LED ACTUATION******************************

/**
* called by user application code to "register" a new
* LED with the ESOS SUI service
*
* \param u32_d1 uint32 for user data 1
* \param u32_d2 uint32 for user data 2
* \retval ESOS_SUI_LED_HANDLE that can be used to reference the LED
* with ESOS SUI service going foward
* 
* \note This routine must be called in user_init()
* \note At this point, LEDs can NOT be unregistered with ESOS SUI 
* service
* \note The two user data fields are very "handy" for storing the
* port and pin, respectively for the associated LED.
*
* \sa esos_sui_registerLED()
* \sa esos_hw_sui_configLED()
* \sa sos_hw_sui_turnLEDOff()
* \sa sos_hw_sui_turnLEDOn()
* \hideinitializer
*/
ESOS_SUI_LED_HANDLE esos_sui_registerLED(uint32_t u32_d1, uint32_t u32_d2) {
  uint8_t	u8_thisLED;
  
  u8_thisLED = __u8_esosSuiNumLEDs;   // "handle" to the new LED
  __u8_esosSuiNumLEDs++;
  
  // setup our LED as being off and no flashing
  __ast_esosSuiLEDs[u8_thisLED].b_LEDOn = FALSE;
  __ast_esosSuiLEDs[u8_thisLED]._u8_state = ESOS_SUI_LED_STATE_ALL_OFF;  
  __ast_esosSuiLEDs[u8_thisLED].u16_LEDFlashPeriod = 0;    
  __ast_esosSuiLEDs[u8_thisLED].u8_lastEvent = ESOS_SUI_LED_EVENT_OFF;
  __ast_esosSuiLEDs[u8_thisLED].u32_lastEventTime = 0;
  // save the user-provided data
  __ast_esosSuiLEDs[u8_thisLED].u32_userData1 = u32_d1;
  __ast_esosSuiLEDs[u8_thisLED].u32_userData2 = u32_d2;    
  // and return the LED "handle"
  return ((ESOS_SUI_LED_HANDLE)u8_thisLED);
} // end esos_sui_registerLED()

/**
* called by user application code to "register" a new
* switch with the ESOS SUI service
*
* \param u32_d1 uint32 for user data 1
* \param u32_d2 uint32 for user data 2
* \retval ESOS_SUI_SWITCH_HANDLE that can be used to reference the switch
* with ESOS SUI service going foward
* 
* \note This routine must be called in user_init()
* \note At this point, switches can NOT be unregistered with ESOS SUI 
* service
* \note The two user data fields are very "handy" for storing the
* port and pin, respectively for the associated switch.
*
* \sa esos_sui_registerSwitch()
* \sa esos_hw_sui_configSwitch()
* \hideinitializer
*/
ESOS_SUI_SWITCH_HANDLE   esos_sui_registerSwitch(uint32_t u32_d1, uint32_t u32_d2) {
  uint8_t     u8_thisSwitch;
  
  u8_thisSwitch = __u8_esosSuiNumSWs;       // "handle" to the new switch
  __u8_esosSuiNumSWs++;
  
  // setup our switches structures
  __ast_esosSuiSwitches[u8_thisSwitch].b_pressed = FALSE;
  __ast_esosSuiSwitches[u8_thisSwitch].b_doublePressed = FALSE;    
  __ast_esosSuiSwitches[u8_thisSwitch]._u8_state = 0;

  // save the user-provided data
  __ast_esosSuiSwitches[u8_thisSwitch].u32_userData1 = u32_d1;
  __ast_esosSuiSwitches[u8_thisSwitch].u32_userData2 = u32_d2;    
  // and return the switch "handle"
  return ((ESOS_SUI_SWITCH_HANDLE) u8_thisSwitch);
} // end esos_sui_registerSwitch()

/**
* A "hidden" (system task) that manages the LEDs and switches for
* the ESOS SUI service
*
* \hideinitializer
*/

// SUI task to periodically manage the simple user-interface elements
ESOS_USER_TASK( __esos_sui_task ){
  ESOS_SUI_LED_HANDLE     	h_LED;
  ESOS_SUI_SWITCH_HANDLE  	h_SW;
  uint32_t    				u32_now;
  BOOL						b_swState;
  
  ESOS_TASK_BEGIN();
  while(TRUE) {
      u32_now = esos_GetSystemTick();
      // process the LEDS
	  for (h_LED=0; h_LED<esos_sui_getNumberOfLEDs(); h_LED++) {
		  // process the state information for each LED, in turn.
		  //  At the conclusion of each LED, actually change the hardware.
		  
		  // if this LED has a flashPeriod == 0, then we can NOT be
		  //   flashing or one-shot, so update the LEDs state.
		  if (__ast_esosSuiLEDs[h_LED].u16_LEDFlashPeriod == 0 ) {
			  __ast_esosSuiLEDs[h_LED]._u8_state &= (~ESOS_SUI_LED_STATE_ONE_SHOT);
			  __ast_esosSuiLEDs[h_LED]._u8_state &= (~ESOS_SUI_LED_STATE_FLASHING);
		  }

		  // check state of LEDs and act.  First let's make sure LEDs that
		  //   are OFF are really off
		  if (__ast_esosSuiLEDs[h_LED]._u8_state == ESOS_SUI_LED_STATE_ALL_OFF ) {
			esos_sui_turnLEDOff( h_LED );
			if (__ast_esosSuiLEDs[h_LED].u8_lastEvent != ESOS_SUI_LED_EVENT_OFF ) {
				// record the LED off EVENT if it hasn't been done yet.
				__ast_esosSuiLEDs[h_LED].u32_lastEventTime = u32_now;
				__ast_esosSuiLEDs[h_LED].u8_lastEvent = ESOS_SUI_LED_EVENT_OFF;
			}
		  } 
		  else if (__ast_esosSuiLEDs[h_LED]._u8_state & ESOS_SUI_LED_STATE_ONE_SHOT ) {
			// we are in ONE-SHOT mode, make sure the start of the ONE-SHOT
			//   event was recorded
			if (__ast_esosSuiLEDs[h_LED].u8_lastEvent != ESOS_SUI_LED_EVENT_ON ) {
				__ast_esosSuiLEDs[h_LED].u32_lastEventTime = u32_now;
				__ast_esosSuiLEDs[h_LED].u8_lastEvent = ESOS_SUI_LED_EVENT_ON;
			}
			if( esos_sui_isLEDOff( h_LED )) {
				// LED is off, so turn it on for the ONE-SHOT
				esos_sui_turnLEDOn( h_LED );
			} else {
				// LED is on, so check to see if ONE-SHOT duration has expired
				if (u32_now >= __ast_esosSuiLEDs[h_LED].u32_lastEventTime + __ast_esosSuiLEDs[h_LED].u16_LEDFlashPeriod) {
					// ONE-SHOT duration has expired, so turn the LED off
					//    and restore to normal mode
					__ast_esosSuiLEDs[h_LED].u32_lastEventTime = u32_now;
					__ast_esosSuiLEDs[h_LED].u8_lastEvent = ESOS_SUI_LED_EVENT_OFF;
					__ast_esosSuiLEDs[h_LED]._u8_state = ESOS_SUI_LED_STATE_ALL_OFF;
					esos_sui_turnLEDOff( h_LED );
				}
			}
		}
		// Now, deal with FLASHING LEDs
		else if (__ast_esosSuiLEDs[h_LED]._u8_state & ESOS_SUI_LED_STATE_FLASHING ) {
				//  compare lastEvent time versus now time to see if we need
				//  to change the LED state
				if( (u32_now - __ast_esosSuiLEDs[h_LED].u32_lastEventTime) > (__ast_esosSuiLEDs[h_LED].u16_LEDFlashPeriod/2) ) {
					__ast_esosSuiLEDs[h_LED].u32_lastEventTime = u32_now;
					if (__ast_esosSuiLEDs[h_LED].u8_lastEvent == ESOS_SUI_LED_EVENT_ON ) {
						esos_sui_turnLEDOff( h_LED );
						__ast_esosSuiLEDs[h_LED].u8_lastEvent = ESOS_SUI_LED_EVENT_OFF;
					} else {
						esos_sui_turnLEDOn( h_LED );
						__ast_esosSuiLEDs[h_LED].u8_lastEvent = ESOS_SUI_LED_EVENT_ON;					
					} // end if-else
				}
			}
			// Now, we have the plain-jane ON/OFF LEDs
			else {	
			  // if LED flash period is ZERO, then we are ON/OFF  
			  if( esos_sui_isLEDOn( h_LED )) {
				if (__ast_esosSuiLEDs[h_LED].u8_lastEvent != ESOS_SUI_LED_EVENT_ON ) {
				  __ast_esosSuiLEDs[h_LED].u32_lastEventTime = u32_now;
				  __ast_esosSuiLEDs[h_LED].u8_lastEvent = ESOS_SUI_LED_EVENT_ON;
				}
			  }
			  if( esos_sui_isLEDOff( h_LED )) {
				if (__ast_esosSuiLEDs[h_LED].u8_lastEvent != ESOS_SUI_LED_EVENT_OFF ) {
				  __ast_esosSuiLEDs[h_LED].u32_lastEventTime = u32_now;
				  __ast_esosSuiLEDs[h_LED].u8_lastEvent = ESOS_SUI_LED_EVENT_OFF;
				}
			  } 
		    }  // end else
		    
		    // Now the state for thhs (h_LED) should be correct, so set
		    //   the hardware LED accordingly
		    if (esos_sui_isLEDOn( h_LED )) {
				esos_hw_sui_turnLEDOn( h_LED );
			} else {
				esos_hw_sui_turnLEDOff( h_LED );
			}
	    } // end for(h_LED) loop

	  // ***********************************************************
      // Now, let's process the SWITCHES
      // ***********************************************************
      for (h_SW=0; h_SW<esos_sui_getNumberOfSwitches(); h_SW++) {
		b_swState = esos_hw_sui_isSwitchPressed(h_SW);   
		// update the instantaneous state of the switch's structure
		//   for apps that want to use that info
		__ast_esosSuiSwitches[h_SW].b_pressed = b_swState;
        // we case use the switch-case statement here because it
        //    will be completely executed in the current context
        // WARNING:   Do not add any ESOS_TASK_WAIT_xxxx statements
        //            within this switch-case statement
        switch( __ast_esosSuiSwitches[h_SW]._u8_state ) {
          case ESOS_SUI_SWSTATE_IDLE: {
            // Waiting for the "button" to be pressed.
            if(b_swState) {
              __ast_esosSuiSwitches[h_SW]._u8_state = ESOS_SUI_SWSTATE_WAIT_SW_UP;
              __ast_esosSuiSwitches[h_SW]._u32_nextTickTime = u32_now + HOLD_TICK;
            }
            break;
          }
          
          // waiting for the button to be released.
          case ESOS_SUI_SWSTATE_WAIT_SW_UP: {
            if(!b_swState && (__ast_esosSuiSwitches[h_SW]._u32_nextTickTime > u32_now)) {
              // Detected that button was released (quickly), before our HOLD_TICK duration
              __ast_esosSuiSwitches[h_SW]._u8_state = ESOS_SUI_SWSTATE_WAIT_CLICK_TIMEOUT;
              __ast_esosSuiSwitches[h_SW]._u32_nextTickTime = u32_now + CLICK_TICK;
              break;
            }
            if(!b_swState && (__ast_esosSuiSwitches[h_SW]._u32_nextTickTime <= u32_now)) {
              // Detected that button was released (slowly), after our HOLD_TICK duration
              __ast_esosSuiSwitches[h_SW].u8_lastEvent = ESOS_SUI_SWITCH_EVENT_HOLD;
              __ast_esosSuiSwitches[h_SW].u32_lastEventTime = u32_now;
              __ast_esosSuiSwitches[h_SW]._u8_state = ESOS_SUI_SWSTATE_IDLE;
              __ast_esosSuiSwitches[h_SW]._u32_nextTickTime = 0;
              break;
            }
            break;
          }
          
          case ESOS_SUI_SWSTATE_WAIT_CLICK_TIMEOUT: {
            // Waiting for a button to be pressed the second time or timeout.
            if(b_swState && (__ast_esosSuiSwitches[h_SW]._u32_nextTickTime > u32_now)) {
              __ast_esosSuiSwitches[h_SW]._u8_state = ESOS_SUI_SWSTATE_WAIT_DCLICK_TIMEOUT;
              __ast_esosSuiSwitches[h_SW]._u32_nextTickTime = u32_now + DOUBLE_CLICK_TICK;
              break;
            }
            if( __ast_esosSuiSwitches[h_SW]._u32_nextTickTime <= u32_now) {
              // Detected a Click event
              __ast_esosSuiSwitches[h_SW].u8_lastEvent = ESOS_SUI_SWITCH_EVENT_CLICK;
              __ast_esosSuiSwitches[h_SW].u32_lastEventTime = u32_now;                            
              __ast_esosSuiSwitches[h_SW]._u8_state = ESOS_SUI_SWSTATE_IDLE;
              __ast_esosSuiSwitches[h_SW]._u32_nextTickTime = 0;
              break;
            }
            break;
          }
          
          case ESOS_SUI_SWSTATE_WAIT_DCLICK_TIMEOUT: {
            // If logic Level = HIGH (release Button) => we got double-click
            // If we timeout, leave switch event at CLICK
            // Either way, change state to IDLE
            if(!b_swState && (__ast_esosSuiSwitches[h_SW]._u32_nextTickTime > u32_now)) {
              // quick release, so generate a double click event.
              __ast_esosSuiSwitches[h_SW].u8_lastEvent = ESOS_SUI_SWITCH_EVENT_DOUBLE_CLICK;
              __ast_esosSuiSwitches[h_SW].u32_lastEventTime = u32_now;
              __ast_esosSuiSwitches[h_SW]._u8_state = ESOS_SUI_SWSTATE_IDLE;
              __ast_esosSuiSwitches[h_SW]._u32_nextTickTime = 0;
            break;
            }
			if( __ast_esosSuiSwitches[h_SW]._u32_nextTickTime <= u32_now) {
              // second click timed-out, so reset switch internal state to IDLE
              __ast_esosSuiSwitches[h_SW]._u8_state = ESOS_SUI_SWSTATE_IDLE;
              __ast_esosSuiSwitches[h_SW]._u32_nextTickTime = 0;
              break;
            }
          break;
          }
        } // end switch
      } // end for(h_SW) loop 
      // END SWITCHES *************************************************
      
    ESOS_TASK_WAIT_TICKS( __ESOS_SUI_TASK_PERIOD );
  } // end while(TRUE)
  ESOS_TASK_END();
} // end __esos_sui_task() TASK


/**
* ESOS system call to initialize the SUI element
*
* \note This routine assumes that all of the desired SUI elements
* have been registered with the ESOS SUI service.   In other words,
* this routine is called by ESOS after user_init()
*
* \sa user_init()
* \sa esos_sui_registerSwitch()
* \sa esos_sui_registerLED()
* \sa esos_hw_sui_configLED()
* \sa sos_hw_sui_turnLEDOff()
* \sa esos_hw_sui_configSwitch()
* \hideinitializer
*/
void __esos_InitSUI(void) {
  uint8_t       u8_i;

  // call the user hardware functions to configure the LEDs
  //   start the LEDs in the "off" state
  for (u8_i=0; u8_i<esos_sui_getNumberOfLEDs(); u8_i++) {
    esos_hw_sui_configLED( u8_i );
    esos_hw_sui_turnLEDOff( u8_i );
  }
  // call the user hardware functions to configure the switches  
  for (u8_i=0; u8_i<esos_sui_getNumberOfSwitches(); u8_i++) {
    esos_hw_sui_configSwitch( u8_i );
  }

  // finally, register the hidden ESOS SUI task
  esos_RegisterTask( __esos_sui_task );
}
