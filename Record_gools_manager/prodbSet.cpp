// prodbSet.cpp : implementation of the CProdbSet class
//

#include "stdafx.h"
#include "prodb.h"
#include "prodbSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProdbSet implementation

IMPLEMENT_DYNAMIC(CProdbSet, CRecordset)

CProdbSet::CProdbSet(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(CProdbSet)
	m_CID = _T("");
	m_CName = _T("");
	m_CPhone = _T("");
	m_CJumin = _T("");
	m_CAddress = _T("");
	m_CPoint = 0;
	m_DCode = _T("");
	m_DName = _T("");
	m_DSinger = _T("");
	m_DMake = _T("");
	m_DStock = 0;
	m_DPrice = 0;
	m_WHNumber = _T("");
	m_WHDate = 0;
	m_WHPrice = 0;
	m_WHAmount = 0;
	m_WH_DCode = _T("");
	m_S_CID = _T("");
	m_S_DCode = _T("");
	m_SDate = 0;
	m_SAmount = 0;
	m_nFields = 21;
	//}}AFX_FIELD_INIT
	m_nDefaultType = snapshot;
}

CString CProdbSet::GetDefaultConnect()
{
	return _T("ODBC;DSN=sql;UID=prodb;PWD=prodb");
}

CString CProdbSet::GetDefaultSQL()
{
	return _T("[PRODB].[고객],[PRODB].[음반],[PRODB].[입고음반],[PRODB].[판매]");
}

void CProdbSet::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(CProdbSet)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Text(pFX, _T("[PRODB].[고객].[고객ID]"), m_CID);
	RFX_Text(pFX, _T("[고객이름]"), m_CName);
	RFX_Text(pFX, _T("[전화번호]"), m_CPhone);
	RFX_Text(pFX, _T("[주민번호]"), m_CJumin);
	RFX_Text(pFX, _T("[주소]"), m_CAddress);
	RFX_Long(pFX, _T("[마일리지]"), m_CPoint);
	RFX_Text(pFX, _T("[PRODB].[음반].[음반코드]"), m_DCode);
	RFX_Text(pFX, _T("[음반이름]"), m_DName);
	RFX_Text(pFX, _T("[가수]"), m_DSinger);
	RFX_Text(pFX, _T("[제작사]"), m_DMake);
	RFX_Long(pFX, _T("[재고수량]"), m_DStock);
	RFX_Long(pFX, _T("[판매단가]"), m_DPrice);
	RFX_Text(pFX, _T("[입고번호]"), m_WHNumber);
	RFX_Date(pFX, _T("[입고일자]"), m_WHDate);
	RFX_Long(pFX, _T("[입고단가]"), m_WHPrice);
	RFX_Long(pFX, _T("[입고수량]"), m_WHAmount);
	RFX_Text(pFX, _T("[PRODB].[입고음반].[음반코드]"), m_WH_DCode);
	RFX_Text(pFX, _T("[PRODB].[판매].[고객ID]"), m_S_CID);
	RFX_Text(pFX, _T("[PRODB].[판매].[음반코드]"), m_S_DCode);
	RFX_Date(pFX, _T("[판매일]"), m_SDate);
	RFX_Long(pFX, _T("[수량]"), m_SAmount);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CProdbSet diagnostics

#ifdef _DEBUG
void CProdbSet::AssertValid() const
{
	CRecordset::AssertValid();
}

void CProdbSet::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG
