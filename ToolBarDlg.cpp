// ToolBarDlg.cpp : implementation file
//

#include "stdafx.h"
#include "fytPhoto.h"
#include "fytPhotoDlg.h"
#include "ViewDlg.h"

#include "ScreenSpyDlg.h"
#include "PhotoEditWnd.h"

#include "FilterSoftglowDlg.h"
#include "EffectLibDlg.h"

#include "ToolBarDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CToolBarDlg dialog


CToolBarDlg::CToolBarDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CToolBarDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CToolBarDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bShowEdit = false;
}


void CToolBarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CToolBarDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CToolBarDlg, CDialog)
	//{{AFX_MSG_MAP(CToolBarDlg)
	ON_BN_CLICKED(IDC_BUTTON_OPEN, OnButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON_SCREEN, OnButtonScreen)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, OnButtonEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolBarDlg message handlers

BOOL CToolBarDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	int l = 0; 
	int t = 0; 
	int w = 60;
	int h = 60;
	int off = 10;
	GetDlgItem(IDC_BUTTON_OPEN)->MoveWindow(l, t, w, h);
	GetDlgItem(IDC_BUTTON_VIEW)->MoveWindow(l + w + off, t, w, h);
	GetDlgItem(IDC_BUTTON_MANAGER)->MoveWindow(l + (w + off)*2, t, w, h);
	GetDlgItem(IDC_BUTTON_EDIT)->MoveWindow(l + (w+off)*3, t, w, h);
	GetDlgItem(IDC_BUTTON_SCREEN)->MoveWindow(l + (w+off)*4, t, w, h);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL GetOpenSaveMRU(LPCTSTR strExtName,char *strMRUPath)
{
	HKEY hKEY;
	const char *mru="Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\ComDlg32\\OpenSaveMRU\\";
	CString strmru(mru);
	strmru += "*";
	long lRet = RegOpenKeyEx(HKEY_CURRENT_USER,strmru,0,KEY_READ,&hKEY);
	if (lRet != ERROR_SUCCESS ) {
		return FALSE;
	}
	
	DWORD keytype1 = REG_SZ;
	DWORD dwInBytes = 256;
	BYTE pInBuf[256];
	lRet = RegQueryValueEx(hKEY,"MRUList",NULL,&keytype1,pInBuf,&dwInBytes);
	if (lRet != ERROR_SUCCESS ) {
		return FALSE;
	}
	dwInBytes = 256;
	char strLast[] = {pInBuf[0],0};
	lRet = RegQueryValueEx(hKEY,(LPTSTR)strLast,NULL,&keytype1,(LPBYTE)strMRUPath,&dwInBytes);
	if (lRet != ERROR_SUCCESS) {
		return FALSE;
	}
	CString str1(strMRUPath);
	int idx=str1.ReverseFind('\\');
	strMRUPath[idx]=0;
	
	RegCloseKey(hKEY);
	return TRUE;
}

#define FILE_FILTER_TEXT \
    TEXT("Image Files (*.bmp; *.jpg; *.png;*.tiff;*.gif;)\0*.bmp; *.jpg; *.png;*.tiff;*.gif;\0")\
    TEXT("All Files (*.*)\0*.*;\0\0")

