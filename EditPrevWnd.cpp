// EditPrevWnd.cpp : implementation file
//

#include "stdafx.h"
#include "EditPrevWnd.h"
#include "layer/PFData.h"
#include "PhotoEditDlg.h"
//#include "EditResizeDlg.h"

#include "math.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditPrevWnd

CEditPrevWnd::CEditPrevWnd(CWnd* pParent /*=NULL*/)
{
	m_pParentWnd = pParent;
	m_ptChanged = CPoint(0,0);
	m_ptCenter = CPoint(0,0);
	m_ptShownLt = CPoint(0,0);
	m_rcFore = CRect(0,0,0,0);
	m_szMoved = CSize(0,0);
	m_bMoveEnabled = FALSE;
	m_bMoveFore = FALSE;
	m_bForeSelected = FALSE;

	m_bRectExist = FALSE;
	m_bAfterCrop = FALSE;

	m_nScalInd = 6;			// dScaling[6] = 1.0
	m_dScaling = dScaling[m_nScalInd];
	m_dLastScaling = m_dScaling;
//	m_pEditResizeDlg = NULL;
	m_bLBtnDown = FALSE;
	m_bCropPress = FALSE;
	m_OldPoint = CPoint(0,0);
	m_PrePoint = CPoint(0,0);
	m_rtPreRect = CRect(0,0,0,0);
	//m_pBmpInfo = NULL;
	//m_pBmpData = NULL;
}

CEditPrevWnd::~CEditPrevWnd()
{
}


BEGIN_MESSAGE_MAP(CEditPrevWnd, CWnd)
	//{{AFX_MSG_MAP(CEditPrevWnd)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CEditPrevWnd message handlers

void CEditPrevWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	Draw(&dc);



	// Do not call CWnd::OnPaint() for painting messages
}

VOID CEditPrevWnd::Draw(CDC* pDC)
{

	LPBYTE       lpData = NULL;
	LPBITMAPINFO lpInfo = NULL;

		if(((CPhotoEditDlg*)m_pParentWnd)->GetCurObject())
			
			(((CPhotoEditDlg*)m_pParentWnd)->GetCurObject())->GetObjectData(lpData, lpInfo);
	 		
		else
			return;

		if((!lpData) || (!lpInfo))
			return;
	
	

	LPBITMAPINFOHEADER lpInfoHeader = &(lpInfo->bmiHeader);

	LONG lWidth = 0 ,	lHeight = 0;		// Width and height to show
	LONG lScalW = 0 ,   lScalH  = 0;		// Width and height of scaled image
	lScalW  = (LONG)(lpInfoHeader->biWidth  * m_dScaling);
	lScalH  = (LONG)(lpInfoHeader->biHeight * m_dScaling);
	lWidth  = (PREVWNDWIDTH < lScalW) ? PREVWNDWIDTH : lScalW;
	lHeight = (PREVWNDHEIGH < lScalH) ? PREVWNDHEIGH : lScalH;

	CPoint ptImgShowLt(0,0);				// Left top position of shown image part
	ptImgShowLt.x = (LONG)(((lScalW - lWidth )/2 - m_szMoved.cx) / m_dScaling);
	ptImgShowLt.y = (LONG)(((lScalH - lHeight)/2 + m_szMoved.cy) / m_dScaling);
	
	if(ptImgShowLt.x < 0)		ptImgShowLt.x = 0;
	else if(ptImgShowLt.x > lpInfoHeader->biWidth - (LONG)(lWidth / m_dScaling))
		ptImgShowLt.x = lpInfoHeader->biWidth - (LONG)(lWidth / m_dScaling);
	if(ptImgShowLt.y < 0)		ptImgShowLt.y = 0;
	else if(ptImgShowLt.y > lpInfoHeader->biHeight - (LONG)(lHeight / m_dScaling))
		ptImgShowLt.y = lpInfoHeader->biHeight - (LONG)(lHeight / m_dScaling);

	// m_ptShownLt is for move fore image
	m_ptShownLt.x = ptImgShowLt.x - (LONG)((PREVWNDWIDTH-lWidth)/(2*m_dScaling));
	m_ptShownLt.y = lpInfoHeader->biHeight - ptImgShowLt.y - (LONG)((PREVWNDHEIGH + lHeight)/(2*m_dScaling));

	//Draw image
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC, lWidth, lHeight);
	CBitmap* pBmp = memDC.SelectObject(&bmp);
	memDC.SetStretchBltMode(COLORONCOLOR);
	::StretchDIBits(memDC.m_hDC, 0, 0, lWidth, lHeight, 
			ptImgShowLt.x, ptImgShowLt.y, (LONG)(lWidth / m_dScaling),  (LONG)(lHeight / m_dScaling),
			lpData, (LPBITMAPINFO)lpInfoHeader, DIB_RGB_COLORS, SRCCOPY);
	
	pDC->SetStretchBltMode(COLORONCOLOR);
	pDC->BitBlt((PREVWNDWIDTH-lWidth)/2, (PREVWNDHEIGH-lHeight)/2, lWidth, lHeight,&memDC,0,0,SRCCOPY);
	
	m_rtImageRect = CRect((PREVWNDWIDTH-lWidth)/2, (PREVWNDHEIGH-lHeight)/2, 
		              (PREVWNDWIDTH-lWidth)/2 + lWidth, (PREVWNDHEIGH-lHeight)/2 + lHeight);
	
	memDC.SelectObject(pBmp);
	memDC.DeleteDC();
	bmp.DeleteObject();

	//Draw fore image rect
	if(m_bForeSelected)
	{
		CGdiObject * Oldbr;
		CPen pen, *Oldpen;
		pen.CreatePen(PS_SOLID,2,RGB(0,255,0));
		Oldpen = pDC->SelectObject(&pen);
		Oldbr  = pDC->SelectStockObject(NULL_BRUSH);

		CRect rc1 = m_rcFore;
		rc1.OffsetRect(-m_ptShownLt.x, -m_ptShownLt.y);
		rc1.left   = (LONG)(rc1.left*m_dScaling);
		rc1.top    = (LONG)(rc1.top*m_dScaling);
		rc1.right  = (LONG)(rc1.right*m_dScaling);
		rc1.bottom = (LONG)(rc1.bottom*m_dScaling);

		pDC->Rectangle(rc1);
		pDC->SelectObject(Oldpen);
		pDC->SelectObject(Oldbr);
	}
	if(m_bCropPress)// && m_bAfterCrop)
		if(m_bRectExist)
			DrawRect(pDC,m_rtPreRect);/////keep rect when Crop	
		
	
}

