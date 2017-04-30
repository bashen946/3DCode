/** 
* @file Transform.h
* @brief ͼ��任ģ�庯��,���Կ�ƽ̨.
* @details ������ͼ�����š���ת����ת������ͼ�����Ȥ���򡢱��桢ֱ��ͼ������
* @author Ԭ����
* @email 962914132@qq.com
* @version V1.0
* @date 2016/10/4
* Copyleft (c), All rights reserved.
* @note �����Ӵ˰���"algorithm.h"�ļ���������Eclipse����
* error: 'sort' is not a member of 'std'��
* ������**����ָ����Ϊ�������������޸Ĵ�һ��ָ��
*/

#include "DataTypes.h"
#include <fstream>

#pragma once

// ��ͼ��д���ļ����ļ�ͷ��С���ֽڣ�
#define HEADER_SIZE 1024

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
template <typename Type> Type* ImageROI(const Type* pHead, int &nWidth, int &nHeight, int &nRowlen, RoiRect roi = RoiRect(0, 0, 0, 0));

// ��ͼ���Զ�������ʽд���ļ�
template <typename Type> BOOL ImageWrite(const char* fileName, const Type* pSrc, int nWidth, int nHeight, int nRowlen);

// ��ɫͼ��תΪ8λ�ĵ�ͨ��ͼ��
template <typename Type> Type* Rgb2Gray(const Type* pHead, int nWidth, int nHeight, int nRowBytes);

// ͳ�ƻҶ�ͼ���ֱ��ͼ
template <typename Type> void ImageHistogram(const Type* pHead, int nRowlen, int nHist[256], RoiRect roi);

//////////////////////////////////////////////////////////////////////////


/** 
* @brief ���·�תͼ��.
* @param[in] *pHead ����ͷ
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


/** 
* @brief ���ҷ�תͼ��
* @param[in] *pHead ����ͷ
* @param[in] nWidth ���
* @param[in] nHeight �߶�
* @param[in] nRowlen ÿ�и���
*/
template <typename Type> void ImageFlipH(Type* pHead, const int nWidth, const int nHeight, const int nRowlen)
{
	Type* temp = new Type[nHeight * nRowlen];
	memcpy(temp, pHead, nHeight * nRowlen * sizeof(Type));

	int nChannel = nRowlen / nWidth;

#pragma omp parallel for
	for (int r = 0; r < nHeight; ++r)
	{
		int y = r * nRowlen;
		for (int c = 0; c < nWidth; ++c)
		{
			memcpy(pHead + c *nChannel + y, temp + (nWidth - 1 - c) * nChannel + y, nChannel * sizeof(Type));
		}
	}
	SAFE_DELETE(temp);
}


/** 
* @brief ���ҷ�תͼ��
* @param[in] **pHead ����ͷ
* @param[in] nWidth ���
* @param[in] nHeight �߶�
* @param[in] nRowlen ÿ�и���
* @note �������޸�ͼ��ָ��
*/
template <typename Type> void ImageTranspose(Type** pHead, const int nWidth, const int nHeight, const int nRowlen)
{
	// ͼ��ÿ����Ԫ�ظ���
	const int nChannel = nRowlen / nWidth;
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
		int x = i * nChannel, y1 = i * nNewRowlen;
		for (int j = 0; j < nHeight; ++j)
		{
			int y = j * nRowlen, x1 = j * nChannel;
			for (int k = 0; k < nChannel; ++k)
			{
				temp[k + x1 + y1] = pSrc[k + x + y];
			}
		}
	}
	delete[] * pHead;
	*pHead = temp;
	temp = NULL;
}


/** 
* @brief ��ȡͼ�����Ȥ����
* @param[in] **pHead ����ͷ
* @param[in] &nWidth ���
* @param[in] &nHeight �߶�
* @param[in] &nRowlen ÿ�и���
* @param[in] roi ͼ�����Ȥ����
* @return ��������ͼ�����Ȥ����(ָ�룬��ҪSAFE_DELETE)
* @note ��������ͼ���ȡ��߶ȵ���Ϣ�����ROI�Ŀ�ȡ��߶ȵ���Ϣ
*/
template <typename Type> Type* ImageROI(const Type* pHead, int &nWidth, int &nHeight, int &nRowlen, RoiRect roi)
{
	/// ���roi�Ƿ�
	if (roi.Width() <= 0 || roi.Height() <= 0 || roi.right > nWidth || roi.bottom > nHeight)
	{
		Type* pDst = new Type[nHeight * nRowlen];
		memcpy(pDst, pHead, nHeight * nRowlen * sizeof(Type));
		return pDst;
	}

	const int nChannel = nRowlen / nWidth;
	const int nNewWidth = roi.Width();
	const int nNewHeight = roi.Height();
	int nNewRowlen = nNewWidth * nChannel;
	// �Ƿ���Ҫ���ж��봦��
	if (1 == sizeof(Type))
	{
		nNewRowlen = WIDTHBYTES(nNewRowlen * 8);
	}

	Type* pDst = new Type[nNewHeight * nNewRowlen];

#pragma omp parallel for
	int x0 = roi.left * nChannel;
	for (int i = 0; i < nNewHeight; ++i)
	{
		int nLine = nHeight - roi.top - i - 1;
		memcpy(pDst + i * nNewRowlen, pHead + x0 + nLine * nRowlen, nNewRowlen * sizeof(Type));
	}

	// ����ͼ����Ϣ
	nWidth = nNewWidth;
	nHeight = nNewHeight;
	nRowlen = nNewRowlen;
	return pDst;
}


