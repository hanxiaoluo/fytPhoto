// fytPhotoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "fytPhoto.h"
#include "ToolBarDlg.h"
#include "ViewDlg.h"
#include "PhotoEditWnd.h"
#include "EffectLibDlg.h"
#include "fytPhotoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFytPhotoDlg dialog
#define TOOLBAR_HEIGHT 60
CFytPhotoDlg::CFytPhotoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFytPhotoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFytPhotoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pToolbarDlg = NULL;
	m_pViewDlg = NULL;
	m_pPhotoEditWnd = NULL;
	m_pEffectDlg = NULL;
}

CFytPhotoDlg::~CFytPhotoDlg()
{
	SAFE_DELETE(m_pToolbarDlg);
	SAFE_DELETE(m_pViewDlg);
	SAFE_DELETE(m_pPhotoEditWnd);
	SAFE_DELETE(m_pEffectDlg);
}

void CFytPhotoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFytPhotoDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFytPhotoDlg, CDialog)
	//{{AFX_MSG_MAP(CFytPhotoDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFytPhotoDlg message handlers

BOOL CFytPhotoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	
	MoveWindow(0,0,800,600);

	CRect rt;
	GetClientRect(&rt);

	m_pToolbarDlg = new CToolBarDlg(this);
	m_pToolbarDlg->Create(IDD_DIALOG_TOOLBAR, this);
	m_pToolbarDlg->MoveWindow(0, 0, rt.Width(), TOOLBAR_HEIGHT);
	m_pToolbarDlg->ShowWindow(SW_SHOW);

	m_pViewDlg = new CViewDlg(this);
	m_pViewDlg->Create(IDD_DIALOG_VIEW, this);
	m_pViewDlg->MoveWindow(0, TOOLBAR_HEIGHT, rt.Width()-THUMBNAIL_WINDOW_WIDTH, rt.Height()-TOOLBAR_HEIGHT);
	m_pViewDlg->ShowWindow(SW_SHOW);

		// create photoedit window
	m_pPhotoEditWnd = new CPhotoEditWnd;
	m_pPhotoEditWnd->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_PHOTOEDITWND, NULL);
	m_pPhotoEditWnd->ShowWindow(FALSE);

	m_pEffectDlg = new CEffectLibDlg;
	m_pEffectDlg->Create(IDD_EFFECT_LIB, this);
	m_pEffectDlg->MoveWindow(rt.Width()-THUMBNAIL_WINDOW_WIDTH, TOOLBAR_HEIGHT, THUMBNAIL_WINDOW_WIDTH, rt.Height()-TOOLBAR_HEIGHT);
	m_pEffectDlg->ShowWindow(TRUE);

	
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFytPhotoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFytPhotoDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFytPhotoDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CFytPhotoDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if (m_pToolbarDlg && m_pToolbarDlg->GetSafeHwnd())
	{
		m_pToolbarDlg->MoveWindow(0, 0, cx, TOOLBAR_HEIGHT);
		m_pViewDlg->MoveWindow(0, TOOLBAR_HEIGHT, cx-THUMBNAIL_WINDOW_WIDTH, cy-TOOLBAR_HEIGHT);
		m_pPhotoEditWnd->MoveWindow(0, TOOLBAR_HEIGHT, cx, cy-TOOLBAR_HEIGHT);
		m_pEffectDlg->MoveWindow(cx-THUMBNAIL_WINDOW_WIDTH, TOOLBAR_HEIGHT, THUMBNAIL_WINDOW_WIDTH, cy-TOOLBAR_HEIGHT);
	}
}

