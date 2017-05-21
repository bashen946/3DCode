#include "stdafx.h"
#include "MatrixConvert2Image.h"
#include "PixelsOperation.h"


/// Ѱ����Сֵ���±�
int CMatrix2Image::FindMinValueIndex(COLORREF x, COLORREF y, COLORREF z, COLORREF w)
{
	COLORREF min1 = x < y ? x : y;
	COLORREF min2 = z < w ? z : w;
	int idx1 = x < y ? 1 : 2;
	int idx2 = z < w ? 3 : 4;
	return min1 < min2 ? idx1 : idx2;
}


/** 
* @brief ����ά�����ת��Ϊ��ɫͼ��. 
* @param[in] bMatrix ��ά������
* @param[in] nSymbleSize ��ά��ߴ�
* @param[in] *pImage Ŀ��ͼ��
* @param[in] nPixelSize ���ش�С
* @param[in] ForegroundColor ǰ��ɫ
* @param[in] BackgroundColor ����ɫ
* @param[in] QREncodeColor1 ��ɫһ
* @param[in] QREncodeColor2 ��ɫ��
*/
BOOL CMatrix2Image::CreateColorImage(COLORREF ForegroundColor, COLORREF BackgroundColor, 
									  COLORREF QREncodeColor1, COLORREF QREncodeColor2)
{
	// ����ģ��ĻҶ�ֵ������С
	int minIndex = FindMinValueIndex(ForegroundColor, BackgroundColor, QREncodeColor1, QREncodeColor2);
	if (minIndex != 2)
	{
		TRACE(" * ���棺����ģ��ĻҶ�ֵ������С�������ڽ���!\n");
	}

	BYTE *m_pHead = m_Src.GetImage();
	int m_nRowlen = m_Src.GetRowlen();
	int m_nChannel = m_Src.GetChannel();
	for (int i = 0; i < m_nQrSize; ++i)
	{
		int nRow = i * m_nPixelSize;
		for (int j = 0; j < m_nQrSize; ++j)
		{
			int nCol = j * m_nPixelSize;
			switch (m_qrMat[i][j])
			{
			case COLOR_FOREGROUND:
				::SetPixel(m_pHead, m_nPixelSize, nRow, nCol, m_nRowlen, m_nChannel, ForegroundColor);
				break;
			case COLOR_BACKGROUND:
				::SetPixel(m_pHead, m_nPixelSize, nRow, nCol, m_nRowlen, m_nChannel, BackgroundColor);
				break;
			default:
				SetColorPixel(nRow, nCol, m_qrMat[i][j], QREncodeColor1, QREncodeColor2);
				break;
			}
		}
	}

	return TRUE;
}


/*** 
* @brief Ϊ(nRow, nCol)���д�������ɫ
* @param[in] nRow ��
* @param[in] nCol ��
* @param[in] nMatVal ��ɫֵ����
* @details һ��ģ�鱻��Ϊ4���֣�����a1,a2,a3,a4˳����ɫ. \n
*			a1		a2	\n
*			a3		a4	\n
*/
void CMatrix2Image::SetColorPixel(int nRow, int nCol, int nMatVal, COLORREF QREncodeColor1, COLORREF QREncodeColor2)
{
	int b[4], v = nMatVal - COLOR_QRCOLOR1;
	b[0] =  v     & 1;
	b[1] = (v>>1) & 1;
	b[2] = (v>>2) & 1;
	b[3] = (v>>3) & 1;
	int R[2] = {GetRValue(QREncodeColor1), GetRValue(QREncodeColor2)};
	int G[2] = {GetGValue(QREncodeColor1), GetGValue(QREncodeColor2)};
	int B[2] = {GetBValue(QREncodeColor1), GetBValue(QREncodeColor2)};
	int nHalfPixel = m_nPixelSize >> 1;
	for (int i = 0; i < nHalfPixel; ++i)
	{
		// ��ָ��
		BYTE *pRow = m_Src.GetImage() + (nRow + i + m_nQRMargin) * m_Src.GetRowlen();
		for (int j = 0; j < nHalfPixel; ++j)//a1
		{
			// ����ָ��
			BYTE *Pixel = pRow + m_Src.GetChannel() * (j + nCol + m_nQRMargin);
			Pixel[0] = B[b[0]];
			Pixel[1] = G[b[0]];
			Pixel[2] = R[b[0]];
		}
		for (int j = nHalfPixel; j < m_nPixelSize; ++j)//a2
		{
			// ����ָ��
			BYTE *Pixel = pRow + m_Src.GetChannel() * (j + nCol + m_nQRMargin);
			Pixel[0] = B[b[1]];
			Pixel[1] = G[b[1]];
			Pixel[2] = R[b[1]];
		}
	}
	for (int i = nHalfPixel; i < m_nPixelSize; ++i)
	{
		// ��ָ��
		BYTE *pRow = m_Src.GetImage() + (nRow + i + m_nQRMargin) * m_Src.GetRowlen();
		for (int j = 0; j < nHalfPixel; ++j)//a3
		{
			// ����ָ��
			BYTE *Pixel = pRow + m_Src.GetChannel() * (j + nCol + m_nQRMargin);
			Pixel[0] = B[b[2]];
			Pixel[1] = G[b[2]];
			Pixel[2] = R[b[2]];
		}
		for (int j = nHalfPixel; j < m_nPixelSize; ++j)//a4
		{
			// ����ָ��
			BYTE *Pixel = pRow + m_Src.GetChannel() * (j + nCol + m_nQRMargin);
			Pixel[0] = B[b[3]];
			Pixel[1] = G[b[3]];
			Pixel[2] = R[b[3]];
		}
	}
}