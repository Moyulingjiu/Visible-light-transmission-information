#include "QRCode.h"

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


QRList locationQR(Mat img)
{
	//��λ��ά���λ��
}
bool QRCode::decode(Mat img)
{
	if (buffer == NULL)
		return false;

	QRList QR = locationQR(img);

	return true;
}