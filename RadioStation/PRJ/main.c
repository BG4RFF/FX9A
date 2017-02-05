#include "bsp_board.h"
#include "bsp_timer.h"

void RCC_Configuration(void)
{
  RCC_DeInit();
 
  RCC_HSICmd(ENABLE);
 
  while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET)
  {
 
  } 
 
  if(1)
  { 
   //FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
  
   //FLASH_SetLatency(FLASH_Latency_2);
  
   RCC_HCLKConfig(RCC_SYSCLK_Div1); //AHBʱ��Ϊϵͳʱ��SYSCLK
  
   RCC_PCLK2Config(RCC_HCLK_Div1);  //APB1ʱ��ΪHCLK/2������HCLKΪAHBʱ��
  
   RCC_PCLK1Config(RCC_HCLK_Div2);  //APB2ʱ��ΪHCLK
  
   //���� PLL ʱ��Դ����Ƶϵ��
  
   RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_8);
  
   //ʹ�ܻ���ʧ�� PLL,�����������ȡ��ENABLE����DISABLE
  
   RCC_PLLCmd(ENABLE);//���PLL������ϵͳʱ��,��ô�����ܱ�ʧ��
  
   //�ȴ�ָ���� RCC ��־λ���óɹ� �ȴ�PLL��ʼ���ɹ�
  
   while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
   {
  
   }
   //����ϵͳʱ�ӣ�SYSCLK�� ����PLLΪϵͳʱ��Դ  
   RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
   //�ȴ�PLL�ɹ�������ϵͳʱ�ӵ�ʱ��Դ
     
   // 0x00��HSI ��Ϊϵͳʱ��
  
   // 0x04��HSE��Ϊϵͳʱ��
  
   // 0x08��PLL��Ϊϵͳʱ��
  
   while(RCC_GetSYSCLKSource() != 0x08)
   {
  
   }
  
   }
//   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
//        RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
  
//   RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1 | RCC_APB1Periph_I2C2, ENABLE);
  
//   RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
}

void main()
{ 
  /* Setup STM32 system (clock, PLL and Flash configuration) */
  SystemInit(); // 24MHz����system_stm32f10x.c�����߶�����SYSCLK_FREQ_24MHZ�꣬BG6RDF
  //RCC_Configuration();
  board_init();
  
  TMR_Start(TIM2);
  while(1)
  {
    board_task();
  }
}
