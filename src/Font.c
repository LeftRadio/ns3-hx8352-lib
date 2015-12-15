/*************************************************************************************
*
Project : HX8352 Graphics Library :: Fonts Layer sourse
Version : 
Date    : 
Author  : Left Radio                          
Comments: Вывод символов, строк и текста
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
* Description    : Выбор текущего шрифта для вывода
* Input          : const FONT_INFO *selectFont
* Return         : None
*******************************************************************************/
void LCD_SetFont(FONT_INFO *selectFont)
{
	font = selectFont;
}


/******************************************************************************
* Function Name  : LCD_DrawChar
* Description    : Вывод символа
* Input          : uint16_t XPos, uint16_t YPos, uint8_t trspr, const char c
* Return         : X1_Pos
*******************************************************************************/
uint8_t LCD_DrawChar(uint16_t XPos, uint16_t YPos, uint8_t trspr, char c)
{    
	uint8_t i, j, k; // переменные для циклов
	uint8_t CharLastBt = (font->HeightBytes * 8) - font->Height;
	uint16_t Y = YPos; // позиция по Y для пропуска точек
	uint8_t *ptr = (uint8_t*)CHAR_DATA(c); // указатель на данные символа
	uint8_t mask = 0x80;  // маска для расшифровки байтов данных символа, в двоичном формате = 0b10000000

	/* Если шрифт не определен или символ находится вне границ символов для шрифта то выход */
	if(!font) return 0;
	if(c < font->FirstChar || c > font->LastChar) return 0;


	Set_LCD_CS();

	/* выводим символ вертикальными линиями снизу-вверх */
	for(i = 0; i < CHAR_WIDTH(c); i++)
	{
		/* установка курсора LCD */
		LCD_SetCursor(XPos + i, YPos);
		Y = YPos; // позиция по Y для пропуска точек

		/* выводим все точки по вертикали */
		for(k = 0; k < font->HeightBytes; k++)
		{
			/* расшифровываем байты данных, один байт содержит информацию о 8-ми точках по вертикали */
			for(j = 0; j < 8; j++)
			{
				if(*ptr & mask) LCD_PutPixel(TextColor);   // если совпало с битом маски выводим точку цветом текста
				else if(trspr == 0)LCD_PutPixel(BackColor);  // выводим символ с не прозрачным фоном то ставим точку цветом фона
				else LCD_SetCursor(XPos + i, Y + 1);	      // иначе если выводим символ с прозрачным фоном пропускаем точку
				mask = mask>>1;                            // сдвигаем маску для проверки следущего бита
				Y++;
				if((k == font->HeightBytes - 1) && (j == 8 - CharLastBt))break;
			}
			mask=0x80;                                   // сбрасываем битовую маску
			ptr++;                                       // инкремент указателя на байт данных символа
		}
	}
	Reset_LCD_CS();

	/* возвращаем ширину символа */
	return CHAR_WIDTH(c);
}


/******************************************************************************
* Function Name  : LCD_PutStrig
* Description    : Вывод строки
* Input          : XPos, YPos, trspr, *str
* Return         : X1_Pos
*******************************************************************************/
uint16_t LCD_PutStrig(uint16_t XPos, uint16_t YPos, uint8_t trspr, char *str)
{ 
	uint8_t i,j;

	/* переменная для запоминания ширины последнего напечатоного символа */
	uint8_t lastPrintCharWidth = 0;

	/* если шрифт не определен то выход */
	if(!font || !str) return 0;

	/* печатаем пока есть символы в строке */
	while(*str!=0)
	{
		lastPrintCharWidth = LCD_DrawChar(XPos, YPos, trspr, *str);  // выводим сивол и запоминаем его ширину
		if(trspr == 0)
		{
			Set_LCD_CS();
			for(i = 0; i < CHAR_SPACE; i++)
			{
				LCD_SetCursor(XPos + lastPrintCharWidth + i, YPos);
				for(j = 0; j <= font->Height; j++)LCD_PutPixel(BackColor);  // ставим точку цветом фона
			}
			Reset_LCD_CS();
		}
		XPos += (lastPrintCharWidth + CHAR_SPACE);                   // смещаем курсор на ширину напечатонного символа + межсимвольное растояние
		str++;	                                                     // инкремент указателя на строку
	}
	return XPos;
}


/******************************************************************************
* Function Name  : LCD_PutStrig
* Description    : Вывод строки
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
* Description    : Вычисление длинны строки в пикселях
* Input          : XPos, *str
* Return         : X1_Pos - конечная координата
*******************************************************************************/
char *LCD_Strig_PointsWidht(uint16_t *XPos, char *str)
{
	/* если шрифт не определен то выход */
	if(!font || !str) return 0;

	/* суммируем пока есть символы в строке */
	while(*str!= 0)
	{
		*XPos = *XPos + (CHAR_WIDTH(*str) + CHAR_SPACE);
		str++;	// инкремент указателя на строку
		//if(*str == 0) *str = 0x20;
	}

	str--;
	return str;
}


/******************************************************************************
* Function Name  : LCD_PutStrig
* Description    : Вывод текста с автоматическим переносом по заданной границе
* Input          : XPos, YPos, Xend, trspr, *text
* Return         : X1_Pos
*******************************************************************************/
//uint16_t LCD_carryText(uint16_t XPos, uint16_t YPos, uint16_t Xend, uint8_t trspr, char *text)
//{
//  // переменная для запоминания конечной координаты последнего слова
//  uint8_t lastPrintStringPosition = 0;
//  uint16_t  widthWord = 0;
//  uint8_t numChar = 0;
//  uint16_t X0 = XPos;
//
//
//  // если шрифт не определен то выход
//  if(!font || !text) return 0;
//
//  // если граница переноса равна нулю или больше ширины LCD то установка ее на ширину LCD
//  if((Xend == 0) || (Xend > LCD_maxWidth)) Xend = LCD_maxWidth;
//
//
//
//  // выводим текст
//  while(1)
//  {
//    text++;			 //
//	widthWord = 0;	 // сбрасываем длину слова
//    numChar = 0;	 // сбрасываем количество символов в слове
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
