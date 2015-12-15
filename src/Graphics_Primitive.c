/*************************************************************************************
*
Project : HX8352 Graphics Library :: Primitive Layer sourse
Version : 
Date    : 
Author  : Left Radio                          
Comments: 
*
**************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "hx8352.h"
#include "Graphics_Primitive.h"
#include "math.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define ON						1
#define OFF						0

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile uint8_t Antialiasing = OFF;

volatile uint16_t NumLevels = 256;
volatile uint16_t IntensityBits = 8;

/* Exported variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/ 





/* Private function  --------------------------------------------------------*/

/******************************************************************************
* Function Name  : DrawPixel
* Description    : Нарисовать точку по указанным координатам
* Input          : Xpos, Ypos, Color
* Return         : None
*******************************************************************************/
void DrawPixel(uint16_t Xpos, uint16_t Ypos, uint16_t Color)
{  
	Set_LCD_CS();
	LCD_SetCursor(Xpos,Ypos);			// Установка курсора в нужную позицию 
	LCD_PutPixel(Color);		// Нарисовать точку на установленной позиции
	Reset_LCD_CS();
}


/******************************************************************************
* Function Name  : AntialiasingState
* Description    : включение/выключение сглаживания линий
* Input          : NewState
* Return         : None
*******************************************************************************/
void SetAntialiasingState(uint8_t NewState)
{
	if((NewState != 0) && (NewState != 1)) return;
	Antialiasing = NewState;
}


/******************************************************************************
* Function Name  : SetAntialiasingParam
* Description    : 
* Input          : Arguments:
					    +  NumLevels is number of color levels. Pass 256.
					    +  IntensityBits denotes bits used to represent color component. Pass 8.
* Return         : None
*******************************************************************************/
void SetAntialiasingParam(uint16_t numLevels, uint16_t intensityBits)
{
	NumLevels = numLevels;
	IntensityBits = intensityBits;
}


