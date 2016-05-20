
// Layer1.cpp : implementation file
//

#include "stdafx.h"
#include "Layer.h"
//#include "SO_CR2.h"
//#include "memDataAlgorithm.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//標準化圖像寬度數據
LONG AdjustWidth(LONG width)
{
	if(width%4==0)	return	width;
	else	return	width-width%4+4;
}

//MemBitTile() is added by Bill 2001/02/22
//pSrcData is tiled to pTarData
VOID  MemBitTile(  LPBYTE pTarData,	//Target data
				   LONG lTarAreaL,  //Target area left
				   LONG lTarAreaT,  //Target area top
				   LONG lTarAreaW,  //Target area width
				   LONG lTarAreaH,  //Target area height
				   LONG lTarWidth,  //Target data height
				   LPBYTE pSrcData, //Source data
				   LONG lSrcWidth,  //Source data width
				   LONG lSrcHeigh ) //Source data height
{
	LONG lTarBPS = (lTarWidth*3+3)/4*4;
	LONG lSrcBPS = (lSrcWidth*3+3)/4*4;

	LONG hn,ht,vt;
	hn = lTarAreaW / lSrcWidth;
	ht = lTarAreaW % lSrcWidth;
	vt = lTarAreaH % lSrcHeigh;
	LONG lTemp = -lTarAreaT+lSrcHeigh-vt;

	for(LONG j=lTarAreaT; j<lTarAreaT+lTarAreaH; j++)
	{
		for(LONG h=0; h<hn; h++)
		{
			memcpy(pTarData + j*lTarBPS + (lTarAreaL + h*lSrcWidth)*3, pSrcData+((j+lTemp)%lSrcHeigh)*lSrcBPS, lSrcWidth*3);
		}
		memcpy(pTarData + j*lTarBPS + (lTarAreaL + hn*lSrcWidth)*3, pSrcData+((j+lTemp)%lSrcHeigh)*lSrcBPS, ht*3);
	}
}

//內存拷貝函數, 類似BitBlt
void  MemBitBlt(LPBYTE pTarData, LONG TarWidth, LONG TarHeight,
				LONG TarX,LONG TarY, LONG nWidth, LONG nHeight,
				LPBYTE pSrcData, LONG SrcWidth, LONG SrcHeight,
				LONG SrcX, LONG SrcY)
{
	CRect	rectTar, rectSrc;

	rectSrc.SetRect(max(0,SrcX),max(0,SrcY),
					min(SrcWidth ,SrcX+nWidth),
					min(SrcHeight,SrcY+nHeight));
	if(rectSrc.IsRectEmpty())	return;

	rectSrc.SetRect(max(rectSrc.left  ,SrcX-TarX),
					max(rectSrc.top   ,SrcY-TarY),
					min(rectSrc.right ,max(rectSrc.left,SrcX-TarX)+TarWidth),
					min(rectSrc.bottom,max(rectSrc.top ,SrcY-TarY)+TarHeight));
	if(rectSrc.IsRectEmpty())	return;

	rectTar.SetRect(max(0,max(TarX,TarX-SrcX)),max(0,max(TarY,TarY-SrcY)),
					min(TarWidth ,max(0,max(TarX,TarX-SrcX))+rectSrc.Width()),
					min(TarHeight,max(0,max(TarY,TarY-SrcY))+rectSrc.Height()));
	if(rectTar.IsRectEmpty())	return;

	int Srcline=SrcHeight-rectSrc.top-1;
	BYTE *pSRC,*pTAR;
	for(int Tarline=TarHeight-rectTar.top-1;
			Tarline>=TarHeight-rectTar.top-rectTar.Height();
			Tarline--)//y ori .
	{
	pSRC=pSrcData+Srcline*AdjustWidth(SrcWidth*3)+rectSrc.left*3;
	pTAR=pTarData+Tarline*AdjustWidth(TarWidth*3)+rectTar.left*3;

	memcpy(pTAR,pSRC,rectTar.Width()*3);//x ori .
	Srcline--;
	}
}

