#include "stdafx.h"
#include "MatrixConvert2Image.h"
#include "PixelsOperation.h"


/// Ѱ����Сֵ���±�
int FindMinValueIndex(COLORREF x, COLORREF y, COLORREF z, COLORREF w)
{
	COLORREF min1 = x < y ? x : y;
	COLORREF min2 = z < w ? z : w;
	int idx1 = x < y ? 1 : 2;
	int idx2 = z < w ? 3 : 4;
	return min1 < min2 ? idx1 : idx2;
}


/** - ת��ͼ�� -
* @param[in] * bDMatrix				01����
* @param[in] nSymbleSize			��ά��ߴ�
* @param[in] * pImage				��ά��ͼ��
* @param[in] nPixelSize				���ش�С
* @param[in] ForegroundColor		ǰ��ɫ
* @param[in] BackgroundColor		����ɫ
*/
BOOL Matrix2Image(BYTE* bDMatrix, int nSymbleSize, CImage* pImage, int nPixelSize, 
				  COLORREF ForegroundColor, COLORREF BackgroundColor)
{
	int nWidth = nPixelSize * nSymbleSize + QR_MARGIN * 2;
	int nHeight = nPixelSize * nSymbleSize + QR_MARGIN * 2;

	if (!pImage->IsNull())
		pImage->Destroy();
	if (FALSE == pImage->Create(nWidth, nHeight, 24, 0UL))
		return FALSE;

	BYTE* pHead = (BYTE*) pImage->GetBits() + pImage->GetPitch() * (pImage->GetHeight() - 1);
	int nRowlen = abs(pImage->GetPitch());
	memset(pHead, 255, nHeight * nRowlen * sizeof(BYTE));

	for (int i = 0; i < nSymbleSize; ++i)
	{
		int nRow = i * nPixelSize;
		for (int j = 0; j < nSymbleSize; ++j)
		{
			int nCol = j * nPixelSize;
			if (bDMatrix[j + (nSymbleSize - i - 1) * nSymbleSize])
			{
				// ���ñ���ɫ
				SetPixel(pHead, nPixelSize, nRow, nCol, nRowlen, 3, BackgroundColor);
			}
			else
			{
				// ����ǰ��ɫ
				SetPixel(pHead, nPixelSize, nRow, nCol, nRowlen, 3, ForegroundColor);
			}
		}
	}
	// Three isolated points(����������)
	SetPixel(pHead, nPixelSize, 7 * nPixelSize, 7 * nPixelSize, nRowlen, 3, ForegroundColor);
	SetPixel(pHead, nPixelSize, 7 * nPixelSize, (nSymbleSize - 8) * nPixelSize, nRowlen, 3, ForegroundColor);
	SetPixel(pHead, nPixelSize, (nSymbleSize - 8) * nPixelSize, 7 * nPixelSize, nRowlen, 3, ForegroundColor);

	return TRUE;
}


/** ����ά�����ת��Ϊ��ɫͼ��. 
* @param[in] bMatrix ��ά������
* @param[in] nSymbleSize ��ά��ߴ�
* @param[in] * pImage Ŀ��ͼ��
* @param[in] nPixelSize ���ش�С
* @param[in] ForegroundColor ǰ��ɫ
* @param[in] BackgroundColor ����ɫ
* @param[in] QREncodeColor1 ��ɫһ
* @param[in] QREncodeColor2 ��ɫ��
*/
BOOL Matrix2ColorImage(BYTE bMatrix[MAX_MODULESIZE][MAX_MODULESIZE], int nSymbleSize, CImage* pImage, int nPixelSize, 
					   COLORREF ForegroundColor, COLORREF BackgroundColor, COLORREF QREncodeColor1, COLORREF QREncodeColor2)
{
	// ����ģ��ĻҶ�ֵ������С
	int minIndex = FindMinValueIndex(ForegroundColor, BackgroundColor, QREncodeColor1, QREncodeColor2);
	if (minIndex != 2)
	{
		TRACE(" * ���棺����ģ��ĻҶ�ֵ������С�������ڽ���!\n");
	}

	int nWidth = nPixelSize * nSymbleSize + QR_MARGIN * 2;
	int nHeight = nPixelSize * nSymbleSize + QR_MARGIN * 2;

	if (!pImage->IsNull())
		pImage->Destroy();
	if (FALSE == pImage->Create(nWidth, nHeight, 24, 0UL))
		return FALSE;

	BYTE* pHead = (BYTE*) pImage->GetBits() + pImage->GetPitch() * (pImage->GetHeight() - 1);
	int nRowlen = abs(pImage->GetPitch());
	memset(pHead, 255, nHeight * nRowlen * sizeof(BYTE));

	for (int i = 0; i < nSymbleSize; ++i)
	{
		int nRow = i * nPixelSize;
		for (int j = 0; j < nSymbleSize; ++j)
		{
			int nCol = j * nPixelSize;
			switch (bMatrix[i][j])
			{
			case COLOR_FOREGROUND:
				SetPixel(pHead, nPixelSize, nRow, nCol, nRowlen, 3, ForegroundColor);break;
			case COLOR_BACKGROUND:
				SetPixel(pHead, nPixelSize, nRow, nCol, nRowlen, 3, BackgroundColor);break;
			case COLOR_QRCOLOR1:
				SetPixel(pHead, nPixelSize, nRow, nCol, nRowlen, 3, QREncodeColor1);break;
			case COLOR_QRCOLOR2:
				SetPixel(pHead, nPixelSize, nRow, nCol, nRowlen, 3, QREncodeColor2);break;
			default:
				break;
			}
		}
	}

	return TRUE;
}