void CToolBarDlg::OnButtonOpen() 
{
	// TODO: Add your control notification handler code here
    OPENFILENAME ofn={0};
    BOOL bSetInitialDir = FALSE;

	char m_strInitDir[256];
	//ZeroMemory(m_strInitDir,0,sizeof(m_strInitDir));
	GetOpenSaveMRU(FILE_FILTER_TEXT,m_strInitDir);

    // Reset filename
    TCHAR buff[8192];// String buffer especially for OFN_ALLOWMULTISELECT
	memset(buff,'\0',8192);

    // Fill in standard structure fields
    ofn.lStructSize       = sizeof(OPENFILENAME);
    ofn.hwndOwner         = NULL;
    ofn.lpstrFilter       = FILE_FILTER_TEXT;
    ofn.lpstrCustomFilter = NULL;
    ofn.nFilterIndex      = 1;
    ofn.lpstrFile         = buff;
    ofn.nMaxFile          = 8192;
    ofn.lpstrTitle        = TEXT("Open Media File...\0");
    ofn.lpstrFileTitle    = NULL;
	ofn.nFileOffset       = 0;
    ofn.lpstrDefExt       = TEXT("*\0");
	ofn.lpstrInitialDir   = m_strInitDir;
    //ofn.Flags             = OFN_FILEMUSTEXIST | OFN_READONLY | OFN_PATHMUSTEXIST|OFN_ALLOWMULTISELECT|OFN_EXPLORER;
	ofn.Flags             = OFN_FILEMUSTEXIST | OFN_READONLY | OFN_PATHMUSTEXIST|OFN_EXPLORER;

    // Remember the path of the first selected file
    /*if (bSetInitialDir == FALSE)
    {
        ofn.lpstrInitialDir = DEFAULT_MEDIA_PATH;
        bSetInitialDir = TRUE;
    }
    else
        ofn.lpstrInitialDir = NULL;*/

	if(!GetOpenFileName(&ofn))
	{
		DWORD dw = CommDlgExtendedError();
		CString sz;
		sz.Format("%x",dw);
		//FNERR_BUFFERTOOSMALL

		return ;
	}
	//szFile =  ofn.lpstrFile;

	CString psz = ofn.lpstrFile + ofn.nFileOffset;
	//psz[-1] = _T('\\');


	CFytPhotoDlg* pDlg = (CFytPhotoDlg*)::AfxGetMainWnd();
	pDlg->m_pViewDlg->OpenFile(psz);
}


HBITMAP GetBitmapFromClipboard(void)
{
	if ( OpenClipboard(NULL))
	{
		//Get the clipboard data
		HBITMAP hBitmap = (HBITMAP)GetClipboardData(CF_BITMAP);

		CloseClipboard();
		return hBitmap;
	}
	else
		return NULL;
}

void CToolBarDlg::OnButtonScreen() 
{
	// TODO: Add your control notification handler code here
	CScreenSpyDlg dlg;
	dlg.DoModal();
/*	CCatchScreenDlg dlg(IDD_CATCHSCREEN_DIALOG, IDC_CURSOR_CATCH_SCREEN, 
					IDB_BITMAP_CATCHSCREENDLG_EDITBKGRND, 
					RGB(0,0,0),
					RGB(255,0,0),
					RGB(0,0,255),
					this);
	if(dlg.DoModal() == IDOK)
	{
		CString strFilter = _T("PNG 文件 (*.PNG)|*.PNG|JPEG 文件 (*.JPG;*.JPEG)|*.JPG;*.JPEG|位图文件 (*.BMP)|*.BMP||");

		CFileDialog fileDlg(FALSE, _T(".BMP"), _T("CatchPicture"), OFN_ENABLESIZING | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFilter, this);
		fileDlg.m_ofn.lpstrTitle = _T("截图另存为");
		if(fileDlg.DoModal() == IDOK)
		{
			CWaitCursor wc;
			
			HBITMAP hBitmap = GetBitmapFromClipboard();
			if(hBitmap)
			{
				fipWinImage image;
				image.copyFromBitmap(hBitmap);
				image.save(fileDlg.GetPathName());
			}
		}
	}*/
}

void CToolBarDlg::OnButtonEdit() 
{
	// TODO: Add your control notification handler code here
	/*m_bShowEdit = !m_bShowEdit;
	CFytPhotoDlg* pDlg = (CFytPhotoDlg*)::AfxGetMainWnd();
	if (m_bShowEdit)
	{
		pDlg->m_pViewDlg->ShowWindow(SW_HIDE);
		pDlg->m_pPhotoEditWnd->ShowWindow(SW_SHOW);
	}
	else
	{
		pDlg->m_pViewDlg->ShowWindow(SW_SHOW);
		pDlg->m_pPhotoEditWnd->ShowWindow(SW_HIDE);
	}*/

	//CEffectLibDlg dlg;
	CFytPhotoDlg* pDlg = (CFytPhotoDlg*)::AfxGetMainWnd();
	if (!pDlg->m_pViewDlg->m_image.isValid())
	{
		AfxMessageBox("先打开图片!");
		return;
	}

	CFilterSoftglowDlg dlg;
	dlg.m_image = pDlg->m_pViewDlg->m_image;
	dlg.DoModal();
}
