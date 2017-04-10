/** 
* @file K_Means.h
* @brief ģ�庯��,���Կ�ƽ̨.
* @details ������K��ֵ����ĺ��Ĵ���.
* @author Ԭ����
* @version V1.0
* @date 2017/3/4
* @note ������Ӵ˰���"algorithm.h"�ļ���������Eclipse����
* error: 'sort' is not a member of 'std'
* ������**����ָ����Ϊ�������������޸Ĵ�һ��ָ��.\n
* Copyleft (c), All rights reserved.
*/

#include "DataTypes.h"

/* K_means */

#pragma once

// ����������(2016/9/19)
template <typename Type> float Difference(Type *Center, int K);

// ��ʼ����������(2016/9/19)
template <typename Type> void InitializeClusterCenters(float *Center, int K, const Type* pHead, int nRowBytes, RoiRect roi);

// ��ʼ����������(random���)
template <typename Type> void RandomClusterCenters(float *Center, int K, const Type* pHead, int nRowBytes, RoiRect roi);

// ��ͼ�������K_means�������ָ�
template <typename Type> void K_means(Type* pHead, int nRowBytes, RoiRect roi, int K, float fBackground, int nMaxepoches);

//////////////////////////////////////////////////////////////////////////


/** 
* @brief ����������(2016/9/19).
* @param[in] *Center ����
* @param[in] K Ԫ����
*/
template <typename Type> float Difference(Type *Center, int K)
{
	float sum = 0.0f;
	for (int i = 0; i < K; ++i)
	{
		for (int j = i; j < K; ++j)
		{
			sum += fabs(Center[j] - Center[i]);
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
template <typename Type> void InitializeClusterCenters(float *Center, int K, const Type* pHead, int nRowBytes, RoiRect roi)
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
			int t;
			t = nHist[j]; nHist[j] = nHist[j - 1]; nHist[j - 1] = t;
			t = index[j]; index[j] = index[j - 1]; index[j - 1] = t;
		}
	}
	// Ѱ��K�����ģ���Щ���ĵļ�಻��С��min_diff
	int count = 0, num;
	float min_diff = 64.0f;
	do
	{
		count++;
		num = 0;
		Center[0] = (float)index[num]; //��1������
		for (int i = 0; i < K; ++i)
		{
			// ���ɺ���K�����ģ�ȷ�����Ĳ��ϴ�
			while(num < 256)// ȷ��������һ��
			{
				Center[i] = (float)index[num++];
				int j = 0;
				for (; j < i; ++j)
				{
					// ��ĳ���������ĵļ���С
					if (fabs(Center[i] - Center[j]) < min_diff)
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
			TRACE("C[%d] = %.6f\t", i, Center[i]);
#endif // _DEBUG
		}
#ifdef _DEBUG
		TRACE("\n");
#endif // _DEBUG
		min_diff -= 1.0f;
	} while (num == 256);
	TRACE(" * [%d]�������������ģ����಻С��%.2f.\n", count, min_diff);
}


/** 
* @brief �������K������.
* @param[in] *Center ��������
* @param[in] K �������
* @param[in] *pHead ͼ������
* @param[in] nRowBytes ͼ��ÿ��Ԫ�ظ���
* @param[in] roi ͼ�����Ȥ����
*/
template <typename Type> void RandomClusterCenters(float *Center, int K, const Type* pHead, int nRowBytes, RoiRect roi)
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
				Cluster[(i - roi.left) + (j - roi.top) * nRowlen] = (Type)tag;
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
	delete[] Center;
	delete[] oldCenter;
	delete[] sumCenter;
	delete[] sum;
	delete[] Cluster;
}