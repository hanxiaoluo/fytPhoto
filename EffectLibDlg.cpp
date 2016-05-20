// EffectLibDlg.cpp : implementation file
//

#include "stdafx.h"
#include "fytphoto.h"
#include "EffectLibDlg.h"
#include <process.h>
#include <vector>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEffectLibDlg dialog


CEffectLibDlg::CEffectLibDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEffectLibDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEffectLibDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bRunning = false;
}


void CEffectLibDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEffectLibDlg)
	DDX_Control(pDX, IDC_LIST_EFFECT, m_ListCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEffectLibDlg, CDialog)
	//{{AFX_MSG_MAP(CEffectLibDlg)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEffectLibDlg message handlers

BOOL CEffectLibDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	m_ImageListThumb.Create(THUMBNAIL_WIDTH, THUMBNAIL_HEIGHT, ILC_COLOR24, 0, 1);

	m_ListCtrl.SetImageList(&m_ImageListThumb,LVSIL_NORMAL);
	//m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()|LVS_EX_FULLROWSELECT);

	MoveWindow(0, 0, THUMBNAIL_WIDTH*2, 600);


	//m_ListCtrl.SetBkColor(RGB(0,0,0));


	LoadThumbImages();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CEffectLibDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if (m_ListCtrl.GetSafeHwnd())
	{
		m_ListCtrl.MoveWindow(0, 0, cx, cy);
	}
}


int CEffectLibDlg::AddItemIconToImageList(CString szImage)
{
	/*fipWinImage image, imageThumb;
	if (!image.load(szImage))
		return -1;

	bool bb = image.rescale(ICON_WIDTH, ICON_HEIGHT, FILTER_BILINEAR);
	imageThumb = image;

	HBITMAP hbmp = (HBITMAP)imageThumb.copyToHandle();
	int width = imageThumb.getWidth();
	CBitmap* pcbImage = new CBitmap();
	//bb = pcbImage->Attach(hbmp);
	//int n = m_imageLarge.Add(pcbImage,RGB(0,0,0));
	int nIndex = m_imageLarge.GetImageCount();
	bb = m_imageLarge.Replace(nIndex, pcbImage, NULL);
	delete pcbImage;
	pcbImage = NULL;

	return nIndex;*/
	return 0;

}

