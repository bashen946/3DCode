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

// ��ȡrow��col��ģ��ƽ��ɫ��(RGB ref)
COLORREF GetPixelRef(const BYTE *pHead, int nPixelSize, int nRow, int nCol, int nRowlen, int nChannel);

// ��ȡrow��col��ģ��ƽ��ɫ��(float3)
float3 GetPixel(const BYTE *pHead, int nPixelSize, int nRow, int nCol, int nRowlen, int nChannel);

// Ϊrow��col��ģ����������ֵ�����ش�С�ɲ���ָ��
void SetPixel(BYTE *pHead, int nPixelSize, int nRow, int nCol, int nRowlen, int nChannel, COLORREF ref = RGB(255, 255, 255));

// Ϊrow��col��ģ����������ֵ�����ش�С�ɲ���ָ��
void SetPixel(BYTE *pHead, int nPixelSize, int nRow, int nCol, int nRowlen, int nChannel, int R, int G, int B);
