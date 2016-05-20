// CatchScreenDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CatchScreenDlg.h"
#include ".\catchscreendlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

#define SHIFTED 0x8000 
#define  IDC_EDIT_BACKGRND 1
// CCatchScreenDlg dialog

CCatchScreenDlg::CCatchScreenDlg(UINT unIDD, UINT unCursorID, UINT unEditBitmapID, 
								 COLORREF clrTextColor, 
								 COLORREF clrWndColor,
								 COLORREF clrWndBackgndColor,
								 CWnd* pParent /*=NULL*/)
	: CDialog(unIDD, pParent)
	, m_bSleeping (true)
	, m_wndTipEdit(clrTextColor)
	, m_clrTextColor(clrTextColor)
	, m_clrWndFrameColor(clrWndColor)
	, m_clrWndBackgndColor(clrWndBackgndColor)
{
	//{{AFX_DATA_INIT(CCatchScreenDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    //初始化像皮筋类
	m_rectTracker.m_nStyle = CRectTracker::resizeInside | CRectTracker::solidLine;  
	m_rectTracker.m_rect.SetRect(-1, -1, -1, -1);
    m_hCursor = AfxGetApp()->LoadCursor(unCursorID);  
	m_bDraw = FALSE;
	m_bDrawTracker = FALSE;
	m_bQuit = FALSE;
	m_bShowMsg = FALSE;
    m_startPt = 0;
    
	//获取屏幕分辩率
	m_nCXScreen = GetSystemMetrics(SM_CXSCREEN);
	m_nCYScreen = GetSystemMetrics(SM_CYSCREEN);
	m_wndTipEdit.LoadBkgrndBitmap(unEditBitmapID);
	m_wndTipEdit.SetBkgrndBitmapID(unEditBitmapID);
	//一定要初始化m_rgn，
    m_rgnUpdate.CreateRectRgn(0,0,50,50);
	
}
CCatchScreenDlg::~CCatchScreenDlg()
{
	HBITMAP hBitmap = (HBITMAP)m_BackgroundBitmap.Detach();
	::DeleteObject(hBitmap);

	hBitmap = (HBITMAP)m_OrgBackgroundBitmap.Detach();
	::DeleteObject(hBitmap);
}
void CCatchScreenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCatchScreenDlg)
	//DDX_Control(pDX, IDC_EDIT1, m_tipEdit);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCatchScreenDlg, CDialog)
	//{{AFX_MSG_MAP(CCatchScreenDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_ERASEBKGND()
	ON_WM_SETCURSOR()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCatchScreenDlg message handlers

BOOL CCatchScreenDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// register system hot key
	HWND handle = GetSafeHwnd ();
	
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	BITMAP bitmapInfo;
	CBitmap* pBitmap = m_wndTipEdit.GetBkgrndBitmap();
	pBitmap->GetBitmap(&bitmapInfo);
	m_wndTipEdit.Create( ES_MULTILINE|WS_CHILD | WS_VISIBLE  ,
		CRect(0, 0, bitmapInfo.bmWidth, bitmapInfo.bmHeight), this, IDC_EDIT_BACKGRND);
	m_wndTipEdit.ModifyStyleEx(NULL,WS_EX_TRANSPARENT);
	
	// TODO: Add extra initialization here
	Sleep ();
	CaptureIt();
	this->UpdateWindow ();
	//::PostMessage (this->GetSafeHwnd (), WM_RBUTTONDOWN, 0 , 0);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCatchScreenDlg::OnPaint() 
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
		if(!m_BackgroundBitmap.GetSafeHandle())
		{
			return;
		}
		CPaintDC dc(this);
		DoPaint(dc);
		//DrawTip();
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCatchScreenDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CCatchScreenDlg::OnOK() 
{
	CDialog::OnOK();
}

void CCatchScreenDlg::OnCancel() 
{
	if(m_bDrawTracker)
	{
		m_bDrawTracker=FALSE;
		m_bDraw=FALSE;
		m_rectTracker.m_rect.SetRect(-1,-1,-1,-1);
		PaintWindow();
		this->SendMessage (WM_SETCURSOR, 0 , 0);
	}
	else
	{
		CDialog::OnCancel();
		//PostMessage(WM_CLOSE);
	}
}

void CCatchScreenDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(m_bDraw)
	{
		m_rectTracker.m_rect.SetRect(m_startPt.x + 1,m_startPt.y + 1,point.x, point.y);
		m_bDrawTracker=TRUE;
		
		PaintWindow();
	}
	CClientDC dc(this);
	DrawTip(dc);
	CDialog::OnMouseMove(nFlags, point);
}

void CCatchScreenDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	int nHitTest;
	nHitTest = m_rectTracker.HitTest(point);
	m_bShowMsg=TRUE;
	PaintWindow();

    //判断击中位置
	if(nHitTest < 0)
	{
		if(!m_bDrawTracker)
		{
			m_startPt=point;
			m_bDraw=TRUE;
						
			PaintWindow();
		}
		else{
			m_bDrawTracker=FALSE;
			m_rectTracker.m_rect.SetRect(-1, -1, -1, -1);
			m_startPt=point;
			m_bDraw=TRUE;
			
			PaintWindow();
		}
	}
	else
	{
		if(m_bDrawTracker)
		{
			m_rectTracker.Track(this, point, TRUE);
			SendMessage(WM_LBUTTONUP, NULL, NULL);
			PaintWindow();

		}
	}

	CDialog::OnLButtonDown(nFlags, point);
}

void CCatchScreenDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_bShowMsg = FALSE;
	m_bDraw = FALSE;
	PaintWindow();

	CDialog::OnLButtonUp(nFlags, point);
}

void CCatchScreenDlg::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnLButtonDblClk(nFlags, point);
	int nHitTest;
	nHitTest=m_rectTracker.HitTest(point);

	if(nHitTest == CRectTracker::hitMiddle)
	{
		m_rectTracker.m_rect.NormalizeRect ();
		CopyScreenToClipboard(m_rectTracker.m_rect);
		SendMessage(WM_COMMAND, IDOK);
	}
}

void CCatchScreenDlg::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CDialog::OnRButtonDown(nFlags, point);
	if(m_bDrawTracker)
	{
		//如果已经截取矩则清除截取矩形
		m_bDrawTracker=FALSE;
		m_rectTracker.m_rect.SetRect(-1, -1, -1, -1);
		PaintWindow();
	}
	else 
	{
		CDialog::OnCancel();
	}
}

BOOL CCatchScreenDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	//设置改变截取矩形大小时光标
	if (pWnd == this && m_rectTracker.SetCursor(this, nHitTest)) 
    {
		return TRUE; 
	}
	else
	{
		//设置彩色光标
		SetCursor(m_hCursor);
		return TRUE;
	}
}

//*增加的函数
//考贝屏幕
HBITMAP CCatchScreenDlg::CopyScreenToBitmap(LPRECT lpRect, BOOL bSave)
//lpRect 代表选定区域
{
	HDC hScrDC, hMemDC;      
	// 屏幕和内存设备描述表
	HBITMAP    hBitmap, hOldBitmap;   
	// 位图句柄
	int       nX, nY, nX2, nY2;      
	// 选定区域坐标
	int       nWidth, nHeight;
	
	// 确保选定区域不为空矩形
	if (IsRectEmpty(lpRect))
		return NULL;
	//为屏幕创建设备描述表
	hScrDC = CreateDC("DISPLAY", NULL, NULL, NULL);

	//为屏幕设备描述表创建兼容的内存设备描述表
	hMemDC = CreateCompatibleDC(hScrDC);
	// 获得选定区域坐标
	nX = lpRect->left;
	nY = lpRect->top;
	nX2 = lpRect->right;
	nY2 = lpRect->bottom;

	//确保选定区域是可见的
	if (nX < 0)
		nX = 0;
	if (nY < 0)
		nY = 0;
	if (nX2 > m_nCXScreen)
		nX2 = m_nCXScreen;
	if (nY2 > m_nCYScreen)
		nY2 = m_nCYScreen;
	nWidth = nX2 - nX;
	nHeight = nY2 - nY;
	// 创建一个与屏幕设备描述表兼容的位图
	hBitmap = CreateCompatibleBitmap
		(hScrDC, nWidth, nHeight);
	
	// 把新位图选到内存设备描述表中
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
	// 把屏幕设备描述表拷贝到内存设备描述表中
	
	if(bSave)
	{
		CDC dcCompatible;
		dcCompatible.CreateCompatibleDC(CDC::FromHandle(hMemDC));
		dcCompatible.SelectObject(m_BackgroundBitmap);
        
		BitBlt(hMemDC, 0, 0, nWidth, nHeight,
			dcCompatible, nX, nY, SRCCOPY);
	}
	else
	{
		BitBlt(hMemDC, 0, 0, nWidth, nHeight,
			hScrDC, nX, nY, SRCCOPY);
	}

	hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);
	//得到屏幕位图的句柄
	//清除 
	DeleteDC(hScrDC);
	DeleteDC(hMemDC);
	// 返回位图句柄
	if(bSave)
	{
		if (OpenClipboard()) 
		{
			//清空剪贴板
			EmptyClipboard();
			//把屏幕内容粘贴到剪贴板上,
			//hBitmap 为刚才的屏幕位图句柄
			SetClipboardData(CF_BITMAP, hBitmap);
			//关闭剪贴板
			CloseClipboard();
		}
	}
	return hBitmap;
}

