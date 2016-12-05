#pragma once

/************************************************************************/
// @file		RS_ecc.h
// @brief		RS �������
// @version		1.0 
// @date		2016/9/14
// @note		���Կ�ƽ̨
// @author		Ԭ����
// 2016/8/26 �������ĵ�
/************************************************************************/

// 2016/9/21 ��ȡ�ַ����е�bits��
void GetBitsStream(BYTE *pSrc, int *pDest, int nCount);

// �����ã������ַ�ÿ��������λ������bits���棬�ӵ�λ����λ����
void Byte2Bits(BYTE ch, int bits[8]);

// �����ã�����������32��������λ(ǰn��)
void Int2Bits(int num, int bits[32], int nPrev = 32);

// ����8��bitλ��Ӧ��BYTE
int Bits2Byte(int bits[8]);

// ������ old�汾�����ַ�����4bits�ָ�
void SplitByteBy4Bits_old(BYTE* dataPtr, int dataLen, int* dst, int dstLen);

// ���ַ�����4bits�ָ�
void SplitByteBy4Bits(BYTE* dataPtr, int dataLen, int* dst, int dstLen);

// �����ݰ�4bitsλ���ϲ�
void MergedIntBy4Bits(int* dataSrc, int totalBits);

// ������ old�汾���ַ�����2bits�ָ�
void SplitByteBy2Bits_old(BYTE* dataPtr, int dataLen, int* dst, int dstLen);

// �ַ�����2bits�ָ�
void SplitByteBy2Bits(BYTE* dataPtr, int dataLen, int* dst, int dstLen);

// �����ݰ�2bitsλ���ϲ�
void MergedIntBy2Bits(int* dataSrc, int totalBits);

// ��ȡRS4����ı�������
int RS4BitsLen(int dataLen, int ecLevel);

// 4bit���Ŵ�С��RS�������
int* RS4Encode(BYTE* dataPtr, int dataLen, int &bitsLen, int ecLevel);

// 4bit���Ŵ�С��RS�������
int* RS4Decode(int* dataSrc, int dataLen, int ecLevel);

// 2bit���Ŵ�С��RS�������
void RS2Encode(BYTE *dataPtr, int dataLen, int *result);

// 2bit���Ŵ�С��RS�������
void RS2Decode(int *dataSrc, int dataLen, int *result);

// ����ͷ��2bit���Ŵ�С��RS�������
void RS2Encode16Bits(BYTE dataPtr[2], int result[48]);

// ��48λ����RS���룬�õ�2�ֽ�
void RS2Decode16Bits(int dataSrc[48], int result[16]);

// ����ͷ��4bit���Ŵ�С��RS�������
void RS2Encode14Bits(BYTE dataPtr[2], int result[42]);

// ��48λ����RS���룬�õ�14λ
void RS2Decode14Bits(int dataSrc[42], int result[14]);