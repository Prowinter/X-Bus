//////////////////////////////////////////////////////////////////////////////////	 
//I2C——HAL库初始化程序     By:Prowinter
//us级延时采用TIM定时提供 
//内含寄存器操作,具体修改请参考数据手册
//不会修改请注释相关内容转换为Hal库操作
////////////////////////////////////////////////////////////////////////////////// 	
#ifndef __TIM_PWM_H__
#define __TIM_PWM_H__
#include "gd32e23x.h"
		 
void timer_config(void);
void PWM_Convert(uint16_t * data);
void timer_exti_config(void);
#endif



