// Text.cpp: implementation of the CText class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Text.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CString MakeIntToString(const INT alignment)
{
	INT temp = abs(alignment);
	CString szOutput;
	while(temp >= 10)
	{
		INT nBits = temp/10;
		szOutput += char(nBits + 48);
		temp = temp % 10;
	}
	if (temp<10) szOutput += char(temp + 48);
	if (alignment < 0) szOutput.Insert(0,"-");
	return szOutput;
}

VOID MakeStringToInt(const CString buf,INT* pTemp)
{
	CString szbuf = buf;
	INT     nLength = szbuf.GetLength();
	INT		curTemp = 0;
	for(int i=0; i<nLength; i++) 
		if (i == 0) curTemp = szbuf[i]-48;
		else curTemp = (INT)(curTemp*pow(10,1) + (szbuf[i]-48));
	*pTemp = curTemp;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CText::CText()
{
	try
	{
		m_pHead = new CLineInfo();
		m_pPos  = 
		m_pTail = m_pHead;
		m_nUpgrade = 2;
		(*m_pHead).SetLineIndex(1);
		m_nTotalLine = 1;
		m_nUpgrade = 2;

		m_nAlign = 
		nLetterspace = 
		nLineHeight = 0;
	}
	catch(CMemoryException* e)
	{
		e->Delete();
		m_pHead = m_pTail = m_pPos = NULL;
	}

}

CText::~CText()
{
	m_pPos = m_pHead;
	m_pHead = (*m_pHead).GetNextLine();

	while(m_pPos)
	{
		ASSERT(m_pPos);
		delete m_pPos;
		m_pPos = m_pHead;
		if (m_pHead != NULL) m_pHead = (*m_pHead).GetNextLine();
	}

	m_pPos = m_pHead = m_pTail = NULL;
}

/*-------------------------------------------------------*
 * Purpose	: Set character space;
 * Input	: if input the litter than zero, nletterspace = 0 
 *				return -1;
 * Return Value	: Return nLetterspace;
 *-------------------------------------------------------*/
int	CText::Set_Letterspace(int space /* =0 */)
{
	if (space < 0) 
	{
		nLetterspace = 0;
		return -1;
	}
	nLetterspace = space;
	return nLetterspace;
}

int CText::Get_Letterspace() const
{
	return nLetterspace;
}
/*-----------------------------------------------------*
 * Purpose	: Set Line Hight ;
 * Input	: if input litter zero , nLineHight = 0
 *				return -1;
 * Return Value	: return nLineHight;
 *-----------------------------------------------------*/
int CText::Set_LineHeight(int height /* =0 */)
{
	if (height < 0)
	{
		nLineHeight = 0;
		return -1;
	}
	nLineHeight = height;
	return nLineHeight;
}

int CText::Get_LineHeight() const
{
	return nLineHeight;
}
/*------------------------------------------------------------------------*
 * Purpose		:	Call function his CText* object::SetTextRect;
 * Action		:	Set mark information; before action program checked it 
 *					mark;
 * return value	:   if no perpare return -1, if successed return 1;
 *------------------------------------------------------------------------*/
int CText::SetTextRect()
{
	//if ( (bitMarks & 0x30) == 0) return -1;
//	int return_valuse = Perpare();
	if (0 == m_nUpgrade) return 1;

	int TextStyle = 0;
	switch(m_nAlign)
	{
	case 0:
	case 1:
	case 2:
		TextStyle = 0;
		break;
	case 3:
	case 4:
	case 5:
		TextStyle = 1;
		break;
	default: 
		ASSERT(0);
//		AfxMessageBox("ERROR Parametic",MB_OK,0);
		return -1;
	}

	m_pPos = m_pHead;
	if (1 == m_nUpgrade)
	{	// reset the last line;
		m_pPos = m_pTail;
		(*m_pPos).SetControlRect(TextStyle, nLetterspace, nLineHeight);
	}
	else
	{
		while(m_pPos)
		{
			ASSERT(m_pPos);
			(*m_pPos).SetControlRect(TextStyle, nLetterspace, nLineHeight);
			if (m_pPos != m_pTail) m_pPos = (*m_pPos).GetNextLine();
			else break;
		}
	}
//	bitMarks |= 0xc0;	// 1100,0000  reset successed;
	m_nUpgrade = 0;
	return 1;
}

/*-----------------------------------------------------------------*
 * Default	: Aligned = 0;
 * Input	: if input value the great than five or the litter than 
 *				zero, m_nAlign = default; return -1;
 * Return Value : Return m_nAlign;
 *-----------------------------------------------------------------*/
// m_nAlign = 0 ;  Horizontal , Left;
// m_nAlign = 1 ;  Horizontal , Center;
// m_nAlign = 2 ;  Horizontal , Right;
// m_nAlign = 3 ;  Vertical , Up;
// m_nAlign = 4 ;  Vertical , Center;
// m_nAlign = 5 ;  Vertical , Down;
int	CText::SetFormat(int Aligned /* =0 */)
{
	if (m_nAlign == (unsigned int)Aligned) return m_nAlign;
	if (Aligned < 0 || Aligned >5) 
	{
		if (m_nAlign == 3 || m_nAlign == 4 || m_nAlign == 5)
		{
			m_nAlign = 0;
			m_nUpgrade = 2;
		}
		return -1;
	}
	else 
	{
		if ( (m_nAlign == 0 || m_nAlign == 1 || m_nAlign == 2)
			&&(Aligned == 3 || Aligned == 4 || Aligned == 5))
			m_nUpgrade = 2;
		else if ( (m_nAlign == 3 || m_nAlign == 4 || m_nAlign == 5)
			     &&(Aligned == 0 || Aligned == 1 || Aligned == 2))
				 m_nUpgrade = 2;
		m_nAlign = Aligned;
	}
	return m_nAlign;
}

int CText::GetFormat() const
{
	return m_nAlign;
}
/*---------------------------------------------------------------*
 * Purpose		:insert listinfo object After the pointer of pos ;
 * Action		:pos direct new insert of item;
 * return value	:
 *---------------------------------------------------------------*/
int CText::InsertLine(LPCTSTR tempstr)
{
	if (m_pPos == NULL)
	{
//		ErrorMessageBox("Error Operation(CText::InsertLine)",MB_OK);
		ASSERT(0);
		return -1;
	}
	CLineInfo	*tempoint = NULL;
	tempoint = new CLineInfo();
	ASSERT(tempoint);
	CLineInfo   *point = (*m_pPos).GetNextLine();

	///////////////////////Initial tempoint;
	struct ListInfo*   pTemp = (*tempoint).GetDataPoint();
	struct ListInfo*   pDefaultList = (*m_pPos).GetDataPoint();
	LOGFONT	pLf = (*pDefaultList).GetFont();
	pTemp->SetFont(&pLf, (*pDefaultList).nSize);
	pTemp->color = (*pDefaultList).color;

	(*(*tempoint).GetDataPoint()).string = tempstr;
	(*tempoint).SetNextLine(point);
	(*tempoint).SetPreLine(m_pPos);

	(*m_pPos).SetNextLine(tempoint);
	m_pPos = (*m_pPos).GetNextLine();
	if (point != NULL) (*point).SetPreLine(tempoint);
	else m_pTail = m_pPos;

	// reset index;
	CLineInfo *pLineInfo = m_pHead;
	INT		  nCount = 1;
	while(pLineInfo)
	{
		ASSERT(pLineInfo);

		pLineInfo->SetLineIndex(nCount++);
		if (pLineInfo != m_pTail)
			pLineInfo = (*pLineInfo).GetNextLine();
		else 
			break;
	}
	m_nTotalLine = nCount;

	return 1;
}

INT	CText::GetTextTotalLine() const
{
	return m_nTotalLine;
}

INT	CText::SetTextTotalLine(INT nIndex)
{
	if (0 == nIndex) return nIndex;

	m_nTotalLine = nIndex;
	return m_nTotalLine;
}

INT	CText::ReCountTotalLine()
{
	CLineInfo* pTempLine = NULL;
	INT	nCount = 1;

	pTempLine = m_pHead;
	while(pTempLine != NULL)
	{
		ASSERT(pTempLine);

		if (pTempLine != m_pTail)
			pTempLine = (*pTempLine).GetNextLine();
		else 
			break;
		nCount++;
	}

	// m_nTotalLine = nCount;
	return nCount;
}

CLineInfo*	CText::GetLineObject(INT nIndex)
{
	CLineInfo *pLineInfo = NULL;
	if (nIndex <= 0 || nIndex > m_nTotalLine) 
		return pLineInfo;

	m_pPos = m_pHead;
	INT nCount = nIndex;
	while(m_pPos != NULL)
	{
		ASSERT(m_pPos);
		if (nCount-- == 1) break;
		if (m_pPos != m_pTail) 
			m_pPos = (*m_pPos).GetNextLine();
		else 
			break;
	}

	if (nCount != 0) 
	{
	//	AfxMessageBox("Error index of Text",MB_OK,0);
		ASSERT(0);
	}

	pLineInfo = m_pPos;
	return pLineInfo;
}

/*-------------------------------------------------------------*
 * Purpose		:	append a new line ;
 * Action		:	refresh m_pTail, m_pPos direct m_pTail pointer;
 * return value	:	
 *-------------------------------------------------------------*/
int CText::Appendline(LPCTSTR tempstr)
{
	CLineInfo  *tempoint = NULL;
	tempoint = new CLineInfo();
	ASSERT(tempoint);

	(*m_pTail).SetNextLine(tempoint);
	(*tempoint).SetPreLine(m_pTail);
	(*tempoint).SetNextLine(NULL);
	m_pTail = (*m_pTail).GetNextLine();

	struct ListInfo* pListpos = NULL;
	pListpos = (*m_pTail).GetDataPoint();
	(*(*m_pTail).GetDataPoint()).string = tempstr;

	m_pPos = m_pTail;

	// reset index;
	CLineInfo *pLineInfo = m_pHead;
	INT		  nCount = 1;
	while(pLineInfo)
	{
		ASSERT(pLineInfo);
		(*pLineInfo).SetLineIndex(nCount++);
		if (pLineInfo != m_pTail)
			pLineInfo = (*pLineInfo).GetNextLine();
		else 
			break;
	}
	m_nTotalLine = nCount;

	return 1;
}

/*-------------------------------------------------------------------*
 * Purpose		:	Delete the pointer of pos direct ListInfo;
 * Action		:	pos direct the next of delete item ;
 *                  if delete item equal tail, pos = tail;
 * return value	:	if successed return 0; else return the litter than 
 *					than zero ;
 *------------------------------------------------------------------*/
int CText::Deleteline(void)
{
	if (m_pPos == NULL) 
	{
#ifdef _DEBUG
	//	AfxMessageBox("ERROR Operation(CText::Deleteline(1))!",MB_OK,0);
		ASSERT(0);
#endif
		return -1;
	}
	CLineInfo *tempoint = m_pPos;

	m_pPos = m_pHead;
	while(m_pPos)
	{
		if (tempoint == m_pPos) break;
		else m_pPos = (*m_pPos).GetNextLine();
	}
	if (m_pPos == NULL) 
	{
#ifdef _DEBUG
	//	AfxMessageBox(" ERROR Operation(CText::Deleteline(2))",MB_OK,0);
		ASSERT(0);
#endif
		return -2;
	}
	
	m_pPos = tempoint;
	m_pPos = (*m_pPos).GetPreLine();
	if (tempoint == m_pHead)
	{
		m_pHead = (*tempoint).GetNextLine();
		(*m_pHead).SetPreLine(NULL);
	}
	if (tempoint == m_pTail)
	{	
		m_pTail = (*tempoint).GetPreLine();
		(*m_pTail).SetNextLine(NULL);			// <==============+
	}												    	//    |
	if (m_pPos != NULL)	// tempoint = head;				    //    |
		(*m_pPos).SetNextLine((*tempoint).GetNextLine());    //	  |
	if ((*tempoint).GetNextLine() != NULL)   // if == NULL  ======+
		(*(*tempoint).GetNextLine()).SetPreLine(m_pPos);

	struct ListInfo* pListpos = NULL;
	pListpos = (*tempoint).GetDataPoint();
	delete tempoint;

	if (m_pPos != NULL) m_pPos = (*m_pPos).GetNextLine();
	else m_pPos = m_pHead;

	// reset index;
	CLineInfo *pLineInfo = m_pHead;
	INT		  nCount = 1;
	while(pLineInfo)
	{
		ASSERT(pLineInfo);
		(*pLineInfo).SetLineIndex(nCount++);
		if (pLineInfo != m_pTail)
			pLineInfo = (*pLineInfo).GetNextLine();
		else 
			break;
	}
	m_nTotalLine = nCount;

	return 0;
}

/*------------------------------------------------------------*
 *	Purpose : delete all item ,but m_pHead 
 *------------------------------------------------------------*/
int CText::DeleteAll(void)
{
	(*(*m_pHead).GetDataPoint()).string.Empty();
	struct ListInfo* pListpos = (*(*m_pHead).GetDataPoint()).pNext;
	while(pListpos != NULL)
	{
		struct ListInfo* pListwork = (*pListpos).pNext;
		delete pListpos;
		pListpos = pListwork;
	}


	m_pPos = (*m_pHead).GetNextLine();
	m_pTail = m_pPos;
	while(m_pPos != NULL)
	{
		ASSERT(m_pPos);

		m_pTail = (*m_pPos).GetNextLine();
		delete m_pPos;
		m_pPos = m_pTail;
	}

	m_pPos = m_pTail = m_pHead;
	(*m_pHead).SetNextLine(NULL);

	(*m_pPos).SetLineIndex(1);
	m_nTotalLine = 1;

	return 1;
}

/*-----------------------------------------------------------------------*
 * Purpose :	Optimize the list of CText Object;
 * Return Value:
 *-----------------------------------------------------------------------*/
int CText::preAction()
{
	m_pPos = m_pHead;
	while(m_pPos != NULL)
	{
		ASSERT(m_pPos);

		struct ListInfo* pListpos = (*m_pPos).GetDataPoint();
		while( (*pListpos).pNext != NULL)
		{
			ASSERT((*pListpos).pNext);

			struct ListInfo* pListwork = (*pListpos).pNext; 
			LOGFONT logfont = (*pListwork).GetFont();
			if ((*pListpos).Compare_LOGFONT(&logfont) && (*pListpos).color == (*pListwork).color &&
				(*pListpos).nSize == (*pListwork).nSize)
			{
				(*pListpos).string = (*pListpos).string + (*pListwork).string;
				(*pListpos).pNext = (*pListwork).pNext;
				delete pListwork;
			}
			else if((*pListwork).string.IsEmpty())
			{
				(*pListpos).pNext = (*pListwork).pNext;
				delete pListwork;
			}
			else 
				pListpos = (*pListpos).pNext;
		}
		m_pPos = (*m_pPos).GetNextLine();
	}

	return 1;
}

/*---------------------------------------------------------------*
* Purpose	: Save TextItem Information
* Return Valuse : If Successed return TRUE; else return FALSE;
* Input		: pszFile -- Save file name, 
*			  pszSection -- Save section name;	
*----------------------------------------------------------------*/
BOOL CText::Save(LPCTSTR pszFile,LPCTSTR pszSection)
{
	UNREFERENCED_PARAMETER(pszFile);
	UNREFERENCED_PARAMETER(pszSection);
//	ASSERT(pszFile);
//	ASSERT(pszSection);

/*	ASSERT(preAction() == 1);

	m_pPos = m_pHead;
	VERIFY(WritePrivateProfileString(pszSection,"Type","Text",pszFile));					// item type : Text
	VERIFY(WritePrivateProfileString(pszSection,"View","20,320,90,350",pszFile));			// position or rect
	VERIFY(WritePrivateProfileString(pszSection,"Text","Acer Printer Utility",pszFile));	// string
	VERIFY(WritePrivateProfileString(pszSection,"Format",MakeIntToString(m_nAlign),pszFile));			// Format	
	VERIFY(WritePrivateProfileString(pszSection,"Spacing",MakeIntToString(nLetterspace),pszFile));		// letters spacing
	VERIFY(WritePrivateProfileString(pszSection,"Margin",MakeIntToString(nLineHeight),pszFile));		// lines interval

	struct TEXTINFO* pTextItem = new struct TEXTINFO();
	int nLineth = 1;
	while(m_pPos != NULL)
	{
		m_pPos->m_pCur = m_pPos->GetDataPoint();
		while(m_pPos->m_pCur != NULL)
		{
			pTextItem->bBold = m_pPos->m_pCur->bBold;
			pTextItem->bItalic = m_pPos->m_pCur->bItalic;
			pTextItem->bUnderline = m_pPos->m_pCur->bUnderline;
			pTextItem->color = m_pPos->m_pCur->color;
			pTextItem->nSize = m_pPos->m_pCur->nSize;
			INT		nlength = m_pPos->m_pCur->string.GetLength();
			CString szTempstr = m_pPos->m_pCur->string;
			lstrcpy(pTextItem->szFaceName ,m_pPos->m_pCur->szFaceName);
			while(nlength > 40)
			{
				pTextItem->chMark = '+';							// + -- 長鏈未完;
				CString		opstr = szTempstr.Left(40);
				lstrcpy(pTextItem->pszStr,opstr);
				VERIFY(WritePrivateProfileStruct( pszSection,
												  MakeIntToString(nLineth++),
												  pTextItem,
												  sizeof(struct TextItem),
												  pszFile));
				szTempstr = szTempstr.Right(nlength - 40);
				nlength = nlength - 40;
			}
			lstrcpy(pTextItem->pszStr ,szTempstr);
			
			m_pPos->m_pCur = m_pPos->m_pCur->pNext;
			if (m_pPos->m_pCur != NULL)	pTextItem->chMark = '\\';	// \ -- 普通鏈結束;
			else if (m_pPos == m_pTail) pTextItem->chMark = '#';	// # -- 文本結束;
			else pTextItem->chMark = '@';							// @ -- 一行結束;
			VERIFY(WritePrivateProfileStruct( pszSection,
											  MakeIntToString(nLineth++),
											  pTextItem,
											  sizeof(struct TextItem),
											  pszFile));
		}
		m_pPos = m_pPos->GetNextLine();
	}
*/
	return TRUE;
}

/*------------------------------------------------------------------*
* Pirpose	: Read TextItem Information; Created an new CText Object
* Return Valuse	: If Successed return TRUE; else return FALSE;
* Input		: pszFile -- Save file name, 
*			  pszSection -- Save section name;	
*-------------------------------------------------------------------*/
BOOL CText::Read(LPCTSTR pszFile,LPCTSTR pszSection)
{
	UNREFERENCED_PARAMETER(pszFile);
	UNREFERENCED_PARAMETER(pszSection);
//	ASSERT(pszFile);
//	ASSERT(pszSection);

/*	DeleteAll();

	TCHAR buf[10];
	CString szTemp;
	int nLen = 0;
	int Temp;
	VERIFY(GetPrivateProfileString(pszSection,"Type","Text",buf,10,pszFile));
	VERIFY(GetPrivateProfileString(pszSection,"View","20,320,90,350",buf,10,pszFile));
	VERIFY(GetPrivateProfileString(pszSection,"Text","Acer Printer Utility",buf,10,pszFile));
	nLen = GetPrivateProfileString(pszSection,"Format","0",buf,10,pszFile);
	buf[nLen] = '\0';	szTemp = buf;
	MakeStringToInt(szTemp, &Temp);
	SetFormat(Temp);
	nLen = GetPrivateProfileString(pszSection,"Spacing","0",buf,10,pszFile);
	buf[nLen] = '\0';	szTemp = buf;
	MakeStringToInt(szTemp, &Temp);
	Set_Letterspace(Temp);
	nLen = GetPrivateProfileString(pszSection,"Margin","0",buf,10,pszFile);
	buf[nLen] = '\0';	szTemp = buf;
	MakeStringToInt(szTemp, &Temp);
	Set_LineHeight(Temp);

	m_pPos = m_pHead;
	struct TEXTINFO* pTextItem = new struct TEXTINFO();
	int nLine = 1;
	while(TRUE)
	{
		VERIFY(GetPrivateProfileStruct( pszSection,
										MakeIntToString(nLine++),
										pTextItem,
										sizeof(struct TextItem),
										pszFile));
		m_pPos->m_pCur = m_pPos->GetDataPoint();
		m_pPos->m_pCur->bBold = pTextItem->bBold;
		m_pPos->m_pCur->bItalic = pTextItem->bItalic;
		m_pPos->m_pCur->bUnderline = pTextItem->bUnderline;
		m_pPos->m_pCur->color = pTextItem->color;
		m_pPos->m_pCur->string = pTextItem->pszStr;
		m_pPos->m_pCur->nSize = pTextItem->nSize;
		m_pPos->m_pCur->szFaceName = pTextItem->szFaceName;
		while(TRUE)
		{
			if (pTextItem->chMark == '@' || pTextItem->chMark == '#') break;
			else if (pTextItem->chMark == '\\')
			{
				VERIFY(GetPrivateProfileStruct( pszSection,
												MakeIntToString(nLine++),
												pTextItem,
												sizeof(struct TextItem),
												pszFile));

				struct ListInfo* pListpos = new struct ListInfo();
				pListpos->bBold = pTextItem->bBold;
				pListpos->bItalic = pTextItem->bItalic;
				pListpos->bUnderline = pTextItem->bUnderline;
				pListpos->color = pTextItem->color;
				pListpos->nSize = pTextItem->nSize;
				pListpos->string = pTextItem->pszStr;
				pListpos->szFaceName = pTextItem->szFaceName;
				pListpos->pNext = NULL;

				m_pPos->m_pCur->pNext = pListpos;
				m_pPos->m_pCur = m_pPos->m_pCur->pNext;
			}
			else if (pTextItem->chMark == '+')
			{
				VERIFY(GetPrivateProfileStruct( pszSection,
												MakeIntToString(nLine++),
												pTextItem,
												sizeof(struct TextItem),
												pszFile));

				m_pPos->m_pCur->string = m_pPos->m_pCur->string + pTextItem->pszStr;
			}
		}// __End of Line__
	if (pTextItem->chMark == '@')//	文本尚未結束;
		this->Appendline("");
	else if (pTextItem->chMark == '#') break;
	}
*/
	return TRUE;
}
//////////////////////////////////////////////////////////////////////
// CLineInfo Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLineInfo::CLineInfo()
{
	try
	{
		m_rect.top    = 
		m_rect.bottom = 
		m_rect.left   = 
		m_rect.right  = 0;
		m_pNextLine = m_pPreLine = NULL;
		m_pData = NULL;
		m_pData = new struct ListInfo();
		m_pCur = m_pData;
	}
	catch(CMemoryException* e)
	{
		e->Delete();
		m_pCur = 
		m_pData = NULL;
	}
}

CLineInfo::~CLineInfo()
{
	while(m_pData)
	{
		ASSERT(m_pData);
		m_pCur = (*m_pData).pNext;
		delete m_pData;
		m_pData = m_pCur;
	}
	m_pNextLine =  m_pPreLine = NULL;
}

/*-----------------------------------------------------*
 * Purpose	: Set current rectangle;
 * Input	: TextStyle	==> Horizontal=0 , Vertical=1;
 * Return Value: if Successed Return 1;else Return -1;
 *-----------------------------------------------------*/
int CLineInfo::SetControlRect(int TextStyle, int nLetterspace /* =0 */,int nLineHeight /* =0 */)
{
	m_pCur = m_pData;
	int nLength ,nMaxWidth ,nMaxHeight, nUnitLength;

	nLength    = 
	nMaxWidth  = 
	nMaxHeight = 
	nUnitLength = 0;

	while(m_pCur)
	{
		ASSERT(m_pCur);
		nLength += (*m_pCur).GetLength();	// the current line charactor total;
		nUnitLength += (*m_pCur).GetUnitLength();
		if (nMaxWidth < int((*m_pCur).GetWidth(2))) nMaxWidth = (*m_pCur).GetWidth(2);
		if (nMaxHeight < int((*m_pCur).GetHeight())) nMaxHeight = (*m_pCur).GetHeight();

		m_pCur = (*m_pCur).pNext;
	}

	if (nUnitLength > nLength) 
	{	// 單元字符比實際的字符還多?
		ASSERT(0);
		return -1;
	}
	m_rect.top    = 
	m_rect.left   = 
	m_rect.top    = 
	m_rect.bottom = 0;

	switch(TextStyle)
	{
	case 0:	

		m_rect.bottom = m_rect.top + nMaxHeight + nLineHeight;
		m_rect.right = m_rect.left;
		m_pCur = m_pData;
		while(m_pCur != NULL)
		{
			ASSERT(m_pCur);
			m_rect.right += nLetterspace * (*m_pCur).GetLength()+ (*m_pCur).GetWidth();
			m_pCur = (*m_pCur).pNext;
		}
		//m_rect.right = m_rect.right;// - nLetterspace;
		break;

	case 1:	

		if ( nUnitLength == nLength)
		{	// 本行只有 singer character;
			m_rect.bottom = m_rect.top + nMaxHeight + nLineHeight;
			m_rect.right = m_rect.left;
			m_pCur = m_pData;
			while(m_pCur)
			{
				int nlenght = (*m_pCur).GetLength();
				int nunitlength = (*m_pCur).GetUnitLength();
				if (nunitlength == nlenght)
				{
					m_rect.right = m_rect.right 
							      + nLetterspace * nlenght + (*m_pCur).GetWidth();
				}
				else // ERROR;
				{
					TRACE("ERROR Operation(CLineInfo::SetControlRect)");
					return -1;
				}
				m_pCur = (*m_pCur).pNext;
			}
		}
		else if (nLength > nUnitLength)
		{	// 本行含有 double character;
			m_rect.right = m_rect.left;
			m_pCur = m_pData;
			nMaxHeight = 0;
			while(m_pCur)
			{
				ASSERT(m_pCur);
				int nlength = (*m_pCur).GetLength();
				int nunitlength = (*m_pCur).GetUnitLength();
				if ( nlength == nunitlength)
				{	// 本鏈只有 singer character;
					if (nMaxHeight < int((*m_pCur).GetHeight())) nMaxHeight = (*m_pCur).GetHeight();
					m_rect.right = m_rect.left + (*m_pCur).GetWidth()
								  + nLetterspace * nlength;
				}
				else if ( nlength > nunitlength)
				{	// 本鏈含有 double character; 
					int temp = ((*m_pCur).GetHeight() > (*m_pCur).GetWidth(2)) ? int((*m_pCur).GetHeight()) : int((*m_pCur).GetWidth(2));
					if (nMaxHeight < temp) nMaxHeight = temp;
					m_rect.right = m_rect.left
							      + (nLetterspace + (*m_pCur).GetWidth(2)) *(2*nunitlength - nlength)
								  + (nLetterspace + (*m_pCur).GetHeight()) *(nlength - nunitlength);
				}
				else
					ASSERT(0);
				m_pCur = (*m_pCur).pNext;
			}
			m_rect.right -= nLetterspace;
			m_rect.bottom = m_rect.top - nMaxHeight - nLineHeight;
		}
		else
			ASSERT(0);
		break;

	default:

#ifdef _DEBUG
	//	ErrorMessageBox(MP_ERR_07,MB_OK);
#endif
		return -1;

	}
	return 1;
}

/*----------------------------------------------------------*
 * Purpose	: Get Current Line all character;
 * Return Value : GetLineStr;
 *----------------------------------------------------------*/
void CLineInfo::GetLineStr(CString& szLine)
{
	szLine.Empty();

	m_pCur = m_pData;
	while(m_pCur)
	{
		ASSERT(m_pCur);
		szLine += (*m_pCur).string;
		m_pCur = (*m_pCur).pNext;
	}
}
/*-------------------------------------------------------*
 * Purpose	: Get Current Line all character acount;
 * Return Value: GetLineLength;
 *-------------------------------------------------------*/
UINT CLineInfo::GetLineLength()
{
	m_pCur = m_pData;
	UINT   length = 0;
	while(m_pCur)
	{
		ASSERT(m_pCur);
		length += (*m_pCur).GetLength();
		m_pCur = (*m_pCur).pNext;
	}
	return length;
}

/*---------------------------------------------------------*
 * Purpose	: Get Current Line unit character acount;
 * Return Value	: GetlineUnitLength;
 *---------------------------------------------------------*/
UINT CLineInfo::GetLineUnitLength()
{
	m_pCur = m_pData;
	UINT  ulength = 0;
	while(m_pCur)
	{
		ASSERT(m_pCur);
		ulength += (*m_pCur).GetUnitLength();
		m_pCur = (*m_pCur).pNext;
	}
	return ulength;
}

/*------------------------------------------------------------*
 * Purpose	: Get Control Rectangle;
 * Return Value	: return CRect class;
 *------------------------------------------------------------*/
CRect CLineInfo::GetControlRect() const
{
	CRect rectangle;
	rectangle.CopyRect(m_rect);

	return rectangle;
}

int CLineInfo::SetNextLine(CLineInfo *point /* =NULL */)
{
	m_pNextLine = point;
	return 1;
}

int CLineInfo::SetPreLine(CLineInfo *point /* =NULL */)
{
	m_pPreLine = point;
	return 1;
}

CLineInfo*	CLineInfo::GetNextLine() const
{
	return m_pNextLine;
}

CLineInfo*  CLineInfo::GetPreLine() const
{
	return m_pPreLine;
}

struct ListInfo* CLineInfo::GetDataPoint() const
{
	return m_pData;
}

BOOL CLineInfo::SetDataPoint(struct ListInfo* plistinfo)
{
	m_pData = plistinfo;
	return TRUE;
}

UINT CLineInfo::GetLineIndex() const
{
	return m_nLineIndex;
}

UINT CLineInfo::SetLineIndex(UINT nIndex /* =0*/)
{
	if (nIndex == 0) return 0;

	m_nLineIndex = nIndex;
	return m_nLineIndex;
}
//////////////////////////////////////////////////////////////////////
// ListInfo Struct
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ListInfo::ListInfo()
{
	pNext = NULL;
	//string.Empty();
	string = _T("");
	szFaceName = _T("");

	bBold = FALSE;
	bItalic = FALSE;
	bUnderline = FALSE;
	color = RGB(0, 0, 0);
}

ListInfo::~ListInfo()
{
}

/*----------------------------------------------------*
 * Purpose	: Get string Length;
 * Return Value: Return Length;
 *----------------------------------------------------*/
int	ListInfo::GetLength()
{
	return string.GetLength();
}

/*----------------------------------------------------*
 * Purpose	: Get Unit character Length;
 * Return Value	: Return Unit character Length;
 *----------------------------------------------------*/
int	ListInfo::GetUnitLength()
{
	int count=0;
	for(int i = 0; i < string.GetLength();i++)
	{
		TCHAR ichar;
		ichar = string[i];
		if(int(ichar) < 0) 
		{
			count++;
			i++;
		}
		else count++;
	}
	return count;
}

/*--------------------------------------------------------------------*
 * Purpose		:	Compare twe string between str to tempstr;
 * Return value	:	if it equal return true else return false;
 *--------------------------------------------------------------------*/
BOOL ListInfo::Compare_str(CString tempstr) const
{
	if (string.Compare((LPCTSTR) tempstr) != 0)
		return FALSE;
	else 
		return TRUE;
}

BOOL ListInfo::Compare_str(LPCTSTR tempstr) const
{
	if (string.Compare(tempstr) != 0)
		return FALSE;
	else 
		return TRUE;
}

/*------------------------------------------------*
 * Purpose : compare font;
 * Return Value : if its equal return TRUE;
 *------------------------------------------------*/
BOOL ListInfo::Compare_LOGFONT(LOGFONT* logfont) const
{
	if (bItalic != (*logfont).lfItalic) return FALSE;
	if (bUnderline != (*logfont).lfUnderline) return FALSE;
	if (bBold)
	{
		if ((*logfont).lfWeight != 700) return FALSE;
	}
	else if ((*logfont).lfWeight == 700) return FALSE;

	if (szFaceName.Compare((*logfont).lfFaceName) != 0) return FALSE;
	// nSize 
	return TRUE;
}

/*-------------------------------------------------*
 * Purpose : Set font attribuate;
 *-------------------------------------------------*/
BOOL ListInfo::SetFont(LOGFONT *logfont, UINT size)
{
	bItalic = (*logfont).lfItalic;
	bUnderline = (*logfont).lfUnderline;
	if ((*logfont).lfWeight == 700) 
		bBold = TRUE;
	else 
		bBold = FALSE;
	szFaceName = (*logfont).lfFaceName;

	nSize = size;
	return TRUE;
}

LOGFONT ListInfo::GetFont()
{
	LOGFONT logfont;
	logfont.lfCharSet		= DEFAULT_CHARSET;
	logfont.lfClipPrecision	= CLIP_DEFAULT_PRECIS;
	logfont.lfEscapement	= 0;
	logfont.lfHeight		= GetHeight(2);
	logfont.lfItalic		= (unsigned char)bItalic;
	logfont.lfOrientation	= 0;
	logfont.lfOutPrecision	= OUT_DEVICE_PRECIS;
	logfont.lfQuality		= PROOF_QUALITY;
	logfont.lfStrikeOut		= FALSE;
	logfont.lfUnderline		= (unsigned char)bUnderline;
	logfont.lfPitchAndFamily = DEFAULT_PITCH;
	logfont.lfWeight		= bBold ? 700 : 400;
	logfont.lfWidth			= GetWidth(2);

	lstrcpy(logfont.lfFaceName ,szFaceName.GetBuffer(1));

	return logfont;
}

/*----------------------------------------*
 * Purpose	: Set next List
 *----------------------------------------*/
void ListInfo::SetNextList(struct ListInfo *point)
{
	pNext = point;
	return;
}

/*------------------------------------------------------------------*
 *	Purpose	: virual decvice context get selected font width
 *------------------------------------------------------------------*/
UINT ListInfo::GetWidth(INT temp /*= 1*/)
{
	if (temp !=1)	return UINT(nSize * 0.5);

	CDC memDC;
	CSize sz;
	memDC.CreateCompatibleDC(NULL);
	CFont curFont,*pOldFont;
	LOGFONT logfont;
	logfont = GetFont();
	curFont.CreateFontIndirect(&logfont);
	pOldFont = memDC.SelectObject(&curFont);
	sz = memDC.GetOutputTextExtent(string, GetLength());

	memDC.SelectObject(pOldFont);
	memDC.DeleteDC();
    curFont.DeleteObject ();
	if (bItalic == TRUE)
		return UINT(sz.cx+GetWidth(2)*0.1);
	return sz.cx;
}

/*--------------------------------------------------------*
 * Purpose	: if temp not equal or string was empty 
 *			  we regard the string height is equal 
 *			  nSize*0.866; else with virtal decvice context
 *			  get current string height;
 *--------------------------------------------------------*/
UINT ListInfo::GetHeight(INT temp /* = 1*/)
{
	if (temp != 1) 	return UINT(nSize * 0.866025403784438646763723170752936);
	if (string == _T("")) return UINT(nSize * 0.866025403784438646763723170752936);

	CDC memDC;
	CSize sz;
	memDC.CreateCompatibleDC(NULL);
	CFont curFont,*pOldFont;
	LOGFONT logfont;
	logfont = GetFont();
	curFont.CreateFontIndirect(&logfont);
	pOldFont = memDC.SelectObject(&curFont);
	sz = memDC.GetOutputTextExtent(string, GetLength());

	memDC.SelectObject(pOldFont);
	memDC.DeleteDC();
	curFont.DeleteObject ();
	return sz.cy;

}

