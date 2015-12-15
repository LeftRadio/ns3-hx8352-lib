/*************************************************************************************
*
Project : HX8352 Graphics Library :: Picture Layer sourse
Version : 
Date    : 
Author  : Left Radio                          
Comments: ����� �����������
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

/** ����� ����������� ������� 1bit ����/8bit ������ **/
void LCD_Draw_1_8_IMG(uint16_t X0pos, uint16_t Y0pos, const BITMAP_INFO_8 *IN_bitmap, uint8_t RLE)
{
  uint32_t i;  
  uint8_t *bitmap = IN_bitmap->ptrBitmap; 
  uint8_t mask = 0x80, j;
    
  /** ��������� ������� ������ LCD ������ �������� ����������� **/  
  LCD_SetArea(X0pos, X0pos + IN_bitmap->Width, Y0pos, Y0pos + IN_bitmap->Height);  
  
  
  /** ���� ����������� �����, �� �������� ��� �� ����������� � ���������, � ����� ����� �� ������� **/
  if(RLE == 1)
  {
	  LCD_DrawCompressed_1_8_IMG(IN_bitmap);
	  return;
  }
  

  CS_LCD_set;

  /** ��������� ��� ����� ������� ����������� **/   
  for (i = 0; i < (IN_bitmap->massiveSize - 1); i++ )
  {  
    /* �������������� ����� ������, ���� ���� �������� ���������� � 8-�� ������ �� ��������� */	
    for(j = 0; j < 8; j++)
    {
       if(*(bitmap + i) & mask) LCD_PutPixel(GraphicsColor);   // ���� ������� � ����� ����� ������� ����� ������ �������
       else LCD_PutPixel(BackColor);                          // ����� ������ ����� ������ ����
       mask = mask>>1;                                      // �������� ����� ��� �������� ��������� ����     
    }
    mask=0x80;    // ���������� ������� �����      
  }
  
  //restBits = pntNum - (IN_bitmap->massiveSize << 8);
  
//  /* �������������� ��������� ���� ������ */	
//  for(j = 0; j < restBits; j++)
//  {
//     if(*(bitmap + i) & mask) LCD_PutPixel(graf_color);   // ���� ������� � ����� ����� ������� ����� ������ �������
//     else LCD_PutPixel(BkColor);                 // ����� ������ ����� ������ ����
//     mask = mask>>1;                             // �������� ����� ��� �������� ��������� ����     
//  }  
  
  CS_LCD_clr;
  
  LCD_SetFullScreen();
}

/** ����� ������� ����������� ������� 1bit ����/8bit ������ **/
void LCD_DrawCompressed_1_8_IMG( const BITMAP_INFO_8 *IN_bitmap )
{
  uint32_t i;
  uint8_t j, k;  
  uint8_t *bitmap = IN_bitmap->ptrBitmap; 
  uint8_t mask = 0x80;  // ����� ��� ����������� ������ ������, � �������� ������� = 0b10000000 
    
  CS_LCD_set;
  
  /** ��������� ��� ����� ������� ����������� **/   
  for (i = 0; i < (IN_bitmap->massiveSize - 1); i++ )
  {  
    if ((*(bitmap + i) & 0x80) == 128)
    {
      for (j = 0; j < (*(bitmap + i) & 0x7F) + 1; j++)
      {        
		/* �������������� ����� ������, ���� ���� �������� ���������� � 8-�� ������ �� ��������� */	
        for(k = 0; k < 8; k++)
        {
           if(*(bitmap + i + 1) & mask) LCD_PutPixel(GraphicsColor);   // ���� ������� � ����� ����� ������� ����� ������ �������
           else LCD_PutPixel(BackColor);                          // ����� ������ ����� ������ ����
           mask = mask>>1;                                      // �������� ����� ��� �������� ��������� ����     
        }
        mask=0x80;		// ���������� ������� ����� 
      }
      i++;
    }
    else if ((*(bitmap + i) & 0x80) == 0)
    {
      for (j = 0; j < (*(bitmap + i) & 0x7F) + 1; j++)
      {
        /* �������������� ����� ������, ���� ���� �������� ���������� � 8-�� ������ �� ��������� */	
        for(k = 0; k < 8; k++)
        {
           if(*(bitmap + i + 1 + j) & mask) LCD_PutPixel(GraphicsColor);   // ���� ������� � ����� ����� ������� ����� ������ �������
           else LCD_PutPixel(BackColor);                          // ����� ������ ����� ������ ����
           mask = mask>>1;                                      // �������� ����� ��� �������� ��������� ����     
        } 
        mask=0x80;    // ���������� ������� �����   
      }
      i = i + (int)(*(bitmap + i) & 0x7F) + 1;
    }
  } 
  CS_LCD_clr;
  
  LCD_SetFullScreen();
}

