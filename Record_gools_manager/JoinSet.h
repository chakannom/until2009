#if !defined(AFX_JOINSET_H__10215AD7_410F_4FBA_9628_2151D34C8DE5__INCLUDED_)
#define AFX_JOINSET_H__10215AD7_410F_4FBA_9628_2151D34C8DE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// JoinSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CJoinSet recordset

class CJoinSet : public CRecordset
{
public:
	CJoinSet(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CJoinSet)

// Field/Param Data
	//{{AFX_FIELD(CJoinSet, CRecordset)
	CString	m_CID;
	CString	m_CName;
	CString	m_CPhone;
	CString	m_CJumin;
	CString m_CAddress;
	long	m_CPoint;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJoinSet)
	public:
	virtual CString GetDefaultConnect();    // Default connection string
	virtual CString GetDefaultSQL();    // Default SQL for Recordset
	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX support
	//}}AFX_VIRTUAL

// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JOINSET_H__10215AD7_410F_4FBA_9628_2151D34C8DE5__INCLUDED_)
