/*
 * myPID.h
 *
 *  Created on: 2019年7月22日
 *      Author: ding
 */

#ifndef MYPID_H_
#define MYPID_H_

#define PC	2.5  //比例系数
#define IC  0.5  //积分系数
#define DC  4.5  //微分系数

void PID_Clear();
int PID_Cal();

#endif /* MYPID_H_ */