// Draw fore image, used when moving fore image, it can improve move speed
VOID CEditPrevWnd::DrawFore(CDC* pDC)
{
	//Draw fore image rect
	if(m_bMoveFore)
	{
		LPBYTE       lpData = NULL;
		LPBITMAPINFO lpInfo = NULL;
	
	
			if(((CPhotoEditDlg*)m_pParentWnd)->GetCurObject())
			
				(((CPhotoEditDlg*)m_pParentWnd)->GetCurObject())->GetObjectData(lpData, lpInfo);
			
			else
				return;

			if((!lpData) || (!lpInfo))
				return;
		
			
			LPBITMAPINFOHEADER lpInfoHeader = &(lpInfo->bmiHeader);

			CRect rc1 = m_rcFore;
			rc1.OffsetRect(-m_ptShownLt.x, -m_ptShownLt.y);
			rc1.left   = (LONG)(rc1.left*m_dScaling);
			rc1.top    = (LONG)(rc1.top*m_dScaling);
			rc1.right  = (LONG)(rc1.right*m_dScaling);
			rc1.bottom = (LONG)(rc1.bottom*m_dScaling);

			CDC memDC;
			memDC.CreateCompatibleDC(pDC);
			CBitmap bmp;
			bmp.CreateCompatibleBitmap(pDC, rc1.Width(), rc1.Height());
			CBitmap* pBmp = memDC.SelectObject(&bmp);
			memDC.SetStretchBltMode(COLORONCOLOR);
			::StretchDIBits(memDC.m_hDC, 0, 0, rc1.Width(), rc1.Height(), 
					m_rcFore.left, lpInfoHeader->biHeight-m_rcFore.top-m_rcFore.Height(), m_rcFore.Width(), m_rcFore.Height(),
					lpData, (LPBITMAPINFO)lpInfoHeader, DIB_RGB_COLORS, SRCCOPY);
			
			pDC->SetStretchBltMode(COLORONCOLOR);
			pDC->BitBlt(rc1.left, rc1.top, rc1.Width(), rc1.Height(), &memDC, 0, 0, SRCCOPY);

			memDC.SelectObject(pBmp);
			memDC.DeleteDC();
			bmp.DeleteObject();

			CGdiObject * Oldbr;
			CPen pen, *Oldpen;
			pen.CreatePen(PS_SOLID,2,RGB(0,255,0));
			Oldpen = pDC->SelectObject(&pen);
			Oldbr  = pDC->SelectStockObject(NULL_BRUSH);

			pDC->Rectangle(rc1);
			pDC->SelectObject(Oldpen);
			pDC->SelectObject(Oldbr);
		}
}

