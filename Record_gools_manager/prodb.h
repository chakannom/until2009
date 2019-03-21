// prodb.h : main header file for the PRODB application
//

#if !defined(AFX_PRODB_H__66D5D9BA_F3F0_4B6F_B39B_390986AE7802__INCLUDED_)
#define AFX_PRODB_H__66D5D9BA_F3F0_4B6F_B39B_390986AE7802__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CProdbApp:
// See prodb.cpp for the implementation of this class
//

class CProdbApp : public CWinApp
{
public:
	CProdbApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProdbApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CProdbApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PRODB_H__66D5D9BA_F3F0_4B6F_B39B_390986AE7802__INCLUDED_)
