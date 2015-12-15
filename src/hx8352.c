/*************************************************************************************
*
Project : HX8352 Graphics Library :: Low-lewel Layer sourse
Version : 
Date    : 
Author  : Left Radio                          
Comments: 
*
**************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "hx8352.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile uint16_t TextColor;
volatile uint16_t BackColor;
volatile uint16_t GraphicsColor;

volatile uint16_t LCD_maxWidth, LCD_maxHight;

LCD_Direction_TypeDef LCD_Direction = _0_degree;    // _0_degree = album, _90_degree = portret


/* Private function prototypes -----------------------------------------------*/ 
void LCD_PutPixel_16bpp(uint16_t Color);
void LCD_PutPixel_18bpp(uint16_t Color);

void (*LCD_PutPixel)(uint16_t Color) = LCD_PutPixel_16bpp;

/* прототип функции задержки, используется только для инициализации HX8352 */
static void LCD_Delay_ms (uint16_t mS);



/* Private function  --------------------------------------------------------*/ 
 
/*************************************************************************************
*
*  Установка и считывание цветов
*
**************************************************************************************/

/******************************************************************************
* Function Name  : LCD_SetTextColor
* Description    : Установить цвет текста
* Input          : color  
* Return         : None
*******************************************************************************/
void LCD_SetTextColor(uint16_t color)
{
	TextColor = color;
}


/******************************************************************************
* Function Name  : LCD_GetTextColor
* Description    : Получить цвета текста
* Input          : None  
* Return         : TextColor
*******************************************************************************/
uint16_t LCD_GetTextColor(void)
{
	return TextColor;
}


/******************************************************************************
* Function Name  : LCD_SetGraphicsColor
* Description    : Установить цвет графики
* Input          : color 
* Return         : None
*******************************************************************************/ 
void LCD_SetGraphicsColor(uint16_t color)
{
	GraphicsColor = color;
}


/******************************************************************************
* Function Name  : LCD_GetGraphicsColor
* Description    : Получить цвет графики
* Input          : None  
* Return         : GraphicsColor
*******************************************************************************/
uint16_t LCD_GetGraphicsColor(void)
{
	return GraphicsColor;
}
 

/******************************************************************************
* Function Name  : LCD_SetBackColor
* Description    : Установить цвет фона
* Input          : color   
* Return         : None
*******************************************************************************/ 
void LCD_SetBackColor(uint16_t color)
{
	BackColor = color;
}


/******************************************************************************
* Function Name  : LCD_GetBackColor
* Description    : Получить цвет фона
* Input          : None  
* Return         : BackColor
*******************************************************************************/
uint16_t LCD_GetBackColor(void)
{
	return BackColor;
}
 

 
/*************************************************************************************
*
*  Low level функции работы с контроллером ЖК
*
**************************************************************************************/


/** --------------------------------  Low level команды и запись в HX8352  -------------------------------- **/

/******************************************************************************
* Function Name  : Set_LCD_CS
* Description    : Установить активный уровень на выводе Chip Select LCD
* Input          : None  
* Return         : None
*******************************************************************************/
void Set_LCD_CS(void)
{
	CS_LCD_set;
}


/******************************************************************************
* Function Name  : Set_LCD_CS
* Description    : Сбросить активный уровень на выводе Chip Select LCD
* Input          : None  
* Return         : None
*******************************************************************************/
void Reset_LCD_CS(void)
{
	CS_LCD_clr;
}


/******************************************************************************
* Function Name  : cmd_WR
* Description    : Строб записи в LCD
* Input          : None  
* Return         : None
*******************************************************************************/
static __inline void cmd_WR(void)
{
	WR_LCD_clr; 
	WR_LCD_set;
}


/******************************************************************************
* Function Name  : HC573_CMD_WR
* Description    : Строб записи в 74HC573
* Input          : None
* Return         : None
*******************************************************************************/
static __inline void HC573_CMD_WR(void)
{
	HC573_LE_set;
	HC573_LE_clr;
}


