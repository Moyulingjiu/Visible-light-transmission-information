#include "QRCode.h"

char xorMatrix[NumberofColorBlocks][NumberofColorBlocks];

QRCode::QRCode()
{
	buffer = NULL;
}
QRCode::QRCode(DataBuffer* dataBuffer)
{
	setBuffer(dataBuffer);
}
void QRCode::setBuffer(DataBuffer* dataBuffer)
{
	buffer = dataBuffer;
}

QRList QRCode::locationQR(Mat img)
{
	//��λ��ά���λ��
	QRLocation location(img);
	return location.get();
}

bool QRCode::decode(Mat img)
{
	if (buffer == NULL)
		return false;

	int totalQR = 0; //�ܼƶ�ά������
	int nowQR = 0; //��ǰ�ǵڼ��Ŷ�ά��
	QRList QRlist = locationQR(img);
	for (int i = 0; i < QRlist.size(); i++)
	{
		//for (int h = 0; h < QRlist[i].height(); h++) 
		//{
		//	for (int w = 0; w < QRlist[i].width(); w++) 
		//	{
		//		printf("%d ", QRlist[i].at(h, w));
		//	}
		//	printf("\n");
		//}
		if (versionCheck(QRlist[i]) && sizeCheck(QRlist[i]) && EffectivenessCheck(QRlist[i]))
		{
			read(QRlist[i]);
		}
	}

	return true;
}
void QRCode::fixPoint(int& x, int& y)
{
	//xΪ�У�yΪ��
	//�Զ�����һ�����ҵ�������ʵĿ�λ��һ������ͻ���Ϊ0,0
	x++;
	if (x >= NumberofColorBlocks)
	{
		x = 0;
		y++;
	}
	if (y >= NumberofColorBlocks)
	{
		x = NumberofColorBlocks - 1;
		y = NumberofColorBlocks - 1;
		return;
	}
}

bool QRCode::versionCheck(QRMatrix& matrix)
{
	return true;
}
bool QRCode::sizeCheck(QRMatrix& matrix)
{
	return true;
}
bool QRCode::EffectivenessCheck(QRMatrix& matrix)
{
	return true;
}
void QRCode::read(QRMatrix& matrix)
{

}