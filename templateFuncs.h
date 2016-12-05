/************************************************************************
* @file 
* @details ģ�庯��,���Կ�ƽ̨.
* @author 
* @email 
* @version V1.0
* @date 2016/10/4
* Copyleft (c), All rights reserved.
* @note �����Ӵ˰���"algorithm.h"�ļ���������Eclipse����
	* error: 'sort' is not a member of 'std'
	* ������**����ָ����Ϊ�������������޸Ĵ�һ��ָ��
************************************************************************/

#include "DataTypes.h"
#include <ctime>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace  std;

#pragma once

// ��ͼ��д���ļ����ļ�ͷ��С���ֽڣ�
#define HEADER_SIZE 1024

// ��ȡ����(x, y)����ֵ
template <typename Type> inline float GetPositionValue(Type *pSrc, int nWidth, int nHeight, int nRowlen, int nChannel, int nCurChannel, int x, int y);

// ��ֵ������(x, y)����ֵ,˫���Բ�ֵ
template <typename Type> float biLinearInterp(Type *pSrc, int nWidth, int nHeight, int nRowlen, int nChannel, int nCurChannel, float x, float y);

// �Ŵ�ͼ�񣺻��޸�pfHead������ͼ����Ϣ
template <typename Type> void ZoomImage(Type** pfHead, int &nWidth, int &nHeight, int &nRowlen, int nChannel, int NewWidth, int NewHeight);

// ����ͼ�����߳���������ʱ�޸Ĵ�С
template <typename Type> void LimitImage(Type** pfHead, int &nWidth, int &nHeight, int &nRowlen, int nChannel, int nMaxSize = 320);

// ���·�תͼ��
template <typename Type> void ImageFlipV(Type* pHead, const int nWidth, const int nHeight, const int nRowlen);

// ���ҷ�תͼ��
template <typename Type> void ImageFlipH(Type* pHead, const int nWidth, const int nHeight, const int nRowlen);

// ת��ͼ��
template <typename Type> void ImageTranspose(Type** pHead, const int nWidth, const int nHeight, const int nRowlen);

// ��ȡͼ�����Ȥ����
template <typename Type> Type* ImageROI(Type* pHead, int &nWidth, int &nHeight, int &nRowlen, RoiRect roi = RoiRect(0, 0, 0, 0));

// ��ͼ���Զ�������ʽд���ļ�
template <typename Type> BOOL ImageWrite(const char* fileName, Type* pSrc, int nWidth, int nHeight, int nRowlen);

// ��ͼ���ȡ���������ڽ���
template <typename Type> char* GetDecodeString(Type* pHead, const int nWidth, const int nHeight, const int nRowlen);

// ��Byte(uchar)���ݿ�����������
template <typename Type> void MemcpyByte2Float(float* pDst, Type* pSrc, int nWidth, int nHeight, int nChannel);

// ���������ݿ�����BYTE(uchar)
template <typename Type> void MemcpyFloat2Byte(Type* pDst, float* pSrc, int nWidth, int nHeight, int nChannel);

// ����ΪnWidth����ΪnHeight������д���ļ�(�Ƕ�������ʽ)
template <typename Type> BOOL WriteVector(const char* filePath, Type* pSrc, int nWidth, int nHeight);

// �������ݿ�������Сֵ
template <typename Type> void MinMax(Type* pSrc, int nWidth, int nHeight, Type &Min, Type &Max);

/* K_means */

/************************************************************************/
/*
     ���ߣ�Ԭ����
	 ʱ�䣺2015��12��3��
	 ���䣺yuan_yuanxiang@163.com
*/
/************************************************************************/

// ��ɫͼ��תΪ8λ�ĵ�ͨ��ͼ��
template <typename Type> Type* Rgb2Gray(Type* pHead, int nWidth, int nHeight, int nRowBytes);

// ��ͼ�������K_means�������ָ�
template <typename Type> void K_means(Type* pHead, int nRowBytes, RoiRect roi, int K, float fBackground, int nMaxepoches);

// ����������(2016/9/19)
template <typename Type> float Difference(Type *Center, int K);

// ��ʼ����������(2016/9/19)
template <typename Type> void InitializeClusterCenters(float *Center, int K, Type* pHead, int nRowBytes, RoiRect roi);

// ��ʼ����������(random���)
template <typename Type> void RandomClusterCenters(float *Center, int K, Type* pHead, int nRowBytes, RoiRect roi);

// ͳ�ƻҶ�ͼ���ֱ��ͼ
template <typename Type> void ImageHistogram(Type* pHead, int nRowlen, int nHist[256], RoiRect roi);

/************************************************************************/
/*   ��ֵ�˲�                                                           */
/*   �ο���http://blog.csdn.net/mfcing/article/details/43817715         */
/************************************************************************/

// ��������
template <typename Type> void InsertionSort(Type *pArray, int Num);

// ��ֵ�˲�
template <typename Type> BOOL MedianFilter(Type* pData, int nWidth, int nHeight, int nRowlen, int nSize = 3);

// ������ֵ�˲�
template <typename Type> BOOL FastMedianFilter(Type* pData, int nWidth, int nHeight, int nRowlen);

// ����Ӧ��ֵ�˲�
template <typename Type> BOOL SapMedianFilter(Type* pData, int nWidth, int nHeight, int nRowlen, int nMaxSize = 7);