//////// 放大寬度函數, 內部調用 ///////////
void  MemAddWidth(LPBYTE  pTar,LONG TarLen,
				  LPBYTE  pSrc,LONG SrcLen)
{
	LONG	i,len,srclen;
	float	fmod,fadd;
	LPBYTE  pInSrc;
	fmod=(float)(SrcLen)/(float)(TarLen);
	fadd=fmod;
	pInSrc=pSrc;
	srclen=0;
	len=0;
	for(i=0;i<TarLen;i++)
	{
		memcpy(pTar+len,pInSrc,3);
		if((int)(fadd+fmod)-(int)fadd==1&&srclen<SrcLen-1)
		{
			pInSrc+=3;
			srclen++;
		}
		len+=3;
		fadd+=fmod;
	}
}

///////縮小寬度函數 , 內部調用////////
void  MemDecWidth(LPBYTE  pTar,LONG TarLen,
				  LPBYTE  pSrc,LONG SrcLen)
{
	LONG	i,len;
	float	fmod,fadd;
	LPBYTE  pInSrc;
	fmod=(float)(TarLen)/(float)(SrcLen);
	fadd=0;//fmod;
	pInSrc=pSrc;
	len=0;
	TarLen=AdjustWidth(TarLen*3)-3;
	for(i=0;i<SrcLen;i++)
	{
//		pInSrc+=3;
		if(len>TarLen) break;
		if((int)(fadd+fmod)-(int)fadd==1||i==0)
		{
			memcpy(pTar+len,pInSrc,3);
			len+=3;
		}
		pInSrc+=3;
		fadd+=fmod;
	}
	if(len<=TarLen)	memcpy(pTar+len,pInSrc-3,3);
}

///////縮小高度函數 , 內部調用////////
void	MemDecHeight(LPBYTE	pTar,LONG TarWidth, LONG TarHeight,
					 LPBYTE pSrc,LONG SrcWidth, LONG SrcHeight)
{
	LONG	i,j,n,m,len,AdTarWidth,AdSrcWidth;
	float	fmod,fadd;
	LPBYTE  pInTar;
	n=SrcHeight/TarHeight;
	m=SrcHeight%TarHeight;

	fmod=(float)m/(float)TarHeight;
	fadd=fmod;

	AdTarWidth=AdjustWidth(TarWidth*3);
	AdSrcWidth=AdjustWidth(SrcWidth*3);	

	pInTar=pTar;
	len=0;

  if(TarWidth>=SrcWidth)
	for(i=0;i<TarHeight;i++)
	{
		MemAddWidth(pInTar,TarWidth,pSrc+len,SrcWidth);
		for(j=0;j<n;j++) len+=AdSrcWidth;
			if((int)(fadd+fmod)-(int)fadd==1)
				len+=AdSrcWidth;
			fadd+=fmod;
		pInTar+=AdTarWidth;
	}
  else
	for(i=0;i<TarHeight;i++)
	{
		MemDecWidth(pInTar,TarWidth,pSrc+len,SrcWidth);
		for(j=0;j<n;j++) len+=AdSrcWidth;
		if(m!=0)
		{
			if((int)(fadd+fmod)-(int)fadd==1)
				len+=AdSrcWidth;
			fadd+=fmod;
		}
		pInTar+=AdTarWidth;
	}
}

