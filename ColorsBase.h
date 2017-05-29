/** 
* @file ColorsBase.h
* @brief ��ɫ��ά���ɫ����������
* @date 2016/10/15
*/

#pragma once

#include "DataTypes.h"
#include <vector>
using namespace std;

#ifndef MAX_MODULESIZE
#define MAX_MODULESIZE 177 /**< QR�����ߴ� */
#endif

/** 
* @enum EncodeModule
* @brief ��ɫ����ģ��
* @details 1 - �ڰ�ɫģ����룻0 - �ں�ɫģ�����
*/
enum EncodeModule
{
	BlackModule = 0,		/**< ��ǰ��ɫ���루��ɫ�� */
	WhiteModule = 1,		/**< �ڱ���ɫ���루��ɫ�� */
};

/// �������
const static int HEADER_MASK[90] = {
	// 16 x 3 ǰ48λΪ Color Infomation
	0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 
	0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 
	0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1,
	// 14 x 3 ��42λΪ Logo & Version
	0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 
	0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1,
	0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1};

/// �����°�
#define NEW_VERSION 1

/** 
* @class ColorsBase 
* @brief ��ɫ��ά�����������
*/
class ColorsBase
{
protected:
	BYTE*				m_pBitMatrix[MAX_MODULESIZE];	/**< ԭ��ά�� */
	int					m_nSymbolSize;					/**< ��ά��ߴ� */
	vector<CPixelPoint> m_HeaderIndex;					/**< ����ͷ���� */
	CLogoRect			m_LogoRect;						/**< ��ά��LOGO */

private:
	EncodeModule		m_EncodeModule;/**< ��ɫ����ģ�飨1��Ĭ��Ϊ��ɫ�� */
	vector<CPixelPoint> m_vColorsModuleIndex;			/**< ��ɫģ������ */

public:
	/** 
	* @brief ����һ����ɫ�������������
	* @param[in] bMatrix 									��ά������
	* @param[in] nSymbolSize								��ά��ߴ�
	*/
	ColorsBase(qrMat bMatrix[MAX_MODULESIZE], int nSymbolSize)
	{
		/// ����forѭ������bMatrix��ÿ�п�����m_pBitMatrix
		/// @note ֻ�ܲ���ѭ��������ʹ��memcpy
		for (int i = 0; i < nSymbolSize; ++i)
		{
			m_pBitMatrix[i] = bMatrix[i];
		}
		/// ����Ķ���Ĭ����LOGO
		m_LogoRect = 0;
		/// Ĭ���ڰ�ɫģ�����ɫ��
		m_EncodeModule = WhiteModule;
		m_nSymbolSize = nSymbolSize;
	}

	/// Ĭ�ϵ���������
	~ColorsBase() { }

	/// ���ñ���ģ��
	inline void SetEncodeModule(EncodeModule Module)
	{
		m_EncodeModule = Module;
	}

	/// ��ȡ����ģ��
	inline EncodeModule GetEncodeModule() const 
	{
		return m_EncodeModule;
	}

	/// ���ö�ά��LOGO
	inline void SetLogoRect(CMyRect logo)
	{
		m_LogoRect = logo;
	}

	/// ��ȡ��ά��LOGO
	inline CMyRect GetLogoRect() const 
	{
		return m_LogoRect;
	}

	/// ��ȡģ������
	inline int GetModulesCount() const 
	{
		return m_nSymbolSize * m_nSymbolSize - m_LogoRect.Width() * m_LogoRect.Height();
	}

	// ��ȡ����ģ������
	int GetColorsMoudlesCount() const 
	{
		return m_vColorsModuleIndex.size();
	}

	// ��ȡ����ģ�������
	const vector<CPixelPoint>& GetColorsModuleIndex() const 
	{
		return m_vColorsModuleIndex;
	}

	//  ��ʼ������ģ�������
	void InitColorsModuleIndex(BOOL bFore = TRUE);

	// (row, col)������ͷ����
	BOOL NotHeaderIndex(int row, int col) const;

	// ��ȡ��ɫ����ͷ������
	virtual void GetDataHeaderIndex() = 0;
};