/******************************************************************************
* Function Name  : LCD_Write_Index
* Description    : Запись индекса регистра
* Input          : RegisterIndex  
* Return         : None
*******************************************************************************/
static __inline void LCD_Write_Index(uint8_t RegisterIndex)
{ 
	RS_LCD_clr;				  //register index write active
	
	LCD_DATA(RegisterIndex); 
	cmd_WR();
	
	RS_LCD_set;                  //register index write inactive 
}


/******************************************************************************
* Function Name  : Set_LCD_8B_REG
* Description    : Запись данных в регистр
* Input          : RegisterIndex, data  
* Return         : None
*******************************************************************************/
static __inline void Set_LCD_8B_REG(uint8_t RegisterIndex, uint8_t Data)   
{  
	LCD_Write_Index(RegisterIndex);		// Запись индекса регистра
	  
	LCD_DATA(Data);						// Данные в порт
	cmd_WR();							// Строб для записи данных в регистр
}


/******************************************************************************
* Function Name  : Get_LCD_8B_REG
* Description    : чтение данных из регистра
* Input          : RegisterIndex
* Return         : Data
*******************************************************************************/
static __inline uint8_t Get_LCD_8B_REG(uint8_t RegisterIndex)
{
	LCD_Write_Index(RegisterIndex);
	
	RD_LCD_set;
	//RegisterIndex = read_LCD_DATA();
	RD_LCD_clr;
	
	return RegisterIndex;
}





/** -----------------------------------  Mid level команды HX8352  ----------------------------------- **/

/******************************************************************************
* Function Name  : LCD_PutPixel_16bpp
* Description    : Нарисовать точку	по текущим координатам
* Input          : Color
* Return         : None
*******************************************************************************/
void LCD_PutPixel_16bpp(uint16_t Color)
{
	LCD_DATA(Color >> 8);          		// Записать старший бит
	cmd_WR();
	
	LCD_DATA(Color & 0x00FF);     // Записать младший бит
	cmd_WR(); 
}


/******************************************************************************
* Function Name  : LCD_PutPixel_18bpp
* Description    : Нарисовать точку	по текущим координатам
* Input          : Color
* Return         : None
*******************************************************************************/
void LCD_PutPixel_18bpp(uint16_t Color)
{
	LCD_DATA((Color & 0xF800) >> 8);      // Записать красный
	cmd_WR();
	
	LCD_DATA((Color & 0x07E0) >> 3);      // Записать зеленый
	cmd_WR(); 

	LCD_DATA((Color & 0x001F) << 3);      // Записать синий
	cmd_WR();
}


/******************************************************************************
* Function Name  : LCD_PutPixel_18bpp_16
* Description    : Нарисовать точку	по текущим координатам
* Input          : Color
* Return         : None
*******************************************************************************/
void LCD_PutPixel_18bpp_16(uint16_t Color)
{
	LCD_DATA((Color & 0xFF00) >> 8);      // Записать HC573
	HC573_CMD_WR();
	LCD_DATA(Color & 0x00FF);
	cmd_WR();
}


/******************************************************************************
* Function Name  : LCD_SetCursor
* Description    : установить курсор по координате
* Input          : Xpos, Ypos
* Return         : None
*******************************************************************************/
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos)
{ 
	Set_LCD_8B_REG(0x02, Ypos >> 8);	// Column high
	Set_LCD_8B_REG(0x03, Ypos);			// Column low 
	Set_LCD_8B_REG(0x06, Xpos >> 8);	// Row high
	Set_LCD_8B_REG(0x07, Xpos);			// Row low
	     
	LCD_Write_Index(0x22);
}