BOOL CEditPrevWnd::OnEraseBkgnd(CDC* pDC) 
{
/*	CRect rc;
	GetClientRect(rc);

	CBitmap  Bmp;
	CBitmap* pOldBmp;
	Bmp.Attach(LoadBitmapFromResPH(IDB_EDIT_MAINUI));

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	pOldBmp = memDC.SelectObject(&Bmp);
	pDC->BitBlt(0,0,rc.Width(),rc.Height(), &memDC, 227, 90, SRCCOPY); // 227/90 : PrevWnd left top point
	memDC.SelectObject(pOldBmp);
	memDC.DeleteDC();
	Bmp.DeleteObject();*/

	return CWnd::OnEraseBkgnd(pDC);
}

VOID CEditPrevWnd::ResetPreview()
{
	m_ptChanged = CPoint(0,0);
	m_ptCenter = CPoint(0,0);
	m_szMoved = CSize(0,0);
	m_rcFore = CRect(0,0,0,0);
	m_bMoveEnabled = FALSE;
	m_bMoveFore = FALSE;
	m_bForeSelected = FALSE;
	m_nScalInd = 6;			// dScaling[6] = 1.0
	m_dScaling = dScaling[m_nScalInd];
	m_dLastScaling = m_dScaling;

	m_ObjSize = CSize(0,0);
	if(((CPhotoEditDlg*)m_pParentWnd)->GetCurObject())
	{
		LPBYTE       pTempData = NULL;
		LPBITMAPINFO pTempInfo = NULL;
		(((CPhotoEditDlg*)m_pParentWnd)->GetCurObject())->Draw();
		(((CPhotoEditDlg*)m_pParentWnd)->GetCurObject())->GetObjectData(pTempData, pTempInfo);
		if(pTempInfo)
			m_ObjSize = CSize(pTempInfo->bmiHeader.biWidth,pTempInfo->bmiHeader.biHeight);
	}
	m_ptCenter = CPoint(m_ObjSize.cx/2,m_ObjSize.cy/2);

	if( (m_ObjSize.cx <= PREVWNDWIDTH) && (m_ObjSize.cy <= PREVWNDHEIGH) )
	{
		m_lWidth  = (PREVWNDWIDTH < m_ObjSize.cx) ? PREVWNDWIDTH : m_ObjSize.cx;
		m_lHeight = (PREVWNDHEIGH < m_ObjSize.cy) ? PREVWNDHEIGH : m_ObjSize.cy;
		m_ObjScalSize = m_ObjSize;

		Invalidate();
	}
	else
		ViewFitInWindow();

	//change the scaling value shown in tip
//	((CPhotoEditDlg*)m_pParentWnd)->ShowZoomTip();
}

VOID CEditPrevWnd::ViewFitInWindow()
{
	m_dLastScaling = m_dScaling;
	if(PREVWNDWIDTH * m_ObjSize.cy >= PREVWNDHEIGH * m_ObjSize.cx)
	{
		m_dScaling = (DOUBLE)PREVWNDHEIGH / m_ObjSize.cy;
	}
	else
	{
		m_dScaling = (DOUBLE)PREVWNDWIDTH / m_ObjSize.cx;
	}

	UpdateScaledPara();

	Invalidate();

	CalScalingInd();
}

VOID CEditPrevWnd::ViewActualSize()
{
	m_dLastScaling = m_dScaling;
	m_dScaling = 1.0;
	m_nScalInd = 6;			// dScaling[6] = 1.0

	UpdateScaledPara();

	Invalidate();
}

VOID CEditPrevWnd::ViewWithSize(DOUBLE dValue)
{
	m_dLastScaling = m_dScaling;
	m_dScaling = dValue;

	UpdateScaledPara();

	Invalidate();

	CalScalingInd();
}

