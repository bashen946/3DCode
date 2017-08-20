/** 
* @file Mask.h
* @brief ��ɫ��ά��ɰ�����
*/

#pragma once

class COldMask
{
public:
	/**
	* @brief ����һ���ɵ�����
	* @param[in] nMin �������Сֵ
	* @param[in] nMax ��������ֵ
	* @param[in] nSeed ���������
	* @note �ɵ������ǲ����������������ģ��
	*/
	COldMask(int nMin, int nMax, unsigned int nSeed) 
		: m_nMin(nMin), m_nMax(nMax), m_nSeed(nSeed), m_pMaskData(0), m_nMaskNum(0), m_nIndex(0) { }

	~COldMask() { SAFE_DELETE(m_pMaskData); }

	// �������nNums����ͬ������������nMin��nMax
	void GenerateInterferenceData(int nNums, int nMaskingNo = 0, int nVersion = 0);

	/// �Ƿ��Ǹ�����Ϣ
	inline bool IsMasked(int &k)
	{
		if (k == m_pMaskData[m_nIndex] && m_nIndex < m_nMaskNum)
		{
			k++;
			m_nIndex++;
			return true;
		}
		return false;
	}

private:
	/** �������Сֵ */
	int m_nMin;
	/** ��������ֵ */
	int m_nMax;
	/** ��������� */
	unsigned int m_nSeed;
	/** �������������Ĭ������ */
	static unsigned int ___next;

	/// ��������
	int *m_pMaskData;

	/// �������
	int m_nMaskNum;

	/// �±�
	int m_nIndex;

	/**
	* @brief rand��windows��android�²�����������в�һ���Ľ���취
	* @see http://blog.csdn.net/zhch152/article/details/47862853
	* @date 2015-08-22 20:08
	*/
	inline int my_rand()
	{
		___next = ___next * 0x343fd + 0x269EC3;
		return (___next >> 0x10) & 0x7FFF;
	}


	/** 
	* @brief ���������������������
	*/
	inline void my_srand(unsigned int ___seed)
	{
		___next = ___seed;
	}
};