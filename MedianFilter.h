#include "DataTypes.h"

#pragma once

/** 
* @file MedianFilter.h
* @brief  ��ֵ�˲�
* @details ��������ֵ�˲�������Ӧ��ֵ�˲�����
* @see http://blog.csdn.net/mfcing/article/details/43817715
*/

// ��ֵ�˲�
template <typename Type> BOOL MedianFilter(Type* pData, int nWidth, int nHeight, int nRowlen, int nSize = 3);

// ������ֵ�˲�
template <typename Type> BOOL FastMedianFilter(Type* pData, int nWidth, int nHeight, int nRowlen, int nSize = 3);

// ����Ӧ��ֵ�˲�
template <typename Type> BOOL SapMedianFilter(Type* pData, int nWidth, int nHeight, int nRowlen, int nMaxSize = 7);

template <typename Type> int SapGetMinMedMax(Type* pData, int nWidth, int nHeight, int nRowlen, int nChannel,
	Type *pCopy, int r, int c, rgb<Type> *temp, int nSize, int nMaxSize);

//////////////////////////////////////////////////////////////////////////

/** 
* @brief ��ֵ�˲�.
* @details ��ֵ�˲��ȽϺ�ʱ.
* @param[in] *pData ͼ������
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
	const int nChannel = nRowlen / nWidth;
	ASSERT(nChannel >= 3);
	const int K = nSize >> 1;// �˲��˰뾶
	rgb<Type> *temp = new rgb<Type>[nSize * nSize];// �����������
	
	// ������ؽ����˲�(�߽粻���������㲻��Ҫ��Խ���ж�)
	// �����˲���Ϊ3���˲�����ѭ����˷�����������27
	for (int r = K; r < nHeight - K; ++r)
	{
		for (int c = K; c < nWidth - K; ++c)
		{
			int rgbCount = 0;// ��ǰ����(r, c)���������ظ���
			for (int r0 = r - K; r0 <= r + K; ++r0)
			{
				int y = r0 * nRowlen;
				for (int c0 = c - K; c0 <= c + K; ++c0)
				{
					// (r0, c0)����
					Type* Pixel = pData + y + c0 * nChannel;
					temp[rgbCount].b = *Pixel++;
					temp[rgbCount].g = *Pixel++;
					temp[rgbCount].r = *Pixel++;
					temp[rgbCount].Init();
					if (rgbCount > 0)
					{
						// ��С�����������
						for (int j = rgbCount; j > 0 && temp[j] < temp[j-1]; --j)
						{
							temp[j].Swap(temp[j-1]);
						}
					}
					++rgbCount;
				}
			}
			ASSERT(rgbCount == nSize * nSize);
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


/** 
* @brief ������ֵ�˲�.
* @details ��ֵ�˲��ȽϺ�ʱ.
* @param[in] *pData ͼ������
* @param[in] nWidth ͼ����
* @param[in] nHeight ͼ��߶�
* @param[in] nRowlen ͼ��ÿ���ֽ���
* @param[in] nSize �˲��˳ߴ�
* @warning ���޲�ɫͼ��. ��ͼ��ͨ��������4���˲��˳ߴ����3ʱ����MedianFilter.
* @see http://wenda.chinabaike.com/z/shenghuo/20131226/725816.html
*/
template <typename Type> BOOL FastMedianFilter(Type* pData, int nWidth, int nHeight, int nRowlen, int nSize)
{
	const int nChannel = nRowlen / nWidth;
	ASSERT(nChannel >= 3);
	if (nChannel == 3 || nSize > 3)
		return MedianFilter(pData, nWidth, nHeight, nRowlen, nSize);
	// ���4ͨ��ͼ����п����˲�
	const int C = 4; // ͨ����
	Type *pCopy = new Type[nHeight * nRowlen];
	memcpy(pCopy, pData, nHeight * nRowlen * sizeof(Type));
	const int K = 1; // �˲��˰뾶

	// ָ���������ݵ�ָ��
	typedef unsigned* pINT;

	// ��ia,ib����Ϊ ia < ib
#define Minmax(ia, ib) if( ia > ib ) {int ic(ia); ia = ib; ib = ic;}

	// ��ia,ib����Ϊ ia > ib
#define Maxmin(ia, ib) if( ia < ib ) {int ic(ia); ia = ib; ib = ic;}

	// ������ؽ����˲�(�߽粻���������㲻��Ҫ��Խ���ж�)
	int offsety = K * nRowlen + K * C;
	Type *pLine = pData + offsety;
	for (int row = K; row < nHeight - K; ++row)
	{
		int offsetx = offsety; // ƫ����
		Type *pCenter = pLine; // ��������
		for (int col = K; col < nWidth - K; ++col)
		{
			Type *pCur = pCenter;
			register unsigned int p0, p1, p2, p3, p4, p5, p6, p7, p8;

			pCur -= nRowlen; // �ƶ�ָ��
			p0 = *(pINT(pCur - C)); p1 = *(pINT(pCur)); p2 = *(pINT(pCur + C));
			pCur += nRowlen;
			p3 = *(pINT(pCur - C)); p4 = *(pINT(pCur)); p5 = *(pINT(pCur + C));
			pCur += nRowlen;
			p6 = *(pINT(pCur - C)); p7 = *(pINT(pCur)); p8 = *(pINT(pCur + C));

			Minmax(p0, p1); Minmax(p1, p2); // p0<p1 p0<p2 p1<p2��p0<p1<p2
			Minmax(p3, p4); Minmax(p4, p5); // p3<p4 p3<p5 p4<p5��p3<p4<p5
			Minmax(p6, p7); Minmax(p7, p8); // p6<p7 p6<p8 p7<p8��p6<p7<p8
			Maxmin(p6, p0); Maxmin(p6, p3); // p6>p0 p6>p3, �ų�p0,p3
			Minmax(p2, p5); Minmax(p2, p8); // p2<p5 p2<p8, �ų�p5,p8
			Minmax(p1, p4); Minmax(p4, p7); // p1<p4 p1<p7 p4<p7��p1<p4<p7, �ų�p1,p7
			Minmax(p2, p4); Minmax(p4, p6); // p2<p4 p2<p6 p4<p6��p2<p4<p6, �ų�p2,p6

			memcpy(pCopy + offsetx, &p4, C);
			offsetx += C;
			pCenter += C;
		}
		offsety += nRowlen;
		pLine += nRowlen;
	}
	memcpy(pData, pCopy, nHeight * nRowlen * sizeof(Type));
	SAFE_DELETE(pCopy);
	return TRUE;
}


