#if !defined(AFX_SELLVIEW_H__B5DF93D7_F572_4A45_A218_B5E81B5CBD8C__INCLUDED_)
#define AFX_SELLVIEW_H__B5DF93D7_F572_4A45_A218_B5E81B5CBD8C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SellView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSellView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CSellSet;
class CSellView : public CFormView
{
protected:
	DECLARE_DYNCREATE(CSellView)

// Form Data
public:
	CSellView();           // protected constructor used by dynamic creation
	//{{AFX_DATA(CSellView)
	enum { IDD = IDD_LIST_SELL };
	CListCtrl	m_listC;
	CSellSet* m_sSet;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSellView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void Init();
	virtual ~CSellView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CSellView)
	afx_msg void OnNew();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELLVIEW_H__B5DF93D7_F572_4A45_A218_B5E81B5CBD8C__INCLUDED_)
