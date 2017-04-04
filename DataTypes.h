#pragma once

/** 
* @file DataTypes.h
* @brief ���ĵ������˳�������õ�����������
* @note �����Ҫ��ANDROIDʹ�ã���#define ANDROID_CPP
* @date 2016/8/24 ע
*/

#include <math.h>
typedef int BOOL;
typedef unsigned char BYTE;
typedef unsigned char uchar;
typedef BYTE *LPBYTE;
typedef unsigned int uint;

#define TRUE 1
#define FALSE 0

#ifndef NULL
#define NULL 0
#endif

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))					/**< ���ֵ */
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))					/**< ��Сֵ */
#endif

#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4)					/**< ����ÿ���ֽ���(4�ֽڶ���) */

#ifndef SAFE_DELETE
	#define SAFE_DELETE(p) if((p) != NULL){ delete [] (p); (p) = NULL; }/**< ��ȫɾ��ָ��p  */
#endif

#ifndef RGB
/// RGB
#define RGB(r,g,b)          ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))
#endif

#ifndef ZeroMemory
/// ����
#define ZeroMemory(Destination,Length) memset((Destination),0,(Length))
#endif

#define PI 3.141592653f /**< PI */
#define QR_MARGIN	4	/**< QR�߽� */

#ifdef ANDROID_CPP		/**< ����ڰ�װ�±��룬��Ҫ����ĳЩ�ļ� */

#include "stdlib.h"
#include "string.h"

#define TRACE printf

typedef unsigned short      WORD;
typedef unsigned long       DWORD;
typedef unsigned long		COLORREF;
typedef unsigned long		ULONG_PTR;
typedef ULONG_PTR			DWORD_PTR;
#define LOBYTE(w)           ((BYTE)(((DWORD_PTR)(w)) & 0xff))
#define GetRValue(rgb)      (LOBYTE(rgb))
#define GetGValue(rgb)      (LOBYTE(((WORD)(rgb)) >> 8))
#define GetBValue(rgb)      (LOBYTE((rgb)>>16))

#endif // ANDROID_CPP

/** 
* @class float2
* @brief ��ά��������(x, y)
*/
class float2
{
public:
	float x, y;
public:
	float2() : x(0), y(0)
	{
	}
	float2(float a, float b) : x(a), y(b)
	{
	}
	// =
	float2 operator = (const float2& point)
	{
		x = point.x;
		y = point.y;
		return *this;
	}
	float2 operator = (const float val)
	{
		x = val;
		y = val;
		return *this;
	}
	// -
	float2 operator - ()
	{
		x = -x;
		y = -y;
		return *this;
	}
	// ==
	bool operator == (const float2 & point)
	{
		return (x == point.x && y == point.y);
	}
	bool operator == (const float val)
	{
		return (x == val && y == val);
	}
	// !=
	bool operator != (const float2 & point)
	{
		return (x != point.x || y != point.y);
	}
	bool operator != (const float val)
	{
		return (x != val || y != val);
	}
	// += -= *= /=
	float2 operator += (const float2 & pos)
	{
		x += pos.x;
		y += pos.y;
		return *this;
	}
	float2 operator -= (const float2 & pos)
	{
		x -= pos.x;
		y -= pos.y;
		return *this;
	}
	float2 operator *= (const float2 & pos)
	{
		x *= pos.x;
		y *= pos.y;
		return *this;
	}
	float2 operator /= (const float2 & pos)
	{
		x /= pos.x;
		y /= pos.y;
		return *this;
	}
	float2 operator += (const float val)
	{
		x += val;
		y += val;
		return *this;
	}
	float2 operator -= (const float val)
	{
		x -= val;
		y -= val;
		return *this;
	}
	float2 operator *= (const float val)
	{
		x *= val;
		y *= val;
		return *this;
	}
	float2 operator /= (const float val)
	{
		x /= val;
		y /= val;
		return *this;
	}
	// ��������(1)
	friend float2 operator + (const float2 & pos, const float val)
	{
		return float2(pos.x + val, pos.y + val);
	}
	friend float2 operator - (const float2 & pos, const float val)
	{
		return float2(pos.x - val, pos.y - val);
	}
	friend float2 operator * (const float2 & pos, const float val)
	{
		return float2(pos.x * val, pos.y * val);
	}
	friend float2 operator / (const float2 & pos, const float val)
	{
		return float2(pos.x / val, pos.y / val);
	}
	// ��������(2)
	friend float2 operator + (const float val, const float2 & pos)
	{
		return float2(val + pos.x, val + pos.y);
	}
	friend float2 operator - (const float val, const float2 & pos)
	{
		return float2(val - pos.x, val - pos.y);
	}
	friend float2 operator * (const float val, const float2 & pos)
	{
		return float2(val * pos.x, val * pos.y);
	}
	friend float2 operator / (const float val, const float2 & pos)
	{
		return float2(val / pos.x, val / pos.y);
	}
	// ��������
	friend float2 operator + (const float2 & pos1, const float2 & pos2)
	{
		return float2(pos1.x + pos2.x, pos1.y + pos2.y);
	}
	friend float2 operator - (const float2 & pos1, const float2 & pos2)
	{
		return float2(pos1.x - pos2.x, pos1.y - pos2.y);
	}
	friend float2 operator * (const float2 & pos1, const float2 & pos2)
	{
		return float2(pos1.x * pos2.x, pos1.y * pos2.y);
	}
	friend float2 operator / (const float2 & pos1, const float2 & pos2)
	{
		return float2(pos1.x / pos2.x, pos1.y / pos2.y);
	}
	friend float2 abs(const float2 & point)
	{
		return float2(abs(point.x), abs(point.y));
	}
};


