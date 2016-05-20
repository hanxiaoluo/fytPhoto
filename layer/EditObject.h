//EditObject.h
#ifndef PHOTOFAMILY_EDITOR_EDITOBJECT
#define PHOTOFAMILY_EDITOR_EDITOBJECT

#if _MSC_VER >= 1000
#pragma once
#endif //// _MSC_VER >= 1000

#include "PFDATA.h"
// For Type List
#include <afxtempl.h>
#include "Layer.h"

class CImageObject
{
public:
	CImageObject();
	~CImageObject();
//	BOOL Undo();
//	BOOL Redo();
	virtual BOOL Read(LPCTSTR PfmFile) = 0;
	virtual BOOL UpdateImage() = 0;
	virtual VOID Delete() = 0;
	virtual VOID Draw() = 0;
	virtual VOID MoveForeImage(CSize size) = 0;
	virtual VOID GetForeImageRect(CRect &rect) = 0;

	BOOL Save(LPCTSTR szFile);
	VOID SaveAs(LPCTSTR szFile);
	VOID GetObjectData(LPBYTE& pObjData, LPBITMAPINFO& pObjInfo);
//	BOOL GetUndoFlag();
//	BOOL GetRedoFlag();
	BOOL GetModifiedFlag();
	BOOL SetSrcImgData(LPBYTE pData);		//Set source image data
	BOOL SetNewImgData(LPBYTE pData);		//When apply in operation dialogue
	LPBYTE GetSrcImgData();					//Get source  image data
	LPBYTE GetCurImgData();					//Get current image data
	OBJECTTYPE GetObjectType();

protected:
//	VOID  DeleteUndoHead();
//	VOID  DeleteUndoList();
//	VOID  DeleteRedoList();

	BOOL   m_bImgModified;					//The image is modified
	LPBYTE m_pSrcData;						//Source  image data of object
	LPBYTE m_pCurData;						//Current image data of object
	SHORT  m_nScalInd;						//Scaling index in dScaling[]
	LPBYTE m_pObjData;						//Compound object image data
	LPBITMAPINFO m_pObjInfo;				//Compound object image info
	OBJECTTYPE m_iObjectType;
//	CTypedPtrList <CPtrList, LPBYTE>	m_pUndo;
//	CTypedPtrList <CPtrList, LPBYTE>	m_pRedo;
};

typedef struct tag_FamilyHeader
{
	TCHAR chSignature[8];	/* signature, must equal to "PRINTFUN" */
	WORD wVersion;			/* version info, this version equal to 1 */
	DWORD dwReserved;		/* reserved, equal to 0 */
	OBJECTTYPE iObjectType;		/* task type */
	DWORD dwWidth;			/* page (template) width */
	DWORD dwHeight;			/* page (template) height */
	DWORD dwThumbnailSize;	/* thumbnail image size, 0 means none thumbnail image */
}FAMILYHEADER, *LPFAMILYHEADER;


class CAlgImgObject : public CImageObject
{
public:
	CAlgImgObject();
	~CAlgImgObject();
	virtual BOOL Read(LPCTSTR PfmFile);
	virtual BOOL UpdateImage();
	virtual VOID Delete();
	virtual VOID Draw();

	virtual VOID MoveForeImage(CSize size) ;
	virtual VOID GetForeImageRect(CRect &rect) ;
};


#include "Text.h"

class CTextObject 
{
public:
	CTextObject();
	~ CTextObject ();

private:
	CDC*   pDefaultDC;
	CRect  m_rect;
	CRichEditCtrl*	m_pRichEdit;

public:
	BOOL  Set_DefaultDC(CDC *);
	BOOL  Move(CSize sz=CSize(0,0));
	BOOL  ReSet_Rectangle(CRect);
//	BOOL  ReSet_Control_Text(CText *);

public:
	VOID	Draw(CDC* pDC);		
	DWORD   GetTextData(LPVOID&);
	INT		SetTextData(const LPVOID , DWORD);
};


// define calendar item every day information structure
typedef struct tag_DAYDATA
{
	INT nDay;
	INT nWeek;
	COLORREF color;
	LPTSTR lpszMask;
	LPBITMAPINFO lpInfo;
	LPBYTE lpData;

	tag_DAYDATA()
	{
		color = RGB(0, 0, 0);
		lpszMask = NULL;
		lpInfo = NULL;
		lpData = NULL;
	}

	~tag_DAYDATA()
	{	// Author :G.H	Data: 10/26/1999
		if (lpszMask) delete lpszMask;
		if (lpInfo)   delete lpInfo;
		if (lpData)   delete lpData;
	}
}DAYDATA, *LPDAYDATA;
typedef CTypedPtrList <CPtrList, LPDAYDATA> DAYDATALIST;
typedef DAYDATALIST* LPDAYDATALIST;

