/*
 * myfan.c
 *
 *  Created on: 2019年7月20日
 *      Author: ding
 */
#include <msp430f5438.h>
#include <myfan.h>

int MYLED_TABLE[10] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};
extern volatile unsigned int SetPWM;

void ADS7950_PORT_Init()
{
	SET_PIN_DIR_ADS7950_SCLK_OUT; //set pin output,connect to ads7950 sclk
	SET_PIN_DIR_ADS7950_SDI_OUT;  //set pin output,connect to ads7950 sdi
    SET_PIN_DIR_ADS7950_SDO_IN;   //set pin input, connect to ads7950 sdo
    SET_PIN_DIR_ADS7950_CS_OUT;   //set pin output,connect to ads7950 cs

}
/*
 * Simulate SPI Read and Write ADS7950;
 *
 * SDO--The first bit clocks out at the falling edge of  CS;
 *    --next 15 bits clocks out at the falling edge of SCLK;
 * SDI--latch data on the rising edge of SCLK;
 *
 * */

unsigned int ADS7950_ReadResultAndSetNextSample(unsigned int uiSendData)
{
	  unsigned int uiSendTimes,result;

	  result=0;

	  ADS7950_CS_HIGH;
	  ADS7950_SCLK_LOW;
	  __delay_cycles(25);//delay 20ns at least
	  ADS7950_CS_LOW;
	  __delay_cycles(30);//delay 40ns at least  for the first SDO

	  for(uiSendTimes=16;uiSendTimes>0;uiSendTimes--)
	  {

		    //prepare one bit to send
		    if((uiSendData & 0x8000)== 0x8000) ADS7950_SDI_HIGH;
	        else  ADS7950_SDI_LOW;

		    uiSendData <<=1;

		    //read one bit
		    result<<=1;
		    if(( ADS7950_IN & SDO)== SDO) result |= 0x0001;

		    //delay 20ns at least  to set sclk high
		    __delay_cycles(25);

		    ADS7950_SCLK_HIGH; //rising edge of SCLK, latch one bit
		    __delay_cycles(25);
		    ADS7950_SCLK_LOW;  //falling edge of SCLK, SDO clocks on one bit
		    __delay_cycles(25);
	  }

	  ADS7950_CS_HIGH;
	 // __delay_cycles(50);

	  return result;

}

volatile unsigned int ADC_Value;
int updataADVALUE(){
	volatile unsigned int ADC_CH, SPI_Result;
    	SPI_Result = ADS7950_ReadResultAndSetNextSample(MODE_MANUAL_CH3);
    	ADC_CH = SPI_Result >> 12;
    	if(ADC_CH == 3){				//判断读取通道是否正确
    		ADC_Value = SPI_Result & 0x0FFF;//低12位为AD值
    		return 1;
    	}
    	else return 0;
}

/*  fuction: initilize Scan Keyboard
 *  author:  Ding
 */
void Scan_Key_init(){
	P5DIR &= ~SW1;  //设置SW1~SW4为输入
	P5DIR &= ~SW2;
	P5DIR &= ~SW3;
	P5DIR &= ~SW4;
}

/*  fuction: Scan Keyboard
 *  author:  Ding
 */
volatile unsigned int lastKey = 0;
volatile unsigned int KeyCount;
unsigned int Scan_Key(){
	volatile unsigned int SW;

	SW = ~(KeyBoard_IN | 0x000F) & 0x00FF;
	if(SW != lastKey){
		lastKey = SW;
		KeyCount = 0;
		return 0;
	}
	else{
		if(KeyCount < 10){
			KeyCount++;
			return 0;
		}
		else if(KeyCount == 10){
			KeyCount++;
			switch(SW){
					case SW1: return 1;
					case SW2: return 2;
					case SW3: return 3;
					case SW4: return 4;
			}
		}
	}
	return 0;
}

/*  fuction: initilize LED
 *  author:  Ding
 */
void LED_init(){
	P8DIR |= 0x00FF;
	P3DIR |= BIT0;
	P11DIR |= BIT1;
	P3DIR |= SET_N1;
	P3DIR |= SET_N2;
	P11DIR |= SET_N3;
	P11DIR |= REAL_N1;
	P1DIR |= REAL_N2;
	P1DIR |= REAL_N3;
	P3OUT |= 0x31;
	P11OUT |= 0x07;
	P1OUT |= 0xC0;
}

/*  fuction: LED
 *  author:  Ding
 */

void LED_Show(unsigned int NUM,int type){
	if(type == 0){

		P8OUT = MYLED_TABLE[NUM%10];
		P11OUT &= ~SET_N3; // P11.0 = 0
		NUM /= 10;
		__delay_cycles(80);
		P11OUT |= 0x07;

		P8OUT = MYLED_TABLE[NUM%10];
		P3OUT &= ~SET_N2;
		NUM /= 10;
		__delay_cycles(80);
		P3OUT |= 0x31;

		P8OUT = MYLED_TABLE[NUM];
		P3OUT &= ~SET_N1;
		__delay_cycles(80);
		P3OUT |= 0x31;
	}
	else{

		P8OUT = MYLED_TABLE[NUM%10];
		P1OUT &= ~REAL_N3;
		NUM /= 10;
		__delay_cycles(80);
		P1OUT |= 0xC0;

		P8OUT = MYLED_TABLE[NUM%10];
		P1OUT &= ~REAL_N2;
		NUM /= 10;
		__delay_cycles(80);
		P1OUT |= 0xC0;

		P8OUT = MYLED_TABLE[NUM];
		P11OUT &= ~REAL_N1;
		__delay_cycles(80);
		P11OUT |= 0x07;
	}
}

/*  fuction: initilize Timer1.2 output PWM
 *  author:  Ding
 */
void Motor_init(){
	P2SEL |= BIT3;							//使用第二功能
	P2DIR |= BIT3;							//PWM输出口P2.3
	TA1CCTL2 = OUTMOD_3;                          // CCR0中断使能
	TA1CCR0 = 500;
	TA1CCR2 = 50;
	TA1CTL = TASSEL_2 + ID_1 + MC_1 + TACLR;     // SMCLK, 增计数模式, 清除
}

/*  fuction: initilize Clock
 *  author:  KanKan
 */

void initClock()
{
	/*//产生振荡频率
	P5SEL |= BIT2 + BIT3; 										//XT2引脚功能选择
	UCSCTL6 &= ~XT2OFF; 										//启动XT2

	UCSCTL4 |= SELA__XT2CLK + SELS__XT2CLK + SELM__XT2CLK; 		//避免DCO调整中跑飞
	UCSCTL5 |= DIVA__32 + DIVS__32 + DIVM__2; 						*///设定几个CLK的分频

	TA0CCTL0 |= CCIE;							//允许中断
	TA0CCR0 = 3000;							//定时3ms
	TA0CTL |= TASSEL_1 + MC_1 + TACLR + ID_0;     // SMCLK, 增计数模式, 清除TAR计数器
    _bis_SR_register(GIE);  // 进入LPM0,使能中断
}

extern volatile unsigned int PID_flag;
#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0(void)
{
	PID_flag = 1;
}



