/*************************************************************************************
*
Project : HX8352 Graphics Library :: Picture Layer sourse
Version : 
Date    : 
Author  : Left Radio                          
Comments: Вывод изображений
*
**************************************************************************************/

#include "hx8352.h"
#include "Picture.h"
#include "math.h"

void LCD_DrawCompressed_1_8_IMG( const BITMAP_INFO_8 *IN_bitmap );
void LCD_DrawCompressed_1_16_IMG( const BITMAP_INFO_16 *IN_bitmap );

void LCD_DrawCompressed_8_IMG(const BITMAP_INFO_8 *IN_bitmap);
void LCD_DrawCompressed_16_IMG(const BITMAP_INFO_16 *IN_bitmap);


// ==========================================================================

/** Вывод изображения формата 1bit цвет/8bit данные **/
void LCD_Draw_1_8_IMG(uint16_t X0pos, uint16_t Y0pos, const BITMAP_INFO_8 *IN_bitmap, uint8_t RLE)
{
  uint32_t i;  
  uint8_t *bitmap = IN_bitmap->ptrBitmap; 
  uint8_t mask = 0x80, j;
    
  /** Установка области вывода LCD равной размерам изображению **/  
  LCD_SetArea(X0pos, X0pos + IN_bitmap->Width, Y0pos, Y0pos + IN_bitmap->Height);  
  
  
  /** Если изображение сжато, то передаем его на расшифровку и отрисовку, и затем выход из функции **/
  if(RLE == 1)
  {
	  LCD_DrawCompressed_1_8_IMG(IN_bitmap);
	  return;
  }
  

  CS_LCD_set;

  /** Пробегаем все байты массива изображения **/   
  for (i = 0; i < (IN_bitmap->massiveSize - 1); i++ )
  {  
    /* расшифровываем байты данных, один байт содержит информацию о 8-ми точках по вертикали */	
    for(j = 0; j < 8; j++)
    {
       if(*(bitmap + i) & mask) LCD_PutPixel(GraphicsColor);   // если совпало с битом маски выводим точку цветом графики
       else LCD_PutPixel(BackColor);                          // иначе ставим точку цветом фона
       mask = mask>>1;                                      // сдвигаем маску для проверки следущего бита     
    }
    mask=0x80;    // сбрасываем битовую маску      
  }
  
  //restBits = pntNum - (IN_bitmap->massiveSize << 8);
  
//  /* расшифровываем последний байт данных */	
//  for(j = 0; j < restBits; j++)
//  {
//     if(*(bitmap + i) & mask) LCD_PutPixel(graf_color);   // если совпало с битом маски выводим точку цветом графики
//     else LCD_PutPixel(BkColor);                 // иначе ставим точку цветом фона
//     mask = mask>>1;                             // сдвигаем маску для проверки следущего бита     
//  }  
  
  CS_LCD_clr;
  
  LCD_SetFullScreen();
}

/** Вывод сжатого изображения формата 1bit цвет/8bit данные **/
void LCD_DrawCompressed_1_8_IMG( const BITMAP_INFO_8 *IN_bitmap )
{
  uint32_t i;
  uint8_t j, k;  
  uint8_t *bitmap = IN_bitmap->ptrBitmap; 
  uint8_t mask = 0x80;  // маска для расшифровки байтов данных, в двоичном формате = 0b10000000 
    
  CS_LCD_set;
  
  /** Пробегаем все байты массива изображения **/   
  for (i = 0; i < (IN_bitmap->massiveSize - 1); i++ )
  {  
    if ((*(bitmap + i) & 0x80) == 128)
    {
      for (j = 0; j < (*(bitmap + i) & 0x7F) + 1; j++)
      {        
		/* расшифровываем байты данных, один байт содержит информацию о 8-ми точках по вертикали */	
        for(k = 0; k < 8; k++)
        {
           if(*(bitmap + i + 1) & mask) LCD_PutPixel(GraphicsColor);   // если совпало с битом маски выводим точку цветом графики
           else LCD_PutPixel(BackColor);                          // иначе ставим точку цветом фона
           mask = mask>>1;                                      // сдвигаем маску для проверки следущего бита     
        }
        mask=0x80;		// сбрасываем битовую маску 
      }
      i++;
    }
    else if ((*(bitmap + i) & 0x80) == 0)
    {
      for (j = 0; j < (*(bitmap + i) & 0x7F) + 1; j++)
      {
        /* расшифровываем байты данных, один байт содержит информацию о 8-ми точках по вертикали */	
        for(k = 0; k < 8; k++)
        {
           if(*(bitmap + i + 1 + j) & mask) LCD_PutPixel(GraphicsColor);   // если совпало с битом маски выводим точку цветом графики
           else LCD_PutPixel(BackColor);                          // иначе ставим точку цветом фона
           mask = mask>>1;                                      // сдвигаем маску для проверки следущего бита     
        } 
        mask=0x80;    // сбрасываем битовую маску   
      }
      i = i + (int)(*(bitmap + i) & 0x7F) + 1;
    }
  } 
  CS_LCD_clr;
  
  LCD_SetFullScreen();
}

