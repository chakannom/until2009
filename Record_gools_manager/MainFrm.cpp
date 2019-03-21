// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "prodb.h"
#include "prodbDoc.h"
#include "prodbView.h"
#include "SellView.h"
#include "WhView.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_COMMAND(ID_SELL_LIST, OnSellList)
	ON_COMMAND(ID_WH_LIST, OnWhList)
	ON_COMMAND(ID_SELL, OnSell)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.lpszName = "음반판매점 데이터베이스";
	cs.style &= ~FWS_ADDTOTITLE; //타이틀바의 "제목없음"을 제거

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::OnSellList() 
{
	// TODO: Add your command handler code here
	if(m_nView!=2) SwitchForm(IDD_LIST_SELL); 
	m_nView=2;
}

void CMainFrame::OnWhList() 
{
	// TODO: Add your command handler code here
	if(m_nView!=3) SwitchForm(IDD_LIST_WH); 
	m_nView=3;	
}

void CMainFrame::OnSell() 
{
	// TODO: Add your command handler code here
	if(m_nView!=1) SwitchForm(IDD_PRODB_FORM); 
	m_nView=1;
	
}
void CMainFrame::SwitchForm(int nForm)
{
	CView* pOldView = GetActiveView();
	CView* pView=NULL;
	CDocument* pCurrentDoc = GetActiveDocument();
	switch(nForm)
	{
	case IDD_PRODB_FORM:
		pView = FindView(RUNTIME_CLASS(CProdbView));
		break;
	case IDD_LIST_SELL:
		pView = FindView(RUNTIME_CLASS(CSellView));
		break;
	case IDD_LIST_WH:
		pView = FindView(RUNTIME_CLASS(CWhView));
		break;
	}
	SetActiveView(pView);
	int nID = pView->GetDlgCtrlID();
	pView->SetDlgCtrlID(AFX_IDW_PANE_FIRST);
	pOldView->SetDlgCtrlID(nID);

	pView->ShowWindow(SW_SHOW);
	pOldView->ShowWindow(SW_HIDE);

	RecalcLayout();
}
CView * CMainFrame::FindView(CRuntimeClass *pClass)
{
	CDocument* pCurrentDoc = GetActiveDocument();
	POSITION pos = pCurrentDoc->GetFirstViewPosition();
	CView *pView = NULL;
	while(pos)
	{
		pView = pCurrentDoc->GetNextView(pos);
		if(pView->IsKindOf(pClass)) return pView;
	}
	return NULL;
}
