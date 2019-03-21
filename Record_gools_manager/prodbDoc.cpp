// prodbDoc.cpp : implementation of the CProdbDoc class
//

#include "stdafx.h"
#include "prodb.h"

#include "prodbSet.h"
#include "prodbDoc.h"
#include "JoinDlg.h"
#include "JoinSet.h"
#include "DiscSet.h"
#include "WareHouseDlg.h"
#include "WareHouseSet.h"
#include "prodbView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProdbDoc

IMPLEMENT_DYNCREATE(CProdbDoc, CDocument)

BEGIN_MESSAGE_MAP(CProdbDoc, CDocument)
	//{{AFX_MSG_MAP(CProdbDoc)
	ON_COMMAND(ID_JOIN, OnJoin)
	ON_COMMAND(ID_WAREHOUSE, OnWarehouse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProdbDoc construction/destruction

CProdbDoc::CProdbDoc()
{
	// TODO: add one-time construction code here
}

CProdbDoc::~CProdbDoc()
{
}

BOOL CProdbDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CProdbDoc diagnostics

#ifdef _DEBUG
void CProdbDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CProdbDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CProdbDoc commands

void CProdbDoc::OnJoin() 
{
	// TODO: Add your command handler code here
	CJoinDlg dlg;
	if(dlg.DoModal()!=IDOK)	return;

	if(!m_joinSet.IsOpen()) m_joinSet.Open();
	m_joinSet.AddNew();
	m_joinSet.m_CID=_T(dlg.m_strCID);
	m_joinSet.m_CName=_T(dlg.m_strCName);
	m_joinSet.m_CName=_T(dlg.m_strCName);
	m_joinSet.m_CPhone=_T(dlg.m_strCPhone);
	m_joinSet.m_CJumin=_T(dlg.m_strCJumin);
	m_joinSet.m_CAddress=_T(dlg.m_strCAddress);
	m_joinSet.m_CPoint=0;
	m_joinSet.Update();
	
	AfxMessageBox(dlg.m_strCName+"님이 등록되었습니다.");
}

void CProdbDoc::OnWarehouse() 
{
	// TODO: Add your command handler code here
	CWareHouseDlg dlg;
	if(dlg.DoModal()!=IDOK) return;

	if(!dlg.IsData_)	{
		if(!m_discSet.IsOpen()) m_discSet.Open();
		m_discSet.AddNew();
		m_discSet.m_DCode=_T(dlg.m_strDCode);
		m_discSet.m_DName=_T(dlg.m_strDName);
		m_discSet.m_DSinger=_T(dlg.m_strDSinger);
		m_discSet.m_DMake=_T(dlg.m_strDMake);
		m_discSet.m_DPrice=dlg.m_longDPrice;
		m_discSet.m_DStock=dlg.m_longWAmount;
		m_discSet.Update();
	}
	else	{
		m_discSet.m_strFilter.Format("음반코드=%s",dlg.m_strDCode);
		m_discSet.Requery();
		m_discSet.Edit();
		m_discSet.m_DStock=m_discSet.m_DStock+dlg.m_longWAmount;
		m_discSet.Update();
		m_discSet.Requery();
	}

	if(!m_warehouseSet.IsOpen())	m_warehouseSet.Open();
	m_warehouseSet.AddNew();
	m_warehouseSet.m_WH_DCode=_T(dlg.m_strDCode);
	m_warehouseSet.m_WHNumber=_T(dlg.m_strWNumber);
	m_warehouseSet.m_WHAmount=dlg.m_longWAmount;
	m_warehouseSet.m_WHPrice=dlg.m_longWPrice;
	m_warehouseSet.m_WHDate=CTime::GetCurrentTime();
	m_warehouseSet.Update();
}

void CProdbDoc::OnSellingAdd()
{
	CProdbView* pView=((CProdbView*)((CMainFrame*)AfxGetMainWnd())->GetActiveView());
	if(!m_sellSet.IsOpen())	m_sellSet.Open();
	m_sellSet.AddNew();
	m_sellSet.m_S_CID=_T(pView->m_strCID);
	m_sellSet.m_S_DCode=_T(pView->m_strDCode);
	m_sellSet.m_SDate=CTime::GetCurrentTime();
	m_sellSet.m_SAmount=pView->m_longDAmount;
	m_sellSet.Update();

	if(!m_discSet.IsOpen()) m_discSet.Open();
	m_discSet.m_strFilter.Format("음반코드=%s",pView->m_strDCode);
	m_discSet.Requery();
	m_discSet.Edit();
	m_discSet.m_DStock=pView->m_longCStock-pView->m_longDAmount;
	m_discSet.Update();
	m_discSet.Requery();
}
