#if !defined(AFX_PHOTOEDITDLG_H__9FC4E7C5_33D5_4AAC_BE86_1FBED9979BBC__INCLUDED_)
#define AFX_PHOTOEDITDLG_H__9FC4E7C5_33D5_4AAC_BE86_1FBED9979BBC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PhotoEditDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPhotoEditDlg dialog

#include "layer/EditObject.h"
#include "resource.h"

class CPhotoEditDlg : public CDialog
{
// Construction
public:
	CPhotoEditDlg(CWnd* pParent = NULL);   // standard constructor
	~CPhotoEditDlg();
public:
	CImageObject* GetCurObject();
	VOID SetCurImageModified(BOOL bModified);
	EDITTYPE GetEditType();
// Dialog Data
	//{{AFX_DATA(CPhotoEditDlg)
	enum { IDD = IDD_DIALOG_EDIT_MAIN };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPhotoEditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CImageObject* m_pCurObject;
	BOOL m_bCurImageModified;
	EDITTYPE m_iEditType;

	// Generated message map functions
	//{{AFX_MSG(CPhotoEditDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PHOTOEDITDLG_H__9FC4E7C5_33D5_4AAC_BE86_1FBED9979BBC__INCLUDED_)
