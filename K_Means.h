/** 
* @file K_Means.h
* @brief ģ�庯��,���Կ�ƽ̨.
* @details ������K��ֵ����ĺ��Ĵ���.
* @author Ԭ����
* @version V1.0
* @date 2017/3/4
* @note �����Ӵ˰���"algorithm.h"�ļ���������Eclipse����
* error: 'sort' is not a member of 'std'
* ������**����ָ����Ϊ�������������޸Ĵ�һ��ָ��.\n
* Copyleft (c), All rights reserved.
*/

#include "DataTypes.h"

/* K_means */

#pragma once

// ����������(2016/9/19)
template <typename Type> Type Difference(const Type *Center, int K);

// ��ʼ����������(2016/9/19)
template <typename T1, typename T2> void InitializeClusterCenters(T1 *Center, int K, const T2* pHead, int nRowBytes, RoiRect roi);

// ��ʼ����������(random���)
template <typename T1, typename T2> void RandomClusterCenters(T1 *Center, int K, const T2* pHead, int nRowBytes, RoiRect roi);

// ��ͼ�������K_means�������ָ�
template <typename Type> void K_means(Type* pHead, int nRowBytes, RoiRect roi, int K, float fBackground, int nMaxepoches);

//////////////////////////////////////////////////////////////////////////


/** 
* @brief ����������(2016/9/19).
* @param[in] *Center ����
* @param[in] K Ԫ����
*/
template <typename Type> Type Difference(const Type *Center, int K)
{
	Type sum(0);
	for (int i = 0; i < K; ++i)
	{
		for (int j = i; j < K; ++j)
		{
			sum += abs(Center[j] - Center[i]);
		}
	}
	return sum;
}


/** 
* @brief ��ʼ����������(2016/9/19).
* @param[in] *Center ��������
* @param[in] K �������
* @param[in] *pHead ͼ������
* @param[in] nRowBytes ͼ��ÿ��Ԫ�ظ���
* @param[in] roi ͼ�����Ȥ����
* @note ��������ͼ��ֱ��ͼ��ȡ��ֵ���ĻҶ�Ϊ����.
*/
template <typename T1, typename T2> void InitializeClusterCenters(T1 *Center, int K, const T2* pHead, int nRowBytes, RoiRect roi)
{
#ifdef _DEBUG
	TRACE(" * ���ڳ�ʼ����������...\n");
#endif // _DEBUG
	// ��ȡֱ��ͼ
	int nHist[256];
	ImageHistogram(pHead, nRowBytes, nHist, roi);
	int index[256];// ֱ��ͼ���±�
	for (int i = 0; i < 256; )
	{
		index[i] = i; ++i;
		index[i] = i; ++i;
		index[i] = i; ++i;
		index[i] = i; ++i;
	}
	// ��������,ʹ�Ҷ�ֵ��ֵ����ǰͷ
	for (int i = 1; i < 256; ++i)
	{
		for (int j = i; j > 0 && nHist[j] > nHist[j - 1]; --j)
		{
			SWAP(nHist[j], nHist[j-1]);
			SWAP(index[j], index[j-1]);
		}
	}
	// Ѱ��K�����ģ���Щ���ĵļ�಻��С��min_diff
	int count = 0, num;
	T1 min_diff = (T1)64;
	do
	{
		count++;
		num = 0;
		Center[0] = (T1)index[num]; //��1������
		for (int i = 0; i < K; ++i)
		{
			// ���ɺ���K�����ģ�ȷ�����Ĳ��ϴ�
			while(num < 256)// ȷ��������һ��
			{
				Center[i] = (T1)index[num++];
				int j = 0;
				for (; j < i; ++j)
				{
					// ��ĳ���������ĵļ���С
					if (abs(Center[i] - Center[j]) < min_diff)
						break;
				}
				// Cj��ȫ��Ci�ľ���ϴ��˳�whileѭ��������������һ��
				if (j == i)
					break;
			}
			// ����min_diff����̫��
			if (num == 256)
				break;
#ifdef _DEBUG
			TRACE("C[%d] = %.6f\t", i, (float)Center[i]);
#endif // _DEBUG
		}
#ifdef _DEBUG
		TRACE("\n");
#endif // _DEBUG
		min_diff -= (T1)1;
	} while (num == 256);
#ifdef _DEBUG
	TRACE(" * [%d]�������������ģ����಻С��%.2f.\n", count, (float)min_diff);
#endif
}


/** 
* @brief �������K������.
* @param[in] *Center ��������
* @param[in] K �������
* @param[in] *pHead ͼ������
* @param[in] nRowBytes ͼ��ÿ��Ԫ�ظ���
* @param[in] roi ͼ�����Ȥ����
*/
template <typename T1, typename T2> void RandomClusterCenters(T1 *Center, int K, const T2* pHead, int nRowBytes, RoiRect roi)
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
		TRACE("C[%d] = %.6f\t", i, (float)Center[i]);
#endif // _DEBUG
		seed = rand();
	}
#ifdef _DEBUG
	TRACE("\n");
#endif // _DEBUG
}


/** 
* @brief K_means ����ָ�.
* @param[in] *pHead ͼ������
* @param[in] nRowBytes ÿ���ֽ���
* @param[in] roi ͼ�����Ȥ����
* @param[in] K �������
* @param[in] nMaxepoches ����������
* @note (1)�����ǻҶ�ͼ��;(2)�˺������޸�ͼ������pHead.
*/
template <typename Type> void K_means(Type* pHead, int nRowBytes, RoiRect roi, int K, float fBackground, int nMaxepoches)
{
	// ��������ĳߴ�
	const int nWidth = roi.Width();
	const int nHeight = roi.Height();
	const int nRowlen = nWidth;// ������ÿ��Ԫ�ظ���
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
			int y = (j - roi.top) * nRowlen;
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
				Cluster[(i - roi.left) + y] = (Type)tag;
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
		int y0 = j * nRowlen;
		int y = (j + roi.top) * nRowBytes;
		for (int i = 0; i < nWidth; ++i)
		{
			pHead[i + roi.left + y] = (Cluster[i + y0] == backValIdx) ? 0 : 255;
		}
	}
	delete[] Center;
	delete[] oldCenter;
	delete[] sumCenter;
	delete[] sum;
	delete[] Cluster;
}