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

/** Структурный тип данных параметров кннопок **/
typedef struct 
{
    const uint16_t Left;	             // Левая граница кнопки
	const uint16_t Lower;                // Нижняя граница кнопки
    const uint16_t Width;		         // Ширина
	const uint16_t Height;		         // Высота 
	uint16_t Color;			     		 // Цвет
    uint16_t Active_Color;	             // Цвет фона когда кнопка активна
    uint16_t Active_FontColor;		     // Цвет шрифта когда кнопка активна
	char*    Text;  			         // Текст
	void (*btnEvent_func)(void);		 // указатель на функцию обработчик	события
} btnINFO;

/** Структурный тип данных, сетка **/
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

/* Draw Button/Нарисовать кнопку */
void LCD_DrawButton(btnINFO *btn, uint8_t active);

/* Draw "SIN"/Нарисовать синус с заданными размерами и периодом */
void LCD_DrawSIN(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, float periods, uint16_t Color);

#endif /* __GUI_ELEMENTS_H */
/*********************************************************************************************************
      END FILE
*********************************************************************************************************/

