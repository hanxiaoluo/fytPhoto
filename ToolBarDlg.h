#if !defined(AFX_TOOLBARDLG_H__62070F48_00FA_4819_9BE0_A87EAD26DDB9__INCLUDED_)
#define AFX_TOOLBARDLG_H__62070F48_00FA_4819_9BE0_A87EAD26DDB9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ToolBarDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CToolBarDlg dialog
/*#include "MiscTools/CatchScreenEdit.h"
#include "MiscTools/CatchScreenTrack.h"
#include "MiscTools/CatchScreenDlg.h"*/

class CToolBarDlg : public CDialog
{
// Construction
public:
	CToolBarDlg(CWnd* pParent = NULL);   // standard constructor

	bool m_bShowEdit;
// Dialog Data
	//{{AFX_DATA(CToolBarDlg)
	enum { IDD = IDD_DIALOG_TOOLBAR };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolBarDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CToolBarDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonOpen();
	afx_msg void OnButtonScreen();
	afx_msg void OnButtonEdit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLBARDLG_H__62070F48_00FA_4819_9BE0_A87EAD26DDB9__INCLUDED_)
