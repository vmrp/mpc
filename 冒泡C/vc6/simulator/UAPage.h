#if !defined(AFX_UAPAGE_H__BF1CB82D_32EB_485B_A70B_B9B132D7B3BD__INCLUDED_)
#define AFX_UAPAGE_H__BF1CB82D_32EB_485B_A70B_B9B132D7B3BD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UAPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// UAPage dialog

class UAPage : public CPropertyPage
{
	DECLARE_DYNCREATE(UAPage)

// Construction
public:
	UAPage();
	~UAPage();

// Dialog Data
	//{{AFX_DATA(UAPage)
	enum { IDD = IDD_UA };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(UAPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(UAPage)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UAPAGE_H__BF1CB82D_32EB_485B_A70B_B9B132D7B3BD__INCLUDED_)
