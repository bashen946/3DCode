/************************************************************************
* @file 
* @details ͸�ӱ任�ࣺ����ο���ZXing - PerspectiveTransform
* @author 
* @email 
* @version V1.0
* @date 2016/10/4
* Copyleft (c), All rights reserved.
/************************************************************************/

#pragma once
#include <vector>
#include "DataTypes.h"

/**
* @class PerspectTransform - ͸��У����
*/
class PerspectTransform
{
private:
	/// ͸�ӱ任����
	float a11, a12, a13, a21, a22, a23, a31, a32, a33;
	/// Ĭ�Ϲ��캯��
	PerspectTransform();
	/// ���캯��
	PerspectTransform(float a11, float a21, float a31, float a12, float a22, float a32, float a13, float a23, float a33);

public:
	/// �ı��ε��ı��α任
	static PerspectTransform 
		quadrilateralToQuadrilateral(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3,
		float x0p, float y0p, float x1p, float y1p, float x2p, float y2p, float x3p, float y3p);
	/// �����ε��ı��α任
	static PerspectTransform 
		squareToQuadrilateral(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3);
	/// �ı��ε������α任
	static PerspectTransform 
		quadrilateralToSquare(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3);
	/// �����������
	PerspectTransform buildAdjoint();
	/// ����˷�
	PerspectTransform times(PerspectTransform & other);
	/// ��任
	void transformPoints(std::vector<float> &points);

	/// ��ȡ͸�Ӿ���
	void GetPptMatrix(float ppt[9]);

	/// ��ȡ͸�Ӿ����Ԫ��
	void GetPptMatrix(	float &a11, float &a12, float &a13, 
						float &a21, float &a22, float &a23, 
						float &a31, float &a32, float &a33);
};


/// ����һ��͸�ӱ任
PerspectTransform createTransform(float2 pos[4], int dimension, float fmodulesize);

/// ����͸�ӱ任
BYTE *PerspectiveTransform(BYTE* pSrc, int nWidth, int nHeight, int nRowlen, int nChannel, 
						   PerspectTransform & transform, 
						   float2 m_fPos[4], int nSymbolSize, int nModulesize, int &nDstSize);