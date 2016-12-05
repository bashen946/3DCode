
// 3DCodeDoc.cpp : C3DCodeDoc ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "3DCode.h"
#endif

#include "3DCodeDoc.h"
#include "3DCodeView.h"

#include <propkey.h>
#include <fstream>
#include "FileIO.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// C3DCodeDoc

IMPLEMENT_DYNCREATE(C3DCodeDoc, CDocument)

BEGIN_MESSAGE_MAP(C3DCodeDoc, CDocument)
END_MESSAGE_MAP()


// C3DCodeDoc ����/����

C3DCodeDoc::C3DCodeDoc()
{
	m_pImage = new CyImage;
	m_bImageLocked = FALSE;
}

C3DCodeDoc::~C3DCodeDoc()
{
	SAFE_DELETE(m_pImage);
}

BOOL C3DCodeDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO:  �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}

// C3DCodeDoc ���л�

void C3DCodeDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO:  �ڴ���Ӵ洢����
	}
	else
	{
		// TODO:  �ڴ���Ӽ��ش���
	}
}

#ifdef SHARED_HANDLERS

// ����ͼ��֧��
void C3DCodeDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// �޸Ĵ˴����Ի����ĵ�����
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// ������������֧��
void C3DCodeDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ���ĵ����������������ݡ�
	// ���ݲ���Ӧ�ɡ�;���ָ�

	// ����:     strSearchContent = _T("point;rectangle;circle;ole object;")��
	SetSearchContent(strSearchContent);
}

void C3DCodeDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// C3DCodeDoc ���

#ifdef _DEBUG
void C3DCodeDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void C3DCodeDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// C3DCodeDoc ����


BOOL C3DCodeDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	m_bImageLocked = TRUE;

	return SUCCEEDED(m_pImage->Load(lpszPathName));
}

BOOL C3DCodeDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	return 	SUCCEEDED(m_pImage->Save(lpszPathName));
}


CyImage* C3DCodeDoc::GetImage()
{
	return m_pImage;
}


// ��ȡ����Ȥ����
RoiRect& C3DCodeDoc::GetROI()
{
	return m_roi;
}


BOOL C3DCodeDoc::ImageIsNull()
{
	return m_pImage->IsNull();
}

// ��Ϊ�������������ͼ�񣬹ʽ��ⲿͼ������
BOOL C3DCodeDoc::ImageIsLocked()
{
	return m_bImageLocked;
}

// ����CSize = (nWidth, nHeight).
const CSize C3DCodeDoc::GetImageSize()
{
	if (m_pImage->IsNull())
		return CSize(0, 0);
	return CSize(m_pImage->GetWidth(), m_pImage->GetHeight());
}

// ��ȡ�ĵ���Ӧ��C3DCodeView���ͼ
C3DCodeView* C3DCodeDoc::GetView()
{
	CView* pView = NULL;
	POSITION pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		pView = GetNextView(pos);
		if (pView->IsKindOf(RUNTIME_CLASS(C3DCodeView)))
			return (C3DCodeView*)pView;
		else
			continue;
	}
	if (pView && pView->IsKindOf(RUNTIME_CLASS(C3DCodeView)))
		return (C3DCodeView*)pView;
	else
	{
		TRACE(" * ���ܶ�λ��ͼ��\n");
		return NULL;
	}
}