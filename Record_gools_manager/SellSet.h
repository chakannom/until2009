#if !defined(AFX_SELLSET_H__FBE5B9AB_80C9_421A_9714_544C4C700CC9__INCLUDED_)
#define AFX_SELLSET_H__FBE5B9AB_80C9_421A_9714_544C4C700CC9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SellSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSellSet recordset

class CSellSet : public CRecordset
{
public:
	CSellSet(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CSellSet)

// Field/Param Data
	//{{AFX_FIELD(CSellSet, CRecordset)
	CString	m_S_CID;
	CString	m_S_DCode;
	CTime	m_SDate;
	long	m_SAmount;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSellSet)
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

#endif // !defined(AFX_SELLSET_H__FBE5B9AB_80C9_421A_9714_544C4C700CC9__INCLUDED_)
