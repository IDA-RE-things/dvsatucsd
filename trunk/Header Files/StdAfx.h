// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#define _WIN32_WINNT 0x0501

#if !defined(AFX_STDAFX_H__7662D3E6_47AA_4F60_87E3_F2476CF6A25B__INCLUDED_)
#define AFX_STDAFX_H__7662D3E6_47AA_4F60_87E3_F2476CF6A25B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__7662D3E6_47AA_4F60_87E3_F2476CF6A25B__INCLUDED_)

/*++

Copyright (c) Microsoft Corporation. All rights reserved.

--*/

#ifndef __STDAFX__
#define __STDAFX__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN		
#endif //WIN32_LEAN_AND_MEAN

#include <tchar.h>
#include <stdio.h>
#include <windows.h>
#include <commctrl.h>
#include <shellapi.h>
#include <objbase.h>
#include <atlbase.h>
#include <wia.h>
#include <sti.h>
#include "gdiplus.h"

#define COUNTOF(x) ( sizeof(x) / sizeof(*x) )

#define DEFAULT_STRING_SIZE 256

extern HINSTANCE g_hInstance;

//{{AFX_INSERT_LOCATION}}

#endif //__STDAFX__

