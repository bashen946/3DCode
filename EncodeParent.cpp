// DlgQREncode.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "encodeparent.h"
#include "afxdialogex.h"
#include "QrEncodeFuncs.h"
#include "AfxGlobal.h"
#include "FunctionsUsingMFC.h"
#include "ColorsEncode.h"
#include "CodeTransform.h"
#include "MatrixConvert2Image.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CEncodeParent �Ի���

IMPLEMENT_DYNAMIC(CEncodeParent, CDialogEx)

CEncodeParent::CEncodeParent(CWnd* pParent) : CDialogEx(CEncodeParent::IDD, pParent)
{
	// ͼ��
	m_pImage = NULL;
	m_pLogo = new CyImage;
	m_pLogo->Create(22, 22, 24, 0UL);
	// ������ɫ
	m_BackgroundColor = RGB(0, 0, 0);
	m_ForegroundColor = RGB(255, 255, 255);

	// ����/���� ��С
	m_nSymbleSize = 0;			//���ųߴ�
	m_nPixelSize = 4;			//���ش�С
	m_nEnableMixed = 0;			//��ģʽ

	// ����logo��С�Ĳ���
	m_bHasALogo = FALSE;
	m_fBorder1 = 1/2.7f;
	m_fBorder2 = 1/2.7f;

	m_nInnerVersion = 0;
}

CEncodeParent::~CEncodeParent()
{
	SAFE_DELETE(m_pLogo);
}

void CEncodeParent::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBOLEVEL, m_comboLevel);
	DDX_Control(pDX, IDC_COMBOVERSION, m_comboVersion);
	DDX_Control(pDX, IDC_STATICVERSION, m_staticVersion);
	DDX_Control(pDX, IDC_CHECKAUTOEXTENT, m_checkAutoExtent);
	DDX_Control(pDX, IDC_COMBOMASKINGNO, m_comboMaskingNo);
	DDX_Control(pDX, IDC_STATICMASKINGNO, m_staticMaskingNo);
	DDX_Control(pDX, IDC_EDITMODULESIZE, m_editModuleSize);
	DDX_Control(pDX, IDC_SPINMODULESIZE, m_spinModuleSize);
	DDX_Control(pDX, IDC_STATICBMPSIZE, m_staticBmpSize);
	DDX_Control(pDX, IDC_EDITSOURCEDATA_PUBLIC, m_editSourceDataPublic);
	DDX_Control(pDX, IDC_EDITSOURCEDATA_PRIVATE, m_editSourceDataPrivate);
	DDX_Control(pDX, IDC_SLIDER_LOGO_WIDTH, m_sliderWidth);
	DDX_Control(pDX, IDC_SLIDER_LOGO_HEIGHT, m_sliderHeight);
	DDX_Control(pDX, IDC_ENABLE_MIXED, m_buttonEnableMixed);
	DDX_Control(pDX, IDC_STATICQRCODESIZE, m_staticQRCodeSize);
	DDX_Control(pDX, IDC_COMBOLEVEL2, m_comboInnerEcLevel);
	DDX_Control(pDX, IDC_COMBOMASKINGNO2, m_comboInnerMaskNo);
	DDX_Control(pDX, IDC_CHECK_COMPRESS, m_BnCompress);
}


