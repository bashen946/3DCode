/** 
* @file ColorsDecode.h
* @details �����ɫ���ݣ�����ANDROID��ʹ�á�
* @author 
* @email 
* @version V1.0
* @date 2016/10/16
* Copyleft (c), All rights reserved.
*/

#pragma once
#include "ColorsEncode.h"

/** 
* @class ColorsDecode 
* @brief ��ɫ����ģ��
* @details ��������8����Ա��������ͨ�����캯����Init������ʼ����
*/
class ColorsDecode : public ColorsEncode
{
private:
	// ͼ����Ϣ
	BYTE*			m_pHead;		/**< ͼ��ͷָ�� */
	int				m_nWidth;		/**< ͼ���� */
	int				m_nHeight;		/**< ͼ��߶� */
	int				m_nRowlen;		/**< ͼ��ÿ���ֽ��� */
	int				m_nChannel;		/**< ͼ��ÿ�����ֽ� */
	int				m_nModuleSize;	/**< ��ά��ģ���С */
	// ������ɫ
	float3			m_Foreground;	/**< ��ɫǰ�� */
	float3			m_Background;	/**< ��ɫ���� */
	
public:
	/** 
	* @brief ����һ����ɫ������
	* @param[in] bMatrix 									��ά������
	* @param[in] nSymbolSize								��ά��ߴ�
	* @param[in] nModuleSize								ģ���С
	* @note ��������ʼ�������7������
	*/
	ColorsDecode(BYTE bMatrix[MAX_MODULESIZE][MAX_MODULESIZE], int nSymbolSize, int nModuleSize)
		: ColorsEncode(bMatrix, nSymbolSize, 0)
	{
		m_nModuleSize = nModuleSize;
		m_Foreground = 0;
		m_Background = 0;
	}

	~ColorsDecode() { }

	/** 
	* @brief ��ʼ����ɫ������
	* @param[in] * pHead	��ά��ͼ��ָ��
	* @param[in] nWidth		ͼ����
	* @param[in] nHeight	ͼ��߶�
	* @param[in] nChannel	ͼ��ͨ����
	* @note ��������ʼ�������5������
	*/
	void Init(BYTE* pHead, int nWidth, int nHeight, int nChannel)
	{
		m_pHead = pHead;
		m_nWidth = nWidth;
		m_nHeight = nHeight;
		m_nChannel = nChannel;
		m_nRowlen = WIDTHBYTES(nChannel * 8 * nWidth);
	}

	// RS4���������ɫ
	char* DecodeColors(COLORREF QREncodeColor1, COLORREF QREncodeColor2);

	/** 
	* @brief ��ȡ��ɫ����ͷ��Ϣ
	* @param[out] &strLen			���ݳ���
	* @param[out] &nInnerecLevel	����ȼ�
	* @param[out] &nInnerMask		����汾
	* @note ����֮����ô˺�����ȡ������
	*/
	void GetHeaderInfo(int &strLen, int &nInnerecLevel, int &nInnerMask) const 
	{
		strLen = m_strLen;
		nInnerecLevel = m_ecLevel;
		nInnerMask = m_nMaskingNo;
	}

private:
	// ��������ͷ
	BOOL DecodeHeader();
};