#include "bsp_usart.h"
#include "bsp_kenwood.h"

#define IC_706 0x48
#define CIV_ADDR IC_706

unsigned char UartRxLen = 0;
unsigned char UartRxBuf[32];

static UartTxBufDef UTBuffer;
static bool DMA_IsReady = true;

static uint8_t Uart_Send_Buffer[64];

//uint8_t viewBuf[64];
//uint8_t viewLen = 0;

USART_InitTypeDef RadioCom = {
  9600,
  USART_WordLength_8b,
  USART_StopBits_1,
  USART_Parity_No,
  USART_Mode_Rx | USART_Mode_Tx,
  USART_HardwareFlowControl_None
};

void UartBufInit()
{
  UTBuffer.head = 0;
  UTBuffer.rear = 0;
  UTBuffer.size	= 0;
  memset(UTBuffer.dat, 0, 32);
}

void dma_init()
{
  NVIC_InitTypeDef NVIC_InitStructure;
  DMA_InitTypeDef DMA_InitStructure;
  //����DMAʱ��  
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); 
  
  //DMA�����ж�����  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;  
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
  NVIC_Init(&NVIC_InitStructure);  

  //DMA1ͨ��4����  
  DMA_DeInit(DMA1_Channel4);  
  
  //�����ַ  
  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&USART1->DR);
  
  //�ڴ��ַ  
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)Uart_Send_Buffer;

  //dma���䷽����  
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;  
  
  //����DMA�ڴ���ʱ�������ĳ���  
  DMA_InitStructure.DMA_BufferSize = 100;  
  
  //����DMA���������ģʽ��һ������  
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  
  
  //����DMA���ڴ����ģʽ  
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  
  
  //���������ֳ�  
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  
  
  //�ڴ������ֳ�  
  DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;  
  
  //����DMA�Ĵ���ģʽ  
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  
  
  //����DMA�����ȼ���  
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;  
  
  //����DMA��2��memory�еı����������  
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  
  DMA_Init(DMA1_Channel4, &DMA_InitStructure);  
  DMA_ITConfig(DMA1_Channel4, DMA_IT_TC,ENABLE);  
    
  //ʹ��ͨ��4  
  //DMA_Cmd(DMA1_Channel4, ENABLE); 
}
void usart_init()
{  
  NVIC_InitTypeDef NVIC_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;                   //���ڳ�ʼ���ṹ������
  USART_ClockInitTypeDef USART_ClockInitStruct;
  
  UartBufInit();
  
  dma_init();

  // Release reset and enable clock
  USART_DeInit(USART1);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  
  // GPIO Init
  // Enable GPIO clock and release reset
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, DISABLE);
  
  // Assign PA9 to UART1 (Tx)
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  // Assign PA10 to UART1 (Rx)GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  
  //USART_ClockInitTypeDef USART_ClockInitStruct;
  USART_InitStructure.USART_BaudRate = 115200;      //���ò�����Ϊ115200bps
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;  //����λ8λ
  USART_InitStructure.USART_StopBits = USART_StopBits_1;   //ֹͣλ1λ
  USART_InitStructure.USART_Parity = USART_Parity_No;    //��У��λ
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   //��Ӳ������
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;     //���ܺͷ���ģʽ����
  
  USART_ClockInitStruct.USART_Clock=USART_Clock_Disable;      //����ʱ�ӽ�ֹ
  USART_ClockInitStruct.USART_CPOL=USART_CPOL_Low;        //���ݵ͵�ƽ��Ч
  USART_ClockInitStruct.USART_CPHA=USART_CPHA_2Edge;    //����CPHAʹ�����ڵ�2�����ص�ʱ�򱻲���
  USART_ClockInitStruct.USART_LastBit=USART_LastBit_Disable;  // �������һλ,ʹ��Ӧ��ʱ�����岻���������SCLK����
  USART_ClockInit(USART1, &USART_ClockInitStruct);      //����USART��ʱ����ص�����
  
  // Init UART1
  USART_Init(USART1, &USART_InitStructure);
  
  // Enable and configure the priority of the UART1 Update IRQ Channel
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  USART_ClearFlag(USART1, USART_FLAG_TC  | USART_FLAG_RXNE );
  
  // Enable UART1 interrupts
  USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
  
  //����DMA��ʽ����  
  USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);  

  // Enable the UART1
  USART_Cmd(USART1, ENABLE);
}

void UartTxBufWrite(unsigned char wdat)
{
  if(UTBuffer.size < UART_TX_BUF_MAX_SIZE)
  {
    UTBuffer.dat[ UTBuffer.rear++ ] = wdat;
    if(UTBuffer.rear >= UART_TX_BUF_MAX_SIZE)
    {
      UTBuffer.rear = 0;
    }
    UTBuffer.size ++;	
  }
}

void UartTx(uint8_t *wdat, uint8_t len)
{
  for(uint8_t i=0; i<len; i++)
  {
    UartTxBufWrite(wdat[i]);  
  }
}

unsigned char UartTxBufRead()
{
  unsigned char rval = 0;
  if(UTBuffer.size > 0)
  {
    rval = UTBuffer.dat[ UTBuffer.head++ ];
    if(UTBuffer.head >= UART_TX_BUF_MAX_SIZE)
    {
      UTBuffer.head = 0;
    }							
    UTBuffer.size --; 
  }
  
  return rval;
}

unsigned char UART_TxBufSize()
{
  return UTBuffer.size;
}

void usart_rx_analysis(uint8_t wdat)
{
  if(wdat == ';')
  {
    Kenwood_CmdParser(UartRxBuf, UartRxLen);
    UartRxLen = 0;
  }
  else
  {
    UartRxBuf[ UartRxLen++ ] = wdat;
  }
}

bool DMA_IsTxComplete()
{
  return DMA_IsReady;
}

void CopyTBufToDMA()
{
  uint8_t len = UTBuffer.size;
  for(uint8_t i=0; i<len; i++)
  {
    Uart_Send_Buffer[i] = UartTxBufRead();
  }
}

void UART_DMAStart(uint8_t length)
{
  DMA_IsReady = 0;
  DMA_ClearFlag(DMA1_FLAG_TC4);      //��DMA������ɱ�־
  DMA_Cmd(DMA1_Channel4, DISABLE);   //ֹͣDMA
  
  //���ô������ݳ���  
  DMA1_Channel4->CNDTR = length;      //���贫�䳤��
  //��DMA  
  DMA_Cmd(DMA1_Channel4,ENABLE);  
}

void USART1_IRQHandler( void )
{
  uint8_t dat;
  
  if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
  {   
    dat = USART_ReceiveData(USART1);
    //USART_ClearITPendingBit(USART1, USART_IT_RXNE);

    usart_rx_analysis(dat);  
  }
  

}

  //����1DMA��ʽ�����ж�  
void DMA1_Channel4_IRQHandler(void)  
{  
  //�����־λ  
  DMA_ClearFlag(DMA1_FLAG_TC4);  
  
  //�ر�DMA  
  DMA_Cmd(DMA1_Channel4,DISABLE);  

  //�����ٴη���  
  DMA_IsReady = 1;  
} 