unsigned __stdcall CEffectLibDlg::LoadThumbNail(LPVOID lpParam)
{
	CEffectLibDlg* pView=(CEffectLibDlg*)lpParam;

	CListCtrl& ListCtrl=pView->GetListCtrl();
	CImageList* pImgList=&pView->m_ImageListThumb;

	// reset our image list
	for(int i=0; i<pImgList->GetImageCount(); i++)
		pImgList->Remove(i);	

	// remove all items from list view
	ListCtrl.DeleteAllItems();

	pImgList->SetImageCount(1);

	TCHAR path[MAX_PATH];
	vector<CString>::iterator iter;
	
	// Set redraw to FALSE to avoid flickering during adding new items
	ListCtrl.SetRedraw(FALSE);
	int nIndex=0;
	/*for(iter=pDoc->m_vFileName.begin(); iter!=pDoc->m_vFileName.end() && pView->m_bTerminate!=true; iter++, nIndex++)
	{
		ListCtrl.InsertItem(nIndex, *iter, nIndex);
	}*/


	ListCtrl.SetRedraw(TRUE);
	ListCtrl.Invalidate();

	// Create Brushes for Border and BackGround
	HBRUSH hBrushBorder=::CreateSolidBrush(RGB(192, 192, 192));
	HBRUSH hBrushBk=::CreateSolidBrush(RGB(255, 255, 255));

	// Border Size
	RECT rcBorder;
	rcBorder.left=rcBorder.top=0;
	rcBorder.right=THUMBNAIL_WIDTH;
	rcBorder.bottom=THUMBNAIL_HEIGHT;

	const float fRatio=(float)THUMBNAIL_HEIGHT/THUMBNAIL_WIDTH;

	int XDest, YDest, nDestWidth, nDestHeight;
	nIndex=0;
	//for(iter=pDoc->m_vFileName.begin(); iter!=pDoc->m_vFileName.end() && pView->m_bTerminate!=true; iter++, nIndex++)
	for (int j = 0; j < 20; j++)
	{
		// Load Image File
		ListCtrl.InsertItem(j, "test", nIndex);
	
		fipWinImage image;
		image.load("c:\\3.bmp");
		//CxImage image(path, nImageType);

		if(image.isValid()==false)
			return -1;

		// Calculate Rect to fit to canvas
		const float fImgRatio=(float)image.getHeight()/image.getWidth();
		
		long nWidth=abs(image.getWidth());
		long nHeight = abs(image.getHeight());
	/*	long nWndWidth = THUMBNAIL_WIDTH-20;
		long nWndHeight = THUMBNAIL_HEIGHT;
		int nL,nT;
		if(nWidth > nWndWidth)
		{
			double radio = (double)nWidth / (double)nWndWidth;
			nWidth = nWndWidth;
			nHeight = (long)((double)nHeight/radio);
			nL = 0;
		}
		else
		{
			nL = (nWndWidth-nWidth)/2;
		}
		if(nHeight > nWndHeight)
		{
			double radio = (double)nHeight / (double)nWndHeight;
			nHeight = nWndHeight;
			nWidth = (long)((double)nWidth/radio);
			nT = 0;
			nL = (nWndWidth-nWidth)/2;
		}
		else
		{
			nT = (nWndHeight-nHeight)/2;
		}
		int m_nL = nL;
		int m_nT = nT;
		int m_nR = nL+nWidth;
		int m_nB = nT+nHeight;*/


		CClientDC cdc(pView);
		HDC hDC=::CreateCompatibleDC(cdc.m_hDC);
		HBITMAP bm = CreateCompatibleBitmap(cdc.m_hDC, THUMBNAIL_WIDTH, THUMBNAIL_HEIGHT);
		HBITMAP pOldBitmapImage = (HBITMAP)SelectObject(hDC,bm);
		// Draw Background
		::FillRect(hDC, &rcBorder, hBrushBk);

		// Draw Image
		//image.Stretch(hDC, XDest, YDest, nDestWidth, nDestHeight);
		//RECT rt = {m_nL, m_nT, m_nR, m_nB};
		image.draw(hDC, rcBorder);

		// Draw Border
		::FrameRect(hDC, &rcBorder, hBrushBorder);

		SelectObject(hDC, pOldBitmapImage);

		// Attach to Bitmap and Replace image in CImageList
		CBitmap bitmap;
		bitmap.Attach(bm);
		pImgList->Replace(j, &bitmap, NULL);

		// Redraw only a current item for removing flickering and fast speed.
		ListCtrl.RedrawItems(j, j);

		// Release used DC and Object
		DeleteDC(hDC);
		DeleteObject(bm);
	}
	DeleteObject(hBrushBorder);
	DeleteObject(hBrushBk);

	ListCtrl.Invalidate();
	pView->m_bRunning=false;
	pView->m_bTerminate=false;

	pView->Invalidate();
	_endthreadex( 0 );

	
	return 0;
}

void CEffectLibDlg::LoadThumbImages()
{
	//CListCtrl& ListCtrl=GetListCtrl();

	TerminateThread();
	m_hThread = (HANDLE)_beginthreadex(NULL,0, LoadThumbNail,(LPVOID)this,0/* CREATE_SUSPENDED*/,&m_dwThreadID); 

	m_bRunning=true;
}

BOOL CEffectLibDlg::TerminateThread()
{
	if ( !m_bRunning )
    return TRUE;
  
  m_bTerminate=true;
 
  for( ; ; )
  {
    if ( ::WaitForSingleObject(m_hThread, 0) == WAIT_OBJECT_0 )
      break;
 
    MSG msg;
    while (::PeekMessage(&msg,NULL,0,0,PM_NOREMOVE)) 
    { 
      if (!AfxGetApp()->PumpMessage()) 
        break; 
    } 
  }
 
  ::CloseHandle(m_hThread);
  return TRUE;

}

