#include "stdafx.h"
#include "DataTypes.h"
#include "ImageTransform.h"
#include "templateFuncs.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/* - ��תͼ�� - 
ͼ����Ϣ��float* pSrc, int nWidth, int nHeight, int nRowlen, int nChannel
��ת�Ƕȣ�float angle
��ת���ģ�float x0, float y0
��ת�󶥵㣺int &Xmin, int &Ymin, int &Xmax, int &Ymax
�¿�ȣ�int &NewWidth
�¸߶ȣ�int &NewHeight
��ÿ���ֽ�����int &NewRowlen
*/
float* ImageRotate(float* pSrc, int nWidth, int nHeight, int nRowlen, int nChannel, float angle, float x0, float y0, int &Xmin, int &Ymin, int &Xmax, int &Ymax, int &NewWidth, int &NewHeight, int &NewRowlen)
{
	// ԭʼͼ���ĸ����������
	float x1, x2, x3, x4, y1, y2, y3, y4;
	x1 = 0;							y1 = 0;
	x2 = float(nWidth - 1);			y2 = 0;
	x3 = x2;						y3 = float(nHeight - 1);
	x4 = x1;						y4 = y3;
	// 2015.5.19 Ϊ�˼��ټ��㣬�����Ǻ����ŵ�ѭ������
	float cos_theta = cos(angle);
	float sin_theta = sin(angle);
	// �ĸ�����˳ʱ����ת,��ͼ������
	PositionTransform(x1, y1, cos_theta, sin_theta, x0, y0);
	PositionTransform(x2, y2, cos_theta, sin_theta, x0, y0);
	PositionTransform(x3, y3, cos_theta, sin_theta, x0, y0);
	PositionTransform(x4, y4, cos_theta, sin_theta, x0, y0);
	Xmax = int(FindMaxBetween4Numbers(x1, x2, x3, x4));
	Ymax = int(FindMaxBetween4Numbers(y1, y2, y3, y4));
	Xmin = int(FindMinBetween4Numbers(x1, x2, x3, x4));
	Ymin = int(FindMinBetween4Numbers(y1, y2, y3, y4));
	// ��ͼ���ȡ��߶ȡ�ÿ���ֽ����ı仯
	NewWidth = Xmax - Xmin + 1;
	NewHeight = Ymax - Ymin + 1;
	NewRowlen = nChannel * NewWidth;
	float *pDst = new float[NewRowlen * NewHeight];
	memset(pDst, 0, NewRowlen * NewHeight * sizeof(float));

#pragma omp parallel for
	for (int i = 0; i < NewWidth; ++i)
	{
		for (int j = 0; j < NewHeight; ++j)
		{
			float x = float(i + Xmin);
			float y = float(j + Ymin);
			PositionTransform(x, y, cos_theta, -sin_theta, x0, y0);
			for (int nCurChannel = 0; nCurChannel < nChannel; ++nCurChannel)
			{
				pDst[nCurChannel + i * nChannel + j * NewRowlen] = biLinearInterp(pSrc, nWidth, nHeight, nRowlen, nChannel, nCurChannel, x, y);
			}
		}
	}

	return pDst;
}


/* - ����rect�ü�ͼ�� -
float* pSrc				ͼ������
int &nWidth				ͼ����
int &nHeight			ͼ��߶�
int &nRowlen			ÿ���ֽ���
int nChannel			ͼ��ͨ��
CLogoRect rect			�ü�����
��ע�����زü�����ͼ�����ݣ����Ҹ��¿�ȡ��߶Ⱥ�ÿ���ֽ���
*/
float* ImageCut(float* pSrc, int &nWidth, int &nHeight, int &nRowlen, int nChannel, CLogoRect rect)
{
	if (rect == CLogoRect())
		return pSrc;
	CLogoRect cutRect = rect;

	ASSERT (0 <= cutRect.left && cutRect.left <= nWidth && 0 <= cutRect.top && cutRect.top <= nHeight);

	if (cutRect.right > nWidth)
		cutRect.right = nWidth;
	if (cutRect.bottom > nHeight)
		cutRect.bottom = nHeight;
	int nNewWidth = cutRect.Width();
	int nNewHeight = cutRect.Height();
	if (nNewWidth <= 0 || nNewHeight <= 0)
		return NULL;
	int nNewRowlen = nNewWidth * nChannel;
	float* pDst = new float[nNewHeight * nNewRowlen];

#pragma omp parallel for
	for (int i = 0; i < nNewHeight; ++i)
	{
		memcpy(pDst + i * nNewRowlen, pSrc + cutRect.left * nChannel + (i + cutRect.top) * nRowlen, nNewRowlen * sizeof(float));
	}
	// ����ͼ����Ϣ
	nWidth = nNewWidth;
	nHeight = nNewHeight;
	nRowlen = nNewRowlen;
	return pDst;
}