/** Вывод изображения формата 1bit цвет/16bit данные **/
void LCD_Draw_1_16_IMG(uint16_t X0pos, uint16_t Y0pos, const BITMAP_INFO_16 *IN_bitmap, uint8_t RLE)
{
  uint32_t i;  
  uint16_t *bitmap = IN_bitmap->ptrBitmap; 
  uint16_t mask = 0x8000;  // маска для расшифровки байтов данных, в двоичном формате = 0b10000000 
  uint8_t j;

  /** Установка области вывода LCD равной размерам изображению **/  
  LCD_SetArea(X0pos, X0pos + IN_bitmap->Width, Y0pos, Y0pos + IN_bitmap->Height);   
  
  /** Если изображение сжато, то передаем его на расшифровку и отрисовку, и затем выход из функции **/
  if(RLE == 1)
  {
	  LCD_DrawCompressed_1_16_IMG(IN_bitmap);
	  return;
  }
  
  
  CS_LCD_set;

  /** Пробегаем все байты массива изображения **/   
  for (i = 0; i < (IN_bitmap->massiveSize - 1); i++ )
  {  
    /* расшифровываем байты данных, один байт содержит информацию о 16-ти точках по вертикали */	
    for(j = 0; j < 16; j++)
    {
       if(*(bitmap + i) & mask) LCD_PutPixel(GraphicsColor);   // если совпало с битом маски выводим точку цветом графики
       else LCD_PutPixel(BackColor);                   // иначе ставим точку цветом фона
       mask = mask>>1;                               // сдвигаем маску для проверки следущего бита     
    }
    mask=0x8000;                                     // сбрасываем битовую маску	                                           
  }
  
  CS_LCD_clr;
  
  LCD_SetFullScreen();
}

/** Вывод сжатого изображения формата 1bit цвет/8bit данные **/
void LCD_DrawCompressed_1_16_IMG( const BITMAP_INFO_16 *IN_bitmap )
{
  uint32_t i;
  uint8_t j, k;  
  uint16_t *bitmap = IN_bitmap->ptrBitmap; 
  uint16_t mask = 0x8000;  // маска для расшифровки байтов данных, в двоичном формате = 0b10000000 
    
  
  CS_LCD_set;
  
  /** Пробегаем все байты массива изображения **/   
  for (i = 0; i < (IN_bitmap->massiveSize - 1); i++ )
  {  
    if ((*(bitmap + i) & 0x8000) == 128)
    {
      for (j = 0; j < (*(bitmap + i) & 0x7F) + 1; j++)
      {        
		/* расшифровываем байты данных, один байт содержит информацию о 8-ми точках по вертикали */	
        for(k = 0; k < 16; k++)
        {
           if(*(bitmap + i + 1) & mask) LCD_PutPixel(GraphicsColor);   // если совпало с битом маски выводим точку цветом графики
           else LCD_PutPixel(BackColor);                          // иначе ставим точку цветом фона
           mask = mask>>1;                                      // сдвигаем маску для проверки следущего бита     
        }
        mask=0x80;		// сбрасываем битовую маску 
      }
      i++;
    }
    else if ((*(bitmap + i) & 0x8000) == 0)
    {
      for (j = 0; j < (*(bitmap + i) & 0x7F) + 1; j++)
      {
        /* расшифровываем байты данных, один байт содержит информацию о 8-ми точках по вертикали */	
        for(k = 0; k < 16; k++)
        {
           if(*(bitmap + i + 1 + j) & mask) LCD_PutPixel(GraphicsColor);   // если совпало с битом маски выводим точку цветом графики
           else LCD_PutPixel(BackColor);                          // иначе ставим точку цветом фона
           mask = mask>>1;                                      // сдвигаем маску для проверки следущего бита     
        } 
        mask=0x8000;    // сбрасываем битовую маску   
      }
      i = i + (int)(*(bitmap + i) & 0x7F) + 1;
    }
  } 
  CS_LCD_clr;
  
  LCD_SetFullScreen();
}



// ========================================================================== 

