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

/// ������������ֵ����
typedef float3 pixel;

/** 
* @class ColorsDecode 
* @brief ��ɫ����ģ��
* @details ��������8����Ա��������ͨ�����캯����Init������ʼ����
*/
class ColorsDecode : public ColorsEncode
{
private:
	// ͼ����Ϣ
	const BYTE*		m_pHead;		/**< ͼ��ͷָ�� */
	int				m_nWidth;		/**< ͼ���� */
	int				m_nHeight;		/**< ͼ��߶� */
	int				m_nRowlen;		/**< ͼ��ÿ���ֽ��� */
	int				m_nChannel;		/**< ͼ��ÿ�����ֽ� */
	int				m_nModuleSize;	/**< ��ά��ģ���С */
	static char		m_pData[MAX_CODEUTF8NUM];/**< ������ */
	// ������ɫ
	pixel			m_Foreground;	/**< ��ɫǰ�� */
	pixel			m_Background;	/**< ��ɫ���� */
	// Color Infomation + Logo & Version
	int				m_nHeaderBits[90];/**< ��ɫ�����ͷ */
	// �Ƿ���Ҫ��ת������
	bool			m_bInversed;	/**< ��ɫǰ�������㷴�� */
	
public:
	/** 
	* @brief ����һ����ɫ������
	* @param[in] bMatrix 									��ά������
	* @param[in] nSymbolSize								��ά��ߴ�
	* @param[in] nModuleSize								ģ���С
	* @note ��������ʼ�������7������
	*/
	ColorsDecode(qrMat bMatrix[MAX_MODULESIZE], int nSymbolSize, int nModuleSize)
		: ColorsEncode(bMatrix, nSymbolSize, 0)
	{
		m_nModuleSize = nModuleSize;
		m_Foreground = 0;
		m_Background = 0;
		m_bInversed = false;
	}

	~ColorsDecode() { }

	// RS4���������ɫ
	const char* DecodeColors(const BYTE* pHead, int nWidth, int nHeight, int nChannel);

	/** 
	* @brief ��ȡ��ɫ����ͷ��Ϣ
	* @param[out] &strLen			���ݳ���
	* @param[out] &nInnerecLevel	����ȼ�
	* @param[out] &nInnerMask		����汾
	* @note ����֮����ô˺�����ȡ������
	*/
	inline void GetHeaderInfo(int &strLen, int &nInnerecLevel, int &nInnerMask) const 
	{
		strLen = m_strLen;
		nInnerecLevel = m_ecLevel;
		nInnerMask = m_nMaskingNo;
	}

private:
	// ��ȡ����ͷ
	void GetHeaderBits(int nMaxepoches = 10);
	// ��������ͷ
	BOOL DecodeHeader(bool bInverse = false);
	// K��ֵ��ֵ��
	void K_means(const pixel *pArray, int *Cluster, int nNum, int nMaxepoches);
};