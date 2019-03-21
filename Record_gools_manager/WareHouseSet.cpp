// WareHouseSet.cpp : implementation file
//

#include "stdafx.h"
#include "prodb.h"
#include "WareHouseSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWareHouseSet

IMPLEMENT_DYNAMIC(CWareHouseSet, CRecordset)

CWareHouseSet::CWareHouseSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CWareHouseSet)
	m_WHNumber = _T("");
	m_WHDate = 0;
	m_WHPrice = 0;
	m_WHAmount = 0;
	m_WH_DCode = _T("");
	m_nFields = 5;
	//}}AFX_FIELD_INIT
	m_nDefaultType = dynaset;
}


CString CWareHouseSet::GetDefaultConnect()
{
	return _T("ODBC;DSN=sql;UID=prodb;PWD=prodb");
}

CString CWareHouseSet::GetDefaultSQL()
{
	return _T("[PRODB].[입고음반]");
}

void CWareHouseSet::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CWareHouseSet)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Text(pFX, _T("[입고번호]"), m_WHNumber);
	RFX_Date(pFX, _T("[입고일자]"), m_WHDate);
	RFX_Long(pFX, _T("[입고단가]"), m_WHPrice);
	RFX_Long(pFX, _T("[입고수량]"), m_WHAmount);
	RFX_Text(pFX, _T("[음반코드]"), m_WH_DCode);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CWareHouseSet diagnostics

#ifdef _DEBUG
void CWareHouseSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CWareHouseSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
