#include "QRList.h"
#include "Location.h"

QRList::QRList()
{
	QR.clear();
	string start = "C:\\Users\\xc\\Desktop\\��\\����\\��Ŀ\\data\\pic\\x", end = ".png";
	string start_2 = "C:\\Users\\xc\\Desktop\\��\\����\\��Ŀ\\data\\res\\x";
	int i = 0;
	while (++i)
	{
		//if (i > 1)return 0;
		printf("\n\n%d:\n\n", i);

		string num = int2str(i);
		string path = start + num + end;
		src = imread(path, 1);
		//src = imread("out.jpg");
		//canny(src);
		if (src.empty()) {
			//����Ƿ��ȡͼ��
			cout << "Error! Input image cannot be read...\n";
			return;
		}
		Location a = Location(src);
		append(a.res);
	}
}

int QRList::size()
{
	return QR.size();
}
QRMatrix& QRList::at(int index)
{
	return QR[index];
}

void QRList::append(QRMatrix tmp)
{
	QR.push_back(tmp);
}
bool QRList::edit(int index, QRMatrix tmp)
{
	if (index >= size())
		return false;
	QR[index] = tmp;
	return true;
}

QRMatrix& QRList::operator[](int i)
{
	if (i >= size()) //�������Ͳ���
		QR.resize(i + 1);
	return QR[i];
}