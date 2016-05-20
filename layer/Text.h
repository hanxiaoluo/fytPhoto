/* =============================================================== *\


	File Description:
			this file is description text structuct
			define some data struct/class

    Maintain : 
		class CText, class CLineInfo, struct ListInfo;
		as MFC's CDocument ;


\* =========================================================== */
// Text.h: interface for the CText class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEXT_H__7647DB01_6CD9_11D3_8E0A_00902722AEBA__INCLUDED_)
#define AFX_TEXT_H__7647DB01_6CD9_11D3_8E0A_00902722AEBA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*--------------------------------------------------------*
* First auther : G.H. Lai     Data : 1999/9/3
*---------------------------------------------------------*/
struct ListInfo  
{
public:
	ListInfo();// set next is NULL;
	virtual ~ListInfo();

public:
	// ------ Font Format ------ //		
	BOOL		bBold;					
	BOOL		bItalic;				
	BOOL		bUnderline;				
	UINT		nSize;
	CString		szFaceName;
	COLORREF	color;

	// ---- char Inforation ---- //
	CString     string;

	// ---- List Inforation ----- //
	struct ListInfo  *pNext;

public:
	int		GetLength();	//Get the length of string;
	int		GetUnitLength();//Get the unit length of string;

	//compare string;
	BOOL	Compare_str(CString) const;
	BOOL	Compare_str(LPCTSTR) const;

	BOOL    Compare_LOGFONT(LOGFONT*) const;//compare the structect of LOGFONT;
	BOOL    SetFont(LOGFONT*, UINT);		//set the properties of string;
	LOGFONT GetFont();						//Get the properties of string;

	UINT	GetWidth(INT temp = 1);//get the width of string from size
	UINT    GetHeight(INT temp = 1);//get the heigth of string from size;

	void    SetNextList(ListInfo *point = NULL);
};

/*------------------------------------------------------*
* First auther : G.H. Lai   Data : 1999/9/03
*-------------------------------------------------------*/
class CLineInfo  
{
public:
	CLineInfo();
	virtual ~CLineInfo();

private:
	struct ListInfo	*m_pData;		// line's data
	CLineInfo		*m_pPreLine;	// previous line
	CLineInfo		*m_pNextLine;	// next line
	UINT			 m_nLineIndex;	// the index in text

	CRect			m_rect;			// line rect

	public:
	struct ListInfo *m_pCur;		// current list 

public:
	int SetControlRect(int TextStyle, int nLetterspace=0,int nLineHeight=0);

	int SetNextLine(CLineInfo *point = NULL);	// set next line point
	int SetPreLine(CLineInfo *point = NULL);	// set previous line point

	CLineInfo*	GetNextLine() const;			// return next line
	CLineInfo*  GetPreLine() const;				// return previous line

	struct ListInfo*   GetDataPoint() const;	// return data
	BOOL   SetDataPoint(struct ListInfo*);		// set data

	CRect  GetControlRect() const;		// return rect	
	UINT   GetLineLength();				// get line length
	UINT   GetLineUnitLength();
	void   GetLineStr(CString& szLine);	// get string of line

	UINT	GetLineIndex() const;
	UINT	SetLineIndex(UINT nIndex = 0);
};

class CText 
{
friend class CTextObject;
public:
	CText();
	virtual ~CText();

private:
	// -------- nLetterspace : character space ---------- //
	// -------- nLineHight   : Line space --------------- //
	// -------- Default equal zero ----------- //
	int		nLetterspace, nLineHeight;

	// ------------------- Text Aligned ----------------- //
	// m_nAlign = 0 ;  Horizontal , Left;
	// m_nAlign = 1 ;  Horizontal , Center;
	// m_nAlign = 2 ;  Horizontal , Right;
	// m_nAlign = 3 ;  Vertical , Up;
	// m_nAlign = 4 ;  Vertical , Center;
	// m_nAlign = 5 ;  Vertical , Down;
	UINT    m_nAlign;	// 0 ----- 5;

	// Sum Line in it manager CText object;
	INT		m_nTotalLine;			// 1 ---- ;

public:
	// --------------------------------------------------- //
	// ------ m_pHead : head pointer -------- //
	// ------ m_pTail : tail pointer -------- //
	// ------ m_pPos  : work pointer -------- //
	CLineInfo	*m_pHead,*m_pTail,*m_pPos;

	INT	   m_nUpgrade;				//  0 --- have nothing do;
									//  1 --- maybe set the last line;
									//  2 --- must Reset Control Rect;
									
public:
	int		Set_Letterspace(int space = 0);
	int     Set_LineHeight(int height = 0);
	int		SetFormat(int Aligned = 0);

	int     Get_Letterspace() const;
	int     Get_LineHeight() const;
	int     GetFormat() const;
	// ======== set every line control rectangle ===== //
	int     SetTextRect();

	// ============ Save / Open File =============== //
	BOOL     Save(LPCTSTR ,LPCTSTR);
	BOOL     Read(LPCTSTR ,LPCTSTR);

	int     InsertLine(LPCTSTR);
	int     Appendline(LPCTSTR);
	int     Deleteline(void);
	int     DeleteAll(void);

	INT		GetTextTotalLine() const;
	INT		SetTextTotalLine(INT nIndex);
	INT		ReCountTotalLine();	// return current Line count; check??

	CLineInfo*	GetLineObject(INT);//Get the direct CLineInfo Object;

private:
	// Optimize ;
	int    preAction();
};

////////////////////////////////////////////////////////////////////
// the structuct of TextItem is writen of save/read procedure.
// define some of data and it must be save/read information;
typedef struct tagTextInfo
{
	BOOL	bBold;
	BOOL	bItalic;
	BOOL	bUnderline;
	UINT	nSize;
	TCHAR		szFaceName[24];
	COLORREF	color;
	TCHAR		pszStr[40];
	TCHAR		chMark;	// + -- at the middle of the list;
						// \ -- the end of general list
						// @ -- the end of every line;
						// # -- the end of text;
} TEXTINFO , *LPTEXTINFO;

#endif // !defined(AFX_TEXT_H__7647DB01_6CD9_11D3_8E0A_00902722AEBA__INCLUDED_)