//显示操作提示信息
void CCatchScreenDlg::DrawTip(CDC &dc)
{
    
    //得当前坐标像素,
	CPoint pt;
	GetCursorPos(&pt);
	
	//pt.x+=3;
	//pt.y+=3;

	//当到当前R,G,B,各像素值
	COLORREF color;
	CDC dcCompatible;
	dcCompatible.CreateCompatibleDC(&dc);

	dcCompatible.SelectObject(m_OrgBackgroundBitmap);

	color = dcCompatible.GetPixel(pt);
	dcCompatible.DeleteDC();
	BYTE rValue, gValue, bValue;
	rValue = GetRValue(color);
	gValue = GetGValue(color);
	bValue = GetBValue(color);
	
	//按格式排放字符串
	CString string;
	CString strTemp;
	string.Format(_T("\r\n\r\n\r\n ·当前像素   RGB(%d,%d,%d)\r\n"),rValue, gValue, bValue);
    
	if(!m_bDraw&&!m_bDrawTracker)
	{
		strTemp = _T("\r\n ·按下鼠标左键不放选择截取范围\r\n\r\n ·按ESC键或鼠标右键退出");
	}
	else if(m_bDraw&&m_bDrawTracker)
	{
		strTemp = _T("\r\n ·松开鼠标左键确定截取范围\r\n\r\n ·按ESC键退出");
	}
	else if(m_bDrawTracker)
	{
		strTemp = _T("\r\n ·用鼠标左键调整截取范围的大小\r\n   和位置\r\n\r\n ·截取范围内双击鼠标左键保存图\r\n   像，结束操作\r\n\r\n ·点击鼠标右键重新选择");
	}
	string += strTemp;
	m_wndTipEdit.SetCurSelectedColor(color);
	//显示到编缉框中,操作提示窗口
	m_wndTipEdit.SetWindowText(string);
	m_wndTipEdit.SetInfoText(string);
}

