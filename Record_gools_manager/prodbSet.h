// prodbSet.h : interface of the CProdbSet class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRODBSET_H__C298D442_7769_4CA7_8A97_FA94D751001D__INCLUDED_)
#define AFX_PRODBSET_H__C298D442_7769_4CA7_8A97_FA94D751001D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CProdbSet : public CRecordset
{
public:
	CProdbSet(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CProdbSet)

// Field/Param Data
	//{{AFX_FIELD(CProdbSet, CRecordset)
	CString	m_CID;
	CString	m_CName;
	CString	m_CPhone;
	CString	m_CJumin;
	CString m_CAddress;
	long	m_CPoint;
	CString	m_DCode;
	CString	m_DName;
	CString	m_DSinger;
	CString	m_DMake;
	long	m_DStock;
	long	m_DPrice;
	CString	m_WHNumber;
	CTime	m_WHDate;
	long	m_WHPrice;
	long	m_WHAmount;
	CString	m_WH_DCode;
	CString	m_S_CID;
	CString	m_S_DCode;
	CTime	m_SDate;
	long	m_SAmount;
	//}}AFX_FIELD

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProdbSet)
	public:
	virtual CString GetDefaultConnect();	// Default connection string
	virtual CString GetDefaultSQL(); 	// default SQL for Recordset
	virtual void DoFieldExchange(CFieldExchange* pFX);	// RFX support
	//}}AFX_VIRTUAL

// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PRODBSET_H__C298D442_7769_4CA7_8A97_FA94D751001D__INCLUDED_)

