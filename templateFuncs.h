#pragma once

/**
* @file templateFuncs.h
* @brief ����ģ��
*/

#include "DataTypes.h"

// ��ȡ����(x, y)����ֵ
template <typename Type> inline float GetPositionValue(const Type *pSrc, int nWidth, int nHeight, int nRowlen, int nChannel, int nCurChannel, int x, int y);

// ��ֵ������(x, y)����ֵ,˫���Բ�ֵ
template <typename Type> float biLinearInterp(const Type *pSrc, int nWidth, int nHeight, int nRowlen, int nChannel, int nCurChannel, float x, float y);

// ��ͼ���ȡ���������ڽ���
template <typename Type> char* GetDecodeString(const Type* pHead, const int nWidth, const int nHeight, const int nRowlen);

// ��Byte(uchar)���ݿ�����������
template <typename Type> void MemcpyByte2Float(float* pDst, const Type* pSrc, int nWidth, int nHeight, int nChannel);

// ���������ݿ�����BYTE(uchar)
template <typename Type> void MemcpyFloat2Byte(Type* pDst, const float* pSrc, int nWidth, int nHeight, int nChannel);

// ����ΪnWidth����ΪnHeight������д���ļ�(�Ƕ�������ʽ)
template <typename Type> BOOL WriteVector(const char* filePath, const Type* pSrc, int nWidth, int nHeight);

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
template <typename Type> inline float GetPositionValue(const Type *pSrc, int nWidth, int nHeight, int nRowlen, int nChannel, int nCurChannel, int x, int y)
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
template <typename Type> float biLinearInterp(const Type *pSrc, int nWidth, int nHeight, int nRowlen, int nChannel, int nCurChannel, float x, float y)
{
	/** 
	// ˫���Բ�ֵ��ʾ��ͼ:
	// Ans2��		(x4, y4)				(x3, y3)
	//							(x, y)
	// Ans1��		(x1, y1)				(x2, y2)
	*/
	int x1, x2, x3, x4, y1, y2, y3, y4;
	float Ans1, Ans2;
	x1 = int(x);	y1 = int(y);
	x2 = x1 + 1;	y2 = y1;
	x3 = x2;		y3 = y1 + 1;
	x4 = x1;		y4 = y3;
	// ��Խ��Ĵ���
	if (x1 < 0 || x3 >= nWidth || y1 < 0 || y3 >= nHeight)
		return 0.0F;

	Ans1 = *(pSrc + nCurChannel + x1 * nChannel + y1 * nRowlen) * (x2 - x) 
		+ *(pSrc + nCurChannel + x2 * nChannel + y2 * nRowlen) * (x - x1);
	Ans2 = *(pSrc + nCurChannel + x4 * nChannel + y4 * nRowlen) * (x3 - x) 
		+ *(pSrc + nCurChannel + x3 * nChannel + y3 * nRowlen) * (x - x4);
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
			for (int j = 0; j < nWidth; j++)
			{
				pDst[    4 * j + i * NewRowlen] = pHead[j + i * nRowlen];
				pDst[1 + 4 * j + i * NewRowlen] = pHead[j + i * nRowlen];
				pDst[2 + 4 * j + i * NewRowlen] = pHead[j + i * nRowlen];
				pDst[3 + 4 * j + i * NewRowlen] = 0;// Alpha
			}
		}
		break;
	case 24:// RGB
#pragma omp parallel for
		for (int i = 0; i < nHeight; i++)
		{
			for (int j = 0; j < nWidth; j++)
			{
				pDst[    4 * j + i * NewRowlen] = pHead[	3 * j + i * nRowlen];
				pDst[1 + 4 * j + i * NewRowlen] = pHead[1 + 3 * j + i * nRowlen];
				pDst[2 + 4 * j + i * NewRowlen] = pHead[2 + 3 * j + i * nRowlen];
				pDst[3 + 4 * j + i * NewRowlen] = 0;// Alpha
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
		for (int j = 0; j < nHeight; ++j)
		{
			for (int k = 0; k < nChannel; ++k)
				pDst[k + i * nChannel + j * nRowlen] = (Type)pSrc[k + i * nChannel + j * nFloatRowlen];
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
		for (int j = 0; j < nHeight; ++j)
		{
			for (int k = 0; k < nChannel; ++k)
				pDst[k + i * nChannel + j * nFloatRowlen] = (float)pSrc[k + i * nChannel + j * nRowlen];
		}
	}
}


/** 
* @brief ������д���ļ�
@ details ����ΪnWidth����ΪnHeight�������Ը�����ʽд���ļ�
* @param[in] *filePath		�ļ���
* @param[in] *pSrc			ͼ������
* @param[in] nWidth			ͼ����
* @param[in] nHeight		ͼ��߶�
* @return �ɹ���ʧ��
* @note д���ļ�������ݿ���ֱ�ӱ�MATLAB��ȡ��������
		Mat = load('test.txt');
		�ú�����Ŀ������ MATLAB ��������Ƿ���ȷ
*/
template <typename Type> BOOL WriteVector(const char* filePath, const Type* pSrc, int nWidth, int nHeight)
{
	ofstream InputFile(filePath);
	if (InputFile)
	{
		for (int i = 0; i < nHeight; i++)
		{
			for (int j = 0; j < nWidth - 1; j++)
			{
				InputFile << (float)pSrc[j + i * nWidth] << ",";
			}
			// ÿ�����һ������
			InputFile << (float)pSrc[nWidth - 1 + i * nWidth] << endl;
		}
		InputFile.close();
		return TRUE;
	}
	return FALSE;
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