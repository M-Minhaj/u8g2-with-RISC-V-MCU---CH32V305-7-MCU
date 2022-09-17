/********************************** (C) COPYRIGHT *******************************

* File Name          : main.c
* Author             : Muhammad_Minhaj
* Version            : V1.0.0
* Date               : 2021/06/06
* Description        : Main program body.

*******************************************************************************/

/*

I2C CH_1
Pins
I2C1_SCL(PB8)¡¢
I2C1_SDA(PB9)¡£

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debug.h"
#include "u8g2.h"

/* I2C Mode Definition */
#define HOST_MODE     0
#define SLAVE_MODE    1

/* I2C Communication Mode Selection */
#define I2C_MODE      HOST_MODE
//#define I2C_MODE   SLAVE_MODE

/* Global define */
#define Size          7

#define OLED_ADDRESS     0x3C<<1


#define TX_TIMEOUT		100








void IIC_Init(u32 bound, u16 address)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    I2C_InitTypeDef  I2C_InitTSturcture = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_I2C1, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    I2C_InitTSturcture.I2C_ClockSpeed = bound;
    I2C_InitTSturcture.I2C_Mode = I2C_Mode_I2C;
    I2C_InitTSturcture.I2C_DutyCycle = I2C_DutyCycle_16_9;
    I2C_InitTSturcture.I2C_OwnAddress1 = address;
    I2C_InitTSturcture.I2C_Ack = I2C_Ack_Enable;
    I2C_InitTSturcture.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_Init(I2C1, &I2C_InitTSturcture);

    I2C_Cmd(I2C1, ENABLE);

#if(I2C_MODE == HOST_MODE)
    I2C_AcknowledgeConfig(I2C1, ENABLE);

#endif
}




uint8_t u8g2_gpio_and_delay_stm32(U8X8_UNUSED u8x8_t *u8x8, U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int, U8X8_UNUSED void *arg_ptr)
{

  switch(msg)
  {
    case U8X8_MSG_GPIO_AND_DELAY_INIT:	// called once during init phase of u8g2/u8x8

    	Delay_Init();
    	break;							// can be used to setup pins
    case U8X8_MSG_DELAY_100NANO:		// delay arg_int * 100 nano seconds
      for (uint16_t n = 0; arg_int*n < 20; n++)
          {
    	   __NOP();
    	  }
      break;
    case U8X8_MSG_DELAY_10MICRO:		// delay arg_int * 10 micro seconds
      for (uint16_t n = 0; n < arg_int*2000; n++)
    	  {
    	   __NOP();
    	  }


      break;
    case U8X8_MSG_DELAY_MILLI:			// delay arg_int * 1 milli second
    	Delay_Ms(arg_int);

      break;

    default:
      return 0;
    }

  return 1;
}


 


uint8_t u8x8_byte_wch32_hw_i2c(u8x8_t *u8g2, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
	// u8g2/u8x8 will never send more than 32 bytes between START_TRANSFER and END_TRANSFER
//	 add extra byte for the i2c address
	static uint8_t buffer[34];
	static uint8_t buf_idx;
	uint8_t *data;
	switch(msg)  {
		case U8X8_MSG_BYTE_SEND:
		    data = (uint8_t *)arg_ptr;
		    while( arg_int > 0 )
		    {
		    	buffer[buf_idx++] = *data;
		    	data++;
		    	arg_int--;
		    }
			break;
		case U8X8_MSG_BYTE_INIT:
			// add your custom code to init i2c subsystem
			break;
		case U8X8_MSG_BYTE_SET_DC:
		//	 ignored for i2c
			break;
		case U8X8_MSG_BYTE_START_TRANSFER:
			buf_idx = 0;
			break;
		case U8X8_MSG_BYTE_END_TRANSFER:
			I2C_Master_Transmit(I2C1,OLED_ADDRESS, (uint8_t *)buffer, buf_idx);
			break;
		default:
			return 0;
	}


	return 1;
}




u8g2_t u8g2;







int main(void)
{

 Delay_Init();
 USART_Printf_Init(115200);
 printf("SystemClk:%d\r\n", SystemCoreClock);
 #if(I2C_MODE == HOST_MODE)
 printf("IIC Host mode\r\n");
 u8g2_Setup_ssd1306_i2c_128x64_noname_f(&u8g2, U8G2_R0,u8x8_byte_wch32_hw_i2c,u8g2_gpio_and_delay_stm32);

 u8g2_InitDisplay(&u8g2); // send init sequence to the display, display is in sleep mode after this,
 u8g2_SetPowerSave(&u8g2,0);
 u8g2_ClearBuffer(&u8g2);
 u8g2_SetFont(&u8g2, u8g2_font_fub14_tf);

 u8g2_DrawStr(&u8g2,2,30,"Hello World");


 u8g2_SendBuffer(&u8g2);

#elif(I2C_MODE == SLAVE_MODE)
#endif

        while(1)
        {

           

        }



}




void I2C_Master_Transmit( I2C_TypeDef *I2Cx , uint16_t slave_add , uint8_t *buff, uint8_t idx )
{

 IIC_Init(100000, slave_add);

 u8 i = 0;

 while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY) != RESET);
 I2C_GenerateSTART(I2C1, ENABLE);
 while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
 I2C_Send7bitAddress(I2Cx, slave_add , I2C_Direction_Transmitter);
 while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

 while(idx > 0)
  {
   if(I2C_GetFlagStatus(I2Cx, I2C_FLAG_TXE) != RESET)
    {
     I2C_SendData(I2Cx, buff[i]);
     idx--;
     i++;
    }
  }

 while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
 I2C_GenerateSTOP(I2Cx, ENABLE);





}


