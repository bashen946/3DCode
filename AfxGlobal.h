#ifdef _AFX

#pragma once

/** 
* @file AfxGlobal.h
* @brief ȫ�ֺ���
* @details ����������������
*		AfxGetActiveDoc ��ȡ�������ĵ�
*		AfxGetActiveView ��ȡ������ͼ
*/

// ��ȡ��ĵ���ָ��
extern CDocument* AfxGetActiveDoc();

// ��ȡ���ͼ��ָ��
extern CView* AfxGetActiveView();

#endif