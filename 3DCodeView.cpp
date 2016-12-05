
// 3DCodeView.cpp : C3DCodeView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "3DCode.h"
#endif

#include "3DCodeDoc.h"
#include "3DCodeView.h"
#include "DlgMessage.h"
#include "MainFrm.h"
#include "AfxGlobal.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// C3DCodeView

IMPLEMENT_DYNCREATE(C3DCodeView, CScrollView)

BEGIN_MESSAGE_MAP(C3DCodeView, CScrollView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_VIEW_ZOOM_IN, &C3DCodeView::OnViewZoomIn)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOM_IN, &C3DCodeView::OnUpdateViewZoomIn)
	ON_COMMAND(ID_VIEW_ZOOM_OUT, &C3DCodeView::OnViewZoomOut)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOM_OUT, &C3DCodeView::OnUpdateViewZoomOut)
	ON_COMMAND(ID_VIEW_ZOOM_DEFAULT, &C3DCodeView::OnViewZoomDefault)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOM_DEFAULT, &C3DCodeView::OnUpdateViewZoomDefault)
	ON_COMMAND(ID_VIEW_FLIP_H, &C3DCodeView::OnViewFlipH)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FLIP_H, &C3DCodeView::OnUpdateViewFlipH)
	ON_COMMAND(ID_VIEW_FLIP_V, &C3DCodeView::OnViewFlipV)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FLIP_V, &C3DCodeView::OnUpdateViewFlipV)
	ON_COMMAND(ID_VIEW_TRANSPOSE, &C3DCodeView::OnViewTranspose)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TRANSPOSE, &C3DCodeView::OnUpdateViewTranspose)
	ON_COMMAND(ID_EDIT_CUT, &C3DCodeView::OnEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, &C3DCodeView::OnUpdateEditCut)
	ON_COMMAND(ID_EDIT_COPY, &C3DCodeView::OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, &C3DCodeView::OnUpdateEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, &C3DCodeView::OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, &C3DCodeView::OnUpdateEditPaste)
	ON_COMMAND(ID_EDIT_BPP, &C3DCodeView::OnEditBpp)
	ON_UPDATE_COMMAND_UI(ID_EDIT_BPP, &C3DCodeView::OnUpdateEditBpp)
	ON_COMMAND(ID_EDIT_ROTATE, &C3DCodeView::OnEditRotate)
	ON_UPDATE_COMMAND_UI(ID_EDIT_ROTATE, &C3DCodeView::OnUpdateEditRotate)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// C3DCodeView ����/����

C3DCodeView::C3DCodeView()
{
	m_PaintRect = CRect(0, 0, 100, 100);
	m_fZoomRate = 1.f;
	m_bLeftButtonDown = FALSE;
}

C3DCodeView::~C3DCodeView()
{
}

BOOL C3DCodeView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CScrollView::PreCreateWindow(cs);
}

// C3DCodeView ����

void C3DCodeView::OnDraw(CDC* pDC)
{
	C3DCodeDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (!pDoc->ImageIsNull())
	{
		pDoc->GetImage()->Draw(pDC->GetSafeHdc(), m_PaintRect, Gdiplus::InterpolationModeBilinear);
	}
	if (!m_RoiRect.IsEmpty())
	{
		pDC->MoveTo(m_RoiRect.left, m_RoiRect.top);
		pDC->LineTo(m_RoiRect.right, m_RoiRect.top);
		pDC->MoveTo(m_RoiRect.right, m_RoiRect.top);
		pDC->LineTo(m_RoiRect.right, m_RoiRect.bottom);
		pDC->MoveTo(m_RoiRect.right, m_RoiRect.bottom);
		pDC->LineTo(m_RoiRect.left, m_RoiRect.bottom);
		pDC->MoveTo(m_RoiRect.left, m_RoiRect.bottom);
		pDC->LineTo(m_RoiRect.left, m_RoiRect.top);
	}
}

void C3DCodeView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	C3DCodeDoc* pDoc = GetDocument();
	int nWidth = 100, nHeight = 100;
	if (!pDoc->ImageIsNull())
	{
		CSize sz = pDoc->GetImageSize();
		nWidth = sz.cx;
		nHeight = sz.cy;
	}
	SetPaintRect(nWidth, nHeight);
}


// C3DCodeView ��ӡ

BOOL C3DCodeView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void C3DCodeView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void C3DCodeView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  ��Ӵ�ӡ����е��������
}