/** Вывод изображения формата 8bit цвет/8bit данные **/
void LCD_Draw_8_IMG(uint16_t X0pos, uint16_t Y0pos, const BITMAP_INFO_8 *IN_bitmap, uint8_t RLE)
{
  uint32_t i;
  uint16_t _16bppColor = 0;   
  uint8_t *bitmap = IN_bitmap->ptrBitmap; 
      
  /** Установка области вывода LCD равной размерам изображению **/  
  LCD_SetArea(X0pos, X0pos + IN_bitmap->Width, Y0pos, Y0pos + IN_bitmap->Height); 

  /** Если изображение сжато, то передаем его на расшифровку и отрисовку, и затем выход из функции **/
  if(RLE == 1)
  {
	  LCD_DrawCompressed_8_IMG(IN_bitmap);
	  return;
  }

  CS_LCD_set;  
  for (i = 0; i < IN_bitmap->massiveSize; i++ )
  {    
      _16bppColor = (((uint16_t)*(bitmap + i) & 0xE0) << 8) + (((uint16_t)*(bitmap + i) & 0x1C) << 6) + (((uint16_t)*(bitmap + i) & 0x03) << 3);
      
	  LCD_PutPixel( _16bppColor);
      
      i++;    
  }
  CS_LCD_clr;
  
  LCD_SetFullScreen();
}

/** Вывод сжатого изображения формата 8bit цвет/8bit данные **/
void LCD_DrawCompressed_8_IMG(const BITMAP_INFO_8 *IN_bitmap)
{
  uint32_t i;
  uint8_t j;
  uint16_t _16bppColor = 0;
  uint8_t *bitmap = IN_bitmap->ptrBitmap; 
  
  
  CS_LCD_set;  
  for (i = 0; i < IN_bitmap->massiveSize; i++ )
  {
    if ((*(bitmap + i) & 0x80) == 128)
    {
      for (j = 0; j < (*(bitmap + i) & 0x7F) + 1; j++)
      {
        _16bppColor = (((uint16_t)*(bitmap + i + 1) & 0xE0) << 8) + (((uint16_t)*(bitmap + i + 1) & 0x1C) << 6) + (((uint16_t)*(bitmap + i + 1) & 0x03) << 3);
        LCD_PutPixel( _16bppColor);
      }
      i++;
    }
    else if ((*(bitmap + i) & 0x80) == 0)
    {
      for (j = 0; j < (*(bitmap + i) & 0x7F) + 1; j++)
      {
        _16bppColor = (((uint16_t)*(bitmap + i + 1 + j) & 0xE0) << 8) + (((uint16_t)*(bitmap + i + 1 + j) & 0x1C) << 6) + (((uint16_t)*(bitmap + i + 1 + j) & 0x03) << 3);
        LCD_PutPixel(_16bppColor);
      }
      i = i + (int)(*(bitmap + i) & 0x7F) + 1;
    }
  }
  CS_LCD_clr;  
  LCD_SetFullScreen();
}

 

// ==========================================================================

/** Вывод изображения формата 16bit цвет/16bit данные **/
void LCD_Draw_16_IMG(uint16_t X0pos, uint16_t Y0pos, const BITMAP_INFO_16 *IN_bitmap, uint8_t RLE)
{
  uint32_t i;  
  uint16_t *bitmap = IN_bitmap->ptrBitmap; 
      
  /** Установка области вывода LCD равной размерам изображению **/  
  LCD_SetArea(X0pos, X0pos + IN_bitmap->Width, Y0pos, Y0pos + IN_bitmap->Height); 
  
  /** Если изображение сжато, то передаем его на расшифровку и отрисовку, и затем выход из функции **/
  if(RLE == 1)
  {
	  LCD_DrawCompressed_16_IMG(IN_bitmap);
	  return;
  }

  CS_LCD_set;  
  for (i = 0; i < IN_bitmap->massiveSize; i++ )
  {   
	  LCD_PutPixel(*(bitmap + i));      
      i++;    
  }
  CS_LCD_clr;
  
  LCD_SetFullScreen();
}

/** Вывод сжатого изображения формата 16bit цвет/16bit данные **/
void LCD_DrawCompressed_16_IMG(const BITMAP_INFO_16 *IN_bitmap)
{  
  uint32_t i;
  uint8_t j;	  
  uint16_t *bitmap = IN_bitmap->ptrBitmap; 
      
  
  CS_LCD_set;  
  for (i = 0; i < IN_bitmap->massiveSize; i++ )
  {
    if ((*(bitmap + i) & 0x80) == 128)
    {
      for (j = 0; j < (*(bitmap + i) & 0x7F) + 1; j++) LCD_PutPixel(*(bitmap + i + 1));
      i++;
    }
    else if ((*(bitmap + i) & 0x80) == 0)
    {
      for (j = 0; j < (*(bitmap + i) & 0x7F) + 1; j++) LCD_PutPixel(*(bitmap + i + 1 + j));
      i = i + (int)(*(bitmap + i) & 0x7F) + 1;
    }
  }
  CS_LCD_clr; 
  
  LCD_SetFullScreen();
}












