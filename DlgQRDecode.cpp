// DlgQRDecode.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "3DCode.h"
#include "DlgQRDecode.h"
#include "afxdialogex.h"

#include "3DCodeDoc.h"
#include "AfxGlobal.h"
#include "FunctionsUsingMFC.h"
#include "CodeTransform.h"

#include "DMDecoder.h"
#include "PDF417Decoder.h"
#include "AztecDecoder.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CDlgQRDecode �Ի���

IMPLEMENT_DYNAMIC(CDlgQRDecode, CDialogEx)

CDlgQRDecode::CDlgQRDecode(CWnd* pParent) : CDialogEx(CDlgQRDecode::IDD, pParent)
	, m_bUseHybrid(TRUE)
	, m_bTryHarder(TRUE)
{
	m_pImage = NULL;

	// ������ɫ
	m_BackgroundColor = RGB(0, 0, 0);

	ImageInfo::Init("yuanyuanxiang");
}

CDlgQRDecode::~CDlgQRDecode()
{
}

void CDlgQRDecode::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_USE_HYBRID, m_bUseHybrid);
	DDX_Check(pDX, IDC_CHECK_TRY_HARDER, m_bTryHarder);
	DDX_Control(pDX, IDC_CHECK_USE_HYBRID, m_checkUseHybrid);
	DDX_Control(pDX, IDC_CHECK_TRY_HARDER, m_checkTryHarder);
}


void CDlgQRDecode::SetImage()
{
	m_pImage = AfxGetImage();
	C3DCodeDoc* pDoc = (C3DCodeDoc*)AfxGetActiveDoc();
	if (pDoc != NULL)
	{
		m_pImage = pDoc->GetImage();
		m_roi = pDoc->GetROI();
	}
	else
	{
		m_pImage = NULL;
		m_roi = RoiRect(0, 0, 0, 0);
	}
}

/// ���뵱ǰͼ��
BOOL CDlgQRDecode::Decode()
{
	SetImage();
	if (m_pImage == NULL || m_pImage->IsNull())
		return FALSE;

	// ���»��߻�ȡ�ؼ���������
	m_strPublicString.Empty();
	m_strPrivateString.Empty();
	m_bUseHybrid = m_checkUseHybrid.GetCheck();
	m_bTryHarder = m_checkTryHarder.GetCheck();	
	// ��ȡ�������
	int nWidth = 0, nHeight = 0, nRowlen = 0;
	// m_pImage->ChangeBPP(32);
	m_pImage->GetInfomation(nWidth, nHeight, nRowlen);
	int nChannel = m_pImage->GetChannel();
	int nFloatRowlen = m_pImage->GetFloatDataRowlen();
	BYTE *pHead = m_pImage->GetHeadAddress();

	// ��������ͼ��
	ImageInfo pImage(pHead, nWidth, nHeight, nChannel, m_roi);
	BOOL success = DecodeWholeImage(DecodeSrcInfo(pImage, TRUE, TRUE), &qr, &inner);
	if (qr.m_pData == NULL)
	{
		// ���Խ���DM��PDF417��Aztec��ά�����
		DMDecoder dm;
		dm.Init(pHead, nWidth, nHeight, nChannel, m_roi);
		if (dm.Decode(m_BackgroundColor))
		{
			qr.m_pData = dm.GetData();
		}
		else
		{
			PDF417Decoder pdf417;
			pdf417.CopyOf(dm);
			if (pdf417.Decode(m_BackgroundColor))
			{
				qr.m_pData = pdf417.GetData();
			}
			else
			{
				AztecDecoder az;
				az.CopyOf(dm);
				if (az.Decode(m_BackgroundColor))
				{
					qr.m_pData = az.GetData();
				}
			}
		}
	}
	if (qr.m_pData == NULL)
	{
		GetDlgItem(IDC_EDITSOURCEDATA_PUBLIC)->SetWindowText(_T("*** Decode failed ***"));
		return FALSE;
	}
	int m_ncLength = 0;
	m_strPublicString = UTF8Convert2Unicode(qr.m_pData, m_ncLength);
	UpdateDecodeInfo();
	if (inner.m_pData != NULL)
	{
		m_strPrivateString = UTF8Convert2Unicode(inner.m_pData, m_ncLength);
		GetDlgItem(IDC_EDITSOURCEDATA_PRIVATE)->SetWindowText(m_strPrivateString);
		SetWindowInt(GetDlgItem(IDC_EDIT_INNER_ECLEVEL), inner.m_nEcLevel);
		SetWindowInt(GetDlgItem(IDC_EDIT_INNER_MASK), inner.m_nMaskingNo);
		UpdateecLevelTipText(inner.m_nEcLevel);
		return TRUE;
	}
	else
	{
		GetDlgItem(IDC_EDITSOURCEDATA_PRIVATE)->SetWindowText(_T("*** Decode failed ***"));
	}
	return TRUE;
}