/** 
* @brief ��ͼ���Զ�������ʽд���ļ�
* @param[in] *fileName �ļ�����
* @param[in] *pHead ����ͷ
* @param[in] nWidth ���
* @param[in] nHeight �߶�
* @param[in] nRowlen ÿ�и���
* @see FileIO�е�WriteTxt/ReadTxt����
*/
template <typename Type> BOOL ImageWrite(const char* fileName, const Type* pSrc, int nWidth, int nHeight, int nRowlen)
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
* @param[in] **pHead ����ͷ
* @param[in] &nWidth ���
* @param[in] &nHeight �߶�
* @param[in] &nRowlen ÿ�и���
* @param[in] nChannel ͼ��ͨ��
* @param[in] NewWidth ͼ���¿��
* @param[in] NewHeight ͼ���¸߶�
* @note �������޸�pfHeadָ������ݣ���Ϊ���
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
		int x = i * nChannel;
		for (int j = 0; j < NewHeight; ++j)
		{
			int y = j * NewRowlen;
			for (int nCurChannel = 0; nCurChannel < nChannel; ++nCurChannel)
				pDst[nCurChannel + x + y] =
				(Type)biLinearInterp(*pfHead, nWidth, nHeight, nRowlen, nChannel, nCurChannel, i * wRatio, j * hRatio);
		}
	}

	// �޸�pHeadָ�������
	delete[] * pfHead;
	*pfHead = pDst;
	pDst = NULL;

	// ����ͼ����Ϣ
	nWidth = NewWidth;
	nHeight = NewHeight;
	nRowlen = NewRowlen;
}


/* - ����ͼ�����߳� -
* @param[in] **pHead ����ͷ
* @param[in] &nWidth ���
* @param[in] &nHeight �߶�
* @param[in] &nRowlen ÿ�и���
* @param[in] nChannel ͼ��ͨ��
* @param[in] nMaxSize ���߳�320
* @note ��ͼ����һ�߳��ȳ�������ʱ�������Ų�������Ϣ
* @warning ��ȡ��߶ȡ�ÿ���ֽ����ᱻ�޸�
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

/** 
* @brief ��ɫת�ڰ�.
* @param[in] *pHead ͼ������
* @param[in] nWidth ͼ����
* @param[in] nHeight ͼ��߶�
* @param[in] nRowBytes ÿ���ֽ���
* @return ���������ػҶ�ͼ��
* @note ���޶�char/BYTE���д���
* @warning ��Ҫ�Է���ֵ����delete����.
*/
template <typename Type> Type* Rgb2Gray(const Type* pHead, int nWidth, int nHeight, int nRowBytes)
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
			int y1 = i * nRowBytes, y2 = i * nRowLenNew;
			for (int j = 0; j < nWidth; j++)
			{
				const Type *pSrcTemp = pHead + nChannel * j + y1;
				Type *pDstTemp = pDst + nChannelNew * j + y2;
				// ����ǿ��ת��Ϊ�޷����ַ�������(2016/9/12 ע)
				BYTE R = *(pSrcTemp + 2);
				BYTE G = *(pSrcTemp + 1);
				BYTE B = *pSrcTemp;
				*pDstTemp = BYTE(RGB2GRAY(R, G, B));
			}
		}
		break;
	default:
		SAFE_DELETE(pDst);
		return NULL;
	}
	return pDst;
}


/** 
* @brief ͳ�ƻҶ�ͼ���ֱ��ͼ.
* @param[in] *pHead ͼ��ָ��
* @param[in] nRowlen ͼ��ÿ���ֽ���
* @param[in] nHist ͼ��ֱ��ͼ
* @param[in] roi ͼ�����Ȥ����
*/
template <typename Type> void ImageHistogram(const Type* pHead, int nRowlen, int nHist[256], RoiRect roi)
{
	ASSERT(sizeof(Type) == 1);
	memset(nHist, 0, 256 * sizeof(int));
	for (int i = roi.top; i < roi.bottom; ++i)
	{
		int y = i * nRowlen;
		for (int j = roi.left; j < roi.right; ++j)
		{
			int index = (BYTE)pHead[j + y];
			++ nHist[index];
		}
	}
}