// Author : G.H. Lai; Festival List ... 
typedef struct tag_LIST
{
	INT		nLength;
	INT*	parrDayList;	// pointer to Array;

	tag_LIST()
	{
		nLength = 0;
		parrDayList = NULL;
	}

	~tag_LIST()
	{
		if (parrDayList != NULL) delete[] parrDayList;
	}
}DATALIST,*LPDATALIST;

typedef struct tag_DATAFRAME
{
	POINT	Pos_Point;
	INT		nIndex;
}DATAFRAME;

class CMonthObject 
{
public:
	CMonthObject();
	~CMonthObject();

private:
//	CImageObect* m_pImageObj;
	CMonthObject* m_pNext;
	CMonthObject* m_pPrev;
	INT			  m_nIndex;

	RECT		  m_rcFrame;	/* Main Frame */

protected:
	INT			m_nYear;
	INT			m_nMonth;
	BOOL		m_bFromMonday;	/* =FALSE */
	COLORREF	m_crWorkday;    /* =RGB(0,0,255) */
	COLORREF	m_crHoliday;    /* =RGB(255,0,0) */
	INT			m_nFestivalStyle;
	DAYDATALIST	m_DayList;

	//RECT        m_rcBk; -- CPFItem::m_pRect;
	RECT		m_rcYear;
	RECT		m_rcMonth;
	RECT		m_rcWeek;
	RECT		m_rcDays;

	CString		m_szImage;
	CString		m_szDateImage;
	CString     m_szYearImage;
	CString     m_szWeekImage;
	CString     m_szMonthImage;

	INT			m_nImageStyle;
	INT			m_nStyle;

	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	LPBITMAPINFO m_lpInfo[5];		// m_lpInfo[0] & m_lpData[0] -- m_szImage --- Blackground bitmap;
	LPBYTE		 m_lpData[5];		// m_lpInfo[1] & m_lpData[1] -- m_szDataImage 
									// m_lpInfo[2] & m_lpData[2] -- m_szYearImage
									// m_lpInfo[3] & m_lpData[3] -- m_szWeekImage
									// m_lpInfo[4] & m_lpData[4] -- m_szMonthImage
	DATALIST	m_olddataList;

public:
	VOID	Draw(CDC *pDC);
	CMonthObject* GetNextObj() const
	{ return m_pNext;}
	VOID	SetNextObj(CMonthObject* pMonthObj = NULL)
	{ m_pNext = pMonthObj;}

	CMonthObject* GetPrevObj() const
	{ return m_pPrev;}
	VOID	SetPrevObj(CMonthObject* pMonthObj = NULL)
	{ m_pPrev = pMonthObj;}

	////////////////////////////////////////////////////////////
	INT		SetObjectData(const LPVOID pData, DWORD dwSize);//Read;
	DWORD   GetObjectData(LPVOID& pData);// write;

	///////////////////////////////////////////////////////////
	INT	 GetPosition() const;
	INT	 SetPosition(INT nIndex);

	///////////////////////////////////////////////////////////
	VOID SetMonth(INT nYear, INT nMonth);
	VOID SetFromMonday(BOOL bFrom);
	VOID SetWorkdayColor(COLORREF color);
	VOID SetHolidayColor(COLORREF color);

	VOID SetFestivalStyle(INT nStyle, INT parr[]= NULL, INT nLen =0);
	VOID SetFestivalStyle(LPCTSTR pszImage, INT nIndex);

	VOID SetDayColor(INT nDay, COLORREF color);
	VOID SetDayMask(INT nDay, LPTSTR lpszMask);

	/////////////////////////////////////////////////////////////
	INT GetYear() const;
	INT GetMonth() const;
	INT GetFestivalStyle() const;
	INT GetStyle() const;
	INT GetDayCount() const;
	INT GetIndex() const;
	BOOL IsFromMonday() const;
	VOID SetStyle(INT nStyle);
	VOID SetIndex(INT nIndex);
	VOID GetDayMask(INT nDay, CString& szMask);
	VOID GetDayInfo(LPDAYDATA pDayInfo, INT nDay);
	COLORREF GetWorkdayColor() const;
	COLORREF GetHolidayColor() const;
	COLORREF GetDayColor(INT nDay);

	///////////////////////////////////////////////////////////////
	VOID PreviousDraw(CDC* pDC, CRect, INT, DATAFRAME* FrameList=NULL, INT* pMax=NULL, BOOL bUpdate=FALSE);
	VOID PreDraw(CDC* pDC, CRect, INT);

