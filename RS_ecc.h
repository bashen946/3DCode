#pragma once

/**
* @file			RS_ecc.h
* @brief		RS �������
* @version		1.0 
* @date			2016/9/14
* @note			���Կ�ƽ̨
* @author		Ԭ����
* 2016/8/26		�������ĵ�
* 2017/4/4		�޸Ĵ��ĵ���ɾ��������
*/

// ��01��������ת
void InverseBitsStream(int *pBits, int nLength);

// ��ȡ�ַ����е�bits��
void GetBitsStream(const BYTE *pSrc, int *pDest, int nCount);

// ����8��bitλ��Ӧ��BYTE
int Bits2Byte(const int bits[8]);

// ���ַ�����4bits�ָ�
void SplitByteBy4Bits(const BYTE* dataPtr, int dataLen, int* dst, int dstLen);

// �����ݰ�4bitsλ���ϲ�
void MergedIntBy4Bits(int* dataSrc, int totalBits);

// �ַ�����2bits�ָ�
void SplitByteBy2Bits(const BYTE* dataPtr, int dataLen, int* dst, int dstLen);

// �����ݰ�2bitsλ���ϲ�
void MergedIntBy2Bits(int* dataSrc, int totalBits);

// ��ȡRS4����ı�������
int RS4BitsLen(int dataLen, int ecLevel, int & bitsLen);

// 4bit���Ŵ�С��RS�������
int* RS4Encode(const BYTE* dataPtr, int dataLen, int bitsLen, int ecLevel);

// 4bit���Ŵ�С��RS�������
int* RS4Decode(int* dataSrc, int dataLen, int bitsLen, int ecLevel);

// 2bit���Ŵ�С��RS�������
void RS2Encode(const BYTE *dataPtr, int dataLen, int *result);

// 2bit���Ŵ�С��RS�������
bool RS2Decode(const int *dataSrc, int dataLen, int *result);