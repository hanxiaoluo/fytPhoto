#if !defined(AFX_VIEWDLG_H__68BB6C19_F8AB_4A4D_BBAA_6AE7F579B8C3__INCLUDED_)
#define AFX_VIEWDLG_H__68BB6C19_F8AB_4A4D_BBAA_6AE7F579B8C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CViewDlg dialog

class CViewDlg : public CDialog
{
// Construction
public:
	CViewDlg(CWnd* pParent = NULL);   // standard constructor
	~CViewDlg();

	void SetImage(CString& szFile);

	//全窗口背景色和图片重绘
	void PaintRGN();
	void OpenFile(CString& szFile);

	fipWinImage m_image;
// Dialog Data
	//{{AFX_DATA(CViewDlg)
	enum { IDD = IDD_DIALOG_VIEW };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BITMAPINFO* m_pInfo;
	BYTE* m_pData;

	CDC BackDC;
	CBitmap	BackBmp;

	CDC m_ImageDC;
	CBitmap	m_ImageBmp;
	// Generated message map functions
	//{{AFX_MSG(CViewDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWDLG_H__68BB6C19_F8AB_4A4D_BBAA_6AE7F579B8C3__INCLUDED_)
