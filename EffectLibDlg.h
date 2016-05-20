#if !defined(AFX_EFFECTLIBDLG_H__BC9F0E8F_64C7_4015_AC32_970889DBCB57__INCLUDED_)
#define AFX_EFFECTLIBDLG_H__BC9F0E8F_64C7_4015_AC32_970889DBCB57__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EffectLibDlg.h : header file
//
#include "ListCtrlEx.h"
/////////////////////////////////////////////////////////////////////////////
// CEffectLibDlg dialog

class CEffectLibDlg : public CDialog
{
// Construction
public:
	CEffectLibDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEffectLibDlg)
	enum { IDD = IDD_EFFECT_LIB };
	CListCtrlEx	m_ListCtrl;
	//}}AFX_DATA

	unsigned int m_dwThreadID;	// Thread ID
	HANDLE m_hThread, m_hEvent; // Thread and Event handle

	CListCtrlEx& GetListCtrl(){return m_ListCtrl;};
	CImageList	m_ImageListThumb;
	int AddItemIconToImageList(CString image);

	void LoadThumbImages();
	static unsigned __stdcall LoadThumbNail(LPVOID lpParam);


	bool m_bRunning;			// Flag to whether thread is on running or not
	bool m_bTerminate;			// Flag to Thread to be terminated
	BOOL TerminateThread();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEffectLibDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEffectLibDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EFFECTLIBDLG_H__BC9F0E8F_64C7_4015_AC32_970889DBCB57__INCLUDED_)
