/*************************************************************************************
*
Project : HX8352 Graphics Library :: Fonts Layer header
Version : 
Date    : 
Author  : Left Radio                          
Comments: ����� ��������, ����� � ������
*
**************************************************************************************/

#ifndef __FONT_H
#define __FONT_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>                           /* Include standard types */

/* Exported typedef -----------------------------------------------------------*/
//��������� ������� � ������� �������� ��������
typedef struct FontTable {
	uint16_t	width;           // ������ �������
	uint16_t	start;           // ��������� ������ �� ������ ���� ������� � ������� ������ ��������
} FONT_CHAR_INFO;

//��������� ��� ������� � ���������� ������������� ������
typedef struct
{
	uint8_t Height;		                // ������ �������� � ������
	uint8_t HeightBytes;				// ������ �������� � ������
	uint8_t FirstChar;	                // ������ ������� �������
	uint8_t LastChar;		            // ������ ���������� �������
	uint8_t FontSpace;                  // ������ ����� ���������
	const FONT_CHAR_INFO *FontTable;	// ������� �������� ��������
	const uint8_t *FontBitmaps;         // ��������� �� ������ � ������� � ��������
} FONT_INFO;

/* Exported define -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported function --------------------------------------------------------*/
extern void LCD_SetFont(FONT_INFO *selectFont);		// ����� �������� ������ ��� ������
extern uint8_t LCD_DrawChar(uint16_t XPos, uint16_t YPos, uint8_t trspr, char c);	// ����� �������
extern uint16_t LCD_PutStrig(uint16_t XPos, uint16_t YPos, uint8_t trspr, char *str);		// ����� ������
extern char* LCD_Strig_PointsWidht(uint16_t *XPos, char *str);	// ���������� ������ ������ � ��������
//uint16_t  LCD_carryText(uint16_t XPos, uint16_t YPos, uint16_t Xend, uint8_t trspr, char *text);


#endif /* __FONT_H */
/*********************************************************************************************************
      END FILE
*********************************************************************************************************/