/** 
* @class float3
* @brief ��ά��������(x, y. z)
*/
class float3
{
public:
	float x, y, z;
public:
	float3() : x(0), y(0), z(0)
	{
	}
	float3(float a, float b, float c) : x(a), y(b), z(c)
	{
	}
	float3(const float3 & other) : x(other.x), y(other.y), z(other.z)
	{
	}
	// =
	float3 operator = (const float3& other)
	{
		x = other.x;
		y = other.y;
		z = other.z;
		return *this;
	}
	float3 operator = (const float val)
	{
		x = val;
		y = val;
		z = val;
		return *this;
	}
	// -
	float3 operator - ()
	{
		x = -x;
		y = -y;
		z = -z;
		return *this;
	}
	// ==
	bool operator == (const float3& other)
	{
		return (x == other.x && y == other.y && z == other.z);
	}
	bool operator == (const float val)
	{
		return (x == val && y == val && z == val);
	}
	// !=
	bool operator != (const float3& other)
	{
		return (x != other.x || y != other.y || z != other.z);
	}
	bool operator != (const float val)
	{
		return (x != val || y != val || z != val);
	}
	// += -= *= /=
	float3 operator += (const float3& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}
	float3 operator -= (const float3& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}
	float3 operator *= (const float3& other)
	{
		x *= other.x;
		y *= other.y;
		z *= other.z;
		return *this;
	}
	float3 operator /= (const float3& other)
	{
		x /= other.x;
		y /= other.y;
		z /= other.z;
		return *this;
	}
	float3 operator += (const float other)
	{
		x += other;
		y += other;
		z += other;
		return *this;
	}
	float3 operator -= (const float other)
	{
		x -= other;
		y -= other;
		z -= other;
		return *this;
	}
	float3 operator *= (const float other)
	{
		x *= other;
		y *= other;
		z *= other;
		return *this;
	}
	float3 operator /= (const float other)
	{
		x /= other;
		y /= other;
		z /= other;
		return *this;
	}
	// �븡������(1)
	friend float3 operator + (const float3 & point, float val)
	{
		return float3(point.x + val, point.y + val, point.z + val);
	}
	friend float3 operator - (const float3 & point, float val)
	{
		return float3(point.x - val, point.y - val, point.z - val);
	}
	friend float3 operator * (const float3 & point, float val)
	{
		return float3(point.x * val, point.y * val, point.z * val);
	}
	friend float3 operator / (const float3 & point, float val)
	{
		return float3(point.x / val, point.y / val, point.z / val);
	}
	// �븡������(2)
	friend float3 operator + (float val, const float3 & point)
	{
		return float3(val + point.x, val + point.y, val + point.z);
	}
	friend float3 operator - (float val, const float3 & point)
	{
		return float3(val - point.x, val - point.y, val - point.z);
	}
	friend float3 operator * (float val, const float3 & point)
	{
		return float3(val * point.x, val * point.y, val * point.z);
	}
	friend float3 operator / (float val, const float3 & point)
	{
		return float3(val / point.x, val / point.y, val / point.z);
	}
	// ��������
	friend float3 operator + (const float3 & P, const float3 & Q)
	{
		return float3(P.x + Q.x, P.y + Q.y, P.z + Q.z);
	}
	friend float3 operator - (const float3 & P, const float3 & Q)
	{
		return float3(P.x - Q.x, P.y - Q.y, P.z - Q.z);
	}
	friend float3 operator * (const float3 & P, const float3 & Q)
	{
		return float3(P.x * Q.x, P.y * Q.y, P.z * Q.z);
	}
	friend float3 operator / (const float3 & P, const float3 & Q)
	{
		return float3(P.x / Q.x, P.y / Q.y, P.z / Q.z);
	}
	friend float3 abs(const float3 & point)
	{
		return float3(abs(point.x), abs(point.y), abs(point.z));
	}
};

