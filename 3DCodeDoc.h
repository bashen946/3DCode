
// 3DCodeDoc.h : C3DCodeDoc ��Ľӿ�
//


#pragma once
#include "CyImage.h"

class C3DCodeView;

class C3DCodeDoc : public CDocument
{
protected: // �������л�����
	C3DCodeDoc();
	DECLARE_DYNCREATE(C3DCodeDoc)

// ����
private:
	// �����ͼ��
	CyImage* m_pImage;

	/// ����Ȥ����
	RoiRect m_roi;

	// ����ͼ��
	BOOL m_bImageLocked;

	// ���������
	CString m_strData;

public:
	// ���ͼ��ָ��
	CyImage* GetImage();

	// ��ȡ����Ȥ����
	RoiRect& GetROI();

	// �ж�ͼ��Ϊ��
	BOOL ImageIsNull();

	// �ж�ͼ���Ƿ�����
	BOOL ImageIsLocked();

	// ��ȡͼ���С
	const CSize GetImageSize();

	// ��ȡ��ͼ
	C3DCodeView* GetView();

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// ʵ��
public:
	virtual ~C3DCodeDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// ����Ϊ����������������������ݵ� Helper ����
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
};
