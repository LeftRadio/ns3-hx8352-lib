/*************************************************************************************
*
Project : HX8352 Graphics Library :: Fonts Layer sourse
Version : 
Date    : 
Author  : Left Radio                          
Comments: ����� ��������, ����� � ������
*
**************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "hx8352.h"
#include "Font.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define CHAR_WIDTH(c)        font->FontTable[(c) - font->FirstChar].width
#define CHAR_START_INDEX(c)  font->FontTable[(c) - font->FirstChar].start
#define CHAR_DATA(c)         &font->FontBitmaps[CHAR_START_INDEX(c)]
#define CHAR_SPACE           font->FontSpace

/* Private variables ---------------------------------------------------------*/
FONT_INFO *font = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private function  --------------------------------------------------------*/

/******************************************************************************
* Function Name  : LCD_SetFont
* Description    : ����� �������� ������ ��� ������
* Input          : const FONT_INFO *selectFont
* Return         : None
*******************************************************************************/
void LCD_SetFont(FONT_INFO *selectFont)
{
	font = selectFont;
}


/******************************************************************************
* Function Name  : LCD_DrawChar
* Description    : ����� �������
* Input          : uint16_t XPos, uint16_t YPos, uint8_t trspr, const char c
* Return         : X1_Pos
*******************************************************************************/
uint8_t LCD_DrawChar(uint16_t XPos, uint16_t YPos, uint8_t trspr, char c)
{    
	uint8_t i, j, k; // ���������� ��� ������
	uint8_t CharLastBt = (font->HeightBytes * 8) - font->Height;
	uint16_t Y = YPos; // ������� �� Y ��� �������� �����
	uint8_t *ptr = (uint8_t*)CHAR_DATA(c); // ��������� �� ������ �������
	uint8_t mask = 0x80;  // ����� ��� ����������� ������ ������ �������, � �������� ������� = 0b10000000

	/* ���� ����� �� ��������� ��� ������ ��������� ��� ������ �������� ��� ������ �� ����� */
	if(!font) return 0;
	if(c < font->FirstChar || c > font->LastChar) return 0;


	Set_LCD_CS();

	/* ������� ������ ������������� ������� �����-����� */
	for(i = 0; i < CHAR_WIDTH(c); i++)
	{
		/* ��������� ������� LCD */
		LCD_SetCursor(XPos + i, YPos);
		Y = YPos; // ������� �� Y ��� �������� �����

		/* ������� ��� ����� �� ��������� */
		for(k = 0; k < font->HeightBytes; k++)
		{
			/* �������������� ����� ������, ���� ���� �������� ���������� � 8-�� ������ �� ��������� */
			for(j = 0; j < 8; j++)
			{
				if(*ptr & mask) LCD_PutPixel(TextColor);   // ���� ������� � ����� ����� ������� ����� ������ ������
				else if(trspr == 0)LCD_PutPixel(BackColor);  // ������� ������ � �� ���������� ����� �� ������ ����� ������ ����
				else LCD_SetCursor(XPos + i, Y + 1);	      // ����� ���� ������� ������ � ���������� ����� ���������� �����
				mask = mask>>1;                            // �������� ����� ��� �������� ��������� ����
				Y++;
				if((k == font->HeightBytes - 1) && (j == 8 - CharLastBt))break;
			}
			mask=0x80;                                   // ���������� ������� �����
			ptr++;                                       // ��������� ��������� �� ���� ������ �������
		}
	}
	Reset_LCD_CS();

	/* ���������� ������ ������� */
	return CHAR_WIDTH(c);
}


