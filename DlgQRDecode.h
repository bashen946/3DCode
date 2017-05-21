/**
* @file DlgQRDecode.h
* @brief CDlgQRDecode ��Ľӿ�
*/

#pragma once
#include "afxwin.h"
#include "CyImage.h"
#include "DecodeFuncs.h"

/**
* @class CDlgQRDecode
* @brief CDlgQRDecode �Ի���
*/
class CDlgQRDecode : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgQRDecode)

public:
	CDlgQRDecode(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgQRDecode();

	CyImage*			m_pImage;			/**<  ��ǰͼ�� */
	RoiRect				m_roi;				/**<  ����Ȥ���� */
	CString				m_strPublicString;	/**<  ������Ϣ */
	CString				m_strPrivateString; /**<  ������Ϣ */
	BarCodeInfo			qr;
	BarCodeInfo			inner;
	CToolTipCtrl		m_ToolTip;			/**<  ������ʾ */

	// ������ɫ
	COLORREF			m_BackgroundColor;		/**< ����ɫ */

	// �������
	BOOL				m_bUseHybrid;			/**< �����ֵ�� */
	BOOL				m_bTryHarder;			/**< ǿ������ */

	CButton				m_checkUseHybrid;		/**< �����ֵ����ť */
	CButton				m_checkTryHarder;		/**< ǿ��������ť */

	void SetImage();						// ���õ�ǰͼ��
	BOOL Decode();							// ����ͼ��
	void UpdateDecodeInfo();				// ���½�����Ϣ
	void UpdateecLevelTipText(int nEcLevel);// ������Ϣ��ʾ
	void Reset();							// �������пؼ�

// �Ի�������
	enum { IDD = IDD_QRDECODE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonDecode();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedButtonSaveImage();
	afx_msg void OnBnClickedBnBackground();
	afx_msg void OnBnClickedButtonClear();
};