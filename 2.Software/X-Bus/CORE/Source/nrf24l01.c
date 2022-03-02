#include "nrf24l01.h"

const uint8_t TxAddr_data[5]={0x11,0x22,0x33,0x44,0x66};
const uint8_t RxAddr_data[5]={0x11,0x22,0x33,0x44,0x66};

void spi0_init(void)
{
    spi_parameter_struct spi_init_struct;

    /* enable the gpio clock */
    rcu_periph_clock_enable(RCU_GPIOA);
		rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_SPI0);

    /* GPIOB config, PB3(SPI0_SCK), PB4(SPI0_MISO), PB5(SPI0_MOSI) */
    gpio_af_set(GPIOB, GPIO_AF_0, GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5);
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5);

    /* GPIOA config */
		gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, GPIO_PIN_15);
		gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_15);
		
		gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, GPIO_PIN_5);
		gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_5);
	
		gpio_mode_set(GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, GPIO_PIN_6);
	
    /* SPI0 parameter config */
    spi_init_struct.trans_mode           = SPI_TRANSMODE_FULLDUPLEX;
    spi_init_struct.device_mode          = SPI_MASTER;
    spi_init_struct.frame_size           = SPI_FRAMESIZE_8BIT;
    spi_init_struct.clock_polarity_phase = SPI_CK_PL_LOW_PH_1EDGE;
    spi_init_struct.nss                  = SPI_NSS_SOFT;
    spi_init_struct.prescale             = SPI_PSC_8;
    spi_init_struct.endian               = SPI_ENDIAN_MSB;
    spi_init(SPI0, &spi_init_struct);

    /* set crc polynomial */
    spi_enable(SPI0);
}


void delay_us_config(void)
{
    timer_parameter_struct timer_initpara;
		rcu_periph_clock_enable(RCU_TIMER5);
    timer_deinit(TIMER5);
    /* TIMER5 configuration */
    timer_initpara.prescaler         = 71;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 65535;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER5,&timer_initpara);

    timer_auto_reload_shadow_disable(TIMER5);
    /* auto-reload preload enable */
    timer_disable(TIMER5);
}

void delay_us(unsigned short int nus)
{
		timer_counter_value_config(TIMER5,0);
		timer_enable(TIMER5);
		while(timer_counter_read(TIMER5)<nus);
		timer_disable(TIMER5);
}

uint8_t SPI_RW(uint8_t byte)
{
    while(RESET == (SPI_STAT(SPI0)&SPI_FLAG_TBE));
    SPI_DATA(SPI0) = byte;
    while(RESET == (SPI_STAT(SPI0)&SPI_FLAG_RBNE));
    return(SPI_DATA(SPI0));
} 


uint8_t NRf24L01_Write_READ_Data(uint8_t addr,uint8_t write_data)
{
    uint8_t read_data;
    NRF_CS_RESET();
    SPI_RW(addr);
    read_data = SPI_RW(write_data);
    NRF_CS_SET();
    return read_data;
}

uint8_t NRf24L01_WriteBuf(uint8_t addr, uint8_t *pBuf, uint8_t len)
{
	uint8_t status;    
	NRF_CS_RESET();
	status = SPI_RW(addr);
	for(uint8_t i=0; i<len; i++)	
    {
		SPI_RW(pBuf[i]);		
	}
	NRF_CS_SET();
	return status;
}
 
uint8_t NRf24L01_ReadBuf(uint8_t addr, uint8_t *pBuf, uint8_t len)
{
	uint8_t status;	
	NRF_CS_RESET();
	status = SPI_RW(addr);
	for(uint8_t i = 0; i<len ;i++)
    {
		pBuf[i] = SPI_RW(0xFF);		
	}
	NRF_CS_SET();
	return status;
}

void NRf24L01_Init(void)
{
    NRF_CE_RESET();
    NRf24L01_Write_READ_Data(NRF_WRITE_REG + NRF_RF_CH,110);                    //  设置射频通道
    NRf24L01_Write_READ_Data(NRF_WRITE_REG + NRF_SETUP_AW,0x03);                //  设置地址长度
    NRf24L01_Write_READ_Data(NRF_WRITE_REG + NRF_RF_SETUP,0x06);                //  1Mbps,0dBm,No Lna
    NRf24L01_Write_READ_Data(NRF_WRITE_REG + NRF_SETUP_RETR,0x1f);              //  设置重传延时，重传次数500us,15
    NRf24L01_Write_READ_Data(NRF_FLUSH_TX,0xff);                                //  清除TX_FIFO数据
    NRf24L01_Write_READ_Data(NRF_FLUSH_RX,0xff);                                //  清除RX_FIFO数据
    NRf24L01_Write_READ_Data(NRF_WRITE_REG + NRF_STATUS,0x7E);                  //  清空状态寄存器数据
    NRf24L01_Write_READ_Data(NRF_WRITE_REG + NRF_RX_PW_P0,32);                  //  选择通道0的有效数据宽度
    NRf24L01_Write_READ_Data(NRF_WRITE_REG + NRF_EN_AA,0x01);                   //  使能通道0的自动应答
    NRf24L01_Write_READ_Data(NRF_WRITE_REG + NRF_CONFIG,0x02);                  //  切换到Start Up模式
    delay_1ms(2);
}

