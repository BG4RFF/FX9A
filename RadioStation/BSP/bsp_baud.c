#include "bsp_baud.h"

//NE5090���ο��ƶˣ�PB15/54/A0  PD8/55/A1  PD9/56/A2
#define A0 GPIO_Pin_15
#define A1 GPIO_Pin_8
#define A2 GPIO_Pin_9
#define NE5090_A0_PORT GPIOB
#define NE5090_A12_PORT GPIOD

//HC4028���ο��ƶˣ�PC7/64-10(A)  PC8/65-13(B)   PC9/66-12(C)   PA8/67-11(D)
#define A GPIO_Pin_7
#define B GPIO_Pin_8
#define C GPIO_Pin_9
#define D GPIO_Pin_8
#define HC4028_ABC_PORT GPIOC
#define HC4028_D_PORT GPIOA

void baud_init()
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  GPIO_InitStructure.GPIO_Pin =  A0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(NE5090_A0_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin =  A1 | A2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(NE5090_A12_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin =  A | B | C;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(HC4028_ABC_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin =  D;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(HC4028_D_PORT, &GPIO_InitStructure);
}

void baud_chn_ctrl(uint8_t numOfbaud)
{
  /*
  *      ԭ��   EN5090�����޸ģ�160M ��4��/Q0     80M��5��/Q1    60M/40M��6��/Q2    30M��9��/Q4   20M��7��/Q3  17M/15M��11��/Q6   12M/10M��10��/Q5
  * 2016.03.09  EN5090�����޸ģ�160M ��11��/Q6    80M��10��/Q5   60M/40M��9��/Q4    30M��7��/Q3   20M��6��/Q2  17M/15M��5��/Q1    12M/10M��4��/Q0
  */
  switch(numOfbaud)
  {
  case 0: // 160M   EN5090:Q6-����͵�ƽ    HC4028:Y0-����ߵ�ƽ
    GPIO_SetBits(NE5090_A12_PORT, A2);
    GPIO_SetBits(NE5090_A12_PORT, A1);
    GPIO_ResetBits(NE5090_A0_PORT, A0);
    
    GPIO_ResetBits(HC4028_D_PORT, D);
    GPIO_ResetBits(HC4028_ABC_PORT, C);
    GPIO_ResetBits(HC4028_ABC_PORT, B); 
    GPIO_ResetBits(HC4028_ABC_PORT, A); 
    break;
  case 1: // 80M      EN5090:Q5-����͵�ƽ    HC4028:Y1-����ߵ�ƽ
    GPIO_SetBits(NE5090_A12_PORT, A2);
    GPIO_ResetBits(NE5090_A12_PORT, A1);
    GPIO_SetBits(NE5090_A0_PORT, A0);
    
    GPIO_ResetBits(HC4028_D_PORT, D);
    GPIO_ResetBits(HC4028_ABC_PORT, C);
    GPIO_ResetBits(HC4028_ABC_PORT, B); 
    GPIO_SetBits(HC4028_ABC_PORT, A);
    break;
  case 2: // 60M      EN5090:Q4-����͵�ƽ    HC4028:Y8-����ߵ�ƽ
    GPIO_SetBits(NE5090_A12_PORT, A2);
    GPIO_ResetBits(NE5090_A12_PORT, A1);
    GPIO_ResetBits(NE5090_A0_PORT, A0);
     
    GPIO_SetBits(HC4028_D_PORT, D);
    GPIO_ResetBits(HC4028_ABC_PORT, C);
    GPIO_ResetBits(HC4028_ABC_PORT, B); 
    GPIO_ResetBits(HC4028_ABC_PORT, A);
    break;
  case 3: // 40M      EN5090:Q4-����͵�ƽ    HC4028:Y2-����ߵ�ƽ
    GPIO_SetBits(NE5090_A12_PORT, A2);
    GPIO_ResetBits(NE5090_A12_PORT, A1);
    GPIO_ResetBits(NE5090_A0_PORT, A0);
    
    GPIO_ResetBits(HC4028_D_PORT, D);
    GPIO_ResetBits(HC4028_ABC_PORT, C);
    GPIO_SetBits(HC4028_ABC_PORT, B); 
    GPIO_ResetBits(HC4028_ABC_PORT, A); 
    break;
  case 4: // 30M    EN5090:Q3-����͵�ƽ    HC4028:Y3-����ߵ�ƽ
    GPIO_ResetBits(NE5090_A12_PORT, A2);
    GPIO_SetBits(NE5090_A12_PORT, A1);
    GPIO_SetBits(NE5090_A0_PORT, A0);
    
    GPIO_ResetBits(HC4028_D_PORT, D);
    GPIO_ResetBits(HC4028_ABC_PORT, C);
    GPIO_SetBits(HC4028_ABC_PORT, B); 
    GPIO_SetBits(HC4028_ABC_PORT, A);
    break;
  case 5: // 20M      EN5090:Q2-����͵�ƽ    HC4028:Y4-����ߵ�ƽ
    GPIO_ResetBits(NE5090_A12_PORT, A2);
    GPIO_SetBits(NE5090_A12_PORT, A1);
    GPIO_ResetBits(NE5090_A0_PORT, A0);
    
    GPIO_ResetBits(HC4028_D_PORT, D);
    GPIO_SetBits(HC4028_ABC_PORT, C);
    GPIO_ResetBits(HC4028_ABC_PORT, B); 
    GPIO_ResetBits(HC4028_ABC_PORT, A);
    break;
  case 6: // 17M    EN5090:Q1-����͵�ƽ    HC4028:Y5-����ߵ�ƽ
    GPIO_ResetBits(NE5090_A12_PORT, A2);
    GPIO_ResetBits(NE5090_A12_PORT, A1);
    GPIO_SetBits(NE5090_A0_PORT, A0); 
    
    GPIO_ResetBits(HC4028_D_PORT, D);
    GPIO_SetBits(HC4028_ABC_PORT, C);
    GPIO_ResetBits(HC4028_ABC_PORT, B); 
    GPIO_SetBits(HC4028_ABC_PORT, A);
    break;
  case 7: // 15M    EN5090:Q1-����͵�ƽ    HC4028:Y6-����ߵ�ƽ
    GPIO_ResetBits(NE5090_A12_PORT, A2);
    GPIO_ResetBits(NE5090_A12_PORT, A1);
    GPIO_SetBits(NE5090_A0_PORT, A0); 
    
    GPIO_ResetBits(HC4028_D_PORT, D);
    GPIO_SetBits(HC4028_ABC_PORT, C);
    GPIO_SetBits(HC4028_ABC_PORT, B); 
    GPIO_ResetBits(HC4028_ABC_PORT, A);
    break;
  case 8: // 12M      EN5090:Q0-����͵�ƽ    HC4028:Y7-����ߵ�ƽ
    GPIO_ResetBits(NE5090_A12_PORT, A2);
    GPIO_ResetBits(NE5090_A12_PORT, A1);
    GPIO_ResetBits(NE5090_A0_PORT, A0);
    
    GPIO_ResetBits(HC4028_D_PORT, D);
    GPIO_SetBits(HC4028_ABC_PORT, C);
    GPIO_SetBits(HC4028_ABC_PORT, B); 
    GPIO_SetBits(HC4028_ABC_PORT, A);
    break;
  case 9: // 10M      EN5090:Q0-����͵�ƽ    HC4028:Y8-����ߵ�ƽ
    GPIO_ResetBits(NE5090_A12_PORT, A2);
    GPIO_ResetBits(NE5090_A12_PORT, A1);
    GPIO_ResetBits(NE5090_A0_PORT, A0);
    
    GPIO_SetBits(HC4028_D_PORT, D);
    GPIO_ResetBits(HC4028_ABC_PORT, C);
    GPIO_ResetBits(HC4028_ABC_PORT, B); 
    GPIO_SetBits(HC4028_ABC_PORT, A);
    break;
  }
}