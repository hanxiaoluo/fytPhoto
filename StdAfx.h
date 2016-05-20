// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__576AACFB_7202_42E1_9CA4_C09FEBC123F3__INCLUDED_)
#define AFX_STDAFX_H__576AACFB_7202_42E1_9CA4_C09FEBC123F3__INCLUDED_

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

#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }

void DrawDib(LPBITMAPINFO pInfo,LPBYTE pData,HDC hDC,LPRECT pDestRect,LPRECT pSrcRect);

#include "include/FreeImage.h"
#include "include/FreeImagePlus.h"

#ifndef _DEBUG
#pragma comment(lib,"lib/FreeImaged.lib")
#pragma comment(lib,"lib/FreeImagePlusd.lib")
#else
#pragma comment(lib,"lib/FreeImage.lib")
#pragma comment(lib,"lib/FreeImagePlus.lib")
#endif

#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;

#include <atlconv.h>

#define IDC_PHOTOEDITWND                40003


typedef struct _GimpRGB
{
  float r, g, b, a;
}GimpRGB;

typedef int   gint32;
typedef BYTE   guchar;
typedef USHORT guint16;
typedef bool gboolean;
typedef char gchar;

#define ICON_WIDTH  50
#define ICON_HEIGHT 50
#define LIST_ITEM_HEIGHT 66

#define	THUMBNAIL_WIDTH		80
#define	THUMBNAIL_WINDOW_WIDTH		140
#define	THUMBNAIL_HEIGHT	90

CRect CalcRect(const int nWidth, const int nHeight, CRect* pDestRect);
//#pragma comment(lib, "lib/MiscToolsD.lib")
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__576AACFB_7202_42E1_9CA4_C09FEBC123F3__INCLUDED_)
