// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__DE7AF4CA_12C5_41CE_ADC2_B3C7ED4E78DE__INCLUDED_)
#define AFX_MAINFRM_H__DE7AF4CA_12C5_41CE_ADC2_B3C7ED4E78DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	CView* FindView(CRuntimeClass *pClass);
	void SwitchForm(int nForm);
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg void OnSellList();
	afx_msg void OnWhList();
	afx_msg void OnSell();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	int m_nView;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__DE7AF4CA_12C5_41CE_ADC2_B3C7ED4E78DE__INCLUDED_)
