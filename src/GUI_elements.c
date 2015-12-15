/*************************************************************************************
*
Project : HX8352 Graphics Library :: GUI Elements sourse
Version : 
Date    : 
Author  : Left Radio                          
Comments: 
*
**************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include <math.h>

#include "hx8352.h"
#include "Graphics_Primitive.h"
#include "GUI_elements.h"
#include "Clip.h"
#include "Font.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/ 
/* Private function  --------------------------------------------------------*/ 


/******************************************************************************
* Function Name  : LCD_DrawButton
* Description    : Draw Button  / ���������� ������
* Input          : 	X0, Y0, Height, Width, 	
				   	shadowColor 			- button shadow color	/	���� ���� ������
					active  				- state, ON or OFF		/	���������, ������ ��� ���
					*Color					- base color			/	������� ����
* Return         : None
*******************************************************************************/
void LCD_DrawButton(btnINFO *btn, uint8_t active)
{
	uint16_t i, j;
	uint16_t bColor, smColor[btn->Height];
	uint8_t R, G, B, tmp = 0;
	
	uint16_t X0 = btn->Left, X1 = btn->Width + btn->Left; //(btn->Width) + (btn->Left);
	uint16_t Y0 = btn->Lower, Y1 = btn->Height + btn->Lower - 1;
	
	LCD_SetFullScreen();

	if(active == 1) // ���� ������ �������, ��
	{
		bColor = btn->Active_Color;	
		GraphicsColor = BackColor;					// ������������� ���� ���� 
		LCD_SetTextColor(btn->Active_FontColor);	// ���������� ���� ������
		
		// ��� ��� ������ ������� �� �������� ��������� ����� � ���� �� 1 �������
	 	LCD_DrawLine(X1 - 1, Y0 - 1, X1 - 1, Y1 - 1);
	 	LCD_DrawLine(X0 - 1, Y1 - 1, X1 - 1, Y1 - 1);
	 	X0--;
	 	Y0--;		
	}
	else
	{
		bColor = btn->Color;
		GraphicsColor = 0xFFFF;		// ������������� ���� ���� 
		LCD_SetTextColor(btn->Active_FontColor);	// ���������� ���� ������

		// ������ ����
	 	LCD_DrawLine(X0 - 1, Y0 - 1, X0 - 1, Y1 - 1);
	 	LCD_DrawLine(X0 - 1, Y0 - 1, X1 - 1, Y0 - 1);
		
		tmp = 1;	
	}
	
	/* ������������ ����� �������� � ������ */ 
	R = (uint8_t)(bColor >> 11);
	G = (uint8_t)((bColor >> 5) & 0x3F);
	B = (uint8_t)(bColor & 0x1F);
	
	for(i = 0; i < btn->Height; i++)
	{
		smColor[i] = (R << 11) + (G << 5) + B;
		if((R < 0x1F) && (R > 0))R += 1;
		if((G < 0x3F) && (G > 0))G += 1;
		if((B < 0x1F) && (B > 0))B += 1;	
	}
	
	/* ������ ������ � ������� �� ��� ����� */ 
	Set_LCD_CS(); // ���������� �������� ������� �� ������ Chip Select LCD 
	
	// ����������� ������ ����������
	for(i = X0; i < X1; i++)
	{
	  LCD_SetCursor(i, Y0);  // ���������� ������ ������ �� ����� �������
	  for(j = Y0; j < Y1; j++)
	  {
	     LCD_PutPixel(smColor[j - Y0]);			// ���������� �����
	  }
	}  
	
	Reset_LCD_CS();  // ���������� 0 �� ������ Chip Select LCD  

	LCD_PutStrig(X0 + 4 + tmp, Y0 + tmp, 1, btn->Text);
}



/******************************************************************************
* Function Name  : LCD_DrawGrid
* Description    : draw custom grid				/	���������� �����
* Input          : GridType *grid	
* Return         : None
*******************************************************************************/
void LCD_DrawGrid(GridType *Grid, DrawState state)
{
	uint16_t i, j, k, l; 
	uint16_t tmpColor = (state == CLEAR)?  BackColor : Grid->Color;
	uint16_t X0, Y0, X1, Y1;

	uint16_t left  = Grid->centerX - (Grid->Width / 2);
	uint16_t right = Grid->centerX + (Grid->Width / 2);
	uint16_t lower = Grid->centerY - (Grid->Height / 2);
	uint16_t upper = Grid->centerY + (Grid->Height / 2);

	LCD_SetGraphicsColor(tmpColor);
	
	/* if need draw central lines */
	if(Grid->EnableCentralLines == ENABLE)
	{		
		X0 = left;	Y0 = Grid->centerY;
		X1 = right; Y1 = Grid->centerY;
		if(Verify_Clip_Line(&X0, &Y0, &X1, &Y1) != SET) LCD_DrawLine(X0, Y0, X1, Y1);
		
		X0 = Grid->centerX; Y0 = lower;
		X1 = Grid->centerX; Y1 = upper;
		if(Verify_Clip_Line(&X0, &Y0, &X1, &Y1) != SET) LCD_DrawLine(X0, Y0, X1, Y1);		
	}
	
	/* Draw 1 & 4 quadrant */
	// ������������ ������������ ����� �� ����� �� ������ ������
	for(i = Grid->centerX; i < right - 10; i = i + 25)
	{     
		l = 0;

		for(j = Grid->centerY; j <= upper; j = j + 5)
		{
			// ������������ �������������� ����� �� ����� �� ������ ����� � ����
			if(l++ == 5)
			{
				for(k = i; k < i + 25; k = k + 5)
				{
					if(Verify_Clip_Point(k, j) != SET)	DrawPixel(k, j, tmpColor);
					if(Verify_Clip_Point(k, (Grid->centerY * 2) - j) != SET)  DrawPixel(k, (Grid->centerY * 2) - j, tmpColor);
				}
				l = 1;
			}
			if(Verify_Clip_Point(i, j) != SET) DrawPixel(i, j, tmpColor);
			if(Verify_Clip_Point(i, (Grid->centerY * 2) - j) != SET) DrawPixel(i, (Grid->centerY * 2) - j, tmpColor);
		}
	}

	/* Draw 2 & 3 quadrant */
	// ������������ ������������ ����� �� ����� �� ������ �����
	for(i = Grid->centerX; i > left + 10; i = i - 25)
	{     
		l = 0;

		for(j = Grid->centerY; j <= upper; j = j + 5)
		{
			// ������������ �������������� ����� �� ����� �� ������ ����� � ����
			if(l++ == 5)
			{
				for(k = i; k > i - 25; k = k - 5)
				{
					if(Verify_Clip_Point(k, j) != SET)	DrawPixel(k, j, tmpColor);
					if(Verify_Clip_Point(k, (Grid->centerY * 2) - j) != SET)  DrawPixel(k, (Grid->centerY * 2) - j, tmpColor);
				}
				l = 1;
			}
			if(Verify_Clip_Point(i, j) != SET) DrawPixel(i, j, tmpColor);
			if(Verify_Clip_Point(i, (Grid->centerY * 2) - j) != SET) DrawPixel(i, (Grid->centerY * 2) - j, tmpColor);
		}
	}
}