//显示截取矩形信息
void CCatchScreenDlg::DrawMessage(CDC &dc, CRect &inRect)
{
	//截取矩形大小信息离鼠标间隔
	const int space=3;
    
	//设置字体颜色大小
	CPoint pt;
	CPen pen(PS_SOLID,1,m_clrWndFrameColor);
	dc.SetTextColor(m_clrTextColor);
	CFont font;
	font.CreatePointFont(90,_T("宋体"));
	CFont *pOldFont = (CFont*) dc.SelectObject(&font);

	//得到字体宽度和高度
	GetCursorPos(&pt);
	dc.SetBkMode(TRANSPARENT);
	TEXTMETRIC tm;
	int charHeight;
	CSize size;
	int	nLineLength;
	dc.GetTextMetrics(&tm);
	charHeight = tm.tmHeight + tm.tmExternalLeading;
	size = dc.GetTextExtent(_T("顶点位置  "), strlen(_T("顶点位置  ")));
	nLineLength = size.cx;
    
	//初始化矩形, 以保证写下六行文字
	CRect rect(pt.x + space, pt.y - charHeight * 6 - space, pt.x + nLineLength + space, pt.y - space);

	int x = GetDeviceCaps(dc, HORZRES);
	int y = GetDeviceCaps(dc, VERTRES);

    //创建临时矩形
    CRect rectTemp;
	//当矩形到达桌面边缘时调整方向和大小
	if((pt.x+rect.Width()) >= x)
	{
		//桌面上方显示不下矩形
		rectTemp = rect;
		rectTemp.left = rect.left - rect.Width() - space * 2;
		rectTemp.right = rect.right - rect.Width() - space * 2;;
		rect = rectTemp;
	}

	if((pt.y - rect.Height())<=0)
	{
		//桌面右方显示不下矩形
		rectTemp = rect;
		rectTemp.top = rect.top + rect.Height() + space * 2;
		rectTemp.bottom = rect.bottom + rect.Height() + space * 2;
		rect=rectTemp;
		
	}
    
	//创建空画刷画矩形
	CBrush brush(m_clrWndBackgndColor);
	CPen* pOldPen = dc.SelectObject(&pen);
	CBrush* pOldBrush = dc.SelectObject(&brush);
	dc.Rectangle(rect);
	dc.SelectObject(pOldBrush);
   	rect.top+=2;
	//在矩形中显示文字
	CString string(_T(" 顶点位置 "));
    dc.TextOut(rect.left,rect.top,string);

	string.Format(" (%d,%d)", inRect.left,inRect.top);
	dc.TextOut(rect.left, rect.top + charHeight, string);

	string = _T(" 矩形大小 ");
	dc.TextOut(rect.left, rect.top + charHeight*2, string);

	string.Format(" (%d,%d)", inRect.Width(), inRect.Height());
	dc.TextOut(rect.left, rect.top+charHeight*3, string);

	string = _T(" 光标坐标 ");
	dc.TextOut(rect.left, rect.top+charHeight*4, string);

	string.Format(" (%d,%d)", pt.x, pt.y);
	dc.TextOut(rect.left, rect.top + charHeight*5, string);
	dc.SelectObject(pOldFont);
	dc.SelectObject(pOldPen);
	pen.DeleteObject();
	brush.DeleteObject();
	font.DeleteObject();
}
//重画窗口
void CCatchScreenDlg::PaintWindow()
{
	//获取当全屏对话框窗口大小
	CRect rectWindow;
	GetWindowRect(rectWindow);

	//获取编辑框窗口大小
	CRect rectEdit;
	m_wndTipEdit.GetWindowRect(rectEdit);

	CRgn rgnWindow, rgnEdit;
	rgnWindow.CreateRectRgnIndirect(rectWindow);
	rgnEdit.CreateRectRgnIndirect(rectEdit);

	//获取更新区域,就是除了编辑框窗口不更新
	m_rgnUpdate.CombineRgn(&rgnWindow,&rgnEdit, RGN_DIFF);
	
	InvalidateRgn(&m_rgnUpdate);
	rgnWindow.DeleteObject();
	rgnEdit.DeleteObject();
}

int CCatchScreenDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	lpCreateStruct->style &= ~WS_VISIBLE;
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}

void CCatchScreenDlg::CaptureIt()
{
	m_bSleeping = false;
	//截取屏幕到位图中
	CRect rect(0, 0, m_nCXScreen, m_nCYScreen);
	SetRedraw(FALSE);
	m_OrgBackgroundBitmap.Attach(CopyScreenToBitmap(&rect));
	CClientDC dc(this);
	m_BackgroundBitmap.Attach(CreateTransparentColorBitmap(&dc, &m_OrgBackgroundBitmap, rect));
	
	//把对化框设置成全屏顶层窗口
	 SetWindowPos(&wndTopMost, 0, 0, m_nCXScreen, m_nCYScreen, SWP_SHOWWINDOW);

	SetRedraw();
	//移动操作提示窗口
	m_wndTipEdit.GetWindowRect(&rect);
    m_wndTipEdit.MoveWindow(10, 10, rect.Width(), rect.Height());
    
	m_bDrawTracker=FALSE;
	m_rectTracker.m_rect.SetRect(-1, -1, -1, -1);
	PaintWindow();
	//显示操作提示窗口文字
	DrawTip(dc);
}

