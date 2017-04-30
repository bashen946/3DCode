#pragma once

/** 
* @file FileIO.h
* @brief �ļ���д���������Կ�ƽ̨
*/

#include "DataTypes.h"

// ��ͼ�������Զ�������ʽд���ļ���nChannel��ʾһ������ռ�ü����ֽ�
BOOL WriteTxt(const char* fileName, const BYTE* pSrc, int nWidth, int nHeight, int nChannel);

// ��ͼ�������Զ�������ʽ�����ļ���nChannel��ʾһ������ռ�ü����ֽ�
BYTE* ReadTxt(const char* fileName, int &nWidth, int &nHeight, int &nRowlen, int &nChannel);