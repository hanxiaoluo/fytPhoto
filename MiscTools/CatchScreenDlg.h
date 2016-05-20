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

	BOOL m_bShowMsg;                //��ʾ��ȡ���δ�С��Ϣ
	BOOL m_bDraw;                   //�Ƿ�Ϊ��ȡ״̬
	BOOL m_bDrawTracker;              //�Ƿ�Ϊ�״ν�ȡ
	BOOL m_bQuit;                   //�Ƿ�Ϊ�˳�
	CPoint m_startPt;				//��ȡ�������Ͻ�
	CCatchScreenTrack m_rectTracker;     //��Ƥ����
	CBrush m_brush;					//
    HCURSOR m_hCursor;              //���
	CBitmap m_BackgroundBitmap;            //����λͼ
	CBitmap m_OrgBackgroundBitmap;		//û�м���͸��������λͼ
	CRgn m_rgnUpdate;						//������������
	CCatchScreenEdit	m_wndTipEdit;
	COLORREF m_clrTextColor;
	COLORREF m_clrWndFrameColor;
	COLORREF m_clrWndBackgndColor;
public:
	HBITMAP CopyScreenToBitmap(LPRECT lpRect,BOOL bSave=FALSE);   //�������浽λͼ
	void DrawTip(CDC &dc);                            //��ʾ������ʾ��Ϣ
	void DrawMessage(CDC &dc, CRect &inRect);       //��ʾ��ȡ������Ϣ
	void PaintWindow();               //�ػ�����
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
	// ��һ��λͼ����һ������͸����ɫ��λͼ
	HBITMAP CreateTransparentColorBitmap(CDC *pDC, CBitmap* pOrgBitmap, const CRect& rcRect);
public:
	// ������Ƥ�������ԭʼͼ��
	void DrawOrgRect(CDC *pDC, const CRect & rcRect);
	// //�������ڻ���͸����ɫ
	void DrawTransparentColor(CDC* pDC, const CRect& rcRect);
	// ������Ļͼ��ճ����
	void CopyScreenToClipboard(const CRect rcRect);
	void DoPaint(CDC& dc);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnNcDestroy();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CATCHSCREENDLG_H__536FDBC8_7DB2_4BEF_8943_70DBE8AD845F__INCLUDED_)
