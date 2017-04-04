/**
* @file 3DCode.h 
* @brief 3DCode Ӧ�ó������ͷ�ļ�
* @author Ԭ����
*/

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������

class CyImage;

class CDlgQREncode;

class C3DCodeDoc;

class C3DCodeView;

/** @class C3DCodeApp
* @brief �йش����ʵ�֣��� @see 3DCode.cpp
*/
class C3DCodeApp : public CWinAppEx
{
public:
	C3DCodeApp();

	// �ĵ�ģ��
	CMultiDocTemplate* m_pDocTemplate;

// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	// ��ȡ�ĵ�ģ��
	CMultiDocTemplate* GetMultiDocTemplate() { return m_pDocTemplate; }

	// ���ĵ�
	C3DCodeDoc* OpenDocument(LPCTSTR lpszPathName, BOOL bMakeVisible = TRUE);

// ʵ��
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	// ��Ϣ��Ӧ����
	DECLARE_MESSAGE_MAP()
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
	afx_msg void OnAppAbout();
	virtual CDocument* OpenDocumentFile(LPCTSTR lpszFileName);
};

extern C3DCodeApp theApp;

// ��ȡ��ĵ���ͼ��ָ��
extern CyImage* AfxGetImage();

// ���µ��ĵ�
extern C3DCodeDoc* AfxOpenDocument(LPCTSTR lpszPathName, BOOL bMakeVisible = TRUE);