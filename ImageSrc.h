#pragma once
#include "DataTypes.h"

/** 
* @class ImageSrc
* @brief ͼ������Դ
* @details �����ݽṹ�����ͼ�����Ҫ��Ϣ, ͼ���ע������Ϣ.

* @author yuanyuanxiang
* @version 1.0
* @date 2017/5/14
*/
class ImageSrc
{
public:
	/**
	* @brief ����һ��ͼ��ṹ
	* @param[in] *pHead				ͼ������
	* @param[in] nWidth				ͼ����
	* @param[in] nHeight			ͼ��߶�
	* @param[in] nChannel			ͼ��ͨ��
	* @param[in] roi				����Ȥ����
	*/
	ImageSrc(const BYTE *pHead, int nWidth, int nHeight, int nChannel, 
		const RoiRect &roi = 0) : 
		m_pSrc(pHead), 
		m_nWidth(nWidth), 
		m_nHeight(nHeight), 
		m_nRowlen(WIDTHBYTES(8 * nChannel * nWidth)),
		m_nChannel(nChannel),
		m_roi(roi),
		m_Background(0) { }

	~ImageSrc(void) { }

	/// �ļ�ͷ��ʶ
	static const char *m_sTag;
	/// ͼ������Դ
	const BYTE *m_pSrc;
	/// ͼ����
	int m_nWidth;
	/// ͼ��߶�
	int m_nHeight;
	/// ͼ��ÿ��Ԫ�ظ���
	int m_nRowlen;
	/// ͼ��ÿ����Ԫ�ظ���
	int m_nChannel;
	/// ����Ȥ����
	RoiRect m_roi;
	/// ������ɫ
	COLORREF m_Background;

	/// ��ʼ��
	static void Init(const char *tag) { m_sTag = tag; }
};


/**
* @class DecodeSrc
* @brief ��ά����������Դ�ṹ

* @author yuanyuanxiang
* @version 1.0
* @date 2017/5/21
*/
class DecodeSrc
{
public:
	/// ͼ������Դ
	ImageSrc m_imgSrc;
	/// QR��������1(�����ֵ��)
	BOOL m_bUseHybrid;
	/// QR��������2(ǿ������)
	BOOL m_bTryHarder;

	/** 
	* @brief ���캯��
	* @param[in] src		ͼ������Դ
	* @param[in] bUseHybrid	QR��������1
	* @param[in] bTryHarder	QR��������2
	*/
	DecodeSrc(const ImageSrc & src, BOOL bUseHybrid, BOOL bTryHarder) :
		m_imgSrc(src), m_bUseHybrid(bUseHybrid), m_bTryHarder(bTryHarder)
	{
	}
};