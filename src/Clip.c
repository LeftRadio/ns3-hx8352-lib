/*************************************************************************************
*
Project : HX8352 Graphics Library :: Clip Point, Lines sourse
Version : 
Date    : 
Author  : Left Radio                          
Comments: 
*
**************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "hx8352.h"
#include "Clip.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define ClipMaxObjects				5

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
Clip_Obj_TypeDef ClipObjects[ClipMaxObjects];
uint8_t NumOfActiveClipObjects = 0;
FunctionalState gClipState = DISABLE;

/* Exported variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/ 
FlagStatus CS_ClipPoint_IN_Window(uint16_t *x0, uint16_t *y0, uint8_t objNum);
FlagStatus CS_ClipPoint_OUT_Window(uint16_t *x0, uint16_t *y0, uint8_t objNum);

FlagStatus CS_ClipLine_Out_Window(uint16_t *x0, uint16_t *y0, uint16_t *x1, uint16_t *y1, uint8_t objNum);
FlagStatus CS_ClipLine_IN_Window(uint16_t *x0, uint16_t *y0, uint16_t *x1, uint16_t *y1, uint8_t objNum);


/* Private function  ---------------------------------------------------------*/

/******************************************************************************
* Function Name  : Set_NewRestrictObject
* Description    : ������ ����� ������ ��� ���������
* Input          : Xpos, Ypos, Color
* Return         : X0, Y0, X1, Y1
*******************************************************************************/
ErrorStatus Set_New_ClipObject(uint16_t X0, uint16_t Y0, uint16_t X1, uint16_t Y1, ClipMode NewMode, uint8_t NumInd)
{
	uint8_t i;

	if(NumInd == 0) return ERROR;

	/* verify coordinates */
	else if(X0 >= X1) return ERROR;
	else if(Y0 >= Y1) return ERROR;

	/* Find first empty Object */
	for(i = 0; i < ClipMaxObjects; i++)
	{
		//str_cmp = strcmp (ClipObjects[i].Name, Name);
		
		if(ClipObjects[i].NumInd == NumInd) return ERROR;
		else if(ClipObjects[i].State != ENABLE)
		{
			/* init new restriction object */
			ClipObjects[i].X_Left = X0;
			ClipObjects[i].Y_Down = Y0;
			ClipObjects[i].X_Right = X1;
			ClipObjects[i].Y_Up = Y1;
			ClipObjects[i].NumInd = NumInd;
			ClipObjects[i].State = ENABLE;
						
			if(NewMode == IN_OBJECT)
			{
				ClipObjects[i].pClipPoint = CS_ClipPoint_IN_Window;
				ClipObjects[i].pClipLine = CS_ClipLine_IN_Window;
			}
			else
			{
				ClipObjects[i].pClipPoint = CS_ClipPoint_OUT_Window;
				ClipObjects[i].pClipLine = CS_ClipLine_Out_Window;
			}

			NumOfActiveClipObjects++;

			gClipState = ENABLE;	
			return SUCCESS;
		}
	}
	return ERROR;	
}


/******************************************************************************
* Function Name  : Clear_RestrictObject
* Description    : ������� ������ ��� ���������
* Input          : Xpos, Ypos, Color
* Return         : X0, Y0, X1, Y1
*******************************************************************************/
ErrorStatus Clear_ClipObject(uint8_t NumInd)
{
	uint8_t i, cnt = 0;
	
	if(NumInd == 0) return ERROR;

	/* Find Object compare Name */
	for(i = 0; i < ClipMaxObjects; i++)
	{
		if(ClipObjects[i].NumInd == NumInd)
		{
			/* delete restriction object */
			ClipObjects[i].X_Left = 0;
			ClipObjects[i].Y_Down = 0;
			ClipObjects[i].X_Right = 0;
			ClipObjects[i].Y_Up = 0;
			ClipObjects[i].NumInd = 0;
			ClipObjects[i].State = DISABLE;
			ClipObjects[i].pClipPoint = CS_ClipPoint_IN_Window;
			ClipObjects[i].pClipLine = CS_ClipLine_IN_Window;

			NumOfActiveClipObjects--;
			break;
		}
		else if(ClipObjects[i].State == DISABLE) cnt++;
	}

	if(NumOfActiveClipObjects == 0)	gClipState = DISABLE;
	else if(cnt == ClipMaxObjects){ gClipState = DISABLE; return ERROR; }
	
	return SUCCESS;	
}



