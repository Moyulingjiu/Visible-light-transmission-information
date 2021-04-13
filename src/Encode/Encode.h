#pragma once
#include <string>
#include <opencv2/opencv.hpp>
#include <vector>

#include "DataBuffer.h"
#include "Video.h"
#include "QRCode.h"
#include "GlobalMacroDefine.h"

using namespace std;
using namespace cv;

class Encode
{
	DataBuffer buffer;
	Video video;
	int videoLenth;
	bool playVideo; //������ֶ������򲥷���Ƶ�������������ģʽ�򲻲�����Ƶ
public:
	Encode();
	Encode(string filename, string videoFilename, int length, int argc);
	Encode(DataBuffer _buffer, string videoFilename, int length, int argc);

	bool isEmpty();
	bool data2Video();
};