void CDlgQRDecode::UpdateDecodeInfo()
{
	// ���¿ؼ�״̬
	CString ecLevel;
	switch (qr.m_nEcLevel)
	{
	case 0:ecLevel = _T("L(7%)"); break;
	case 1:ecLevel = _T("M(15%)"); break;
	case 2:ecLevel = _T("Q(25%)"); break;
	case 3:ecLevel = _T("H(30%)"); break;
	default:break;
	}
	GetDlgItem(IDC_EDIT_EC_LEVEL)->SetWindowText(ecLevel);
	SetWindowInt(GetDlgItem(IDC_EDIT_QR_VERSION), qr.m_nVersion);
	SetWindowInt(GetDlgItem(IDC_EDIT_MASK_VERSION), qr.m_nMaskingNo);
	SetWindowFloat(GetDlgItem(IDC_EDIT_MODULESIZE), qr.m_fModuleSize);
	GetDlgItem(IDC_EDITSOURCEDATA_PUBLIC)->SetWindowText(m_strPublicString);
}


BEGIN_MESSAGE_MAP(CDlgQRDecode, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_DECODE, &CDlgQRDecode::OnBnClickedButtonDecode)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BUTTON_SAVE_IMAGE, &CDlgQRDecode::OnBnClickedButtonSaveImage)
	ON_BN_CLICKED(IDC_BN_BACKGROUND, &CDlgQRDecode::OnBnClickedBnBackground)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CDlgQRDecode::OnBnClickedButtonClear)
END_MESSAGE_MAP()


// CDlgQRDecode ��Ϣ�������


void CDlgQRDecode::OnBnClickedButtonDecode()
{
	BOOL result = Decode();
	TRACE(" * ������: %d\n", result);
}

BOOL CDlgQRDecode::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ������ʾ
	EnableToolTips(true);
	m_ToolTip.Create(this);
	m_ToolTip.Activate(true);
	m_ToolTip.SetDelayTime(TTDT_INITIAL, 200); //�ӳ�200ms����ʾ
	m_ToolTip.SetDelayTime(TTDT_AUTOPOP, 3000);//��ʾ����3000 ms

	m_ToolTip.AddTool(GetDlgItem(IDC_CHECK_USE_HYBRID), _T("���û�϶�ֵ��"));
	m_ToolTip.AddTool(GetDlgItem(IDC_CHECK_TRY_HARDER), _T("����ǿ����������"));
	m_ToolTip.AddTool(GetDlgItem(IDC_EDIT_EC_LEVEL), _T("QR����������Ĵ�С"));
	m_ToolTip.AddTool(GetDlgItem(IDC_EDIT_MODULESIZE), _T("ģ���Сͨ������1"));
	m_ToolTip.AddTool(GetDlgItem(IDC_EDIT_QR_VERSION), _T("QR��汾��1��40"));
	m_ToolTip.AddTool(GetDlgItem(IDC_EDIT_MASK_VERSION), _T("QR�빲��8������"));
	m_ToolTip.AddTool(GetDlgItem(IDC_BUTTON_DECODE), _T("�Ե�ǰͼ�����"));
	m_ToolTip.AddTool(GetDlgItem(IDC_BUTTON_SAVE_IMAGE), _T("���������ά��"));
	m_ToolTip.AddTool(GetDlgItem(IDC_BUTTON_CLEAR), _T("���������"));
	m_ToolTip.AddTool(GetDlgItem(IDC_EDIT_INNER_ECLEVEL), _T("���ĵľ���ȼ�"));
	m_ToolTip.AddTool(GetDlgItem(IDC_EDIT_INNER_MASK), _T("���ĵ�����汾"));
	m_ToolTip.AddTool(GetDlgItem(IDC_BN_BACKGROUND), _T("���ñ���ɫ(Ĭ�Ϻ�ɫ)"));

	return TRUE;
}