void Rx_Mode(void)
{
    NRF_CE_RESET();
    NRf24L01_Write_READ_Data(NRF_WRITE_REG + NRF_CONFIG,0x2f);    //PWR_UP,PRIM_RX使能 
		NRf24L01_WriteBuf(NRF_WRITE_REG + NRF_TX_ADDR,(uint8_t *)TxAddr_data,5);    
    NRf24L01_WriteBuf(NRF_WRITE_REG + NRF_RX_ADDR_P0,(uint8_t *)TxAddr_data,5); 
    NRf24L01_Write_READ_Data(NRF_WRITE_REG + NRF_EN_RXADDR,0x01);
    NRf24L01_Write_READ_Data(NRF_WRITE_REG + NRF_EN_AA,0x01);
    NRF_CE_SET();
}

void Tx_Mode(void)
{
    NRf24L01_Write_READ_Data(NRF_WRITE_REG + NRF_CONFIG,0x0E);    //PWR_UP,PRIM_RX使能
    NRf24L01_WriteBuf(NRF_WRITE_REG + NRF_TX_ADDR,(uint8_t *)TxAddr_data,5);    
    NRf24L01_WriteBuf(NRF_WRITE_REG + NRF_RX_ADDR_P0,(uint8_t *)TxAddr_data,5); 
    NRf24L01_Write_READ_Data(NRF_WRITE_REG + NRF_EN_AA,0x01);
    NRf24L01_Write_READ_Data(NRF_WRITE_REG + NRF_CONFIG,0x0E);
}

//uint8_t NRf24L01_TxPacket(uint8_t *txbuf,uint8_t len)
//{
//    uint8_t status;
//    NRF_CE_RESET();
//    NRf24L01_WriteBuf(NRF_WR_TX_PLOAD,txbuf,len);   // 写数据到TX_BUFF    
//    // NRf24L01_Write_READ_Data(NRF_WRITE_REG + NRF_CONFIG,0x0E);	             // 设置为发送模式,开启所有中断	    
//    NRF_CE_SET();
//    while(NRF_IRQ_READ());
//    NRf24L01_Write_READ_Data(NRF_FLUSH_TX,0xff); 
//    status = NRf24L01_Write_READ_Data(NRF_READ_REG + NRF_STATUS,0xff);
//    NRf24L01_Write_READ_Data(NRF_WRITE_REG + NRF_STATUS,status);
//    if(status&NRF_MAX_RT)
//    {
//        return NRF_MAX_RT;
//    }
//    else if(status&NRF_TX_DS)
//    {
//        return NRF_TX_DS;
//    }
//    return 0xff;
//}

uint8_t NRf24L01_RePacket(uint8_t *txbuf,uint8_t len)
{
    uint8_t status,s=0;
		uint16_t RTD_wait = 0;
    NRF_CE_SET();
		while(gpio_input_bit_get(GPIOB,GPIO_PIN_6)==SET)
		{
			delay_us(1);
			RTD_wait++;
			if(RTD_wait>=1000)
			{
				s = 1;
				break;
			}
		}
		if (s==0)
		{
			NRF_CE_RESET();
			status = NRf24L01_Write_READ_Data(NRF_READ_REG + NRF_STATUS,0xff);
			if(status&NRF_RX_DR)
			{
					NRf24L01_Write_READ_Data(NRF_WRITE_REG + NRF_STATUS,status|NRF_RX_DR);
					NRf24L01_ReadBuf(NRF_RD_RX_PLOAD,txbuf,len);
					NRf24L01_Write_READ_Data(NRF_FLUSH_RX,0xff); 
					return NRF_RX_DR;   //接收成功
			}
		}
    return 0xff;    //其他原因失败

}

