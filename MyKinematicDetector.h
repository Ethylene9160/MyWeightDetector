#pragma once
#ifndef MY_KINEMATICS_DETECTOR
#define MY_KINEMATICS_DETECTOR 3
//#include<Arduino.h>
//#include <Wire.h>
#include <Math.h>

#ifndef MY_DEMICAL
#define MY_DEMICAL
typedef double demical;
#endif

#include "Kalman.h"
typedef int int16_t;//remenber to delete this!

class MyKinematicDetector
{
private:
	const demical fRad2Deg = 57.295779513f; //将弧度转为角度的乘数
	static const int MPU = 0x68; //MPU-6050的I2C地址
	static const int nValCnt = 7; //一次读取寄存器的数量

	const int nCalibTimes = 1000; //校准时读数的次数
	int calibData[nValCnt]; //校准数据

	unsigned long nLastTime = 0; //上一次读数的时间
	demical fLastRoll = 0.0f; //上一次滤波得到的Roll角
	demical fLastPitch = 0.0f; //上一次滤波得到的Pitch角
	Kalman kalmanRoll; //Roll角滤波器
	Kalman kalmanPitch; //Pitch角滤波器


	/*
	a init function for initializing this class, with MPU8265 inside.
	Do not use it.
	*/
	void begin();

	/*
	*向MPU6050写入一个字节的数据
	* 指定寄存器地址与一个字节的值
	*/
	void writeMPUReg(int nReg, unsigned char nVal);

	/* 从MPU6050读出加速度计三个分量、温度和三个角速度计
	* 保存在指定的数组中
	*/
	void readAccGyr(int* pVals);

	/*对大量读数进行统计，校准平均偏移量*/
	void calibration();

	/*从MPU6050读出一个字节的数据
	* 指定寄存器地址，返回读出的值
	*/
	unsigned char readMPUReg(int nReg);

	/*算得Roll角*/
	demical getRoll(demical* pRealVals, demical fNorm);

	/*算得Pitch角*/
	demical getPitch(demical* pRealVals, demical fNorm);

	/*算得Yaw角*/
	demical getYaw(demical* pRealVals, demical fNorm);

	/*对读数进行纠正，消除偏移，并转换为物理量。*/
	void rectify(int* pReadout, demical* pRealVals);
public:
	/*
	constructor.
	Ples initialize it in <code>void setup()</code> function,
	use keyword <code>new</code> to establish this in the stack space, 
	will be a better choice.
	*/
	MyKinematicDetector();

	void mainLoop();
};

#endif