// C3DCodeView ���

#ifdef _DEBUG
void C3DCodeView::AssertValid() const
{
	CScrollView::AssertValid();
}

void C3DCodeView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

C3DCodeDoc* C3DCodeView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(C3DCodeDoc)));
	return (C3DCodeDoc*)m_pDocument;
}
#endif //_DEBUG


// C3DCodeView ��Ϣ�������


BOOL C3DCodeView::OnEraseBkgnd(CDC* pDC)
{
	C3DCodeDoc* pDoc = GetDocument();
	if (!pDoc->ImageIsNull())
	{
		// 1	2	3
		// 4	5	6
		// 7	8	9
		// ����5�����ͼ����
		CRect rect;
		GetClientRect(&rect);
		CyImage *pImage = pDoc->GetImage();
		// SetPaintRect(pImage->GetWidth(), pImage->GetHeight());		//ע�͵���仰�Ŵ���С���з�Ӧ(2016.3.8)
		pDC->PatBlt(0, 0, m_PaintRect.left, rect.bottom, PATCOPY);		// 1 4 7
		pDC->PatBlt(0, 0, rect.right, m_PaintRect.top, PATCOPY);		// 1 2 3
		pDC->PatBlt(m_PaintRect.right, 0, rect.right - m_PaintRect.right, rect.bottom, PATCOPY);	// 3 6 9	
		pDC->PatBlt(0, m_PaintRect.bottom, rect.right, rect.bottom - m_PaintRect.bottom, PATCOPY);	// 7 8 9
		return TRUE;
	}
	return CScrollView::OnEraseBkgnd(pDC);
}

void C3DCodeView::SetPaintRect(const CSize sz)
{
	SetPaintRect(sz.cx, sz.cy);
}

// ���û�ͼ���ο����߶�
void C3DCodeView::SetPaintRect(int nWidth, int nHeight)
{
	m_PaintRect.right = m_PaintRect.left + nWidth;
	m_PaintRect.bottom = m_PaintRect.top + nHeight;
	SetScrollSizes(MM_TEXT, CSize(m_PaintRect.right, m_PaintRect.bottom));
	CScrollView::Invalidate(TRUE);
}

void C3DCodeView::CopyImage(CyImage* pImage)
{
	if (OpenClipboard())
	{
		if (pImage->IsNull())
		{
			CloseClipboard();
			return;
		}

		int width, height;
		pImage->GetInfomation(width, height);
		CDC *pDC = GetDC();
		CBitmap cBmp;
		cBmp.CreateCompatibleBitmap(pDC, width, height);
		CDC MemDC;
		MemDC.CreateCompatibleDC(pDC);
		CBitmap* pOldBitmap = MemDC.SelectObject(&cBmp);
		pImage->Draw(MemDC, CRect(0, 0, width, height), Gdiplus::InterpolationModeBicubic);

		EmptyClipboard();
		SetClipboardData(CF_BITMAP, cBmp.GetSafeHandle());
		CloseClipboard();
		MemDC.SelectObject(pOldBitmap);
		MemDC.DeleteDC();
		ReleaseDC(pDC);
		BITMAP bmp = { 0 };
		cBmp.GetBitmap(&bmp);
		cBmp.Detach();
	}
}

