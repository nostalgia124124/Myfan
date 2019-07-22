#include <msp430f5438.h>
#include <myfan.h>
#include <myPID.h>
volatile unsigned int SetPWM;
extern volatile unsigned int ADC_Value;
extern volatile unsigned int PID_flag;
extern volatile int sumerror;
/*
 * main.c
 */
int main(void) {
	volatile unsigned int ADC_COUNT = 0,ADC_SHOW = 0;
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    LED_init();
    Scan_Key_init();
    ADS7950_PORT_Init();
    initClock();
    PID_Clear();

    Motor_init();
    SetPWM = 300;
    P2SEL &= ~BIT3;			//close motor
    P2OUT |= BIT3;
    volatile unsigned int cursor;
    while(1){
    	if(PID_flag){
			if(updataADVALUE() == 1){
				ADC_SHOW = ADC_Value/5;
				ADC_COUNT++;
				SetMotorV();
				PID_flag = 0;
			}
    	}
    	if(ADC_COUNT > 5) sumerror = 0;
    	LED_Show(ADC_SHOW,1);
		LED_Show(SetPWM,0);
    	//LED_Show(269,1);
    	cursor = Scan_Key();
    	switch(cursor){
    		case 1: P2SEL |= BIT3;			//将P2.3转成PWM波输出
    				break;
    		case 2: P2SEL &= ~BIT3; 		//将P2.3转成IO口模式且置高。关闭电机
    				P2OUT |= BIT3;
    				break;
    		case 3:
    				if(SetPWM <300)	SetPWM += 10;
    				break;
    		case 4:
    				if(SetPWM >50) SetPWM -= 10;
    				break;
    	}
    }
	return 0;
}





