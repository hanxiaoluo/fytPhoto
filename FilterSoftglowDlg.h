#if !defined(AFX_FILTERSOFTGLOWDLG_H__E92FB356_1C51_4875_9F58_CA35CE0CB5D4__INCLUDED_)
#define AFX_FILTERSOFTGLOWDLG_H__E92FB356_1C51_4875_9F58_CA35CE0CB5D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FilterSoftglowDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFilterSoftglowDlg dialog

#include "LineTrackBar.h"

class CFilterSoftglowDlg : public CDialog
{
// Construction
public:
	CFilterSoftglowDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFilterSoftglowDlg)
	enum { IDD = IDD_FILTER_SOFTGLOW };
	CSignEdit	m_Edit3;
	CSignEdit	m_Edit2;
	CSignEdit	m_Edit1;
	CString	m_strValue1;
	CString	m_strValue2;
	CString	m_strValue3;
	//}}AFX_DATA

	CLineTrackBar m_wndLineTrackBar1;
	CLineTrackBar m_wndLineTrackBar2;
	CLineTrackBar m_wndLineTrackBar3;


	fipWinImage m_image;
	fipWinImage m_imageDest;
	BYTE* m_pData;
	BITMAPINFO* m_pInfo;
	void softglow();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFilterSoftglowDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFilterSoftglowDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnChangeEdit1();
	afx_msg void OnChangeEdit2();
	afx_msg void OnChangeEdit3();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILTERSOFTGLOWDLG_H__E92FB356_1C51_4875_9F58_CA35CE0CB5D4__INCLUDED_)
