#include "tim_pwm.h"


void timer_config(void)
{
    /* TIMER0 configuration: generate PWM signals with different duty cycles:
       TIMER0CLK = SystemCoreClock / 72 = 1MHz */
    timer_oc_parameter_struct timer_ocintpara;
    timer_parameter_struct timer_initpara;

    rcu_periph_clock_enable(RCU_TIMER0);
		rcu_periph_clock_enable(RCU_TIMER2);
		rcu_periph_clock_enable(RCU_TIMER13);
		rcu_periph_clock_enable(RCU_GPIOA);
		rcu_periph_clock_enable(RCU_GPIOB);
    
	
    /*Configure PA8(TIMER0_CH0),PA9(TIMER0_CH1),PA10(TIMER0_CH2) as alternate function*/
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10);
    gpio_af_set(GPIOA, GPIO_AF_2, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10);
	
		/*Configure PA6(TIMER2_CH0),PA7(TIMER2_CH1) as alternate function*/
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_6|GPIO_PIN_7);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_6|GPIO_PIN_7);
    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_6|GPIO_PIN_7);
		
		/*Configure PB0(TIMER2_CH2),PB1(TIMER2_CH3) as alternate function*/
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_0|GPIO_PIN_1);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_0|GPIO_PIN_1);
    gpio_af_set(GPIOB, GPIO_AF_1, GPIO_PIN_0|GPIO_PIN_1);
		
		/*Configure PA4(TIMER13_CH0) as alternate function*/
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_4);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_4);
    gpio_af_set(GPIOA, GPIO_AF_4, GPIO_PIN_4);
		
		timer_deinit(TIMER0);
		timer_deinit(TIMER2);
		timer_deinit(TIMER13);
    /* TIMER0 configuration */
    timer_initpara.prescaler         = 71;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 2499;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER0,&timer_initpara);

     /* TIM0 CH0,CH1,CH2 configuration in PWM mode */
    timer_ocintpara.outputstate  = TIMER_CCX_ENABLE;
    timer_ocintpara.outputnstate = TIMER_CCXN_DISABLE;
    timer_ocintpara.ocpolarity   = TIMER_OC_POLARITY_HIGH;
    timer_ocintpara.ocnpolarity  = TIMER_OCN_POLARITY_HIGH;
    timer_ocintpara.ocidlestate  = TIMER_OC_IDLE_STATE_LOW;
    timer_ocintpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;
    timer_channel_output_config(TIMER0,TIMER_CH_0,&timer_ocintpara);
		timer_channel_output_config(TIMER0,TIMER_CH_1,&timer_ocintpara);
		timer_channel_output_config(TIMER0,TIMER_CH_2,&timer_ocintpara);
		
    timer_channel_output_pulse_value_config(TIMER0,TIMER_CH_0,1000);
		timer_channel_output_pulse_value_config(TIMER0,TIMER_CH_1,500);
		timer_channel_output_pulse_value_config(TIMER0,TIMER_CH_2,1500);
    timer_channel_output_mode_config(TIMER0,TIMER_CH_0,TIMER_OC_MODE_PWM0);
		timer_channel_output_mode_config(TIMER0,TIMER_CH_1,TIMER_OC_MODE_PWM0);
		timer_channel_output_mode_config(TIMER0,TIMER_CH_2,TIMER_OC_MODE_PWM0);
		
    timer_channel_output_shadow_config(TIMER0,TIMER_CH_0,TIMER_OC_SHADOW_ENABLE);
		timer_channel_output_shadow_config(TIMER0,TIMER_CH_1,TIMER_OC_SHADOW_ENABLE);
		timer_channel_output_shadow_config(TIMER0,TIMER_CH_2,TIMER_OC_SHADOW_ENABLE);

    timer_primary_output_config(TIMER0,ENABLE);
    timer_auto_reload_shadow_enable(TIMER0);
    timer_enable(TIMER0);
		
    /* TIMER2 configuration */
    timer_initpara.prescaler         = 71;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 2499;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER2,&timer_initpara);

     /* TIM2 CH0,CH1,CH2,CH3 configuration in PWM mode */
    timer_ocintpara.outputstate  = TIMER_CCX_ENABLE;
    timer_ocintpara.outputnstate = TIMER_CCXN_DISABLE;
    timer_ocintpara.ocpolarity   = TIMER_OC_POLARITY_HIGH;
    timer_ocintpara.ocnpolarity  = TIMER_OCN_POLARITY_HIGH;
    timer_ocintpara.ocidlestate  = TIMER_OC_IDLE_STATE_LOW;
    timer_ocintpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;
    timer_channel_output_config(TIMER2,TIMER_CH_0,&timer_ocintpara);
		timer_channel_output_config(TIMER2,TIMER_CH_1,&timer_ocintpara);
		timer_channel_output_config(TIMER2,TIMER_CH_2,&timer_ocintpara);
		timer_channel_output_config(TIMER2,TIMER_CH_3,&timer_ocintpara);
		
    timer_channel_output_pulse_value_config(TIMER2,TIMER_CH_0,1000);
		timer_channel_output_pulse_value_config(TIMER2,TIMER_CH_1,500);
		timer_channel_output_pulse_value_config(TIMER2,TIMER_CH_2,1500);
		timer_channel_output_pulse_value_config(TIMER2,TIMER_CH_3,2000);
		
    timer_channel_output_mode_config(TIMER2,TIMER_CH_0,TIMER_OC_MODE_PWM0);
		timer_channel_output_mode_config(TIMER2,TIMER_CH_1,TIMER_OC_MODE_PWM0);
		timer_channel_output_mode_config(TIMER2,TIMER_CH_2,TIMER_OC_MODE_PWM0);
		timer_channel_output_mode_config(TIMER2,TIMER_CH_3,TIMER_OC_MODE_PWM0);
		
    timer_channel_output_shadow_config(TIMER2,TIMER_CH_0,TIMER_OC_SHADOW_ENABLE);
		timer_channel_output_shadow_config(TIMER2,TIMER_CH_1,TIMER_OC_SHADOW_ENABLE);
		timer_channel_output_shadow_config(TIMER2,TIMER_CH_2,TIMER_OC_SHADOW_ENABLE);
		timer_channel_output_shadow_config(TIMER2,TIMER_CH_3,TIMER_OC_SHADOW_ENABLE);

    timer_primary_output_config(TIMER2,ENABLE);
    timer_auto_reload_shadow_enable(TIMER2);
    timer_enable(TIMER2);
		
    /* TIMER13 configuration */
    timer_initpara.prescaler         = 71;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 2499;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER13,&timer_initpara);

     /* TIM13 CH0 configuration in PWM mode */
    timer_ocintpara.outputstate  = TIMER_CCX_ENABLE;
    timer_ocintpara.outputnstate = TIMER_CCXN_DISABLE;
    timer_ocintpara.ocpolarity   = TIMER_OC_POLARITY_HIGH;
    timer_ocintpara.ocnpolarity  = TIMER_OCN_POLARITY_HIGH;
    timer_ocintpara.ocidlestate  = TIMER_OC_IDLE_STATE_LOW;
    timer_ocintpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;
    timer_channel_output_config(TIMER13,TIMER_CH_0,&timer_ocintpara);

    timer_channel_output_pulse_value_config(TIMER13,TIMER_CH_0,1000);

    timer_channel_output_mode_config(TIMER13,TIMER_CH_0,TIMER_OC_MODE_PWM0);
		
    timer_channel_output_shadow_config(TIMER13,TIMER_CH_0,TIMER_OC_SHADOW_ENABLE);

    timer_primary_output_config(TIMER13,ENABLE);
    timer_auto_reload_shadow_enable(TIMER13);
    timer_enable(TIMER13);
}

