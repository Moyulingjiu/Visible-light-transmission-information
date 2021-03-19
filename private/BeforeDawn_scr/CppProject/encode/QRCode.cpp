#include "QRCode.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <random>
#include <vector>
#include "Buffer.h"

using namespace cv;
using namespace std;

//=======================================
//���ݶ���
//=======================================
#define WihtePadding 30        //�ױ�����
#define NumberofColorBlocks 29 //�����С���߳�����Ӧ�������㣺1����С����С��21,2��Ϊ������
#define Numbmerofpixels 5      //�������Ӧ������
#define LinePixels (Numbmerofpixels * NumberofColorBlocks + 2 * WihtePadding)
#define DataContain (NumberofColorBlocks * NumberofColorBlocks - 217 - 30 - (NumberofColorBlocks - 16) * 2) //���Դ������������217Ϊ��λ��ռ�ã�30Ϊ�汾��Ϣ

char matrix[NumberofColorBlocks][NumberofColorBlocks]; //��ά����Ϣ������һ��û����������飡��
char xorMatrix[NumberofColorBlocks][NumberofColorBlocks]; //����ڸǲ�
vector<Vec3b> rgb_pixels;                             //���� rgb���ص��б�
//��λ��
const int bigAnchorPoint[8][8] = {
    1, 1, 1, 1, 1, 1, 1, 0,
    1, 0, 0, 0, 0, 0, 1, 0,
    1, 0, 1, 1, 1, 0, 1, 0,
    1, 0, 1, 1, 1, 0, 1, 0,
    1, 0, 1, 1, 1, 0, 1, 0,
    1, 0, 0, 0, 0, 0, 1, 0,
    1, 1, 1, 1, 1, 1, 1, 0,
    0, 0, 0, 0, 0, 0, 0, 0 };
//С��λ��
const int smallAnchorPoint[5][5] = {
    1, 1, 1, 1, 1,
    1, 0, 0, 0, 1,
    1, 0, 1, 0, 1,
    1, 0, 0, 0, 1,
    1, 1, 1, 1, 1 };
//�汾��Ϣ��101 1010 1010 0101��
const char version[15] = {
    1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1 };

//=======================================
//���岿��
//=======================================

QRCode::QRCode()
{
    //��ʼ��
    rgb_pixels.push_back({ 255, 255, 255 }); //0--��ɫ
    rgb_pixels.push_back({ 0, 0, 0 });       //1--��ɫ
}

//���ɶ�ά��
void QRCode::getQRCode()
{
    //��ʼ��ͼƬ����ֵΪ��ɫ
    Mat img(LinePixels, LinePixels, CV_8UC3, Scalar(255, 255, 255));

    //��д������Ϣ
    QRCodeBasic();
    //cout << "���Դ���Ķ�����λ����" << DataContain << endl;

    //д������
    //writeData();

    //�������
    Xor2();

    //д��ͼƬ
    for (int i = 0; i < NumberofColorBlocks; i++)
    {
        for (int j = 0; j < NumberofColorBlocks; j++)
        {
            //д�뵥��0��1
            for (int p = 0; p < Numbmerofpixels; p++)
            {
                for (int q = 0; q < Numbmerofpixels; q++)
                {
                    int x = i * Numbmerofpixels + p + WihtePadding;
                    int y = j * Numbmerofpixels + q + WihtePadding;
                    int color = (matrix[i][j] == -1) ? 0 : matrix[i][j];
                    img.at<Vec3b>(x, y) = rgb_pixels[color];
                }
            }
        }
    }
    imshow("img", img);
    waitKey(0);
}

//��Ӷ�ά��Ļ�����Ϣ����λ�㡢����ͼ����ʽ�㣩
void QRCode::QRCodeBasic()
{
    int i, j;
    //ȫ������-1������û�����ݣ�-1���ᱻ����Ϊ0�����ͼƬ�
    for (i = 0; i < NumberofColorBlocks; i++)
    {
        for (j = 0; j < NumberofColorBlocks; j++)
        {
            matrix[i][j] = -1;
        }
    }

    //���λ��
    for (i = 0; i < 8; i++)
    {
        for (j = 0; j < 8; j++)
        {
            matrix[i][j] = bigAnchorPoint[i][j];
            matrix[NumberofColorBlocks - 1 - i][j] = bigAnchorPoint[i][j];
            matrix[i][NumberofColorBlocks - 1 - j] = bigAnchorPoint[i][j];
        }
    }
    //�С��λ��
    for (i = 0; i < 5; i++)
    {
        for (j = 0; j < 5; j++)
        {
            matrix[NumberofColorBlocks - 5 - i][NumberofColorBlocks - 5 - j] = smallAnchorPoint[i][j];
        }
    }
    //���λ��
    for (i = 8; i < NumberofColorBlocks - 8; i++)
    {
        matrix[6][i] = (i + 1) % 2;
        matrix[i][6] = (i + 1) % 2;
    }
    //��д�汾��Ϣ
    for (i = 0; i < 8; i++)
    {
        if (i > 1)
            matrix[7 - i][8] = version[i];
        else
            matrix[8 - i][8] = version[i];
        matrix[8][NumberofColorBlocks - i - 1] = version[i];
    }
    for (i = 8; i < 14; i++)
    {
        matrix[NumberofColorBlocks - 9][i - 8] = version[i];
        matrix[8][i - 8] = version[i];
    }
    matrix[NumberofColorBlocks - 9][7] = version[14];
    matrix[8][7] = version[14];
}

