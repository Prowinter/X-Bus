/*!
    \file  main.c
    \brief running LED
    
    \version 2019-02-19, V1.0.0, firmware for GD32E23x
*/

/*
    Copyright (c) 2019, GigaDevice Semiconductor Inc.

    All rights reserved.

    Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this 
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, 
       this list of conditions and the following disclaimer in the documentation 
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors 
       may be used to endorse or promote products derived from this software without 
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE.
*/

#include "gd32e23x.h"
#include "systick.h"
#include <stdio.h>
#include "tim_pwm.h"
#include "nrf24l01.h"
/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
uint8_t receive_tem_data[32]={0};
uint16_t Remote_data[16]={0};
uint8_t sbus_data[25]={0};
char isSignalLoss;
char isFailsafe;

void com_usart_init(void)
{
    /* enable USART and GPIO clock */
    rcu_periph_clock_enable(RCU_USART0);
		rcu_periph_clock_enable(RCU_GPIOA);

    /* connect port to USARTx_Tx */
    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_2);

    /* connect port to USARTx_Rx */
    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_3);

    /* configure USART Tx as alternate function push-pull */
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_2);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_2);

    /* configure USART Rx as alternate function push-pull */
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_3);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_3);
	
    /* USART configure */
    usart_deinit(USART0);
    usart_word_length_set(USART0, USART_WL_9BIT);
    usart_stop_bit_set(USART0, USART_STB_2BIT);
    usart_parity_config(USART0, USART_PM_EVEN);
    usart_baudrate_set(USART0, 100000U);
    usart_receive_config(USART0, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
    usart_enable(USART0);
}

int fputc(int ch, FILE *f)
{
    usart_data_transmit(USART0, (uint8_t) ch);
    while(RESET == usart_flag_get(USART0, USART_FLAG_TBE));
    return ch;
}

void TIMER15_IRQHandler(void)
{
		uint8_t j=0;
    if(RESET != timer_interrupt_flag_get(TIMER15,TIMER_INT_FLAG_UP)){
		sbusPreparePacket(sbus_data,Remote_data, isSignalLoss, isFailsafe);
		for(j=0;j<25;j++)
		{
			while ((USART_STAT(USART0) & USART_STAT_TBE) == RESET);
			USART_TDATA(USART0) = (uint8_t) sbus_data[j];
		}
			timer_interrupt_flag_clear(TIMER15,TIMER_INT_FLAG_UP);
    }
}

int main(void)
{
		uint8_t i=0,k=0;
		uint8_t tem = 0;
		uint8_t status;
    systick_config();
    uint8_t test=0x88;
    /* enable the LED1 GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOA);
    /* configure LED1 GPIO port */ 
    gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_PIN_0|GPIO_PIN_1);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_0|GPIO_PIN_1);
		gpio_bit_reset(GPIOA,GPIO_PIN_1|GPIO_PIN_0);
		com_usart_init();
		timer_config();
		timer_exti_config();
		delay_us_config();
		spi0_init();
		NRf24L01_Init();
		Rx_Mode();
    while(1){
			if(i>10)
			{
				gpio_bit_toggle(GPIOA, GPIO_PIN_0);
				i=0;
			}
			if(NRf24L01_RePacket((uint8_t *)receive_tem_data,32)==NRF_RX_DR)
			{
					i++;
					Data_Convert(receive_tem_data,Remote_data);
					PWM_Convert(Remote_data);
			}
    }
}
