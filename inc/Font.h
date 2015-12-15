/*************************************************************************************
*
Project : HX8352 Graphics Library :: Fonts Layer header
Version : 
Date    : 
Author  : Left Radio                          
Comments: Вывод символов, строк и текста
*
**************************************************************************************/

#ifndef __FONT_H
#define __FONT_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>                           /* Include standard types */

/* Exported typedef -----------------------------------------------------------*/
//Структура доступа к массиву индексов символов
typedef struct FontTable {
	uint16_t	width;           // Ширина символа
	uint16_t	start;           // Стартовый индекс на первый байт символа в массиве данных символов
} FONT_CHAR_INFO;

//Структура для доступа к параметрам используемого шрифта
typedef struct
{
	uint8_t Height;		                // Высота символов в точках
	uint8_t HeightBytes;				// Высота символов в байтах
	uint8_t FirstChar;	                // Индекс первого символа
	uint8_t LastChar;		            // Индекс последнего символа
	uint8_t FontSpace;                  // Пробел между символами
	const FONT_CHAR_INFO *FontTable;	// Таблица индексов символов
	const uint8_t *FontBitmaps;         // Указатель на массив с данными о символах
} FONT_INFO;

/* Exported define -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported function --------------------------------------------------------*/
extern void LCD_SetFont(FONT_INFO *selectFont);		// Выбор текущего шрифта для вывода
extern uint8_t LCD_DrawChar(uint16_t XPos, uint16_t YPos, uint8_t trspr, char c);	// Вывод символа
extern uint16_t LCD_PutStrig(uint16_t XPos, uint16_t YPos, uint8_t trspr, char *str);		// Вывод строки
extern char* LCD_Strig_PointsWidht(uint16_t *XPos, char *str);	// Вычисление длинны строки в пикселях
//uint16_t  LCD_carryText(uint16_t XPos, uint16_t YPos, uint16_t Xend, uint8_t trspr, char *text);


#endif /* __FONT_H */
/*********************************************************************************************************
      END FILE
*********************************************************************************************************/

