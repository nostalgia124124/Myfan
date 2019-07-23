/*
 * myPID.c
 *
 *  Created on: 2019Äê7ÔÂ22ÈÕ
 *      Author: ding
 */
#include <msp430f5438.h>
#include<myPID.h>

volatile int lasterror,sumerror,error,derror,lastAD;
volatile unsigned int PID_flag;
extern volatile unsigned int SetPWM;
extern volatile unsigned int ADC_Value;

void PID_Clear(){
	lasterror = 0;
	sumerror = 0;
	error = 0;
	PID_flag = 0;
}

int PID_Cal(){
	error = SetPWM - ADC_Value/5;
	//derror = error - lasterror;
	lastAD = ADC_Value;
	derror = (ADC_Value - lastAD)/5;
	sumerror += error;
	//if(sumerror > 800) sumerror = 800;
	//else if(sumerror < -800) sumerror = -800;
	//lasterror = error;
	return (int)(PC*error+IC*sumerror+DC*derror);
}

void SetMotorV(){
	volatile int PID_RES;
	PID_RES = SetPWM + 50 + PID_Cal();
	if(PID_RES < 0)	TA1CCR2 = 0;
	else if(PID_RES > 500) TA1CCR2 = 500;
	else TA1CCR2 = PID_RES;
}



