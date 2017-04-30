/** 
* @file PixelsOperation.h
* @brief ���ز�������
* @details 
*		Set...�Զ�ά���ģ�������ɫ���ã�
*		Get...��ȡ��ά��ģ�����ɫƽ���Ҷȣ�
*		���Կ�ƽ̨ʹ�á�
* Copyleft (c), All rights reserved.
*/

#include "DataTypes.h"

#pragma once

// ��ȡrow��col��ģ��ƽ��ɫ��(float3)
float3 GetPixel(const BYTE *pHead, int nPixelSize, int nRow, int nCol, int nRowlen, int nChannel);

// Ϊrow��col��ģ����������ֵ�����ش�С�ɲ���ָ��
void SetPixel(BYTE *pHead, int nPixelSize, int nRow, int nCol, int nRowlen, int nChannel, int R, int G, int B);

/** 
* @brief - ��ȡ����ֵ -
* @param[in] *pHead			�����׵�ַ
* @param[in] nPixelSize		ÿģ�����ظ���
* @param[in] nRow			��ָ��
* @param[in] nCol			��ָ��
* @param[in] nRowlen		ÿ���ֽ���
* @param[in] nChannel		ͼ��ͨ����
* @note ��ȡnRow��nCol����������ֵ
*/
inline COLORREF GetPixelRef(const BYTE *pHead, int nPixelSize, int nRow, int nCol, int nRowlen, int nChannel)
{
	float3 avg = GetPixel(pHead, nPixelSize, nRow, nCol, nRowlen, nChannel);
	return RGB(avg.x, avg.y, avg.z);
}

/** 
* @brief - ��������ֵ -
* @param[in] *pHead			�����׵�ַ
* @param[in] nPixelSize		ÿģ�����ظ���
* @param[in] nRow			��ָ��
* @param[in] nCol			��ָ��
* @param[in] nRowlen		ÿ���ֽ���
* @param[in] nChannel		ͼ��ͨ����
* @param[in]  ref			��ɫֵ
* @note ΪnRow��nCol��������������ֵ
*/
inline void SetPixel(BYTE *pHead, int nPixelSize, int nRow, int nCol, int nRowlen, int nChannel, COLORREF ref)
{
	SetPixel(pHead, nPixelSize, nRow, nCol, nRowlen, nChannel, GetRValue(ref), GetGValue(ref), GetBValue(ref));
}