
/** 
* @file QrDecoder.h
* @brief Qr����뺯�������Կ�ƽ̨
*/


#pragma once

#include "ZXingDecoder.h"
using namespace zxing::qrcode;


/**
* @class QrDecoder
* @brief QR�������
*/
class QrDecoder : public ZXingDecoder<QRCodeReader>
{
private:
	// QR data
	CQR_Encode* m_qr;

public:
	/**
	* @brief ����һ��QR�������
	* @param[in] bUseHybrid �������1
	* @param[in] bTryHarder �������2
	*/
	QrDecoder(BOOL bUseHybrid, BOOL bTryHarder) : 
		ZXingDecoder(DecodeHints::QR_CODE_HINT, bUseHybrid, bTryHarder)
	{
		memset(this, 0, sizeof(QrDecoder));
	}

	~QrDecoder()
	{
		if (m_qr) delete m_qr;
	}

	/// ͸����ת�任
	int perspectTransform(BYTE **pDst, int nMoudleSize) const;

	/// ���QR����ؾ���
	inline qrMat* InverseQr()
	{
		if (!m_qr)
		{
			m_qr = new CQR_Encode;
			Decoder QRDecoder = m_READER->getDecoder();
			m_qr->EncodeDataUtf8(QRDecoder.GetErrorCorrectLevel(), QRDecoder.GetQRCodeVersion(), 
				TRUE, QRDecoder.GetMaskingNo(), m_pData, m_nStrlen);
		}
		return m_qr->m_byModuleData;
	}

	/// ���QR��汾
	inline int GetQrVersion() const { return m_READER->getDecoder().GetQRCodeVersion(); }

	/// ���QR���С
	inline int GetQrSize() const { return 17 + 4 * GetQrVersion(); }

	/// ��ȡ������Ϣ
	inline void GetQrInfo(float &fModuleSize, int &nLevel, int &nVersion, int &nMaskingNo) const 
	{
		Decoder QRDecoder = m_READER->getDecoder();
		fModuleSize = m_READER->m_fModuleSize;
		nLevel = QRDecoder.GetErrorCorrectLevel();
		nVersion = QRDecoder.GetQRCodeVersion();
		nMaskingNo = QRDecoder.GetMaskingNo();
	}
};