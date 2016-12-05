#include "stdafx.h"
#include "MatrixConvert2Image.h"
#include "PixelsOperation.h"


int FindMinValueIndex(COLORREF x, COLORREF y, COLORREF z, COLORREF w)
{
	COLORREF min1 = x < y ? x : y;
	COLORREF min2 = z < w ? z : w;
	int idx1 = x < y ? 1 : 2;
	int idx2 = z < w ? 3 : 4;
	return min1 < min2 ? idx1 : idx2;
}


/* - ת��ͼ�� -
BYTE* bDMatrix					01����
int nSymbleSize					��ά��ߴ�
CyImage* pImage					��ά��ͼ��
int nPixelSize					���ش�С
COLORREF ForegroundColor		ǰ��ɫ
COLORREF BackgroundColor		����ɫ
*/
BOOL Matrix2Image(BYTE* bDMatrix, int nSymbleSize, CyImage* pImage, int nPixelSize, COLORREF ForegroundColor, COLORREF BackgroundColor)
{
	int nWidth = nPixelSize * nSymbleSize + QR_MARGIN * 2;
	int nHeight = nPixelSize * nSymbleSize + QR_MARGIN * 2;

	if (FALSE == pImage->Create(nWidth, nHeight, 24, 0UL))
		return FALSE;

	BYTE* pHead = pImage->GetHeadAddress();
	int nRowlen = pImage->GetRowlen();
	memset(pHead, 255, nHeight * nRowlen * sizeof(BYTE));

	for (int i = 0; i < nSymbleSize; ++i)
	{
		for (int j = 0; j < nSymbleSize; ++j)
		{
			if (bDMatrix[j + (nSymbleSize - i - 1) * nSymbleSize])
			{
				// ���ñ���ɫ
				SetPixel(pHead, nPixelSize, i * nPixelSize, j * nPixelSize, nRowlen, 3, BackgroundColor);
			}
			else
			{
				// ����ǰ��ɫ
				SetPixel(pHead, nPixelSize, i * nPixelSize, j * nPixelSize, nRowlen, 3, ForegroundColor);
			}
		}
	}
	// Three isolated points(����������)
	SetPixel(pHead, nPixelSize, 7 * nPixelSize, 7 * nPixelSize, nRowlen, 3, ForegroundColor);
	SetPixel(pHead, nPixelSize, 7 * nPixelSize, (nSymbleSize - 8) * nPixelSize, nRowlen, 3, ForegroundColor);
	SetPixel(pHead, nPixelSize, (nSymbleSize - 8) * nPixelSize, 7 * nPixelSize, nRowlen, 3, ForegroundColor);

	// �޸ĸ�������
	pImage->MemcpyByteToFloat();
	return TRUE;
}


/** ����ά�����ת��Ϊ��ɫͼ��. 
��ά�����ݣ�BYTE bMatrix[MAX_MODULESIZE][MAX_MODULESIZE]
��ά��ߴ磺int nSymbleSize
Ŀ��ͼ��CyImage* pImage
���ش�С��int nPixelSize
ǰ��ɫ��COLORREF ForegroundColor
����ɫ��COLORREF BackgroundColor
��ɫһ��COLORREF QREncodeColor1
��ɫ����COLORREF QREncodeColor2
*/
BOOL Matrix2ColorImage(BYTE bMatrix[MAX_MODULESIZE][MAX_MODULESIZE], int nSymbleSize, CyImage* pImage, int nPixelSize, 
					   COLORREF ForegroundColor, COLORREF BackgroundColor, COLORREF QREncodeColor1, COLORREF QREncodeColor2)
{
	// ����ģ��ĻҶ�ֵ������С
	int minIndex = FindMinValueIndex(ForegroundColor, BackgroundColor, QREncodeColor1, QREncodeColor2);
	if (minIndex != 2)
	{
		TRACE(" * ���棺����ģ��ĻҶ�ֵ������С�������ڽ���!");
	}

	int nWidth = nPixelSize * nSymbleSize + QR_MARGIN * 2;
	int nHeight = nPixelSize * nSymbleSize + QR_MARGIN * 2;

	if (FALSE == pImage->Create(nWidth, nHeight, 24, 0UL))
		return FALSE;

	BYTE* pHead = pImage->GetHeadAddress();
	int nRowlen = pImage->GetRowlen();
	memset(pHead, 255, nHeight * nRowlen * sizeof(BYTE));

	for (int i = 0; i < nSymbleSize; ++i)
	{
		for (int j = 0; j < nSymbleSize; ++j)
		{
			switch (bMatrix[i][j])
			{
			case COLOR_FOREGROUND:
				SetPixel(pHead, nPixelSize, i * nPixelSize, j * nPixelSize, nRowlen, 3, ForegroundColor);break;
			case COLOR_BACKGROUND:
				SetPixel(pHead, nPixelSize, i * nPixelSize, j * nPixelSize, nRowlen, 3, BackgroundColor);break;
			case COLOR_QRCOLOR1:
				SetPixel(pHead, nPixelSize, i * nPixelSize, j * nPixelSize, nRowlen, 3, QREncodeColor1);break;
			case COLOR_QRCOLOR2:
				SetPixel(pHead, nPixelSize, i * nPixelSize, j * nPixelSize, nRowlen, 3, QREncodeColor2);break;
			default:
				break;
			}
		}
	}

	// �޸ĸ�������
	pImage->MemcpyByteToFloat();
	return TRUE;
}