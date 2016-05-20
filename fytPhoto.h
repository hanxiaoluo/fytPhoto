// fytPhoto.h : main header file for the FYTPHOTO application
//

#if !defined(AFX_FYTPHOTO_H__7671C123_4404_4DF0_8E83_26B469584654__INCLUDED_)
#define AFX_FYTPHOTO_H__7671C123_4404_4DF0_8E83_26B469584654__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CFytPhotoApp:
// See fytPhoto.cpp for the implementation of this class
//

class CFytPhotoApp : public CWinApp
{
public:
	CFytPhotoApp();
	ULONG_PTR m_gdiToken;  

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFytPhotoApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CFytPhotoApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FYTPHOTO_H__7671C123_4404_4DF0_8E83_26B469584654__INCLUDED_)