/** 
* @brief ����Ӧ��ֵ�˲�.
* @details ��ֵ�˲��ȽϺ�ʱ.
* @param[in] *pData ͼ������
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
	const int nChannel = nRowlen / nWidth;
	ASSERT(nChannel >= 3);
	// ������ؽ����˲�
	const int nSize = 3;// ��ʼ�˲��˳ߴ�
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
template <typename Type> int SapGetMinMedMax(Type* pData, int nWidth, int nHeight, int nRowlen, int nChannel,
	Type *pCopy, int r, int c, rgb<Type> *temp, int nSize, int nMaxSize)
{
	const int K = nSize >> 1;// �˲��˰뾶
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
					temp[rgbCount].Init();
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
	int nMin = Min->Init();
	int nMed = Med->Init();
	int nMax = Max->Init();
	int nCur = cur.Init();
	if (nMin < nMed && nMed < nMax)
	{
		if (nMin < nCur && nCur < nMax)
		{
			// ���޸�pCopy
			return 1;
		}
		else
		{
			// ȡ��ֵ
			*Pixel++ = Med->b;
			*Pixel++ = Med->g;
			*Pixel++ = Med->r;
			return 2;
		}
	}
	else
	{
		// �˲��˴�С���磬�˳�; ���������˲��˽��д���
		return nSize >= nMaxSize ? 0 : 
			SapGetMinMedMax(pData, nWidth, nHeight, nRowlen, nChannel, pCopy, r, c, temp, nSize + 2, nMaxSize);
	}
}