VOID CEditPrevWnd::CalScalingInd()
{
	SHORT nIndex = 6;

	for(SHORT i = 0; i < 15; i++)
	{
		if(m_dScaling < dScaling[i])
		{
			nIndex = SHORT(i - 1);
			break;
		}
	}

	if(nIndex < 0)	nIndex = 0;

	if(m_dScaling >= dScaling[15])
		nIndex = 15;

	m_nScalInd = nIndex;
}

VOID CEditPrevWnd::ZoomOut()
{
	m_nScalInd --;
	if(m_nScalInd < 0)
	{
		m_nScalInd = 0;
		return ;
	}
	m_dLastScaling = m_dScaling;
	m_dScaling = dScaling[m_nScalInd];

	UpdateScaledPara();

	Invalidate();
}

VOID CEditPrevWnd::ZoomIn()
{
	m_nScalInd ++;
	if(m_nScalInd > 15)
	{
		m_nScalInd = 15;
		return ;
	}
	m_dLastScaling = m_dScaling;
	m_dScaling = dScaling[m_nScalInd];

	UpdateScaledPara();

	Invalidate();
}

VOID CEditPrevWnd::UpdateScaledPara()

{
	m_ObjScalSize = CSize((LONG)(m_ObjSize.cx * m_dScaling), (LONG)(m_ObjSize.cy * m_dScaling));

	m_lWidth  = (PREVWNDWIDTH < m_ObjScalSize.cx) ? PREVWNDWIDTH : m_ObjScalSize.cx;
	m_lHeight = (PREVWNDHEIGH < m_ObjScalSize.cy) ? PREVWNDHEIGH : m_ObjScalSize.cy;

	m_ptCenter = CPoint((LONG)(m_ptCenter.x * m_dScaling / m_dLastScaling),
						(LONG)(m_ptCenter.y * m_dScaling / m_dLastScaling));

	m_szMoved  = CSize((LONG)(m_szMoved.cx * m_dScaling / m_dLastScaling),
						(LONG)(m_szMoved.cy * m_dScaling / m_dLastScaling));
}
void CEditPrevWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{

	if(!m_bCropPress)
	{
		::SetCursor(::AfxGetApp()->LoadCursor(IDC_CURSOR_HAND_DOWN));
		m_ptMoveStart = point;
		m_ptChanged = CPoint(0,0);
		m_bMoveEnabled = TRUE;
		if(((CPhotoEditDlg*)m_pParentWnd)->GetCurObject())
		{
			OBJECTTYPE iObjType = (((CPhotoEditDlg*)m_pParentWnd)->GetCurObject())->GetObjectType();
			if( (iObjType == OBJECT_CARD) || (iObjType == OBJECT_CALENDAR) || (iObjType == OBJECT_STATIONARY) )
			{
				CRect rc;
				(((CPhotoEditDlg*)m_pParentWnd)->GetCurObject())->GetForeImageRect(rc);
				CPoint pt = CPoint(m_ptShownLt.x + (LONG)(point.x/m_dScaling), m_ptShownLt.y + (LONG)(point.y/m_dScaling));
				CDC* pDC = GetDC();
				if(rc.PtInRect(pt))
				{
					m_rcFore = rc;
					m_bMoveFore = TRUE;
					m_bForeSelected = TRUE;
					Draw(pDC);
				}
				else if(m_bForeSelected)
				{
					m_bForeSelected = FALSE;
					Draw(pDC);
				}
				ReleaseDC(pDC);
			}
		}
		SetCapture();
	}
	else
	{
		if(!m_rtImageRect.PtInRect(point))
			return;
		if(m_rtPreRect.PtInRect(point))
		{
			m_bRectExist =	FALSE;
			m_rtCropRect = m_rtPreRect;
		}
		else
		{
			InvalidateItemRect(m_rtPreRect);
			m_rtPreRect = CRect(point.x,point.y,point.x,point.y);
			if(m_bAfterCrop)
			{
				InvalidateItemRect(m_rtCropRect);
				m_rtCropRect = CRect(point.x,point.y,point.x,point.y);
				m_bAfterCrop = FALSE;
			}
		   	::SetCursor(::AfxGetApp()->LoadStandardCursor(IDC_CROSS));
			SetCapture();
			m_OldPoint = point;
			m_PrePoint = point;
		}
	}
	CWnd::OnLButtonDown(nFlags, point);
}

void CEditPrevWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(!m_bCropPress)
	{
		if(m_bMoveEnabled)
		{
			m_ptChanged = point - m_ptMoveStart;
			m_ptMoveStart = point;

			if(!m_bMoveFore)
			{
				if(m_ptChanged != CPoint(0,0))
					PointContrast();
			}
			else	//move fore image
			{
				CSize sz(0,0);
				sz = CSize((LONG)(m_ptChanged.x/m_dScaling), (LONG)(m_ptChanged.y/m_dScaling));
				if(sz != CSize(0,0))
				{
					(((CPhotoEditDlg*)m_pParentWnd)->GetCurObject())->MoveForeImage(sz);
					((CPhotoEditDlg*)m_pParentWnd)->SetCurImageModified(true);
					CDC* pDC = GetDC();
					DrawFore(pDC);
					ReleaseDC(pDC);
				}
			}
		}
		else
		{
			::SetCursor(::AfxGetApp()->LoadCursor(IDC_CURSOR_HAND_ON));
		}
	}
	else
	{
		if(((CPhotoEditDlg*)m_pParentWnd)->GetEditType() == EDIT_RESIZE)
		{
		if(!m_rtImageRect.PtInRect(point))
			return;
		::SetCursor(::AfxGetApp()->LoadStandardCursor(IDC_CROSS));
		{
			
			if((nFlags & MK_LBUTTON) == MK_LBUTTON)//&&(m_rtImageRect.PtInRect(m_OldPoint)))
			{
				if(m_bAfterCrop)
				{
					Invalidate();
					m_bAfterCrop = FALSE;
				}
			
				CDC * pDC = GetDC();
				CRect rtOld;
				if(m_OldPoint.x < m_rtImageRect.left)
					 m_OldPoint.x = m_rtImageRect.left;
				if (m_OldPoint.x > m_rtImageRect.right)
					 m_OldPoint.x = m_rtImageRect.right;
				if(m_OldPoint.y < m_rtImageRect.top)
					 m_OldPoint.y = m_rtImageRect.top;
				if (m_OldPoint.y > m_rtImageRect.bottom)
					 m_OldPoint.y = m_rtImageRect.bottom;
				int x1 = m_OldPoint.x;
				int y1 = m_OldPoint.y;
				int x2 = m_PrePoint.x;
				int y2 = m_PrePoint.y;
				rtOld.left = min(x1,x2);
				rtOld.top = min(y1,y2);
				rtOld.right = max(x1,x2);
				rtOld.bottom = max(y1,y2);
				DrawRect(pDC,rtOld);
				if(point.x < m_rtImageRect.left)
					 point.x = m_rtImageRect.left;
				if (point.x > m_rtImageRect.right)
					 point.x = m_rtImageRect.right;
				if(point.y < m_rtImageRect.top)
					 point.y = m_rtImageRect.top;
				if (point.y > m_rtImageRect.bottom)
					 point.y = m_rtImageRect.bottom;
				m_PrePoint = point;
				CRect rtNew;
				x1 = m_OldPoint.x;
				y1 = m_OldPoint.y;
				x2 = point.x;
				y2 = point.y;
				rtNew.left = min(x1,x2);
				rtNew.top = min(y1,y2);
				rtNew.right = max(x1,x2);
				rtNew.bottom = max(y1,y2);
				m_rtPreRect = rtNew;
				DrawRect(pDC,m_rtPreRect);
			
				m_bRectExist = TRUE;
				//Draw(pDC);
				ReleaseDC(pDC);
			}
			}
		}
	}
	CWnd::OnMouseMove(nFlags, point);
}

void CEditPrevWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_bMoveFore = FALSE;
	m_bMoveEnabled = FALSE;
	ReleaseCapture();
	CWnd::OnLButtonUp(nFlags, point);
}
void CEditPrevWnd::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
/*	if((m_bCropPress)&&(m_rtCropRect.PtInRect(point))&&(m_bAfterCrop == FALSE))
	{
		if(((CPhotoEditDlg*)m_pParentWnd)->GetEditType() == EDIT_RESIZE)
		{
			CEditResizeDlg* pEditResizeDlg = (CEditResizeDlg*)(((CPhotoEditDlg*)m_pParentWnd)->GetOperatDlg());
			
			CRect rect1;

			if(m_dScaling != 0)

			{
				POINT point;
				if(m_ptShownLt.x > 0)
					point.x = m_ptShownLt.x;
				else
					point.x = 0;
				if(m_ptShownLt.y >0)
					point.y = m_ptShownLt.y;
				else
					point.y = 0;
			
				rect1.left = (long)((m_rtPreRect.left - m_rtImageRect.left +point.x*m_dScaling)/m_dScaling);
				rect1.top = (long)((m_rtPreRect.top - m_rtImageRect.top+point.y*m_dScaling)/m_dScaling);
				rect1.right = (long)(rect1.left + m_rtPreRect.Width()/m_dScaling);
				rect1.bottom = (long)(rect1.top + m_rtPreRect.Height()/m_dScaling);
				
				m_rtCropRect.left = (long)(rect1.left*m_dScaling);
				m_rtCropRect.right = (long)(rect1.right*m_dScaling);
				m_rtCropRect.top = (long)(rect1.top*m_dScaling);
				m_rtCropRect.bottom = (long)(rect1.bottom*m_dScaling);

				pEditResizeDlg->CropImage(rect1);

				m_bRectExist = FALSE;
				pEditResizeDlg->m_btnCrop.SetState(BS_NORMAL);
				m_bCropPress = FALSE;
				m_bAfterCrop = TRUE;
				InvalidateItemRect(m_rtCropRect);
			}
		}
	}
	else
	{
		::SendMessage(m_pParentWnd->m_hWnd,WM_COMMAND,MAKEWPARAM(IDC_BN_FULLSCR,0),0);
	}*/
	CWnd::OnLButtonDblClk(nFlags, point);
}

void CEditPrevWnd::Refresh()
{
	CDC * pDC = GetDC();
	DrawRect(pDC,m_rtPreRect);
	ReleaseDC(pDC);
	//Invalidate();
}

VOID CEditPrevWnd::PointContrast()
{
	CPoint ptTemp = m_ptCenter + m_ptChanged;
	if(ptTemp.x < m_lWidth/2)								ptTemp.x = m_lWidth/2;
	else if(ptTemp.x > m_ObjScalSize.cx - m_lWidth/2)		ptTemp.x = m_ObjScalSize.cx - m_lWidth/2;
	if(ptTemp.y < m_lHeight/2)								ptTemp.y = m_lHeight/2;
	else if(ptTemp.y > m_ObjScalSize.cy - m_lHeight/2)		ptTemp.y = m_ObjScalSize.cy - m_lHeight/2;

	if(ptTemp == m_ptCenter)
		return ;
	m_ptCenter = ptTemp;
	m_szMoved.cx = m_ptCenter.x - m_ObjScalSize.cx/2;
	m_szMoved.cy = m_ptCenter.y - m_ObjScalSize.cy/2;

	CDC* pDC = GetDC();
	Draw(pDC);
	ReleaseDC(pDC);
}


DOUBLE CEditPrevWnd::GetPrevScaling()
{
	return m_dScaling;
}

void CEditPrevWnd::DrawRect(CDC *pDC, CRect rt)
{
	//INT nMode = pDC->SetROP2(R2_NOT);
	CPen pen(PS_DOT, 1, RGB(192, 192, 192));
	CPen* p = pDC->SelectObject(&pen);
	//pDC->SetROP2(R2_NOT);
	pDC->SetROP2(R2_XORPEN);
	pDC->MoveTo(rt.left,rt.top);
	pDC->LineTo(rt.right,rt.top);
	pDC->LineTo(rt.right,rt.bottom);
	pDC->LineTo(rt.left,rt.bottom);
	pDC->LineTo(rt.left,rt.top);
	pDC->SelectObject(p);
	pen.DeleteObject();
	//pDC->SetROP2(nMode);
	//pDC->SetROP2(R2_XORPEN);

}

void CEditPrevWnd::InvalidateItemRect(CRect rect)
{
	CRect rt1 = CRect(rect.left-1,rect.top-1,rect.left+1,rect.bottom+1);
	CRect rt2 = CRect(rect.left-1,rect.top-1,rect.right+1,rect.top+1);
	CRect rt3 = CRect(rect.right-1,rect.top-1,rect.right+1,rect.bottom+1);
	CRect rt4 = CRect(rect.left-1,rect.bottom-1,rect.right+1,rect.bottom+1);
	InvalidateRect(rt1);
	InvalidateRect(rt2);
	InvalidateRect(rt3);
	InvalidateRect(rt4);
}