/******************************************************************************
* Function Name  : DrawLine
* Description    : нарисовать линию
* Input          : Arguments:
					    +  (X0, Y0) is start point of line.
					    +  (X1, Y1) is end point of line.
					    +  BaseColor is intensity of line. Pass 0 for black line.					    
* Return         : None
*******************************************************************************/
void LCD_DrawLine(uint16_t X0pos, uint16_t Y0pos, uint16_t X1pos, uint16_t Y1pos)
{
	unsigned short IntensityShift;
	short DeltaX = X1pos - X0pos; 
	short DeltaY = Y1pos - Y0pos;
	unsigned short ErrorAdj, ErrorAcc;
	unsigned short ErrorAccTemp, Weighting, WeightingComplementMask;
	short XDir, YDir, error2;
	
	Set_LCD_CS();	
	LCD_SetCursor(X0pos, Y0pos);
	
	if (DeltaX >= 0) XDir = 1;
	else { XDir = -1; DeltaX = -DeltaX; /* make DeltaX positive */	}
	
	if (DeltaY >= 0) YDir = 1;
	else{ YDir = -1; DeltaY = -DeltaY; /* make DeltaX positive */	}

//	if((DeltaX == 0) && (DeltaY == 0))
//	{
//		LCD_SetCursor(X0pos, Y0pos);
//		LCD_PutPixel(GraphicsColor);
//		return;
//	}

	/* Special-case horizontal, vertical, and diagonal lines, which
	require no weighting because they go right through the center of
	every pixel */
	if (DeltaY == 0)
	{
		/* Horizontal line */
		do
		{
			LCD_PutPixel(GraphicsColor);
		
			X0pos += XDir;		
			LCD_SetCursor(X0pos, Y0pos);
		}
		while(DeltaX-- != 0);
		Reset_LCD_CS();
		return;
	}
	else if (DeltaX == 0)
	{
		/* Vertical line */
		do
		{
			LCD_PutPixel(GraphicsColor);
		}
		while(--DeltaY != 0);	  
		Reset_LCD_CS();
		return;
	}
	else if (DeltaX == DeltaY)
	{
		/* Diagonal line */
		do
		{
			LCD_PutPixel(GraphicsColor);
		
			X0pos += XDir;
			Y0pos++;
			LCD_SetCursor(X0pos, Y0pos);
		}
		while (--DeltaY != 0);
		Reset_LCD_CS();
		return;
	}
	
	/* ---- Line is not horizontal, diagonal, or vertical */
	
	if(Antialiasing == OFF)
	{
	    ErrorAcc = DeltaX - DeltaY;

		for (;;)
	    {
		    LCD_SetCursor(X0pos, Y0pos);
	        LCD_PutPixel(GraphicsColor);
	        
	        if(X0pos == X1pos && Y0pos == Y1pos)break;
	 
	        error2 = ErrorAcc * 2;
	 
	        if(error2 > -DeltaY)
	        {
	            ErrorAcc -= DeltaY;
	            X0pos += XDir;
	        }
	 
	        if(error2 < DeltaX)
	        {
	            ErrorAcc += DeltaX;
	            Y0pos += YDir;
	        }
	    }
		Reset_LCD_CS();
		return;
	}


	ErrorAcc = 0;  /* initialize the line error accumulator to 0 */
	
	/* # of bits by which to shift ErrorAcc to get intensity level */
	IntensityShift = 16 - IntensityBits;
	
	/* Mask used to flip all bits in an intensity weighting, producing the
	result (1 - intensity weighting) */
	WeightingComplementMask = NumLevels - 1;
	
	/* Is this an X-major or Y-major line? */
	if (DeltaY > DeltaX)
	{
		/* Y-major line; calculate 16-bit fixed-point fractional part of a
		pixel that X advances each time Y advances 1 pixel, truncating the
		result so that we won't overrun the endpoint along the X axis */
		ErrorAdj = ((unsigned long) DeltaX << 16) / (unsigned long) DeltaY;
		
		/* Draw all pixels other than the first and last */
		while (--DeltaY)
		{
			ErrorAccTemp = ErrorAcc;   /* remember currrent accumulated error */
			ErrorAcc += ErrorAdj;      /* calculate error for next pixel */
			if (ErrorAcc <= ErrorAccTemp)
			{
				/* The error accumulator turned over, so advance the X coord */
				X0pos += XDir;
			}
			
			Y0pos += YDir; /* Y-major, so always advance Y */
			
			/* The IntensityBits most significant bits of ErrorAcc give us the
			intensity weighting for this pixel, and the complement of the
			weighting for the paired pixel */
			Weighting = ErrorAcc >> IntensityShift;
			DrawPixel(X0pos, Y0pos, GraphicsColor - Weighting);
			DrawPixel(X0pos + XDir, Y0pos, GraphicsColor - (Weighting ^ WeightingComplementMask));
		}
		
		/* Draw the final pixel, which is 
		always exactly intersected by the line
		and so needs no weighting */
		DrawPixel(X1pos, Y1pos, GraphicsColor);
		
		Reset_LCD_CS();
		return;
	}
   

	/* It's an X-major line; calculate 16-bit fixed-point fractional part of a
	pixel that Y advances each time X advances 1 pixel, truncating the
	result to avoid overrunning the endpoint along the X axis */
	ErrorAdj = ((unsigned long) DeltaY << 16) / (unsigned long) DeltaX;
	
	/* Draw all pixels other than the first and last */
	while (--DeltaX)
	{
		ErrorAccTemp = ErrorAcc;   /* remember currrent accumulated error */
		ErrorAcc += ErrorAdj;      /* calculate error for next pixel */
		
		if (ErrorAcc <= ErrorAccTemp)
		{
			/* The error accumulator turned over, so advance the Y coord */
			Y0pos += YDir;
		}

		X0pos += XDir; /* X-major, so always advance X */
		
		/* The IntensityBits most significant bits of ErrorAcc give us the
		intensity weighting for this pixel, and the complement of the
		weighting for the paired pixel */
		Weighting = ErrorAcc >> IntensityShift;
		DrawPixel(X0pos, Y0pos, GraphicsColor + Weighting);
		DrawPixel(X0pos, Y0pos + 1, GraphicsColor + (Weighting ^ WeightingComplementMask));
	}
	
	/* Draw the final pixel, which is always exactly intersected by the line
	and so needs no weighting */
	DrawPixel(X1pos, Y1pos, GraphicsColor);

	Reset_LCD_CS();
}