BEGIN_MESSAGE_MAP(CEncodeParent, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBOLEVEL, &CEncodeParent::OnCbnSelchangeCombolevel)
	ON_CBN_SELCHANGE(IDC_COMBOVERSION, &CEncodeParent::OnCbnSelchangeComboversion)
	ON_BN_CLICKED(IDC_CHECKAUTOEXTENT, &CEncodeParent::OnBnClickedCheckautoextent)
	ON_CBN_SELCHANGE(IDC_COMBOMASKINGNO, &CEncodeParent::OnCbnSelchangeCombomaskingno)
	ON_EN_CHANGE(IDC_EDITMODULESIZE, &CEncodeParent::OnEnChangeEditmodulesize)
	ON_BN_CLICKED(IDC_IMPORT_LOGO, &CEncodeParent::OnBnClickedImportLogo)
	ON_EN_CHANGE(IDC_EDITSOURCEDATA_PUBLIC, &CEncodeParent::OnEnChangeEditsourcedataPublic)
	ON_EN_CHANGE(IDC_EDITSOURCEDATA_PRIVATE, &CEncodeParent::OnEnChangeEditsourcedataPrivate)
	// �ı���ɫ��Ӧ����
	ON_EN_KILLFOCUS(IDC_FOREGROUND_R, &CEncodeParent::OnEnChangeForegroundR)
	ON_EN_KILLFOCUS(IDC_FOREGROUND_G, &CEncodeParent::OnEnChangeForegroundG)
	ON_EN_KILLFOCUS(IDC_FOREGROUND_B, &CEncodeParent::OnEnChangeForegroundB)
	ON_EN_KILLFOCUS(IDC_BACKGROUND_R, &CEncodeParent::OnEnChangeBackgroundR)
	ON_EN_KILLFOCUS(IDC_BACKGROUND_G, &CEncodeParent::OnEnChangeBackgroundG)
	ON_EN_KILLFOCUS(IDC_BACKGROUND_B, &CEncodeParent::OnEnChangeBackgroundB)
	ON_EN_KILLFOCUS(IDC_QRCOLOR1_R, &CEncodeParent::OnEnChangeEnCodeColor1R)
	ON_EN_KILLFOCUS(IDC_QRCOLOR1_G, &CEncodeParent::OnEnChangeEnCodeColor1G)
	ON_EN_KILLFOCUS(IDC_QRCOLOR1_B, &CEncodeParent::OnEnChangeEnCodeColor1B)
	ON_EN_KILLFOCUS(IDC_QRCOLOR2_R, &CEncodeParent::OnEnChangeEnCodeColor2R)
	ON_EN_KILLFOCUS(IDC_QRCOLOR2_G, &CEncodeParent::OnEnChangeEnCodeColor2G)
	ON_EN_KILLFOCUS(IDC_QRCOLOR2_B, &CEncodeParent::OnEnChangeEnCodeColor2B)
	ON_BN_CLICKED(IDC_BN_FOREGROUND, &CEncodeParent::OnBnClickedBnForeground)
	ON_BN_CLICKED(IDC_BN_BACKGROUND, &CEncodeParent::OnBnClickedBnBackground)
	ON_BN_CLICKED(IDC_BN_ENCODE_COLOR1, &CEncodeParent::OnBnClickedBnEncodeColor1)
	ON_BN_CLICKED(IDC_BN_ENCODE_COLOR2, &CEncodeParent::OnBnClickedBnEncodeColor2)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPINMODULESIZE, &CEncodeParent::OnDeltaposSpinmodulesize)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_LOGO_WIDTH, &CEncodeParent::OnReleasedcaptureSliderLogoWidth)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_LOGO_HEIGHT, &CEncodeParent::OnReleasedcaptureSliderLogoHeight)
	ON_BN_CLICKED(IDC_RESET_LOGO_SIZE, &CEncodeParent::OnBnClickedResetLogoSize)
	ON_BN_CLICKED(IDC_CLEAR_LOGO, &CEncodeParent::OnBnClickedClearLogo)
	ON_BN_CLICKED(IDC_ENABLE_MIXED, &CEncodeParent::OnBnClickedEnableMixed)
	ON_BN_CLICKED(IDC_BUTTON_ENCODE, &CEncodeParent::OnBnClickedButtonEncode)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CEncodeParent::OnBnClickedButtonSave)
	ON_CBN_SELCHANGE(IDC_COMBOLEVEL2, &CEncodeParent::OnCbnSelchangeCombolevel2)
	ON_CBN_SELCHANGE(IDC_COMBOMASKINGNO2, &CEncodeParent::OnCbnSelchangeCombomaskingno2)
	ON_BN_CLICKED(IDC_CHECK_COMPRESS, &CEncodeParent::OnBnClickedCheckCompress)
END_MESSAGE_MAP()

/// ��ȡ��ĵ������õ�ǰ��ͼ��
BOOL CEncodeParent::SetImage()
{
	m_pImage = NULL;
	return TRUE;
}

