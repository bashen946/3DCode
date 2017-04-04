#include "QR_Encode.h"
#include <vector>
using namespace std;

/**
* @file DecodeFuncs.h
* @brief ��ɫ��ά����뺯����ANDROID����
*/

/* ����ͼ�񵽸óߴ���н��� */
#define IMAGE_ZOOM_TO 320

#ifndef NEW_VERSION /// ��������꣬��ʾ�����°�ı�����뺯��

// �ж�����(i��, j��)���ھ�����
BOOL IndexNotInRect(int i, int j, const CLogoRect &rect);

// �����������󣬻�ȡǰ��ɫ����
vector<CPixelPoint> GetForegroundColorIndex(const CQR_Encode* pQR_Encode, BOOL bForegroundColorReturned = TRUE, 
											CLogoRect logoRect = CLogoRect(0, 0, 0, 0));

// �����ɫ����(�ڲ����ú���)
BYTE* DecodePrivateData(const BYTE *pHead, int nRowlen, int nChannel, int nPixelSize, int nSymbolSize,
						COLORREF QREncodeColor1, COLORREF QREncodeColor2,
						int nLevel, int nVersion, BOOL bAutoExtent, int nMaskingNo, 
						char* strPublicString, int ncLength, int &strLen, int &nInnerecLevel, int &nInnerMask);
#endif

/// �����ɫͼ��
BOOL DecodeWholeImage(char* pDst[2], const BYTE *pHead, int nWidth, int nHeight, int nChannel, 
					  float2 m_fPos[4], float &m_fModuleSize, int &nLevel, int &nVersion, int &nMaskingNo, 
					  BOOL bUseHybrid, BOOL bTryHarder, 
					  COLORREF Foreground, COLORREF Background, 
					  COLORREF QREncodeColor1, COLORREF QREncodeColor2, 
					  int &strLen, int &nInnerecLevel, int &nInnerMask, 
					  const RoiRect roi = RoiRect(0, 0, 0, 0));

/// �����ɫͼ��
void DecodeWholeImage(char* pDst[2], const int *pIntHead, int nWidth, int nHeight, 
					  float2 m_fPos[4], float &m_fModuleSize, int &nLevel, int &nVersion, int &nMaskingNo, 
					  BOOL bUseHybrid, BOOL bTryHarder, 
					  COLORREF Foreground, COLORREF Background, 
					  COLORREF QREncodeColor1, COLORREF QREncodeColor2, 
					  int &strLen, int &nInnerecLevel, int &nInnerMask, 
					  const RoiRect roi = RoiRect(0, 0, 0, 0));

/// �����ɫͼ��
void EasyDecode(char* pDst[2], const BYTE *pHead, int nWidth, int nHeight, int nChannel, 
				COLORREF Foreground, COLORREF Background, 
				COLORREF QREncodeColor1, COLORREF QREncodeColor2, 
				const RoiRect roi = RoiRect(0, 0, 0, 0));