#pragma once

/************************************************************************/
// �ļ���д���������Կ�ƽ̨
/************************************************************************/

// ��ͼ�������Զ�������ʽд���ļ���nChannel��ʾһ������ռ�ü����ֽ�
BOOL WriteTxt(const char* fileName, BYTE* pSrc, int nRowlen, int nHeight, int nChannel);

// ��ͼ�������Զ�������ʽ�����ļ���nChannel��ʾһ������ռ�ü����ֽ�
BYTE* ReadTxt(const char* fileName, int &nWidth, int &nHeight, int &nRowlen, int &nChannel);