/** 
* @file ColorsEncode.h
* @details �����ɫ���ݣ�����ANDROID��ʹ�á�
* @author 
* @email 
* @version V1.1
* @date 2016/10/15
* Copyleft (c), All rights reserved.							
*/

#pragma once

#include "ColorsBase.h"

#define ENCODE_MOUDLE		1			/**< ����뷽������ 1-��ɫ 0-��ɫ */

#define COLOR_FOREGROUND	0			/**< ǰ��ɫ */

#define COLOR_BACKGROUND	1			/**< ����ɫ */

#define COLOR_QRCOLOR1		2			/**< ��ɫһ(ǰ��) */

#define COLOR_QRCOLOR2		17			/**< ��ɫ��(����) */

#define SUCCESS				0			/**< �ɹ� */

#define ERROR_ZCOMPRESS		1			/**< ѹ������ */

#define ERROR_UNSURPORT		2			/**< ��֧�� */

#define ERROR_LONGSTRING	3			/**< �ַ�̫�� */

/**

	00		01		10		11
	00		00		00		00

	00		01		10		11
	10		10		10		10

	00		01		10		11
	01		01		01		01

	00		01		10		11
	11		11		11		11
*/

/** 
* @class ColorsEncode 
* @brief ��ɫ����ģ��
*/
class ColorsEncode : public ColorsBase
{
public:
	/** 
	* @brief ����һ����ɫ�������������
	* @param[in] bMatrix									��ά������
	* @param[in] nSymbolSize								��ά��ߴ�
	* @param[in] LogoRect									��ά��LOGO
	*/
	ColorsEncode(qrMat bMatrix[MAX_MODULESIZE], int nSymbolSize, CLogoRect LogoRect = 0)
		: ColorsBase(bMatrix, nSymbolSize)
	{
		// ����LOGO
		SetLogoRect(LogoRect);

#if (!ENCODE_MOUDLE)
		// �����ں�ɫģ�����ɫ��
		SetEncodeModule(BlackModule);
#endif // QRCOLORS_ANOTHER

		// ��ȡ����ͷ����
		GetDataHeaderIndex();

		m_strLen = 0;
		m_ecLevel = 0;
		m_nMaskingNo = 0;
		m_nVersion = 0;
	}

	/// Ĭ�ϵ���������
	~ColorsEncode() { }

	// Ϊԭ��ά����ɫ
	int EncodeColors(char *pUtf8, int nStrLen, int nInnerEcLevel, int nInnerMaskNo, int nInnerVersion);

protected:
	// ��ɫ���뼰���ݸ�ʽ
	int				m_strLen;		/**< ���ݳ��� */
	int				m_ecLevel;		/**< ����ȼ� */
	int				m_nMaskingNo;	/**< ����汾 */
	int				m_nVersion;		/**< ����汾 - 
									0�汾���þɵ����룬��0�����µ�����
									30�汾SUPER_QR���������Ƿ�ǿ������
									31�汾����Zlib�����ݽ���ѹ������ */

#if (ENCODE_MOUDLE)
	// ��ȡλ��ʶ������16��ģ�������
	void GetPositionParternIndex(CMyRect rect);

	// ��ȡ�ָ�ͼ�ε�14��ģ�������
	void GetSeparateParternIndex(int nDirection);
#endif

	// ��ȡ��ɫ����ͷ������
	virtual void GetDataHeaderIndex();

private:
	// ��������ͷ
	int EncodeHeader();
};