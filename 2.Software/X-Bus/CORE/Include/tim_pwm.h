//////////////////////////////////////////////////////////////////////////////////	 
//I2C����HAL���ʼ������     By:Prowinter
//us����ʱ����TIM��ʱ�ṩ 
//�ں��Ĵ�������,�����޸���ο������ֲ�
//�����޸���ע���������ת��ΪHal�����
////////////////////////////////////////////////////////////////////////////////// 	
#ifndef __TIM_PWM_H__
#define __TIM_PWM_H__
#include "gd32e23x.h"
		 
void timer_config(void);
void PWM_Convert(uint16_t * data);
void timer_exti_config(void);
#endif



