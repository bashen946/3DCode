/**
* @file MatrixConvert2Image.h
* @brief ����תͼ��
*/


#include "DataTypes.h"

#pragma once
#include "ImageSrc.h"

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
	* @param[in] *mat ��ά�����
	* @param[in] nSymbleSize ��ά��ߴ�
	* @param[in] nPixelSize ���ش�С
	* @param[in] nQRMargin QR�߽�
	*/
	CMatrix2Image(qrMat *mat, int nSymbleSize, int nPixelSize, int nQRMargin = QR_MARGIN) :
		m_qrMat (mat),
		m_nQrSize (nSymbleSize),
		m_nPixelSize (nPixelSize),
		m_nQRMargin (nQRMargin), 
		m_Src(m_nPixelSize * m_nQrSize + m_nQRMargin * 2, m_nPixelSize * m_nQrSize + m_nQRMargin * 2, 4) { }

	~CMatrix2Image(){ }

	// ����ά�����ת��Ϊ��ɫͼ��
	BOOL CreateColorImage(COLORREF ForegroundColor, COLORREF BackgroundColor, 
		COLORREF QREncodeColor1, COLORREF QREncodeColor2);

	const ImageSrc* GetImageSrc() const { return &m_Src; }

private:
	// Ѱ���ĸ�Ԫ��֮����С�ģ���������
	int FindMinValueIndex(COLORREF x, COLORREF y, COLORREF z, COLORREF w);

	// Ϊ(nRow, nCol)���д�������ɫ
	void SetColorPixel(int nRow, int nCol, int nMatVal, COLORREF QREncodeColor1, COLORREF QREncodeColor2);

	qrMat*		m_qrMat;		/**< QR���� */
	int			m_nQrSize;		/**< QR�ߴ� */
	int			m_nPixelSize;	/**< ���ش�С */
	int			m_nQRMargin;	/**< QR�߽� */
	ImageSrc	m_Src;			/**< ͼ������ */
};