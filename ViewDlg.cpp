// ViewDlg.cpp : implementation file
//

#include "stdafx.h"
#include "fytPhoto.h"
#include "ViewDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewDlg dialog


CViewDlg::CViewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CViewDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CViewDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pInfo = NULL;
	m_pData = NULL;
}

CViewDlg::~CViewDlg()
{
	SAFE_DELETE(m_pInfo);
	SAFE_DELETE(m_pData);

	BackDC.DeleteDC();
	BackBmp.DeleteObject();

	if(m_ImageDC.GetSafeHdc())
	{
		m_ImageDC.DeleteDC();
		m_ImageBmp.DeleteObject();
	}
}


void CViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewDlg, CDialog)
	//{{AFX_MSG_MAP(CViewDlg)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_DROPFILES()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewDlg message handlers

BOOL CViewDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	int nWidth = GetSystemMetrics ( SM_CXSCREEN ); 
	int nHeight= GetSystemMetrics ( SM_CYSCREEN ); 

	CClientDC clientDC(this);
	BackDC.CreateCompatibleDC(&clientDC);
	BackBmp.CreateCompatibleBitmap(&clientDC, nWidth, nHeight);
	BackDC.SelectObject(BackBmp);

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


 
void CViewDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if (m_image.isValid())
	{
		CDC* pDC = GetDC();
		//if (pDC && pDC->m_hDC)
		{
			//PaintRGN();

			CRect rt;
			GetClientRect(rt);

			CBrush brush(RGB(128, 128, 128));
			CBrush* pOldBrush = pDC->SelectObject(&brush);
			BackDC.FillRect(rt, &brush);
			
			CRect rt1 = CalcRect(m_image.getWidth(), m_image.getHeight(), &rt);
			m_image.drawEx(BackDC.m_hDC, rt1);

			pDC->BitBlt(
				    0,
				    0,
				    rt.Width(),
				    rt.Height(),
				  	&BackDC,
					0,
					0,
					SRCCOPY);
		}
	}
	//Invalidate();
}

BOOL CViewDlg::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return CDialog::OnEraseBkgnd(pDC);
	if (m_image.isValid())
	{
		PaintRGN();
	}
	else
	{
		CRect Rect;
		GetClientRect(Rect);
		CBrush brush(RGB(128, 128, 128));
		CBrush* pOldBrush = pDC->SelectObject(&brush);
		pDC->FillRect(Rect, &brush);
		
		pDC->SetTextColor(RGB(0,255,0)); //ÉèÖÃ×ÖÌåÑÕÉ«
		pDC->SetBkMode(TRANSPARENT);
		//pDC->TextOut(0, 0, m_strID, m_strID.GetLength());
		pDC->SelectObject(pOldBrush);
		brush.DeleteObject();
	}
	return FALSE;


	//return CDialog::OnEraseBkgnd(pDC);
}

void CViewDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect rc;
	GetClientRect(rc);

	COLORREF colorBK=RGB(128, 128, 128);
	CBrush bkbrush(colorBK);
	FillRect(dc.m_hDC,rc,(HBRUSH)bkbrush);

	INT nMode = dc.SetStretchBltMode(COLORONCOLOR);
	// TODO: Add your message handler code here
	if (m_image.isValid())
	{
		dc.BitBlt(
				    0,
				    0,
				    rc.Width(),
				    rc.Height(),
				  	&BackDC,
					0,
					0,
					SRCCOPY);
	}
	dc.SetStretchBltMode(nMode);

	return;
	// Do not call CDialog::OnPaint() for painting messages

	
	/*::StretchDIBits(dc.m_hDC, lDstLeft, lDstTop, cx, cy, 
		lSrcLeft, lSrcTop,
		m_pCurImageItem->pSrcBmpInfo->bmiHeader.biWidth,
		m_pCurImageItem->pSrcBmpInfo->bmiHeader.biHeight,
		(LPVOID)pData, 
		(LPBITMAPINFO)pBmpInfo,
		DIB_RGB_COLORS,
		SRCCOPY);*/

	
}



