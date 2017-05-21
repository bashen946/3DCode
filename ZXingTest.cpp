/************************************************************************/
// �ú�����ZXing QR����ģ����в���
// ���뿽������ĿTestDecode��������
// 2016/9/1 �������ĵ�
// 2017/4/28 �޸�
/************************************************************************/
#define _CRT_SECURE_NO_WARNINGS

#include "stdio.h"
#include <fstream>
#include <Windows.h>
#include "DecodeFuncs.h"
#include "FileIO.h"
#include <time.h>

/************************************************************************/
// ���ǽ���Ĳ��Գ��� - ��һ����ɫ��ά��ͼ����н���
// �ö�ά��ͼ���Զ�������ʽ����(��WriteBinaryFile��������)

// ��utf-8����ת����Unicode����
wchar_t* UTF8Convert2UnicodeEx(const char* strUtf8, int &ncLength)
{
	ncLength = MultiByteToWideChar(CP_UTF8, 0, strUtf8, -1, NULL, 0);
	if (ncLength == 0)
		return NULL;
	wchar_t *strUnicode = new wchar_t[ncLength];
	ncLength = MultiByteToWideChar(CP_UTF8, 0, strUtf8, -1, strUnicode, ncLength);
	return strUnicode;
}

// ���ļ�test���н������
int DecodeTest(const char *test = "./ZXingTest.txt");

int main()
{
	// ���ж�ν�����������Һ�ʱ�Ĵ���
	int start = clock();
	int nSuccessCount = 0, nColorCount = 0;
	const int TIMES = 10000;
	for (int i = 0; i < TIMES; ++i)
	{
		TRACE(" * ��%d�ν��롣\n", i+1);
		int result = DecodeTest();
		if (result)
		{
			nSuccessCount++;
		}
		if (2 == result)
		{
			nColorCount++;
		}
		else
		{
			TRACE(" * ��%d�ν���ʧ��!\n", i+1);
		}
	}
	int stop = clock();
	double totaltime = (double)(stop - start)/CLOCKS_PER_SEC;
	nSuccessCount = max(nSuccessCount, 1);
	TRACE(" * ����ɹ�:%d(%d)��, ռ%.3f\n * ƽ����ʱ��%.3f��", nColorCount, nSuccessCount, 
		1.0 * nColorCount / nSuccessCount, totaltime / TIMES);
	return getchar();
}

/**
* @brief ��ָ���ļ����н������
* @return ����ʧ�ܷ���0���ɹ�����1��2��2��ʾ�����ɫ�ɹ�.
*/
int DecodeTest(const char *test)
{
	// ͼ����Ϣ
	int nRowlen = 0, nHeight = 0, nChannel = 0;

	// ����ά����Ϣ
	float m_fModuleSize = 0.f;
	int nLevel = 0, nVersion = 0, nMaskingNo = 0;
	int ncLength = 0, _ncLength = 0;

	// ͼ����
	int nWidth = 0;
	BYTE *pSrc = ReadTxt(test, nWidth, nHeight, nRowlen, nChannel);
	if (pSrc == NULL)
		return 0;

	BarCodeInfo qr, inner;
	ImageSrc pImage(pSrc, nWidth, nHeight, nChannel);
	BOOL success = DecodeWholeImage(DecodeSrc(pImage, TRUE, TRUE), &qr, &inner);

	SAFE_DELETE(pSrc);
	wchar_t *pOuter = UTF8Convert2UnicodeEx(qr.m_pData, ncLength);
	wchar_t *pInner = UTF8Convert2UnicodeEx(inner.m_pData, ncLength);
	SAFE_DELETE(pOuter);
	SAFE_DELETE(pInner);
	return success;
}