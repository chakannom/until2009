// prodbView.h : interface of the CProdbView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRODBVIEW_H__3FA79603_3DFB_4AE7_88B6_9B6C9C252654__INCLUDED_)
#define AFX_PRODBVIEW_H__3FA79603_3DFB_4AE7_88B6_9B6C9C252654__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CProdbSet;
class CJoinSet;
class CDiscSet;

class CProdbView : public CRecordView
{
protected: // create from serialization only

	DECLARE_DYNCREATE(CProdbView)

public:
	CProdbView();
	//{{AFX_DATA(CProdbView)
	enum { IDD = IDD_PRODB_FORM };
	CListCtrl	m_listCustomer;
	CListCtrl	m_listDisc;
	CProdbSet* m_pSet;
	CJoinSet* m_jSet;
	CDiscSet* m_dSet;
	CString	m_strCID;
	CString	m_strCName;
	CString	m_strCPhone;
	long	m_longDAmount;
	CString	m_strDCode;
	CString	m_strDName;
	CString	m_strCPoint;
	long	m_longCStock;
	//}}AFX_DATA

// Attributes
public:
	CProdbDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProdbView)
	public:
	virtual CRecordset* OnGetRecordset();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	void InitList();
	virtual ~CProdbView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CProdbView)
	afx_msg void OnChangeViewCid();
	afx_msg void OnChangeViewCphone();
	afx_msg void OnChangeViewDname();
	afx_msg void OnChangeViewDcode();
	afx_msg void OnDblclkListCustomer(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListDisc(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSellBn();
	afx_msg void OnInit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in prodbView.cpp
inline CProdbDoc* CProdbView::GetDocument()
   { return (CProdbDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PRODBVIEW_H__3FA79603_3DFB_4AE7_88B6_9B6C9C252654__INCLUDED_)