void CCatchScreenDlg::OnCapture() 
{
	CaptureIt ();	
}

void CCatchScreenDlg::Sleep()
{
	if(IsWindowVisible())
		this->ShowWindow (SW_HIDE);
	m_bSleeping = true;
}

// 画出橡皮筋里面的原始图像
void CCatchScreenDlg::DrawOrgRect(CDC *pDC, const CRect & rcRect)
{
	CDC dcMem;
	dcMem.CreateCompatibleDC(pDC);
	// 把新位图选到内存设备描述表中
	HBITMAP hOldBitmap = (HBITMAP) dcMem.SelectObject(m_OrgBackgroundBitmap);
	pDC->BitBlt(rcRect.left, rcRect.top, rcRect.Width() , rcRect.Height(), &dcMem, rcRect.left, rcRect.top, SRCCOPY);
	dcMem.SelectObject(hOldBitmap);
}

// //在区域内画上透明颜色
void CCatchScreenDlg::DrawTransparentColor(CDC* pDC, const CRect& rcRect)
{
	Graphics graphics(pDC->GetSafeHdc());
	// Declare and initialize color component values as BYTE types.
	BYTE a = 80;
	BYTE r = 0;
	BYTE g = 0;
	BYTE b = 0xff;

	// Create an ARGB value from the four component values.
	ARGB argb = Color::MakeARGB(a, r, g, b);

	// Create a Color object from the ARGB value.
	Color argbColor(argb);

	// Create a SolidBrush object with argbColor, and use it to fill a rectangle.
	SolidBrush argbBrush(argbColor);
	Status status = graphics.FillRectangle(&argbBrush, Rect(0, 0, rcRect.Width(), rcRect.Height()));
	graphics.ReleaseHDC(pDC->GetSafeHdc());
}

// 从一个位图创建一个加上透明颜色的位图
HBITMAP CCatchScreenDlg::CreateTransparentColorBitmap(CDC *pDC, CBitmap* pOrgBitmap, const CRect& rcRect)
{
	// 创建一个与屏幕设备描述表兼容的位图
	HBITMAP hBitmap = CreateCompatibleBitmap(pDC->GetSafeHdc(),rcRect.Width() , rcRect.Height());
	CDC dcMem;
	dcMem.CreateCompatibleDC(pDC);
	// 把新位图选到内存设备描述表中
	HBITMAP hOldBitmap = (HBITMAP) dcMem.SelectObject(hBitmap);
	CDC dcCompatible;
	dcCompatible.CreateCompatibleDC(CDC::FromHandle(dcMem.m_hDC));
	HBITMAP hOldBitmap2 =  (HBITMAP)dcCompatible.SelectObject(pOrgBitmap->GetSafeHandle());

	dcMem.BitBlt(0, 0, rcRect.Width() , rcRect.Height(),&dcCompatible, 0, 0, SRCCOPY);
	DrawTransparentColor(&dcMem,rcRect);
	hBitmap = (HBITMAP)dcMem.SelectObject(hOldBitmap);
	dcCompatible.SelectObject(hOldBitmap2);
	dcCompatible.DeleteDC();
	dcMem.DeleteDC();
	return hBitmap;
}

