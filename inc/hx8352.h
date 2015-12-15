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

typedef enum	/* --- LCD_Direction_TypeDef портрет/альбом  --- */
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

/* --- Переменные отвечающие за цвет текста, фона и графики соответсвенно  --- */
extern volatile uint16_t TextColor;
extern volatile uint16_t BackColor;
extern volatile uint16_t GraphicsColor;

extern volatile uint16_t LCD_maxWidth, LCD_maxHight;


/* Exported function --------------------------------------------------------*/

/*************************************************************************************
*
*  Установка и считывание цветов
*
**************************************************************************************/
/* Установить цвет текста */
void LCD_SetTextColor(uint16_t color);

/* Получить цвета текста */
uint16_t LCD_GetTextColor(void); 

/* Установить цвет графики */
void LCD_SetGraphicsColor(uint16_t color);

/* Получить цвет графики */
uint16_t LCD_GetGraphicsColor(void);
  
/* Установить цвет фона */
void LCD_SetBackColor(uint16_t color);

/* Получить цвет фона */
uint16_t LCD_GetBackColor(void); 

 

/*************************************************************************************
*
*  Low level функции работы с контроллером ЖК
*
**************************************************************************************/

/** --------------------------------  Low level команды и запись в HX8352  -------------------------------- **/

/* Установить активный уровень на выводе Chip Select LCD */
void Set_LCD_CS(void);

/* Сбросить активный уровень на выводе Chip Select LCD */
void Reset_LCD_CS(void);

/* Строб записи в LCD */
//static __inline void cmd_WR(void);

/* Запись индекса регистра */
//static __inline void LCD_Write_Index(uint8_t index);

/* Запись данных в регистр */                
//static __inline void Set_LCD_8B_REG(uint8_t index, uint8_t data_lcd);



/** -----------------------------------  Mid level команды HX8352  ----------------------------------- **/

/* Нарисовать точку */
extern void (*LCD_PutPixel)(uint16_t Color);

/* Установить курсор */
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos);

/* Установить область вывода */
void LCD_SetArea(uint16_t X0pos, uint16_t Y0pos, uint16_t X1pos, uint16_t Y1pos);

/* Установить область вывода на весь экран */
void LCD_SetFullScreen(void); 



/** -----------------------------------  Hight level команды HX8352  ----------------------------------- **/

/* Инициализация HX8352 */
void LCD_Init(void);

/* Полностью залить экран одним цветом */
void LCD_FillScreen(uint16_t Color);

/* Очистка или закраска области экрана одним цветом */
void LCD_ClearArea(uint16_t X0pos, uint16_t Y0pos, uint16_t X1pos, uint16_t Y1pos, uint16_t Color);

/* Сменить ориентацию LCD, портрет/альбом */
void LCD_Change_Direction(uint8_t Direction);



#endif /* __HX8352_H */
/*********************************************************************************************************
      END FILE
*********************************************************************************************************/

