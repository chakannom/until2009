// JoinSet.cpp : implementation file
//

#include "stdafx.h"
#include "prodb.h"
#include "JoinSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CJoinSet

IMPLEMENT_DYNAMIC(CJoinSet, CRecordset)

CJoinSet::CJoinSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CJoinSet)
	m_CID = _T("");
	m_CName = _T("");
	m_CPhone = _T("");
	m_CJumin = _T("");
	m_CAddress = _T("");
	m_CPoint = 0;
	m_nFields = 6;
	//}}AFX_FIELD_INIT
	m_nDefaultType = dynaset;
}


CString CJoinSet::GetDefaultConnect()
{
	return _T("ODBC;DSN=sql;UID=prodb;PWD=prodb");
}

CString CJoinSet::GetDefaultSQL()
{
	return _T("[PRODB].[��]");
}

void CJoinSet::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CJoinSet)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Text(pFX, _T("[��ID]"), m_CID);
	RFX_Text(pFX, _T("[���̸�]"), m_CName);
	RFX_Text(pFX, _T("[��ȭ��ȣ]"), m_CPhone);
	RFX_Text(pFX, _T("[�ֹι�ȣ]"), m_CJumin);
	RFX_Text(pFX, _T("[�ּ�]"), m_CAddress);
	RFX_Long(pFX, _T("[���ϸ���]"), m_CPoint);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CJoinSet diagnostics

#ifdef _DEBUG
void CJoinSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CJoinSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