/******************************************************************************
* Function Name  : DrawPixel
* Description    : Нарисовать прямоугольник
* Input          : X0, Y0, X1, Y1
* Return         : None
*******************************************************************************/
void LCD_DrawRect(uint16_t X0pos, uint16_t Y0pos, uint16_t X1pos, uint16_t Y1pos)
{
	LCD_DrawLine(X0pos, Y0pos, X0pos, Y1pos);			// рисуем левое ребро
	LCD_DrawLine(X1pos, Y0pos, X1pos, Y1pos);			// рисуем правое ребро
	LCD_DrawLine(X0pos, Y0pos, X1pos, Y0pos);			// рисуем нижнее ребро
	LCD_DrawLine(X0pos, Y1pos, X1pos, Y1pos);			// рисуем вверхнее ребро
}


/******************************************************************************
* Function Name  : LCD_DrawFillRect
* Description    : Нарисовать закрашенный прямоугольник
* Input          : X0, Y0, X1, Y1, border, borderColor
* Return         : None
*******************************************************************************/
void LCD_DrawFillRect(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, uint8_t border, uint16_t borderColor)
{
  uint16_t tmpColor = GraphicsColor;  // сохраняем текущий цвет для графики
    
  LCD_ClearArea(X0, Y0, X1, Y1, GraphicsColor);  // закрашиваем прямоугольник
  
  GraphicsColor = borderColor;      // устанавливаем цвет бордюра  
  if(border == 1) LCD_DrawRect(X0, Y0, X1, Y1);  // рисуем бордюр
  else if(border == 2)
  {
     LCD_DrawLine(X0 - 1, Y0 - 1, X1, Y1);
	 LCD_DrawLine(X0 - 1, Y0 - 1, X1 + 1, Y1);
  }
  else if(border == 3)
  {
     LCD_DrawLine(X1 + 1, Y0, X1 + 1, Y1);
	 LCD_DrawLine(X0-1, Y0 + 1, X1 + 1, Y0 + 1);
  }

  GraphicsColor = tmpColor;  // восстанавливаем исходный цвет

  LCD_SetFullScreen();
}


/******************************************************************************
* Function Name  : LCD_DrawCircle
* Description    : Нарисовать окружность, Брезенхем
* Input          : X0, Y0, Radius
* Return         : None
*******************************************************************************/
void LCD_DrawCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius)
{
  int32_t  D;       /* Decision Variable */
  uint32_t  CurX;   /* Current X Value */
  uint32_t  CurY;   /* Current Y Value */

  D = 3 - (Radius << 1);
  CurX = 0;
  CurY = Radius;
  
  Set_LCD_CS();
  
  while (CurX <= CurY)
  {
    LCD_SetCursor(Xpos + CurX, Ypos + CurY);
    LCD_PutPixel(GraphicsColor);

    LCD_SetCursor(Xpos + CurX, Ypos - CurY);
    LCD_PutPixel(GraphicsColor);

    LCD_SetCursor(Xpos - CurX, Ypos + CurY);
    LCD_PutPixel(GraphicsColor);

    LCD_SetCursor(Xpos - CurX, Ypos - CurY);
    LCD_PutPixel(GraphicsColor);

    LCD_SetCursor(Xpos + CurY, Ypos + CurX);
    LCD_PutPixel(GraphicsColor);

    LCD_SetCursor(Xpos + CurY, Ypos - CurX);
    LCD_PutPixel(GraphicsColor);

    LCD_SetCursor(Xpos - CurY, Ypos + CurX);
    LCD_PutPixel(GraphicsColor);

    LCD_SetCursor(Xpos - CurY, Ypos - CurX);
    LCD_PutPixel(GraphicsColor);
   
    if (D < 0)
    {
      D += (CurX << 2) + 6;
    }
    else
    {
      D += ((CurX - CurY) << 2) + 10;
      CurY--;
    }
    CurX++;
  }
  Reset_LCD_CS();
}


