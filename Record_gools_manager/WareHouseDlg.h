#if !defined(AFX_WAREHOUSEDLG_H__211BE557_36E4_46FA_A384_73956B69B3F5__INCLUDED_)
#define AFX_WAREHOUSEDLG_H__211BE557_36E4_46FA_A384_73956B69B3F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "DiscSet.h"
#include "WareHouseSet.h"
// WareHouseDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWareHouseDlg dialog

class CWareHouseDlg : public CDialog
{
// Construction
public:
	CWareHouseDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CWareHouseDlg)
	enum { IDD = IDD_WAREHOUSE };
	CListCtrl	list_WHDlg;
	CString	m_strDMake;
	CString	m_strDName;
	long	m_longDPrice;
	CString	m_strDSinger;
	long	m_longWAmount;
	long	m_longWPrice;
	CString	m_strDCode;
	//}}AFX_DATA

public:
	CDiscSet* m_discSet;
	CWareHouseSet* m_warehouseSet;
	BOOL IsData_;
	CString m_strWNumber;
	CString CountDRecord();
	CString CountWRecord();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWareHouseDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWareHouseDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkListDisc(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeDiscName();
	afx_msg void OnChangeDiscSinger();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WAREHOUSEDLG_H__211BE557_36E4_46FA_A384_73956B69B3F5__INCLUDED_)
