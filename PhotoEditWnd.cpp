// PhotoEditWnd.cpp : implementation file
//

#include "stdafx.h"
#include "fytPhoto.h"
#include "PhotoEditWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPhotoEditWnd

CPhotoEditWnd::CPhotoEditWnd()
{
	m_pEditDlg = NULL;
}

CPhotoEditWnd::~CPhotoEditWnd()
{
}


BEGIN_MESSAGE_MAP(CPhotoEditWnd, CWnd)
	//{{AFX_MSG_MAP(CPhotoEditWnd)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPhotoEditWnd message handlers

int CPhotoEditWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if(m_pEditDlg == NULL)
	{
		m_pEditDlg = new CPhotoEditDlg(this);
		m_pEditDlg->Create(IDD_DIALOG_EDIT_MAIN,this);
	}
	m_pEditDlg->ShowWindow(FALSE);

	return 0;
}

BOOL CPhotoEditWnd::OnEraseBkgnd(CDC* pDC) 
{
	CRect Rect;
	GetClientRect(Rect);
	CBrush brush(RGB(0,0,0));
	CBrush* pOldBrush = pDC->SelectObject(&brush);
	pDC->Rectangle(&Rect);
	pDC->SelectObject(pOldBrush);
	brush.DeleteObject();

	return TRUE;	
//	return CWnd::OnEraseBkgnd(pDC);
}

void CPhotoEditWnd::OnDestroy() 
{
	CWnd::OnDestroy();
	
	if(m_pEditDlg != NULL)
	{
		delete m_pEditDlg;
		m_pEditDlg = NULL;
	}
}

void CPhotoEditWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if (m_pEditDlg && m_pEditDlg->GetSafeHwnd())
	{
		m_pEditDlg->MoveWindow(0, 0, cx, cy);
	}
}
