/**
* @file MatrixConvert2Image.h
* @brief ����תͼ��
*/

#ifdef WIN32

#include "DataTypes.h"

#pragma once

#define COLOR_FOREGROUND	0		//ǰ��ɫ
#define COLOR_BACKGROUND	1		//����ɫ
#define COLOR_QRCOLOR1		2		//��ɫһ(ǰ��)
#define COLOR_QRCOLOR2		17		//��ɫ��(����)
#define MAX_MODULESIZE		177		//QR�����ߴ�

class CMatrix2Image
{
public:
	/**
	* @brief 
	* @param[in] bMatrix ��ά������
	* @param[in] nSymbleSize ��ά��ߴ�
	* @param[in] *pImage Ŀ��ͼ��
	* @param[in] nPixelSize ���ش�С
	*/
	CMatrix2Image(qrMat *mat, int nSymbleSize, CImage* pImage, int nPixelSize) 
	{
		m_qrMat = mat;
		m_nQrSize = nSymbleSize;
		m_pImage = pImage;
		m_nPixelSize = nPixelSize;
		m_nWidth = m_nPixelSize * m_nQrSize + QR_MARGIN * 2;
		m_nHeight = m_nPixelSize * m_nQrSize + QR_MARGIN * 2;
		m_nChannel = 3;

		if (!m_pImage->IsNull())
			m_pImage->Destroy();
		m_pImage->Create(m_nWidth, m_nHeight, 8 * m_nChannel, 0UL);

		m_pHead = (BYTE*) m_pImage->GetBits() + m_pImage->GetPitch() * (m_pImage->GetHeight() - 1);
		m_nRowlen = abs(m_pImage->GetPitch());
		memset(m_pHead, 255, m_nHeight * m_nRowlen * sizeof(BYTE));
	}
	~CMatrix2Image(){ }

	// ����ά�����ת��Ϊ��ɫͼ��
	BOOL Matrix2ColorImage(COLORREF ForegroundColor, COLORREF BackgroundColor, 
		COLORREF QREncodeColor1, COLORREF QREncodeColor2);

private:
	// Ѱ���ĸ�Ԫ��֮����С�ģ���������
	int FindMinValueIndex(COLORREF x, COLORREF y, COLORREF z, COLORREF w);

	// Ϊ(nRow, nCol)���д�������ɫ
	void SetColorPixel(int nRow, int nCol, int nMatVal, COLORREF QREncodeColor1, COLORREF QREncodeColor2);

	qrMat *m_qrMat;
	int m_nQrSize;
	CImage* m_pImage;
	BYTE* m_pHead;
	int m_nWidth;
	int m_nHeight;
	int m_nRowlen;
	int m_nChannel;
	int m_nPixelSize;
};

#endif // WIN32