// QR_Encode.h : CQR_Encode
// Date 2006/05/17	Ver. 1.22	Psytec Inc.

/************************************************************************/
// QR������ͷ�ļ�
// �����Ҫ��ֲ��Linux����#define ANDROID_CPP
// 2016/8/24 ע
/************************************************************************/

#if !defined(AFX_QR_ENCODE_H)
#define AFX_QR_ENCODE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DataTypes.h"

/////////////////////////////////////////////////////////////////////////////
//

// �������ȼ�
#define QR_LEVEL_L	0
#define QR_LEVEL_M	1
#define QR_LEVEL_Q	2
#define QR_LEVEL_H	3

// ����ģʽ
#define QR_MODE_NUMERAL		0
#define QR_MODE_ALPHABET	1
#define QR_MODE_8BIT		2
#define QR_MODE_KANJI		3

// QR�汾����
#define QR_VRESION_S	0 // 1 ~ 9
#define QR_VRESION_M	1 // 10 ~ 26
#define QR_VRESION_L	2 // 27 ~ 40

#define MAX_ALLCODEWORD	 3706 // ���ȫ����������
#define MAX_DATACODEWORD 2956 // ���ɱ�������(40-L)
#define MAX_CODEBLOCK	  153 // ����������
#define MAX_MODULESIZE	  177 // ����ά��ߴ�

// QR �߽�
#define QR_MARGIN	4


/////////////////////////////////////////////////////////////////////////////
typedef struct tagRS_BLOCKINFO
{
	int ncRSBlock;		// ��������ű����
	int ncAllCodeWord;	// 
	int ncDataCodeWord;	// 

} RS_BLOCKINFO, *LPRS_BLOCKINFO;


/////////////////////////////////////////////////////////////////////////////
// QR�汾��Ϣ�ṹ��

typedef struct tagQR_VERSIONINFO
{
	int nVersionNo;	   // �汾��(1~40)
	int ncAllCodeWord; // 

	// ���������(0 = L, 1 = M, 2 = Q, 3 = H) 
	int ncDataCodeWord[4];	// 

	int ncAlignPoint;	// 
	int nAlignPoint[6];	// 

	RS_BLOCKINFO RS_BlockInfo1[4]; // (1)
	RS_BLOCKINFO RS_BlockInfo2[4]; // (2)

} QR_VERSIONINFO, *LPQR_VERSIONINFO;


/////////////////////////////////////////////////////////////////////////////
// CQR_Encode ͷ�ļ�

class CQR_Encode
{
public:
	CQR_Encode();
	~CQR_Encode();

public:
	int m_nLevel;		// ����ˮƽ
	int m_nVersion;		// QR�汾
	BOOL m_bAutoExtent;	// �Զ���չ
	int m_nMaskingNo;	// ����汾

public:
	int m_nSymbleSize;	// ��ά���С
	BYTE m_byModuleData[MAX_MODULESIZE][MAX_MODULESIZE]; // [x][y]
	// bit5:
	// bit4:
	// bit1:
	// bit0:
	// 20h

private:
	int m_ncDataCodeWordBit; // 
	BYTE m_byDataCodeWord[MAX_DATACODEWORD]; // 

	int m_ncDataBlock;// 
	BYTE m_byBlockMode[MAX_DATACODEWORD];// 
	int m_nBlockLength[MAX_DATACODEWORD];// 

	int m_ncAllCodeWord; // 
	BYTE m_byAllCodeWord[MAX_ALLCODEWORD]; // 
	BYTE m_byRSWork[MAX_CODEBLOCK]; // 

public:

#ifndef ANDROID_CPP
	/* �������� */
	BOOL EncodeData(int nLevel, int nVersion, BOOL bAutoExtent, int nMaskingNo, LPCSTR lpsSource, int ncSource = 0);
#endif
	
	/* �ַ�����ת��Ϊutf-8 */
	BOOL EncodeDataUtf8(int nLevel, int nVersion, BOOL bAutoExtent, int nMaskingNo, char* lpsSource, int ncLength);

private:
#ifndef ANDROID_CPP
	/* ��ȡ����汾 */
	int GetEncodeVersion(int nVersion, LPCSTR lpsSource, int ncLength);
#endif

	/* �ַ�����ת��Ϊutf-8 */
	int GetEncodeVersionUtf8(int nVersion, char* lpsSource, int ncLength);

#ifndef ANDROID_CPP
	/* �������� */
	BOOL EncodeSourceData(LPCSTR lpsSource, int ncLength, int nVerGroup);
#endif

	/* �ַ�����ת��Ϊutf-8 */
	BOOL EncodeSourceDataUtf8(char *lpsSource, int ncLength, int nVerGroup);

	int GetBitLength(BYTE nMode, int ncData, int nVerGroup);

	int SetBitStream(int nIndex, WORD wData, int ncData);

	BOOL IsNumeralData(unsigned char c);
	BOOL IsAlphabetData(unsigned char c);
	BOOL IsKanjiData(unsigned char c1, unsigned char c2);

	BYTE AlphabetToBinaly(unsigned char c);
	WORD KanjiToBinaly(WORD wc);

	void GetRSCodeWord(LPBYTE lpbyRSWork, int ncDataCodeWord, int ncRSCodeWord);

private:
	void FormatModule();

	void SetFunctionModule();
	void SetFinderPattern(int x, int y);
	void SetAlignmentPattern(int x, int y);
	void SetVersionPattern();
	void SetCodeWordPattern();
	void SetMaskingPattern(int nPatternNo);
	void SetFormatInfoPattern(int nPatternNo);
	int CountPenalty();
};

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_QR_ENCODE_H)
