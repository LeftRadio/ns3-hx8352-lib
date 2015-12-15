/*************************************************************************************
*
Project : HX8352 Graphics Library :: Primitive Layer header
Version : 
Date    : 
Author  : Left Radio                          
Comments: 
*
**************************************************************************************/

#ifndef __GRAPHIC_PRIMITIVE_H
#define __GRAPHIC_PRIMITIVE_H 

/* Includes ------------------------------------------------------------------*/	   


/* Exported typedef -----------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported function --------------------------------------------------------*/

/* Нарисовать точку */
void DrawPixel(uint16_t X0, uint16_t Y0, uint16_t Color);

/* Нарисовать призвольную прямую линию */
void LCD_DrawLine(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1);  

/* Нарисовать прямоугольник */
void LCD_DrawRect(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1);

/* Нарисовать закрашенный прямоугольник */
void LCD_DrawFillRect(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, uint8_t border, uint16_t borderColor);

/* Нарисовать окружность */
void LCD_DrawCircle(uint16_t X0, uint16_t Y0, uint16_t Radius);  

/* Нарисовать прямоугольную фигуру с закругленными углами */
void LCD_DrawBeveledRect(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, uint16_t Radius);

/* Нарисовать закрашенную прямоугольную фигуру с закругленными углами */
void LCD_DrawFillBeveledRect(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, uint16_t Radius, uint8_t border, uint16_t borderColor);

/* Нарисовать треугольник */
void LCD_DrawTriangle(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2, uint16_t Color);


#endif /* __GRAPHIC_PRIMITIVE_H */
/*********************************************************************************************************
      END FILE
*********************************************************************************************************/


