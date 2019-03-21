// prodbDoc.h : interface of the CProdbDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRODBDOC_H__311C3043_1ED1_4469_91C4_89C87E95BA06__INCLUDED_)
#define AFX_PRODBDOC_H__311C3043_1ED1_4469_91C4_89C87E95BA06__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "prodbSet.h"
#include "JoinSet.h"
#include "DiscSet.h"
#include "SellSet.h"
#include "WareHouseSet.h"

class CProdbDoc : public CDocument
{
protected: // create from serialization only
	CProdbDoc();
	DECLARE_DYNCREATE(CProdbDoc)

// Attributes
public:
	CProdbSet m_prodbSet;
	CJoinSet m_joinSet;
	CDiscSet m_discSet;
	CSellSet m_sellSet;
	CWareHouseSet m_warehouseSet;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProdbDoc)
	public:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	void OnSellingAdd();
	virtual ~CProdbDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CProdbDoc)
	afx_msg void OnJoin();
	afx_msg void OnWarehouse();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PRODBDOC_H__311C3043_1ED1_4469_91C4_89C87E95BA06__INCLUDED_)