///////增大高度函數 , 內部調用////////
void	MemAddHeight(LPBYTE	pTar,LONG TarWidth, LONG TarHeight,
					 LPBYTE pSrc,LONG SrcWidth, LONG SrcHeight)
{
	LONG	i,len,Srclen, AdTarWidth,AdSrcWidth;
	float	fmod,fadd;
	LPBYTE  pInSrc;
	fmod=(float)SrcHeight/(float)TarHeight;
	fadd=fmod;
	pInSrc=pSrc;
	len=0;
	Srclen=0;
	AdTarWidth=AdjustWidth(TarWidth*3);
	AdSrcWidth=AdjustWidth(SrcWidth*3);
  if(TarWidth>=SrcWidth)
	for(i=0;i<TarHeight;i++)
	{
		MemAddWidth(pTar+len,TarWidth,pInSrc,SrcWidth);
		if((int)(fadd+fmod)-(int)fadd==1&&Srclen<SrcHeight-1)
		{
			pInSrc+=AdSrcWidth;
			Srclen++;
		}
		len+=AdTarWidth;
		fadd+=fmod;
	}
  else
	for(i=0;i<TarHeight;i++)
	{
		MemDecWidth(pTar+len,TarWidth,pInSrc,SrcWidth);
		if((int)(fadd+fmod)-(int)fadd==1&&Srclen<SrcHeight-1)
		{
			pInSrc+=AdSrcWidth;
			Srclen++;
		}
		len+=AdTarWidth;
		fadd+=fmod;
	}

}

////////內存塊放縮函數, 直接使用//////
void  MemStretchBlt(LPBYTE pTarData, //目標數據塊
				   LONG TarWidth, 
				   LONG TarHeight,
				   LPBYTE pSrcData, //源數據塊
				   LONG SrcWidth, 
				   LONG SrcHeight)
{

	if(TarHeight>=SrcHeight)
	MemAddHeight(pTarData,TarWidth,TarHeight,
				 pSrcData,SrcWidth,SrcHeight);
	else
	MemDecHeight(pTarData,TarWidth,TarHeight,
				 pSrcData,SrcWidth,SrcHeight);
}

void  Bmp256to16M(LPBITMAPINFO p256Info, 
				  LPBYTE p256Data, 
				  LPBITMAPINFO p16MInfo,
				  LPBYTE p16MData)
{
	INT	i ,j ,width1,width2,len1,len2;
		width1 = AdjustWidth(p256Info->bmiHeader.biWidth);
		width2 = AdjustWidth(p16MInfo->bmiHeader.biWidth*3);
	for(j=0; j<p256Info->bmiHeader.biHeight; j++)
	{
		len1 = j*width1;
		len2 = j*width2;
		for(i=0; i<p256Info->bmiHeader.biWidth ; i++)
		{
			*(p16MData+len2+i*3)   = p256Info->bmiColors[*(p256Data+len1+i)].rgbBlue ;
			*(p16MData+len2+i*3+1) = p256Info->bmiColors[*(p256Data+len1+i)].rgbGreen;
			*(p16MData+len2+i*3+2) = p256Info->bmiColors[*(p256Data+len1+i)].rgbRed  ;
		}
	}
}

// CLayer
CLayerObject::CLayerObject()
{
	m_TemplateBmpData=NULL ; 
	m_TemplateMaskData=NULL ;
	m_ForeMaskData=NULL ;
	m_LayerBmpData=NULL ;
	m_ForeData=NULL;
}

CLayerObject::~CLayerObject()
{
	FreeAllDC();
}
/////////////////////////////////////////////////////////////////////////////
// CLayer message handlers

//建立模版圖像
bool CLayerObject::SetTemplate(LPBITMAPINFO pInfo, LPBYTE pData)
{
	m_LayerWidth=pInfo->bmiHeader.biWidth;
	m_LayerHeight=pInfo->bmiHeader.biHeight;
	m_TemplateBmpInfo=*pInfo;
/////////	Creat TemplateBmpDC ///////////
	if(m_TemplateBmpData)
	{
		delete	m_TemplateBmpData;
		m_TemplateBmpData=NULL;
	}
	m_TemplateBmpData=pData;

	return	TRUE;
}