// 自检函数 自检通过返回1，自检不通过返回0.
uint8_t NRf24L01_Check(void)
{
	uint8_t buf[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
	uint8_t i;
	NRf24L01_WriteBuf(NRF_WRITE_REG + NRF_TX_ADDR,buf,5);
	NRf24L01_ReadBuf(NRF_READ_REG + NRF_TX_ADDR,buf,5);
	for(i=0; i<5; i++)if(buf[i]!=0xA5)break;
	if(i!=5)return 0;
	return 1;
}

void Data_Convert(uint8_t * pre_data,uint16_t * data)
{
	for(uint8_t i=0;i<=15;i++)
	{
		*(data+i) = (uint16_t) (*(pre_data+i*2))<<8 | (*(pre_data+i*2+1));
		*(data+i) = Limit_data(*(data+i));
	}
}



uint16_t Limit_data(uint16_t data)
{
	return data>Max_data?Max_data:data<Min_data?Min_data:data;
}


void sbusPreparePacket(uint8_t packet[],uint16_t output[], char isSignalLoss, char isFailsafe)
{
    for (uint8_t i = 0; i < 16; i++) {
        output[i] = (int)map(output[i], RC_CHANNEL_MIN, RC_CHANNEL_MAX, SBUS_MIN_OFFSET, SBUS_MAX_OFFSET);
    }
		
    uint8_t stateByte = 0x00;
    if (isSignalLoss) {
        stateByte |= SBUS_STATE_SIGNALLOSS; // 丢失信号标志
    }
    if (isFailsafe) {
        stateByte |= SBUS_STATE_FAILSAFE;   // 激活故障安全标志
    }
    packet[0] = 0x0F;
    packet[1] = (uint8_t) (output[0] & 0x07FF);
    packet[2] = (uint8_t) ((output[0] & 0x07FF)>>8 | (output[1] & 0x07FF)<<3);
    packet[3] = (uint8_t) ((output[1] & 0x07FF)>>5 | (output[2] & 0x07FF)<<6);
    packet[4] = (uint8_t) ((output[2] & 0x07FF)>>2);
    packet[5] = (uint8_t) ((output[2] & 0x07FF)>>10 | (output[3] & 0x07FF)<<1);
    packet[6] = (uint8_t) ((output[3] & 0x07FF)>>7 | (output[4] & 0x07FF)<<4);
    packet[7] = (uint8_t) ((output[4] & 0x07FF)>>4 | (output[5] & 0x07FF)<<7);
    packet[8] = (uint8_t) ((output[5] & 0x07FF)>>1);
    packet[9] = (uint8_t) ((output[5] & 0x07FF)>>9 | (output[6] & 0x07FF)<<2);
    packet[10] = (uint8_t) ((output[6] & 0x07FF)>>6 | (output[7] & 0x07FF)<<5);
    packet[11] = (uint8_t) ((output[7] & 0x07FF)>>3);
    packet[12] = (uint8_t) ((output[8] & 0x07FF));
    packet[13] = (uint8_t) ((output[8] & 0x07FF)>>8 | (output[9] & 0x07FF)<<3);
    packet[14] = (uint8_t) ((output[9] & 0x07FF)>>5 | (output[10] & 0x07FF)<<6);  
    packet[15] = (uint8_t) ((output[10] & 0x07FF)>>2);
    packet[16] = (uint8_t) ((output[10] & 0x07FF)>>10 | (output[11] & 0x07FF)<<1);
    packet[17] = (uint8_t) ((output[11] & 0x07FF)>>7 | (output[12] & 0x07FF)<<4);
    packet[18] = (uint8_t) ((output[12] & 0x07FF)>>4 | (output[13] & 0x07FF)<<7);
    packet[19] = (uint8_t) ((output[13] & 0x07FF)>>1);
    packet[20] = (uint8_t) ((output[13] & 0x07FF)>>9 | (output[14] & 0x07FF)<<2);
    packet[21] = (uint8_t) ((output[14] & 0x07FF)>>6 | (output[15] & 0x07FF)<<5);
    packet[22] = (uint8_t) ((output[15] & 0x07FF)>>3);
    packet[23] = 0x00;         // 标志位
    packet[24] = 0x00; // SBUS结束字节
}

/*函数：float map(float value,float fromLow,float fromHigh,float toLow,float toHigh)
* 说明：仿Arduino,将一个数字从一个范围重新映射到另一个范围
		也就是说，fromLow的值将映射到toLow，fromlhigh到toHigh的值等等。
* 参数：value：待映射的数值；
		fromLow：原范围的最小值
		fromHigh：原范围的最大值
		toLow：要转换的范围的最小值
		toHigh：要转换的范围的最大值
* 返回：转换后的数值
*/
float map(float value,float fromLow,float fromHigh,float toLow,float toHigh)
{
	return ((value-fromLow)*(toHigh-toLow)/(fromHigh-fromLow)+toLow);
}

