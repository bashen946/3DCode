
/** 
* @file QrDecoder.h
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

using namespace std;
using namespace zxing;
using namespace zxing::qrcode;

/* ����ͼ�񵽸óߴ���н��� */
#define IMAGE_ZOOM_TO 320

class QrDecoder
{
private:
	// param[in]
	char *		m_pImage;				/**< ����ͼ��[4ͨ��] */
	char *		m_pBuffer;				/**< ���뻺���� */
	int			m_nWidth;				/**< ����ͼ���� */
	int			m_nHeight;				/**< ����ͼ��߶� */
	// param[out]
	float2		m_pos[4];				/**< ��λԪ���� */
	float		m_fModuleSize;			/**< QR��ģ��ߴ� */
	int			m_nLevel;				/**< QR�����ȼ� */
	int			m_nVersion;				/**< QR��汾�� */
	int			m_nMaskingNo;			/**< QR������� */
	// QR data
	int			m_nStrlen;				/**< QR��������ݳ��� */
	static char	m_pData[MAX_CODEUTF8NUM];/**< QR������ַ��� */
	CQR_Encode* m_qr;					/**< ԭQR�� */
	int			m_nBackground;			/**< ԭQR�뱳��ɫ */
public:
	QrDecoder()
	{
		memset(this, 0, sizeof(QrDecoder));
	}

	~QrDecoder()
	{
		SAFE_DELETE(m_pImage);
		SAFE_DELETE(m_pBuffer);
		if (m_qr) delete m_qr;
	}

	// ��ʼ������ͼ��
	void Init(const BYTE *pHead, int nWidth, int nHeight, int nChannel, const RoiRect & roi);

	// ����QR��
	bool Decode(COLORREF Background, BOOL bUseHybrid, BOOL bTryHarder);

	// ͸����ת�任
	int perspectTransform(BYTE **pDst, int nMoudleSize) const;

	/// ���QR����ؾ���
	inline qrMat* InverseQr()
	{
		if (!m_qr)
		{
			m_qr = new CQR_Encode;
			m_qr->EncodeDataUtf8(m_nLevel, m_nVersion, TRUE, m_nMaskingNo, m_pData, m_nStrlen);
		}
		return m_qr->m_byModuleData;
	}

	/// ��ȡ����ͼ����
	inline int GetWidth() const { return m_nWidth; }

	/// ��ȡ����ͼ��߶�
	inline int GetHeight() const { return m_nHeight; }

	/// ��ý���ͼ��ָ��
	inline char* GetImage() const { return m_pImage; }

	/// ��ý����ַ�������
	inline int GetStrlen() const { return m_nStrlen; }

	/// ���QR���С
	inline int GetQrSize() const { return 17 + 4 * m_nVersion; }

	/// ��ȡ������Ϣ
	inline void GetQrInfo(float &fModuleSize, int &nLevel, int &nVersion, int &nMaskingNo) const 
	{
		fModuleSize = m_fModuleSize;
		nLevel = m_nLevel;
		nVersion = m_nVersion;
		nMaskingNo = m_nMaskingNo;
	}

	/// ��ý�������
	inline const char* GetQrData() const { return m_pData; }

	/// ��ȡ��λ����
	inline void GetPostion(float2 pos[4]) const { memcpy(pos, m_pos, 4 * sizeof(float2)); }

private:
	// ����ZXing����QR��
	vector<Ref<Result> > decode(Ref<BinaryBitmap> image, DecodeHints hints, 
		float &fModuleSize, int &nLevel, int &nVersion, int &nMaskingNo);

	// ����ͼ������
	bool DecodePublicData(bool bFlip, BOOL use_hybrid, BOOL try_harder);

	/// ���õ�ǰ����Ļ���
	inline void SetImage(const char* pSrc)
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
		int d1 = FastAbs(x - c1), d2 = FastAbs(x - c2), d3 = FastAbs(x - c3);
		return d1 < d2 ? (d1 < d3 ? 0 : 2) : (d2 < d3 ? 1 : 2);
	}
};