/*******************************************************************************
* Function Name  : Verify_Restriction_Point
* Description    : 
* Input          : uint16_t X0, uint16_t Y0
* Return         : FlagStatus  SET or RESET
*******************************************************************************/
FlagStatus Verify_Clip_Point(uint16_t X0, uint16_t Y0)
{
	uint8_t i;
	FlagStatus VisibleStatus = SET;

	/* if restriction is ON for one or more objects */
	if(gClipState == ENABLE)
	{
		for(i = 0; i <= NumOfActiveClipObjects; i++)
		{
			if(ClipObjects[i].State == ENABLE)
			{
				VisibleStatus = ClipObjects[i].pClipPoint(&X0, &Y0, i);

				if(NumOfActiveClipObjects == 1) return VisibleStatus;
				else if(VisibleStatus == SET) return SET;
			}
		}		
	}  
	
	return RESET;
}


/*******************************************************************************
* Function Name  : CS_ClipLine
* Description    :
* Input          : uint16_t X0, uint16_t Y0
* Return         : FlagStatus  SET or RESET
* Output         : if line is part restrict return new X0, Y0 coordinate
*******************************************************************************/
FlagStatus CS_ClipPoint_IN_Window(uint16_t *x0, uint16_t *y0, uint8_t objNum)
{
	if(*y0 <= ClipObjects[objNum].Y_Up)
	{
		if(*y0 >= ClipObjects[objNum].Y_Down)
		{
			if((*x0 >= ClipObjects[objNum].X_Left) && (*x0 <= ClipObjects[objNum].X_Right))
			{
				return SET;
			}
		}
	}
	return RESET;
}


/*******************************************************************************
* Function Name  : CS_ClipLine
* Description    :
* Input          : uint16_t X0, uint16_t Y0
* Return         : FlagStatus  SET or RESET
* Output         : if line is part restrict return new X0, Y0 coordinate
*******************************************************************************/
FlagStatus CS_ClipPoint_OUT_Window(uint16_t *x0, uint16_t *y0, uint8_t objNum)
{
	if(*y0 <= ClipObjects[objNum].Y_Up)
	{
		if(*y0 >= ClipObjects[objNum].Y_Down)
		{
			if(*x0 >= ClipObjects[objNum].X_Left)
			{
				if(*x0 <= ClipObjects[objNum].X_Right)
				{
					return RESET;
				}
				else *x0 = ClipObjects[objNum].X_Right;
			}
			else *x0 = ClipObjects[objNum].X_Left;
		}
		else *y0 = ClipObjects[objNum].Y_Down;
	}

	return SET;
}




/*--------------------------------------------------- CS_RestrictLine
* ��������� �������� ��������� �����-���������� �
* ������������ ������ ����������� �������
*
* int V_CSclip (float *x0, float *y0, float *x1, float *y1)
*
* �������� �������, �������� ���������� ��������� ���
* ����� (x0,y0), (x1,y1), �� ���� ���������, ���������
* ����������� ��������� Wxlef, Wybot, Wxrig, Wytop
*
* �������� ������ ������� ������������� ����,
* ��������������� ��� ��������� ������������ ���� ���������
* �� �������:
*

 *  1001 | 1000 | 1010
 *  -----|------|-----
 *       | ���� |
 *  0001 | 0000 | 0010
 *  -----|------|-----
 *  0101 | 0100 | 0110
 *


* ������� ������� ����� ���� ��� ��� ����� ����� ���� 0000
* ���� ���������� � ����� ������ �� ����� 0, �� �������
* ������� ��� ���� � �� ������ �������������.
* ���� �� ��������� ���� �������� = 0, �� �������
* ��������������. �� ����� ���� � ��� � ���������� ����.
* ��� �������������� �������� ������������ ���������� ��
* ����������� � ���� ���������, � �������� ��� ����� ��
* ���������� � ������������ � ������ ������.
* ��� ���� ������������ ���������������� � ��������������
* ������ ����, ��� ��������� ���������� ���� �� ���������
* ��� ����������.
* ����� �������, ���������� �� ����� �������������.
* ���������� ����� ������� ����������� �� ����������� ���
* �������� ��� ������������ �������. ���� ��� ����������,
* �� ������� ����������� ��� ������ ������� ����.
* �� ������ ����� ���������� �������� ����� �������,
* ���������� �� ����, ���������� �� �����, ������� ��� ��
* ������� ���� ��� ��� �����������.
*
* ��������������� ��������� Code ��������� ��� ���������
* ��� ����� �������.
*
*/ 



