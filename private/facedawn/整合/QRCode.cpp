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


QRList QRCode::locationQR(Mat img)
{
	//��λ��ά���λ��
	Location pic(img);
	return pic.get();
}

bool QRCode::decode(Mat img)
{
	if (buffer == NULL)
		return false;

	QRList QRlist = locationQR(img);
	for (int i = 0; i < QRlist.size(); i++)
	{
		for (int j = 0; j < QRlist[i].height(); j++)
		{
			for (int k = 0; k < QRlist[i].width(); k++)
			{
				printf("%d ", QRlist[i].at(j, k));
			}
			printf("\n");
		}
	}

	return true;
}