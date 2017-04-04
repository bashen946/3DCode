/**
* @file EncodeParent.h
* @brief CEncodeParent ��Ľӿ�
*/

#pragma once

#include "afxwin.h"
#include "afxcmn.h"
#include "Resource.h"

#include "CyImage.h"
#include "QR_Encode.h"
#include <vector>
using namespace std;

// CEncodeParent �Ի���

#define MIXED_RATE				0.5 /**< ��ϱ��� */

#define BOUNDING_MODE_REPLACED	0	/**< ����� */

#define BOUNDING_MODE_MIXED		1	/**< ��ϰ� */

#define PIXEL_MAX_SIZE			32	/**< ��������С */

/**
* @class CEncodeParent
* @brief ���봰�ڵĸ���
*/
class CEncodeParent : public CDialogEx
{
	DECLARE_DYNAMIC(CEncodeParent)

public:
	CEncodeParent(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CEncodeParent();

	CyImage*						m_pImage;				/**< ��ά�� */
	CyImage*						m_pLogo;				/**< ��ʶ */
	//  ������ɫ
	COLORREF						m_BackgroundColor;		/**< ����ɫ */
	COLORREF						m_ForegroundColor;		/**< ǰ��ɫ */
	COLORREF						m_QREncodeColor1;		/**< ��Ԫһ */
	COLORREF						m_QREncodeColor2;		/**< ��Ԫ�� */
	// ����/���� ��С
	int								m_nSymbleSize;			/**< ���ųߴ� */
	int								m_nPixelSize;			/**< ���ش�С */
	int								m_nEnableMixed;			/**< ������ϰ� */
	BOOL							m_bHasALogo;			/**< ӵ��LOGO */
	CLogoRect						m_LogoRect;				/**< logo������� */
	CLogoRect						m_realLogoRect;			/**< ��ʵ��logo���� */
	vector<CPixelPoint>				m_ForegroundElemsIndex;	/**< ǰ��ɫ������ */
	int								m_nInnerEcLevel;		/**< RS�������� */
	int								m_nInnerMaskNo;			/**< ��ɫ����汾 */
	int								m_nInnerVersion;		/**< ��ɫ����汾 */

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
	float							m_fBorder1;				/**< ����LOGO��С�Ĳ��� */
	float							m_fBorder2;				/**< ����LOGO��С�Ĳ��� */

	// ��ʾ��
	CToolTipCtrl					m_ToolTip;				/**< ������ʾ�� */
	CComboBox						m_comboLevel;			/**< ����ȼ� */
	CComboBox						m_comboVersion;			/**< ����汾 */
	CStatic							m_staticVersion;		/**< ��ʾ����汾 */
	CButton							m_checkAutoExtent;		/**< �Զ���չ */
	CComboBox						m_comboMaskingNo;		/**< ����汾 */
	CStatic							m_staticMaskingNo;		/**< ��ʾ����汾 */
	CEdit							m_editModuleSize;		/**< ģ���С�༭�� */
	CSpinButtonCtrl					m_spinModuleSize;		/**< ģ���СSpin */
	CStatic							m_staticBmpSize;		/**< ��ʾλͼ��С */
	CEdit							m_editSourceDataPublic;	/**< �������� */
	CEdit							m_editSourceDataPrivate;/**< ˽������ */
	CSliderCtrl						m_sliderWidth;			/**< ��Ȼ����� */
	CSliderCtrl						m_sliderHeight;			/**< �߶Ȼ����� */
	CButton							m_buttonEnableMixed;	/**< ������� */
	CStatic							m_staticQRCodeSize;		/**< ��ʵ��ά���С */
	CComboBox						m_comboInnerEcLevel;	/**< �ڲ����ȼ� */
	CComboBox						m_comboInnerMaskNo;		/**< �ڲ�����汾 */
	CButton							m_BnCompress;			/**< ����ѹ������ */

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
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnBnClickedBnForeground();
	afx_msg void OnBnClickedBnBackground();
	afx_msg void OnBnClickedBnEncodeColor1();
	afx_msg void OnBnClickedBnEncodeColor2();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDeltaposSpinmodulesize(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnReleasedcaptureSliderLogoWidth(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnReleasedcaptureSliderLogoHeight(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedResetLogoSize();
	afx_msg void OnBnClickedClearLogo();
	afx_msg void OnBnClickedEnableMixed();
	afx_msg void OnBnClickedButtonEncode();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnCbnSelchangeCombomaskingno2();
	afx_msg void OnBnClickedCheckCompress();
};