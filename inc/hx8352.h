/*************************************************************************************
*
Project : HX8352 Graphics Library :: Low-lewel Layer header
Version : 
Date    : 
Author  : Left Radio                          
Comments: 
*
**************************************************************************************/

#ifndef __HX8352_H
#define __HX8352_H 

/* Includes ------------------------------------------------------------------*/	   
#include <stdint.h>                           /* Include standard types */
#include "hx8352_GPIO.h"

/* Exported typedef -----------------------------------------------------------*/

typedef enum	/* --- LCD_Direction_TypeDef �������/������  --- */
{ 
	_0_degree = 0,
	_90_degree,	  
} LCD_Direction_TypeDef;


typedef enum {RESET = 0, SET = !RESET} FlagStatus;
typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;
typedef enum {ERROR = 0, SUCCESS = 3} ErrorStatus;

typedef enum {CLEAR = 0, DRAW = !CLEAR} DrawState;
typedef enum {IN_OBJECT = 0, OUT_OBJECT = !IN_OBJECT} ClipMode;

/* Exported define -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/

/* --- ���������� ���������� �� ���� ������, ���� � ������� �������������  --- */
extern volatile uint16_t TextColor;
extern volatile uint16_t BackColor;
extern volatile uint16_t GraphicsColor;

extern volatile uint16_t LCD_maxWidth, LCD_maxHight;


/* Exported function --------------------------------------------------------*/

/*************************************************************************************
*
*  ��������� � ���������� ������
*
**************************************************************************************/
/* ���������� ���� ������ */
void LCD_SetTextColor(uint16_t color);

/* �������� ����� ������ */
uint16_t LCD_GetTextColor(void); 

/* ���������� ���� ������� */
void LCD_SetGraphicsColor(uint16_t color);

/* �������� ���� ������� */
uint16_t LCD_GetGraphicsColor(void);
  
/* ���������� ���� ���� */
void LCD_SetBackColor(uint16_t color);

/* �������� ���� ���� */
uint16_t LCD_GetBackColor(void); 

 

/*************************************************************************************
*
*  Low level ������� ������ � ������������ ��
*
**************************************************************************************/

/** --------------------------------  Low level ������� � ������ � HX8352  -------------------------------- **/

/* ���������� �������� ������� �� ������ Chip Select LCD */
void Set_LCD_CS(void);

/* �������� �������� ������� �� ������ Chip Select LCD */
void Reset_LCD_CS(void);

/* ����� ������ � LCD */
//static __inline void cmd_WR(void);

/* ������ ������� �������� */
//static __inline void LCD_Write_Index(uint8_t index);

/* ������ ������ � ������� */                
//static __inline void Set_LCD_8B_REG(uint8_t index, uint8_t data_lcd);



/** -----------------------------------  Mid level ������� HX8352  ----------------------------------- **/

/* ���������� ����� */
extern void (*LCD_PutPixel)(uint16_t Color);

/* ���������� ������ */
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos);

/* ���������� ������� ������ */
void LCD_SetArea(uint16_t X0pos, uint16_t Y0pos, uint16_t X1pos, uint16_t Y1pos);

/* ���������� ������� ������ �� ���� ����� */
void LCD_SetFullScreen(void); 



/** -----------------------------------  Hight level ������� HX8352  ----------------------------------- **/

/* ������������� HX8352 */
void LCD_Init(void);

/* ��������� ������ ����� ����� ������ */
void LCD_FillScreen(uint16_t Color);

/* ������� ��� �������� ������� ������ ����� ������ */
void LCD_ClearArea(uint16_t X0pos, uint16_t Y0pos, uint16_t X1pos, uint16_t Y1pos, uint16_t Color);

/* ������� ���������� LCD, �������/������ */
void LCD_Change_Direction(uint8_t Direction);



#endif /* __HX8352_H */
/*********************************************************************************************************
      END FILE
*********************************************************************************************************/

