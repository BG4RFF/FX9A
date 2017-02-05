#include "bsp_transmit_power.h"

//���书�ʿ��ƣ���3�������ʣ��й��ʣ�С���ʣ��ڲ˵����3�H/M/L   H/0-60  M/0-60  L/0-60) 
//PE14/45/CS   PE15/46/SDI   PB10/47/CLK

#define TP_CS   GPIO_Pin_14
#define TP_SDI  GPIO_Pin_15
#define TP_CLK  GPIO_Pin_10
#define TP_CS_SDI_PORT GPIOE
#define TP_CLK_PORT GPIOB

// PE11/42��  TX������ƶˣ�������SSBģʽ����CWģʽ���ڷ���ʱ����ߵ�ƽ��
#define TX_CONTROL   GPIO_Pin_11
// PB11/48��    �������ƣ��ڷ���ʱ����ߵ�ƽ
// PB12/51��    MIC���ƣ���CWģʽ�·���ʱ����͵�ƽ��RXʱҲ����͵�ƽ��ֻ����SSBģʽ������ʱ����ߵ�ƽ
#define Quiet GPIO_Pin_11
#define MIC GPIO_Pin_12

//PB13/52��    CW�˲����л����ƣ���SSBģʽ��Ĭ�ϵ͵�ƽ����CWģʽ�� RXʱĬ�ϸߵ�ƽ��N խ������
//         ��CMģʽ ����ʱ Ϊ�͵�ƽ��W �����
//PB14/53��    BFO�л� CW����״̬�£�����ߵ�ƽ; �������ȫΪ�͵�ƽ
#define CW_FIL_CONTROL GPIO_Pin_13
#define CW_BFO_CONTROL GPIO_Pin_14

void tp_init()
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  GPIO_InitStructure.GPIO_Pin =  TP_CS | TP_SDI | TX_CONTROL;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(TP_CS_SDI_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin =  TP_CLK | Quiet | MIC | CW_FIL_CONTROL | CW_BFO_CONTROL;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(TP_CLK_PORT, &GPIO_InitStructure);
  
}

void CW_FilterControl(uint8_t high)
{
  if( high )
  {
    GPIO_SetBits(TP_CLK_PORT, CW_FIL_CONTROL);
  }
  else
  {
    GPIO_ResetBits(TP_CLK_PORT, CW_FIL_CONTROL); 
  }
}

void CW_BFOControl(uint8_t high)
{
  if( high )
  {
    GPIO_SetBits(TP_CLK_PORT, CW_BFO_CONTROL);
  }
  else
  {
    GPIO_ResetBits(TP_CLK_PORT, CW_BFO_CONTROL); 
  }
}
void QuietModeOn()
{
  GPIO_SetBits(TP_CLK_PORT, Quiet); 
}

void QuietModeOff()
{
  GPIO_ResetBits(TP_CLK_PORT, Quiet); 
}

void MICControlOn()
{
  GPIO_SetBits(TP_CLK_PORT, MIC); 
}

void MICControlOff()
{
  GPIO_ResetBits(TP_CLK_PORT, MIC); 
}

void tx_control_on()
{
  GPIO_SetBits(TP_CS_SDI_PORT, TX_CONTROL); 
}

void tx_control_off()
{
  GPIO_ResetBits(TP_CS_SDI_PORT, TX_CONTROL); 
}

void tp_control(uint8_t data)
{
  GPIO_ResetBits(TP_CLK_PORT, TP_CLK);  
  GPIO_SetBits(TP_CLK_PORT, TP_CLK); 
  
  GPIO_ResetBits(TP_CS_SDI_PORT, TP_CS);

  for(uint8_t i=0; i<8; i++)
  {
    GPIO_ResetBits(TP_CLK_PORT, TP_CLK); 
    
    if(data & 0x80)
    {
      GPIO_SetBits(TP_CS_SDI_PORT, TP_SDI);  
    }else
    {
      GPIO_ResetBits(TP_CS_SDI_PORT, TP_SDI);  
    }
    data <<= 1;
    
   // delay_us(1);
    GPIO_SetBits(TP_CLK_PORT, TP_CLK); 
   // delay_us(1);
  }
  
  GPIO_SetBits(TP_CS_SDI_PORT, TP_CS); 
  GPIO_ResetBits(TP_CLK_PORT, TP_CLK);  
  GPIO_SetBits(TP_CLK_PORT, TP_CLK); 
  GPIO_ResetBits(TP_CLK_PORT, TP_CLK);  
}