template <typename Type> void SapGetMinMedMax(Type* pData, int nWidth, int nHeight, int nRowlen, int nChannel, 
											 Type *pCopy, int r, int c, rgb<Type> *temp, int nSize, int nMaxSize);

/************************************************************************/
/* ********************** ģ�庯����ʵ�� ********************************/
/************************************************************************/

/** @brief ���·�תͼ��. 
* @param[in] pHead ����ͷ
* @param[in] nWidth ���
* @param[in] nHeight �߶�
* @param[in] nRowlen ÿ�и���
@return void
*/
template <typename Type> void ImageFlipV(Type* pHead, const int nWidth, const int nHeight, const int nRowlen)
{
	// ��������
	Type* temp = new Type[nHeight * nRowlen * sizeof(Type)];
	memcpy(temp, pHead, nHeight * nRowlen * sizeof(Type));

#pragma omp parallel for
	for (int r = 0; r < nHeight; ++r)
	{
		memcpy(pHead + r * nRowlen, temp + (nHeight - 1 - r) * nRowlen, nRowlen * sizeof(Type));
	}
	SAFE_DELETE(temp);
}


/* - ���ҷ�תͼ�� - 
Type* pHead������ͷ
const int nWidth�����
const int nHeight���߶�
const int nRowlen��ÿ�и���
*/
template <typename Type> void ImageFlipH(Type* pHead, const int nWidth, const int nHeight, const int nRowlen)
{
	Type* temp = new Type[nHeight * nRowlen];
	memcpy(temp, pHead, nHeight * nRowlen * sizeof(Type));

	int nChannel = nRowlen / nWidth;

#pragma omp parallel for
	for (int r = 0; r < nHeight; ++r)
	{
		for (int c = 0; c < nWidth; ++c)
		{
			memcpy(pHead + c *nChannel + r * nRowlen, temp + (nWidth - 1 - c) * nChannel + r * nRowlen, nChannel * sizeof(Type));
		}
	}
	SAFE_DELETE(temp);
}


/* - ���ҷ�תͼ�� - 
Type** pHead������ͷ
const int nWidth�����
const int nHeight���߶�
const int nRowlen��ÿ�и���
* @note �������޸�ͼ��ָ��
*/
template <typename Type> void ImageTranspose(Type** pHead, const int nWidth, const int nHeight, const int nRowlen)
{
	// ͼ��ÿ����Ԫ�ظ���
	int nChannel = nRowlen / nWidth;
	// ÿ��Ԫ�ظ���
	int nNewRowlen = nHeight * nChannel;
	// �Ƿ���Ҫ���ж��봦��
	if (1 == sizeof(Type))
	{
		nNewRowlen = WIDTHBYTES(nNewRowlen * 8);
	}

	Type *pSrc = *pHead;
	Type *temp = new Type[nWidth * nNewRowlen];
#pragma omp parallel for
	for (int i = 0; i < nWidth; ++i)
	{
		for (int j = 0; j < nHeight; ++j)
		{
			for (int k = 0; k < nChannel; ++k)
			{
				temp[k + j * nChannel + i * nNewRowlen] = pSrc[k + i * nChannel + j * nRowlen];
			}
		}
	}
	delete [] *pHead;
	*pHead = temp;
	temp = NULL;
}


/** ��ȡͼ�����Ȥ����
Type* pHead��ͼ��ͷָ��
int &nWidth��ͼ����
int &nHeight��ͼ��߶�
int &nRowlen��ͼ��ÿ��Ԫ�ظ���
RoiRect roi��ͼ�����Ȥ����
* @return ��������ͼ�����Ȥ����(ָ�룬��ҪSAFE_DELETE)
*/
template <typename Type> Type* ImageROI(Type* pHead, int &nWidth, int &nHeight, int &nRowlen, RoiRect roi)
{
	/// ���roi�Ƿ�
	if (roi.Width() <= 0 || roi.Height() <= 0 || roi.right > nWidth || roi.bottom > nHeight)
	{
		Type* pDst = new Type[nHeight * nRowlen];
		memcpy(pDst, pHead, nHeight * nRowlen * sizeof(Type));
		return pDst;
	}

	int nChannel = nRowlen / nWidth;
	int nNewWidth = roi.Width();
	int nNewHeight = roi.Height();
	int nNewRowlen = nNewWidth * nChannel;
	// �Ƿ���Ҫ���ж��봦��
	if (1 == sizeof(Type))
	{
		nNewRowlen = WIDTHBYTES(nNewRowlen * 8);
	}

	Type* pDst = new Type[nNewHeight * nNewRowlen];

#pragma omp parallel for
	for (int i = 0; i < nNewHeight; ++i)
	{
		int nLine = nHeight - roi.top - i - 1;
		memcpy(pDst + i * nNewRowlen, pHead + roi.left * nChannel + nLine * nRowlen, nNewRowlen * sizeof(Type));
	}

	// ����ͼ����Ϣ
	nWidth = nNewWidth;
	nHeight = nNewHeight;
	nRowlen = nNewRowlen;
	return pDst;
}


