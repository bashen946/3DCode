/**
* @file MainFrm.h
* @brief CMainFrame ��Ľӿ�
*/

#pragma once
#include "3DCodePane.h"
#include "DlgQREncode.h"
#include "DlgQRDecode.h"

/**
* @class CMainFrame
* @brief �����
*/
class CMainFrame : public CMDIFrameWndEx
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW 
		| FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // �ؼ���Ƕ���Ա
	CMFCMenuBar			m_wndMenuBar;		/**< �˵��� */
	CMFCToolBar			m_wndToolBar;		/**< ������ */
	CMFCStatusBar		m_wndStatusBar;		/**< ״̬�� */
	CMFCToolBarImages	m_UserImages;		/**< ״̬��ͼ�� */
	C3DCodePane			m_paneQREncode;		/**< ������� */
	C3DCodePane			m_paneQRDecode;		/**< ������� */

public:
	CDlgQREncode*		m_pQREncodeDlg;		/**< ����Ի��� */
	CDlgQRDecode*		m_pQRDecodeDlg;		/**< ����Ի��� */

	/// ��ñ���Ի���ָ��
	CDlgQREncode* GetEncodeDlg() { return m_pQREncodeDlg; }
	/// ��ý���Ի���ָ��
	CDlgQRDecode* GetDecodeDlg() { return m_pQRDecodeDlg; }
	/// ��ȡ״̬��
	CMFCStatusBar& GetStatusBar() { return m_wndStatusBar; }

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowManager();
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	DECLARE_MESSAGE_MAP()

	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);
public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
};