/*******************************************************************************
* Function Name  : Verify_Clip_Line
* Description    : 
* Input          : uint16_t X0, uint16_t Y0
* Return         : FlagStatus  SET or RESET
* Output         : if line is part restrict return new X0, Y0 coordinate 
*******************************************************************************/
FlagStatus Verify_Clip_Line(uint16_t *x0, uint16_t *y0, uint16_t *x1, uint16_t *y1)
{
	uint8_t i;
	FlagStatus Line_VisibleStatus = SET;

	/* if restriction is ON for one or more objects */
	if(gClipState != DISABLE)
	{
		for(i = 0; i <= NumOfActiveClipObjects; i++)
		{
			if(ClipObjects[i].State == ENABLE)
			{
				Line_VisibleStatus = ClipObjects[i].pClipLine(x0, y0, x1, y1, i);
				
				if(NumOfActiveClipObjects == 1) return Line_VisibleStatus;	
				else if(Line_VisibleStatus == SET) return SET;
			}
		}
		
		return RESET;		
	}  
	
	return RESET;
}



static uint16_t X0, Y0; /* ���������� ������ ������� */

/*******************************************************************************
* Function Name  : CScode
* Description    : 
* Input          : uint8_t obj
* Return         : Code of virify Point
*******************************************************************************/
static __inline int32_t CScode (uint8_t objNum) /* ���������� ��� ����� x0, y0 */
{ 
	uint8_t i = 0;

	if (X0 < ClipObjects[objNum].X_Left) ++i;
	else if (X0 > ClipObjects[objNum].X_Right) i+= 2;
	
	if (Y0 < ClipObjects[objNum].Y_Down) i+= 4;
	else if (Y0 > ClipObjects[objNum].Y_Up) i+= 8;
	
	return i;
} /* CScode */



/*******************************************************************************
* Function Name  : CS_ClipLine
* Description    : 
* Input          : uint16_t X0, uint16_t Y0
* Return         : FlagStatus  SET or RESET
* Output         : if line is part restrict return new X0, Y0 coordinate 
*******************************************************************************/
FlagStatus CS_ClipLine_Out_Window(uint16_t *x0, uint16_t *y0, uint16_t *x1, uint16_t *y1, uint8_t objNum)
{
	
	uint16_t X1, Y1; 					/* ���������� ����� ������� */
	uint16_t c0, c1; 					/* ���� ������ ������� */
	FlagStatus visible = RESET; 		/* 0/1 - �� �����/�����*/
	int32_t ii, s; 						/* ������� ���������� */
	uint32_t dx, dy, 					/* ���������� ���������*/
	dxdy,dydx, 							/* ���������� ��������� ����� */
	r; 									/* ������� ���������� */	
	
	/* ���������� ���� ����� ������ ����� */
	X1= *x1; Y1= *y1;
	X0 = *x1; Y0= *y1; c1 = CScode(objNum);
	X0= *x0; Y0= *y0; c0 = CScode(objNum);	
	
	/* ����������� ���������� ��������� � �������� �������
	* � ����. ������ ����� �� ���������� ���������� �������,
	* ��������� �� ������������ �����, �������� � ����
	*/
	dx= X1 - X0;
	dy= Y1 - Y0;
	if(dx != 0) dydx = 0;
	else if(dy == 0)
	{
		if ((c0 == 0) && (c1 == 0)) goto all;
		else goto out;		
	}
	if (dy != 0) dxdy= 0;
	
	
	/* �������� ���� ��������� */
	visible = RESET;  ii= 4;
	do
	{
		if(c0 & c1)	   /* ������� ��� ���� */
		{ 			
			break;
		}			
		if(c0 == 0 && c1 == 0)		/* ������� ������ ���� */
		{ 
			visible = SET;
			break;
		}
		
		/* ���� P0 ������ ����, �� */
		if(c0 == 0)   
		{ 
			s= c0; c0= c1; c1= s; /* ����������� ���� ����� P0,P1 � �����*/
			r=X0; X0=X1; X1=r; 		/* ���, ����� P0 */
			r=Y0; Y0=Y1; Y1=r; 		/* ��������� ��� ���� */
		}
		
		/* ������ ������� �����������. P0 ���������� � �����
		* ����������� ������� �� �������� ����.
		*/
		if (c0 & 1)				/* ����������� � ����� �������� */
		{ 
			Y0= Y0 + dydx * (ClipObjects[objNum].X_Left - X0);
			X0= ClipObjects[objNum].X_Left;
		}
		else if (c0 & 2)   		/* ����������� � ������ ��������*/
		{ 
			Y0= Y0 + dydx * (ClipObjects[objNum].X_Right - X0);
			X0= ClipObjects[objNum].X_Right;
		} else if (c0 & 4)	   	/* ����������� � ������ ��������*/
		{
			X0= X0 + dxdy * (ClipObjects[objNum].Y_Down - Y0);
			Y0= ClipObjects[objNum].Y_Down;
		} else if (c0 & 8)	  	/*����������� � ������� ��������*/
		{ 
			X0= X0 + dxdy * (ClipObjects[objNum].Y_Up - Y0);
			Y0= ClipObjects[objNum].Y_Up;
		}
		c0= CScode (objNum); /* �������������� ���� ����� Pn */

	} while (--ii >= 0);
	
	if(visible == SET)
	{
		out: 
		if(X0 < X1){ *x0 = X0; *x1= X1; }
		else{ *x0 = X1; *x1= X0; }

		if(Y0 < Y1){ *y0= Y0; *y1= Y1; }
		else{ *y0= Y1; *y1= Y0; }		 
	}
	
	all:		
	return visible;
} /* V_CSclip */