/******************************************************************************
* Function Name  : LCD_SetArea
* Description    : Установить область вывода
* Input          : X0pos, Y0pos, X1pos, Y1pos
* Return         : None
*******************************************************************************/
void LCD_SetArea(uint16_t X0pos, uint16_t Y0pos, uint16_t X1pos, uint16_t Y1pos)
{    
	uint16_t x_0 = 0, y_0 = 0;
  	uint16_t x_1 = 0, y_1 = 0;

	if(LCD_Direction == _0_degree)   // Если альбомный режим
	{
		/* Set max coordinate */
		x_0 = X0pos; y_0 = Y0pos;
		x_1 = X1pos; y_1 = Y1pos;
		LCD_maxWidth = 399;  LCD_maxHight = 239;
	}
	else if(LCD_Direction == _90_degree)   // Если портретный режим
	{
		/* Set max coordinate */
		x_0 = Y0pos; y_0 = X0pos;
		x_1 = Y1pos; y_1 = X1pos;
		LCD_maxWidth = 239;  LCD_maxHight = 399;
	}
	
	CS_LCD_set;
	
	Set_LCD_8B_REG(0x02,y_0 >> 8); // Column address start2
	Set_LCD_8B_REG(0x03,y_0);   // Column address start1
	Set_LCD_8B_REG(0x04,y_1 >> 8); // Column address end2
	Set_LCD_8B_REG(0x05,y_1);   // Column address end1
	Set_LCD_8B_REG(0x06,x_0 >> 8); // Row address start2
	Set_LCD_8B_REG(0x07,x_0); // Row address start1
	Set_LCD_8B_REG(0x08,x_1 >> 8); // Row address end2
	Set_LCD_8B_REG(0x09,x_1); // Row address end1  
	
	LCD_Write_Index(0x22);
	
	CS_LCD_clr;
}


/******************************************************************************
* Function Name  : LCD_SetFullScreen
* Description    : Установить область вывода на весь экран
* Input          : None
* Return         : None
*******************************************************************************/
void LCD_SetFullScreen(void)
{
//  uint16_t x = 0, y = 0;
  
  LCD_SetArea(0, 0, 399, 239);
//  if(LCD_Direction == _0_degree)   // Если альбомный режим
//  {
//    /* Set max coordinate */
//    x = 399; y = 239;
//	LCD_maxWidth = 399;  LCD_maxHight = 239;
//  }
//  else if(LCD_Direction == _90_degree)   // Если портретный режим
//  {
//    /* Set max coordinate */
//    x = 239; y = 399;
//	LCD_maxWidth = 239;  LCD_maxHight = 399;
//  }
//
//  CS_LCD_set;
//
//  Set_LCD_8B_REG(0x02,0); // Column address start2
//  Set_LCD_8B_REG(0x03,0);   // Column address start1
//  Set_LCD_8B_REG(0x04,y >> 8); // Column address end2
//  Set_LCD_8B_REG(0x05,y);   // Column address end1
//  Set_LCD_8B_REG(0x06,0); // Row address start2
//  Set_LCD_8B_REG(0x07,0); // Row address start1
//  Set_LCD_8B_REG(0x08,x >> 8); // Row address end2
//  Set_LCD_8B_REG(0x09,x); // Row address end1
//
//  LCD_Write_Index(0x22);
//
//  CS_LCD_clr;
}





/** -----------------------------------  Hight level команды HX8352  ----------------------------------- **/

