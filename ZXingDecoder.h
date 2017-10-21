
/** 
* @file ZXingDecoder.h
* @brief Qr����뺯�������Կ�ƽ̨
*/


#pragma once


#include <iostream>
#include <fstream>
#include <string>
#include "zxing/common/Counted.h"
#include "zxing/Binarizer.h"
#include "zxing/Result.h"
#include "zxing/ReaderException.h"
#include "zxing/common/GlobalHistogramBinarizer.h"
#include "zxing/common/HybridBinarizer.h"
#include <exception>
#include "zxing/Exception.h"
#include "zxing/common/IllegalArgumentException.h"
#include "zxing/BinaryBitmap.h"
#include "zxing/DecodeHints.h"
#include "zxing/qrcode/QRCodeReader.h"
#include "datatypes.h"
#include "QR_Encode.h"
#include "K_Means.h"

#include "ImageReaderSource.h"
#include "Transform.h"
#include "MedianFilter.h"
#include "PerspectTransform.h"
#include "templateFuncs.h"
#include "medianBlur.h"

using namespace std;
using namespace zxing;

#pragma warning(disable : 4101 4996)

/* ����ͼ�񵽸óߴ���н��� */
#define IMAGE_ZOOM_TO 320

/**
* @class ZXingDecoder
* @brief ZXing��ά�������
*/
template <class Type>
class ZXingDecoder
{
protected:
	// param[in]
	BYTE *		m_pImage;				/**< ����ͼ��[4ͨ��] */
	char *		m_pBuffer;				/**< ���뻺���� */
	int			m_nWidth;				/**< ����ͼ���� */
	int			m_nHeight;				/**< ����ͼ��߶� */
	BOOL		m_bUseHybrid;			/**< use hybrid */
	BOOL		m_bFilter;				/**< �˲� HINT */
	BOOL		m_bScreenImage;			/**< �Ƿ���Ļ��ͼ */
	DecodeHints m_hint;					/**< HINT */
	// param[out]
	float2		m_pos[4];				/**< ��λԪ���� */
	// QR data
	int			m_nStrlen;				/**< QR��������ݳ��� */
	char		m_pData[MAX_CODEUTF8NUM];/**< QR������ַ��� */
	int			m_nBackground;			/**< ԭQR�뱳��ɫ */
	char		m_sLastError[256];		/**< ���������Ϣ */

	/// ��ά�������Ķ���
	Ref<Type> m_READER;

public:
	/**
	* @brief ����һ��ZXing������
	* @param[in] h HINT
	* @param[in] bUseHybrid �������1
	* @param[in] bTryHarder �������2
	*/
	ZXingDecoder(DecodeHints h, BOOL bUseHybrid = TRUE, BOOL bTryHarder = TRUE)
	{
		memset(this, 0, sizeof(ZXingDecoder));
		m_hint = h;
		m_hint.setTryHarder(TRUE == bTryHarder);
		m_bUseHybrid = bUseHybrid;
		m_bFilter = TRUE;
		m_bScreenImage = FALSE;
	}

	~ZXingDecoder()
	{
		SAFE_DELETE(m_pImage);
		SAFE_DELETE(m_pBuffer);
	}

	// ��ʼ������ͼ��
	void SetImgSrc(const BYTE *pHead, int nWidth, int nHeight, int nChannel, const RoiRect & roi);

	// ǿ������QR��
	inline bool DecodeHard(COLORREF Background)
	{
		return DecodeSrcImage() ? true : 
			DecodeBackImage(Background, m_bFilter) ? true : DecodeBackImage(Background, !m_bFilter);
	}

	// ��ȡ��һ������
	inline const char* GetLastError() const { return m_sLastError; }

	/// ��ȡ����ͼ����
	inline int GetWidth() const { return m_nWidth; }

	/// ��ȡ����ͼ��߶�
	inline int GetHeight() const { return m_nHeight; }

	/// �Ƿ�Use Hybrid
	inline BOOL UseHybrid() const { return m_bUseHybrid; }