/** ��ͼ���Զ�������ʽд���ļ� 
const char* fileName:�ļ�����
Type* pSrc:ͼ������Դ
int nWidth:ͼ����
int nHeight:ͼ��߶�
int nRowlen:ÿ��Ԫ�ظ���
�ο���FileIO�е�WriteTxt/ReadTxt����
*/
template <typename Type> BOOL ImageWrite(const char* fileName, Type* pSrc, int nWidth, int nHeight, int nRowlen)
{
	ofstream fout;
	fout.open(fileName, ofstream::binary);

	if (fout.is_open())
	{
		try
		{
			// д�ļ�ͷ����СHEADER_SIZE��
			char* pHead = "yuanyuanxiang";
			int nLen = strlen(pHead);
			fout.write(pHead, nLen);
			int nChannel = nRowlen / nWidth;
			int nInt = sizeof(int);
			fout.write((char*)&nRowlen, nInt);
			fout.write((char*)&nHeight, nInt);
			fout.write((char*)&nChannel, nInt);
			fout.seekp(HEADER_SIZE);
			// дͼ������
			fout.write((char*)pSrc, nHeight * nRowlen);
			fout.close();
			return TRUE;
		}
		catch (...)
		{
			fout.close();
			return FALSE;
		}
	}
	return FALSE;
}


/* - �Ŵ�ͼ�� -
Type* pfHead��ͼ��ͷָ��
int &nWidth��ͼ����
int &nHeight��ͼ��߶�
int &nRowlen��ͼ��ÿ���ֽ���
int nChannel��ͼ��ͨ��
int NewWidth��ͼ���¿��
int NewHeight��ͼ���¸߶�
��ע���������޸�pfHeadָ������ݣ���Ϊ���
*/
template <typename Type> void ZoomImage(Type** pfHead, int &nWidth, int &nHeight, int &nRowlen, int nChannel, int NewWidth, int NewHeight)
{
	if (nWidth == NewWidth && nHeight == NewHeight)
		return;

	int NewRowlen = NewWidth * nChannel;
	// �Ƿ���Ҫ���ж��봦��
	if (1 == sizeof(Type))
	{
		NewRowlen = WIDTHBYTES(NewRowlen * 8);
	}

	Type* pDst = new Type[NewRowlen * NewHeight];
	float wRatio = 1.f * nWidth / NewWidth;
	float hRatio = 1.f * nHeight / NewHeight;

#pragma omp parallel for
	for (int i = 0; i < NewWidth; ++i)
	{
		for (int j = 0; j < NewHeight; ++j)
		{
			for (int nCurChannel = 0; nCurChannel < nChannel; ++nCurChannel)
				pDst[nCurChannel + i * nChannel + j * NewRowlen] = 
					(Type)biLinearInterp(*pfHead, nWidth, nHeight, nRowlen, nChannel, nCurChannel, i * wRatio, j * hRatio);
		}
	}

	// �޸�pHeadָ�������
	delete [] *pfHead;
	*pfHead = pDst;
	pDst = NULL;

	// ����ͼ����Ϣ
	nWidth = NewWidth;
	nHeight = NewHeight;
	nRowlen = NewRowlen;
}


/* - ����ͼ�����߳� -
ͼ��ָ�룺Type** pfHead
ͼ���ȣ�int &nWidth
ͼ��߶ȣ�int &nHeight
ͼ��ÿ���ֽ�����int &nRowlen
ͼ��ͨ����int nChannel
���߳���int nMaxSize = 320
��ע����ͼ����һ�߳��ȳ�������ʱ�������Ų�������Ϣ
��Ҫ����ȡ��߶ȡ�ÿ���ֽ����ᱻ�޸�
*/
template <typename Type> void LimitImage(Type** pfHead, int &nWidth, int &nHeight, int &nRowlen, int nChannel, int nMaxSize)
{
	float rate = 0.0f;
	if (nWidth > nHeight)
	{
		if (nWidth > nMaxSize)
		{
			rate = 1.f * nMaxSize / nWidth;
			ZoomImage(pfHead, nWidth, nHeight, nRowlen, nChannel, nMaxSize, int(rate * nHeight));
		}
	}
	else
	{
		if (nHeight > nMaxSize)
		{
			rate = 1.f * nMaxSize / nHeight;
			ZoomImage(pfHead, nWidth, nHeight, nRowlen, nChannel, int(rate * nWidth), nMaxSize);
		}
	}
}


// ��ȡ����(x, y)���ĺ���ֵ
template <typename Type> inline float GetPositionValue(Type *pSrc, int nWidth, int nHeight, int nRowlen, int nChannel, int nCurChannel, int x, int y)
{
	if (x < 0 || x >= nWidth || y < 0 || y >= nHeight)
		return 0.0F;

	return float(*(pSrc + nCurChannel + x * nChannel + y * nRowlen));
}


