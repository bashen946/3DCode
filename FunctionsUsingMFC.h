#ifdef _AFX

#pragma once

/** 
* @file FunctionsUsingMFC.h
* @brief �õ���MFC�ĺ�����ֻ����WIN32�������
* @date 2016/8/24 �������ĵ�
*/

// ���ô�����ʾ����
void SetWindowInt(CWnd* pWnd, int nInt);

// ���ô�����ʾ������
void SetWindowFloat(CWnd* pWnd, float val);

// ȡ�ô�������ʾ������
int GetWindowInt(CWnd* pWnd);

// ȡ�ô�������ʾ�ĸ�����
double GetWindowFloat(CWnd* pWnd);

#endif // _AFX