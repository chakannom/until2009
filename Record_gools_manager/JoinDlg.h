#if !defined(AFX_JOINDLG_H__5F9659E5_7A10_41F2_A40E_33A63B6D8AB7__INCLUDED_)
#define AFX_JOINDLG_H__5F9659E5_7A10_41F2_A40E_33A63B6D8AB7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// JoinDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CJoinDlg dialog

class CJoinDlg : public CDialog
{
// Construction
public:
	CJoinDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CJoinDlg)
	enum { IDD = IDD_JOIN };
	CString	m_strCAddress;
	CString	m_strCID;
	CString	m_strCJumin;
	CString	m_strCName;
	CString	m_strCPhone;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJoinDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CJoinDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JOINDLG_H__5F9659E5_7A10_41F2_A40E_33A63B6D8AB7__INCLUDED_)
