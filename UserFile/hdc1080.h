#ifndef __HDC1080_H
#define __HDC1080_H

#include "mystm8sxxx.h"


/* This is library for temperature and humidity sensor HDC1080
 * Using msp430f5529 with driverlib
 *
 * Author 	:	Thiepnx
 * Version	:	1.0
 * Date		:	22/2/2017
 *
 */

/* Define slave address and internal register of hdc1080 */
#define         HDC_1080_ADD                            0x40
#define         Configuration_register_add              0x02
#define         Temperature_register_add                0x00
#define         Humidity_register_add                   0x01


/* Define temperature resolution */
typedef enum
{
  Temperature_Resolution_14_bit = 0,
  Temperature_Resolution_11_bit = 1
}Temp_Reso;


/* Define humidity resolution */
typedef enum
{
  Humidity_Resolution_14_bit = 0,
  Humidity_Resolution_11_bit = 1,
  Humidity_Resolution_8_bit =2
}Humi_Reso;


/* @Brief	: 	Initialize hdc1080 with configure temperature resolution and
 * 				humidity resolution
 *
 * @Para	:	+ Temperature_Resolution_x_bit - This is temperature resolution
 *
 * 				+ Humidity_Resolution_x_bit - humidity resolution
 *
 * @Return	:	+ 0 - If communicate with hdc1080 OK
 * 				+ !0 - If error occur
 *
 * @Note	:   User must be call this function one time before start measuring temperature and
 * 				humidity.
 */
uint8_t hdc1080_init(Temp_Reso Temperature_Resolution_x_bit,Humi_Reso Humidity_Resolution_x_bit);


/* @Brief	: 	Measuring temperature and humidity
 *
 * @Para	:	+ temperature - This is pointer to store temperature data
 *
 * 				+ humidity - This is pointer to store humidity data
 *
 * @Return	:	+ 0 - If communicate with hdc1080 OK
 * 				+ !0 - If error occur
 *
 * @Note	:   None
 */
uint8_t hdc1080_start_measurement(float* temperature, uint8_t* humidity);
#endif

