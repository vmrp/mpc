#if !defined(AFX_OTHERPAGE_H__1EA14EA6_BAEF_478E_A8DB_FD0C31D8D5C0__INCLUDED_)
#define AFX_OTHERPAGE_H__1EA14EA6_BAEF_478E_A8DB_FD0C31D8D5C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OTHERPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// OTHERPage dialog

class OTHERPage : public CPropertyPage
{
	DECLARE_DYNCREATE(OTHERPage)

// Construction
public:
	OTHERPage();
	~OTHERPage();

// Dialog Data
	//{{AFX_DATA(OTHERPage)
	enum { IDD = IDD_OTHER };
	BOOL	m_event;
	BOOL	m_file;
	BOOL	m_mem;
	BOOL	m_plaxex;
	BOOL	m_socket;
	BOOL	m_timer;
	BOOL	m_ui;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(OTHERPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(OTHERPage)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OTHERPAGE_H__1EA14EA6_BAEF_478E_A8DB_FD0C31D8D5C0__INCLUDED_)
