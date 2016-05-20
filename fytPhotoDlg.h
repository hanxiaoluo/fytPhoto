// fytPhotoDlg.h : header file
//

#if !defined(AFX_FYTPHOTODLG_H__1DAEBF8E_577B_417D_83B6_B8067D3CB211__INCLUDED_)
#define AFX_FYTPHOTODLG_H__1DAEBF8E_577B_417D_83B6_B8067D3CB211__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CFytPhotoDlg dialog
class CToolBarDlg;
class CViewDlg;
class CPhotoEditWnd;
class CEffectLibDlg;

class CFytPhotoDlg : public CDialog
{
// Construction
public:
	CFytPhotoDlg(CWnd* pParent = NULL);	// standard constructor
	~CFytPhotoDlg();

	CToolBarDlg* m_pToolbarDlg;
	CViewDlg* m_pViewDlg;
	CPhotoEditWnd* m_pPhotoEditWnd;
	CEffectLibDlg* m_pEffectDlg;

// Dialog Data
	//{{AFX_DATA(CFytPhotoDlg)
	enum { IDD = IDD_FYTPHOTO_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFytPhotoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	
	// Generated message map functions
	//{{AFX_MSG(CFytPhotoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FYTPHOTODLG_H__1DAEBF8E_577B_417D_83B6_B8067D3CB211__INCLUDED_)