// 拷贝屏幕图像到粘贴板
void CCatchScreenDlg::CopyScreenToClipboard(const CRect rcRect)
{
	// 获得选定区域坐标
	int nLeft = rcRect.left;
	int nTop = rcRect.top;
	int nRight = rcRect.right;
	int nBottom = rcRect.bottom;

	//确保选定区域是可见的
	if (nLeft < 0)
		nLeft = 0;
	if (nTop < 0)
		nTop = 0;
	if (nRight > m_nCXScreen)
		nRight = m_nCXScreen;
	if (nBottom > m_nCYScreen)
		nBottom = m_nCYScreen;
	int nWidth = nRight - nLeft;
	int nHeight = nBottom - nTop;

	CClientDC dc(this);
	CDC dcCompatible;
	dcCompatible.CreateCompatibleDC(&dc);
	HBITMAP hOldCompBitmap  =(HBITMAP) dcCompatible.SelectObject(m_OrgBackgroundBitmap.GetSafeHandle());
	CDC dcMem;
	dcMem.CreateCompatibleDC(&dcCompatible);
	HBITMAP hBitmap =  CreateCompatibleBitmap(dcCompatible.GetSafeHdc(),nWidth,nHeight);
	HBITMAP hOldBitmap  = (HBITMAP) dcMem.SelectObject(hBitmap);
	dcMem.BitBlt(0,0, nWidth, nHeight,
		&dcCompatible, nLeft, nTop, SRCCOPY);
	if (OpenClipboard()) 
	{
		//清空剪贴板
		EmptyClipboard();
		//把屏幕内容粘贴到剪贴板上,
		//hBitmap 为刚才的屏幕位图句柄
		SetClipboardData(CF_BITMAP, hBitmap);
		//关闭剪贴板
		CloseClipboard();
	}

	dcMem.SelectObject(hOldBitmap);
	dcCompatible.SelectObject(hOldCompBitmap);
	dcMem.DeleteDC();
	dcCompatible.DeleteDC();
	DeleteObject(hBitmap);
}

void CCatchScreenDlg::DoPaint(CDC& dc)
{
	CRect rect;
	GetClientRect(&rect);
	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);
	HBITMAP hBitmap = CreateCompatibleBitmap(dc.GetSafeHdc(), rect.Width(), rect.Height());

	// 把新位图选到内存设备描述表中
	HBITMAP hOldBitmap = (HBITMAP)dcMem.SelectObject( hBitmap);
	//用整个桌面填充全屏对话框背景
	if (m_BackgroundBitmap.GetSafeHandle()) {
		BITMAP bmp;
		m_BackgroundBitmap.GetBitmap(&bmp);

		CDC dcCompatible;
		dcCompatible.CreateCompatibleDC(&dcMem);
		dcCompatible.SelectObject(m_BackgroundBitmap);
		CRect rect;
		GetClientRect(&rect);
		dcMem.BitBlt(0,0,rect.Width(),rect.Height(),&dcCompatible,0,0,SRCCOPY);
	}
	//画出像皮筋矩形
	if(m_bDrawTracker)
	{
		//画出橡皮筋里面的原始图像
		DrawOrgRect(&dcMem,m_rectTracker.m_rect);
		m_rectTracker.Draw(&dcMem);
		//显示截取矩形大小信息
		if(m_bShowMsg)
		{
			CRect rect;
			m_rectTracker.m_rect.NormalizeRect();
			DrawMessage(dcMem,m_rectTracker.m_rect);
		}
	}
	
	dc.BitBlt(0,0,rect.Width(),rect.Height(),&dcMem, 0, 0, SRCCOPY);
	hBitmap = (HBITMAP)dcMem.SelectObject( hOldBitmap);
	dcMem.DeleteDC();
	DeleteObject(hBitmap);
}

BOOL CCatchScreenDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message==WM_KEYDOWN)
	{
		CRect rect(0, 0, 0, 0);
		rect=m_rectTracker.m_rect;

		if(m_bDrawTracker)
		{

			//如果Shift键按下则方向键调整大小
			BOOL isShifeDowm=FALSE;
			int nVirtKey;
			nVirtKey = GetKeyState(VK_SHIFT); 
			if (nVirtKey & SHIFTED) 
				isShifeDowm = TRUE;

			switch(pMsg->wParam)
			{
			case VK_UP:
				//如果按下Shift,则只调整一边
				if(!isShifeDowm)
					rect.top -= 1;
				rect.bottom -= 1;
				m_rectTracker.m_rect=rect;
				PaintWindow();
				break;
			case VK_DOWN:
				rect.top += 1;
				if(!isShifeDowm)
					rect.bottom += 1;
				m_rectTracker.m_rect = rect;
				PaintWindow();
				break;
			case VK_LEFT:
				if(!isShifeDowm)
					rect.left -= 1;
				rect.right -= 1;
				m_rectTracker.m_rect = rect;
				PaintWindow();
				break;
			case VK_RIGHT:
				rect.left += 1;
				if(!isShifeDowm)
					rect.right += 1;
				m_rectTracker.m_rect = rect;
				PaintWindow();
				break;
			}
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CCatchScreenDlg::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}