/// RGBת�Ҷ�(@see http://blog.csdn.net/cool1949/article/details/6649429)
#define RGB2GRAY(R, G, B) (  ((R)*38 + (G)*75 + (B)*15) >> 7 )

/// RGB�꣺COLORREFתΪ�Ҷ�ֵ
#define RgbColorRef2Gray(ref)			( (GetRValue(ref)*38 + GetGValue(ref)*75 + GetBValue(ref)*15) >> 7 )

/// RGB�꣺rgbתΪfloat3����
#define RgbColor2Float3(r, g, b)		float3(r, g, b)

/// ��COLORREFתΪfloat3����
#define RgbColorRef2Float3(ref)			float3(GetRValue(ref), GetGValue(ref), GetBValue(ref))

/// float3����ת�Ҷ�
#define RgbColor2Gray(rgbc)				(0.299f * rgbc.x + 0.587f * rgbc.y + 0.114f * rgbc.z)

#define BIT_SET(a, b) ((a) |= (1<<(b)))			/**< ����a��bλΪ1 */
#define BIT_CLEAR(a, b) ((a) &= ~(1<<(b)))		/**< ����a��bλΪ0 */
#define BIT_FLIP(a, b) ((a) ^= (1<<(b)))		/**< ����a��bλ */
#define BIT_CHECK(a, b) ((a) & (1<<(b)))		/**< ���a��bλ */

/// ���a��iλΪ1��������b��jλΪ1
#define BIT_CHECK_SET(a, i, b, j) if(BIT_CHECK(a, i)) BIT_SET(b, j)

/// �������b��jλΪ1��ʹ1�Ը���p����
#define BIT_RANDOM_SET(p, b, j) if(1.0F * rand() / RAND_MAX <= p) BIT_SET(b, j)

/** 
* @class CMyRect
* @brief ������
*/
class CMyRect
{
public:
	CMyRect() : left(0), top(0), right(0), bottom(0)
	{
	}
	CMyRect(int l, int t, int r, int b) : left(l), top(t), right(r), bottom(b)
	{
	}
	CMyRect(int nC) : left(nC), top(nC), right(nC), bottom(nC)
	{
	}
	~CMyRect()
	{
	}
	int Width() const 
	{
		return right - left;
	}
	int Height() const 
	{
		return bottom - top;
	}
	bool operator == (const CMyRect & rect) const 
	{
		return (left == rect.left && right == rect.right
			&& top == rect.top && bottom == rect.bottom);
	}
	bool IsEmpty() const 
	{
		return (left == 0 && right == 0 && top == 0 && bottom == 0);
	}

public:
	int left, top, right, bottom;
};

/// ͼ�����
typedef CMyRect CLogoRect;

/// ����Ȥ����
typedef CMyRect RoiRect;

/** 
* @class CPixelPoint
* @brief ����
*/
class CPixelPoint
{
public:
	CPixelPoint() : x(0), y(0)
	{
	}
	CPixelPoint(int xPos, int yPos) : x(xPos), y(yPos)
	{
	}
	~CPixelPoint()
	{
	}
	// ����x����Ƚ��������С(P1>P2)
	friend bool operator > (const CPixelPoint &P1, const CPixelPoint &P2)
	{
		return (P1.x > P2.x);
	}
public:
	int x, y;
};

