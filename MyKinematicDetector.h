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
	const demical fRad2Deg = 57.295779513f; //������תΪ�Ƕȵĳ���
	static const int MPU = 0x68; //MPU-6050��I2C��ַ
	static const int nValCnt = 7; //һ�ζ�ȡ�Ĵ���������

	const int nCalibTimes = 1000; //У׼ʱ�����Ĵ���
	int calibData[nValCnt]; //У׼����

	unsigned long nLastTime = 0; //��һ�ζ�����ʱ��
	demical fLastRoll = 0.0f; //��һ���˲��õ���Roll��
	demical fLastPitch = 0.0f; //��һ���˲��õ���Pitch��
	Kalman kalmanRoll; //Roll���˲���
	Kalman kalmanPitch; //Pitch���˲���


	/*
	a init function for initializing this class, with MPU8265 inside.
	Do not use it.
	*/
	void begin();

	/*
	*��MPU6050д��һ���ֽڵ�����
	* ָ���Ĵ�����ַ��һ���ֽڵ�ֵ
	*/
	void writeMPUReg(int nReg, unsigned char nVal);

	/* ��MPU6050�������ٶȼ������������¶Ⱥ��������ٶȼ�
	* ������ָ����������
	*/
	void readAccGyr(int* pVals);

	/*�Դ�����������ͳ�ƣ�У׼ƽ��ƫ����*/
	void calibration();

	/*��MPU6050����һ���ֽڵ�����
	* ָ���Ĵ�����ַ�����ض�����ֵ
	*/
	unsigned char readMPUReg(int nReg);

	/*���Roll��*/
	demical getRoll(demical* pRealVals, demical fNorm);

	/*���Pitch��*/
	demical getPitch(demical* pRealVals, demical fNorm);

	/*���Yaw��*/
	demical getYaw(demical* pRealVals, demical fNorm);

	/*�Զ������о���������ƫ�ƣ���ת��Ϊ��������*/
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