/** 
* @brief ����
* @details ���ȣ�����SetImage����û�ĵ���ͼ��ָ�룻
*		���ţ����ձ�������->��������->���LOGO�Ĳ������ɶ�ά�롣
*/
BOOL CEncodeParent::Encode()
{
	SetImage();
	if (m_pImage == NULL)
		return TRUE;

	CString strPublicEncodeData;
	m_editSourceDataPublic.GetWindowText(strPublicEncodeData);

	int nLevel = m_comboLevel.GetCurSel();
	int nVersion = m_comboVersion.GetCurSel();
	BOOL bAutoExtent = (m_checkAutoExtent.GetCheck() != 0);
	int nMaskingNo = m_comboMaskingNo.GetCurSel() - 1;

	m_staticVersion.SetWindowText(_T(""));
	m_staticMaskingNo.SetWindowText(_T(""));

	/* �������� */
	CQR_Encode* pQR_Encode = new CQR_Encode;
	BOOL bDataEncoded = EncodeData(strPublicEncodeData, pQR_Encode, nLevel, nVersion, bAutoExtent, nMaskingNo);

	if (bDataEncoded)
	{
		// ���°汾�š������
		CString strWork;
		if (nVersion != pQR_Encode->m_nVersion)
			strWork.Format(_T("(%d)"), pQR_Encode->m_nVersion);
		else
			strWork.Empty();
		m_staticVersion.SetWindowText(strWork);
		if (nMaskingNo == -1)
			strWork.Format(_T("(%d)"), pQR_Encode->m_nMaskingNo);
		else
			strWork.Empty();
		m_staticMaskingNo.SetWindowText(strWork);

		m_ToolTip.UpdateTipText(_T(""), &m_editSourceDataPublic);

		// ��������
		EncodePrivateData(pQR_Encode);

		// ���LOGO
		Addlogo();

		// �ػ�
		Repaint();
	}

	delete pQR_Encode;
	return TRUE;
}


/// �ú����ز����٣���������Ŀ�̳в���̬����
BOOL CEncodeParent::EncodePrivateData(CQR_Encode* pQR_Encode)
{
	// ��ʼ��logo����
	InitLogoRect(m_fBorder1, m_fBorder2);

	// ��������
	CString strPrivateEncodeData;
	m_editSourceDataPrivate.GetWindowText(strPrivateEncodeData);
	int strLen = 0;
	char *temp = UnicodeConvert2UTF8(strPrivateEncodeData, strLen);
	ColorsEncode ce(pQR_Encode->m_byModuleData, pQR_Encode->m_nSymbleSize, m_LogoRect);
	BOOL bEncodeSucess = ce.EncodeColors(temp, strLen, m_nInnerEcLevel, m_nInnerMaskNo, m_nInnerVersion);
	SAFE_DELETE(temp);
	// �жϲ�ɫ�����Ƿ�ɹ�
	if (bEncodeSucess)
	{
		m_ToolTip.UpdateTipText(_T(""), &m_editSourceDataPrivate);
	}
	else
	{
		m_ToolTip.UpdateTipText(_T("��������̫��"), &m_editSourceDataPrivate);
	}

	// ��ȡ���Ŵ�СȻ�󽫷���ת��Ϊͼ��
	m_nSymbleSize = pQR_Encode->m_nSymbleSize;
	Matrix2ColorImage(pQR_Encode->m_byModuleData, m_nSymbleSize, m_pImage, m_nPixelSize, 
		m_ForegroundColor, m_BackgroundColor, m_QREncodeColor1, m_QREncodeColor2);

	return bEncodeSucess;
}

/// ��ӱ�ʶ
void CEncodeParent::Addlogo()
{
	if (m_nEnableMixed == BOUNDING_MODE_REPLACED)
	{
		ReplacedLogo(m_pImage, m_pLogo, m_realLogoRect);
	}
	else if (m_nEnableMixed == BOUNDING_MODE_MIXED)
	{
		MixedLogo(m_pImage, m_pLogo, m_realLogoRect, 0.5f);
	}
}


/// ��ɫ�Ƿ���Ա���
BOOL CEncodeParent::IsColorsEncodeAble()
{
	// ��С��QR��ߴ�Ϊ21
	return (m_ForegroundElemsIndex.size() >= 21 * 21);
}


void CEncodeParent::UpdateecLevelTipText()
{
	switch (m_nInnerEcLevel)
	{
	case 1:m_ToolTip.UpdateTipText(_T("1/15"), &m_comboInnerEcLevel); break;
	case 2:m_ToolTip.UpdateTipText(_T("2/15"), &m_comboInnerEcLevel); break;
	case 3:m_ToolTip.UpdateTipText(_T("3/15"), &m_comboInnerEcLevel); break;
	case 4:m_ToolTip.UpdateTipText(_T("4/15"), &m_comboInnerEcLevel); break;
	case 5:m_ToolTip.UpdateTipText(_T("5/15"), &m_comboInnerEcLevel); break;
	case 6:m_ToolTip.UpdateTipText(_T("6/15"), &m_comboInnerEcLevel); break;
	case 7:m_ToolTip.UpdateTipText(_T("7/15"), &m_comboInnerEcLevel); break;
	default:break;
	}
}


