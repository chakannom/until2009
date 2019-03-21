#if !defined(AFX_DISCSET_H__583DEEE2_53BF_4ACC_A5BA_06323AB0F73D__INCLUDED_)
#define AFX_DISCSET_H__583DEEE2_53BF_4ACC_A5BA_06323AB0F73D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DiscSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDiscSet recordset

class CDiscSet : public CRecordset
{
public:
	CDiscSet(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CDiscSet)

// Field/Param Data
	//{{AFX_FIELD(CDiscSet, CRecordset)
	CString	m_DCode;
	CString	m_DName;
	CString	m_DSinger;
	CString	m_DMake;
	long	m_DStock;
	long	m_DPrice;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDiscSet)
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

#endif // !defined(AFX_DISCSET_H__583DEEE2_53BF_4ACC_A5BA_06323AB0F73D__INCLUDED_)
