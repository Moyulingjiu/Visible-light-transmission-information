#pragma once
class QRCode
{
	vector<Vec3b> rgb_pixels;                             //���� rgb���ص��б�
public:
	QRCode();
	void getQRCode();
	void QRCodeBasic();
	void writeData();
	void Xor();
};

