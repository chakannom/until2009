// WhView.cpp : implementation file
//

#include "stdafx.h"
#include "prodb.h"
#include "WhView.h"
#include "WarehouseSet.h"
#include "prodbDoc.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWhView

IMPLEMENT_DYNCREATE(CWhView, CFormView)

CWhView::CWhView()
	: CFormView(CWhView::IDD)
{
	//{{AFX_DATA_INIT(CWhView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CWhView::~CWhView()
{
}

void CWhView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWhView)
	DDX_Control(pDX, IDC_LIST_WH, m_listW);
	//}}AFX_DATA_MAP
	CProdbDoc* pDoc=((CProdbDoc*)((CMainFrame*)AfxGetMainWnd())->GetActiveDocument());
	m_wSet=&pDoc->m_warehouseSet;
}


BEGIN_MESSAGE_MAP(CWhView, CFormView)
	//{{AFX_MSG_MAP(CWhView)
	ON_BN_CLICKED(ID_NEW, OnNew)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWhView diagnostics

#ifdef _DEBUG
void CWhView::AssertValid() const
{
	CFormView::AssertValid();
}

void CWhView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWhView message handlers

void CWhView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	m_listW.InsertColumn(0,"입고일자",LVCFMT_LEFT,70);
	m_listW.InsertColumn(1,"입고번호",LVCFMT_LEFT,70);
	m_listW.InsertColumn(2,"음반코드",LVCFMT_LEFT,70);
	m_listW.InsertColumn(3,"입고단가",LVCFMT_LEFT,80);
	m_listW.InsertColumn(4,"입고수량",LVCFMT_LEFT,70);
	m_listW.SetExtendedStyle(m_listW.GetExtendedStyle()|LVS_EX_FULLROWSELECT);

	Init();
}
void CWhView::Init()
{
	int i=0;
	CString str;

	if(!m_wSet->IsOpen()) m_wSet->Open();
	m_listW.DeleteAllItems();
	m_wSet->m_strFilter.Format("");
	m_wSet->Requery();
	while(m_wSet->IsEOF()!=1)	{
		str=m_wSet->m_WHDate.Format("%y년%m월%d일");
		m_listW.InsertItem(i,str,0);
		m_listW.SetItemText(i,1,m_wSet->m_WHNumber);
		m_listW.SetItemText(i,2,m_wSet->m_WH_DCode);
		str.Format("%d",m_wSet->m_WHPrice);
		m_listW.SetItemText(i,3,str);
		str.Format("%d",m_wSet->m_WHAmount);
		m_listW.SetItemText(i,4,str);
		m_wSet->MoveNext();
		i++;
	}
}

void CWhView::OnNew() 
{
	// TODO: Add your control notification handler code here
	Init();	
}
