/** 
* @file 3DCodeView.h
* @brief C3DCodeView ��Ľӿ�
*/

#pragma once

/**
* @class C3DCodeView
* @brief ��ͼ
*/
class C3DCodeView : public CScrollView
{
protected: // �������л�����
	C3DCodeView();
	DECLARE_DYNCREATE(C3DCodeView)

// ����
public:
	C3DCodeDoc* GetDocument() const;

// ����
private:
	CRect							m_PaintRect;		/**< ��ͼ���� */
	RoiRect							m_RoiRect;			/**< ����Ȥ����*/
	float							m_fZoomRate;		/**< �Ŵ���� */
	BOOL							m_bLeftButtonDown;	/**< ������� */

public:
	void SetPaintRect(const CSize sz);					// ���û�ͼ���δ�С
	void SetPaintRect(int nWidth, int nHeight);			// ���û�ͼ���ο��
	void CopyImage(CyImage* pImage);					// ����ͼ���ڴ�
	void PasteImage();									// ���ͼ���ĵ�
	void Invalidate(BOOL bErase = TRUE);				// �ػ�ͼ��

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // ������һ�ε���
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~C3DCodeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnViewZoomIn();
	afx_msg void OnUpdateViewZoomIn(CCmdUI *pCmdUI);
	afx_msg void OnViewZoomOut();
	afx_msg void OnUpdateViewZoomOut(CCmdUI *pCmdUI);
	afx_msg void OnViewZoomDefault();
	afx_msg void OnUpdateViewZoomDefault(CCmdUI *pCmdUI);
	afx_msg void OnViewFlipH();
	afx_msg void OnUpdateViewFlipH(CCmdUI *pCmdUI);
	afx_msg void OnViewFlipV();
	afx_msg void OnUpdateViewFlipV(CCmdUI *pCmdUI);
	afx_msg void OnViewTranspose();
	afx_msg void OnUpdateViewTranspose(CCmdUI *pCmdUI);
	afx_msg void OnEditCut();
	afx_msg void OnUpdateEditCut(CCmdUI *pCmdUI);
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI *pCmdUI);
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditPaste(CCmdUI *pCmdUI);
	afx_msg void OnEditBpp();
	afx_msg void OnUpdateEditBpp(CCmdUI *pCmdUI);
	afx_msg void OnEditRotate();
	afx_msg void OnUpdateEditRotate(CCmdUI *pCmdUI);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // 3DCodeView.cpp �еĵ��԰汾
inline C3DCodeDoc* C3DCodeView::GetDocument() const
   { return reinterpret_cast<C3DCodeDoc*>(m_pDocument); }
#endif