	//%%%%%%%%%%%%%%%%%%%%%%
	VOID SetBkImage(LPCTSTR pszImage);
	VOID SetYearImage(LPCTSTR pszImage);
	VOID SetDateImage(LPCTSTR pszImage);
	VOID SetWeekImage(LPCTSTR pszImage);
	VOID SetMonthImage(LPCTSTR pszImage);
	INT  Ready();

	BOOL SetImageStyle(INT nStyle = 0);
	INT	 GetImageStyle() const;

	CString GetBkImage() const;
	CString GetYearImage() const;
	CString GetMonthImage() const;
	CString GetWeekImage() const;
	CString GetDateImage() const;

	VOID SetYearRect(LPRECT pRect);
	VOID SetMonthRect(LPRECT pRect);
	VOID SetWeekRect(LPRECT pRect);
	VOID SetDaysRect(LPRECT pRect);
	VOID SetFrameRect(LPRECT pRect);

	RECT GetBkRect() const;
	RECT GetYearRect() const;
	RECT GetMonthRect() const;
	RECT GetWeekRect() const;
	RECT GetDaysRect() const;

private:
	INT		LoadingImage(LPBITMAPINFO& pInfo,LPBYTE& pData,LPCTSTR pszImage);
	VOID	DrawDate(CDC* pDC, RECT rcFrame);
	VOID	DrawWeek(CDC* pDC, RECT rcFrame);
	VOID    DrawYear(CDC* pDC, RECT rcFrame);
	VOID    DrawMonth(CDC* pDC,RECT rcFrame);
};


typedef struct tag_MonthInfo
{
	CRect rectMonth ;
	CRect rectYear  ;
	CRect rectWeek  ;
	CRect rectDay   ;
	DWORD nType	   ;
}MONTHINFO, *LPMONTHINFO;

typedef struct tag_CalendarHeader
{
	WORD nMonthCount;	
	char YFONTPIC[11];
	char MFONTPIC[11];
	char DFONTPIC[11];
	char WFONTPIC[11];
	MONTHINFO MonthInfo[12];

}CALENDARHEADER, *LPCALENDARHEADER;

class CCalendarObject : public CImageObject
{
public:
	CCalendarObject();
	~CCalendarObject();

	VOID	Default();

protected:
	CMonthObject *	m_pFirstObj;
	CMonthObject *  m_pTailObj;
	CMonthObject *  m_pCurObj;
	INT				m_nTemplateStyle;	/// 1 , 2 , 12

	RECT	m_rcImage;

	CALENDARHEADER  m_CalendarInfo ;

///		picture member ** Robert /////
	LONG			m_Width;
	LONG			m_Height;
	LONG			m_ImageWidth,	m_ImageHeight;
	LPBYTE			m_pPic;
	CDC*			m_pPicDC;
	CBitmap			m_PicBmp;
	HBITMAP			m_hbmp;


	LPBYTE			m_BackGround;
	LPBYTE			m_ImageData;
	POINT			m_LayerPlace;
	LPBITMAPINFOHEADER	m_ImageInfo;
	LPBITMAPINFOHEADER	m_BackGroundInfo;	
	CLayerObject*	m_pLayerObj;
	VOID			DrawImage();

public:
	virtual BOOL Read(LPCTSTR PfmFile);
	virtual BOOL UpdateImage();
	virtual VOID Delete();
	virtual VOID Draw();
	virtual VOID MoveForeImage(CSize size);
	virtual VOID GetForeImageRect(CRect &rect);

	CMonthObject*	GetFirstObject() const;
	CMonthObject*   GetTailObject() const;
	CMonthObject*   GetCurObject() const;
	CMonthObject*	GetIndexObject(INT nIndex) const;

	VOID	SetImageFrame(LPRECT pRect);
	RECT	GetImageFrame();
	BOOL 	OpenFile2(LPCTSTR lpszPathName, CLayerObject *layer,LONG type);

//	RECT	GetIndexFrame(INT nIndex = 1);

	VOID	SetFirstObject(CMonthObject* pMonthObj);
//	VOID	SetCurObject(INT nIndex);

	INT		GetTemplateStyle() const;
	INT		SetTemplateStyle(INT nStyle);

	/////////////////////////////////////////// New Method///
	VOID	Add_Head_Object();
	VOID	Add_Tail_Object();

	//////////////////////// Read/Write ////////////////////////////
	VOID	ReadData(const LPVOID pData, DWORD dwSize);
	DWORD	Write(LPVOID& pData);

private:
	VOID    GetNextYearMonth(INT* pMonth, INT* pYear);
};

#endif