// ������ת�任��������ת�Ƕ�
void PositionTransform(float &x, float &y, float theta, float x0, float y0)
{
	float delta_x = x - x0;
	float delta_y = y - y0;
	PositionTransform(delta_x, delta_y, theta);
	x = x0 + delta_x;
	y = y0 + delta_y;
}


// ������ת�任������ѡ��Ƕ�
void PositionTransform(float &x, float &y, float theta)
{
	float cos_theta = cos(theta);
	float sin_theta = sin(theta);
	float x_temp = x * cos_theta - y * sin_theta;
	y = x * sin_theta + y * cos_theta;
	x = x_temp;
}


// ������ת���������Ϊ��ת�Ƕȵ����ҡ�����ֵ
void PositionTransform(float &x, float &y, float cos_sin[2], float x0, float y0)
{
	float delta_x = x - x0;
	float delta_y = y - y0;
	PositionTransform(delta_x, delta_y, cos_sin);
	x = x0 + delta_x;
	y = y0 + delta_y;
}


// ������ת���������Ϊ��ת�Ƕȵ����ҡ�����ֵ
void PositionTransform(float &x, float &y, float cos_sin[2])
{
	float x_temp = x * cos_sin[0] - y * cos_sin[1];
	y = x * cos_sin[1] + y * cos_sin[0];
	x = x_temp;
}


// ������ת���������Ϊ��ת�Ƕȵ����ҡ�����ֵ
void PositionTransform(float &x, float &y, float cos_theta, float sin_theta, float x0, float y0)
{
	float delta_x = x - x0;
	float delta_y = y - y0;
	PositionTransform(delta_x, delta_y, cos_theta, sin_theta);
	x = x0 + delta_x;
	y = y0 + delta_y;
}


// ������ת���������Ϊ��ת�Ƕȵ����ҡ�����ֵ
void PositionTransform(float &x, float &y, float cos_theta, float sin_theta)
{
	float x_temp = x * cos_theta - y * sin_theta;
	y = x * sin_theta + y * cos_theta;
	x = x_temp;
}


// Ѱ���ĸ����������
float FindMaxBetween4Numbers(float x, float y, float z, float w)
{
	float Max1, Max2;
	Max1 = x > y ? x : y;
	Max2 = z > w ? z : w;
	return (Max1 > Max2 ? Max1 : Max2);
}


// Ѱ���ĸ�������С��
float FindMinBetween4Numbers(float x, float y, float z, float w)
{
	float Min1, Min2;
	Min1 = x < y ? x : y;
	Min2 = z < w ? z : w;
	return (Min1 < Min2 ? Min1 : Min2);
}


/* - ��תͼ�� -
����ͼ����Ϣ��float* pSrc, int nWidth, int nHeight, int nRowlen, int nChannel
��ת�Ƕȣ�float angle
���ͼ����Ϣ��int &NewWidth, int &NewHeight, int &NewRowlen
*/
float* ImageRotate(float* pSrc, int nWidth, int nHeight, int nRowlen, int nChannel, float angle, int &NewWidth, int &NewHeight, int &NewRowlen)
{
	// ԭʼͼ���ĸ����������
	float x1, x2, x3, x4, y1, y2, y3, y4;
	x1 = 0;							y1 = 0;
	x2 = float(nWidth - 1);			y2 = 0;
	x3 = x2;						y3 = float(nHeight - 1);
	x4 = x1;						y4 = y3;
	// 2015.5.19 Ϊ�˼��ټ��㣬�����Ǻ����ŵ�ѭ������
	float cos_theta = cos(angle);
	float sin_theta = sin(angle);
	// �ĸ�����˳ʱ����ת,��ͼ�����½�
	PositionTransform(x1, y1, cos_theta, sin_theta);
	PositionTransform(x2, y2, cos_theta, sin_theta);
	PositionTransform(x3, y3, cos_theta, sin_theta);
	PositionTransform(x4, y4, cos_theta, sin_theta);
	int Xmin, Xmax, Ymin, Ymax;
	Xmax = int(FindMaxBetween4Numbers(x1, x2, x3, x4));
	Ymax = int(FindMaxBetween4Numbers(y1, y2, y3, y4));
	Xmin = int(FindMinBetween4Numbers(x1, x2, x3, x4));
	Ymin = int(FindMinBetween4Numbers(y1, y2, y3, y4));
	// ��ͼ���ȡ��߶ȡ�ÿ���ֽ����ı仯
	NewWidth = Xmax - Xmin + 1;
	NewHeight = Ymax - Ymin + 1;
	NewRowlen = nChannel * NewWidth;
	float *pDst = new float[NewRowlen * NewHeight];
	memset(pDst, 0, NewRowlen * NewHeight * sizeof(float));

#pragma omp parallel for
	for (int i = 0; i < NewWidth; ++i)
	{
		for (int j = 0; j < NewHeight; ++j)
		{
			float x = float(i + Xmin);
			float y = float(j + Ymin);
			PositionTransform(x, y, cos_theta, -sin_theta);
			for (int nCurChannel = 0; nCurChannel < nChannel; ++nCurChannel)
				pDst[nCurChannel + i * nChannel + j * NewRowlen] = biLinearInterp(pSrc, nWidth, nHeight, nRowlen, nChannel, nCurChannel, x, y);
		}
	}

	return pDst;
}


