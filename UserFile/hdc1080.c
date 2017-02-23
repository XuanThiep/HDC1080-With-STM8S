#include "hdc1080.h"


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
uint8_t hdc1080_init(Temp_Reso Temperature_Resolution_x_bit,Humi_Reso Humidity_Resolution_x_bit)
{
	/* Temperature and Humidity are acquired in sequence, Temperature first
	 * Default:   Temperature resolution = 14 bit,
	 *            Humidity resolution = 14 bit
	 */

	/* Set the acquisition mode to measure both temperature and humidity by setting Bit[12] to 1 */
	uint16_t config_reg_value=0x1000;
	uint8_t data_send[2];

	if(Temperature_Resolution_x_bit == Temperature_Resolution_11_bit)
	{
		config_reg_value |= (1<<10); //11 bit
	}

	switch(Humidity_Resolution_x_bit)
	{
	case Humidity_Resolution_11_bit:
		config_reg_value|= (1<<8);
		break;
	case Humidity_Resolution_8_bit:
		config_reg_value|= (1<<9);
		break;
	}

	data_send[0]= (config_reg_value>>8);
	data_send[1]= (config_reg_value&0x00ff);

	return i2c_mem_write_arr(HDC_1080_ADD<<1,Configuration_register_add,I2C_MEMORY_ADDRESS_SIZE_8BIT,data_send,2);
}

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
uint8_t hdc1080_start_measurement(float* temperature, uint8_t* humidity)
{
	uint8_t receive_data[4];
	uint16_t temp=0,humi=0;
	uint16_t time_out= 5000;

	/* If I2C bus is busy wait ultil it is free */
	while (I2C_GetFlagStatus(I2C_FLAG_BUSBUSY)&& (--time_out));
	if(!time_out) return 1;

	/* Send Start Condition then wait event EV5 */
	I2C_GenerateSTART(ENABLE);
	while ((!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT))&& (--time_out));
	if(!time_out) return 1;

	/* Send device address to write data then wait event EV6 */
	I2C_Send7bitAddress(HDC_1080_ADD<<1, I2C_DIRECTION_TX);
	while((!I2C_CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))&& (--time_out));
	if(!time_out) return 1;

	/* Send memory address pointer then wait event EV8_2 */
	I2C_SendData((uint8_t)Temperature_register_add);
	while((!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED))&& (--time_out));
	if(!time_out) return 1;

	I2C_GenerateSTOP(ENABLE);

	//Delay here ~14ms for conversion compelete
	delay_ms(14);

	/* Read temperature and humidity */

	/* Send Repeat Start Condition then wait event EV5 */
	I2C_GenerateSTART(ENABLE);
	while ((!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT))&& (--time_out));
	if(!time_out)	return 1;

	/* Send device address to read data then wait event EV6 */
	I2C_Send7bitAddress(HDC_1080_ADD<<1, I2C_DIRECTION_RX);
	while((!I2C_CheckEvent(I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))&& (--time_out));
	if(!time_out) return 1;

	/* Receive bytes from first byte until byte 0 */
	while ((I2C_GetFlagStatus(I2C_FLAG_TRANSFERFINISHED) == RESET)&& (--time_out));
	if(!time_out) return 1;

	/* Read a byte from the Slave */
	receive_data[0] = I2C_ReceiveData();

	while ((I2C_GetFlagStatus(I2C_FLAG_TRANSFERFINISHED) == RESET)&& (--time_out));
	if(!time_out) return 1;

	/* Clear ACK */
	I2C_AcknowledgeConfig(I2C_ACK_NONE);

	/* Disable general interrupts */
	disableInterrupts();

	/* Read byte 1 */
	receive_data[1] = I2C_ReceiveData();

	/* Program the STOP */
	I2C_GenerateSTOP(ENABLE);

	/* Read byte 2 */
	receive_data[2] = I2C_ReceiveData();

	/* Enable General interrupts */
	enableInterrupts();

	while ((I2C_GetFlagStatus(I2C_FLAG_RXNOTEMPTY) == RESET)&& (--time_out)); /* Poll on RxNE */
	if(!time_out) return 1;

	/* Read the last byte */
	receive_data[3] = I2C_ReceiveData();

	I2C_AcknowledgeConfig(I2C_ACK_CURR);


	temp=((receive_data[0]<<8)|receive_data[1]);
	humi=((receive_data[2]<<8)|receive_data[3]);

	*temperature=((temp/65536.0)*165.0)-40.0;
	*humidity=(uint8_t)((humi/65536.0)*100.0);

	return 0;
}
