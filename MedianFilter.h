#include "DataTypes.h"

#pragma once

/** 
* @file MedianFilter.h
* @brief  ��ֵ�˲�
* @details ��������ֵ�˲�������Ӧ��ֵ�˲�����
* @see http://blog.csdn.net/mfcing/article/details/43817715
*/

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

//////////////////////////////////////////////////////////////////////////


/// ����Ϊ1�������ÿ�������
#define QUICK_SORT 1

/** 
* @brief ��ֵ�˲�.
* @details ��ֵ�˲��ȽϺ�ʱ.
* @param[in] * pData ͼ������
* @param[in] nWidth ͼ����
* @param[in] nHeight ͼ��߶�
* @param[in] nRowlen ͼ��ÿ���ֽ���
* @param[in] nSize �˲��˳ߴ�
* @warning ���޲�ɫͼ��
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
			for (int i = 0; i < (rgbCount >> 1); ++i)
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

/// ����ָ��(3bytes)�Ķ���ֵ
#define PIXEL2INT(p) ( 299 * p[0] + 587 * p[1] + 114 * p[2] )

/// ������ָ��a,b����Ϊ a < b (ia, ibΪ���ض���ֵ)
#define Minmax(ia, ib, a, b) if( ia > ib ) { BYTE *t = a; a = b; b = t; int it = ia; ia = ib; ib = it; }

/// ������ָ��a,b����Ϊ a > b (ia, ibΪ���ض���ֵ)
#define Maxmin(ia, ib, a, b) if( ia < ib ) { BYTE *t = a; a = b; b = t; int it = ia; ia = ib; ib = it; }

/** 
* @brief ������ֵ�˲�.
* @details ��ֵ�˲��ȽϺ�ʱ.
* @param[in] * pData ͼ������
* @param[in] nWidth ͼ����
* @param[in] nHeight ͼ��߶�
* @param[in] nRowlen ͼ��ÿ���ֽ���
* @warning ���޲�ɫͼ��
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
			memcpy(p1, pData + r0*nRowlen + (c0 + 1)*nChannel, 3);
			memcpy(p2, pData + r0*nRowlen + (c0 + 2)*nChannel, 3);
			memcpy(p3, pData + (r0 + 1)*nRowlen + c0*nChannel, 3);
			memcpy(p4, pData + (r0 + 1)*nRowlen + (c0 + 1)*nChannel, 3);
			memcpy(p5, pData + (r0 + 1)*nRowlen + (c0 + 2)*nChannel, 3);
			memcpy(p6, pData + (r0 + 2)*nRowlen + c0*nChannel, 3);
			memcpy(p7, pData + (r0 + 2)*nRowlen + (c0 + 1)*nChannel, 3);
			memcpy(p8, pData + (r0 + 2)*nRowlen + (c0 + 2)*nChannel, 3);

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
	delete[] p0; delete[] p1; delete[] p2;
	delete[] p3; delete[] p4; delete[] p5;
	delete[] p6; delete[] p7; delete[] p8;
	memcpy(pData, pCopy, nHeight * nRowlen * sizeof(Type));
	SAFE_DELETE(pCopy);
	return TRUE;
}


/** 
* @brief ����Ӧ��ֵ�˲�.
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


/** 
* @brief ��ȡ��Сֵ����ֵ�����ֵ.
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
	rgb<Type> cur(*Pixel, *(Pixel + 1), *(Pixel + 2));// ��ǰ����
	int nMin = RGB2INT(*Min);
	int nMed = RGB2INT(*Med);
	int nMax = RGB2INT(*Max);
	int nCur = RGB2INT(cur);
	if (nMin < nMed && nMed < nMax)
	{
		if (nMin < nCur && nCur < nMax)
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


/** 
* @brief ��������.
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