void timer_exti_config(void)
{
    /* -----------------------------------------------------------------------
    TIMER15 is 200KHz
    ----------------------------------------------------------------------- */
    timer_oc_parameter_struct timer_ocintpara;
    timer_parameter_struct timer_initpara;

    rcu_periph_clock_enable(RCU_TIMER15);
		nvic_irq_enable(TIMER15_IRQn,0);
//		eclic_global_interrupt_enable(1);
//		eclic_set_nlbits(ECLIC_GROUP_LEVEL3_PRIO1);
//		eclic_irq_enable(TIMER3_IRQn,1,0);
    timer_deinit(RCU_TIMER15);

    /* TIMER configuration */
    timer_initpara.prescaler         = 7199;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 99;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER15,&timer_initpara);
		
		timer_interrupt_enable(TIMER15,TIMER_INT_UP);
		timer_interrupt_flag_clear(TIMER15,TIMER_INT_FLAG_UP);
		timer_enable(TIMER15);
}

void PWM_Convert(uint16_t * data)
{
	TIMER_CH0CV(TIMER13) = *(data);
	TIMER_CH0CV(TIMER2) = *(++data);
	TIMER_CH1CV(TIMER2) = *(++data);
	TIMER_CH2CV(TIMER2) = *(++data);
	TIMER_CH3CV(TIMER2) = *(++data);
	TIMER_CH0CV(TIMER0) = *(++data);
	TIMER_CH1CV(TIMER0) = *(++data);
}