/// rgb(r, g, b, a)תint�����ڱȽϴ�С
#define RGB2INT(rgba) (299 * BYTE((rgba).r) + 587 * BYTE((rgba).g) + 114 * BYTE((rgba).b))

/** 
* @class rgb
* @brief ��ɫ(r, g, b)
*/
template <typename Type>
class rgb
{
public:
	Type r, g, b, a; // a������ʹ4�ֽڶ���
	rgb() : r(0), g(0), b(0), a(0)
	{
	}
	rgb(Type R, Type G, Type B) : r(R), g(G), b(B), a(0)
	{
	}
	rgb(const rgb & in) : r(in.r), g(in.g), b(in.b)
	{
	}
	// �Ƚ�����rgb���صĴ�С(C1<C2)
	friend bool operator < (const rgb<Type> & C1, const rgb<Type> & C2)
	{
		return RGB2INT(C1) < RGB2INT(C2);
	}
	// ��������rgb�ṹ��
	friend void Swap(rgb<Type> & C1, rgb<Type> & C2)
	{
		Type t;				
		t = C1.r; C1.r = C2.r; C2.r = t;
		t = C1.g; C1.g = C2.g; C2.g = t;
		t = C1.b; C1.b = C2.b; C2.b = t;
	}
	// ��rgb�ṹ���в�������
	friend void Sort(rgb<Type> *pArray, int Num)
	{
		for (int i = 1; i < Num; ++i)
		{
			int Aj = RGB2INT(pArray[i]);// A[j]
			int Aj_1 = RGB2INT(pArray[i - 1]);// A[j-1]
			for (int j = i; j > 0 && Aj < Aj_1; )
			{
				Type t;
				t = pArray[j].r; pArray[j].r = pArray[j - 1].r; pArray[j - 1].r = t;
				t = pArray[j].g; pArray[j].g = pArray[j - 1].g; pArray[j - 1].g = t;
				t = pArray[j].b; pArray[j].b = pArray[j - 1].b; pArray[j - 1].b = t;
				--j;
				Aj = Aj_1;
				Aj_1 = RGB2INT(pArray[j - 1]);
			}
		}
	}
};

/** 
* @struct ImageHeader
* @brief ͼ�����Ϣͷ
* @details �����ݽṹ�����ͼ�����Ҫ��Ϣ, ͼ���ע������Ϣ.
* @var ImageHeader::m_sTag - ͼ���ʶ
* @var ImageHeader::m_nWidth - ͼ����
* @var ImageHeader::m_nHeight - ͼ��߶�
* @var ImageHeader::m_nRowlen - ͼ��ÿ��Ԫ�ظ���
* @var ImageHeader::m_nChannel - ͼ��ÿ����Ԫ����
* 
* @author yuanyuanxiang
* @version 1.0
* @date 2016/9/12
* 
*/
struct ImageHeader
{
	/// �ļ�ͷ��ʶ
	char	m_sTag[32];
	/// ͼ����
	int		m_nWidth;
	/// ͼ��߶�
	int		m_nHeight;
	/// ͼ��ÿ��Ԫ�ظ���
	int		m_nRowlen;
	/// ͼ��ÿ����Ԫ�ظ���
	int		m_nChannel;
};

// �ж�����(i��, j��)���ھ�����
BOOL IndexNotInRect(int i, int j, const CLogoRect &rect);

// ��ά�������ľ���
float Distance(const float2 & P, const float2 & Q);

// ��ά�������ľ���ƽ��
float _Distance(const float2 & P, const float2 & Q);

// ��ά�������ľ���
float Distance(const float3 & P, const float3 & Q);

// ��ά�������ľ���ƽ��
float _Distance(const float3 & P, const float3 & Q);

// �����ΰ������Ŵ�
void ZoomRect(CLogoRect &rect, float rate = 1.f);

// ������(dx, dy)�ƶ�����
void MoveRect(CLogoRect &rect, float dx = 0.f, float dy = 0.f);

// �����ݽ����������
void XOR(int *nCodes, const int *nMaskingNo, int nLength);

// �ж�ͼ���Ƿ�4�ֽڶ���
BOOL IsAligned(int nWidth, int nChannel);

// ����2D�����ģ
float fabs(const float2 & pos);

// ����3D�����ģ
float fabs(const float3 & pos);