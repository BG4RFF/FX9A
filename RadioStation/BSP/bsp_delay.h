#ifndef __BSP_DELAY_H__
#define __BSP_DELAY_H__

#include "bsp_board.h"

void delay_init(uint8_t clk); //��ʱ������ʼ��
void delay_us(uint32_t nus);  //us����ʱ����,���65536us.
void delay_ms(uint32_t nms);  //ms����ʱ����

#endif