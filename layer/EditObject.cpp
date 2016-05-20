
#include "stdafx.h"
#include "EditObject.h"
//#include "ImageRW.h"
//#include "ErrorMessageBox.h"
//#include "MessageDataPH.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//CImageObject
CImageObject::CImageObject()
{
	m_pSrcData = NULL;
	m_pCurData = NULL;
	m_pObjData = NULL;
	m_pObjInfo = NULL;
	m_nScalInd = 6;			// dScaling[6] = 1.0
	m_iObjectType = OBJECT_NONE;
	m_bImgModified = FALSE;
}

CImageObject::~CImageObject()
{
//	DeleteUndoList();
//	DeleteRedoList();

	if((m_pCurData) && (m_pCurData != m_pSrcData))
	{
		delete m_pCurData;
		m_pCurData = NULL;
	}
	if(m_pSrcData)
	{
		delete m_pSrcData;
		m_pSrcData = NULL;
	}
}

OBJECTTYPE CImageObject::GetObjectType()
{
	return m_iObjectType;
}

BOOL CImageObject::SetSrcImgData(LPBYTE pData)
{
	if(!pData)
		return FALSE;

	//Get image data size
	LPBITMAPINFOHEADER lpInfoHeader = (LPBITMAPINFOHEADER)pData;
	LONG lWidth = lpInfoHeader->biWidth;
	LONG lHeigh = lpInfoHeader->biHeight;
	LONG lSrcSize = 0;
	switch(lpInfoHeader->biBitCount)
	{
	case 1:
		lSrcSize = (((lWidth+31)/32)*4) * lHeigh;
		break;
	case 2:
		lSrcSize = (((lWidth+15)/16)*4) * lHeigh;
		break;
	case 4:
		lSrcSize = (((lWidth+7)/8)*4) * lHeigh;
		break;
	case 8:
		lSrcSize = (((lWidth+3)/4)*4) * lHeigh;
		break;
	case 16:
		lSrcSize = (((lWidth*2+3)/4)*4) * lHeigh;
		break;
	case 24:
		lSrcSize = (((lWidth*3+3)/4)*4) * lHeigh;
		break;
	case 32:
		lSrcSize = lWidth * 4 * lHeigh;
		break;
	case 48:
		lSrcSize = (((lWidth*6+3)/4)*4) * lHeigh;
		break;
	default:
		return FALSE;
	}
	lSrcSize = lSrcSize + sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD);

	//Source data locate
	LPBYTE lpTempData = NULL;
	try
	{
		lpTempData = new BYTE[lSrcSize];
	}
	catch(CMemoryException *e)
	{
		//::ErrorMessageBox(MP_PHOTOFAMILY_OFM, MB_OK);
		e->Delete();
		return FALSE;
	}
	memcpy(lpTempData, pData, lSrcSize);

	//Data initial
//	DeleteUndoList();
//	DeleteRedoList();

	if((m_pCurData) && (m_pCurData != m_pSrcData))
	{
		delete m_pCurData;
		m_pCurData = NULL;
	}
	if(m_pSrcData)
	{
		delete m_pSrcData;
		m_pSrcData = NULL;
	}

	//Get data pointer
	m_pSrcData = lpTempData;
	m_pCurData = lpTempData;

	return TRUE;
}

BOOL CImageObject::SetNewImgData(LPBYTE pData)
{
	if(!pData)
		return FALSE;

	//Get image data size
	LPBITMAPINFOHEADER lpInfoHeader = (LPBITMAPINFOHEADER)pData;
	LONG lWidth = lpInfoHeader->biWidth;
	LONG lHeigh = lpInfoHeader->biHeight;
	LONG lSrcSize = 0;
	switch(lpInfoHeader->biBitCount)
	{
	case 1:
		lSrcSize = (((lWidth+31)/32)*4) * lHeigh;
		break;
	case 2:
		lSrcSize = (((lWidth+15)/16)*4) * lHeigh;
		break;
	case 4:
		lSrcSize = (((lWidth+7)/8)*4) * lHeigh;
		break;
	case 8:
		lSrcSize = (((lWidth+3)/4)*4) * lHeigh;
		break;
	case 16:
		lSrcSize = (((lWidth*2+3)/4)*4) * lHeigh;
		break;
	case 24:
		lSrcSize = (((lWidth*3+3)/4)*4) * lHeigh;
		break;
	case 32:
		lSrcSize = lWidth * 4 * lHeigh;
		break;
	case 48:
		lSrcSize = (((lWidth*6+3)/4)*4) * lHeigh;
		break;
	default:
		return FALSE;
	}
	lSrcSize = lSrcSize + sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD);

	//Source data locate
	LPBYTE lpTempData = NULL;
	try
	{
		lpTempData = new BYTE[lSrcSize];
	}
	catch(CMemoryException *e)
	{
		//::ErrorMessageBox(MP_PHOTOFAMILY_OFM, MB_OK);
		e->Delete();
		return FALSE;
	}
	memcpy(lpTempData, pData, lSrcSize);

	if((m_pCurData) && (m_pCurData != m_pSrcData))
	{
		delete m_pCurData;
		m_pCurData = NULL;
	}

	m_pCurData = lpTempData;

	return TRUE;

/*  // these codes are with undo/redo condition
	if(pData == NULL)
		return FALSE;

//	if(m_pUndo.GetCount() >= EDITUNDOMAXCOUNT)
//		DeleteUndoHead();

//	m_pUndo.AddTail(m_pCurData);
	m_pCurData = pData;
//	DeleteRedoList();
	m_bImgModified = TRUE;
	return TRUE;*/
}

