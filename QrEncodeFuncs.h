#ifdef WIN32

#pragma once
#include "CyImage.h"
#include "QR_Encode.h"

#include <vector>
using namespace std;


// QR������뷽��
BOOL EncodeData(CString strEncodeData, CQR_Encode* pQR_Encode, int nLevel, int nVersion, BOOL bAutoExtent, int nMaskingNo);

// ���������ö�ά��ת��Ϊͼ����ʾ������ǰ��ɫ����
vector<CPixelPoint> Convert2Image(CyImage* pImage, int nPixelSize, COLORREF ForegroundColor, COLORREF BackgroundColor,
								  CLogoRect LogoRect, CQR_Encode* pQR_Encode, BOOL bForegroundColorReturned = TRUE);

#endif