	/// ��ñ�����ɫ
	inline int GetBackground() const { return m_nBackground; }

	/// ��ý���ͼ��ָ��
	inline BYTE* GetImage() const { return m_pImage; }

	/// ��ý����ַ�������
	inline int GetStrlen() const { return m_nStrlen; }

	/// ��ȡ��λ����
	inline void GetPostion(float2 pos[4]) const { memcpy(pos, m_pos, 4 * sizeof(float2)); }

	/// ��ý�������
	inline const char* GetData() const { return m_pData; }

	/// �����������������Ѿ���ʼ���ĳ�Ա
	template <class T>
	inline void CopyOf(const ZXingDecoder<T> & other)
	{
		m_nWidth = other.GetWidth();
		m_nHeight = other.GetHeight();
		m_bUseHybrid = other.UseHybrid();
		m_pImage = new BYTE[4 * m_nWidth * m_nHeight];
		memcpy(m_pImage, other.GetImage(), 4 * m_nWidth * m_nHeight * sizeof(BYTE));
	}

	/// �Ƿ��п����ǽ���ͼ��
	bool IsScreenImage() const { return m_bScreenImage; }

private:
	// ����ZXing����QR��ͼ������
	bool CallZXingDecode(int nWidth, int nHeight, bool bFlip);

	// ����ZXing����ԭʼͼ��
	bool DecodeSrcImage();

	// ��ȡ����ͼ����н���
	bool DecodeBackImage(COLORREF Background, bool bBlur = true);

	/// ���õ�ǰ����Ļ���
	inline void SetDecBuffer(const BYTE* pSrc)
	{
		SAFE_DELETE(m_pBuffer);
		m_pBuffer = new char[4 * m_nWidth * m_nHeight];
		memcpy(m_pBuffer, pSrc, 4 * m_nWidth * m_nHeight * sizeof(char));
	}

	// K_means ����ָ�
	void K_means(BYTE* pHead, int nRowBytes, RoiRect roi, int nMaxepoches);

	/// x���ĸ����ĸ��������������ı��
	inline int CLUSTER(int x, int c1, int c2, int c3) const
	{
		register int d1 = abs(x - c1), d2 = abs(x - c2), d3 = abs(x - c3);
		return d1 < d2 ? (d1 < d3 ? 0 : 2) : (d2 < d3 ? 1 : 2);
	}
};


/**
* @brief ��ʼ������ͼ��
* @details ��Ҫ������ȡ����Ȥ�������ŵ�Ԥ����
* @param[in] *pHead ԭʼͼ��
* @param[in] nWidth ͼ����
* @param[in] nHeight ͼ��߶�
* @param[in] nChannel ͼ��ͨ��
* @param[in] roi ����Ȥ����
*/
template <class Type>
void ZXingDecoder<Type>::SetImgSrc(const BYTE *pHead, int nWidth, int nHeight, int nChannel, const RoiRect & roi)
{
	//////////////////////////////////////////////////////////////////////////
	/// Step1 ͼ��Ԥ����
	ASSERT(nChannel >= 3);
	m_bFilter = TRUE;
	int nRowlen = WIDTHBYTES(nWidth * 8 * nChannel);// ����ͼ��ÿ���ֽ���
	BYTE *pRoi = ImageROI(pHead, nWidth, nHeight, nRowlen, roi);// ��ȡ����Ȥ����
#ifdef _DEBUG
	ImageWrite(".\\ImageROI.txt", pRoi, nWidth, nHeight, nRowlen);
#endif
	double fRate = max((double)nWidth / nHeight, (double)nHeight / nWidth);// ��߱�
	if(fRate >= 1.5)// ��ȡ����ͼ��
	{
		m_bScreenImage = TRUE;
		m_bFilter = FALSE;// �ٶ���ͼ���˲�
		RoiRect cutAero;
		GetQrRect(pRoi, nWidth, nHeight, nChannel, nRowlen, cutAero);
		BYTE *pBackup = pRoi;
		pRoi = ImageROI(pRoi, nWidth, nHeight, nRowlen, cutAero);
		SAFE_DELETE(pBackup);
#ifdef _DEBUG
		ImageWrite(".\\CutImage.txt", pRoi, nWidth, nHeight, nRowlen);
#endif
	}
	LimitImage(&pRoi, nWidth, nHeight, nRowlen, nChannel, IMAGE_ZOOM_TO);// ����
	m_nWidth = nWidth;  // ͼ����
	m_nHeight = nHeight; // ͼ��߶�

	/// Step2 ��ȡ�����ַ���
	SAFE_DELETE(m_pImage);
	m_pImage = GetDecodeString(pRoi, m_nWidth, m_nHeight, nRowlen);
	// ������
	SAFE_DELETE(pRoi);

	//////////////////////////////////////////////////////////////////////////
}