//д������
void QRCode::writeData()
{
    for (int i = 0; i < NumberofColorBlocks; i++)
    {
        for (int j = 0; j < NumberofColorBlocks; j++)
        {
            if (matrix[i][j] == -1)
                matrix[i][j] = rand() % 2;
        }
    }
}

void QRCode::Xor1()
{
    int i, j;
    for (i = 9; i < NumberofColorBlocks - 9; i++)
    {
        for (j = 0; j < 6; j++)
        {
            int cover = (i + j + 1) % 2;
            //��
            int compare = (matrix[i][j] == -1) ? 0 : matrix[i][j];
            if (compare == cover)
                matrix[i][j] = 0;
            else
                matrix[i][j] = 1;
            //��
            compare = (matrix[j][i] == -1) ? 0 : matrix[j][i];
            if (compare == cover)
                matrix[j][i] = 0;
            else
                matrix[j][i] = 1;
        }
    }
    for (i = 9; i < NumberofColorBlocks - 9; i++)
    {
        for (j = 7; j < 9; j++)
        {
            int cover = (i + j + 1) % 2;
            //��
            int compare = (matrix[i][j] == -1) ? 0 : matrix[i][j];
            if (compare == cover)
                matrix[i][j] = 0;
            else
                matrix[i][j] = 1;
            //��
            compare = (matrix[j][i] == -1) ? 0 : matrix[j][i];
            if (compare == cover)
                matrix[j][i] = 0;
            else
                matrix[j][i] = 1;
        }
    }
    for (i = NumberofColorBlocks - 1; i > NumberofColorBlocks - 10; i--)
    {
        int cover = (i + 8 + 1) % 2;
        int compare = (matrix[i][8] == -1) ? 0 : matrix[i][8];
        if (compare == cover)
            matrix[i][8] = 0;
        else
            matrix[i][8] = 1;
    }
    for (i = 8; i < NumberofColorBlocks; i++)
    {
        for (j = 8; j < NumberofColorBlocks; j++)
        {
            if (i > NumberofColorBlocks - 10 && i < NumberofColorBlocks - 4) //ȥ��С��λ��
                if (j > NumberofColorBlocks - 10 && j < NumberofColorBlocks - 4)
                    continue;
            int cover = (i + j + 1) % 2;
            int compare = (matrix[i][j] == -1) ? 0 : matrix[i][j];
            if (compare == cover)
                matrix[i][j] = 0;
            else
                matrix[i][j] = 1;
        }
    }
}

void QRCode::Xor2()
{
    for (int i = 0; i < NumberofColorBlocks; i++)
    {
        for (int j = 0; j < NumberofColorBlocks; j++)
        {
            const int block = 5;
            int x = i / block;
            int y = j / block;
            xorMatrix[i][j] = (char)((x + y + 1) % 2);
        }
    }

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            xorMatrix[i][j] = -1;
            xorMatrix[NumberofColorBlocks - 1 - i][j] = -1;
            xorMatrix[i][NumberofColorBlocks - 1 - j] = -1;
        }
    }
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            xorMatrix[NumberofColorBlocks - 5 - i][NumberofColorBlocks - 5 - j] = -1;
        }
    }
    for (int i = 8; i < NumberofColorBlocks - 8; i++)
    {
        xorMatrix[6][i] = -1;
        xorMatrix[i][6] = -1;
    }

    for (int i = 0; i < 8; i++)
    {
        if (i > 1)
            xorMatrix[7 - i][8] = -1;
        else
            xorMatrix[8 - i][8] = -1;
        xorMatrix[8][NumberofColorBlocks - i - 1] = -1;
    }
    for (int i = 8; i < 14; i++)
    {
        xorMatrix[NumberofColorBlocks - 9][i - 8] = -1;
        xorMatrix[8][i - 8] = -1;
    }
    xorMatrix[NumberofColorBlocks - 9][7] = -1;
    xorMatrix[8][7] = -1;

    for (int i = 0; i < NumberofColorBlocks; i++)
    {
        for (int j = 0; j < NumberofColorBlocks; j++)
        {
            if (xorMatrix[i][j] != -1)
            {
                int compare = (matrix[i][j] == -1) ? 0 : matrix[i][j];
                if (compare != xorMatrix[i][j])
                    matrix[i][j] = 1;
                else
                    matrix[i][j] = 0;
            }
        }
    }
}