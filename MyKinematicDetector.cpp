#include "MyKinematicDetector.h"

//�������������ƭ���������ģ�ʹ��ʱ��Ҫɾ����
unsigned long micros() {
	return 0;
}
void MyKinematicDetector::begin()
{
	//Serial.begin(9600); //��ʼ�����ڣ�ָ��������
	//Wire.begin(); //��ʼ��Wire��
	//writeMPUReg(0x6B, 0); //����MPU6050�豸

	//calibration(); //ִ��У׼
	//nLastTime = micros(); //��¼��ǰʱ��
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
	//�����
	for (int i = 0; i < nCalibTimes; ++i) {
		int mpuVals[nValCnt];
		readAccGyr(mpuVals);
		for (int j = 0; j < nValCnt; ++j) {
			valSums[j] += mpuVals[j];
		}
	}
	//����ƽ��
	for (int i = 0; i < nValCnt; ++i) {
		calibData[i] = int(valSums[i] / nCalibTimes);
	}
	calibData[2] += 16384; //��оƬZ����ֱ���£��趨��̬�����㡣
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
	readAccGyr(readouts); //��������ֵ

	demical realVals[7];
	rectify(readouts, realVals); //����У׼��ƫ�������о���

	//������ٶ�������ģ��������gΪ��λ
	demical fNorm = sqrt(realVals[0] * realVals[0] + realVals[1] * realVals[1] + realVals[2] * realVals[2]);
	demical fRoll = getRoll(realVals, fNorm); //����Roll��
	if (realVals[1] > 0) {
		fRoll = -fRoll;
	}
	demical fPitch = getPitch(realVals, fNorm); //����Pitch��
	if (realVals[0] < 0) {
		fPitch = -fPitch;
	}

	//�������β�����ʱ����dt������Ϊ��λ
	unsigned long nCurTime = micros();
	demical dt = (double)(nCurTime - nLastTime) / 1000000.0;
	//��Roll�Ǻ�Pitch�ǽ��п������˲�
	demical fNewRoll = kalmanRoll.getAngle(fRoll, realVals[4], dt);
	demical fNewPitch = kalmanPitch.getAngle(fPitch, realVals[5], dt);
	//�����˲�ֵ����Ƕ���
	demical fRollRate = (fNewRoll - fLastRoll) / dt;
	demical fPitchRate = (fNewPitch - fLastPitch) / dt;

	//����Roll�Ǻ�Pitch��
	fLastRoll = fNewRoll;
	fLastPitch = fNewPitch;
	//���±��β��ʱ��
	nLastTime = nCurTime;

	//�򴮿ڴ�ӡ���Roll�Ǻ�Pitch�ǣ�����ʱ��Arduino�Ĵ��ڼ������в鿴
	//Serial.print("Roll:");
	//Serial.print(fNewRoll); Serial.print('(');
	//Serial.print(fRollRate); Serial.print("),\tPitch:");
	//Serial.print(fNewPitch); Serial.print('(');
	//Serial.print(fPitchRate); Serial.print(")\n");
	//delay(10);
}