/** ����� ����������� ������� 1bit ����/16bit ������ **/
void LCD_Draw_1_16_IMG(uint16_t X0pos, uint16_t Y0pos, const BITMAP_INFO_16 *IN_bitmap, uint8_t RLE)
{
  uint32_t i;  
  uint16_t *bitmap = IN_bitmap->ptrBitmap; 
  uint16_t mask = 0x8000;  // ����� ��� ����������� ������ ������, � �������� ������� = 0b10000000 
  uint8_t j;

  /** ��������� ������� ������ LCD ������ �������� ����������� **/  
  LCD_SetArea(X0pos, X0pos + IN_bitmap->Width, Y0pos, Y0pos + IN_bitmap->Height);   
  
  /** ���� ����������� �����, �� �������� ��� �� ����������� � ���������, � ����� ����� �� ������� **/
  if(RLE == 1)
  {
	  LCD_DrawCompressed_1_16_IMG(IN_bitmap);
	  return;
  }
  
  
  CS_LCD_set;

  /** ��������� ��� ����� ������� ����������� **/   
  for (i = 0; i < (IN_bitmap->massiveSize - 1); i++ )
  {  
    /* �������������� ����� ������, ���� ���� �������� ���������� � 16-�� ������ �� ��������� */	
    for(j = 0; j < 16; j++)
    {
       if(*(bitmap + i) & mask) LCD_PutPixel(GraphicsColor);   // ���� ������� � ����� ����� ������� ����� ������ �������
       else LCD_PutPixel(BackColor);                   // ����� ������ ����� ������ ����
       mask = mask>>1;                               // �������� ����� ��� �������� ��������� ����     
    }
    mask=0x8000;                                     // ���������� ������� �����	                                           
  }
  
  CS_LCD_clr;
  
  LCD_SetFullScreen();
}

/** ����� ������� ����������� ������� 1bit ����/8bit ������ **/
void LCD_DrawCompressed_1_16_IMG( const BITMAP_INFO_16 *IN_bitmap )
{
  uint32_t i;
  uint8_t j, k;  
  uint16_t *bitmap = IN_bitmap->ptrBitmap; 
  uint16_t mask = 0x8000;  // ����� ��� ����������� ������ ������, � �������� ������� = 0b10000000 
    
  
  CS_LCD_set;
  
  /** ��������� ��� ����� ������� ����������� **/   
  for (i = 0; i < (IN_bitmap->massiveSize - 1); i++ )
  {  
    if ((*(bitmap + i) & 0x8000) == 128)
    {
      for (j = 0; j < (*(bitmap + i) & 0x7F) + 1; j++)
      {        
		/* �������������� ����� ������, ���� ���� �������� ���������� � 8-�� ������ �� ��������� */	
        for(k = 0; k < 16; k++)
        {
           if(*(bitmap + i + 1) & mask) LCD_PutPixel(GraphicsColor);   // ���� ������� � ����� ����� ������� ����� ������ �������
           else LCD_PutPixel(BackColor);                          // ����� ������ ����� ������ ����
           mask = mask>>1;                                      // �������� ����� ��� �������� ��������� ����     
        }
        mask=0x80;		// ���������� ������� ����� 
      }
      i++;
    }
    else if ((*(bitmap + i) & 0x8000) == 0)
    {
      for (j = 0; j < (*(bitmap + i) & 0x7F) + 1; j++)
      {
        /* �������������� ����� ������, ���� ���� �������� ���������� � 8-�� ������ �� ��������� */	
        for(k = 0; k < 16; k++)
        {
           if(*(bitmap + i + 1 + j) & mask) LCD_PutPixel(GraphicsColor);   // ���� ������� � ����� ����� ������� ����� ������ �������
           else LCD_PutPixel(BackColor);                          // ����� ������ ����� ������ ����
           mask = mask>>1;                                      // �������� ����� ��� �������� ��������� ����     
        } 
        mask=0x8000;    // ���������� ������� �����   
      }
      i = i + (int)(*(bitmap + i) & 0x7F) + 1;
    }
  } 
  CS_LCD_clr;
  
  LCD_SetFullScreen();
}



// ========================================================================== 

/** ����� ����������� ������� 8bit ����/8bit ������ **/
void LCD_Draw_8_IMG(uint16_t X0pos, uint16_t Y0pos, const BITMAP_INFO_8 *IN_bitmap, uint8_t RLE)
{
  uint32_t i;
  uint16_t _16bppColor = 0;   
  uint8_t *bitmap = IN_bitmap->ptrBitmap; 
      
  /** ��������� ������� ������ LCD ������ �������� ����������� **/  
  LCD_SetArea(X0pos, X0pos + IN_bitmap->Width, Y0pos, Y0pos + IN_bitmap->Height); 

  /** ���� ����������� �����, �� �������� ��� �� ����������� � ���������, � ����� ����� �� ������� **/
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

/** ����� ������� ����������� ������� 8bit ����/8bit ������ **/
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

/** ����� ����������� ������� 16bit ����/16bit ������ **/
void LCD_Draw_16_IMG(uint16_t X0pos, uint16_t Y0pos, const BITMAP_INFO_16 *IN_bitmap, uint8_t RLE)
{
  uint32_t i;  
  uint16_t *bitmap = IN_bitmap->ptrBitmap; 
      
  /** ��������� ������� ������ LCD ������ �������� ����������� **/  
  LCD_SetArea(X0pos, X0pos + IN_bitmap->Width, Y0pos, Y0pos + IN_bitmap->Height); 
  
  /** ���� ����������� �����, �� �������� ��� �� ����������� � ���������, � ����� ����� �� ������� **/
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

/** ����� ������� ����������� ������� 16bit ����/16bit ������ **/
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












