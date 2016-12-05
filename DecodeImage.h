#ifdef WIN32

#pragma once

#include "CyImage.h"

/************************************************************************/
// ���ļ���������QR��ĺ���
/************************************************************************/

// ��ȡ��������ַ���
BYTE* GetDecodeString(CyImage* pImage);

// ��ȡ�������ִ�
void GetDecodeString(BYTE* pDst, CyImage* pImage);

// ����ͼ��
BOOL DecodeImage(CyImage* pImage, CString &strDecodeString, int &ncLength, float2 pos[4], float &fModuleSize, int &nLevel, int &nVersion, int &nMaskingNo, BOOL use_hybrid, BOOL try_harder);

#endif // WIN32