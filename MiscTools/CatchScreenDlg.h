// CatchScreenDlg.h : header file
//

#if !defined(AFX_CATCHSCREENDLG_H__536FDBC8_7DB2_4BEF_8943_70DBE8AD845F__INCLUDED_)
#define AFX_CATCHSCREENDLG_H__536FDBC8_7DB2_4BEF_8943_70DBE8AD845F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Resource.h"
#include "CatchScreenEdit.h"
#include "CatchScreenTrack.h"
/////////////////////////////////////////////////////////////////////////////
// CCatchScreenDlg dialog

class CCatchScreenDlg : public CDialog
{
protected:
	int m_nCXScreen;
	int m_nCYScreen;

	BOOL m_bShowMsg;                //显示截取矩形大小信息
	BOOL m_bDraw;                   //是否为截取状态
	BOOL m_bDrawTracker;              //是否为首次截取
	BOOL m_bQuit;                   //是否为退出
	CPoint m_startPt;				//截取矩形左上角
	CCatchScreenTrack m_rectTracker;     //像皮筋类
	CBrush m_brush;					//
    HCURSOR m_hCursor;              //光标
	CBitmap m_BackgroundBitmap;            //背景位图
	CBitmap m_OrgBackgroundBitmap;		//没有加上透明背景的位图
	CRgn m_rgnUpdate;						//背景擦除区域
	CCatchScreenEdit	m_wndTipEdit;
	COLORREF m_clrTextColor;
	COLORREF m_clrWndFrameColor;
	COLORREF m_clrWndBackgndColor;
public:
	HBITMAP CopyScreenToBitmap(LPRECT lpRect,BOOL bSave=FALSE);   //考贝桌面到位图
	void DrawTip(CDC &dc);                            //显示操作提示信息
	void DrawMessage(CDC &dc, CRect &inRect);       //显示截取矩形信息
	void PaintWindow();               //重画窗口
	//CCatchScreenEdit	m_tipEdit;
// Construction
public:
	void CaptureIt();
	
	
	CCatchScreenDlg(UINT unIDD, UINT unCursorID, UINT unEditBitmapID, 
								COLORREF clrTextColor, COLORREF clrWndColor,
								COLORREF clrWndBackgndColor,
								CWnd* pParent = NULL);	// standard constructor
	virtual ~CCatchScreenDlg();
// Dialog Data
	//{{AFX_DATA(CCatchScreenDlg)
	
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCatchScreenDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCatchScreenDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnCapture();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void Sleep();
	bool m_bSleeping;
	// 从一个位图创建一个加上透明颜色的位图
	HBITMAP CreateTransparentColorBitmap(CDC *pDC, CBitmap* pOrgBitmap, const CRect& rcRect);
public:
	// 画出橡皮筋里面的原始图像
	void DrawOrgRect(CDC *pDC, const CRect & rcRect);
	// //在区域内画上透明颜色
	void DrawTransparentColor(CDC* pDC, const CRect& rcRect);
	// 拷贝屏幕图像到粘贴板
	void CopyScreenToClipboard(const CRect rcRect);
	void DoPaint(CDC& dc);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnNcDestroy();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CATCHSCREENDLG_H__536FDBC8_7DB2_4BEF_8943_70DBE8AD845F__INCLUDED_)
