#pragma once

/************************************************************************
* @file DataMask.h
* @brief ���ζ�ά�������ģʽ - ����QR��İ�������.
* @author Ԭ����
* @date 2016/9/18
* 
************************************************************************/

/** 
* @class CDataMask
* @brief ����ģʽ�Ļ���. 
*/
class CDataMask
{
public:
	CDataMask() { }
	~CDataMask() { }
	/** 
	* @brief �ж��Ƿ���Ҫ�����������. 
	* @param[in] x ������
	* @param[in] y �����
	* @return ��������
	*/
	virtual BOOL IsMasked(int x, int y) { return 0; };
};


class CDataMask0 : public CDataMask
{
public:
	CDataMask0() { }
	~CDataMask0() { }
	virtual BOOL IsMasked(int x, int y) { return (x + y) % 2; }
};


class CDataMask1 : public CDataMask
{
public:
	CDataMask1() { }
	~CDataMask1() { }
	virtual BOOL IsMasked(int x, int y) { return y % 2; }
};


class CDataMask2 : public CDataMask
{
public:
	CDataMask2() { }
	~CDataMask2() { }
	virtual BOOL IsMasked(int x, int y) { return x % 3; }
};


class CDataMask3 : public CDataMask
{
public:
	CDataMask3() { }
	~CDataMask3() { }
	virtual BOOL IsMasked(int x, int y) { return (x + y) % 3; }
};


class CDataMask4 : public CDataMask
{
public:
	CDataMask4() { }
	~CDataMask4() { }
	virtual BOOL IsMasked(int x, int y) { return (x/3 + y/2) % 2; }
};


class CDataMask5 : public CDataMask
{
public:
	CDataMask5() { }
	~CDataMask5() { }
	virtual BOOL IsMasked(int x, int y) { return x*y % 2 + x*y % 3; }
};


class CDataMask6 : public CDataMask
{
public:
	CDataMask6() { }
	~CDataMask6() { }
	virtual BOOL IsMasked(int x, int y) { return (x*y % 2 + x*y % 3) % 2; }
};


class CDataMask7 : public CDataMask
{
public:
	CDataMask7() { }
	~CDataMask7() { }
	virtual BOOL IsMasked(int x, int y) { return (x*y % 3 + (x + y) % 2) % 2; }
};
