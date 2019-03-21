// SellSet.cpp : implementation file
//

#include "stdafx.h"
#include "prodb.h"
#include "SellSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSellSet

IMPLEMENT_DYNAMIC(CSellSet, CRecordset)

CSellSet::CSellSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CSellSet)
	m_S_CID=_T("");
	m_S_DCode=_T("");
	m_SAmount=0;
	m_nFields = 4;
	//}}AFX_FIELD_INIT
	m_nDefaultType = dynaset;
}


CString CSellSet::GetDefaultConnect()
{
	return _T("ODBC;DSN=sql;UID=prodb;PWD=prodb");
}

CString CSellSet::GetDefaultSQL()
{
	return _T("[PRODB].[�Ǹ�]");
}

void CSellSet::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CSellSet)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Text(pFX, _T("[��ID]"), m_S_CID);
	RFX_Text(pFX, _T("[�����ڵ�]"), m_S_DCode);
	RFX_Date(pFX, _T("[�Ǹ���]"), m_SDate);
	RFX_Long(pFX, _T("[����]"), m_SAmount);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CSellSet diagnostics

#ifdef _DEBUG
void CSellSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CSellSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
