// Track.cpp : implementation file
//

#include "stdafx.h"
#include "CatchScreenTrack.h"
#include "CatchScreenDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CCatchScreenTrack

//IMPLEMENT_DYNCREATE(CCatchScreenTrack, CFrameWnd)

CCatchScreenTrack::CCatchScreenTrack()
{

}

CCatchScreenTrack::~CCatchScreenTrack()
{
}

void CCatchScreenTrack::DrawTrackerRect( LPCRECT lpRect, CWnd* pWndClipTo,CDC* pDC, CWnd* pWnd )
{	    
    //更新窗口,消除TRACK时的虚线框
    ((CCatchScreenDlg *)pWnd)->PaintWindow();
	((CCatchScreenDlg *)pWnd)->SendMessage(WM_MOUSEMOVE);		
}