/** 
* @note border1,border2(����0��0.5)��������logo���ά��߽�ľ���
*/
void CEncodeParent::InitLogoRect(float border1, float border2)
{
	if (!m_bHasALogo)
		return;
	int nWidth = m_nSymbleSize;
	int nHeight = m_nSymbleSize;
	// ����logo����ά��߽�ľ���
	m_LogoRect.left = int(border1 * nWidth);
	m_LogoRect.top = int(border2 * nHeight);
	m_LogoRect.right = nWidth - m_LogoRect.left;
	m_LogoRect.bottom = nHeight - m_LogoRect.top;
	m_realLogoRect = m_LogoRect;
	ZoomRect(m_realLogoRect, 1.f * m_nPixelSize);
	MoveRect(m_realLogoRect, QR_MARGIN, QR_MARGIN);
}


// CEncodeParent ��Ϣ�������


void CEncodeParent::OnCbnSelchangeCombolevel()
{
	Encode();
	ShowImageSize();
}


void CEncodeParent::OnCbnSelchangeComboversion()
{
	m_checkAutoExtent.EnableWindow(m_comboVersion.GetCurSel() && m_comboVersion.GetCurSel() < 40);
	Encode();
	ShowImageSize();
}


void CEncodeParent::OnBnClickedCheckautoextent()
{
	Encode();
	ShowImageSize();
}


void CEncodeParent::OnCbnSelchangeCombomaskingno()
{
	Encode();
	ShowImageSize();
}


void CEncodeParent::ShowImageSize()
{
	CString strImageSize = _T("");
	CString strQRCodeSize = _T("");

	CString text;
	m_editSourceDataPublic.GetWindowText(text);
	if (text != _T(""))
	{
		m_nPixelSize = GetDlgItemInt(IDC_EDITMODULESIZE);

		if (m_nPixelSize >= 1 && m_nPixelSize <= PIXEL_MAX_SIZE)
		{
			int nSize = m_nSymbleSize * m_nPixelSize + 2 * QR_MARGIN;
			strImageSize.Format(_T("%d��%d"), nSize, nSize);
			strQRCodeSize.Format(_T("%d��%d"), m_nSymbleSize, m_nSymbleSize);
		}
	}
	m_staticBmpSize.SetWindowText(strImageSize);
	m_staticQRCodeSize.SetWindowText(strQRCodeSize);
}


void CEncodeParent::OnEnChangeEditmodulesize()
{
	ShowImageSize();
	Encode();
}


void CEncodeParent::OnBnClickedImportLogo()
{
	if (FAILED(m_pLogo->Load(this)))
		return;
	if (m_pLogo->ChangeBPP(24))
		TRACE(" * Info: ͼ��λ����Ѿ����޸�Ϊ24.\n");
	m_bHasALogo = TRUE;
	Encode();
	ShowImageSize();
}


void CEncodeParent::OnEnChangeEditsourcedataPublic()
{
	Encode();
	ShowImageSize();
}


void CEncodeParent::OnEnChangeEditsourcedataPrivate()
{
	Encode();
	ShowImageSize();
}


