/**
* @file DlgQREncode.h
* @brief CDlgQREncode ��Ľӿ�
*/

#pragma once

#include "EncodeParent.h"

/**
* @class CDlgQREncode
* @brief CDlgQREncode �Ի���
*/
class CDlgQREncode : public CEncodeParent
{
public:
	// ����ͼ��
	virtual BOOL SetImage();
	// ˢ����ͼ
	virtual void Repaint();
};