/*
 * "Copyright (c) 2020 J. W. Bruce ("AUTHOR(S)")"
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

#include "esos_sensor.h"
#include <esos.h>
#include <stdlib.h>
 
/**
* \addtogroup ESOS_Task_Sensor_Service
* @{
*/

/**
* Waits until a sensor is available.
*
* \param e_senCh          enumeration to select sensor channel
* \param e_senVRef        enumeration to select sensor voltage reference
*
* \hideinitializer
*/
ESOS_CHILD_TASK(_WAIT_ON_AVAILABLE_SENSOR, esos_sensor_ch_t e_senCh, esos_sensor_vref_t e_senVRef)
{
	ESOS_TASK_BEGIN();
	
	// Wait for and then grab the ADC.
	ESOS_TASK_WAIT_WHILE(__esos_IsSystemFlagSet(__ESOS_SYS_ADC_IS_BUSY));
	__esos_SetSystemFlag(__ESOS_SYS_ADC_IS_BUSY);

    esos_sensor_config_hw(e_senCh, e_senVRef);

	ESOS_TASK_END();
}

/**
* Waits until a sensor is read (quick version).
*
* \param pu16_data        pointer to the location of the resulting data
*
* \hideinitializer
*/
ESOS_CHILD_TASK(_WAIT_SENSOR_QUICK_READ, uint16_t* pu16_data)
{
	ESOS_TASK_BEGIN();
	
    esos_sensor_initiate_hw();
    ESOS_TASK_WAIT_WHILE(esos_sensor_is_converting_hw());
    *pu16_data = esos_sensor_getvalue_u16_hw();

	ESOS_TASK_END();
}

/**
* Compares two data values
*
* \param pv_a        pointer to the first value in comparison
* \param pv_b        pointer to the second value in comparison
*
* \hideinitializer
*/
int compareUint16(const void *pv_a, const void *pv_b)
{
	return *((uint16_t*)pv_a) - *((uint16_t*)pv_b);
}

/**
* Finds the median of a buffer of data
*
* \param pu16_buf        pointer to the buffer
* \param u8_nelem        number of elements in the buffer
*
* \hideinitializer
*/
uint16_t medianOfBuffer(uint16_t *pu16_buf, uint8_t u8_nelem)
{
	qsort(pu16_buf, u8_nelem, sizeof(uint16_t), compareUint16);

	// Always assumes buflen is even, and that the inputs are max 12 bit!
	return (pu16_buf[u8_nelem / 2 - 1] + pu16_buf[u8_nelem / 2]) / 2;
}

/**
* Finds the maximum value of a buffer of data
*
* \param pu16_buf        pointer to the buffer
* \param u8_nelem        number of elements in the buffer
*
* \hideinitializer
*/
uint16_t maxOfBuffer(uint16_t *pu16_buf, uint8_t u8_nelem)
{
	uint16_t max = 0;
	uint8_t i = 0; 

	for(;i<u8_nelem;i++){
		if(max < pu16_buf[i]){
			max = pu16_buf[i];
		}
	}
	return max;
}

