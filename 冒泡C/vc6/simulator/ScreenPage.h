#if !defined(AFX_SCREENPAGE_H__41BA39FE_8359_4116_8FC3_08ECA2B2E256__INCLUDED_)
#define AFX_SCREENPAGE_H__41BA39FE_8359_4116_8FC3_08ECA2B2E256__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScreenPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// ScreenPage dialog

class ScreenPage : public CPropertyPage
{
	DECLARE_DYNCREATE(ScreenPage)

// Construction
public:
	ScreenPage();
	~ScreenPage();

// Dialog Data
	//{{AFX_DATA(ScreenPage)
	enum { IDD = IDD_SCREEN };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(ScreenPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(ScreenPage)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////
// SCREENPage dialog

class SCREENPage : public CPropertyPage
{
	DECLARE_DYNCREATE(SCREENPage)

// Construction
public:
	SCREENPage();
	~SCREENPage();

// Dialog Data
	//{{AFX_DATA(SCREENPage)
	enum { IDD = IDD_SCREEN };
	BOOL	m_event;
	BOOL	m_file;
	BOOL	m_mem;
	BOOL	m_plaxex;
	BOOL	m_socket;
	BOOL	m_timer;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(SCREENPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(SCREENPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCREENPAGE_H__41BA39FE_8359_4116_8FC3_08ECA2B2E256__INCLUDED_)
