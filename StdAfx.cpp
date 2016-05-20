// stdafx.cpp : source file that includes just the standard includes
//	fytPhoto.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

CRect CalcRect(const int nWidth, const int nHeight, CRect* pDestRect)
{
	//long nWidth=abs(pInfo->bmiHeader.biWidth);
	//long nHeight = abs(pInfo->bmiHeader.biHeight);
	long newWidth = nWidth;
	long newHeight = nHeight;

	long nWndWidth = pDestRect->right-pDestRect->left;
	long nWndHeight = pDestRect->bottom - pDestRect->top;
	int nL,nT;
	if(nWidth > nWndWidth)
	{
		float radio = (float)nWidth /(float)nWndWidth;
		newWidth = nWndWidth;
		newHeight = (long)((float)nHeight/radio);
		nL = 0;
	}
	else
	{
		nL = (nWndWidth-nWidth)/2;
	}

	if(newHeight > nWndHeight)
	{
		float radio = (float)newHeight /(float)nWndHeight;
		newHeight = nWndHeight;
		newWidth = (long)((float)newWidth/radio);
		nT = 0;
		nL = (nWndWidth-newWidth)/2;
	}
	else
	{
		nT = (nWndHeight-newHeight)/2;
	}

	int m_nL =  pDestRect->left+nL;
	int m_nT = pDestRect->top+nT;
	int m_nR = pDestRect->left+nL+newWidth;
	int m_nB = pDestRect->top+nT+newHeight;
	CRect newRT(m_nL, m_nT, m_nR, m_nB);
	return newRT;
}