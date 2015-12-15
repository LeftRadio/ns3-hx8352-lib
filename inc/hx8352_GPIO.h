
#include <stdint.h>                           /* Include standard types */

#define     __IO    volatile                  /*!< defines 'read / write' permissions   */

//extern const uint16_t rvs_data[256]; 

/** --------------------------  Обьявление переменных для назначения выводов МК  -------------------------- **/

/** Обьявляем структуру типа GPIO_TypeDef, полностью идентична стандартной в stm32f10x.h **/
typedef struct
{
  __IO uint32_t CRL;
  __IO uint32_t CRH;
  __IO uint32_t IDR;
  __IO uint32_t ODR;
  __IO uint32_t BSRR;
  __IO uint32_t BRR;
  __IO uint32_t LCKR;
} GPIO_TypeDef;

/** Обьявляем структуру типа LCD_Pin_TypeDef **/
typedef struct 
{
  GPIO_TypeDef* GPIOx;
  uint16_t Pin;
} LCD_Pin_TypeDef;

/** Обьявляем структуру типа LCD_DataPort_TypeDef **/
typedef struct
{
  GPIO_TypeDef* GPIOx;
} LCD_DataPort_TypeDef;


/** Переменные для управляющих сигналов LCD типа LCD_DataPort_TypeDef **/
extern LCD_Pin_TypeDef RES_LCD;
extern LCD_Pin_TypeDef RD_LCD;
extern LCD_Pin_TypeDef RS_LCD;
extern LCD_Pin_TypeDef WR_LCD;
extern LCD_Pin_TypeDef CS_LCD;
extern LCD_Pin_TypeDef HC573_LE;

/** Переменную для шины данных LCD типа LCD_DataPort_TypeDef **/
extern LCD_DataPort_TypeDef LCD_DataPort;

/** Битовая маска для порта шины данных LCD **/
extern uint16_t LCD_portMask; 




/** --------------------  Макросы установки и сброса управляющих сигналов, шины данных  -------------------- **/

#define LCD_DATA(data)   LCD_DataPort.GPIOx->BRR = LCD_portMask; LCD_DataPort.GPIOx->BSRR = data;

//#define LCD_DATA(data)   LCD_DataPort.GPIOx->ODR = ( LCD_DataPort.GPIOx->IDR & LCD_portMask) | data;


#define RES_LCD_set			RES_LCD.GPIOx->BSRR = RES_LCD.Pin;
#define RES_LCD_clr			RES_LCD.GPIOx->BSRR = RES_LCD.Pin << 16;

#define RD_LCD_set			RD_LCD.GPIOx->BSRR = RD_LCD.Pin;
#define RD_LCD_clr			RD_LCD.GPIOx->BSRR = RD_LCD.Pin << 16;

#define RS_LCD_set			RS_LCD.GPIOx->BSRR = RS_LCD.Pin;
#define RS_LCD_clr			RS_LCD.GPIOx->BSRR = RS_LCD.Pin << 16;

#define WR_LCD_set			WR_LCD.GPIOx->BSRR = WR_LCD.Pin;
#define WR_LCD_clr			WR_LCD.GPIOx->BSRR = WR_LCD.Pin << 16;

#define CS_LCD_clr			CS_LCD.GPIOx->BSRR = CS_LCD.Pin;
#define CS_LCD_set			CS_LCD.GPIOx->BSRR = CS_LCD.Pin << 16;

#define HC573_LE_set		HC573_LE.GPIOx->BSRR = HC573_LE.Pin;
#define HC573_LE_clr		HC573_LE.GPIOx->BSRR = HC573_LE.Pin << 16;


/** -------------------------------  Назначение выводов МК для управления LCD  ------------------------------- **/

/** Установка порта под 8-ми битную шинну данных **/
extern void set_LCD_DATAPort_Init(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pins);

/* Установка выводов МК под управляющие сигналы LCD */
extern void set_RES_LCD(GPIO_TypeDef* GPIOx, uint16_t Pin);
extern void set_RD_LCD(GPIO_TypeDef* GPIOx, uint16_t Pin);
extern void set_RS_LCD(GPIO_TypeDef* GPIOx, uint16_t Pin);
extern void set_WR_LCD(GPIO_TypeDef* GPIOx, uint16_t Pin);
extern void set_CS_LCD(GPIO_TypeDef* GPIOx, uint16_t Pin);