/******************************************************************************
* Function Name  : LCD_PutStrig
* Description    : ����� ������
* Input          : XPos, YPos, trspr, *str
* Return         : X1_Pos
*******************************************************************************/
uint16_t LCD_PutStrig(uint16_t XPos, uint16_t YPos, uint8_t trspr, char *str)
{ 
	uint8_t i,j;

	/* ���������� ��� ����������� ������ ���������� ������������ ������� */
	uint8_t lastPrintCharWidth = 0;

	/* ���� ����� �� ��������� �� ����� */
	if(!font || !str) return 0;

	/* �������� ���� ���� ������� � ������ */
	while(*str!=0)
	{
		lastPrintCharWidth = LCD_DrawChar(XPos, YPos, trspr, *str);  // ������� ����� � ���������� ��� ������
		if(trspr == 0)
		{
			Set_LCD_CS();
			for(i = 0; i < CHAR_SPACE; i++)
			{
				LCD_SetCursor(XPos + lastPrintCharWidth + i, YPos);
				for(j = 0; j <= font->Height; j++)LCD_PutPixel(BackColor);  // ������ ����� ������ ����
			}
			Reset_LCD_CS();
		}
		XPos += (lastPrintCharWidth + CHAR_SPACE);                   // ������� ������ �� ������ ������������� ������� + ������������� ���������
		str++;	                                                     // ��������� ��������� �� ������
	}
	return XPos;
}


/******************************************************************************
* Function Name  : LCD_PutStrig
* Description    : ����� ������
* Input          : XPos, YPos, trspr, *str
* Return         : X1_Pos
*******************************************************************************/
uint16_t LCD_PutColorStrig(uint16_t XPos, uint16_t YPos, uint8_t trspr, char *str, uint16_t Color)
{
	TextColor = Color;
	return LCD_PutStrig(XPos, YPos, trspr, str);
}


/******************************************************************************
* Function Name  : LCD_Strig_PointsWidht
* Description    : ���������� ������ ������ � ��������
* Input          : XPos, *str
* Return         : X1_Pos - �������� ����������
*******************************************************************************/
char *LCD_Strig_PointsWidht(uint16_t *XPos, char *str)
{
	/* ���� ����� �� ��������� �� ����� */
	if(!font || !str) return 0;

	/* ��������� ���� ���� ������� � ������ */
	while(*str!= 0)
	{
		*XPos = *XPos + (CHAR_WIDTH(*str) + CHAR_SPACE);
		str++;	// ��������� ��������� �� ������
		//if(*str == 0) *str = 0x20;
	}

	str--;
	return str;
}


/******************************************************************************
* Function Name  : LCD_PutStrig
* Description    : ����� ������ � �������������� ��������� �� �������� �������
* Input          : XPos, YPos, Xend, trspr, *text
* Return         : X1_Pos
*******************************************************************************/
//uint16_t LCD_carryText(uint16_t XPos, uint16_t YPos, uint16_t Xend, uint8_t trspr, char *text)
//{
//  // ���������� ��� ����������� �������� ���������� ���������� �����
//  uint8_t lastPrintStringPosition = 0;
//  uint16_t  widthWord = 0;
//  uint8_t numChar = 0;
//  uint16_t X0 = XPos;
//
//
//  // ���� ����� �� ��������� �� �����
//  if(!font || !text) return 0;
//
//  // ���� ������� �������� ����� ���� ��� ������ ������ LCD �� ��������� �� �� ������ LCD
//  if((Xend == 0) || (Xend > LCD_maxWidth)) Xend = LCD_maxWidth;
//
//
//
//  // ������� �����
//  while(1)
//  {
//    text++;			 //
//	widthWord = 0;	 // ���������� ����� �����
//    numChar = 0;	 // ���������� ���������� �������� � �����
//
//    while(*text != ' ')
//	{
//	   widthWord = widthWord + CHAR_WIDTH(*text);
//	   numChar++;
//	   text++;
//	}
//    *text = 0;
//
//	if( (lastPrintStringPosition + widthWord) > Xend )
//	{
//	  XPos =  X0 + CHAR_SPACE;
//	  lastPrintStringPosition = 0;
//
//	  if( YPos < font->Height  ) return (*text - numChar + 1);
//	  else YPos = YPos - ((font->Height) + 5);
//	}
//	else  XPos = lastPrintStringPosition + CHAR_SPACE;
//
//    lastPrintStringPosition = lastPrintStringPosition + LCD_PutStrig(XPos, YPos, trspr, text);	 //
//  }
//  //return 0;
//}