/**
* @brief K_means ����ָ�.
* @param[in] *pHead ͼ������
* @param[in] nRowBytes ÿ���ֽ���
* @param[in] roi ͼ�����Ȥ����
* @param[in] nMaxepoches ����������
* @note (1)�����ǻҶ�ͼ��;(2)�˺������޸�ͼ������pHead.
*/
template <class Type>
void ZXingDecoder<Type>::K_means(BYTE* pHead, int nRowBytes, RoiRect roi, int nMaxepoches)
{
	// ��������ĳߴ�
	const int nWidth = roi.Width();
	const int nHeight = roi.Height();
	const int nRowlen = nWidth;// ������ÿ��Ԫ�ظ���
	BYTE* Cluster = new BYTE[nHeight * nRowlen];// ������TAG
	// ��������
	int Center[3], oldCenter[3];
	int sumCenter[3];// �����������
	int sum[3];// ÿ����������Ԫ�ظ���
	// ����3������
	InitializeClusterCenters<int, BYTE>(Center, 3, pHead, nRowBytes, roi);
	/* ��ε���ֱ�������������������nMaxepoches�� */
	for (int it = 0; it < nMaxepoches; ++it)
	{
		sumCenter[0] = sumCenter[1] = sumCenter[2] = 0;
		sum[0] = sum[1] = sum[2] = 0;
		/* ���ÿ���������Ӧ��������һ������ */
		BYTE *pLeftTop = pHead + roi.top * nRowBytes + roi.left;
		int y = - roi.left;
		for (int j = roi.top; j < roi.bottom; ++j)
		{
			BYTE *pCur = pLeftTop;
			for (int i = roi.left; i < roi.right; ++i)
			{
				/* ��ǰ���صĸ���ֵ */
				register int curPixel = *pCur++;
				register int tag = CLUSTER(curPixel, Center[0], Center[1], Center[2]);
				// ��(i, j)���Ϊtag
				Cluster[i + y] = tag;
				sumCenter[tag] += curPixel;
				++sum[tag];
			}
			pLeftTop += nRowBytes;
			y += nRowlen;
		}
		/* ������һ������������¾������� */
		oldCenter[0] = Center[0];
		oldCenter[1] = Center[1];
		oldCenter[2] = Center[2];
		if (sum[0]) Center[0] = sumCenter[0] / sum[0];
		if (sum[1]) Center[1] = sumCenter[1] / sum[1];
		if (sum[2]) Center[2] = sumCenter[2] / sum[2];
		int diff = abs(oldCenter[0] - Center[0]) + abs(oldCenter[1] - Center[1])
			+ abs(oldCenter[2] - Center[2]);
#ifdef _DEBUG
		TRACE(" * �������� = %d��\n", it + 1);
		for (int i = 0; i < 3; ++i)
		{
			TRACE("C[%d] = %.6f\t%d��\t", i, (float)Center[i], sum[i]);
		}
		TRACE("\n");
#endif // _DEBUG
		// ��ǰ�������������ı仯��Сʱ��0.01�����أ��˳�
		if (diff < 3)
			break;
	}

	// Ѱ�ұ���ֵ��Ӧ�����
	int backIdx = CLUSTER(m_nBackground, Center[0], Center[1], Center[2]);
	int backVal = Center[backIdx] < 128 ? 0 : 255, foreVal = 255 - backVal;// ������ǰ��

	// ��ԭʼͼ����зָ�
	BYTE *pSrc0 = Cluster;
	BYTE *pDst0 = pHead + roi.left + roi.top * nRowBytes;
	for (int j = 0; j < nHeight; ++j)
	{
		BYTE *pSrc = pSrc0;
		BYTE *pDst = pDst0;
		for (int i = 0; i < nWidth; ++i)
		{
			*pDst ++ = (*pSrc++ == backIdx) ? backVal : foreVal;
		}
		pSrc0 += nRowlen;
		pDst0 += nRowBytes;
	}
	delete [] Cluster;
}


