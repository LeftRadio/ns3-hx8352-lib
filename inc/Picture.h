/*************************************************************************************
*
Project : HX8352 Graphics Library :: Picture Layer header
Version : 
Date    : 
Author  : Left Radio                          
Comments: Вывод изображений
*
**************************************************************************************/


// Обьявляем структуры параметров изображения
typedef struct   
{ 
   uint16_t Width;            // Picture Width 
   uint16_t Height;           // Picture Height 
   uint8_t RLE;               // RLE Copressed flag, if yes value = 1 
   uint32_t massiveSize;      // Last index of bitmap massive
   uint8_t *ptrBitmap;       // Bitmap point array 
}BITMAP_INFO_8;

typedef struct   
{ 
   uint16_t Width;            // Picture Width 
   uint16_t Height;           // Picture Height 
   uint8_t RLE;               // RLE Copressed flag, if yes value = 1 
   uint32_t massiveSize;      // Last index of bitmap massive
   uint16_t *ptrBitmap;       // Bitmap point array 
}BITMAP_INFO_16;




/** Вывод изображения формата 1bit цвет/8bit данные **/
void LCD_Draw_1_8_IMG(uint16_t X0pos, uint16_t Y0pos, const BITMAP_INFO_8 *IN_bitmap, uint8_t RLE);

/** Вывод изображения формата 1bit цвет/16bit данные **/
void LCD_Draw_1_16_IMG(uint16_t X0pos, uint16_t Y0pos, const BITMAP_INFO_16 *IN_bitmap, uint8_t RLE);

/** Вывод изображения формата 8bit цвет/8bit данные **/
void LCD_Draw_8_IMG(uint16_t X0pos, uint16_t Y0pos, const BITMAP_INFO_8 *IN_bitmap, uint8_t RLE);

/** Вывод изображения формата 16bit цвет/16bit данные **/
void LCD_Draw_16_IMG(uint16_t X0pos, uint16_t Y0pos, const BITMAP_INFO_16 *IN_bitmap, uint8_t RLE);

