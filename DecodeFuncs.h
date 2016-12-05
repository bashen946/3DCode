#include "DataTypes.h"

/************************************************************************/
/// ��ɫ��ά����뺯����ANDROID����
/************************************************************************/

/* ����ͼ�񵽸óߴ���н��� */
#define IMAGE_ZOOM_TO 320


/// �����ɫͼ��
BOOL DecodeWholeImage(char* pDst[2], BYTE *pHead, int nWidth, int nHeight, int nChannel, 
					  float2 m_fPos[4], float &m_fModuleSize, int &nLevel, int &nVersion, int &nMaskingNo, 
					  BOOL bUseHybrid, BOOL bTryHarder, 
					  COLORREF Foreground, COLORREF Background, 
					  COLORREF QREncodeColor1, COLORREF QREncodeColor2, 
					  int &strLen, int &nInnerecLevel, int &nInnerMask, 
					  RoiRect roi = RoiRect(0, 0, 0, 0));

/// �����ɫͼ��
void DecodeWholeImage(char* pDst[2], int *pIntHead, int nWidth, int nHeight, 
					  float2 m_fPos[4], float &m_fModuleSize, int &nLevel, int &nVersion, int &nMaskingNo, 
					  BOOL bUseHybrid, BOOL bTryHarder, 
					  COLORREF Foreground, COLORREF Background, 
					  COLORREF QREncodeColor1, COLORREF QREncodeColor2, 
					  int &ncLength, int &strLen, int &nInnerecLevel, int &nInnerMask, 
					  RoiRect roi = RoiRect(0, 0, 0, 0));

/// �����ɫͼ��
void EasyDecode(char* pDst[2], BYTE *pHead, int nWidth, int nHeight, int nChannel, 
				COLORREF Foreground, COLORREF Background, 
				COLORREF QREncodeColor1, COLORREF QREncodeColor2, 
				RoiRect roi = RoiRect(0, 0, 0, 0));