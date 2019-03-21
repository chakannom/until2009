// prodbView.cpp : implementation of the CProdbView class
//

#include "stdafx.h"
#include "prodb.h"

#include "prodbSet.h"
#include "prodbDoc.h"
#include "prodbView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProdbView

IMPLEMENT_DYNCREATE(CProdbView, CRecordView)

BEGIN_MESSAGE_MAP(CProdbView, CRecordView)
	//{{AFX_MSG_MAP(CProdbView)
	ON_EN_CHANGE(IDC_VIEW_CID, OnChangeViewCid)
	ON_EN_CHANGE(IDC_VIEW_CPHONE, OnChangeViewCphone)
	ON_EN_CHANGE(IDC_VIEW_DNAME, OnChangeViewDname)
	ON_EN_CHANGE(IDC_VIEW_DCODE, OnChangeViewDcode)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_CUSTOMER, OnDblclkListCustomer)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_DISC, OnDblclkListDisc)
	ON_BN_CLICKED(ID_SELL_BN, OnSellBn)
	ON_BN_CLICKED(ID_INIT, OnInit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProdbView construction/destruction

CProdbView::CProdbView()
	: CRecordView(CProdbView::IDD)
{
	//{{AFX_DATA_INIT(CProdbView)
	m_pSet = NULL;
	m_strCID = _T("");
	m_strCName = _T("");
	m_strCPhone = _T("");
	m_longDAmount = 0;
	m_strDCode = _T("");
	m_strDName = _T("");
	m_strCPoint = _T("");
	//}}AFX_DATA_INIT
	// TODO: add construction code here

}

CProdbView::~CProdbView()
{
}

void CProdbView::DoDataExchange(CDataExchange* pDX)
{
	CRecordView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProdbView)
	DDX_Control(pDX, IDC_LIST_CUSTOMER, m_listCustomer);
	DDX_Control(pDX, IDC_LIST_DISC, m_listDisc);
	DDX_Text(pDX, IDC_VIEW_CID, m_strCID);
	DDX_Text(pDX, IDC_VIEW_CNAME, m_strCName);
	DDX_Text(pDX, IDC_VIEW_CPHONE, m_strCPhone);
	DDX_Text(pDX, IDC_VIEW_DAMOUNT, m_longDAmount);
	DDX_Text(pDX, IDC_VIEW_DCODE, m_strDCode);
	DDX_Text(pDX, IDC_VIEW_DNAME, m_strDName);
	DDX_Text(pDX, IDC_VIEW_POINT, m_strCPoint);
	//}}AFX_DATA_MAP
}

BOOL CProdbView::PreCreateWindow(CREATESTRUCT& cs)
{
 	// TODO: Modify the Window class or styles here by modifying
 	//  the CREATESTRUCT cs
 
 	return CRecordView::PreCreateWindow(cs);
}

