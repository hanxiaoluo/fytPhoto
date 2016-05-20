
// FrameObject.cpp: implementation of the CFrameObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FrameObject.h"
//#include "ImageRW.h"
//#include "Scaling.h"
//#include "SO_CR2.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFrameObject::CFrameObject()
{
	m_pLayerObj = NULL;
	m_pPic		= NULL;
	m_pLayerBak = NULL;
	m_ImageData = NULL;
	m_ImageInfo = NULL;
	m_iObjectType = OBJECT_FRAME;
}

CFrameObject::~CFrameObject()
{
	Delete();
}

VOID CFrameObject::DrawLayer()
{
	MemBitBlt(m_pPic,
			  m_FrameWidth,
			  m_FrameHeight,
			  0,
			  0,
			  m_FrameWidth,
			  m_FrameHeight,
			  m_ImageData,
			  m_FrameWidth,
			  m_FrameHeight,
			  0,
			  0);

//	memcpy(m_pPic,m_ImageData,AdjustWidth(m_FrameWidth*3)*m_FrameHeight);
	m_pLayerObj->CopyToBack(m_pPic,m_ImageInfo,0,0);
}

VOID CFrameObject::Draw()
{
	m_pObjInfo = (LPBITMAPINFO)m_ImageInfo;
	m_pObjData = m_pPic ;

}

BOOL CFrameObject::UpdateImage()
{

/*	//// copy image data ////
	LPBITMAPINFOHEADER pInfo = NULL;
	LPBYTE pData = NULL;
	pInfo = (LPBITMAPINFOHEADER)m_pCurData;
	pData = (LPBYTE)(m_pCurData + sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD));

	CScaling* pScaling = NULL;
	pScaling = new CScaling();
	CSize szTemp( pInfo->biWidth,pInfo->biHeight);
	INT dwSize = pScaling->GetScaledImgSize(pInfo,szTemp);
	delete pScaling;

		if(m_ImageInfo)
		{
			delete m_ImageInfo ;
			m_ImageInfo = NULL ;
		}

	m_ImageInfo =(LPBITMAPINFO)new BYTE[sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD)];
	m_ImageData = new BYTE[dwSize];
	memcpy(m_ImageInfo, pInfo,sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD));
	memcpy(m_ImageData, pData,dwSize);

	m_FrameHeight = m_ImageInfo->bmiHeader.biHeight ;
	m_FrameWidth  = m_ImageInfo->bmiHeader.biWidth  ;

	if(m_ImageInfo->bmiHeader.biBitCount < 24 )
		ImageLib::ConvertToRGB(m_ImageInfo,m_ImageData);

	Resize(m_FrameWidth,m_FrameHeight);
	DrawLayer();

	Draw();*/

	return true;
}

BOOL CFrameObject::Read(LPCTSTR PfmFile)
{
	CFile file;
	if(!file.Open(PfmFile, CFile::modeRead))
		return FALSE;
/*
	TCHAR strHead[5];
	file.Read(strHead,5);
	if (::strncmp(strHead, _TEXT("Frame"), 5) != 0)
	{
		file.Close();
//		ErrorMessageBox("Read Frame file format error!");
		return FALSE;	// file format error
	}

	LONG	len;
	////////  read apf file  ///////
	TCHAR  path[_MAX_PATH];
	CString cPathName;
	GetTempPath(_MAX_PATH,path);
	cPathName=path ;
	cPathName+="Frame.Tmp";

	CFile	tmpFile;
	Delete(); /////// Release Frame

	file.Read(&m_OldWidth ,sizeof(DWORD));
	file.Read(&m_OldHeight,sizeof(DWORD));

	file.Read(&len,sizeof(DWORD));
	file.Seek(len,CFile::current);
*/
	FAMILYHEADER	head ;
	
	file.Read(&head,sizeof(FAMILYHEADER));
	if (head.iObjectType != OBJECT_FRAME)
	{
		file.Close();
//		ErrorMessageBox("Read Frame file format error!");
		return FALSE;	// file format error
	}

	LONG	len;
	////////  read apf file  ///////
	TCHAR  path[_MAX_PATH];
	CString cPathName;
	GetTempPath(_MAX_PATH,path);
	cPathName=path ;
	cPathName+="Frame.Tmp";

	CFile	tmpFile;
	Delete(); /////// Release Frame

	m_OldWidth = head.dwWidth ;
	m_OldHeight= head.dwHeight;

	file.Seek(head.dwThumbnailSize,CFile::current);

	/////////// set layer /////////	
	m_pLayerBak = new CLayerObject ;
	m_pLayerObj = new CLayerObject ;

	file.Read(&len,sizeof(DWORD));
	LPBYTE tmpData = new BYTE[len];
	file.Read(tmpData,len);

	tmpFile.Open(cPathName,CFile::modeWrite | CFile::modeCreate | CFile::modeNoTruncate);
	tmpFile.Write(tmpData,len);
	tmpFile.Close();
	delete	tmpData;
	OpenFile2(cPathName,m_pLayerBak,3);

		/////* set fore///
	file.Read(&len,sizeof(DWORD));
	tmpData = new BYTE[len];
	file.Read(tmpData,len);

	tmpFile.Open(cPathName,CFile::modeWrite | CFile::modeCreate | CFile::modeNoTruncate);
	tmpFile.Write(tmpData,len);
	tmpFile.Close();
	delete	tmpData;
	OpenFile2(cPathName,m_pLayerBak,4);

	file.Close();
	DeleteFile(cPathName);

	m_pLayerBak->CopyToFore(0,0);
	m_pLayerBak->SetKind(LAYER_SINGLE);
	
	UpdateImage();

	return TRUE;
}

