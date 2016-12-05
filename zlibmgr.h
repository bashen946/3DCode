#ifndef _ZLIBMGR
#define _ZLIBMGR

// 10KB
#define  MAXBUFFERSIZE 10240

#include "zlib/zlib.h"

class CZlibMgr
{
public:
	CZlibMgr();
	~CZlibMgr();

	// ѹ�� - pcContentBuf Ҫѹ��������;pcCompBuf ѹ���������;ulCompLen ѹ����ĳ���
	bool Compress(const char* pcContentBuf, char* pcCompBuf, unsigned long& ulCompLen);

	// ��ѹ - pcCompBuf ѹ��������;pcUnCompBuf ��ѹ�������;ulCompLen ѹ�����ݵĳ���
	uLong UnCompress(const char* pcCompBuf, char* pcUnCompBuf, unsigned long ulCompLen);

private:
	Byte m_zlibBuffer[MAXBUFFERSIZE];
};


#endif