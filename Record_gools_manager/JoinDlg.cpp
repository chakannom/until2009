// JoinDlg.cpp : implementation file
//

#include "stdafx.h"
#include "prodb.h"
#include "JoinDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CJoinDlg dialog


CJoinDlg::CJoinDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CJoinDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CJoinDlg)
	m_strCAddress = _T("");
	m_strCID = _T("");
	m_strCJumin = _T("");
	m_strCName = _T("");
	m_strCPhone = _T("");
	//}}AFX_DATA_INIT
}


void CJoinDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CJoinDlg)
	DDX_Text(pDX, IDC_JOIN_CADDRESS, m_strCAddress);
	DDX_Text(pDX, IDC_JOIN_CID, m_strCID);
	DDX_Text(pDX, IDC_JOIN_CJUMIN, m_strCJumin);
	DDX_Text(pDX, IDC_JOIN_CNAME, m_strCName);
	DDX_Text(pDX, IDC_JOIN_CPHONE, m_strCPhone);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CJoinDlg, CDialog)
	//{{AFX_MSG_MAP(CJoinDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJoinDlg message handlers

void CJoinDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	
	if(m_strCID=="") {
		AfxMessageBox("고객ID를 입력하여 주십시오.");
		return;
	}

	if(m_strCName=="")	{
		AfxMessageBox("고객이름을 입력하여 주십시오.");
		return;
	}

	if(m_strCPhone=="")	{
		AfxMessageBox("전화번호을 입력하여 주십시오.");
		return;
	}

	if(m_strCJumin=="")	{
		AfxMessageBox("주민등록번호를 입력하여 주십시오.");
		return;
	}
	
	CDialog::OnOK();
}