/******************************************************************************
* Function Name  : LCD_Init
* Description    : Инициализация HX8352
* Input          : None
* Return         : None
*******************************************************************************/
void LCD_Init(void)
{  
   RD_LCD_set;
   WR_LCD_set;
   RS_LCD_set;

   /* Reset chip */
   RES_LCD_clr;    
   LCD_Delay_ms(1);
   RES_LCD_set;
   
   LCD_Delay_ms(5);    
   
   CS_LCD_set; 
   
   Set_LCD_8B_REG(0x83,0x02); // TESTM=1
   Set_LCD_8B_REG(0x85,0x03); // VDC_SEL=011.
   Set_LCD_8B_REG(0x8C,0x93); // STBA[7]=1, STBA[5:4]=01, STBA[1:0]=11
   Set_LCD_8B_REG(0x91,0x01); // DCDC_SYNC=1
   Set_LCD_8B_REG(0x83,0x00); // TESTM=0
   //--------------------------------------------------------------------------------
   // Gamma Setting
   Set_LCD_8B_REG(0x3E,0xB0);
   Set_LCD_8B_REG(0x3F,0x03);
   Set_LCD_8B_REG(0x40,0x10);
   Set_LCD_8B_REG(0x41,0x56);
   Set_LCD_8B_REG(0x42,0x13);
   Set_LCD_8B_REG(0x43,0x46);
   Set_LCD_8B_REG(0x44,0x23);
   Set_LCD_8B_REG(0x45,0x76);
   Set_LCD_8B_REG(0x46,0x4F);
   Set_LCD_8B_REG(0x47,0x5F);
   Set_LCD_8B_REG(0x48,0x4F);
   Set_LCD_8B_REG(0x49,0x4F);
   //--------------------------------------------------------------------------------
   // Power Supply Setting
   Set_LCD_8B_REG(0x17,0x61); // RADJ=0110(85%), OSC_EN=1
   Set_LCD_8B_REG(0x2B,0xF9); // N_DCDC=0xF9.
   LCD_Delay_ms(10);
   Set_LCD_8B_REG(0x1B,0x14); // BT=0001, AP=100
   Set_LCD_8B_REG(0x1A,0x11); // VC3=001, VC1=001
   Set_LCD_8B_REG(0x1C,0x0d); // VRH=1101
   Set_LCD_8B_REG(0x1F,0x3D); // VCM=100_0010
   LCD_Delay_ms(20);
   Set_LCD_8B_REG(0x19,0x0A); // GASENB=0, PON=0, DK=1, XDK=0,
   // VLCD_TRI=1, STB=0
   Set_LCD_8B_REG(0x19,0x1A); // GASENB=0, PON=1, DK=1, XDK=0,
   // VLCD_TRI=1, STB=0
   LCD_Delay_ms(40);
   Set_LCD_8B_REG(0x19,0x12); // GASENB=0, PON=1, DK=0, XDK=0,
   // VLCD_TRI=1, STB=0
   LCD_Delay_ms(40);
   Set_LCD_8B_REG(0x1E,0x20); // VCOMG=1, VDV=0_1100
   LCD_Delay_ms(50);
   //--------------------------------------------------------------------------------
   // Display ON Setting
   Set_LCD_8B_REG(0x3C,0x60); // N_SAP=0110 000
   Set_LCD_8B_REG(0x3D,0x40); // I_SAP =0100 0000
   Set_LCD_8B_REG(0x34,0x38); // EQS=0011 1000
   Set_LCD_8B_REG(0x35,0x38); // EQP=0011 1000
   Set_LCD_8B_REG(0x24,0x38); // GON=1, DTE=1, D=10
   LCD_Delay_ms(40);
   Set_LCD_8B_REG(0x24,0x3C); // GON=1, DTE=1, D=11
   Set_LCD_8B_REG(0x16,0x1C); // BGR=0
   Set_LCD_8B_REG(0x01,0x06); // INVON=0, NORNO=1
   Set_LCD_8B_REG(0x55,0x00);
   //--------------------------------------------------------------------------------        
   Set_LCD_8B_REG(0x02,0x00); // Column address start2
   Set_LCD_8B_REG(0x03,0x00); // Column address start1
   Set_LCD_8B_REG(0x04,0x00); // Column address end2
   Set_LCD_8B_REG(0x05,0xEF); // Column address end1
   Set_LCD_8B_REG(0x06,0x00); // Row address start2
   Set_LCD_8B_REG(0x07,0x00); // Row address start1
   Set_LCD_8B_REG(0x08,0x01); // Row address end2
   Set_LCD_8B_REG(0x09,0x8F); // Row address end1    
      
   LCD_Write_Index(0x22);
   
   LCD_maxWidth = 399; LCD_maxHight = 239;

   CS_LCD_clr; 
}


