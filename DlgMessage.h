#pragma once
#include "afxwin.h"


// CDlgMessage �Ի���

class CDlgMessage : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgMessage)

public:
	CDlgMessage(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgMessage();

// �Ի�������
	enum { IDD = IDD_MESSAGE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_EditInput;
	float m_fInput;
	CStatic m_StaticInputName;
	CString m_strInputName;
	virtual BOOL OnInitDialog();
};