void CProdbView::OnInitialUpdate()
{
	m_pSet = &GetDocument()->m_prodbSet;
	m_jSet = &GetDocument()->m_joinSet;
	m_dSet = &GetDocument()->m_discSet;
	CRecordView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	m_listCustomer.InsertColumn(0,"ID",LVCFMT_LEFT,70);
	m_listCustomer.InsertColumn(1,"이름",LVCFMT_LEFT,70);
	m_listCustomer.InsertColumn(2,"전화번호",LVCFMT_LEFT,80);
	m_listCustomer.InsertColumn(3,"마일리지",LVCFMT_LEFT,60);
	m_listCustomer.InsertColumn(4,"주소",LVCFMT_LEFT,70);
	m_listCustomer.InsertColumn(5,"주민번호",LVCFMT_LEFT,60);
	m_listCustomer.SetExtendedStyle(m_listCustomer.GetExtendedStyle()|LVS_EX_FULLROWSELECT);

	m_listDisc.InsertColumn(0,"음반코드",LVCFMT_LEFT,70);
	m_listDisc.InsertColumn(1,"음반이름",LVCFMT_LEFT,70);
	m_listDisc.InsertColumn(2,"가수",LVCFMT_LEFT,70);
	m_listDisc.InsertColumn(3,"수량",LVCFMT_LEFT,40);
	m_listDisc.InsertColumn(4,"가격",LVCFMT_LEFT,55);
	m_listDisc.InsertColumn(5,"제작사",LVCFMT_LEFT,60);
	m_listDisc.SetExtendedStyle(m_listDisc.GetExtendedStyle()|LVS_EX_FULLROWSELECT);

	InitList();
}
void CProdbView::InitList()
{
	int i;
	CString str;

	i=0;
	if(!m_jSet->IsOpen())	m_jSet->Open();
	m_jSet->m_strFilter.Format("");
	m_jSet->m_strSort.Format("고객ID ASC");
	m_jSet->Requery();
	m_listCustomer.DeleteAllItems();
	while(m_jSet->IsEOF()!=1)	{
		m_listCustomer.InsertItem(i,m_jSet->m_CID,0);
		m_listCustomer.SetItemText(i,1,m_jSet->m_CName);
		m_listCustomer.SetItemText(i,2,m_jSet->m_CPhone);
		str.Format("%d",m_jSet->m_CPoint);
		m_listCustomer.SetItemText(i,3,str);
		m_listCustomer.SetItemText(i,4,m_jSet->m_CAddress);
		m_listCustomer.SetItemText(i,5,m_jSet->m_CJumin);
		m_jSet->MoveNext();
		i++;
	}

	i=0;
	if(!m_dSet->IsOpen())	m_dSet->Open();
	m_dSet->m_strFilter.Format("");
	m_dSet->m_strSort.Format("음반코드 ASC");
	m_dSet->Requery();
	m_listDisc.DeleteAllItems();
	while(m_dSet->IsEOF()!=1)	{
		m_listDisc.InsertItem(i,m_dSet->m_DCode,0);
		m_listDisc.SetItemText(i,1,m_dSet->m_DName);
		m_listDisc.SetItemText(i,2,m_dSet->m_DSinger);
		str.Format("%d",m_dSet->m_DStock);
		m_listDisc.SetItemText(i,3,str);
		str.Format("%d",m_dSet->m_DPrice);
		m_listDisc.SetItemText(i,4,str);
		m_listDisc.SetItemText(i,5,m_dSet->m_DMake);
		m_dSet->MoveNext();
		i++;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CProdbView diagnostics

#ifdef _DEBUG
void CProdbView::AssertValid() const
{
	CRecordView::AssertValid();
}

void CProdbView::Dump(CDumpContext& dc) const
{
	CRecordView::Dump(dc);
}

CProdbDoc* CProdbView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CProdbDoc)));
	return (CProdbDoc*)m_pDocument;
}
#endif //_DEBUG
/////////////////////////////////////////////////////////////////////////////
// CProdbView database support
CRecordset* CProdbView::OnGetRecordset()
{
	return m_pSet;
}
/////////////////////////////////////////////////////////////////////////////
// CProdbView message handlers

void CProdbView::OnChangeViewCid() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CRecordView::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	int i=0;
	CString str;
	UpdateData(TRUE);
	if(m_strCID!="")	{
		m_listCustomer.DeleteAllItems();
		if(!m_jSet->IsOpen())	m_jSet->Open();
		if(m_strCPhone=="")	m_jSet->m_strFilter.Format("고객ID like '%%%s%%'",m_strCID);
		else m_jSet->m_strFilter.Format("고객ID like '%%%s%%' and 전화번호 like '%%%s%%'",m_strCID,m_strCPhone);
		m_jSet->Requery();
		while(m_jSet->IsEOF()!=1)	{
			m_listCustomer.InsertItem(i,m_jSet->m_CID,0);
			m_listCustomer.SetItemText(i,1,m_jSet->m_CName);
			m_listCustomer.SetItemText(i,2,m_jSet->m_CPhone);
			str.Format("%d",m_jSet->m_CPoint);
			m_listCustomer.SetItemText(i,3,str);
			m_listCustomer.SetItemText(i,4,m_jSet->m_CAddress);
			m_listCustomer.SetItemText(i,5,m_jSet->m_CJumin);
			m_jSet->MoveNext();
			i++;
		}
	}	
}

void CProdbView::OnChangeViewCphone() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CRecordView::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	int i=0;
	CString str;
	UpdateData(TRUE);
	if(m_strCPhone!="")	{
		m_listCustomer.DeleteAllItems();
		if(!m_jSet->IsOpen())	m_jSet->Open();
		if(m_strCID=="") m_jSet->m_strFilter.Format("전화번호 like '%%%s%%'",m_strCPhone);
		else m_jSet->m_strFilter.Format("고객ID like '%%%s%%' and 전화번호 like '%%%s%%'",m_strCID,m_strCPhone);
		m_jSet->Requery();
		while(m_jSet->IsEOF()!=1)	{
			m_listCustomer.InsertItem(i,m_jSet->m_CID,0);
			m_listCustomer.SetItemText(i,1,m_jSet->m_CName);
			m_listCustomer.SetItemText(i,2,m_jSet->m_CPhone);
			str.Format("%d",m_jSet->m_CPoint);
			m_listCustomer.SetItemText(i,3,str);
			m_listDisc.SetItemText(i,4,m_dSet->m_DMake);
			m_jSet->MoveNext();
			i++;
		}
	}
}

