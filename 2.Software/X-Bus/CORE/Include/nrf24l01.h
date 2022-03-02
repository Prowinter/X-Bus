#ifndef __NRF24L01_H__
#define __NRF24L01_H__
#include "gd32e23x.h"
#include "systick.h"
//#include "myiic.h"


#define SBUS_STATE_FAILSAFE 0x08
#define SBUS_STATE_SIGNALLOSS 0x04
#define RC_CHANNEL_MIN 1000 	//每个通道的PWM最小值
#define RC_CHANNEL_MAX 2000 	//每个通道的PWM最大值
//#define SBUS_MIN_OFFSET 173 	//SBUS协议的最小值
//#define SBUS_MID_OFFSET 992 	//SBUS协议的中间值
//#define SBUS_MAX_OFFSET 1811 	//SBUS协议的最大值
#define SBUS_MIN_OFFSET 0 	//SBUS协议的最小值
#define SBUS_MID_OFFSET 1000 	//SBUS协议的中间值
#define SBUS_MAX_OFFSET 2000 	//SBUS协议的最大值


// Define nRF24L01 interrupt flag's
#define NRF_IDLE            0x00  // Idle, no interrupt pending
#define NRF_MAX_RT          0x10  // Max #of TX retrans interrupt
#define NRF_TX_DS           0x20  // TX data sent interrupt
#define NRF_RX_DR           0x40  // RX data received

#define NRF_SPI_CFG         0x40  // SPI Configuration register value
#define NRF_SPI_CTR         0x01  // SPI Control register values
#define NRF_SPI_CLK         0x00  // SYSCLK/2*(SPI_CLK+1) == > 12MHz / 2 = 6MHz
#define NRF_SPI0E           0x02  // SPI Enable in XBR0 register

//********************************************************************************************************************//
// SPI(nRF24L01) commands
#define NRF_READ_REG        0x00  // Define read command to register
#define NRF_WRITE_REG       0x20  // Define write command to register
#define NRF_RD_RX_PLOAD     0x61  // Define RX payload register address
#define NRF_WR_TX_PLOAD     0xA0  // Define TX payload register address
#define NRF_FLUSH_TX        0xE1  // Define flush TX register command
#define NRF_FLUSH_RX        0xE2  // Define flush RX register command
#define NRF_REUSE_TX_PL     0xE3  // Define reuse TX payload register command
#define NRF_NOP             0xFF  // Define No Operation, might be used to read status register

//********************************************************************************************************************//
// SPI(nRF24L01) registers(addresses)
#define NRF_CONFIG          0x00  // 'Config' register address
#define NRF_EN_AA           0x01  // 'Enable Auto Acknowledgment' register address
#define NRF_EN_RXADDR       0x02  // 'Enabled RX addresses' register address
#define NRF_SETUP_AW        0x03  // 'Setup address width' register address
#define NRF_SETUP_RETR      0x04  // 'Setup Auto. Retrans' register address
#define NRF_RF_CH           0x05  // 'RF channel' register address
#define NRF_RF_SETUP        0x06  // 'RF setup' register address
#define NRF_STATUS          0x07  // 'Status' register address
#define NRF_OBSERVE_TX      0x08  // 'Observe TX' register address
#define NRF_CD              0x09  // 'Carrier Detect' register address
#define NRF_RX_ADDR_P0      0x0A  // 'RX address pipe0' register address
#define NRF_RX_ADDR_P1      0x0B  // 'RX address pipe1' register address
#define NRF_RX_ADDR_P2      0x0C  // 'RX address pipe2' register address
#define NRF_RX_ADDR_P3      0x0D  // 'RX address pipe3' register address
#define NRF_RX_ADDR_P4      0x0E  // 'RX address pipe4' register address
#define NRF_RX_ADDR_P5      0x0F  // 'RX address pipe5' register address
#define NRF_TX_ADDR         0x10  // 'TX address' register address
#define NRF_RX_PW_P0        0x11  // 'RX payload width, pipe0' register address
#define NRF_RX_PW_P1        0x12  // 'RX payload width, pipe1' register address
#define NRF_RX_PW_P2        0x13  // 'RX payload width, pipe2' register address
#define NRF_RX_PW_P3        0x14  // 'RX payload width, pipe3' register address
#define NRF_RX_PW_P4        0x15  // 'RX payload width, pipe4' register address
#define NRF_RX_PW_P5        0x16  // 'RX payload width, pipe5' register address
#define NRF_FIFO_STATUS     0x17  // 'FIFO Status Register' register address	 

#define NRF_CS_SET() gpio_bit_set(GPIOA, GPIO_PIN_15)
#define NRF_CS_RESET() gpio_bit_reset(GPIOA, GPIO_PIN_15)

#define NRF_CE_SET() gpio_bit_set(GPIOA, GPIO_PIN_5)
#define NRF_CE_RESET() gpio_bit_reset(GPIOA, GPIO_PIN_5)

#define Max_data 2500
#define Min_data 500


void delay_us_config(void);
void delay_us(unsigned short int nus);
void Data_Convert(uint8_t * pre_data,uint16_t * data);
uint16_t Limit_data(uint16_t data);
void spi0_init(void);
uint8_t SPI_RW(uint8_t byte);
void delay_us(unsigned short int nus);
uint8_t NRf24L01_Write_READ_Data(uint8_t addr,uint8_t write_data);
uint8_t NRf24L01_WriteBuf(uint8_t addr, uint8_t *pBuf, uint8_t len);
uint8_t NRf24L01_ReadBuf(uint8_t addr, uint8_t *pBuf, uint8_t len);
void NRf24L01_Init(void);
void Rx_Mode(void);
void Tx_Mode(void);
uint8_t NRf24L01_TxPacket(uint8_t *txbuf,uint8_t len);
uint8_t NRf24L01_RePacket(uint8_t *txbuf,uint8_t len);
uint8_t NRf24L01_Check(void);
void sbusPreparePacket(uint8_t packet[],uint16_t output[], char isSignalLoss, char isFailsafe);
float map(float value,float fromLow,float fromHigh,float toLow,float toHigh);
#endif