/******************************************************************************
* Function Name  : LCD_DrawCircle
* Description    : Нарисовать прямоугольную фигуру с закругленными углами
* Input          : X0, Y0, X1, Y1, Radius
* Return         : None
*******************************************************************************/
void DrawBeveledRect(uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2, uint16_t Radius)
{
  int32_t  D;           /* Decision Variable */
  uint32_t  CurX;   /* Current X Value */
  uint32_t  CurY;   /* Current Y Value */

  D = 3 - (Radius << 1);
  CurX = 0;
  CurY = Radius;
  
  Set_LCD_CS();
  
  /* --- Рисуем закругления  --- */   
  while (CurX <= CurY)
  {
    LCD_SetCursor(X2 + CurX, Y1 + CurY);         // 1 Квадрант
    LCD_PutPixel(GraphicsColor);
	LCD_SetCursor(X2 + CurY, Y1 + CurX);
    LCD_PutPixel(GraphicsColor);
	
	LCD_SetCursor(X2 + CurX, Y2 - CurY);         // 2 Квадрант
    LCD_PutPixel(GraphicsColor);
	LCD_SetCursor(X2 + CurY, Y2 - CurX);
    LCD_PutPixel(GraphicsColor);
	
	LCD_SetCursor(X1 - CurX, Y2 - CurY);         // 3 Квадрант
    LCD_PutPixel(GraphicsColor);
	LCD_SetCursor(X1 - CurY, Y2 - CurX);
    LCD_PutPixel(GraphicsColor);
	
	LCD_SetCursor(X1 - CurX, Y1 + CurY);        // 4 Квадрант
    LCD_PutPixel(GraphicsColor);
	LCD_SetCursor(X1 - CurY, Y1 + CurX);         
    LCD_PutPixel(GraphicsColor);
	
	
    if (D < 0) D += (CurX << 2) + 6;
    else { D += ((CurX - CurY) << 2) + 10;  CurY--; }
    CurX++;
  }  	    
  Reset_LCD_CS();
  
  /* --- Рисуем соединяющие линии --- */
  LCD_DrawLine(X1, (Y1 + Radius), X2, (Y1 + Radius));		// Верхняя соединяющая линия 
  LCD_DrawLine((X1 - Radius), Y2, (X1 - Radius), Y1);		// Левая соединяющая линия 
  LCD_DrawLine((X2 + Radius), Y2, (X2 + Radius), Y1);		// Правая соединяющая линия 
  LCD_DrawLine(X1, (Y2 - Radius), X2, (Y2 - Radius));		// Нижняя соединяющая линия
}


