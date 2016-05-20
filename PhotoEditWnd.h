#if !defined(AFX_PHOTOEDITWND_H__947D6769_A984_11D3_8C4C_00902722A6F0__INCLUDED_)
#define AFX_PHOTOEDITWND_H__947D6769_A984_11D3_8C4C_00902722A6F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PhotoEditWnd.h : header file
//
#include "PhotoEditDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CPhotoEditWnd window

class CPhotoEditWnd : public CWnd
{
// Construction
public:
	CPhotoEditWnd();

// Attributes
public:
	CPhotoEditDlg*		m_pEditDlg;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPhotoEditWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPhotoEditWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPhotoEditWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PHOTOEDITWND_H__947D6769_A984_11D3_8C4C_00902722A6F0__INCLUDED_)