BOOL CFrameObject::OpenFile2(LPCTSTR lpszPathName, CLayerObject *layer,LONG type)
{
/*	CImageRW *pImageRead = new CImageRW;

	LPBITMAPINFO pInfo;
	LPBYTE pData;
	DWORD dwImageSize = pImageRead->GetImageSize(lpszPathName); 

	if(dwImageSize == 0)
	{
//		ErrorMessageBox("Error in reading the image file.");
		delete pImageRead;
		return FALSE;
	}
	 pInfo = (LPBITMAPINFO) new char[40+4*256];
	 pData = new BYTE [dwImageSize]; 
	 
//get the pointers of image file's BITMAPINFO and data
	 
	if(FALSE == pImageRead->ReadImage(lpszPathName,pInfo,pData))
	{
//		ErrorMessageBox("Error in reading the image file.");
		delete pImageRead;
		return FALSE;
	}

///////////// set clayer from type /////////////

	switch(type)
	{
	case	1:
			layer->SetTemplate(pInfo,pData);
			break;
	case	2:
			layer->SetTemplateMask(pInfo,pData);
			break;
	case	3:
			layer->SetForeMask(pInfo,pData);
			break;
	case	4:
			layer->SetFore(pInfo,pData);
			break;
	}

	delete pImageRead;	
	delete	pInfo;//	delete pData;
	*/
	return true;
}

VOID CFrameObject::Delete()
{
	if(m_pLayerObj)
		m_pLayerObj->FreeAllDC();

	if(m_pLayerBak)
		m_pLayerBak->FreeAllDC();
	if(m_pPic)
	{
		delete m_pPic ;
		m_pPic = NULL ;
	}
	if(m_ImageInfo)
	{
		delete m_ImageInfo ;
		m_ImageInfo = NULL ;
	}
	if(m_ImageData)
	{
		delete m_ImageData ;
		m_ImageData = NULL ;
	}
	delete m_pLayerBak ;
	delete m_pLayerObj ;
}

VOID CFrameObject::Resize(LONG width , LONG height)
{
//	m_Frame.FreeAllDC();
	BITMAPINFO	 TempInfo;
	LPBYTE		 TempData;

	m_FrameWidth = width ;
	m_FrameHeight= height;

	////// set Temp data /////////
	TempInfo =m_pLayerBak->m_ForeMaskInfo;
	TempInfo.bmiHeader.biWidth =m_FrameWidth;
	TempInfo.bmiHeader.biHeight=m_FrameHeight;
	TempData=new BYTE[AdjustWidth(m_FrameWidth*3)*m_FrameHeight];
	///////////// set Temp END //////////////

	////////////  change foremask ///////////
	MemStretchBlt(TempData,
				  m_FrameWidth,
				  m_FrameHeight,
				  m_pLayerBak->m_ForeMaskData,
				  m_OldWidth,
				  m_OldHeight);

	m_pLayerObj->SetForeMask(&TempInfo,TempData);

	//////// change fore //////////
	TempData = NULL ;
	TempData=new BYTE[AdjustWidth(m_FrameWidth*3)*m_FrameHeight];

	MemStretchBlt(TempData,
				  m_FrameWidth,
				  m_FrameHeight,
				  m_pLayerBak->m_ForeData,
				  m_OldWidth,
				  m_OldHeight);
	LPBITMAPINFO pTempBmpInfo = (LPBITMAPINFO)(&TempInfo);
	m_pLayerObj->SetFore((LPBITMAPINFO&)pTempBmpInfo,TempData);
	m_pLayerObj->CopyToFore(0,0);
	m_pLayerObj->SetKind(LAYER_SINGLE);
	if(m_pPic)	
	{
		delete m_pPic ;
		m_pPic = NULL ;
	}

	m_pPic = new BYTE[AdjustWidth(m_FrameWidth*3)*m_FrameHeight];
//	delete	TempData;
}


VOID CFrameObject::MoveForeImage(CSize /*size*/) 
{
}

VOID CFrameObject::GetForeImageRect(CRect& /*rect*/)
{
}