/** 
* @brief ����δ���˲���K�ָ��ԭʼͼ��
*/
template <class Type>
bool ZXingDecoder<Type>::DecodeSrcImage()
{
	int W = m_nWidth, H = m_nHeight;

	// 2017-7-27 ��߽���Ч��, ������䲻����
	SetDecBuffer(m_pImage);
	if (!CallZXingDecode(W, H, false))
		if (CallZXingDecode(W, H, true))
			ImageFlipV(m_pImage, W, H, 4 * W);

	return m_nStrlen;
}


/**
* @brief ��ȡͼ��ı���ͼ��(QR��)���н���
* @param[in] Background QR��ı���ɫ
* @param[in] bBlur �Ƿ�����˲�
*/
template <class Type>
bool ZXingDecoder<Type>::DecodeBackImage(COLORREF Background, bool bBlur)
{
	int W = m_nWidth, H = m_nHeight;

	//////////////////////////////////////////////////////////////////////////
	// ĳЩͼ����Ҫ�˲�(�˲��˱߳�Ϊ3�����˹���)
	// ���4ͨ��ͼ����ÿ�����ֵ�˲����������MedianFilter
	if (bBlur)
	{
		FastMedianFilter(m_pImage, W, H, 4 * W);
		//medianBlur_SortNet<MinMax8u, MinMaxVec8u>(m_pImage, W, H, 4 * W, 4);
#ifdef _DEBUG
		ImageWrite(".\\MedianFilter.txt", m_pImage, W, H, 4 * W);
#endif
	}

	BYTE* gray = Rgb2Gray(m_pImage, W, H, 4 * W);// �Ҷ�ͼ��
	m_nBackground = RgbColorRef2Gray(Background);// ������ɫ
	int nRowlenNew = WIDTHBYTES(W * 8);// �Ҷ�ͼ��ÿ���ֽ���
	BYTE *p = (BYTE*)gray;
#ifdef _DEBUG
	ImageWrite(".\\Rgb2Gray.txt", gray, W, H, nRowlenNew);
#endif
	// -------------------
	// | a11 | a12 | a13 |
	// | a21 | a22 | a23 |
	// | a31 | a32 | a33 |
	// -------------------
	// �Խ���
	K_means(p, nRowlenNew, RoiRect(0, 0, W/3, H/3), 10);// a11
	K_means(p, nRowlenNew, RoiRect(W / 3, H / 3, W * 2 / 3, H * 2 / 3), 10);// a22
	K_means(p, nRowlenNew, RoiRect(W * 2 / 3, H * 2 / 3, W, H), 10);// a33
	// ���½�
	K_means(p, nRowlenNew, RoiRect(0, H / 3, W / 3, H * 2 / 3), 10);// a21
	K_means(p, nRowlenNew, RoiRect(0, H * 2 / 3, W / 3, H), 10);// a31
	K_means(p, nRowlenNew, RoiRect(W / 3, H * 2 / 3, W * 2 / 3, H), 10);// a32
	// ���Ͻ�
	K_means(p, nRowlenNew, RoiRect(W / 3, 0, W * 2 / 3, H / 3), 10);// a12
	K_means(p, nRowlenNew, RoiRect(W * 2 / 3, 0, W, H / 3), 10);// a13
	K_means(p, nRowlenNew, RoiRect(W * 2 / 3, H / 3, W, H * 2 / 3), 10);// a23
#ifdef _DEBUG
	ImageWrite(".\\K_means.txt", gray, W, H, nRowlenNew);
#endif
	BYTE* gray_decstring = GetDecodeString(gray, W, H, nRowlenNew);
	SetDecBuffer(gray_decstring);
	if (!CallZXingDecode(W, H, false))
		if (CallZXingDecode(W, H, true))
			ImageFlipV(m_pImage, W, H, 4 * W);
	SAFE_DELETE(gray_decstring);
	SAFE_DELETE(gray);
	return m_nStrlen;
}


