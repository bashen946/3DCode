/************************************************************************
* @file 
* @details 
*		Set...�Զ�ά���ģ�������ɫ����
*		Get...��ȡ��ά��ģ�����ɫƽ���Ҷ�
*		���Կ�ƽ̨ʹ��
* @author 
* @email 
* @version V1.0
* @date 2016/10/4
* Copyleft (c), All rights reserved.
************************************************************************/

#include "DataTypes.h"

#pragma once

// ��ȡrow��col��ģ��ƽ��ɫ��(RGB ref)
COLORREF GetPixelRef(BYTE *pHead, int nPixelSize, int nRow, int nCol, int nRowlen, int nChannel);

// ��ȡrow��col��ģ��ƽ��ɫ��(float3)
float3 GetPixel(BYTE *pHead, int nPixelSize, int nRow, int nCol, int nRowlen, int nChannel);

// Ϊrow��col��ģ����������ֵ�����ش�С�ɲ���ָ��
void SetPixel(BYTE *pHead, int nPixelSize, int nRow, int nCol, int nRowlen, int nChannel, COLORREF ref = RGB(255, 255, 255));

// Ϊrow��col��ģ����������ֵ�����ش�С�ɲ���ָ��
void SetPixel(BYTE *pHead, int nPixelSize, int nRow, int nCol, int nRowlen, int nChannel, int R, int G, int B);