void DrawDib(LPBITMAPINFO pInfo,LPBYTE pData,HDC hDC,LPRECT pDestRect,LPRECT pSrcRect)
{
	
	SetStretchBltMode(hDC,COLORONCOLOR);
//	SetStretchBltMode(hDC,HALFTONE);
//	SetBrushOrgEx(hDC,0,0,NULL);

	LONG	srcX = 0,srcY = 0,srcW = abs(pInfo->bmiHeader.biWidth),srcH = abs(pInfo->bmiHeader.biHeight);
	if(pSrcRect)
	{
		srcX = pSrcRect->left;
		srcY = pSrcRect->top;
		srcW = pSrcRect->right-pSrcRect->left;
		srcH = pSrcRect->bottom-pSrcRect->top;
	}

	long nWidth=abs(pInfo->bmiHeader.biWidth);
	long nHeight = abs(pInfo->bmiHeader.biHeight);
	long nWndWidth = pDestRect->right-pDestRect->left;
	long nWndHeight = pDestRect->bottom - pDestRect->top;
	int nL,nT;
	if(nWidth > nWndWidth)
	{
		double radio = nWidth /nWndWidth;
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
		double radio = nHeight /nWndHeight;
		nHeight = nWndHeight;
		nWidth = (long)((double)nWidth/radio);
		nT = 0;
		nL = (nWndWidth-nWidth)/2;
	}
	else
	{
		nT = (nWndHeight-nHeight)/2;
	}
	int m_nL =  pDestRect->left+nL;
	int m_nT = pDestRect->top+nT;
	int m_nR = pDestRect->left+nL+nWidth;
	int m_nB = pDestRect->top+nT+nHeight;


	/*::StretchDIBits(hDC,
		  pDestRect->left, pDestRect->top,pDestRect->right-pDestRect->left,pDestRect->bottom-pDestRect->top,
		  srcX,srcY,srcW,srcH,
		  pData,pInfo,DIB_RGB_COLORS,SRCCOPY);*/
	::StretchDIBits(hDC,
		  m_nL, m_nT,nWidth,nHeight,
		  srcX,srcY,srcW,srcH,
		  pData,pInfo,DIB_RGB_COLORS,SRCCOPY);
	
}

void CViewDlg::OpenFile(CString& szFile)
{
	m_image.load(szFile.GetBuffer(szFile.GetLength()));
	if (m_image.isValid())
	{
		CDC* pDC = GetDC();
		CRect rt;
		GetClientRect(rt);
		
		CBrush brush(RGB(128, 128, 128));
		CBrush* pOldBrush = pDC->SelectObject(&brush);
		BackDC.FillRect(rt, &brush);
		
		CRect rt1 = CalcRect(m_image.getWidth(), m_image.getHeight(), &rt);
		m_image.drawEx(BackDC.m_hDC, rt1);
		
		Invalidate();
	}
}

void CViewDlg::SetImage(CString& szFile)
{
	/*CDC* pDC = GetDC();
	CRect rt;
	GetClientRect(rt);

	HWND hWnd = ::GetDesktopWindow();
	m_image.captureWindow(this->m_hWnd, hWnd);
	//image.save("c:\\2.jpg");
	//m_image.drawEx(pDC->m_hDC, rt);

	CBrush brush(RGB(128, 128, 128));
	CBrush* pOldBrush = pDC->SelectObject(&brush);
	BackDC.FillRect(rt, &brush);

	CRect rt1 = CalcRect(m_image.getWidth(), m_image.getHeight(), &rt);
	m_image.drawEx(BackDC.m_hDC, rt1);*/

	Invalidate();
}

void CViewDlg::PaintRGN()
{
	if (m_image.isValid())
	{
		CRect rtClient;
		GetClientRect(rtClient);
		CRect rtImage =  CalcRect(m_image.getWidth(), m_image.getHeight(), &rtClient);

		CRgn rgn1;
		rgn1.CreateRectRgnIndirect(rtClient);
		CRgn rgn2 ;
		rgn2.CreateRectRgnIndirect(rtImage);
		
		rgn1.CombineRgn(&rgn1,&rgn2,RGN_DIFF);
		
		CBrush brush(RGB(128, 128, 128));
		CBrush* oldBrush = BackDC.SelectObject(&brush);
		BackDC.PaintRgn(&rgn1);
		BackDC.SelectObject(oldBrush);
	}
}

void CViewDlg::OnDropFiles(HDROP hDropInfo) 
{
	// TODO: Add your message handler code here and/or call default
	DWORD nSize = DragQueryFile(hDropInfo,0,NULL,0);
	char * pFile = new char[nSize+1];
	UINT nCount = DragQueryFile(hDropInfo,0,pFile,nSize+1);
	//OpenFileAndAddList(szFile);
	CString szFile = pFile;
	delete pFile;
	pFile = NULL;
	OpenFile(szFile);

	CDialog::OnDropFiles(hDropInfo);
}

void CViewDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	this->SetFocus();
	CDialog::OnLButtonDown(nFlags, point);
}
