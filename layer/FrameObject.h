	/* =========================================================== *\
     
		Copyright (C) 1999 Acer Peripherals, Inc. 
		All rights reserved.
     
		Revision History:
		version 1.0, Jan 10, 2000.  PhotoFamily 1.0. 
		Author:   Robert Chen
	\* =========================================================== */
// FrameObject.h: interface for the CFrameObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FRAMEOBJECT_H__14805961_C27E_11D3_A3C0_0050BAC2366B__INCLUDED_)
#define AFX_FRAMEOBJECT_H__14805961_C27E_11D3_A3C0_0050BAC2366B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "EditObject.h"
#include "layer.h"

class CFrameObject : public CImageObject		//Seem as CEdgeObject
{
private:
	LONG			m_FrameWidth;
	LONG			m_FrameHeight;
	LONG			m_OldWidth,	m_OldHeight;
	LPBYTE			m_pPic;
	LPBYTE			m_ImageData;
	LPBITMAPINFO	m_ImageInfo;
	CLayerObject	*m_pLayerObj,*m_pLayerBak;

public: 
	CFrameObject();
	~CFrameObject();
	virtual BOOL Read(LPCTSTR PfmFile);
	virtual BOOL UpdateImage();
	virtual VOID Delete();
	virtual VOID Draw();

	virtual VOID MoveForeImage(CSize size) ;
	virtual VOID GetForeImageRect(CRect &rect) ;
	
	VOID DrawLayer();
	BOOL OpenFile2(LPCTSTR lpszPathName, CLayerObject* layer,LONG type);
	VOID Resize(LONG width , LONG height);

};
#endif // !defined(AFX_FRAMEOBJECT_H__14805961_C27E_11D3_A3C0_0050BAC2366B__INCLUDED_)