BOOL CEncodeParent::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ������ʾ
	EnableToolTips(true);
	m_ToolTip.Create(this);
	m_ToolTip.Activate(true);
	m_ToolTip.SetDelayTime(TTDT_INITIAL, 200); //�ӳ�200ms����ʾ
	m_ToolTip.SetDelayTime(TTDT_AUTOPOP, 3000);//��ʾ����ʱ��3000msʱ��

	// ��������Ͽ�
	m_comboLevel.InsertString(0, _T("L( 7%)"));
	m_comboLevel.InsertString(1, _T("M(15%)"));
	m_comboLevel.InsertString(2, _T("Q(25%)"));
	m_comboLevel.InsertString(3, _T("H(30%)"));
	m_comboLevel.SetCurSel(1);

	// �汾����Ͽ�
	m_comboVersion.InsertString(0, _T("�Զ�"));
	for (int i = 1; i <= 40; ++i)
	{
		CString text;
		text.Format(_T("%d"), i);
		m_comboVersion.InsertString(i, text);
	}
	m_comboVersion.SetCurSel(0);
	m_staticVersion.SetWindowText(_T(""));
	m_checkAutoExtent.SetCheck(1);
	m_checkAutoExtent.EnableWindow(FALSE);

	// �������Ͽ�
	m_comboMaskingNo.InsertString(0, _T("�Զ�"));
	for (int i = 0; i <= 7; ++i)
	{
		CString text;
		text.Format(_T("%d"), i);
		m_comboMaskingNo.InsertString(i + 1, text);
	}
	m_comboMaskingNo.SetCurSel(0);
	m_staticMaskingNo.SetWindowText(_T(""));

	// ���ش�С�༭��
	m_editModuleSize.SetWindowText(_T("4"));
	m_spinModuleSize.SetRange(1, PIXEL_MAX_SIZE);
	m_staticBmpSize.SetWindowText(_T(""));
	m_staticQRCodeSize.SetWindowText(_T(""));

	// ������
	m_editSourceDataPublic.SetWindowText(_T(""));

	// �ڲ����
	InitInnerEncodingParameters();

	// ��ɫ�༭��
	SetWindowInt(GetDlgItem(IDC_FOREGROUND_R), GetRValue(m_ForegroundColor));
	SetWindowInt(GetDlgItem(IDC_FOREGROUND_G), GetGValue(m_ForegroundColor));
	SetWindowInt(GetDlgItem(IDC_FOREGROUND_B), GetBValue(m_ForegroundColor));
	SetWindowInt(GetDlgItem(IDC_BACKGROUND_R), GetRValue(m_BackgroundColor));
	SetWindowInt(GetDlgItem(IDC_BACKGROUND_G), GetGValue(m_BackgroundColor));
	SetWindowInt(GetDlgItem(IDC_BACKGROUND_B), GetBValue(m_BackgroundColor));
	SetWindowInt(GetDlgItem(IDC_QRCOLOR1_R), GetRValue(m_QREncodeColor1));
	SetWindowInt(GetDlgItem(IDC_QRCOLOR1_G), GetGValue(m_QREncodeColor1));
	SetWindowInt(GetDlgItem(IDC_QRCOLOR1_B), GetBValue(m_QREncodeColor1));
	SetWindowInt(GetDlgItem(IDC_QRCOLOR2_R), GetRValue(m_QREncodeColor2));
	SetWindowInt(GetDlgItem(IDC_QRCOLOR2_G), GetGValue(m_QREncodeColor2));
	SetWindowInt(GetDlgItem(IDC_QRCOLOR2_B), GetBValue(m_QREncodeColor2));

	// ToolTip
	m_ToolTip.AddTool(&m_comboLevel, _T("ѡ����������"));
	m_ToolTip.AddTool(&m_comboVersion, _T("ѡ��QR��汾��"));
	m_ToolTip.AddTool(&m_checkAutoExtent, _T("�Զ���չ����"));
	m_ToolTip.AddTool(&m_comboMaskingNo, _T("ѡ������汾��"));
	m_ToolTip.AddTool(&m_editModuleSize, _T("�������صĴ�С"));
	m_ToolTip.AddTool(&m_spinModuleSize, _T("�������صĴ�С"));
	m_ToolTip.AddTool(&m_staticBmpSize, _T("��ά��ʵ�ʳߴ�"));
	m_ToolTip.AddTool(GetDlgItem(IDC_IMPORT_LOGO), _T("�����ʶ��"));
	m_ToolTip.AddTool(GetDlgItem(IDC_RESET_LOGO_SIZE), _T("�����ʶ��"));
	m_ToolTip.AddTool(GetDlgItem(IDC_CLEAR_LOGO), _T("�����ʶ��"));
	m_ToolTip.AddTool(GetDlgItem(IDC_BN_FOREGROUND), _T("ѡ��ǰ��ɫ"));
	m_ToolTip.AddTool(GetDlgItem(IDC_BN_BACKGROUND), _T("ѡ�񱳾�ɫ"));
	m_ToolTip.AddTool(GetDlgItem(IDC_BN_ENCODE_COLOR1), _T("���ò�ɫ����1"));
	m_ToolTip.AddTool(GetDlgItem(IDC_BN_ENCODE_COLOR2), _T("���ò�ɫ����2"));
	m_ToolTip.AddTool(&m_sliderWidth, _T("����LOGO�Ŀ��"));
	m_ToolTip.AddTool(&m_sliderHeight, _T("����LOGO�ĸ߶�"));
	m_ToolTip.AddTool(&m_buttonEnableMixed, _T("LOGO���ά����"));
	m_ToolTip.AddTool(GetDlgItem(IDC_BUTTON_ENCODE), _T("����Ϣ���б���"));
	m_ToolTip.AddTool(&m_BnCompress, _T("��zlibѹ������"));
	m_ToolTip.AddTool(&m_editSourceDataPublic, _T(""));
	m_ToolTip.AddTool(&m_editSourceDataPrivate, _T(""));

	// ������
	m_sliderWidth.SetRange(0, 100);
	m_sliderWidth.SetPos(74);
	m_sliderHeight.SetRange(0, 100);
	m_sliderHeight.SetPos(74);

	// ����ͼ��
	m_ToolTip.AddTool(GetDlgItem(IDC_BUTTON_SAVE), _T("���������ά��"));

	CenterWindow();

	return TRUE;
}


