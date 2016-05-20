

#if !defined(AFX_LAYER1_H__181B6CC1_638F_11D3_A3C0_00A0C9934E3F__INCLUDED_)
#define AFX_LAYER1_H__181B6CC1_638F_11D3_A3C0_00A0C9934E3F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Layer1.h : header file
//
//#include "stdafx.h"
#define	LAYER_SINGLE	1
#define	LAYER_MULTI		2
/////////////////////////////////////////////////////////////////////////////
// CLayer command target

class CLayerObject
{
// Attributes
public:
	CLayerObject();           // protected constructor used by dynamic creation

	BITMAPINFO	m_TemplateBmpInfo , //模版圖像信息
				m_TemplateMaskInfo, //模版圖像遮罩信息
				m_ForeMaskInfo	  , //前景圖像遮罩信息
				m_ForeInfo		  ,
				m_LayerBmpInfo	  ; //復合層圖像信息

	LPBYTE 		m_TemplateBmpData ,  //模版圖像數據
				m_TemplateMaskData,	 //模版圖像遮罩數據
				m_ForeMaskData	  ,  //前景圖像遮罩數據
				m_ForeData		  ,
				m_LayerBmpData	  ;	 //復合層圖像數據

	CRect		m_rectLayer;		//圖層位置
	CPoint		m_ForePlace;		//前景圖位置

	LONG		m_LayerWidth,		//圖層長寬
				m_LayerHeight,
				m_ForeWidth ,		//前景圖長寬
				m_ForeHeight ,
				m_LayerKind	 ;		//圖層類別

//	double		m_LayerScale;	//縮放比例
//				m_ForeScale ,
//				m_TemplateScale;
		
// Operations
public:
	bool FreeAllDC();	//釋放所有數據塊
	void SetKind(LONG Kind);	//設定圖層類別
	bool CopyToFore(LONG placex, LONG placey); //改變前景圖位置
	bool CopyToBack(LPBYTE pDCData, LPBITMAPINFO pDCInfo, 
					LONG placex, LONG placey); //改變圖層位置

	bool SetFore(LPBITMAPINFO& pInfo, LPBYTE& pData);		//建立前景圖像
	bool SetTemplate(LPBITMAPINFO pInfo, LPBYTE pData);		//建立模版圖像
	bool SetForeMask(LPBITMAPINFO pInfo, LPBYTE pData);		//建立前景圖像遮罩
	bool SetTemplateMask(LPBITMAPINFO pInfo, LPBYTE pData);	//建立模版圖像遮罩


	virtual ~CLayerObject();
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLayer)
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CLayer)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

};

/*
typedef struct tag_LayerInfo
{
	WORD	LayerKind ;
	LONG	LayerWidth;
	LONG	LayerHeight; 
	LONG	LayerPlaceX ;
	LONG	LayerPlaceY ;
	LONG	ForePlaceX ;
	LONG	ForePlaceY ;
	LONG	ForeWidth  ;
	LONG	ForeHeight ;

}LAYERINFO, *LPLAYERINFO;
*/
/////////////////////////////////////////////////////////////////////////////
LONG AdjustWidth(LONG width);		//標準化圖像寬度數據

VOID  MemBitTile(  LPBYTE pTarData,	//Target data
				   LONG lTarAreaL,  //Target area left
				   LONG lTarAreaT,  //Target area top
				   LONG lTarAreaW,  //Target area width
				   LONG lTarAreaH,  //Target area height
				   LONG lTarWidth,  //Target data height
				   LPBYTE pSrcData, //Source data
				   LONG lSrcWidth,  //Source data width
				   LONG lSrcHeigh); //Source data height

//MemBitTile() is added by Bill 2001/02/22
void  MemBitBlt(					//內存拷貝函數, 類似BitBlt
					LPBYTE pTarData,		//目標數據塊
					LONG TarWidth,			//目標數據塊寬度
					LONG TarHeiight,		//目標數據塊長度
					LONG TarX,
					LONG TarY, 
					LONG nWidth, 
					LONG nHeight,
					LPBYTE pSrcData,		//源數據塊
					LONG SrcWidth,			//源數據塊寬度
					LONG SrcHeiight,		//源數據塊長度
					LONG SrcX,
					LONG SrcY);
					
void  MemAddWidth(LPBYTE  pTar,LONG TarLen,
				LPBYTE  pSrc,LONG SrcLen);
				
void  MemDecWidth(LPBYTE  pTar,LONG TarLen,
				LPBYTE  pSrc,LONG SrcLen);
				
void	MemDecHeight(LPBYTE	pTar,LONG TarWidth, LONG TarHeight,
				LPBYTE pSrc,LONG SrcWidth, LONG SrcHeight);
				
void	MemAddHeight(LPBYTE	pTar,LONG TarWidth, LONG TarHeight,
				LPBYTE pSrc,LONG SrcWidth, LONG SrcHeight);
				
////////內存塊放縮函數, 直接使用//////
void  MemStretchBlt(LPBYTE pTarData, //目標數據塊
				   LONG TarWidth, 
				   LONG TarHeight,
				   LPBYTE pSrcData, //源數據塊
				   LONG SrcWidth, 
				   LONG SrcHeight);											  
				  
void  Bmp256to16M(LPBITMAPINFO p256Info, 
				  LPBYTE p256Data, 
				  LPBITMAPINFO p16MInfo,
				  LPBYTE p16MData);

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LAYER1_H__181B6CC1_638F_11D3_A3C0_00A0C9934E3F__INCLUDED_)
