

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

	BITMAPINFO	m_TemplateBmpInfo , //�Ҫ��Ϲ��H��
				m_TemplateMaskInfo, //�Ҫ��Ϲ��B�n�H��
				m_ForeMaskInfo	  , //�e���Ϲ��B�n�H��
				m_ForeInfo		  ,
				m_LayerBmpInfo	  ; //�_�X�h�Ϲ��H��

	LPBYTE 		m_TemplateBmpData ,  //�Ҫ��Ϲ��ƾ�
				m_TemplateMaskData,	 //�Ҫ��Ϲ��B�n�ƾ�
				m_ForeMaskData	  ,  //�e���Ϲ��B�n�ƾ�
				m_ForeData		  ,
				m_LayerBmpData	  ;	 //�_�X�h�Ϲ��ƾ�

	CRect		m_rectLayer;		//�ϼh��m
	CPoint		m_ForePlace;		//�e���Ϧ�m

	LONG		m_LayerWidth,		//�ϼh���e
				m_LayerHeight,
				m_ForeWidth ,		//�e���Ϫ��e
				m_ForeHeight ,
				m_LayerKind	 ;		//�ϼh���O

//	double		m_LayerScale;	//�Y����
//				m_ForeScale ,
//				m_TemplateScale;
		
// Operations
public:
	bool FreeAllDC();	//����Ҧ��ƾڶ�
	void SetKind(LONG Kind);	//�]�w�ϼh���O
	bool CopyToFore(LONG placex, LONG placey); //���ܫe���Ϧ�m
	bool CopyToBack(LPBYTE pDCData, LPBITMAPINFO pDCInfo, 
					LONG placex, LONG placey); //���ܹϼh��m

	bool SetFore(LPBITMAPINFO& pInfo, LPBYTE& pData);		//�إ߫e���Ϲ�
	bool SetTemplate(LPBITMAPINFO pInfo, LPBYTE pData);		//�إ߼Ҫ��Ϲ�
	bool SetForeMask(LPBITMAPINFO pInfo, LPBYTE pData);		//�إ߫e���Ϲ��B�n
	bool SetTemplateMask(LPBITMAPINFO pInfo, LPBYTE pData);	//�إ߼Ҫ��Ϲ��B�n


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
LONG AdjustWidth(LONG width);		//�зǤƹϹ��e�׼ƾ�

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
void  MemBitBlt(					//���s�������, ����BitBlt
					LPBYTE pTarData,		//�ؼмƾڶ�
					LONG TarWidth,			//�ؼмƾڶ��e��
					LONG TarHeiight,		//�ؼмƾڶ�����
					LONG TarX,
					LONG TarY, 
					LONG nWidth, 
					LONG nHeight,
					LPBYTE pSrcData,		//���ƾڶ�
					LONG SrcWidth,			//���ƾڶ��e��
					LONG SrcHeiight,		//���ƾڶ�����
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
				
////////���s�����Y���, �����ϥ�//////
void  MemStretchBlt(LPBYTE pTarData, //�ؼмƾڶ�
				   LONG TarWidth, 
				   LONG TarHeight,
				   LPBYTE pSrcData, //���ƾڶ�
				   LONG SrcWidth, 
				   LONG SrcHeight);											  
				  
void  Bmp256to16M(LPBITMAPINFO p256Info, 
				  LPBYTE p256Data, 
				  LPBITMAPINFO p16MInfo,
				  LPBYTE p16MData);

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LAYER1_H__181B6CC1_638F_11D3_A3C0_00A0C9934E3F__INCLUDED_)