//建立前景圖像
bool CLayerObject::SetFore(LPBITMAPINFO& pInfo, LPBYTE& pData)
{
	if( pInfo->bmiHeader.biBitCount < 24 )
	{
		//ImageLib::ConvertToRGB(pInfo,pData);

	}
	m_ForeWidth=pInfo->bmiHeader.biWidth;
	m_ForeHeight=pInfo->bmiHeader.biHeight;
	m_ForeInfo=*pInfo;

	if(m_ForeData)
	{
		delete	m_ForeData;
		m_ForeData=NULL;
	}

	m_ForeData = pData ;
	return	TRUE;
}

//改變圖層位置
bool CLayerObject::CopyToBack(LPBYTE pDCData, LPBITMAPINFO pDCInfo,
						LONG placex, LONG placey)
{	
	m_rectLayer.SetRect(placex,placey,placex+m_LayerWidth,placey+m_LayerHeight);
			
	LONG	i,j;
	float percent;

	LPBYTE pTempData;
	////需處理的內存塊的長度
	j=m_LayerHeight*AdjustWidth(m_LayerWidth*3);

	////開闢臨時數據塊
	pTempData=new  BYTE[j];
	////複製源數據塊
//	m_TempDC.BitBlt(0,0,m_LayerWidth,m_LayerHeight,BackDC,placex,placey,SRCCOPY);
	MemBitBlt(pTempData,
			  m_LayerWidth,
			  m_LayerHeight,
			  0,
			  0,
			  m_LayerWidth,
			  m_LayerHeight,
			  pDCData,
			  pDCInfo->bmiHeader.biWidth,
			  pDCInfo->bmiHeader.biHeight,
			  placex,
			  placey);

//////融合圖層
if(m_LayerKind==LAYER_SINGLE)
{
	for (i=0;i<j;i++)
	{
		if(*(m_ForeMaskData+i)<250)
		{
			if(*(m_ForeMaskData+i)<5)
			{
				*(pTempData+i)=*(m_LayerBmpData+i);
			}
			else
			{
			percent=(float)(*(m_ForeMaskData+i)/255.0);
			*(pTempData+i)=(BYTE)(*(pTempData+i)*percent+
							*(m_LayerBmpData+i)*(1-percent));
			}
		}
	}
}
else
if(m_LayerKind==LAYER_MULTI)
{
	for (i=0;i<j;i++)
	{
		if(*(m_TemplateMaskData+i)<250)
		{
			if(*(m_TemplateMaskData+i)<5)
			{
				*(pTempData+i)=*(m_LayerBmpData+i);
			}
			else
			{
			percent=(float)(*(m_TemplateMaskData+i)/255.0);
			*(pTempData+i)=(BYTE)(*(pTempData+i)*percent+
						*(m_LayerBmpData+i)*(1-percent));
			}
		}
	}
}
/////////輸出
//	BackDC->BitBlt(placex,placey,m_LayerWidth,m_LayerHeight,&m_TempDC,0,0,SRCCOPY);
	MemBitBlt(pDCData,
			  pDCInfo->bmiHeader.biWidth,
			  pDCInfo->bmiHeader.biHeight,
			  placex,
			  placey,
			  m_LayerWidth,
			  m_LayerHeight,
			  pTempData,
			  m_LayerWidth,
			  m_LayerHeight,
			  0,
			  0);

	delete pTempData;
	return TRUE;
}