/// ��ʼ���ڲ�������
void CEncodeParent::InitInnerEncodingParameters()
{
	// ����ȼ�
	for (int i = 1; i < 8; ++i)
	{
		m_comboInnerEcLevel.InsertString(i - 1, Num2String(i));
	}
	for (int i = 0; i < 8; ++i)
	{
		m_comboInnerMaskNo.InsertString(i, Num2String(i));
	}
	m_nInnerEcLevel = 3;
	m_nInnerMaskNo = 1;
	m_QREncodeColor1 = RGB(255, 0, 0);
	m_QREncodeColor2 = RGB(0, 255, 0);
	m_comboInnerEcLevel.SetCurSel(2);
	m_comboInnerMaskNo.SetCurSel(1);
	m_ToolTip.AddTool(&m_comboInnerEcLevel, _T("ѡ����������"));
	m_ToolTip.AddTool(&m_comboInnerMaskNo, _T("ѡ������汾��"));
}


void CEncodeParent::OnCancel()
{
	ShowWindow(SW_HIDE);
	CDialogEx::OnCancel();
}


/// ����OnOK��������ENTER��
void CEncodeParent::OnOK()
{
	TRACE(" * OnOK is called.\n");
	return;
}


void CEncodeParent::OnEnChangeForegroundR()
{
	int num = GetWindowInt(GetDlgItem(IDC_FOREGROUND_R));
	int R = GetRValue(m_ForegroundColor);
	int G = GetGValue(m_ForegroundColor);
	int B = GetBValue(m_ForegroundColor);
	if (R == num)
		return;
	m_ForegroundColor = RGB(num, G, B);
	Encode();
}


void CEncodeParent::OnEnChangeForegroundG()
{
	int num = GetWindowInt(GetDlgItem(IDC_FOREGROUND_G));
	int R = GetRValue(m_ForegroundColor);
	int G = GetGValue(m_ForegroundColor);
	int B = GetBValue(m_ForegroundColor);
	if (G == num)
		return;
	m_ForegroundColor = RGB(R, num, B);
	Encode();
}


void CEncodeParent::OnEnChangeForegroundB()
{
	int num = GetWindowInt(GetDlgItem(IDC_FOREGROUND_B));
	int R = GetRValue(m_ForegroundColor);
	int G = GetGValue(m_ForegroundColor);
	int B = GetBValue(m_ForegroundColor);
	if (B == num)
		return;
	m_ForegroundColor = RGB(R, G, num);
	Encode();
}


void CEncodeParent::OnEnChangeBackgroundR()
{
	int num = GetWindowInt(GetDlgItem(IDC_BACKGROUND_R));
	int R = GetRValue(m_BackgroundColor);
	int G = GetGValue(m_BackgroundColor);
	int B = GetBValue(m_BackgroundColor);
	if (R == num)
		return;
	m_BackgroundColor = RGB(num, G, B);
	Encode();
}


void CEncodeParent::OnEnChangeBackgroundG()
{
	int num = GetWindowInt(GetDlgItem(IDC_BACKGROUND_G));
	int R = GetRValue(m_BackgroundColor);
	int G = GetGValue(m_BackgroundColor);
	int B = GetBValue(m_BackgroundColor);
	if (G == num)
		return;
	m_BackgroundColor = RGB(R, num, B);
	Encode();
}


void CEncodeParent::OnEnChangeBackgroundB()
{
	int num = GetWindowInt(GetDlgItem(IDC_BACKGROUND_B));
	int R = GetRValue(m_BackgroundColor);
	int G = GetGValue(m_BackgroundColor);
	int B = GetBValue(m_BackgroundColor);
	if (B == num)
		return;
	m_BackgroundColor = RGB(R, G, num);
	Encode();
}


void CEncodeParent::OnEnChangeEnCodeColor1R()
{
	int num = GetWindowInt(GetDlgItem(IDC_QRCOLOR1_R));
	int R = GetRValue(m_QREncodeColor1);
	int G = GetGValue(m_QREncodeColor1);
	int B = GetBValue(m_QREncodeColor1);
	if (R == num)
		return;
	m_QREncodeColor1 = RGB(num, G, B);
	Encode();
}


void CEncodeParent::OnEnChangeEnCodeColor1G()
{
	int num = GetWindowInt(GetDlgItem(IDC_QRCOLOR1_G));
	int R = GetRValue(m_QREncodeColor1);
	int G = GetGValue(m_QREncodeColor1);
	int B = GetBValue(m_QREncodeColor1);
	if (G == num)
		return;
	m_QREncodeColor1 = RGB(R, num, B);
	Encode();
}


