#if !defined(AFX_WHVIEW_H__0C076E4E_9E68_4FE8_B48C_97DAF364D788__INCLUDED_)
#define AFX_WHVIEW_H__0C076E4E_9E68_4FE8_B48C_97DAF364D788__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WhView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWhView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CWareHouseSet;
class CWhView : public CFormView
{
protected:
	DECLARE_DYNCREATE(CWhView)

// Form Data
public:
	CWhView();	           // protected constructor used by dynamic creation
	//{{AFX_DATA(CWhView)
	enum { IDD = IDD_LIST_WH };
	CListCtrl	m_listW;
	CWareHouseSet* m_wSet;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWhView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void Init();
	virtual ~CWhView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CWhView)
	afx_msg void OnNew();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WHVIEW_H__0C076E4E_9E68_4FE8_B48C_97DAF364D788__INCLUDED_)
