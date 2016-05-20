#if !defined(AFX_EDITPREVWND_H__5CD3EFE1_ABB5_11D3_BC69_0090272293F9__INCLUDED_)
#define AFX_EDITPREVWND_H__5CD3EFE1_ABB5_11D3_BC69_0090272293F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditPrevWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditPrevWnd window
class CEditPrevWnd : public CWnd
{
// Construction
public:
	void DrawRect(CDC* pDC,CRect rt);
	CEditPrevWnd(CWnd* pParent = NULL);
	virtual ~CEditPrevWnd();
	VOID ResetPreview();
	VOID ViewFitInWindow();
	VOID ViewActualSize();
	VOID ViewWithSize(DOUBLE dValue);
	VOID ZoomIn();
	VOID ZoomOut();
	DOUBLE GetPrevScaling();
	void Refresh();
	void InvalidateItemRect(CRect rect);


private:
	VOID PointContrast();			//Contrast the point position
	VOID UpdateScaledPara();		//Scaling changed, update the parameters
	VOID CalScalingInd();			//Calculate the nearest scaling data's index
	VOID Draw(CDC* pDC);			//Draw whole image
	VOID DrawFore(CDC* pDC);		//Draw fore image (Add this function, just in order to improve fore image move speed.)

private:
	CWnd*  m_pParentWnd;
	CPoint m_ptMoveStart;			// Point of mouse move start
	CPoint m_ptChanged;				// Moved distance
	CPoint m_ptCenter;				// Point of image center
	CSize  m_szMoved;				// Moved size

	BOOL   m_bMoveEnabled;			// Enable moving
	CSize  m_ObjSize;				// Object size
	CSize  m_ObjScalSize;			// Scaled object size
	LONG   m_lWidth;				// Shown image width
	LONG   m_lHeight;				// Shown image height

	SHORT  m_nScalInd;				// Scaling index in dScaling[]
	DOUBLE m_dScaling;				// Scaling value
	DOUBLE m_dLastScaling;			// Last scaling value

	// Fore image move (Card, Calendar, Stationary)
	CPoint m_ptShownLt;				// Point of shown image left top
	BOOL   m_bMoveFore;				// If move fore image
	BOOL   m_bForeSelected;			// Mouse selected fore image area
	BOOL   m_bRectExist;
	CRect  m_rcFore;				// Fore image rect


	
	
public:
	CPoint m_OldPoint;
	CPoint m_PrePoint;
	BOOL   m_bLBtnDown;
	BOOL m_bAfterCrop;
//	BOOL CropImage(CRect rect);
	BOOL   m_bExistRect;
	CRect m_rtImageRect;
	BOOL m_bCropPress;
	CRect m_rtPreRect;
	CRect m_rtCropRect;

	//LPBITMAPINFO	m_pBmpInfo;		/* image's bitmap info (edited) */
	//LPBYTE			m_pBmpData;	/* image's data block (edited) */

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditPrevWnd)
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CEditPrevWnd)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITPREVWND_H__5CD3EFE1_ABB5_11D3_BC69_0090272293F9__INCLUDED_)