/**
* Waits until a sensor is read (extended version).
*
* \param pu16_data           pointer to the resultant data
* \param e_senProcess        enumeration of type of processing to be completed
* \param e_senFMT            enumeration of the format of the resultant data
*
* \hideinitializer
*/
ESOS_CHILD_TASK(_WAIT_SENSOR_READ, uint16_t* pu16_data, uint8_t e_senProcess, esos_sensor_format_t e_senFMT)
{
	ESOS_TASK_BEGIN();

	static uint8_t FMT_CONSTANT;
	static uint8_t vRef;

	static uint16_t au16_dataArr[64] = {0};
	static uint8_t arrayCount = 0;
	uint16_t u16_oneShot = *pu16_data;
	static uint32_t u32_algData;

	arrayCount = 0;
	u32_algData = 0;
	//Decompose e_senFMT to determine Vref and Format Constant
	FMT_CONSTANT = e_senFMT & 0b11110000;
	vRef = e_senFMT & 0b00001111;

	//2 Samples
	if((e_senProcess & 0b00001111) == 1){
		while(arrayCount < 2){
            esos_sensor_initiate_hw();
            ESOS_TASK_WAIT_WHILE(esos_sensor_is_converting_hw());
			au16_dataArr[arrayCount++] = esos_sensor_getvalue_u16_hw();
		}
	}
	//4 Samples
	else if((e_senProcess & 0b00001111) == 2){
		while(arrayCount < 4){
            esos_sensor_initiate_hw();
            ESOS_TASK_WAIT_WHILE(esos_sensor_is_converting_hw());
			au16_dataArr[arrayCount++] = esos_sensor_getvalue_u16_hw();
		}
	}
	//8 Samples
	else if((e_senProcess & 0b00001111) == 3){
		while(arrayCount < 8){
            esos_sensor_initiate_hw();
            ESOS_TASK_WAIT_WHILE(esos_sensor_is_converting_hw());
			au16_dataArr[arrayCount++] = esos_sensor_getvalue_u16_hw();
		}
	}
	//16 Samples
	else if((e_senProcess & 0b00001111) == 4){
		while(arrayCount < 16){
            esos_sensor_initiate_hw();
            ESOS_TASK_WAIT_WHILE(esos_sensor_is_converting_hw());
			au16_dataArr[arrayCount++] = esos_sensor_getvalue_u16_hw();
		}
	}
	//32 Samples
	else if((e_senProcess & 0b00001111) == 5){
		while(arrayCount < 32){
            esos_sensor_initiate_hw();
            ESOS_TASK_WAIT_WHILE(esos_sensor_is_converting_hw());
			au16_dataArr[arrayCount++] = esos_sensor_getvalue_u16_hw();
		}
	}
	//64 Samples
	else if((e_senProcess & 0b00001111) == 6){
		while(arrayCount < 64){
            esos_sensor_initiate_hw();
            ESOS_TASK_WAIT_WHILE(esos_sensor_is_converting_hw());
			au16_dataArr[arrayCount++] = esos_sensor_getvalue_u16_hw();
		}
	}
	
	//Reset *pu16_data to zero since everything is now in the array
	*pu16_data = 0;
	arrayCount = 0; // resetting to zero before reusing in process algorithms.

	

	//Do Nothing
	if(e_senProcess == ESOS_SENSOR_ONE_SHOT){
        esos_sensor_initiate_hw();
        ESOS_TASK_WAIT_WHILE(esos_sensor_is_converting_hw());
		*pu16_data = esos_sensor_getvalue_u16_hw();
	}
	//Average
	else if(e_senProcess == ESOS_SENSOR_AVG2){
		while(arrayCount < 2){
			u32_algData += au16_dataArr[arrayCount];
			arrayCount ++;
		}
		u32_algData = u32_algData/arrayCount;
		*pu16_data = (uint16_t)u32_algData;
	}	
	else if(e_senProcess == ESOS_SENSOR_AVG4){
		while(arrayCount < 4){
			u32_algData += au16_dataArr[arrayCount];
			arrayCount ++;
		}
		u32_algData = u32_algData/arrayCount;
		*pu16_data = (uint16_t)u32_algData;
	}
	else if(e_senProcess == ESOS_SENSOR_AVG8){
		while(arrayCount < 8){
			u32_algData += au16_dataArr[arrayCount];
			arrayCount ++;
		}
		u32_algData = u32_algData/arrayCount;
		*pu16_data = (uint16_t)u32_algData;
	}
	else if(e_senProcess == ESOS_SENSOR_AVG16){
		while(arrayCount < 16){
			u32_algData += au16_dataArr[arrayCount];
			arrayCount ++;
		}
		u32_algData = u32_algData/arrayCount;
		*pu16_data = (uint16_t)u32_algData;
	}
	else if(e_senProcess == ESOS_SENSOR_AVG32){
		while(arrayCount < 32){
			u32_algData += au16_dataArr[arrayCount];
			arrayCount ++;
		}
		u32_algData = u32_algData/arrayCount;
		*pu16_data = (uint16_t)u32_algData;
	}
	else if(e_senProcess == ESOS_SENSOR_AVG64){
		while(arrayCount < 64){
			u32_algData += au16_dataArr[arrayCount];
			arrayCount ++;
		}
		u32_algData = u32_algData/arrayCount;
		*pu16_data = (uint16_t)u32_algData;
	}
	//Minimum
	else if(e_senProcess == ESOS_SENSOR_MIN2){
		*pu16_data = ESOS_SENSOR_MAX16; // default to maximum 16 bit value.
		while(arrayCount < 2){
			if (au16_dataArr[arrayCount] < *pu16_data){
				*pu16_data = au16_dataArr[arrayCount];
			}
			arrayCount ++;
		}		
	}			
	else if(e_senProcess == ESOS_SENSOR_MIN4){
		*pu16_data = ESOS_SENSOR_MAX16; // default to maximum 16 bit value.
		while(arrayCount < 4){
			if (au16_dataArr[arrayCount] < *pu16_data){
				*pu16_data = au16_dataArr[arrayCount];
			}
			arrayCount ++;
		}		
	}
	else if(e_senProcess == ESOS_SENSOR_MIN8){
		*pu16_data = ESOS_SENSOR_MAX16; // default to maximum 16 bit value.
		while(arrayCount < 8){
			if (au16_dataArr[arrayCount] < *pu16_data){
				*pu16_data = au16_dataArr[arrayCount];
			}
			arrayCount ++;
		}		
	}
	else if(e_senProcess == ESOS_SENSOR_MIN16){
		*pu16_data = ESOS_SENSOR_MAX16; // default to maximum 16 bit value.
		while(arrayCount < 16){
			if (au16_dataArr[arrayCount] < *pu16_data){
				*pu16_data = au16_dataArr[arrayCount];
			}
			arrayCount ++;
		}		
	}
	else if(e_senProcess == ESOS_SENSOR_MIN32){
		*pu16_data = ESOS_SENSOR_MAX16; // default to maximum 16 bit value.
		while(arrayCount < 32){
			if (au16_dataArr[arrayCount] < *pu16_data){
				*pu16_data = au16_dataArr[arrayCount];
			}
			arrayCount ++;
		}		
	}
	else if(e_senProcess == ESOS_SENSOR_MIN64){
		*pu16_data = ESOS_SENSOR_MAX16; // default to maximum 16 bit value.
		while(arrayCount < 64){
			if (au16_dataArr[arrayCount] < *pu16_data){
				*pu16_data = au16_dataArr[arrayCount];
			}
			arrayCount ++;
		}		
	}
	//Maximum
	else if(e_senProcess == ESOS_SENSOR_MAX2){
		*pu16_data = maxOfBuffer(au16_dataArr, 2);
	}			
	else if(e_senProcess == ESOS_SENSOR_MAX4){
		*pu16_data = maxOfBuffer(au16_dataArr, 4);
	}
	else if(e_senProcess == ESOS_SENSOR_MAX8){
		*pu16_data = maxOfBuffer(au16_dataArr, 8);
	}
	else if(e_senProcess == ESOS_SENSOR_MAX16){
		*pu16_data = maxOfBuffer(au16_dataArr, 16);
	}
	else if(e_senProcess == ESOS_SENSOR_MAX32){
		*pu16_data = maxOfBuffer(au16_dataArr, 32);
	}
	else if(e_senProcess == ESOS_SENSOR_MAX64){
		*pu16_data = maxOfBuffer(au16_dataArr, 64);
	}
	//Median
	else if(e_senProcess == ESOS_SENSOR_MEDIAN2){
		*pu16_data = medianOfBuffer(au16_dataArr, 2);
	}			
	else if(e_senProcess == ESOS_SENSOR_MEDIAN4){
		*pu16_data = medianOfBuffer(au16_dataArr, 4);
	}
	else if(e_senProcess == ESOS_SENSOR_MEDIAN8){
		*pu16_data = medianOfBuffer(au16_dataArr, 8);
	}
	else if(e_senProcess == ESOS_SENSOR_MEDIAN16){
		*pu16_data = medianOfBuffer(au16_dataArr, 16);
	}
	else if(e_senProcess == ESOS_SENSOR_MEDIAN32){
		*pu16_data = medianOfBuffer(au16_dataArr, 32);
	}
	else if(e_senProcess == ESOS_SENSOR_MEDIAN64){
		*pu16_data = medianOfBuffer(au16_dataArr, 64);
	}
	else{}

	//Export as Voltage
	if(FMT_CONSTANT & ESOS_SENSOR_FORMAT_VOLTAGE){
		const uint32_t u32_maxDeciMilliVolts =
			  vRef == ESOS_SENSOR_VREF_1V0 ?   10000
			: vRef == ESOS_SENSOR_VREF_1V024 ? 10240
			: vRef == ESOS_SENSOR_VREF_2V0 ?   20000
			: vRef == ESOS_SENSOR_VREF_2V048 ? 20480
			: vRef == ESOS_SENSOR_VREF_3V0 ?   30000
			: vRef == ESOS_SENSOR_VREF_3V3 ?   33000
			: vRef == ESOS_SENSOR_VREF_4V0 ?   40000
			: vRef == ESOS_SENSOR_VREF_4V096 ? 40960
			: vRef == ESOS_SENSOR_VREF_5V0 ?   50000
			: 0;
		const uint32_t u32_divDeciMillivolts = (uint32_t)(*pu16_data) * u32_maxDeciMilliVolts;
		const uint32_t u32_DeciMillivolts = u32_divDeciMillivolts / 4096;
		*pu16_data = (uint16_t)u32_DeciMillivolts;
	}

	//Export as Percent
	else if(FMT_CONSTANT & ESOS_SENSOR_FORMAT_PERCENT) {
		*pu16_data = (uint32_t)(*pu16_data) * 100 / 4096;
	}


	ESOS_TASK_END();
}

/**
* Waits until a sensor is closed.
*
* \hideinitializer
*/
BOOL ESOS_SENSOR_CLOSE(void)
{
    esos_sensor_release_hw();
	// Release the flag
	__esos_ClearSystemFlag(__ESOS_SYS_ADC_IS_BUSY);

	return TRUE;
}
