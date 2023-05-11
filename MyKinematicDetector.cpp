#include "MyKinematicDetector.h"

//这个方法是用来骗过编译器的，使用时需要删除。
unsigned long micros() {
	return 0;
}
void MyKinematicDetector::begin()
{
	//Serial.begin(9600); //初始化串口，指定波特率
	//Wire.begin(); //初始化Wire库
	//writeMPUReg(0x6B, 0); //启动MPU6050设备

	//calibration(); //执行校准
	//nLastTime = micros(); //记录当前时间
}

void MyKinematicDetector::writeMPUReg(int nReg, unsigned char nVal)
{
	//Wire.beginTransmission(MPU);
	//Wire.write(nReg);
	//Wire.write(nVal);
	//Wire.endTransmission(true);
}

unsigned char MyKinematicDetector::readMPUReg(int nReg)
{
	//Wire.beginTransmission(MPU);
	//Wire.write(nReg);
	//Wire.requestFrom(MPU, 1, true);
	//Wire.endTransmission(true);
	//return Wire.read();
}

void MyKinematicDetector::readAccGyr(int* pVals)
{
	//Wire.beginTransmission(MPU);
	//Wire.write(0x3B);
	//Wire.requestFrom(MPU, nValCnt * 2, true);
	//Wire.endTransmission(true);
	//for (long i = 0; i < nValCnt; ++i) {
	//	pVals[i] = Wire.read() << 8 | Wire.read();
	//}
}

void MyKinematicDetector::calibration()
{
	demical valSums[7] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0 };
	//先求和
	for (int i = 0; i < nCalibTimes; ++i) {
		int mpuVals[nValCnt];
		readAccGyr(mpuVals);
		for (int j = 0; j < nValCnt; ++j) {
			valSums[j] += mpuVals[j];
		}
	}
	//再求平均
	for (int i = 0; i < nValCnt; ++i) {
		calibData[i] = int(valSums[i] / nCalibTimes);
	}
	calibData[2] += 16384; //设芯片Z轴竖直向下，设定静态工作点。
}

demical MyKinematicDetector::getRoll(demical* pRealVals, demical fNorm)
{
	demical fNormXZ = sqrt(pRealVals[0] * pRealVals[0] + pRealVals[2] * pRealVals[2]);
	demical fCos = fNormXZ / fNorm;
	return acos(fCos) * fRad2Deg;
}

demical MyKinematicDetector::getYaw(demical* pRealVals, demical fNorm)
{
	demical fNormXY = sqrt(pRealVals[0] * pRealVals[0] + pRealVals[1] * pRealVals[1]);
	demical fCos = fNormXY / fNorm;
	return acos(fCos) * fRad2Deg;
}

void MyKinematicDetector::rectify(int* pReadout, demical* pRealVals)
{
	for (int i = 0; i < 3; ++i) {
		pRealVals[i] = (demical)(pReadout[i] - calibData[i]) / 16384.0f;
	}
	pRealVals[3] = pReadout[3] / 340.0f + 36.53;
	for (int i = 4; i < 7; ++i) {
		pRealVals[i] = (demical)(pReadout[i] - calibData[i]) / 131.0f;
	}
}

demical MyKinematicDetector ::getPitch(demical* pRealVals, demical fNorm) {
	demical fNormYZ = sqrt(pRealVals[1] * pRealVals[1] + pRealVals[2] * pRealVals[2]);
	demical fCos = fNormYZ / fNorm;
	return acos(fCos) * fRad2Deg;
}

MyKinematicDetector::MyKinematicDetector()
{
	(*this).begin();
}

void MyKinematicDetector::mainLoop()
{
	int readouts[nValCnt];
	readAccGyr(readouts); //读出测量值

	demical realVals[7];
	rectify(readouts, realVals); //根据校准的偏移量进行纠正

	//计算加速度向量的模长，均以g为单位
	demical fNorm = sqrt(realVals[0] * realVals[0] + realVals[1] * realVals[1] + realVals[2] * realVals[2]);
	demical fRoll = getRoll(realVals, fNorm); //计算Roll角
	if (realVals[1] > 0) {
		fRoll = -fRoll;
	}
	demical fPitch = getPitch(realVals, fNorm); //计算Pitch角
	if (realVals[0] < 0) {
		fPitch = -fPitch;
	}

	//计算两次测量的时间间隔dt，以秒为单位
	unsigned long nCurTime = micros();
	demical dt = (double)(nCurTime - nLastTime) / 1000000.0;
	//对Roll角和Pitch角进行卡尔曼滤波
	demical fNewRoll = kalmanRoll.getAngle(fRoll, realVals[4], dt);
	demical fNewPitch = kalmanPitch.getAngle(fPitch, realVals[5], dt);
	//跟据滤波值计算角度速
	demical fRollRate = (fNewRoll - fLastRoll) / dt;
	demical fPitchRate = (fNewPitch - fLastPitch) / dt;

	//更新Roll角和Pitch角
	fLastRoll = fNewRoll;
	fLastPitch = fNewPitch;
	//更新本次测的时间
	nLastTime = nCurTime;

	//向串口打印输出Roll角和Pitch角，运行时在Arduino的串口监视器中查看
	//Serial.print("Roll:");
	//Serial.print(fNewRoll); Serial.print('(');
	//Serial.print(fRollRate); Serial.print("),\tPitch:");
	//Serial.print(fNewPitch); Serial.print('(');
	//Serial.print(fPitchRate); Serial.print(")\n");
	//delay(10);
}
