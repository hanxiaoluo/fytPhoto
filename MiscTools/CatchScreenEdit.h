#if !defined(AFX_MYEDIT_H__A34EEA6D_E8FC_4D15_B03C_BAA42FDF6FCB__INCLUDED_)
#define AFX_MYEDIT_H__A34EEA6D_E8FC_4D15_B03C_BAA42FDF6FCB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCatchScreenEdit window

class CCatchScreenEdit : public CEdit
{
// Construction
public:
	CCatchScreenEdit(COLORREF clrTextColor);
	BOOL m_bMove;
// Attributes
public:
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCatchScreenEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCatchScreenEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCatchScreenEdit)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	// 显示信息
	CString m_strInfo;
public:
	void SetInfoText(const CString& strInfoValue);
	void SetCurSelectedColor(COLORREF clrCurSelectedColor);
protected:
	// 背景位图
	CBitmap m_BkgndBitmap;
	UINT m_unBitmapID;
	COLORREF m_clrTextColor;
	COLORREF m_clrCurSelectedColor;
public:
	// //设置背景位图
	void LoadBkgrndBitmap(UINT unBitmapID);
	// //获取背景位图
	CBitmap* GetBkgrndBitmap(void);
	// //设置背景图ID
	void SetBkgrndBitmapID(UINT nID);
	UINT GetBkgrndBitmapID(void);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYEDIT_H__A34EEA6D_E8FC_4D15_B03C_BAA42FDF6FCB__INCLUDED_)
