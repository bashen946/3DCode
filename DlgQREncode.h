#pragma once

#include "EncodeParent.h"

// CDlgQREncode �Ի���

class CDlgQREncode : public CEncodeParent
{
public:
	// ����ͼ��
	virtual BOOL SetImage();
	// ˢ����ͼ
	virtual void Repaint();
};