void CProdbView::OnChangeViewDname() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CRecordView::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	int i=0;
	CString str;
	UpdateData(TRUE);
	if(m_strDName!="")	{
		m_listDisc.DeleteAllItems();
		if(!m_dSet->IsOpen())	m_dSet->Open();
		if(m_strDCode=="") m_dSet->m_strFilter.Format("음반이름 like '%%%s%%'",m_strDName);
		else m_dSet->m_strFilter.Format("음반코드 like '%%%s%%' and 음반이름 like '%%%s%%'",m_strDCode,m_strDName);
		m_dSet->Requery();
		while(m_dSet->IsEOF()!=1)	{
			m_listDisc.InsertItem(i,m_dSet->m_DCode,0);
			m_listDisc.SetItemText(i,1,m_dSet->m_DName);
			m_listDisc.SetItemText(i,2,m_dSet->m_DSinger);
			str.Format("%d",m_dSet->m_DStock);
			m_listDisc.SetItemText(i,3,str);
			str.Format("%d",m_dSet->m_DPrice);
			m_listDisc.SetItemText(i,4,str);
			m_dSet->MoveNext();
			i++;
		}
	}
}
void CProdbView::OnChangeViewDcode() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CRecordView::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	int i=0;
	CString str;
	UpdateData(TRUE);
	if(m_strDCode!="")	{
		m_listDisc.DeleteAllItems();
		if(!m_dSet->IsOpen())	m_dSet->Open();
		if(m_strDName=="") m_dSet->m_strFilter.Format("음반코드 like '%%%s%%'",m_strDCode);
		else m_dSet->m_strFilter.Format("음반코드 like '%%%s%%' and 음반이름 like '%%%s%%'",m_strDCode,m_strDName);
		m_dSet->Requery();
		while(m_dSet->IsEOF()!=1)	{
			m_listDisc.InsertItem(i,m_dSet->m_DCode,0);
			m_listDisc.SetItemText(i,1,m_dSet->m_DName);
			m_listDisc.SetItemText(i,2,m_dSet->m_DSinger);
			str.Format("%d",m_dSet->m_DStock);
			m_listDisc.SetItemText(i,3,str);
			str.Format("%d",m_dSet->m_DPrice);
			m_listDisc.SetItemText(i,4,str);
			m_dSet->MoveNext();
			i++;
		}
	}
}
void CProdbView::OnDblclkListCustomer(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NMITEMACTIVATE* pNMIA=(NMITEMACTIVATE*)pNMHDR;
	if(pNMIA->iItem!=-1)	{
		m_strCID=m_listCustomer.GetItemText(pNMIA->iItem,0);
		m_strCName=m_listCustomer.GetItemText(pNMIA->iItem,1);
		m_strCPhone=m_listCustomer.GetItemText(pNMIA->iItem,2);
		m_strCPoint=m_listCustomer.GetItemText(pNMIA->iItem,3);
		UpdateData(FALSE);
	}
	*pResult = 0;
}

void CProdbView::OnDblclkListDisc(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NMITEMACTIVATE* pNMIA=(NMITEMACTIVATE*)pNMHDR;
	if(pNMIA->iItem!=-1)	{
		m_strDCode=m_listDisc.GetItemText(pNMIA->iItem,0);
		m_strDName=m_listDisc.GetItemText(pNMIA->iItem,1);
		UpdateData(FALSE);
	}
	*pResult = 0;
}

void CProdbView::OnSellBn() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_strCID=="")	{
		AfxMessageBox("고객ID를 입력하세요.");
		return;
	}
	if(m_strCName=="")	{
		AfxMessageBox("고객이름을 입력하세요.");
		return;
	}
	if(m_strCPhone=="")	{
		AfxMessageBox("전화번호를 입력하세요.");
		return;
	}
	if(m_strDCode=="")	{
		AfxMessageBox("음반코드를 입력하세요.");
		return;
	}
	if(m_strDName=="")	{
		AfxMessageBox("음반이름을 입력하세요.");
		return;
	}
	if(m_longDAmount==0)	{
		AfxMessageBox("수량을 입력하세요.");
		return;
	}
	if(!m_dSet->IsOpen())	m_dSet->Open();
	m_dSet->m_strFilter.Format("음반코드=%s",m_strDCode);
	m_dSet->Requery();
	m_longCStock=m_dSet->m_DStock;

	if(m_longCStock==0)	{
		AfxMessageBox(m_strDName+"의 재고량이 없습니다.");
		return;
	}
	if(m_longCStock<m_longDAmount)	{
		AfxMessageBox(m_strDName+ "의 재고량이 부족합니다.");
		return;
	}
	GetDocument()->OnSellingAdd();

}

void CProdbView::OnInit() 
{
	// TODO: Add your control notification handler code here
	m_strCID="";
	m_strCName="";
	m_strCPhone="";
	m_longDAmount=0;
	m_strDCode="";
	m_strDName="";
	m_strCPoint="";
	UpdateData(FALSE);
	InitList();
}