void C3DCodeView::PasteImage()
{
	if (OpenClipboard())
	{
		// ��ȡ���а���������
		int data_type = 0;
		HANDLE handle = NULL;
		for (int i = 1; i < CF_MAX; ++i)
		{
			handle = GetClipboardData(i);
			if (handle != NULL)
			{
				TRACE(" * ��ʾ: ���а������������%d.\n", i);
				data_type = i;
				break;
			}
		}

		// ��ȡ�ĵ�
		C3DCodeDoc* pDoc = GetDocument();

		switch (data_type)
		{
		case CF_BITMAP:
		{
						  HBITMAP hBitmap = (HBITMAP)handle;
						  if (hBitmap == NULL)
						  {
							  CloseClipboard();
							  return;
						  }
						  // ��ǰ�ĵ��Ѿ���ͼ��
						  if (!pDoc->ImageIsNull())
							  pDoc = AfxOpenDocument(NULL);
						  CBitmap* pBitmap = CBitmap::FromHandle(hBitmap);
						  // ȡ��Դ����
						  BITMAP bmp = { 0 };
						  pBitmap->GetBitmap(&bmp);
						  long Length = bmp.bmHeight * bmp.bmWidthBytes;
						  BYTE* pSrc = new BYTE[Length];
						  pBitmap->GetBitmapBits(Length, pSrc);
						  // ��Ŀ���ַ��������
						  pDoc->GetImage()->Create(bmp.bmWidth, bmp.bmHeight, bmp.bmBitsPixel);
						  BYTE* pDst = pDoc->GetImage()->GetHeadAddress();
						  memcpy(pDst, pSrc, Length);
						  delete[] pSrc;
						  C3DCodeView* pView = pDoc->GetView();
						  pView->SetPaintRect(bmp.bmWidth, bmp.bmHeight);
		}
			break;
		case CF_HDROP:
		{
						 HDROP hDrop = (HDROP)handle;
						 // ��ȡ�϶����ļ�����
						 const int fileCount = DragQueryFile(hDrop, (UINT)-1, NULL, 0);
						 ASSERT(fileCount >= 1);
						 for (int i = 0; i < fileCount; i++)
						 {
							 TCHAR fileName[MAX_PATH] = { 0 };
							 DragQueryFile(hDrop, i, fileName, MAX_PATH);
							 CString str;
							 str.Format(_T("%s"), fileName);
							 // �½�һ���ĵ����ٸ��Ƽ��а�ͼ���½��Ŀհ��ĵ�
							 if (!pDoc->ImageIsNull())
							 {
								 pDoc = AfxOpenDocument(str);
							 }
							 else
							 {
								 // Windows�ļ��а����ʵ�����Ǹ�����һ���ļ��б�
								 if (pDoc->OnOpenDocument(str))
								 {
									 C3DCodeView* pView = pDoc->GetView();
									 pView->SetPaintRect(pDoc->GetImageSize());
								 }
							 }
						 }
		}
			break;
		default:
			break;
		}
		CloseClipboard();
		return;
	}
}

void C3DCodeView::Invalidate(BOOL bErase)
{
	C3DCodeDoc *pDoc = GetDocument();
	CyImage *pImage = pDoc->GetImage();
	if (NULL != pImage && !pImage->IsNull())
	{
		m_PaintRect.right = m_PaintRect.left + pImage->GetWidth();
		m_PaintRect.bottom = m_PaintRect.top + pImage->GetHeight();
	}
	SetScrollSizes(MM_TEXT, CSize(m_PaintRect.right, m_PaintRect.bottom));

	CScrollView::Invalidate(bErase);
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
}

void C3DCodeView::OnViewZoomIn()
{
	m_fZoomRate *= 1.1f;
	if (m_fZoomRate >= 10.f)
		return;
	C3DCodeDoc* pDoc = GetDocument();
	CSize sz = pDoc->GetImageSize();
	SetPaintRect(int(m_fZoomRate * sz.cx), int(m_fZoomRate * sz.cy));
}


void C3DCodeView::OnUpdateViewZoomIn(CCmdUI *pCmdUI)
{
	C3DCodeDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->ImageIsNull());
	pCmdUI->SetCheck(m_fZoomRate > 1.f);
}


void C3DCodeView::OnViewZoomOut()
{
	m_fZoomRate /= 1.1f;
	if (m_fZoomRate <= 0.1f)
		return;
	C3DCodeDoc* pDoc = GetDocument();
	CSize sz = pDoc->GetImageSize();
	SetPaintRect(int(m_fZoomRate * sz.cx), int(m_fZoomRate * sz.cy));
}


void C3DCodeView::OnUpdateViewZoomOut(CCmdUI *pCmdUI)
{
	C3DCodeDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->ImageIsNull());
	pCmdUI->SetCheck(m_fZoomRate < 1.f);
}


void C3DCodeView::OnViewZoomDefault()
{
	m_fZoomRate = 1.f;
	C3DCodeDoc* pDoc = GetDocument();
	CSize sz = pDoc->GetImageSize();
	SetPaintRect(int(m_fZoomRate * sz.cx), int(m_fZoomRate * sz.cy));
}


void C3DCodeView::OnUpdateViewZoomDefault(CCmdUI *pCmdUI)
{
	C3DCodeDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->ImageIsNull());
	pCmdUI->SetCheck(m_fZoomRate == 1.f);
}


void C3DCodeView::OnViewFlipH()
{
	C3DCodeDoc* pDoc = GetDocument();
	pDoc->GetImage()->FlipH();
	CScrollView::Invalidate(TRUE);
}


