/********************************************************************

	LineTrackBar.h - ISeeͼ���������ͼ����ģ����LineTrackBarͷ�ļ�

    ��Ȩ����(C) VCHelp-coPathway-ISee workgroup 2000 all member's

    ��һ����������������������������������������GNU ͨ�����֤
	�������޸ĺ����·�����һ���򡣻��������֤�ĵڶ��棬���ߣ�������
	��ѡ�����κθ��µİ汾��

    ������һ�����Ŀ����ϣ�������ã���û���κε���������û���ʺ��ض�
	Ŀ�ص������ĵ���������ϸ����������GNUͨ�����֤��

    ��Ӧ���Ѿ��ͳ���һ���յ�һ��GNUͨ�����֤(GPL)�ĸ����������û�У�
	д�Ÿ���
    The Free Software Foundation, Inc.,  675  Mass Ave,  Cambridge,
    MA02139,  USA

	�������ʹ�ñ����ʱ��ʲô������飬�����µ�ַ����������ȡ����
	ϵ��
		http://isee.126.com
		http://www.vchelp.net
	��
		iseesoft@china.com

	���ߣ�����
   e-mail:hugesoft@yeah.net

   ����ʵ�֣�PhotoShop���Ļ�����

	�ļ��汾��
		Build xxxxx
		Date  2000-11-18

********************************************************************/
#if !defined(AFX_LINETRACKBAR_H__D4A2173A_2625_4EE6_97EC_F421924361D0__INCLUDED_)
#define AFX_LINETRACKBAR_H__D4A2173A_2625_4EE6_97EC_F421924361D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LineTrackBar.h : header file
//
#include <afxtempl.h>
#include "SignEdit.h"

#define RE_STRING	0
#define RE_INT		1

class CLineTrackObj:public CObject
{
public:
	CLineTrackObj() {;}

	COLORREF m_color;
	int     m_nLower;		// lower bounds
	int     m_nUpper;		// upper bounds
	int		m_nZero;

	int     m_nPos;		// current position within bounds
	int		m_nStep;
	int		m_nStype;
	BOOL	m_bFocus;
	CWnd	*pCtrl;
};

typedef CTypedPtrArray <CObArray, CLineTrackObj*> CItemArray;

/////////////////////////////////////////////////////////////////////////////
// CLineTrackBar window

class CLineTrackBar : public CWnd
{
// Construction
public:
	CLineTrackBar();

// Attributes
public:
	CDC      m_MemDC;
	CBitmap  m_Bitmap;
	CItemArray m_items;
	CLineTrackObj* pItem;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLineTrackBar)
	//}}AFX_VIRTUAL

// Implementation
private:
	void SetGroupControlText();
	void SetGroupControl(int nCtrl);
	void InvalidateCtrl();
	void DrawSpike();

public:
	BOOL RegisterWndClass(HINSTANCE hInstance);
	int GetPos();
	void SetPos(int nPos);
	BOOL InitControl(int nGroupCtrl=-1,COLORREF color=RGB(0,0,0), int nLower=0, int nZero=100, int nUpper=200,int nStep=1,BOOL bFocus=FALSE,int nStype=0);
	virtual ~CLineTrackBar();

	// Generated message map functions
protected:
	//{{AFX_MSG(CLineTrackBar)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LINETRACKBAR_H__D4A2173A_2625_4EE6_97EC_F421924361D0__INCLUDED_)
