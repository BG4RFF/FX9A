#include "bsp_adc.h"

/*
  15��  PC0-ADC12_IN10  �����ѹ�������ˣ���ʾ�����ѹ����5K��1K�����ѹ���ã�
  17��  PC1-ADC12_IN11  ���书�ʵ�ѹ���ˣ���ѹֵ��0-3V�������ο���
  16��  PC2-ADC12_IN12  �����ʵ�ѹ���Σ���ѹֵ��0-3V�������ο���
  18��  PC3-ADC12_IN13  ���յ�ƽ��ѹ���Σ���ѹֵ��0-3V�������ο���
*/
#define BATT_CHK GPIO_Pin_0
#define SWR_CHK GPIO_Pin_1
#define RF_CHK GPIO_Pin_2
#define RX_CHK GPIO_Pin_3

#define ADC1_DR_Address    ((uint32_t)0x4001244C)

uint16_t ADC_Value[16];

void ADC_Config(void)
{
  DMA_InitTypeDef DMA_InitStructure;
  DMA_DeInit(DMA1_Channel1);
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;   //���� DMAͨ���������ַ=ADC1_DR_Address
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADC_Value;       //����DMAͨ���洢����ַ
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;            //ָ������ΪԴ��ַ
  DMA_InitStructure.DMA_BufferSize = 16;                        //����DMA��������С
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;    //��ǰ����Ĵ�����ַ����
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;       //��ǰ�洢����ַ����
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //�����������ݿ��16λ
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //����洢�����ݿ��16λ
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;               //DMAͨ������ģʽΪ���λ���ģʽ
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;           //DMAͨ�����ȼ���
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                  //��ֹDMAͨ���洢�����洢������
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
  DMA_Cmd(DMA1_Channel1, ENABLE);
  
  ADC_InitTypeDef ADC_InitStructure;                  //����ADC��ʼ���ṹ�����
  ADC_DeInit(ADC1);
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;  //ADC1��ADC2�����ڶ���ģʽ
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;        //ʹ��ɨ��
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;  //ADCת�������ڵ���ģʽ
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //�������ת��
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//ת�������Ҷ���
  ADC_InitStructure.ADC_NbrOfChannel = 4;//ɨ��ͨ���� 
  ADC_Init(ADC1, &ADC_InitStructure); //��ʼ��ADC
  
  ADC_RegularChannelConfig(ADC1, ADC_Channel_10,  1, ADC_SampleTime_239Cycles5); //BATT
  ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 2, ADC_SampleTime_239Cycles5); 
  ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 3, ADC_SampleTime_239Cycles5);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 4, ADC_SampleTime_239Cycles5);
  //ADC1ѡ���ŵ�14,�������ȼ�1,����ʱ��239.5������
  ADC_DMACmd(ADC1, ENABLE);//ʹ��ADC1ģ��DMA
  ADC_Cmd(ADC1, ENABLE);//ʹ��ADC1
  
  ADC_ResetCalibration(ADC1); //����ADC1У׼�Ĵ���
  while(ADC_GetResetCalibrationStatus(ADC1));//�ȴ�ADC1У׼�������
  ADC_StartCalibration(ADC1);//��ʼADC1У׼
  while(ADC_GetCalibrationStatus(ADC1));//�ȴ�ADC1У׼���
  ADC_SoftwareStartConvCmd(ADC1, ENABLE); //ʹ��ADC1�����ʼת��
}

//PC0/15/ADC12_IN10
void adc_init()
{
  GPIO_InitTypeDef GPIO_InitStructure; //����GPIO��ʼ���ṹ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOC, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//ʹ��DMA����
  
  //����PC0Ϊģ������
  GPIO_InitStructure.GPIO_Pin = BATT_CHK | SWR_CHK | RF_CHK | RX_CHK;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
//  NVIC_InitTypeDef NVIC_InitStructure;  
//  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
//  
//  NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn; 
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
//  NVIC_Init(&NVIC_InitStructure);
  
  ADC_Config(); 
  
//  ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
}

uint16_t adc_batt()
{
  return (ADC_Value[0] + ADC_Value[4] + ADC_Value[8] + ADC_Value[12]) / 4;
}

uint16_t adc_rf()
{
  return (ADC_Value[1] + ADC_Value[5] + ADC_Value[9] + ADC_Value[13]) / 4;
}

uint16_t adc_swr()
{
  return (ADC_Value[2] + ADC_Value[6] + ADC_Value[10] + ADC_Value[14]) / 4;
}

uint16_t adc_rx()
{
  return (ADC_Value[3] + ADC_Value[7] + ADC_Value[11] + ADC_Value[15]) / 4;
}

void adc_start()
{
  ADC_SoftwareStartConvCmd(ADC1, ENABLE); //ʹ��ADC1�����ʼת��
}
void ADC1_2_IRQHandler()
{
  if (ADC_GetITStatus(ADC1, ADC_IT_EOC) == SET)
  {
    DC.val = ADC_GetConversionValue(ADC1);
    DRMgr.item |= DCItem;
    ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);
  }
}