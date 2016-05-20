// PhotoEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "fytPhoto.h"
#include "PhotoEditDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPhotoEditDlg dialog


CPhotoEditDlg::CPhotoEditDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPhotoEditDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPhotoEditDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pCurObject = NULL;
	m_bCurImageModified = FALSE;
}

CPhotoEditDlg::~CPhotoEditDlg()
{
	if(m_pCurObject)
	{
		m_pCurObject->Delete();
		delete m_pCurObject;
		m_pCurObject = NULL;
	}
}

void CPhotoEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPhotoEditDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPhotoEditDlg, CDialog)
	//{{AFX_MSG_MAP(CPhotoEditDlg)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPhotoEditDlg message handlers

BOOL CPhotoEditDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CPhotoEditDlg::SetCurImageModified(BOOL bModified)
{
	m_bCurImageModified = bModified;
}

EDITTYPE CPhotoEditDlg::GetEditType()
{
	return m_iEditType;
}

CImageObject* CPhotoEditDlg::GetCurObject()
{
	return m_pCurObject;
}

void CPhotoEditDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}