/******************************************************************************
* Function Name  : LCD_DrawCircle
* Description    : Нарисовать закрашенную прямоугольную фигуру с закругленными углами
* Input          : X0, Y0, X1, Y1, Radius, border, borderColor
* Return         : None
*******************************************************************************/
void DrawFillBeveledRect(uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2, uint16_t Radius, uint8_t border, uint16_t borderColor)
{  
  uint16_t tmp_color = GraphicsColor;
  int32_t  D;       /* Decision Variable */
  uint16_t  CurX;   /* Current X Value */
  uint16_t  CurY;   /* Current Y Value */  
 
  if((border != 0) && (border != 1))return;
  if(border == 0) borderColor = GraphicsColor;

  D = 3 - (Radius << 1);
  CurX = 0;
  CurY = Radius;    
  
  /* --- Рисуем закругления  --- */   
  while (CurX <= CurY)
  {
    GraphicsColor = borderColor;
			
	Set_LCD_CS();
	
    LCD_SetCursor(X2 + CurX, Y1 + CurY);         // 1 Квадрант
    LCD_PutPixel(GraphicsColor);
	LCD_SetCursor(X2 + CurY, Y1 + CurX);
    LCD_PutPixel(GraphicsColor);
	
	LCD_SetCursor(X2 + CurX, Y2 - CurY);         // 2 Квадрант
    LCD_PutPixel(GraphicsColor);
	LCD_SetCursor(X2 + CurY, Y2 - CurX);
    LCD_PutPixel(GraphicsColor);
	
	LCD_SetCursor(X1 - CurX, Y2 - CurY);         // 3 Квадрант
    LCD_PutPixel(GraphicsColor);
	LCD_SetCursor(X1 - CurY, Y2 - CurX);
    LCD_PutPixel(GraphicsColor);
	
	LCD_SetCursor(X1 - CurX, Y1 + CurY);         // 4 Квадрант
    LCD_PutPixel(GraphicsColor);
	LCD_SetCursor(X1 - CurY, Y1 + CurX);         
    LCD_PutPixel(GraphicsColor);
	
	GraphicsColor = tmp_color;
		
	// 1,4 Квадрант
	LCD_DrawLine(X2 + CurX - border, Y2 - CurY + border, X2 + CurX - border, (Y1 - 1) + (CurY * 2) - border);
    LCD_DrawLine(X2 + CurY - border, Y2 - CurX + border, X2 + CurY - border, (Y1 - 1) + (CurX * 2) - border);		

    // 2,3 Квадрант
	LCD_DrawLine(X1 - CurX + border, Y2 - CurY + border, X1 - CurX + border, (Y1 - 1) + (CurY * 2) - border);
    LCD_DrawLine(X1 - CurY + border, Y2 - CurX + border, X1 - CurX + border, (Y1 - 1) + (CurX * 2) - border);    
	
	
    if (D < 0) D += (CurX << 2) + 6;
    else { D += ((CurX - CurY) << 2) + 10;  CurY--; }
    CurX++;
  }   
  
  GraphicsColor = borderColor;
  /* --- Рисуем соединяющие линии --- */
  LCD_DrawLine(X1, (Y1 + Radius), X2, (Y1 + Radius));		// Верхняя соединяющая линия 
  LCD_DrawLine((X1 - Radius), Y2, (X1 - Radius), Y1);       // Левая соединяющая линия 
  LCD_DrawLine((X2 + Radius), Y2, (X2 + Radius), Y1);       // Правая соединяющая линия 
  LCD_DrawLine(X1, (Y2 - Radius), X2, (Y2 - Radius));		// Нижняя соединяющая линия 
    
  
  GraphicsColor = tmp_color;  
  LCD_ClearArea(X1 + border, X2  - border, (Y2 - Radius) + border, (Y1 + Radius) - border, GraphicsColor);
  
  LCD_SetFullScreen();   
}



/******************************************************************************
* Function Name  : LCD_DrawTriangle
* Description    : Нарисовать треугольник
* Input          : X0, Y0, X1, Y1, X2, Y2, Color
* Return         : None
*******************************************************************************/
void LCD_DrawTriangle(uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2, uint16_t X3, uint16_t Y3, uint16_t Color)
{		
   GraphicsColor = Color;
   
   LCD_DrawLine(X1, Y1, X2, Y2);	 
   LCD_DrawLine(X1, Y1, X3, Y3);
   LCD_DrawLine(X2, Y2, X3, Y3);   
}