/*******************************************************************************
* Function Name  : CS_ClipLine
* Description    : 
* Input          : uint16_t X0, uint16_t Y0
* Return         : FlagStatus  SET or RESET
* Output         : if line is part restrict return new X0, Y0 coordinate 
*******************************************************************************/
FlagStatus CS_ClipLine_IN_Window(uint16_t *x0, uint16_t *y0, uint16_t *x1, uint16_t *y1, uint8_t objNum)
{
	
	uint16_t X1, Y1; 					/* ���������� ����� ������� */
	uint16_t c0, c1; 					/* ���� ������ ������� */
	FlagStatus visible = SET; 		/* 0/1 - �� �����/�����*/
	int32_t ii, s; 						/* ������� ���������� */
	uint32_t dx, dy, 					/* ���������� ���������*/
	dxdy,dydx, 							/* ���������� ��������� ����� */
	r = 0; 									/* ������� ���������� */	
	
	/* ���������� ���� ����� ������ ����� */
	X1= *x1; Y1= *y1;
	X0 = *x1; Y0= *y1; c1 = CScode(objNum);
	X0= *x0; Y0= *y0; c0 = CScode(objNum);	
	
	/* ����������� ���������� ��������� � �������� �������
	* � ����. ������ ����� �� ���������� ���������� �������,
	* ��������� �� ������������ �����, �������� � ����
	*/
	dx= X1 - X0;
	dy= Y1 - Y0;
	if(dx != 0) dydx = 0;
	else if(dy == 0)
	{
		if ((c0 == 0) & (c1 == 0)) { visible = SET; goto all; }
		else{ visible = RESET; goto out; }		
	}
	if (dy != 0) dxdy= 0;
	
	
	/* �������� ���� ��������� */
	visible = SET;  ii= 4;
	do
	{
		if(c0 & c1)	   /* ������� ��� ���� */
		{ 			
			visible = RESET;
			break;
		}			
		if(c0 == 0 && c1 == 0)		/* ������� ������ ���� */
		{ 			
			visible = SET;
			break;
		}
		
		/* ���� P0 ��� ����, �� */
		if(c0 != 0)   
		{ 
			s= c0; c0= c1; c1= s;  /* ����������� ���� ����� P0,P1 � �����*/
			r=X0; X0=X1; X1=r; 		/* ���, ����� P0 */
			r=Y0; Y0=Y1; Y1=r; 		/* ��������� � ���� */
			r = 1;
		}
		
		/* ������ ������� �����������. P0 ���������� � �����
		* ����������� ������� �� �������� ����.
		*/
		if (c1 & 1)				/* ����������� � ����� �������� */
		{ 
			Y0 = Y0 + dydx * (ClipObjects[objNum].X_Left - X0);
			X0 = ClipObjects[objNum].X_Left;
		}
		else if (c1 & 2)   		/* ����������� � ������ ��������*/
		{ 
			Y0 = Y1 + dydx * (ClipObjects[objNum].X_Right - X0);
			X0 = ClipObjects[objNum].X_Right;
		} else if (c1 & 4)	   	/* ����������� � ������ ��������*/
		{
			X0 = X0 + dxdy * (ClipObjects[objNum].Y_Down - Y0);
			Y0 = ClipObjects[objNum].Y_Down;
		} else if (c1 & 8)	  	/*����������� � ������� ��������*/
		{ 
			X0 = X0 + dxdy * (ClipObjects[objNum].Y_Up - Y0);
			Y0 = ClipObjects[objNum].Y_Up;
		}
		c0 = ~CScode (objNum); /* �������������� ���� ����� Pn */

	} while (--ii >= 0);
	
	if(visible == RESET)
	{
		out: 
		if(r == 0)
		{ 
			*x0 = X0; *x1= X1;
			*y0= Y0; *y1= Y1;
		}
		else
		{
			*x0 = X1; *x1= X0;
			*y0= Y1; *y1= Y0;
		}
	}
	
	all:		
	return visible;
} /* V_CSclip */