LPBYTE CImageObject::GetSrcImgData()
{
	return m_pSrcData;
}

LPBYTE CImageObject::GetCurImgData()
{
	return m_pCurData;
}

BOOL CImageObject::Save(LPCTSTR szFile)
{
	/*CImageRW * pImgRW = NULL;
	try
	{
		pImgRW = new CImageRW;
	}
	catch(CMemoryException* e)
	{
		TRACE0("CImageObject::Write : save image file error!\n");
		e->Delete();
		return FALSE;
	}

	BOOL bSucceed = FALSE;
	bSucceed = pImgRW->WriteImage(szFile,m_pObjInfo,m_pObjData);
	if(bSucceed == FALSE)
	{
		TCHAR buf[MAX_PATH];
		MutiLang.trans2local(MP_IOJ_1, buf, MAX_PATH);
		CString str = buf;
		sprintf(buf,str,szFile);
		//::ErrorMessageBox(buf,MB_OK);
		delete pImgRW;
		pImgRW = NULL;
		return FALSE;
	}

	delete pImgRW;
	pImgRW = NULL;

	m_bImgModified = FALSE;*/
	return TRUE;
}

VOID CImageObject::SaveAs(LPCTSTR szFile)
{
	/*CImageRW * pImgRW = NULL;
	try
	{
		pImgRW = new CImageRW;
	}
	catch(CMemoryException* e)
	{
		TRACE0("CImageObject::Write : save image file error!\n");
		e->Delete();
		return ;
	}

	BOOL bSucceed = FALSE;
	bSucceed = pImgRW->WriteImage(szFile,m_pObjInfo,m_pObjData);
	if(bSucceed == FALSE)
	{
		TCHAR buf[MAX_PATH];
		MutiLang.trans2local(MP_IOJ_1, buf, MAX_PATH);
		CString str = buf;
		sprintf(buf,str,szFile);
		//::ErrorMessageBox(buf,MB_OK);
	}

	delete pImgRW;
	pImgRW = NULL;*/
}

BOOL CImageObject::GetModifiedFlag()
{
	return m_bImgModified;
}

VOID CImageObject::GetObjectData(LPBYTE& pObjData, LPBITMAPINFO& pObjInfo)
{
	pObjData = m_pObjData;
	pObjInfo = m_pObjInfo;
}

/*
BOOL CImageObject::Undo()
{
	INT nCount = m_pUndo.GetCount();
	if( (m_pCurData) && (nCount > 0) )
	{
		m_pRedo.AddTail(m_pCurData);
		m_pCurData = (LPBYTE)m_pUndo.GetTail();
		m_pUndo.RemoveTail();
		return TRUE;
	}
	else
		return FALSE;
}

BOOL CImageObject::Redo()
{
	INT nCount = m_pRedo.GetCount();
	if( (m_pCurData) && (nCount > 0) )
	{
		if(m_pUndo.GetCount() >= EDITUNDOMAXCOUNT)
			DeleteUndoHead();

		m_pUndo.AddTail(m_pCurData);
		m_pCurData = (LPBYTE)m_pRedo.GetTail();
		m_pRedo.RemoveTail();
		return TRUE;
	}
	else
		return FALSE;
}

BOOL CImageObject::GetUndoFlag()
{
	INT nCount = m_pUndo.GetCount();
	if(nCount == 0)
		return FALSE;
	else
		return TRUE;
}

BOOL CImageObject::GetRedoFlag()
{
	INT nCount = m_pRedo.GetCount();
	if(nCount == 0)
		return FALSE;
	else
		return TRUE;
}

VOID CImageObject::DeleteUndoHead()
{
	POSITION pos = m_pUndo.GetHeadPosition();
	if (pos != NULL)
	{
		LPBYTE pTemp = (LPBYTE)m_pUndo.GetNext(pos);
		if((pTemp) && (pTemp != m_pSrcData))
		{
			delete pTemp;
			pTemp = NULL;
		}
	}
	m_pUndo.RemoveHead();
}

VOID CImageObject::DeleteUndoList()
{
	POSITION pos = m_pUndo.GetHeadPosition();
	while (pos != NULL)
	{
		LPBYTE pTemp = (LPBYTE)m_pUndo.GetNext(pos);
		if((pTemp) && (pTemp != m_pSrcData))
		{
			delete pTemp;
			pTemp = NULL;
		}
	}
	m_pUndo.RemoveAll();
}

VOID CImageObject::DeleteRedoList()
{
	POSITION pos = m_pRedo.GetHeadPosition();
	while (pos != NULL)
	{
		LPBYTE pTemp = (LPBYTE)m_pRedo.GetNext(pos);
		if(pTemp)
		{
			delete pTemp;
			pTemp = NULL;
		}
	}
	m_pRedo.RemoveAll();
}
*/

// CAlgImgObject
CAlgImgObject::CAlgImgObject()
{
	m_iObjectType = OBJECT_ALGIMG;
}

CAlgImgObject::~CAlgImgObject()
{

}

BOOL CAlgImgObject::Read(LPCTSTR PfmFile)
{
	UNREFERENCED_PARAMETER(PfmFile);
	return TRUE;
}

VOID CAlgImgObject::Delete()
{
}

BOOL CAlgImgObject::UpdateImage()
{
	Draw();

	return TRUE;
}

VOID CAlgImgObject::Draw()
{
	m_pObjInfo = (LPBITMAPINFO)m_pCurData;
	m_pObjData = (LPBYTE)(m_pCurData + sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD));
}


VOID CAlgImgObject::MoveForeImage(CSize /*size*/) 
{
}

VOID CAlgImgObject::GetForeImageRect(CRect& /*rect*/)
{
}