void C3DCodeView::OnUpdateViewFlipH(CCmdUI *pCmdUI)
{
	C3DCodeDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->ImageIsNull());
}


void C3DCodeView::OnViewFlipV()
{
	C3DCodeDoc* pDoc = GetDocument();
	pDoc->GetImage()->FlipV();
	CScrollView::Invalidate(TRUE);
}


void C3DCodeView::OnUpdateViewFlipV(CCmdUI *pCmdUI)
{
	C3DCodeDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->ImageIsNull());
}


void C3DCodeView::OnViewTranspose()
{
	C3DCodeDoc* pDoc = GetDocument();
	pDoc->GetImage()->Transpose();
	CSize sz = pDoc->GetImageSize();
	SetPaintRect(int(m_fZoomRate * sz.cx), int(m_fZoomRate * sz.cy));
}


void C3DCodeView::OnUpdateViewTranspose(CCmdUI *pCmdUI)
{
	C3DCodeDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->ImageIsNull());
}


void C3DCodeView::OnEditCut()
{
	// TODO:  �ڴ���������������
}


void C3DCodeView::OnUpdateEditCut(CCmdUI *pCmdUI)
{
	C3DCodeDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->ImageIsNull());
}


void C3DCodeView::OnEditCopy()
{
	C3DCodeDoc* pDoc = GetDocument();
	CopyImage(pDoc->GetImage());
}


void C3DCodeView::OnUpdateEditCopy(CCmdUI *pCmdUI)
{
	C3DCodeDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->ImageIsNull());
}


void C3DCodeView::OnEditPaste()
{
	PasteImage();
}


void C3DCodeView::OnUpdateEditPaste(CCmdUI *pCmdUI)
{
	OpenClipboard();
	int num = EnumClipboardFormats(0);
	CloseClipboard();
	pCmdUI->Enable(num);
}


void C3DCodeView::OnEditBpp()
{
	CDlgMessage dlg;
	dlg.m_strInputName = _T("λ���:");
	dlg.m_fInput = 24;
	if (dlg.DoModal() == IDOK)
	{
		if (dlg.m_fInput == 8 || dlg.m_fInput == 24 || dlg.m_fInput == 32)
		{
			C3DCodeDoc* pDoc = GetDocument();
			pDoc->GetImage()->ChangeBPP(UINT(dlg.m_fInput));
			CScrollView::Invalidate(TRUE);
		}
	}
}


void C3DCodeView::OnUpdateEditBpp(CCmdUI *pCmdUI)
{
	C3DCodeDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->ImageIsNull());
}


void C3DCodeView::OnEditRotate()
{
	CDlgMessage dlg;
	dlg.m_strInputName = _T("��ת�Ƕ�:");
	dlg.m_fInput = 30;
	if (dlg.DoModal() == IDOK)
	{
		C3DCodeDoc* pDoc = GetDocument();
		pDoc->GetImage()->Rotate(dlg.m_fInput);
		SetPaintRect(pDoc->GetImageSize());
		CScrollView::Invalidate(TRUE);
	}
}


void C3DCodeView::OnUpdateEditRotate(CCmdUI *pCmdUI)
{
	C3DCodeDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->ImageIsNull());
}


void C3DCodeView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_bLeftButtonDown = TRUE;
	// ����ȡ�����豸���꣬��Ҫת��Ϊ�߼�����
	CClientDC dc(this); // ��ȡ�ͻ�����DC
	OnPrepareDC(&dc); // ����ԭ��ƥ��
	dc.DPtoLP(&point); // ����ת��
	m_RoiRect.left = point.x;
	m_RoiRect.top = point.y;
	CScrollView::OnLButtonDown(nFlags, point);
}


void C3DCodeView::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_bLeftButtonDown = FALSE;

	C3DCodeDoc *pDoc = GetDocument();
	RoiRect *roi = &pDoc->GetROI();
	*roi = m_RoiRect;
	ZoomRect(*roi, 1 / m_fZoomRate);

	CScrollView::OnLButtonUp(nFlags, point);
}


void C3DCodeView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bLeftButtonDown)
	{
		CClientDC dc(this);
		OnPrepareDC(&dc);
		dc.DPtoLP(&point);
		m_RoiRect.right = point.x;
		m_RoiRect.bottom = point.y;
		CScrollView::Invalidate(TRUE);
	}

	CScrollView::OnMouseMove(nFlags, point);
}
