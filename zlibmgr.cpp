#include "stdafx.h"
#include "zlibmgr.h"
#include <string>


CZlibMgr::CZlibMgr()
{
}


CZlibMgr::~CZlibMgr()
{
}


/* - ѹ���ַ��� - 
const char* pcContentBuf����ѹ���ַ���
char* pcCompBuf��ѹ�����ַ���
unsigned long& ulCompLen��ѹ�����ַ�������
*/
bool CZlibMgr::Compress(const char* pcContentBuf, char* pcCompBuf, unsigned long& ulCompLen)
{
	if (NULL == pcContentBuf)
	{
		return false;
	}

	uLong sourceLen = strlen(pcContentBuf);
	if (0 == sourceLen)
	{
		return false;
	}

	// ��ʼ��zlib��������
	memset(m_zlibBuffer, 0, MAXBUFFERSIZE);
	uLong comprLen = sizeof(m_zlibBuffer) / sizeof(m_zlibBuffer[0]);

	int err = compress(m_zlibBuffer, &comprLen, (const Bytef*)pcContentBuf, sourceLen);
	if (err != Z_OK)
	{
		TRACE(" * ѹ������: %d\n", err);
		return false;
	}

	TRACE(" * ԭ�ߴ�:%ld, ѹ����:%ld, ѹ����:%.3f\n", sourceLen, comprLen, 1.0 * comprLen / sourceLen);
	memcpy(pcCompBuf, m_zlibBuffer, comprLen);
	ulCompLen = comprLen;

	return true;
}


/* - ��ѹ���ַ��� - 
const char* pcCompBuf��ѹ�����ַ���
char* pcUnCompBuf����ѹ���ַ���
unsigned long ulCompLen��ѹ�����ַ�������
����ֵ�����ؽ�ѹ���ַ�������
*/
uLong CZlibMgr::UnCompress(const char* pcCompBuf, char* pcUnCompBuf, unsigned long ulCompLen)
{
	if (pcCompBuf == NULL || strlen(pcCompBuf) == 0)
	{
		return 0;
	}

	// ��ʼ��zlib��������
	memset(m_zlibBuffer, 0, MAXBUFFERSIZE);
	uLong sourceLen = MAXBUFFERSIZE;

	int err = uncompress(m_zlibBuffer, &sourceLen, (const Bytef *)pcCompBuf, ulCompLen);
	if (err != Z_OK)
	{
		TRACE(" * ��ѹ����: %d\n", err);
		return 0;
	}

	TRACE(" * ԭ�ߴ�:%ld, ѹ����:%ld, ѹ����:%.3f\n", sourceLen, ulCompLen, 1.0 * ulCompLen / sourceLen);
	memcpy(pcUnCompBuf, m_zlibBuffer, sourceLen + 1);
	pcUnCompBuf[sourceLen] = 0;

	return sourceLen;
}