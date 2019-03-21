#if !defined(AFX_WAREHOUSESET_H__86D44A29_8BA3_47D1_8CC1_CC6314CF5D64__INCLUDED_)
#define AFX_WAREHOUSESET_H__86D44A29_8BA3_47D1_8CC1_CC6314CF5D64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WareHouseSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWareHouseSet recordset

class CWareHouseSet : public CRecordset
{
public:
	CWareHouseSet(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CWareHouseSet)

// Field/Param Data
	//{{AFX_FIELD(CWareHouseSet, CRecordset)
	CString	m_WHNumber;
	CTime	m_WHDate;
	long	m_WHPrice;
	long	m_WHAmount;
	CString	m_WH_DCode;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWareHouseSet)
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

#endif // !defined(AFX_WAREHOUSESET_H__86D44A29_8BA3_47D1_8CC1_CC6314CF5D64__INCLUDED_)
