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
		AfxMessageBox("��ID�� �Է��Ͽ� �ֽʽÿ�.");
		return;
	}

	if(m_strCName=="")	{
		AfxMessageBox("���̸��� �Է��Ͽ� �ֽʽÿ�.");
		return;
	}

	if(m_strCPhone=="")	{
		AfxMessageBox("��ȭ��ȣ�� �Է��Ͽ� �ֽʽÿ�.");
		return;
	}

	if(m_strCJumin=="")	{
		AfxMessageBox("�ֹε�Ϲ�ȣ�� �Է��Ͽ� �ֽʽÿ�.");
		return;
	}
	
	CDialog::OnOK();
}