void CEncodeParent::OnEnChangeEnCodeColor1B()
{
	int num = GetWindowInt(GetDlgItem(IDC_QRCOLOR1_B));
	int R = GetRValue(m_QREncodeColor1);
	int G = GetGValue(m_QREncodeColor1);
	int B = GetBValue(m_QREncodeColor1);
	if (B == num)
		return;
	m_QREncodeColor1 = RGB(R, G, num);
	Encode();
}


void CEncodeParent::OnEnChangeEnCodeColor2R()
{
	int num = GetWindowInt(GetDlgItem(IDC_QRCOLOR2_R));
	int R = GetRValue(m_QREncodeColor2);
	int G = GetGValue(m_QREncodeColor2);
	int B = GetBValue(m_QREncodeColor2);
	if (R == num)
		return;
	m_QREncodeColor2 = RGB(num, G, B);
	Encode();
}


void CEncodeParent::OnEnChangeEnCodeColor2G()
{
	int num = GetWindowInt(GetDlgItem(IDC_QRCOLOR2_G));
	int R = GetRValue(m_QREncodeColor2);
	int G = GetGValue(m_QREncodeColor2);
	int B = GetBValue(m_QREncodeColor2);
	if (G == num)
		return;
	m_QREncodeColor2 = RGB(R, num, B);
	Encode();
}


void CEncodeParent::OnEnChangeEnCodeColor2B()
{
	int num = GetWindowInt(GetDlgItem(IDC_QRCOLOR2_B));
	int R = GetRValue(m_QREncodeColor2);
	int G = GetGValue(m_QREncodeColor2);
	int B = GetBValue(m_QREncodeColor2);
	if (B == num)
		return;
	m_QREncodeColor2 = RGB(R, G, num);
	Encode();
}

void CEncodeParent::OnBnClickedBnForeground()
{
	CColorDialog m_setClrDlg;
	// CC_RGBINIT�������ϴ�ѡ�����ɫ��Ϊ��ʼ��ɫ��ʾ����
	m_setClrDlg.m_cc.Flags |= CC_FULLOPEN | CC_RGBINIT;
	m_setClrDlg.m_cc.rgbResult = m_ForegroundColor;
	if (IDOK == m_setClrDlg.DoModal())
	{
		if (m_ForegroundColor == m_setClrDlg.m_cc.rgbResult)
			return;
		// �����û�ѡ�����ɫ
		m_ForegroundColor = m_setClrDlg.m_cc.rgbResult;
		SetWindowInt(GetDlgItem(IDC_FOREGROUND_R), GetRValue(m_ForegroundColor));
		SetWindowInt(GetDlgItem(IDC_FOREGROUND_G), GetGValue(m_ForegroundColor));
		SetWindowInt(GetDlgItem(IDC_FOREGROUND_B), GetBValue(m_ForegroundColor));
		Encode();
	}
}


void CEncodeParent::OnBnClickedBnBackground()
{
	CColorDialog m_setClrDlg;
	m_setClrDlg.m_cc.Flags |= CC_FULLOPEN | CC_RGBINIT;
	m_setClrDlg.m_cc.rgbResult = m_BackgroundColor;
	if (IDOK == m_setClrDlg.DoModal())
	{
		if (m_BackgroundColor == m_setClrDlg.m_cc.rgbResult)
			return;
		m_BackgroundColor = m_setClrDlg.m_cc.rgbResult;
		SetWindowInt(GetDlgItem(IDC_BACKGROUND_R), GetRValue(m_BackgroundColor));
		SetWindowInt(GetDlgItem(IDC_BACKGROUND_G), GetGValue(m_BackgroundColor));
		SetWindowInt(GetDlgItem(IDC_BACKGROUND_B), GetBValue(m_BackgroundColor));
		Encode();
	}
}


void CEncodeParent::OnBnClickedBnEncodeColor1()
{
	CColorDialog m_setClrDlg;
	m_setClrDlg.m_cc.Flags |= CC_FULLOPEN | CC_RGBINIT;
	m_setClrDlg.m_cc.rgbResult = m_QREncodeColor1;
	if (IDOK == m_setClrDlg.DoModal())
	{
		if (m_QREncodeColor1 == m_setClrDlg.m_cc.rgbResult)
			return;
		m_QREncodeColor1 = m_setClrDlg.m_cc.rgbResult;
		SetWindowInt(GetDlgItem(IDC_QRCOLOR1_R), GetRValue(m_QREncodeColor1));
		SetWindowInt(GetDlgItem(IDC_QRCOLOR1_G), GetGValue(m_QREncodeColor1));
		SetWindowInt(GetDlgItem(IDC_QRCOLOR1_B), GetBValue(m_QREncodeColor1));
		Encode();
	}
}


