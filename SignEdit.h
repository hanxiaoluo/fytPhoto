/********************************************************************

	SignEdit.h - ISeeͼ���������ͼ����ģ����SignEditͷ�ļ�

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
#if !defined(AFX_SIGNEDIT_H__E42C869B_8178_452E_BA85_51850C709A49__INCLUDED_)
#define AFX_SIGNEDIT_H__E42C869B_8178_452E_BA85_51850C709A49__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SignEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSignEdit window

class CSignEdit : public CEdit
{
// Construction
public:
	CSignEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSignEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSignEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSignEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIGNEDIT_H__E42C869B_8178_452E_BA85_51850C709A49__INCLUDED_)
