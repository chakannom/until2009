// WareHouseDlg.cpp : implementation file
//

#include "stdafx.h"
#include "prodb.h"
#include "WareHouseDlg.h"
#include "DiscSet.h"
#include "WareHouseSet.h"
#include "prodbDoc.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWareHouseDlg dialog


CWareHouseDlg::CWareHouseDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWareHouseDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWareHouseDlg)
	m_strDMake = _T("");
	m_strDName = _T("");
	m_longDPrice = 0;
	m_strDSinger = _T("");
	m_longWAmount = 0;
	m_longWPrice = 0;
	m_strDCode = _T("");
	//}}AFX_DATA_INIT
	m_discSet=&((CProdbDoc*)((CMainFrame*)AfxGetMainWnd())->GetActiveDocument())->m_discSet;
	m_warehouseSet=&((CProdbDoc*)((CMainFrame*)AfxGetMainWnd())->GetActiveDocument())->m_warehouseSet;

	IsData_=FALSE;
	m_strDCode = _T("");
}

void CWareHouseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWareHouseDlg)
	DDX_Control(pDX, IDC_LIST_DISC, list_WHDlg);
	DDX_Text(pDX, IDC_DISC_MAKE, m_strDMake);
	DDX_Text(pDX, IDC_DISC_NAME, m_strDName);
	DDX_Text(pDX, IDC_DISC_PRICE, m_longDPrice);
	DDX_Text(pDX, IDC_DISC_SINGER, m_strDSinger);
	DDX_Text(pDX, IDC_WH_AMOUNT, m_longWAmount);
	DDX_Text(pDX, IDC_WH_PRICE, m_longWPrice);
	DDX_Text(pDX, IDC_DISC_CODE, m_strDCode);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CWareHouseDlg, CDialog)
	//{{AFX_MSG_MAP(CWareHouseDlg)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_DISC, OnDblclkListDisc)
	ON_EN_CHANGE(IDC_DISC_NAME, OnChangeDiscName)
	ON_EN_CHANGE(IDC_DISC_SINGER, OnChangeDiscSinger)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWareHouseDlg message handlers

BOOL CWareHouseDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//Set the icon for this dialog. The framework does this automatically
	// when the application's main window is not a dialog
	list_WHDlg.InsertColumn(0,"음반코드",LVCFMT_LEFT,80);
	list_WHDlg.InsertColumn(1,"음반이름",LVCFMT_LEFT,80);
	list_WHDlg.InsertColumn(2,"가수",LVCFMT_LEFT,70);
	list_WHDlg.InsertColumn(3,"제작사",LVCFMT_LEFT,70);
	list_WHDlg.SetExtendedStyle(list_WHDlg.GetExtendedStyle()|LVS_EX_FULLROWSELECT);
	return TRUE;
}

void CWareHouseDlg::OnChangeDiscName() 
{
	// TODO: Add your control notification handler code here
	int i=0;
	CString str;
	UpdateData(TRUE);
	if(m_strDName!="")	{
		list_WHDlg.DeleteAllItems();
		if(!m_discSet->IsOpen())	m_discSet->Open();
		if(m_strDSinger=="") m_discSet->m_strFilter.Format("음반이름 like '%%%s%%'",m_strDName);
		else m_discSet->m_strFilter.Format("음반이름 like '%%%s%%' and 가수 like '%%%s%%'",m_strDName,m_strDSinger);
		m_discSet->Requery();
		IsData_=FALSE;
		while(m_discSet->IsEOF()!=1)	{
			list_WHDlg.InsertItem(i,m_discSet->m_DCode,0);
			list_WHDlg.SetItemText(i,1,m_discSet->m_DName);
			list_WHDlg.SetItemText(i,2,m_discSet->m_DSinger);
			list_WHDlg.SetItemText(i,3,m_discSet->m_DMake);
			m_discSet->MoveNext();
			IsData_=TRUE;
			i++;
		}
	}
}
void CWareHouseDlg::OnChangeDiscSinger()
{
	// TODO: Add your control notification handler code here
	int i=0;
	CString str;
	UpdateData(TRUE);
	if(m_strDSinger!="")	{
		list_WHDlg.DeleteAllItems();
		if(!m_discSet->IsOpen())	m_discSet->Open();
		if(m_strDName=="") m_discSet->m_strFilter.Format("가수 like '%%%s%%'",m_strDSinger);
		else m_discSet->m_strFilter.Format("음반이름 like '%%%s%%' and 가수 like '%%%s%%'",m_strDName,m_strDSinger);
		m_discSet->Requery();
		IsData_=FALSE;
		while(m_discSet->IsEOF()!=1)	{
			list_WHDlg.InsertItem(i,m_discSet->m_DCode,0);
			list_WHDlg.SetItemText(i,1,m_discSet->m_DName);
			list_WHDlg.SetItemText(i,2,m_discSet->m_DSinger);
			list_WHDlg.SetItemText(i,3,m_discSet->m_DMake);
			m_discSet->MoveNext();
			IsData_=TRUE;
			i++;
		}
	}
}
void CWareHouseDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	if(!IsData_)	{
		m_strDCode=CountDRecord();
		UpdateData(FALSE);
	}
	m_strWNumber=CountWRecord();
	if(m_strDName=="")	{
		AfxMessageBox("음반이름을 입력하여 주십시오.");
		return;
	}
	if(m_strDSinger=="")	{
		AfxMessageBox("가수를 입력하여 주십시오.");
		return;
	}
	if(m_longDPrice==0)	{
		AfxMessageBox("판매단가를 입력하여 주십시오.");
		return;
	}
	if(m_longWAmount==0)	{
		AfxMessageBox("입고수량을 입력하여 주십시오.");
		return;
	}
	if(m_longWPrice==0)	{
		AfxMessageBox("입고단가를 입력하여 주십시오.");
		return;
	}
	m_strWNumber=CountWRecord();
	CDialog::OnOK();
}

void CWareHouseDlg::OnDblclkListDisc(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NMITEMACTIVATE* pNMIA=(NMITEMACTIVATE*)pNMHDR;
	if(pNMIA->iItem!=-1)	{
		m_strDCode=list_WHDlg.GetItemText(pNMIA->iItem,0);
		m_strDName=list_WHDlg.GetItemText(pNMIA->iItem,1);
		m_strDSinger=list_WHDlg.GetItemText(pNMIA->iItem,2);
		m_strDMake=list_WHDlg.GetItemText(pNMIA->iItem,3);
		if(!m_discSet->IsOpen())	m_discSet->Open();
		m_discSet->m_strFilter.Format("음반코드=%s",m_strDCode);
		m_discSet->Requery();
		m_longDPrice=m_discSet->m_DPrice;
		UpdateData(FALSE);
	}
	*pResult = 0;
}
CString CWareHouseDlg::CountDRecord()
{
	CString str;
	int cnt=1;
	if(!m_discSet->IsOpen())	m_discSet->Open();
	m_discSet->m_strFilter.Format("");
	m_discSet->Requery();
	while(m_discSet->IsEOF()!=1)	{
		m_discSet->MoveNext();
		cnt++;
	}
	str.Format("%d",cnt);
	return str;
}
CString CWareHouseDlg::CountWRecord()
{
	CString str;
	int cnt=1;
	if(!m_warehouseSet->IsOpen())	m_warehouseSet->Open();
	m_warehouseSet->m_strFilter.Format("");
	m_warehouseSet->Requery();
	while(m_warehouseSet->IsEOF()!=1)	{
		m_warehouseSet->MoveNext();
		cnt++;
	}
	str.Format("%d",cnt);
	return str;
}
