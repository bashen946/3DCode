// QREncodePane.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "3DCodePane.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// C3DCodePane

IMPLEMENT_DYNAMIC(C3DCodePane, CDockablePane)

/// ���죺���Ա����Ϊ��
C3DCodePane::C3DCodePane()
{
	m_pViewParent = NULL;
	m_pEncodeView = NULL;
}

/// Ĭ�ϵ���������
C3DCodePane::~C3DCodePane()
{
	// ����delete����ָ�����
}


BEGIN_MESSAGE_MAP(C3DCodePane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// C3DCodePane ��Ϣ�������

/** 
* @brief ����ʱ���ô˺���
* @details ������ͼ�ĸ���ܣ�����������ͼ
*/
int C3DCodePane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_pViewParent = new CViewParent();
	m_pViewParent->Create(NULL, NULL, WS_VISIBLE | WS_CHILD, CRect(), this);
	CRuntimeClass *pViewClass = RUNTIME_CLASS(CPaneScrollView);
	m_pEncodeView = (CPaneScrollView*)pViewClass->CreateObject();
	m_pEncodeView->Create(NULL, NULL, WS_VISIBLE | WS_CHILD, CRect(), m_pViewParent, ID_ENCODE_VIEW, NULL);

	return 0;
}

/** 
* @brief ���ڴ�С�ı�ʱ����Ӧ����
* @details ������ͼ����ͼ����ܽ����ű�
*/
void C3DCodePane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	if (m_pViewParent->GetSafeHwnd())
	{
		m_pViewParent->SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER);
	}
	if (m_pEncodeView->GetSafeHwnd())
	{
		m_pEncodeView->SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER);
	}
}

/// �����ػ�ʱ����Ӧ����
BOOL C3DCodePane::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}


/************************************************************************/
/*                            CViewParent                               */
/************************************************************************/


IMPLEMENT_DYNCREATE(CViewParent, CFrameWnd)

/// Ĭ�Ϲ��캯��
CViewParent::CViewParent()
{

}

/// Ĭ����������
CViewParent::~CViewParent()
{
	TRACE(" * Pane�ĸ���ܱ�������\n");
}


BEGIN_MESSAGE_MAP(CViewParent, CFrameWnd)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CViewParent ��Ϣ�������

/// �����ػ�ʱ����Ӧ����
BOOL CViewParent::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}


/************************************************************************/
/*                         CPaneScrollView                              */
/************************************************************************/


IMPLEMENT_DYNCREATE(CPaneScrollView, CScrollView)

/// ��������ͼ�Ĺ��캯��
CPaneScrollView::CPaneScrollView()
{
	m_pChildWnd = NULL;
	m_szChildSize = CSize(100, 100);
}

/// Ĭ����������
CPaneScrollView::~CPaneScrollView()
{
	TRACE(" * Pane������ͼ��������\n");
}

BEGIN_MESSAGE_MAP(CPaneScrollView, CScrollView)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CPaneScrollView ��ͼ

/// ��ͼ��ʼ��ʱ���õĺ���
void CPaneScrollView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	SetScrollSizes(MM_TEXT, m_szChildSize);
}

/// ��ͼ���ƺ���
void CPaneScrollView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
}

// CPaneScrollView ���

#ifdef _DEBUG
void CPaneScrollView::AssertValid() const
{
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void CPaneScrollView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif
#endif //_DEBUG

/**
* @brief Ϊ��������ͼ���һ���Ӵ���
* @param[in] * pChild �Ӵ���ָ��
* @param[in] nWidth ���ڿ��
* @param[in] nHeight ���ڸ߶�
*/
void CPaneScrollView::AddChildWnd(CWnd* pChild, int nWidth, int nHeight)
{
	if(pChild && pChild->GetSafeHwnd())
	{
		m_pChildWnd = pChild;
		m_pChildWnd->SetParent(this);
	}

	m_szChildSize = CSize(nWidth, nHeight);
	SetScrollSizes(MM_TEXT, m_szChildSize);
}

/// �����ػ�ʱ����Ӧ����
BOOL CPaneScrollView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}