#include "stm8s.h"
#include "mystm8sxxx.h"
#include "hdc1080.h"

/* PE1	SCL
 * PE2	SDA
 *
 * CHIP STM8S207C8
 */

/* Declaring Global Variables */
volatile float temp;
volatile uint8_t humi;

/* Declaring Function Prototype */


void main(void)
{

	clk_config_16MHz_hse();
	delay_using_timer4_init();
	GPIO_Init(GPIOC,GPIO_PIN_1,GPIO_MODE_OUT_PP_HIGH_FAST);
	i2c_master_init();

	hdc1080_init(Temperature_Resolution_14_bit,Humidity_Resolution_14_bit);

	while (1)
	{
		if(hdc1080_start_measurement((float*)&temp,(uint8_t*)&humi))
		{
			GPIO_WriteReverse(GPIOC,GPIO_PIN_1);// Error indicator
		}
		delay_ms(500);
	}
}

/* User Function */




#ifdef USE_FULL_ASSERT

/**
 * @brief  Reports the name of the source file and the source line number
 *   where the assert_param error has occurred.
 * @param file: pointer to the source file name
 * @param line: assert_param error line source number
 * @retval : None
 */
void assert_failed(u8* file, u32 line)
{ 
	/* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while (1)
	{
	}
}
#endif


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
