// DlgQREncode.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "3DCode.h"
#include "DlgQREncode.h"
#include "AfxGlobal.h"
#include "3DCodeView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CDlgQREncode �Ի���

/// ��ȡ��ǰ��ĵ���ͼ��ָ�룬�������ɶ�ά��
BOOL CDlgQREncode::SetImage()
{
	m_pImage = AfxGetImage();
	return TRUE;
}


/// ����ˢ����ͼ
void CDlgQREncode::Repaint()
{
	/// @remark 2016/9/15 ��Repaint��Ϊ�麯������ʵ������Ķ�̬����
	C3DCodeView* pView = (C3DCodeView*)AfxGetActiveView();
	pView->Invalidate(TRUE);
}