/********************************************************************

	SignEdit.cpp - ISeeͼ���������ͼ����ģ����SignEditʵ�ִ����ļ�

    ��Ȩ����(C) VCHelp-coPathway-ISee workgroup 2000 all member's

    ��һ����������������������������������������GNU ͨ�����֤
	�������޸ĺ����·�����һ���򡣻��������֤�ĵڶ��棬���ߣ�������
	��ѡ�����κθ��µİ汾��

    ������һ�����Ŀ����ϣ�������ã���û���κε���������û���ʺ��ض�
	Ŀ�ص������ĵ���������ϸ����������GNUͨ�����֤��

    ��Ӧ���Ѿ��ͳ���һ���յ�һ��GNUͨ�����֤(GPL)�ĸ����������û�У�
	д�Ÿ���
    The Free Software Foundation, Inc.,  675  Mass Ave,  Cambridge,
    MA02139,  USA

	�������ʹ�ñ����ʱ��ʲô������飬�����µ�ַ����������ȡ����
	ϵ��
		http://isee.126.com
		http://www.vchelp.net
	��
		iseesoft@china.com

	���ߣ�����
   e-mail:hugesoft@yeah.net

   ����ʵ�֣��ɲ��������ݵ������༭��

	�ļ��汾��
		Build xxxxx
		Date  2000-11-18

********************************************************************/
// SignEdit.cpp : implementation file
//

#include "stdafx.h"
#include "SignEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSignEdit

CSignEdit::CSignEdit()
{
}

CSignEdit::~CSignEdit()
{
}


BEGIN_MESSAGE_MAP(CSignEdit, CEdit)
	//{{AFX_MSG_MAP(CSignEdit)
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSignEdit message handlers

void CSignEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	if((nChar<0x20) || (nChar>='0') && (nChar<='9'))
	{
		CEdit::OnChar(nChar, nRepCnt, nFlags);
	}
	else
	{
		::MessageBeep(0xFFFFFFFF);
	}
}
