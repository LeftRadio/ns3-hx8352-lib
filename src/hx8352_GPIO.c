/*************************************************************************************
*
Description :  LCD GPIO header
Version     :  1.0.0
Date        :  09.05.2011
Author      :  Left Radio                          
Comments:  
*
**************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "hx8352_GPIO.h"



/** ������� ����� ��� ����� ���� ������ LCD **/
uint16_t LCD_portMask = 0x00FF;

/** ��������� ���������� ��� ����������� �������� LCD ���� LCD_DataPort_TypeDef **/
LCD_Pin_TypeDef RES_LCD;
LCD_Pin_TypeDef RD_LCD;
LCD_Pin_TypeDef RS_LCD;
LCD_Pin_TypeDef WR_LCD;
LCD_Pin_TypeDef CS_LCD;
LCD_Pin_TypeDef HC573_LE;

/** ��������� ���������� ��� ���� ������ LCD ���� LCD_DataPort_TypeDef **/
LCD_DataPort_TypeDef LCD_DataPort;



/** ��������� ����� ��� 8-�� ������ ����� ������, GPIO_Pins ������ ��������� �������� 0x00FF ��� 0xFF00,
    ������������� ������� ��� ������� 8 ��� ����� **/
void set_LCD_DATAPort_Init(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pins)
{  
	LCD_DataPort.GPIOx = GPIOx;
	LCD_portMask = GPIO_Pins;  
}



/** ��������� ������� �� ��� ����������� ������� LCD **/
void set_RES_LCD(GPIO_TypeDef* GPIOx, uint16_t Pin)
{
	RES_LCD.GPIOx = GPIOx;
	RES_LCD.Pin = Pin;
}
void set_RD_LCD(GPIO_TypeDef* GPIOx, uint16_t Pin)
{
	RD_LCD.GPIOx = GPIOx;
	RD_LCD.Pin = Pin;
}
void set_RS_LCD(GPIO_TypeDef* GPIOx, uint16_t Pin)
{
	RS_LCD.GPIOx = GPIOx;
	RS_LCD.Pin = Pin;
}
void set_WR_LCD(GPIO_TypeDef* GPIOx, uint16_t Pin)
{
	WR_LCD.GPIOx = GPIOx;
	WR_LCD.Pin = Pin;
}
void set_CS_LCD(GPIO_TypeDef* GPIOx, uint16_t Pin)
{
	CS_LCD.GPIOx = GPIOx;
	CS_LCD.Pin = Pin;
}

void set_HC573_LE_LCD(GPIO_TypeDef* GPIOx, uint16_t Pin)
{
	HC573_LE.GPIOx = GPIOx;
	HC573_LE.Pin = Pin;
}