/* - ��תͼ�� -
����ͼ����Ϣ��float* pSrc, int nWidth, int nHeight, int nRowlen, int nChannel
��ת�Ƕȣ�float angle
��ת���ģ�float x0, float y0
���ͼ����Ϣ��int &NewWidth, int &NewHeight, int &NewRowlen
*/
float* ImageRotate(float* pSrc, int nWidth, int nHeight, int nRowlen, int nChannel, float angle, float x0, float y0, int &NewWidth, int &NewHeight, int &NewRowlen)
{
	// ԭʼͼ���ĸ����������
	float x1, x2, x3, x4, y1, y2, y3, y4;
	x1 = 0;							y1 = 0;
	x2 = float(nWidth - 1);			y2 = 0;
	x3 = x2;						y3 = float(nHeight - 1);
	x4 = x1;						y4 = y3;
	// 2015.5.19 Ϊ�˼��ټ��㣬�����Ǻ����ŵ�ѭ������
	float cos_theta = cos(angle);
	float sin_theta = sin(angle);
	// �ĸ�����˳ʱ����ת,��ͼ������
	PositionTransform(x1, y1, cos_theta, sin_theta, x0, y0);
	PositionTransform(x2, y2, cos_theta, sin_theta, x0, y0);
	PositionTransform(x3, y3, cos_theta, sin_theta, x0, y0);
	PositionTransform(x4, y4, cos_theta, sin_theta, x0, y0);
	int Xmin, Xmax, Ymin, Ymax;
	Xmax = int(FindMaxBetween4Numbers(x1, x2, x3, x4));
	Ymax = int(FindMaxBetween4Numbers(y1, y2, y3, y4));
	Xmin = int(FindMinBetween4Numbers(x1, x2, x3, x4));
	Ymin = int(FindMinBetween4Numbers(y1, y2, y3, y4));
	// ��ͼ���ȡ��߶ȡ�ÿ���ֽ����ı仯
	NewWidth = Xmax - Xmin + 1;
	NewHeight = Ymax - Ymin + 1;
	NewRowlen = nChannel * NewWidth;
	float *pDst = new float[NewRowlen * NewHeight];
	memset(pDst, 0, NewRowlen * NewHeight * sizeof(float));

#pragma omp parallel for
	for (int i = 0; i < NewWidth; ++i)
	{
		for (int j = 0; j < NewHeight; ++j)
		{
			float x = float(i + Xmin);
			float y = float(j + Ymin);
			PositionTransform(x, y, cos_theta, -sin_theta, x0, y0);
			for (int nCurChannel = 0; nCurChannel < nChannel; ++nCurChannel)
				pDst[nCurChannel + i * nChannel + j * NewRowlen] = biLinearInterp(pSrc, nWidth, nHeight, nRowlen, nChannel, nCurChannel, x, y);
		}
	}

	return pDst;
}


/* - �Ŵ�ͼ�� -
ԭʼͼ����Ϣ��float* pSrc, int nWidth, int nHeight, int nRowlen, int nChannel
Ŀ��ͼ��ߴ磺int NewWidth, int NewHeight
*/
float* ImageZoom(float* pSrc, int nWidth, int nHeight, int nRowlen, int nChannel, int NewWidth, int NewHeight)
{
	int NewRowlen = nChannel * NewWidth;
	float* pDst = new float[NewRowlen * NewHeight];
	float wRatio = 1.f * nWidth / NewWidth;
	float hRatio = 1.f * nHeight / NewHeight;

#pragma omp parallel for
	for (int i = 0; i < NewWidth; ++i)
	{
		for (int j = 0; j < NewHeight; ++j)
		{
			for (int nCurChannel = 0; nCurChannel < nChannel; ++nCurChannel)
				pDst[nCurChannel + i * nChannel + j * NewRowlen] = biLinearInterp(pSrc, nWidth, nHeight, nRowlen, nChannel, nCurChannel, i * wRatio, j * hRatio);
		}
	}

	return pDst;
}