//改變前景圖位置
bool CLayerObject::CopyToFore(LONG placex, LONG placey)
{
	LONG	i,j;
	float percent;
	LPBITMAPINFO	ForeInfo;
	LPBYTE 		ForeData;

	ForeInfo = &m_ForeInfo ;
	ForeData = m_ForeData ;

	/////清除復合層圖像
	memset(m_LayerBmpData,0xFF,AdjustWidth(m_LayerWidth*3)*m_LayerHeight);

	//////拷貝前景圖
	MemBitBlt(m_LayerBmpData,
			  m_LayerBmpInfo.bmiHeader.biWidth,
			  m_LayerBmpInfo.bmiHeader.biHeight,
			  placex,
			  placey,
			  ForeInfo->bmiHeader.biWidth,
			  ForeInfo->bmiHeader.biHeight,
			  ForeData,
			  ForeInfo->bmiHeader.biWidth,
			  ForeInfo->bmiHeader.biHeight,
			  0,
			  0);

	/////多層還需做融合效果
	if(m_LayerKind==LAYER_MULTI)
	{

		j=m_LayerHeight*AdjustWidth(m_LayerWidth*3)-1;

		for (i=0;i<=j;i++)
		{
			if(*(m_ForeMaskData+i)<250)
			{
				if(*(m_ForeMaskData+i)>5)
				{
			percent=(float)(*(m_ForeMaskData+i)/255.0);
			*(m_LayerBmpData+i)=(BYTE)(*(m_TemplateBmpData+i)*percent+
							*(m_LayerBmpData+i)*(1-percent));
				}
			}
			else
			*(m_LayerBmpData+i)=*(m_TemplateBmpData+i);
		}

	}
		m_ForePlace.x=placex;	m_ForePlace.y=placey;
		return TRUE;
}

//設定圖層類別
void CLayerObject::SetKind(LONG Kind)
{
	m_LayerKind=Kind;
}

//釋放所有數據塊
bool CLayerObject::FreeAllDC()
{
	if(m_ForeMaskData)	
	{
		delete m_ForeMaskData;
		m_ForeMaskData=NULL;
	}

	if(m_TemplateBmpData)
	{
		delete m_TemplateBmpData;
		m_TemplateBmpData=NULL;
	}

	if(m_TemplateMaskData)	
	{
		delete m_TemplateMaskData;
		m_TemplateMaskData=NULL;
	}

	if(m_ForeData)	
	{
		delete m_ForeData;
		m_ForeData=NULL;
	}

	if(m_LayerBmpData)	
	{
		delete m_LayerBmpData;
		m_LayerBmpData=NULL;
	}

	m_rectLayer.SetRect(0,0,0,0);
	return	TRUE;
}

//建立模版圖像遮罩
bool CLayerObject::SetTemplateMask(LPBITMAPINFO pInfo, LPBYTE pData)
{
/////////	Creat	ForeDC //////////
	m_TemplateMaskInfo=*pInfo;

/////////	Creat LayerBmpDC ///////////
	if(m_TemplateMaskData)
	{
		delete	m_TemplateMaskData;
		m_TemplateMaskData=NULL;
	}
	
//	m_TemplateMaskData=new	BYTE[AdjustWidth(m_LayerWidth*3)*m_LayerHeight];

//	memcpy(m_TemplateMaskData, pData, 
//			AdjustWidth(m_LayerWidth*3)*m_LayerHeight);
	m_TemplateMaskData=pData;
	return	TRUE;
}

//建立前景圖像遮罩
bool CLayerObject::SetForeMask(LPBITMAPINFO pInfo, LPBYTE pData)
{
	m_LayerWidth=pInfo->bmiHeader.biWidth;
	m_LayerHeight=pInfo->bmiHeader.biHeight;

	m_ForeMaskInfo=*pInfo;

/////////	Creat LayerBmpDC ///////////
	if (m_ForeMaskData)
	{
		delete m_ForeMaskData;
		m_ForeMaskData = NULL;
	}

//	m_ForeMaskData=new BYTE[AdjustWidth(m_LayerWidth*3)*m_LayerHeight];
	
//	memcpy(m_ForeMaskData, pData, 
//			AdjustWidth(m_LayerWidth*3)*m_LayerHeight);
		m_ForeMaskData = pData;

///////////建立復合層數據
	
	if (m_LayerBmpData)
	{
		delete m_LayerBmpData;
		m_LayerBmpData = NULL;
	}
	m_LayerBmpData=new	BYTE[AdjustWidth(m_LayerWidth*3)*m_LayerHeight];
	m_LayerBmpInfo=m_ForeMaskInfo;
//////////////

/////////	Set size //////////
//	m_LayerScale=(double)m_LayerHeight/(double)m_LayerWidth;
	return	TRUE;
}