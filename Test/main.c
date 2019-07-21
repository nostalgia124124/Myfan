#include <msp430f5438.h>
#include <myfan.h>
extern volatile unsigned int SetPWM;
/*
 * main.c
 */
int main(void) {
	volatile unsigned int SPI_Result,ADC_Value,ADC_CH;
	volatile unsigned int ADC_COUNT = 0,ADC_SHOW = 0;
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    LED_init();
    Scan_Key_init();
    ADS7950_PORT_Init();

    Motor_init();
    P2SEL &= ~BIT3;			//close motor
    P2OUT |= BIT3;
    volatile unsigned int cursor;
    while(1){
    	if(ADC_COUNT >= 100){
        	SPI_Result = ADS7950_ReadResultAndSetNextSample(MODE_MANUAL_CH3);
        	ADC_Value = SPI_Result & 0x0FFF;
        	ADC_CH = SPI_Result >> 12;
        	if(ADC_CH == 3){
        		ADC_SHOW = ADC_Value/5;
        		ADC_COUNT = 0;
        	}
    	}
    	ADC_COUNT ++;
    	LED_Show(ADC_SHOW,1);
		LED_Show(135,0);
    	//LED_Show(269,1);
    	cursor = Scan_Key();
    	switch(cursor){
    		case 1: P2SEL |= BIT3;
    				break;
    		case 2: P2SEL &= ~BIT3;
    				P2OUT |= BIT3;
    				break;
    		case 3: TA1CCR2 += 50;
    				break;
    		case 4:	TA1CCR2 -= 50;
    				break;
    	}
    }
	return 0;
}
