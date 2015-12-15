/*************************************************************************************
*
Project : HX8352 Graphics Library :: GUI Elements header
Version : 
Date    : 
Author  : Left Radio                          
Comments: 
*
**************************************************************************************/

#ifndef __GUI_ELEMENTS_H
#define __GUI_ELEMENTS_H 

/* Includes ------------------------------------------------------------------*/	   
#include "hx8352.h"

/* Exported typedef -----------------------------------------------------------*/

/** ����������� ��� ������ ���������� ������� **/
typedef struct 
{
    const uint16_t Left;	             // ����� ������� ������
	const uint16_t Lower;                // ������ ������� ������
    const uint16_t Width;		         // ������
	const uint16_t Height;		         // ������ 
	uint16_t Color;			     		 // ����
    uint16_t Active_Color;	             // ���� ���� ����� ������ �������
    uint16_t Active_FontColor;		     // ���� ������ ����� ������ �������
	char*    Text;  			         // �����
	void (*btnEvent_func)(void);		 // ��������� �� ������� ����������	�������
} btnINFO;

/** ����������� ��� ������, ����� **/
typedef struct 
{
    uint16_t centerX;						// 
	uint16_t centerY;						// 
    uint16_t Width;							// 
	uint16_t Height;						//  
	uint16_t Color;							// 
    FunctionalState  EnableCentralLines;	// 
} GridType;


/* Exported define -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported function --------------------------------------------------------*/

/* Draw Button/���������� ������ */
void LCD_DrawButton(btnINFO *btn, uint8_t active);

/* Draw "SIN"/���������� ����� � ��������� ��������� � �������� */
void LCD_DrawSIN(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, float periods, uint16_t Color);

#endif /* __GUI_ELEMENTS_H */
/*********************************************************************************************************
      END FILE
*********************************************************************************************************/

