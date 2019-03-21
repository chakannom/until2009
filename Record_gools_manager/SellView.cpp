// SellView.cpp : implementation file
//

#include "stdafx.h"
#include "prodb.h"
#include "SellView.h"
#include "SellSet.h"
#include "prodbDoc.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSellView

IMPLEMENT_DYNCREATE(CSellView, CFormView)

CSellView::CSellView()
	: CFormView(CSellView::IDD)
{
	//{{AFX_DATA_INIT(CSellView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CSellView::~CSellView()
{
}

void CSellView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSellView)
	DDX_Control(pDX, IDC_LIST_SELLING, m_listC);
	//}}AFX_DATA_MAP
	CProdbDoc* pDoc=((CProdbDoc*)((CMainFrame*)AfxGetMainWnd())->GetActiveDocument());
	m_sSet=&pDoc->m_sellSet;
}


BEGIN_MESSAGE_MAP(CSellView, CFormView)
	//{{AFX_MSG_MAP(CSellView)
	ON_BN_CLICKED(ID_NEW, OnNew)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSellView diagnostics

#ifdef _DEBUG
void CSellView::AssertValid() const
{
	CFormView::AssertValid();
}

void CSellView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSellView message handlers

void CSellView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	m_listC.InsertColumn(0,"판매일",LVCFMT_LEFT,70);
	m_listC.InsertColumn(1,"고객ID",LVCFMT_LEFT,70);
	m_listC.InsertColumn(2,"음반코드",LVCFMT_LEFT,80);
	m_listC.InsertColumn(3,"수량",LVCFMT_LEFT,70);
	m_listC.SetExtendedStyle(m_listC.GetExtendedStyle()|LVS_EX_FULLROWSELECT);

	Init();
}
void CSellView::Init()
{
	int i=0;
	CString str;

	if(!m_sSet->IsOpen()) m_sSet->Open();
	m_listC.DeleteAllItems();
	m_sSet->m_strFilter.Format("");
	m_sSet->Requery();
	while(m_sSet->IsEOF()!=1)	{
		str=m_sSet->m_SDate.Format("%y년%m월%d일");
		m_listC.InsertItem(i,str,0);
		m_listC.SetItemText(i,1,m_sSet->m_S_CID);
		m_listC.SetItemText(i,2,m_sSet->m_S_DCode);
		str.Format("%d",m_sSet->m_SAmount);
		m_listC.SetItemText(i,3,str);
		m_sSet->MoveNext();
		i++;
	}
}
void CSellView::OnNew() 
{
	// TODO: Add your control notification handler code here
	Init();
}