void CDlgQRDecode::OnOK()
{
	TRACE(" * OnOK is called.\n");
	return;
}


BOOL CDlgQRDecode::PreTranslateMessage(MSG* pMsg)
{
	m_ToolTip.RelayEvent(pMsg);
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CDlgQRDecode::OnPaint()
{
	CPaintDC dc(this); // device context for painting
}


/// ʹ�û���ͼ����˸
BOOL CDlgQRDecode::OnEraseBkgnd(CDC* pDC)
{
	return CDialog::OnEraseBkgnd(pDC);
}


void CDlgQRDecode::OnBnClickedButtonSaveImage()
{
	if (!m_pImage->IsNull())
	{
		CyImage *roi = m_pImage->ROI(m_roi);
		roi->Save(this);
		delete roi;
	}
}


void CDlgQRDecode::UpdateecLevelTipText(int nEcLevel)
{
	switch (nEcLevel)
	{
	case 1:m_ToolTip.UpdateTipText(_T("1/15"), GetDlgItem(IDC_EDIT_INNER_ECLEVEL)); break;
	case 2:m_ToolTip.UpdateTipText(_T("2/15"), GetDlgItem(IDC_EDIT_INNER_ECLEVEL)); break;
	case 3:m_ToolTip.UpdateTipText(_T("3/15"), GetDlgItem(IDC_EDIT_INNER_ECLEVEL)); break;
	case 4:m_ToolTip.UpdateTipText(_T("4/15"), GetDlgItem(IDC_EDIT_INNER_ECLEVEL)); break;
	case 5:m_ToolTip.UpdateTipText(_T("5/15"), GetDlgItem(IDC_EDIT_INNER_ECLEVEL)); break;
	case 6:m_ToolTip.UpdateTipText(_T("6/15"), GetDlgItem(IDC_EDIT_INNER_ECLEVEL)); break;
	case 7:m_ToolTip.UpdateTipText(_T("7/15"), GetDlgItem(IDC_EDIT_INNER_ECLEVEL)); break;
	default:break;
	}
}


void CDlgQRDecode::Reset()
{
	m_pImage = NULL;
	m_strPublicString.Empty();
	m_strPrivateString.Empty();
	GetDlgItem(IDC_EDIT_EC_LEVEL)->SetWindowText(_T(""));
	GetDlgItem(IDC_EDIT_MODULESIZE)->SetWindowText(_T(""));
	GetDlgItem(IDC_EDIT_QR_VERSION)->SetWindowText(_T(""));
	GetDlgItem(IDC_EDIT_MASK_VERSION)->SetWindowText(_T(""));
	GetDlgItem(IDC_EDIT_INNER_ECLEVEL)->SetWindowText(_T(""));
	GetDlgItem(IDC_EDIT_INNER_MASK)->SetWindowText(_T(""));
	GetDlgItem(IDC_EDITSOURCEDATA_PUBLIC)->SetWindowText(_T(""));
	GetDlgItem(IDC_EDITSOURCEDATA_PRIVATE)->SetWindowText(_T(""));
}


void CDlgQRDecode::OnBnClickedBnBackground()
{
	CColorDialog m_setClrDlg;
	m_setClrDlg.m_cc.Flags |= CC_FULLOPEN | CC_RGBINIT;
	m_setClrDlg.m_cc.rgbResult = m_BackgroundColor;
	if (IDOK == m_setClrDlg.DoModal())
	{
		if (m_BackgroundColor == m_setClrDlg.m_cc.rgbResult)
			return;
		m_BackgroundColor = m_setClrDlg.m_cc.rgbResult;
	}
}


void CDlgQRDecode::OnBnClickedButtonClear()
{
	Reset();
}