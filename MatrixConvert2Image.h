#ifdef WIN32

#include "CyImage.h"
#pragma once

#define COLOR_FOREGROUND	0		//ǰ��ɫ
#define COLOR_BACKGROUND	1		//����ɫ
#define COLOR_QRCOLOR1		2		//��ɫһ(ǰ��)
#define COLOR_QRCOLOR2		3		//��ɫ��(����)
#define MAX_MODULESIZE		177		//QR�����ߴ�

// Ѱ���ĸ�Ԫ��֮����С�ģ���������
int FindMinValueIndex(COLORREF x, COLORREF y, COLORREF z, COLORREF w);

// ����ά�����ת��Ϊͼ��
BOOL Matrix2Image(BYTE* bDMatrix, int nSymbleSize, CyImage* pImage, int nPixelSize, COLORREF ForegroundColor, COLORREF BackgroundColor);

// ����ά�����ת��Ϊͼ��
BOOL Matrix2ColorImage(BYTE bMatrix[MAX_MODULESIZE][MAX_MODULESIZE], int nSymbleSize, CyImage* pImage, int nPixelSize, 
					   COLORREF ForegroundColor, COLORREF BackgroundColor, COLORREF QREncodeColor1, COLORREF QREncodeColor2);

#endif // WIN32