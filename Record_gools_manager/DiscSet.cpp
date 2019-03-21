// DiscSet.cpp : implementation file
//

#include "stdafx.h"
#include "prodb.h"
#include "DiscSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDiscSet

IMPLEMENT_DYNAMIC(CDiscSet, CRecordset)

CDiscSet::CDiscSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CDiscSet)
	m_DCode = _T("");
	m_DName = _T("");
	m_DSinger = _T("");
	m_DMake = _T("");
	m_DStock = 0;
	m_DPrice = 0;
	m_nFields = 6;
	//}}AFX_FIELD_INIT
	m_nDefaultType = dynaset;
}


CString CDiscSet::GetDefaultConnect()
{
	return _T("ODBC;DSN=sql;UID=prodb;PWD=prodb");
}

CString CDiscSet::GetDefaultSQL()
{
	return _T("[PRODB].[����]");
}

void CDiscSet::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CDiscSet)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Text(pFX, _T("[�����ڵ�]"), m_DCode);
	RFX_Text(pFX, _T("[�����̸�]"), m_DName);
	RFX_Text(pFX, _T("[����]"), m_DSinger);
	RFX_Text(pFX, _T("[���ۻ�]"), m_DMake);
	RFX_Long(pFX, _T("[������]"), m_DStock);
	RFX_Long(pFX, _T("[�ǸŴܰ�]"), m_DPrice);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CDiscSet diagnostics

#ifdef _DEBUG
void CDiscSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CDiscSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
