#pragma once

#include "afxwin.h"
#include "afxcmn.h"
#include "Resource.h"

#include "CyImage.h"
#include "QR_Encode.h"
#include <vector>
using namespace std;

// CEncodeParent �Ի���

#define MIXED_RATE				0.5 //��ϱ���

#define BOUNDING_MODE_REPLACED	0	//�����

#define BOUNDING_MODE_MIXED		1	//��ϰ�

#define PIXEL_MAX_SIZE			32	//��������С

class CEncodeParent : public CDialogEx
{
	DECLARE_DYNAMIC(CEncodeParent)

public:
	CEncodeParent(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CEncodeParent();

	CyImage*						m_pImage;				//��ά��
	CyImage*						m_pLogo;				//��ʶ
	// ������ɫ
	COLORREF						m_BackgroundColor;		//����ɫ
	COLORREF						m_ForegroundColor;		//ǰ��ɫ
	COLORREF						m_QREncodeColor1;		//��Ԫһ
	COLORREF						m_QREncodeColor2;		//��Ԫ��
	// ����/���� ��С
	int								m_nSymbleSize;			//���ųߴ�
	int								m_nPixelSize;			//���ش�С
	int								m_nEnableMixed;			//������ϰ�
	BOOL							m_bHasALogo;			//ӵ��LOGO
	CLogoRect						m_LogoRect;				//logo�������
	CLogoRect						m_realLogoRect;			//��ʵ��logo����
	vector<CPixelPoint>				m_ForegroundElemsIndex;	//ǰ��ɫ������
	int								m_nInnerEcLevel;		//RS��������
	int								m_nInnerMaskNo;			//��ɫ����汾
	int								m_nInnerVersion;		//��ɫ����汾

public:
	// �鷽��
	virtual BOOL SetImage();								// ����ͼ��
	virtual BOOL Encode();									// ��������
	virtual BOOL EncodePrivateData(CQR_Encode* pQR_Encode);	// ��������
	virtual void InitInnerEncodingParameters();				// ��ʼ���ڲ�������
	virtual void UpdateecLevelTipText();					// ���»�������Ϣ��ʾ
	afx_msg virtual void OnCbnSelchangeCombolevel2();		// �ı����ȼ�ʱ����
	virtual void Addlogo();									// ��ӱ�ʶ
															// ��ʼ��logo��ͼ��
	virtual void InitLogoRect(float border1 = 1 / 2.7f, float border2 = 1 / 2.7f);

	// ����logo��С
	float							m_fBorder1;
	float							m_fBorder2;

	// ��ʾ��
	CToolTipCtrl					m_ToolTip;				// ������ʾ��
	void ShowImageSize();									// ��ʾ��ǰͼ���С
	BOOL IsColorsEncodeAble();								// ��ɫ�Ƿ���Ա���
	virtual void Repaint();									// ���»���/ˢ����ͼ

	// �Ի�������
	enum { IDD = IDD_QRENCODE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeCombolevel();
	afx_msg void OnCbnSelchangeComboversion();
	afx_msg void OnBnClickedCheckautoextent();
	afx_msg void OnCbnSelchangeCombomaskingno();
	afx_msg void OnEnChangeEditmodulesize();
	afx_msg void OnBnClickedImportLogo();
	afx_msg void OnEnChangeEditsourcedataPublic();
	afx_msg void OnEnChangeEditsourcedataPrivate();
	// �ı���ɫ
	afx_msg void OnEnChangeForegroundR();
	afx_msg void OnEnChangeForegroundG();
	afx_msg void OnEnChangeForegroundB();
	afx_msg void OnEnChangeBackgroundR();
	afx_msg void OnEnChangeBackgroundG();
	afx_msg void OnEnChangeBackgroundB();
	afx_msg void OnEnChangeEnCodeColor1R();
	afx_msg void OnEnChangeEnCodeColor1G();
	afx_msg void OnEnChangeEnCodeColor1B();
	afx_msg void OnEnChangeEnCodeColor2R();
	afx_msg void OnEnChangeEnCodeColor2G();
	afx_msg void OnEnChangeEnCodeColor2B();
	CComboBox m_comboLevel;
	CComboBox m_comboVersion;
	CStatic m_staticVersion;
	CButton m_checkAutoExtent;
	CComboBox m_comboMaskingNo;
	CStatic m_staticMaskingNo;
	CEdit m_editModuleSize;
	CSpinButtonCtrl m_spinModuleSize;
	CStatic m_staticBmpSize;
	CEdit m_editSourceDataPublic;
	CEdit m_editSourceDataPrivate;
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnBnClickedBnForeground();
	afx_msg void OnBnClickedBnBackground();
	afx_msg void OnBnClickedBnEncodeColor1();
	afx_msg void OnBnClickedBnEncodeColor2();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDeltaposSpinmodulesize(NMHDR *pNMHDR, LRESULT *pResult);
	CSliderCtrl m_sliderWidth;
	CSliderCtrl m_sliderHeight;
	afx_msg void OnReleasedcaptureSliderLogoWidth(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnReleasedcaptureSliderLogoHeight(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedResetLogoSize();
	afx_msg void OnBnClickedClearLogo();
	afx_msg void OnBnClickedEnableMixed();
	CButton m_buttonEnableMixed;
	afx_msg void OnBnClickedButtonEncode();
	afx_msg void OnBnClickedButtonSave();
	CStatic m_staticQRCodeSize;
	CComboBox m_comboInnerEcLevel;
	CComboBox m_comboInnerMaskNo;
	afx_msg void OnCbnSelchangeCombomaskingno2();
	afx_msg void OnBnClickedCheckCompress();
	CButton m_BnCompress;
};