/** 
* @brief ����ͼ������
* @param[in] nWidth ���뻺��ͼ����
* @param[in] nHeight ���뻺��ͼ��߶�
* @param[in] bFlip �Ƿ�תͼ����н���
*/
template <class Type>
bool ZXingDecoder<Type>::CallZXingDecode(int nWidth, int nHeight, bool bFlip)
{
	if (bFlip)
	{
		ImageFlipV(m_pBuffer, nWidth, nHeight, 4 * nWidth);
	}

	// ���Դ���LuminanceSource
	Ref<LuminanceSource> source;
	try
	{
		source = ImageReaderSource::create(m_pBuffer, nWidth, nHeight, 3);
	}
	catch (const zxing::IllegalArgumentException &e)
	{
		TRACE(" * zxing::IllegalArgumentException: %s\n", e.what());
		strcpy(m_sLastError, e.what());
		return false;
	}
	catch (...)
	{
		return false;
	}

	// ���Խ��н���
	vector<Ref<Result> > results;
	Ref<Binarizer> binarizer;
	try
	{
		// ��ö�ֵ��ͼ��
		binarizer = m_bUseHybrid ? new HybridBinarizer(source) : 
			new GlobalHistogramBinarizer(source);
		Ref<BinaryBitmap> binary(new BinaryBitmap(binarizer));
		// ��ʼ��������
		m_READER = new Type;
		// ���н���
		Ref<Result> result = m_READER->decode(binary, m_hint);

		results = vector<Ref<Result> >(1, result);
	}
	catch (const zxing::ReaderException& e)
	{
		TRACE(" * zxing::ReaderException: %s\n", e.what());
		strcpy(m_sLastError, e.what());
		return false;
	}
	catch (const zxing::IllegalArgumentException& e)
	{
		TRACE(" * zxing::IllegalArgumentException: %s\n", e.what());
		strcpy(m_sLastError, e.what());
		return false;
	}
	catch (const zxing::Exception& e)
	{
		TRACE(" * zxing::Exception: %s\n", e.what());
		strcpy(m_sLastError, e.what());
		return false;
	}
	catch (const std::exception& e)
	{
		TRACE(" * std::exception: %s\n", e.what());
		strcpy(m_sLastError, e.what());
		return false;
	}
	catch (...)
	{
		return false;
	}

	if (results.size() > 0)
	{
		string temp = results[0]->getText()->getText();
		const char *chr = temp.c_str();
		m_nStrlen = temp.length();
		if (m_nStrlen >= MAX_CODEUTF8NUM)
			return false;
		strcpy(m_pData, chr);
#ifdef _DEBUG
		TRACE(" * Data[%d]: %s \n", m_nStrlen, chr);
		TRACE(" * Format: %s \n", BarcodeFormat::barcodeFormatNames[results[0]->getBarcodeFormat()]);
#endif // _DEBUG
		for (int j = 0; j < results[0]->getResultPoints()->size(); j++)
		{
			// p0-����; p1-����(ԭ��); p2-����; p3-alignmentPattern
			m_pos[j].x = results[0]->getResultPoints()[j]->getX();
			m_pos[j].y = results[0]->getResultPoints()[j]->getY();
#ifdef _DEBUG
			TRACE(" * Point[%d]:(%f, %f) \n", j, m_pos[j].x, m_pos[j].y);
#endif // _DEBUG
		}
		return true;
	}

	return false;
}
