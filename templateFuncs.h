#pragma once

/**
* @file templateFuncs.h
* @brief ����ģ��
* @details ��Ҫ������1��ͼ��˫���Բ�ֵ������2����ͼ���ȡ��������
*		3���ֽ����͸��������໥������4��������ݿ�������Сֵ��
*/

#include "DataTypes.h"

// ��ȡ����(x, y)����ֵ
template <typename Type> inline float GetPositionValue(const Type *pSrc, int nWidth, int nHeight, int nRowlen, int nChannel, 
													   int nCurChannel, int x, int y);

// ��ֵ������(x, y)����ֵ,˫���Բ�ֵ
template <typename Type> float biLinearInterp(const Type *pSrc, int nWidth, int nHeight, int nRowlen, int nChannel, 
											  int nCurChannel, float x, float y);

// ��ͼ���ȡ���������ڽ���
template <typename Type> char* GetDecodeString(const Type* pHead, const int nWidth, const int nHeight, const int nRowlen);

// ��Byte(uchar)���ݿ�����������
template <typename Type> void MemcpyByte2Float(float* pDst, const Type* pSrc, int nWidth, int nHeight, int nChannel);

// ���������ݿ�����BYTE(uchar)
template <typename Type> void MemcpyFloat2Byte(Type* pDst, const float* pSrc, int nWidth, int nHeight, int nChannel);

// �������ݿ�������Сֵ
template <typename Type> void MinMax(const Type* pSrc, int nWidth, int nHeight, Type &Min, Type &Max);


/** 
* @brief ��ȡ����(x, y)���ĺ���ֵ
* @param[in] *pSrc			ͼ������
* @param[in] nWidth			ͼ����
* @param[in] nHeight		ͼ��߶�
* @param[in] nRowlen		ͼ��ÿ���ֽ���
* @param[in] nChannel		ͼ��ͨ��
* @param[in] nCurChannel	��ǰͨ��
* @param[in] x				��ֵ����
* @param[in] y				��ֵ����
*/
template <typename Type> inline float GetPositionValue(const Type *pSrc, int nWidth, int nHeight, int nRowlen, int nChannel, 
													   int nCurChannel, int x, int y)
{
	if (x < 0 || x >= nWidth || y < 0 || y >= nHeight)
		return 0.0F;

	return float(*(pSrc + nCurChannel + x * nChannel + y * nRowlen));
}


/** 
* @brief ˫���Բ�ֵ��ȡ����(x, y) ���ĺ���ֵ
* @param[in] *pSrc			ͼ������
* @param[in] nWidth			ͼ����
* @param[in] nHeight		ͼ��߶�
* @param[in] nRowlen		ͼ��ÿ���ֽ���
* @param[in] nChannel		ͼ��ͨ��
* @param[in] nCurChannel	��ǰͨ��
* @param[in] x				��ֵ����
* @param[in] y				��ֵ����
*/
template <typename Type> float biLinearInterp(const Type *pSrc, int nWidth, int nHeight, int nRowlen, int nChannel, 
											  int nCurChannel, float x, float y)
{
	/** 
	// ˫���Բ�ֵ��ʾ��ͼ:
	// Ans2��		(x4, y4)				(x3, y3)
	//							(x, y)
	// Ans1��		(x1, y1)				(x2, y2)
	*/
	int x1, x3, y1, y3;
	x1 = int(x);	y1 = int(y);
	x3 = x1 + 1;	y3 = y1 + 1;
	// ��Խ��Ĵ���
	if (x1 < 0 || x3 >= nWidth || y1 < 0 || y3 >= nHeight)
		return 0.0F;

	float Ans1, Ans2;
	// ���½ǵĵ�
	const Type* pLB = pSrc + nCurChannel + x1 * nChannel + y1 * nRowlen;
	Ans1 = * pLB * (x3 - x) + *(pLB + nChannel) * (x - x1);
	Ans2 = *(pLB + nRowlen) * (x3 - x) + *(pLB + nChannel + nRowlen) * (x - x1);
	return (Ans1 * (y3 - y) + Ans2 * (y - y1));
}