/******************************************************************************
* Function Name  : LCD_FillScreen
* Description    : Полностью залить экран одним цветом
* Input          : Color
* Return         : None
*******************************************************************************/
void LCD_FillScreen(uint16_t Color)
{  
    uint32_t cntT = 0;   
    
    LCD_SetFullScreen();  // Установить весь экран областью вывода
    
    CS_LCD_set;       
    do
    {      
    	LCD_PutPixel(Color);	// Запись точки   
    }
    while(++cntT != 96000);		// Размер экрана 400х240 - 96000 точек    
    CS_LCD_clr;
}


/******************************************************************************
* Function Name  : LCD_clear_area
* Description    : Очистка или закраска области экрана одним цветом
* Input          : X0pos, Y0pos, X1pos, Y1pos, Color
* Return         : None
*******************************************************************************/
void LCD_ClearArea(uint16_t X0pos, uint16_t Y0pos, uint16_t X1pos, uint16_t Y1pos, uint16_t Color)
{
   uint32_t pntNum;
      
   LCD_SetArea(X0pos, Y0pos, X1pos, Y1pos);  // Установка области вывода LCD равной области очистки
   
   pntNum = ((X1pos-X0pos)+1)*((Y1pos-Y0pos)+1);   // Вычесление количества точек внутри очищаемой области
   
   CS_LCD_set;              
   do
   {    
     LCD_PutPixel(Color);  // Вывод точки
   }
   while(pntNum--);  // Декремент количества точек для вывода     
   CS_LCD_clr;

   LCD_SetFullScreen();
}


/******************************************************************************
* Function Name  : LCD_Change_Direction
* Description    : Сменить ориентацию LCD, портрет/альбом
* Input          : Direction - 0/90 degree
* Return         : None
*******************************************************************************/
void LCD_Change_Direction(uint8_t Direction)
{  
	if (Direction == 0)
	{
		CS_LCD_set;
		
		/* Set GRAM write direction and BGR = 0 */
		Set_LCD_8B_REG(0x16,0x1C); // BGR=0
		LCD_Direction = _0_degree;    
	}
	else if (Direction == 90)
	{
		CS_LCD_set;
		
		/* Set GRAM write direction and BGR = 0 */
		Set_LCD_8B_REG(0x16,0xBC); // BGR=0
		LCD_Direction = _90_degree;    
	}  
	else return;
	
	CS_LCD_clr;  
	LCD_SetFullScreen();  // Установить весь экран областью вывода
}



/******************************************************************************
* Function Name  : LCD_Change_Direction
* Description    : Сменить ориентацию LCD, портрет/альбом
* Input          : Number of Depth Bits, example set 8 for 256bit color
* Return         : None
*******************************************************************************/
void LCD_Set_DataWidth_ColorBitDepth(uint8_t DataWidth, uint8_t NumberDepthBits)
{  
	if(DataWidth == 8)
	{
		switch(NumberDepthBits)
		{
		case 16:  LCD_PutPixel = LCD_PutPixel_16bpp;
		break;
		case 18:  LCD_PutPixel = LCD_PutPixel_18bpp;
		break;
		default:  { LCD_PutPixel = LCD_PutPixel_16bpp; return; }
		}
	}
	else if(DataWidth == 16)
	{
		LCD_PutPixel = LCD_PutPixel_18bpp_16;
	}
}



/******************************************************************************
* Function Name  : LCD_Delay_ms
* Description    : функция задержки, используется только для инициализации HX8352
* Input          : delay in mSec
* Return         : None
*******************************************************************************/
void LCD_Delay_ms (uint16_t mS)
{
   volatile unsigned long d;
   
   for (d = 0; d<(mS * 10000); ++d);
}

