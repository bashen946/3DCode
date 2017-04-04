
// 3DCode.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "3DCode.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "3DCodeDoc.h"
#include "3DCodeView.h"
#include "AfxGlobal.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// C3DCodeApp

BEGIN_MESSAGE_MAP(C3DCodeApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &C3DCodeApp::OnAppAbout)
	// �����ļ��ı�׼�ĵ�����
	ON_COMMAND(ID_FILE_NEW, &C3DCodeApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &C3DCodeApp::OnFileOpen)
	// ��׼��ӡ��������
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


/// C3DCodeApp ����
C3DCodeApp::C3DCodeApp()
{
	m_bHiColorIcons = TRUE;

	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// ���Ӧ�ó��������ù�����������ʱ֧��(/clr)�����ģ���: 
	//     1) �����д˸������ã�������������������֧�ֲ�������������
	//     2) ��������Ŀ�У������밴������˳���� System.Windows.Forms ������á�
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO:  ������Ӧ�ó��� ID �ַ����滻ΪΨһ�� ID �ַ�����������ַ�����ʽ
	//Ϊ CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("3DCode.AppID.NoVersion"));

	// TODO:  �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

/// Ψһ��һ�� C3DCodeApp ����
C3DCodeApp theApp;


/// C3DCodeApp ��ʼ��
BOOL C3DCodeApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()��  ���򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	// ��ʼ�� OLE ��
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction();

	// ʹ�� RichEdit �ؼ���Ҫ  AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO:  Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Capital Normal University"));
	LoadStdProfileSettings(12);  // ���ر�׼ INI �ļ�ѡ��(���� MRU)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// ע��Ӧ�ó�����ĵ�ģ�塣  �ĵ�ģ��
	// �������ĵ�����ܴ��ں���ͼ֮�������
	// CMultiDocTemplate* pDocTemplate;
	m_pDocTemplate = new CMultiDocTemplate(IDR_3DCodeTYPE,
		RUNTIME_CLASS(C3DCodeDoc),
		RUNTIME_CLASS(CChildFrame), // �Զ��� MDI �ӿ��
		RUNTIME_CLASS(C3DCodeView));
	if (!m_pDocTemplate)
		return FALSE;
	AddDocTemplate(m_pDocTemplate);

	// ������ MDI ��ܴ���
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;


	// ������׼ shell ���DDE�����ļ�������������
	CCommandLineInfo cmdInfo;
	// cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
	ParseCommandLine(cmdInfo);

	// ��������������ָ�������  ���
	// �� /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE��
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// �������ѳ�ʼ���������ʾ����������и���
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	//m_pMainWnd->OpenClipboard();
	//EmptyClipboard();
	//CloseClipboard();

	return TRUE;
}

/// �˳�
int C3DCodeApp::ExitInstance()
{
	//TODO:  �����������ӵĸ�����Դ
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

// C3DCodeApp ��Ϣ�������


/** @class CAboutDlg
* @brief ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
*/
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	/// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

/// ��׼���캯��
CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

/// �Ի������ݽ���
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

/// �������жԻ����Ӧ�ó�������
void C3DCodeApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/// C3DCodeApp �Զ������/���淽��
void C3DCodeApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
	bNameValid = strName.LoadString(IDS_EXPLORER);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
}

/// �����Զ���״̬
void C3DCodeApp::LoadCustomState()
{
}

/// �����Զ���״̬
void C3DCodeApp::SaveCustomState()
{
}

/**
* @brief ���ĵ�
* @param[in] lpszPathName �ĵ�����
* @param[in] bMakeVisible �Ƿ�ɼ�
* @return �ĵ�ָ��
*/
C3DCodeDoc* C3DCodeApp::OpenDocument(LPCTSTR lpszPathName, BOOL bMakeVisible)
{
	CDocument* pDoc = m_pDocTemplate->OpenDocumentFile(lpszPathName, bMakeVisible);
	return (C3DCodeDoc*)pDoc;
}

// C3DCodeApp ��Ϣ�������

/// �½��ļ�ʱ����Ӧ����
void C3DCodeApp::OnFileNew()
{
	OpenDocument(NULL, TRUE);
}

/// ���ļ�ʱ����Ӧ����
void C3DCodeApp::OnFileOpen()
{
	CWinAppEx::OnFileOpen();
}

/**
* @brief ���ĵ�
* @param[in] lpszFileName �ĵ�����
* @return �ĵ�ָ��
*/
CDocument* C3DCodeApp::OpenDocumentFile(LPCTSTR lpszFileName)
{
	C3DCodeDoc *pDoc = (C3DCodeDoc*)AfxGetActiveDoc();
	CyImage *pImage = AfxGetImage();
	// �Ѿ�����һ���հ��ĵ��������ͼ��
	if (pDoc && pImage->IsNull())
	{
		if (pDoc->OnOpenDocument(lpszFileName))
		{
			pDoc->SetPathName(lpszFileName);
			pDoc->GetView()->SetPaintRect(pDoc->GetImageSize());
			return pDoc;
		}
		return NULL;
	}

	return CWinAppEx::OpenDocumentFile(lpszFileName);
}


/// ��ȡ��ĵ���ͼ��ָ��
CyImage* AfxGetImage()
{
	C3DCodeDoc* pDoc = (C3DCodeDoc*)AfxGetActiveDoc();

	return pDoc ? pDoc->GetImage() : NULL;
}

/**
* @brief ���µ��ĵ�
* @param[in] lpszPathName �ĵ�����
* @param[in] bMakeVisible �Ƿ�ɼ�
* @return �ĵ�ָ��
* @remark �ú�����ȫ�ֵ�
*/
extern C3DCodeDoc* AfxOpenDocument(LPCTSTR lpszPathName, BOOL bMakeVisible)
{
	C3DCodeApp* pApp = (C3DCodeApp*)AfxGetApp();
	C3DCodeDoc* pDoc = pApp->OpenDocument(lpszPathName, bMakeVisible);
	return pDoc;
}