/**
* @brief ��ý���������
* @param[in] *pHead			ͼ������
* @param[in] nWidth			ͼ����
* @param[in] nHeight		ͼ��߶�
* @param[in] nRowlen		ͼ��ÿ���ֽ���
@return 4ͨ����ͼ������(char)
@note Type����Ϊchar��BYTE
*/
template <typename Type> char* GetDecodeString(const Type* pHead, const int nWidth, const int nHeight, const int nRowlen)
{
	ASSERT(sizeof(Type) == 1);
	// ����ͼ�������λ��
	const int nBPP = nRowlen / nWidth * 8;
	// 4ͨ��ͼ���ÿ���ֽ���
	const int NewRowlen = nWidth * 4;
	// Ϊ����ֵ�����ڴ�
	char *pDst = new char[nHeight * NewRowlen];
	switch (nBPP)
	{
	case 8:// GRAY
#pragma omp parallel for
		for (int i = 0; i < nHeight; i++)
		{
			const Type* y1 = pHead + i * nRowlen;
			char* y2 = pDst + i * NewRowlen;
			for (int j = 0; j < nWidth; j++)
			{
				const Type* pSrc = y1 + j;
				char* pCur = y2 + 4 * j;
				* pCur = *(pCur+1) = *(pCur+2) = *pSrc;
				*(pCur+3) = 0;// Alpha
			}
		}
		break;
	case 24:// RGB
#pragma omp parallel for
		for (int i = 0; i < nHeight; i++)
		{
			const Type* y1 = pHead + i * nRowlen;
			char* y2 = pDst + i * NewRowlen;
			for (int j = 0; j < nWidth; j++)
			{
				const Type* pSrc = y1 + 3 * j;
				char* pCur = y2 + 4 * j;
				* pCur = *pSrc;
				*(pCur+1) = *(pSrc+1);
				*(pCur+2) = *(pSrc+2);
				*(pCur+3) = 0;// Alpha
			}
		}
		break;
	case 32:// RGBA
		memcpy(pDst, pHead, nHeight * NewRowlen * sizeof(char));
		break;
	default:
		SAFE_DELETE(pDst);
		return NULL;
	}
	return pDst;
}


/** 
* @brief ���������ݿ�����BYTE. 
* @param[in] *pDst			�ֽ�����
* @param[in] *pSrc			��������
* @param[in] nWidth			ͼ����
* @param[in] nHeight		ͼ��߶�
* @param[in] nChannel		ͼ��ͨ��
*/
template <typename Type> void MemcpyFloat2Byte(Type* pDst, const float* pSrc, int nWidth, int nHeight, int nChannel)
{
	// BYTEÿ���ֽ���
	const int nRowlen = WIDTHBYTES(nWidth * nChannel * 8);
	// ��������ÿ�и���
	const int nFloatRowlen = nWidth * nChannel;

#pragma omp parallel for
	for (int i = 0; i < nWidth; ++i)
	{
		int x = i * nChannel;
		for (int j = 0; j < nHeight; ++j)
		{
			int y1 = j * nFloatRowlen, y2 = j * nRowlen;
			for (int k = 0; k < nChannel; ++k)
				pDst[k + x + y2] = (Type)pSrc[k + x + y1];
		}
	}
}


/** 
* @brief ��BYTE��������������
* @param[in] *pDst			��������
* @param[in] *pSrc			�ֽ�����
* @param[in] nWidth			ͼ����
* @param[in] nHeight		ͼ��߶�
* @param[in] nChannel		ͼ��ͨ��
*/
template <typename Type> void MemcpyByte2Float(float* pDst, const Type* pSrc, int nWidth, int nHeight, int nChannel)
{
	// BYTE����ÿ���ֽ���
	const int nRowlen = WIDTHBYTES(nWidth * nChannel * 8);
	// ��������ÿ�и���
	const int nFloatRowlen = nWidth * nChannel;

#pragma omp parallel for
	for (int i = 0; i < nWidth; ++i)
	{
		int x = i * nChannel;
		for (int j = 0; j < nHeight; ++j)
		{
			int y1 = j * nRowlen, y2 = j * nFloatRowlen;
			for (int k = 0; k < nChannel; ++k)
				pDst[k + x + y2] = (float)pSrc[k + x + y1];
		}
	}
}


/** 
* @brief �������ݿ�������Сֵ
* @param[in] *pSrc			ͼ������
* @param[in] nWidth			ͼ����
* @param[in] nHeight		ͼ��߶�
* @param[in] &Min			��Сֵ
* @param[in] &Max			���ֵ
* @return ͨ��Min��Max�����С���ֵ
*/
template <typename Type> void MinMax(const Type* pSrc, int nWidth, int nHeight, Type &Min, Type &Max)
{
	// Ѱ�������Сֵ
	pair<Type*, Type*> minmax_pair = minmax_element(pSrc, pSrc + nWidth * nHeight);
	Min = *minmax_pair.first;
	Max = *minmax_pair.second;
}