/*
 * myfan.h
 *
 *  Created on: 2019Äê7ÔÂ20ÈÕ
 *      Author: ding
 */

#ifndef MYFAN_H_


//AD
#define SCLK         (BIT5)    //sclk
#define SDO          (BIT4)    //sdo
#define SDI          (BIT7)    //sdi
#define CS           (BIT1)    //cs

#define ADS7950_SCLK_HIGH  (P5OUT |= SCLK)
#define ADS7950_SCLK_LOW   (P5OUT &= ~SCLK)
#define ADS7950_SDI_HIGH   (P3OUT |= SDI)
#define ADS7950_SDI_LOW    (P3OUT &= ~SDI)
#define ADS7950_CS_HIGH    (P1OUT |= CS)
#define ADS7950_CS_LOW     (P1OUT &= ~CS)
#define ADS7950_IN         (P5IN)

#define SET_PIN_DIR_ADS7950_SCLK_OUT (P5DIR |= SCLK)
#define SET_PIN_DIR_ADS7950_SDO_IN   (P5DIR &= ~SDO)
#define SET_PIN_DIR_ADS7950_SDI_OUT  (P3DIR |= SDI)
#define SET_PIN_DIR_ADS7950_CS_OUT   (P1DIR |= CS)

#define MODE_MANUAL_CH0        0x1800
#define MODE_MANUAL_CH1        0x1880
#define MODE_MANUAL_CH2        0x1900
#define MODE_MANUAL_CH3        0x1980

void ADS7950_PORT_Init();
unsigned int ADS7950_ReadResultAndSetNextSample(unsigned int uiSendData);
int updataADVALUE();

//KeyBoard
#define SW1         (BIT4)    //SW1
#define SW2         (BIT5)    //SW2
#define SW3         (BIT6)    //SW3
#define SW4         (BIT7)    //SW4
#define KeyBoard_IN (P2IN)

void Scan_Key_init();
unsigned int Scan_Key();

//led
#define LED_DATA	(P8OUT)
#define SET_N1		(BIT5) //P3.5
#define SET_N2		(BIT4) //P3.4
#define SET_N3		(BIT0) //P11.0
#define REAL_N1		(BIT2) //P11.2
#define REAL_N2		(BIT6) //P1.6
#define REAL_N3		(BIT7) //P1.7
void LED_init();
void LED_Show(unsigned int NUM,int type);

//motor

#define Set_Motor_High (P2OUT |= BIT3)
#define Set_Motor_Low  (P2OUT &= ~BIT3)
void Motor_init();
void Timer_init();
__interrupt void TIMER0_A0(void);
void initClock();

#endif /* MYFAN_H_ */
