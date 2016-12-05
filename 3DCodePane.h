#pragma once


// C3DCodePane
// ����һ�������Ի���ؼ���CDockablePane���Ի��������й�����ͼ
// �޸Ĵ��ļ�����Ӱ��3dcode��InvisibleCode������Ŀ
// 2015��12��11���ش�˵��
// 

class CViewParent;
class CPaneScrollView;

// ������ͼ��ID
#define ID_ENCODE_VIEW 9999

class C3DCodePane : public CDockablePane
{
	DECLARE_DYNAMIC(C3DCodePane)
public:
	CViewParent*		m_pViewParent;
	CPaneScrollView*	m_pEncodeView;

public:
	C3DCodePane();
	virtual ~C3DCodePane();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


/************************************************************************/
/*                          CViewParent ���                            */
/************************************************************************/


class CViewParent : public CFrameWnd
{
	DECLARE_DYNCREATE(CViewParent)
public:
	CViewParent();
	virtual ~CViewParent();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


/************************************************************************/
/*                     CPaneScrollView ��ͼ                             */
/************************************************************************/


//class CDlgQREncode;

class CPaneScrollView : public CScrollView
{
	DECLARE_DYNCREATE(CPaneScrollView)

public:
	CSize m_szChildSize;
	CWnd* m_pChildWnd;
	void AddChildWnd(CWnd* pChild, int nWidth = 100, int nHeight = 100);

	CPaneScrollView();
	virtual ~CPaneScrollView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void OnDraw(CDC* pDC);      // ��д�Ի��Ƹ���ͼ
	virtual void OnInitialUpdate();     // �����ĵ�һ��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};