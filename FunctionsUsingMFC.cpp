#include "stdafx.h"
#include "FunctionsUsingMFC.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/** ���ô�����ʾ���� */
void SetWindowInt(CWnd* pWnd, int nInt)
{
	CString text;
	text.Format(_T("%d"), nInt);
	pWnd->SetWindowText(text);
}


/** ���ô�����ʾС�� */
void SetWindowFloat(CWnd* pWnd, float val)
{
	CString text;
	text.Format(_T("%.3f"), val);
	pWnd->SetWindowText(text);
}


/** ȡ�ô�������ʾ���� */
int GetWindowInt(CWnd* pWnd)
{
	CString text;
	pWnd->GetWindowText(text);
	return _ttoi(text);
}


/** ȡ�ô�������ʾ�ĸ����� */
double GetWindowFloat(CWnd* pWnd)
{
	CString text;
	pWnd->GetWindowText(text);
	return _ttof(text);
}