/* - ˫���Բ�ֵ -
 * ��ȡ����(x, y) ���ĺ���ֵ
ͼ����Ϣ��Type *pSrc, int nWidth, int nHegiht, int nRowlen, int nChannel
��ǰͨ����int nCurChannel
��ֵλ�ã�float x, float y
*/
template <typename Type> float biLinearInterp(Type *pSrc, int nWidth, int nHeight, int nRowlen, int nChannel, int nCurChannel, float x, float y)
{
	/************************************************************************
	// ˫���Բ�ֵ��ʾ��ͼ:
	// Ans2��		(x4, y4)				(x3, y3)
	//							(x, y)
	// Ans1��		(x1, y1)				(x2, y2)
	************************************************************************/
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


/** - ��ý��������� -
Type* pHead				ͼ��ָ��
const int nWidth		ͼ����
const int nHeight		ͼ��߶�
const int nRowlen		ÿ���ֽ���
@return 4ͨ����ͼ������(char)
@note Type����Ϊchar��BYTE
*/
template <typename Type> char* GetDecodeString(Type* pHead, const int nWidth, const int nHeight, const int nRowlen)
{
	// ����ͼ�������λ��
	int nBPP = nRowlen / nWidth * 8;
	// 4ͨ��ͼ���ÿ���ֽ���
	int NewRowlen = nWidth * 4;
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


/** ���������ݿ�����BYTE. 
Type* pDst��Ŀ������
float* pSrc��ԭʼ����
int nWidth�����
int nHeight���߶�
int nChannel��ͨ��
*/
template <typename Type> void MemcpyFloat2Byte(Type* pDst, float* pSrc, int nWidth, int nHeight, int nChannel)
{
	// BYTEÿ���ֽ���
	int nRowlen = WIDTHBYTES(nWidth * nChannel * 8);
	// ��������ÿ�и���
	int nFloatRowlen = nWidth * nChannel;

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


/* - ��BYTE�������������� -
float* pDst��Ŀ������
Type* pSrc��ԭʼ����
int nWidth�����
int nHeight���߶�
int nChannel��ͨ��
*/
template <typename Type> void MemcpyByte2Float(float* pDst, Type* pSrc, int nWidth, int nHeight, int nChannel)
{
	// BYTE����ÿ���ֽ���
	int nRowlen = WIDTHBYTES(nWidth * nChannel * 8);
	// ��������ÿ�и���
	int nFloatRowlen = nWidth * nChannel;

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


/* - ������д���ļ� - 
����ΪnWidth����ΪnHeight�������Ը�����ʽд���ļ�
������4��
	const char* filePath����д����ļ���
	Type* pSrc������Դ��ͷָ��
	int nWidth������Դ�Ŀ��(ÿ��Ԫ�ظ���)
	int nHeight������Դ�ĸ߶�
����ֵ���ɹ���ʧ��
��ע��д���ļ�������ݿ���ֱ�ӱ�MATLAB��ȡ��������
	Mat = load('test.txt');
 * ע���ú�����Ŀ������ MATLAB ��������Ƿ���ȷ
*/
template <typename Type> BOOL WriteVector(const char* filePath, Type* pSrc, int nWidth, int nHeight)
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


/* - �������ݿ�������Сֵ - 
������4��
	Type* pSrc������Դ
	int nWidth������Դ�Ŀ��(ÿ��Ԫ�ظ���)
	int nHeight������Դ�ĸ߶�
	Type &Min����Сֵ
	Type &Max�����ֵ
����ֵ��ͨ��Min��Max�����С���ֵ
*/
template <typename Type> void MinMax(Type* pSrc, int nWidth, int nHeight, Type &Min, Type &Max)
{
	// Ѱ�������Сֵ
	pair<Type*, Type*> minmax_pair = minmax_element(pSrc, pSrc + nWidth * nHeight);
	Min = *minmax_pair.first;
	Max = *minmax_pair.second;
}


/** ��ɫת�ڰ�.
* @param[in] pHead ͼ������
* @param[in] nWidth ͼ����
* @param[in] nHeight ͼ��߶�
* @param[in] nRowBytes ÿ���ֽ���
* @return ���������ػҶ�ͼ��
* @note ���޶�char/BYTE���д���
* @warning ��Ҫ�Է���ֵ����delete����.
*/
template <typename Type> Type* Rgb2Gray(Type* pHead, int nWidth, int nHeight, int nRowBytes)
{
	ASSERT(1 == sizeof(Type));
	const int nChannel = nRowBytes / nWidth;
	const int nChannelNew = 1;
	const int nRowLenNew = WIDTHBYTES(nWidth * 8);
	Type *pDst = new Type[nHeight * nRowLenNew];

	switch (nChannel)
	{
	case 1:
		memcpy(pDst, pHead, nHeight * nRowBytes);
		break;
	case 3:
	case 4:// 3��4ͨ��������һ����
#pragma omp parallel for
		for (int i = 0; i < nHeight; i++)
		{
			for (int j = 0; j < nWidth; j++)
			{
				Type *pSrcTemp = pHead + nChannel * j + i * nRowBytes;
				Type *pDstTemp = pDst + nChannelNew * j + i * nRowLenNew;
				// ����ǿ��ת��Ϊ�޷����ַ�������(2016/9/12 ע)
				BYTE R = *(pSrcTemp + 2);
				BYTE G = *(pSrcTemp + 1);
				BYTE B = *pSrcTemp;
				*pDstTemp = BYTE ((9798 * R + 19235 * G + 3735 * B) / 32768);
			}
		}
		break;
	default:
		SAFE_DELETE(pDst);
		return NULL;
	}
	return pDst;
}


/** ͳ�ƻҶ�ͼ���ֱ��ͼ. 
*/
template <typename Type> void ImageHistogram(Type* pHead, int nRowlen, int nHist[256], RoiRect roi)
{
	memset(nHist, 0, 256 * sizeof(int));
	for (int i = roi.top; i < roi.bottom; ++i)
	{
		for (int j = roi.left; j < roi.right; ++j)
		{
			int index = (BYTE)pHead[j + i * nRowlen];
			++nHist[index];
		}
	}
}


/** @brief ����������(2016/9/19). 
*/
template <typename Type> float Difference(Type *Center, int K)
{
	float sum = 0.0f;
	for (int i = 0; i < K; ++i)
	{
		for (int j = i; j < K; ++j)
		{
			sum += fabs(1.0f * (Center[j] - Center[i]));
		}
	}
	return sum;
}


/** @brief ��ʼ����������(2016/9/19). 
* @param[in] *Center ��������
* @param[in] K �������
* @param[in] *pHead ͼ������
* @param[in] nRowBytes ͼ��ÿ��Ԫ�ظ���
* @param[in] roi ͼ�����Ȥ����
* ��@ref ��������ͼ��ֱ��ͼ��ȡ��ֵ���ĻҶ�Ϊ����.
*/
template <typename Type> void InitializeClusterCenters(float *Center, int K, Type* pHead, int nRowBytes, RoiRect roi)
{
#ifdef _DEBUG
	TRACE(" * ���ڳ�ʼ����������...\n");
#endif // _DEBUG
	// ��ȡֱ��ͼ
	int nHist[256];
	ImageHistogram(pHead, nRowBytes, nHist, roi);
	int index[256];
	for (int i = 0; i < 256; ++i)
	{
		index[i] = i;
	}
	// ��������,ʹ�Ҷ�ֵ��ֵ����ǰͷ
	for (int i = 1; i < 256; ++i)
	{
		for (int j = i; j > 0 && nHist[j] > nHist[j-1]; --j)
		{
			int t;
			t = nHist[j]; nHist[j] = nHist[j-1]; nHist[j-1] = t;
			t = index[j]; index[j] = index[j-1]; index[j-1] = t;
		}
	}
	// Ѱ��K�����ģ���Щ���ĵļ�಻��С��min_diff
	int num;
	float min_diff = 25.0f;
	do 
	{
		num = 0;
		for (int i = 0; i < K; ++i)
		{
			int flag = 0;
			do
			{
				Center[i] = (float)index[num];
				int j = 0;
				for ( ; j < i; ++j)
				{
					// ��ĳ���������ĵļ���С
					if (fabs(Center[i] - Center[j]) < min_diff)
						break;
				}
				// Cj��ȫ��Ci�ľ���ϴ�
				if (j == i)
					flag = 1;
				++num;
			}while(num < 256 && flag == 0);
#ifdef _DEBUG
			TRACE("C[%d] = %.6f\t", i, Center[i]);
#endif // _DEBUG
		}
#ifdef _DEBUG
		TRACE("\n");
#endif // _DEBUG
		min_diff -= 4.0f;
	}while(num == 256);
}


/** �������K������. 
* @param[in] *Center ��������
* @param[in] K �������
* @param[in] *pHead ͼ������
* @param[in] nRowBytes ͼ��ÿ��Ԫ�ظ���
* @param[in] roi ͼ�����Ȥ����
*/
template <typename Type> void RandomClusterCenters(float *Center, int K, Type* pHead, int nRowBytes, RoiRect roi)
{
#ifdef _DEBUG
	TRACE(" * ���ڳ�ʼ����������...\n");
#endif // _DEBUG
	// �������K������
	unsigned int seed = (unsigned int)time(NULL);
	for (int i = 0; i < K; i++)
	{
		srand(seed);
		int w = roi.left + rand() % roi.Width();
		int h = roi.top + rand() % roi.Height();
		// BYTE ǿ��ת���Ǳ���ģ���ͬ(2016/9/12)
		Center[i] = (BYTE)pHead[w + h * nRowBytes];
#ifdef _DEBUG
		TRACE("C[%d] = %.6f\t", i, Center[i]);
#endif // _DEBUG
		seed = rand();
	}
#ifdef _DEBUG
	TRACE("\n");
#endif // _DEBUG
}


/** @brief K_means ����ָ�. 
* @param[in] pHead ͼ������
* @param[in] nRowBytes ÿ���ֽ���
* @param[in] roi ͼ�����Ȥ����
* @param[in] K �������
* @param[in] nMaxepoches ����������
* @note (1)�����ǻҶ�ͼ��;(2)�˺������޸�ͼ������@ref pHead.
*/
template <typename Type> void K_means(Type* pHead, int nRowBytes, RoiRect roi, int K, float fBackground, int nMaxepoches)
{
	// ��������ĳߴ�
	int nWidth = roi.Width();
	int nHeight = roi.Height();
	int nRowlen = nWidth;// ������ÿ��Ԫ�ظ���
	Type* Cluster = new Type[nHeight * nRowlen];// ������TAG
	// ��������
	float *Center = new float[K];
	float *oldCenter = new float[K];
	int *sumCenter = new int[K];// �����������
	int *sum = new int[K];// ÿ����������Ԫ�ظ���
	// ����K������
	InitializeClusterCenters(Center, K, pHead, nRowBytes, roi);
	/* ��ε���ֱ�������������������nMaxepoches�� */
	for (int it = 0; it < nMaxepoches; ++it)
	{
		ZeroMemory(sumCenter, K * sizeof(int));
		ZeroMemory(sum, K * sizeof(int));
		/* ���ÿ���������Ӧ��������һ������ */
		for (int j = roi.top; j < roi.bottom; ++j)
		{
			Type *pRowj = pHead + j * nRowBytes;
			for (int i = roi.left; i < roi.right; ++i)
			{
				/* ��ǰ���صĸ���ֵ */
				int curPixel = (BYTE)pRowj[i];
				/* ����ʼ�����ڵ�0������ */    
				int tag = 0;
				float temp = curPixel - Center[tag];
				float min_distance = (temp > 0.0F ? temp : -temp);
				for (int s = 1; s < K; s++)
				{
					temp = curPixel - Center[s];
					float new_distance = (temp > 0.0F ? temp : -temp);
					if (new_distance < min_distance)
					{
						min_distance = new_distance;
						tag = s;
					}
				}
				// ��(i, j)���Ϊtag
				Cluster[(i-roi.left) + (j-roi.top) * nRowlen] = (Type)tag;
				sumCenter[tag] += curPixel;
				++sum[tag];
			}
		}
		/* ������һ������������¾������� */
		memcpy(oldCenter, Center, K * sizeof(float));
		float diff = 0.F;
		for (int i = 0; i < K; ++i)
		{
			if (sum[i])
			{
				Center[i] = (float)sumCenter[i] / sum[i];
			}
			diff += (oldCenter[i] - Center[i]) * (oldCenter[i] - Center[i]);
		}
		// ע������������Լ��ӵ�������
#ifdef _DEBUG
		TRACE(" * �������� = %d��\n", it + 1);
		for (int i = 0; i < K; ++i)
		{
			TRACE("C[%d] = %.6f\t%d��\t", i, Center[i], sum[i]);
		}
		TRACE("\n");
#endif // _DEBUG
		// ��ǰ�������������ı仯��Сʱ��0.01�����أ��˳�
		if (diff < 0.0003f)
			break;
	}
	
	// Ѱ�ұ���ֵ��Ӧ�����
	int backValIdx = 0;
	float min_dis = fabs(Center[0] - fBackground);
	for (int i = 1; i < K; ++i)
	{
		float temp_dis = fabs(Center[i] - fBackground);
		if (temp_dis < min_dis)
		{
			backValIdx = i;
			min_dis = temp_dis;
		}
	}

	// ��ԭʼͼ����зָ�
	for (int j = 0; j < nHeight; ++j)
	{
		for (int i = 0; i < nWidth; ++i)
		{
			pHead[i + roi.left + (j + roi.top) * nRowBytes] = (Cluster[i + j * nRowlen] == backValIdx) ? 0 : 255;
		}
	}
	delete [] Center;
	delete [] oldCenter;
	delete [] sumCenter;
	delete [] sum;
	delete [] Cluster;
}

// ����Ϊ1�������ÿ�������
#define QUICK_SORT 1

/** ��ֵ�˲�. 
* @details ��ֵ�˲��ȽϺ�ʱ.
* @param[in] * pData ͼ������
* @param[in] nWidth ͼ����
* @param[in] nHeight ͼ��߶�
* @param[in] nRowlen ͼ��ÿ���ֽ���
* @param[in] nSize �˲��˳ߴ�
* @warning ���޲�ɫͼ��
* �ڽ�������Ϊ��ʱ.
*/
template <typename Type> BOOL MedianFilter(Type* pData, int nWidth, int nHeight, int nRowlen, int nSize)
{
	// �˲��˳ߴ����Ϊ����
	ASSERT(nSize & 0x00000001);
	Type *pCopy = new Type[nHeight * nRowlen];
	memcpy(pCopy, pData, nHeight * nRowlen * sizeof(Type));
	int nChannel = nRowlen / nWidth;
	ASSERT(nChannel >= 3);
	int K = nSize >> 1;// �˲��˰뾶
	rgb<Type> *temp = new rgb<Type>[nSize * nSize];// �����������
	// ������ؽ����˲�(�߽粻���������㲻��Ҫ��Խ���ж�)
	for (int r = K; r < nHeight - K; ++r)
	{
		for (int c = K; c < nWidth - K; ++c)
		{
			int rgbCount = 0;// ��ǰ����(r, c)���������ظ���
			for (int r0 = r - K; r0 <= r + K; ++r0)
			{
				for (int c0 = c - K; c0 <= c + K; ++c0)
				{
					// (r0, c0)����
					Type* Pixel = pData + r0*nRowlen + c0*nChannel;
					temp[rgbCount].b = *Pixel++;
					temp[rgbCount].g = *Pixel++;
					temp[rgbCount].r = *Pixel++;
#if QUICK_SORT
					if (rgbCount > 0)
					{
						// ��С�����������
						int Aj = RGB2INT(temp[rgbCount]);// A[j]
						int Aj_1 = RGB2INT(temp[rgbCount - 1]);// A[j-1]
						for (int j = rgbCount; j > 0 && Aj < Aj_1; )
						{
							Type t;
							t = temp[j].r; temp[j].r = temp[j - 1].r; temp[j - 1].r = t;
							t = temp[j].g; temp[j].g = temp[j - 1].g; temp[j - 1].g = t;
							t = temp[j].b; temp[j].b = temp[j - 1].b; temp[j - 1].b = t;
							--j;
							Aj = Aj_1;
							Aj_1 = RGB2INT(temp[j - 1]);
						}
					}
#endif
					++rgbCount;
				}
			}
			ASSERT(rgbCount == nSize * nSize);
#if !(QUICK_SORT)
			for (int i = 0; i < (rgbCount>>1); ++i)
			{
				int Ai = RGB2INT(temp[i]);
				for (int j = i + 1; j < rgbCount; ++j)
				{
					int Aj = RGB2INT(temp[j]);
					if (Aj < Ai)
					{
						Type t;
						t = temp[j].r; temp[j].r = temp[i].r; temp[i].r = t;
						t = temp[j].g; temp[j].g = temp[i].g; temp[i].g = t;
						t = temp[j].b; temp[j].b = temp[i].b; temp[i].b = t;
						Ai = Aj;
					}
				}
			}
#endif
			rgbCount >>= 1;// ��2,ȡ��ֵ
			Type* Pixel = pCopy + r*nRowlen + c*nChannel;
			*Pixel++ = temp[rgbCount].b;
			*Pixel++ = temp[rgbCount].g;
			*Pixel++ = temp[rgbCount].r;
		}
	}
	SAFE_DELETE(temp);
	memcpy(pData, pCopy, nHeight * nRowlen * sizeof(Type));
	SAFE_DELETE(pCopy);
	return TRUE;
}

// ����ָ��(3bytes)�Ķ���ֵ
#define PIXEL2INT(p) ( 299 * p[0] + 587 * p[1] + 114 * p[2] )

// ������ָ��a,b����Ϊ a < b (ia, ibΪ���ض���ֵ)
#define Minmax(ia, ib, a, b) if( ia > ib ) { BYTE *t = a; a = b; b = t; int it = ia; ia = ib; ib = it; }

// ������ָ��a,b����Ϊ a > b (ia, ibΪ���ض���ֵ)
#define Maxmin(ia, ib, a, b) if( ia < ib ) { BYTE *t = a; a = b; b = t; int it = ia; ia = ib; ib = it; }

/** ������ֵ�˲�. 
* @details ��ֵ�˲��ȽϺ�ʱ.
* @param[in] * pData ͼ������
* @param[in] nWidth ͼ����
* @param[in] nHeight ͼ��߶�
* @param[in] nRowlen ͼ��ÿ���ֽ���
* @warning ���޲�ɫͼ��
* �ڽ�������Ϊ��ʱ.
* @see http://wenda.chinabaike.com/z/shenghuo/20131226/725816.html
*/
template <typename Type> BOOL FastMedianFilter(Type* pData, int nWidth, int nHeight, int nRowlen)
{
	Type *pCopy = new Type[nHeight * nRowlen];
	memcpy(pCopy, pData, nHeight * nRowlen * sizeof(Type));
	int nChannel = nRowlen / nWidth;
	ASSERT(nChannel >= 3);
	int K = 1;// �˲��˰뾶
	BYTE // �˲���
		*p0 = new BYTE[3], *p1 = new BYTE[3], *p2 = new BYTE[3], 
		*p3 = new BYTE[3], *p4 = new BYTE[3], *p5 = new BYTE[3], 
		*p6 = new BYTE[3], *p7 = new BYTE[3], *p8 = new BYTE[3];
	// ������ؽ����˲�(�߽粻���������㲻��Ҫ��Խ���ж�)
	for (int r = K; r < nHeight - K; ++r)
	{
		for (int c = K; c < nWidth - K; ++c)
		{
			int r0 = r - K;
			int c0 = c - K;
			
			memcpy(p0, pData + r0*nRowlen + c0*nChannel, 3);
			memcpy(p1, pData + r0*nRowlen + (c0+1)*nChannel, 3);
			memcpy(p2, pData + r0*nRowlen + (c0+2)*nChannel, 3);
			memcpy(p3, pData + (r0+1)*nRowlen + c0*nChannel, 3);
			memcpy(p4, pData + (r0+1)*nRowlen + (c0+1)*nChannel, 3);
			memcpy(p5, pData + (r0+1)*nRowlen + (c0+2)*nChannel, 3);
			memcpy(p6, pData + (r0+2)*nRowlen + c0*nChannel, 3);
			memcpy(p7, pData + (r0+2)*nRowlen + (c0+1)*nChannel, 3);
			memcpy(p8, pData + (r0+2)*nRowlen + (c0+2)*nChannel, 3);

			int ip0 = PIXEL2INT(p0), ip1 = PIXEL2INT(p1), ip2 = PIXEL2INT(p2);
			int ip3 = PIXEL2INT(p3), ip4 = PIXEL2INT(p4), ip5 = PIXEL2INT(p5);
			int ip6 = PIXEL2INT(p6), ip7 = PIXEL2INT(p7), ip8 = PIXEL2INT(p8);

			Minmax(ip0, ip1, p0, p1); Minmax(ip0, ip2, p0, p2); Minmax(ip1, ip2, p1, p2);
			Minmax(ip3, ip4, p3, p4); Minmax(ip3, ip5, p3, p5); Minmax(ip4, ip5, p4, p5);
			Minmax(ip6, ip7, p6, p7); Minmax(ip6, ip8, p6, p8); Minmax(ip7, ip8, p7, p8);
			Maxmin(ip6, ip0, p6, p0); Maxmin(ip6, ip3, p6, p3);
			Minmax(ip2, ip5, p2, p5); Minmax(ip2, ip8, p2, p8);
			Minmax(ip1, ip4, p1, p4); Minmax(ip1, ip7, p1, p7); Minmax(ip4, ip7, p4, p7);
			Minmax(ip2, ip4, p2, p4); Minmax(ip2, ip6, p2, p6); Minmax(ip4, ip6, p4, p6);

			memcpy(pCopy + r*nRowlen + c*nChannel, p4, 3);
		}
	}
	delete [] p0; delete [] p1; delete [] p2;
	delete [] p3; delete [] p4; delete [] p5;
	delete [] p6; delete [] p7; delete [] p8;
	memcpy(pData, pCopy, nHeight * nRowlen * sizeof(Type));
	SAFE_DELETE(pCopy);
	return TRUE;
}


/** ����Ӧ��ֵ�˲�. 
* @details ��ֵ�˲��ȽϺ�ʱ.
* @param[in] * pData ͼ������
* @param[in] nWidth ͼ����
* @param[in] nHeight ͼ��߶�
* @param[in] nRowlen ͼ��ÿ���ֽ���
* @param[in] nMaxSize ����˲��˳ߴ�
* @warning ���޲�ɫͼ��
* @note �÷���������ֵ�˲�����.
*/
template <typename Type> BOOL SapMedianFilter(Type* pData, int nWidth, int nHeight, int nRowlen, int nMaxSize)
{
	Type *pCopy = new Type[nHeight * nRowlen];
	memcpy(pCopy, pData, nHeight * nRowlen * sizeof(Type));
	int nChannel = nRowlen / nWidth;
	ASSERT(nChannel >= 3);
	// ������ؽ����˲�
	int nSize = 3;// ��ʼ�˲��˳ߴ�
	rgb<Type> *temp = new rgb<Type>[nMaxSize * nMaxSize];// �����������
	// ������ؽ����˲�����
	for (int r = 0; r < nHeight; ++r)
	{
		for (int c = 0; c < nWidth; ++c)
		{
			SapGetMinMedMax(pData, nWidth, nHeight, nRowlen, nChannel, pCopy, r, c, temp, nSize, nMaxSize);
		}
	}
	SAFE_DELETE(temp);
	memcpy(pData, pCopy, nHeight * nRowlen * sizeof(Type));
	SAFE_DELETE(pCopy);
	return TRUE;
}


/** ��ȡ��Сֵ����ֵ�����ֵ. 
* @details ����Ӧ��ֵ�˲�ԭ��
* (1) ����Zmin,Zmed,Zmax,��Zmed����(Zmin, Zmax),ת(2),���������˲����ظ�(1),���ߴ糬��,���Z;
* (2) ��Z����(Zmin, Zmax),���޸�Z,������Z = Zmed
* @param[in] *pData ԭʼͼ������
* @param[in] nWidth ͼ����
* @param[in] nHeight ͼ��߶�
* @param[in] nRowlen ͼ��ÿ���ֽ���
* @param[in] nChannel ͼ��ÿ�����ֽ�
* @param[in] *pCopy ͼ�񿽱�
* @param[in] r ��ǰ�˲����أ��У�
* @param[in] c ��ǰ�˲����أ��У�
* @param[in] *temp ����������������
* @param[in] nSize ��ǰ�˲��˳ߴ�
* @param[in] nMaxSize �˲������ߴ�
*/
template <typename Type> void SapGetMinMedMax(Type* pData, int nWidth, int nHeight, int nRowlen, int nChannel, 
											  Type *pCopy, int r, int c, rgb<Type> *temp, int nSize, int nMaxSize)
{
	int K = nSize >> 1;// �˲��˰뾶
	int rgbCount = 0;  // ��ǰ����(r, c)���������ظ���
	for (int r0 = r - K; r0 <= r + K; ++r0)
	{
		if (0 <= r0 && r0 < nHeight)
		{
			for (int c0 = c - K; c0 <= c + K; ++c0)
			{
				if (0 <= c0 && c0 < nWidth)
				{
					// (r0, c0)����
					Type* Pixel = pData + r0*nRowlen + c0*nChannel;
					// ���������
					temp[rgbCount].b = *Pixel++;
					temp[rgbCount].g = *Pixel++;
					temp[rgbCount].r = *Pixel++;
					++rgbCount;
				}
			}
		}
	}
	rgb<Type> *Min = NULL, *Med = NULL, *Max = NULL;
	if (rgbCount)
	{
		// ��������
		Sort(temp, rgbCount);
		Min = &temp[0];// min
		Med = &temp[rgbCount >> 1];// med
		Max = &temp[rgbCount - 1];// max
	}
	Type* Pixel = pCopy + r*nRowlen + c*nChannel;
	rgb<Type> cur(*Pixel, *(Pixel+1), *(Pixel+2));// ��ǰ����
	int nMin = RGB2INT(*Min);
	int nMed = RGB2INT(*Med);
	int nMax = RGB2INT(*Max);
	int nCur = RGB2INT(cur);
	if (nMin < nMed && nMed < nMax)
	{
		if(nMin < nCur && nCur < nMax)
		{
			// ���޸�pCopy
			return;
		}
		else
		{
			// ȡ��ֵ
			*Pixel++ = Med->b;
			*Pixel++ = Med->g;
			*Pixel++ = Med->r;
			return;
		}
	}
	else
	{
		if (nSize >= nMaxSize)
		{
			// �˲��˴�С���磬�˳�
			return;
		}
		else
		{
			// �����˲��˽��д���
			return SapGetMinMedMax(pData, nWidth, nHeight, nRowlen, nChannel, pCopy, r, c, temp, nSize + 2, nMaxSize);
		}
	}
}


/** ��������. 
* @param[in] pArray ����������
* @param[in] Num ����Ԫ�ظ���
*/
template <typename Type> void InsertionSort(Type *pArray, int Num)
{
	for (int i = 0; i < Num; ++i)
	{
		for (int j = i; (j > 0) && pArray[j] < pArray[j - 1]; --j)
		{
			Type temp = pArray[j];
			pArray[j] = pArray[j - 1];
			pArray[j - 1] = temp;
		}
	}
}