void CEncodeParent::OnBnClickedBnEncodeColor2()
{
	CColorDialog m_setClrDlg;
	m_setClrDlg.m_cc.Flags |= CC_FULLOPEN | CC_RGBINIT;
	m_setClrDlg.m_cc.rgbResult = m_QREncodeColor2;
	if (IDOK == m_setClrDlg.DoModal())
	{
		if (m_QREncodeColor2 == m_setClrDlg.m_cc.rgbResult)
			return;
		m_QREncodeColor2 = m_setClrDlg.m_cc.rgbResult;
		SetWindowInt(GetDlgItem(IDC_QRCOLOR2_R), GetRValue(m_QREncodeColor2));
		SetWindowInt(GetDlgItem(IDC_QRCOLOR2_G), GetGValue(m_QREncodeColor2));
		SetWindowInt(GetDlgItem(IDC_QRCOLOR2_B), GetBValue(m_QREncodeColor2));
		Encode();
	}
}


BOOL CEncodeParent::PreTranslateMessage(MSG* pMsg)
{
	m_ToolTip.RelayEvent(pMsg);
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CEncodeParent::OnDeltaposSpinmodulesize(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	int num = pNMUpDown->iPos + pNMUpDown->iDelta;

	if (num >= 1 && num <= PIXEL_MAX_SIZE)
	{
		SetWindowInt(&m_editModuleSize, num);
		ShowImageSize();
		Encode();
	}

	*pResult = 0;
}


void CEncodeParent::OnReleasedcaptureSliderLogoWidth(NMHDR *pNMHDR, LRESULT *pResult)
{
	int num = m_sliderWidth.GetPos();
	m_fBorder1 = 0.5f * num / 100;
	Encode();
	ShowImageSize();
	*pResult = 0;
}


void CEncodeParent::OnReleasedcaptureSliderLogoHeight(NMHDR *pNMHDR, LRESULT *pResult)
{
	int num = m_sliderHeight.GetPos();
	m_fBorder2 = 0.5f * num / 100;
	Encode();
	ShowImageSize();
	*pResult = 0;
}


void CEncodeParent::OnBnClickedResetLogoSize()
{
	if (m_fBorder1 == 1.f/3 && m_fBorder2 == 1.f/3)
		return;
	m_fBorder1 = m_fBorder2 = 1.f/3;
	m_sliderWidth.SetPos(66);
	m_sliderHeight.SetPos(66);
	Encode();
	ShowImageSize();
}


void CEncodeParent::OnBnClickedClearLogo()
{
	if (m_pLogo->IsNull())
		return;
	m_pLogo->Destroy();
	m_LogoRect = CLogoRect();
	m_bHasALogo = FALSE;
	Encode();
	ShowImageSize();
}


void CEncodeParent::OnBnClickedEnableMixed()
{
	m_nEnableMixed = !m_nEnableMixed;
	m_buttonEnableMixed.SetCheck(m_nEnableMixed);
	Encode();
	ShowImageSize();
}


void CEncodeParent::OnBnClickedButtonEncode()
{
	Encode();
	ShowImageSize();
}


void CEncodeParent::OnBnClickedButtonSave()
{
	if (m_pImage != NULL && !m_pImage->IsNull())
	{
		m_pImage->Save(this, Gdiplus::ImageFormatBMP);
	}
}


void CEncodeParent::OnCbnSelchangeCombolevel2()
{
	m_nInnerEcLevel = m_comboInnerEcLevel.GetCurSel() + 1;
	UpdateecLevelTipText();
	Encode();
	ShowImageSize();
}


void CEncodeParent::OnCbnSelchangeCombomaskingno2()
{
	m_nInnerMaskNo = m_comboInnerMaskNo.GetCurSel();
	Encode();
	ShowImageSize();
}


void CEncodeParent::OnBnClickedCheckCompress()
{
	int nSel = m_BnCompress.GetCheck();
	if (nSel)
	{
		// ����ZLIBѹ��
		m_nInnerVersion = 31;
	}
	else
	{
		// ����Zlibѹ��
		m_nInnerVersion = 0;
	}
	Encode();
	ShowImageSize();
}


// ֪ͨ������ˢ��ͼ��
void CEncodeParent::Repaint() 
{
}