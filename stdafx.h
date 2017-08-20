
// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

// ��MFC��������Ӵ��Ԥ������
#ifndef NO_STDAFX_H

#ifdef USING_VLD
#include "vld/include/vld.h" // should appear before '#include <afxwin.h>'
#pragma comment(lib, "vld/lib/Win32/vld.lib")
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ


#include <afxdisp.h>        // MFC �Զ�����



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // �������Ϳؼ����� MFC ֧��

#define SAFE_DELETE(p) if((p) != NULL){ delete [] (p); (p) = NULL; }/**< ��ȫɾ��ָ��p */

#define SAFE_NEW(type, p) if ((p) == NULL) { (p) = new type; }		/**< ����pΪ��ʱ������ڴ� */

#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4)				/**< ����ÿ���ֽ��� */

#undef ANDROID_CPP

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

#else

// DEBUG_NEW
#define DEBUG_NEW new

// ASSERT
#include <assert.h>
#define ASSERT assert

#ifdef USING_VLD
#include "vld/include/vld.h"
#pragma comment(lib, "vld/lib/Win32/vld.lib")
#endif

// DataTypes
#include "DataTypes.h"

#endif // STDAFX_H