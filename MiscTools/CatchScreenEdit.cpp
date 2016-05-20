// MyEdit.cpp : implementation file
//

#include "stdafx.h"
#include "CatchScreenEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "../Resource.h"
#include ".\catchscreenedit.h"
#include "memdc.h"

/////////////////////////////////////////////////////////////////////////////
// CCatchScreenEdit


CCatchScreenEdit::CCatchScreenEdit(COLORREF clrTextColor)
: m_strInfo(_T(""))
, m_clrTextColor(clrTextColor)
, m_clrCurSelectedColor(clrTextColor)
{
	m_bMove=TRUE;	
}

CCatchScreenEdit::~CCatchScreenEdit()
{
	if(m_BkgndBitmap.GetSafeHandle())
		m_BkgndBitmap.DeleteObject();
}


BEGIN_MESSAGE_MAP(CCatchScreenEdit, CEdit)
	//{{AFX_MSG_MAP(CCatchScreenEdit)
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_WM_SETFOCUS()
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCatchScreenEdit message handler

int CCatchScreenEdit::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	return 0;
}

void CCatchScreenEdit::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//MessageBox("Mouse");
	
	CRect rect;
	GetWindowRect(&rect);

	int xScreen = GetSystemMetrics(SM_CXSCREEN);
	//int ySCreen = GetSystemMetrics(SM_CYSCREEN);

	if(m_bMove)
	{
		//移动到左上角
		MoveWindow(10,10,rect.Width(),rect.Height());
		m_bMove=FALSE;
	}
	else
	{
		//移动到右上角
		MoveWindow(xScreen - rect.Width() - 10,10,rect.Width(),rect.Height());
		m_bMove=TRUE;
	}

	CEdit::OnMouseMove(nFlags, point);
}

void CCatchScreenEdit::OnSetFocus(CWnd* pOldWnd) 
{
	CEdit::OnSetFocus(pOldWnd);
   //隐藏光标提示符
	this->HideCaret();
}

HBRUSH CCatchScreenEdit::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	//设置文字背景透明
	pDC->SetBkMode(TRANSPARENT);
	return NULL;
}

void CCatchScreenEdit::SetInfoText(const CString& strInfoValue)
{
	m_strInfo = strInfoValue;
}

// //设置背景位图
void CCatchScreenEdit::LoadBkgrndBitmap(UINT unBitmapID)
{
	m_BkgndBitmap.LoadBitmap(unBitmapID);
}

// //获取背景位图
CBitmap* CCatchScreenEdit::GetBkgrndBitmap(void)
{
	return &m_BkgndBitmap;
}

// //设置背景图ID
void CCatchScreenEdit::SetBkgrndBitmapID(UINT nID)
{
	m_unBitmapID = nID;
}

UINT CCatchScreenEdit::GetBkgrndBitmapID(void)
{
	return m_unBitmapID;
}
void CCatchScreenEdit::OnPaint()
{
	CPaintDC dc(this);
	this->SetSel(0, 0);
	CRect rect;
	GetClientRect(&rect);
	CBitmap bitmap;
	bitmap.LoadBitmap(m_unBitmapID);
	CDC dcCompatible;
	dcCompatible.CreateCompatibleDC(&dc);
	dcCompatible.SetTextColor(m_clrTextColor);

	CFont font;
	font.CreatePointFont(90,"宋体");
	CFont *pOldFont = (CFont*) dcCompatible.SelectObject(&font);

	HBITMAP* pOldBitmap =(HBITMAP*) dcCompatible.SelectObject(&bitmap);
	dcCompatible.SetBkMode(TRANSPARENT);

	
	CRect rcText(rect);
	rcText.top += 2;
	dcCompatible.DrawText(m_strInfo, &rcText, DT_TOP|DT_WORDBREAK);
	CRect rcColorItem(rcText);
	const int nWidth = 14;
	rcColorItem.top += 35;
	rcColorItem.left = rcColorItem.left + 68;
	rcColorItem.right = rcColorItem.left + nWidth;
	rcColorItem.bottom = rcColorItem.top + nWidth;
	CPen penBorder;
	CBrush brushFill;
	penBorder.CreatePen(PS_SOLID, 1, m_clrTextColor);
	brushFill.CreateSolidBrush(m_clrCurSelectedColor);
	CPen* pOldPen = dcCompatible.SelectObject(&penBorder);
	CBrush* pOldBrush = dcCompatible.SelectObject(&brushFill);
	dcCompatible.Rectangle(rcColorItem);
	dcCompatible.SelectObject(pOldPen);
	dcCompatible.SelectObject(pOldBrush);
	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &dcCompatible, 0, 0, SRCCOPY);
	dcCompatible.SelectObject(pOldBitmap);
	penBorder.DeleteObject();
	brushFill.DeleteObject();
}

BOOL CCatchScreenEdit::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CCatchScreenEdit::SetCurSelectedColor(COLORREF clrCurSelectedColor)
{
	m_clrCurSelectedColor = clrCurSelectedColor;
}