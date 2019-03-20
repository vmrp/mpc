#if !defined(AFX_CHECK_H__9E1B7985_9C9A_481F_BB06_0532C20950CE__INCLUDED_)
#define AFX_CHECK_H__9E1B7985_9C9A_481F_BB06_0532C20950CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Check.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCheck dialog

class CCheck : public CDialog
{
// Construction
public:
	CCheck(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCheck)
	enum { IDD = IDD_CHECK };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCheck)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCheck)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHECK_H__9E1B7985_9C9A_481F_BB06_0532C20950CE__INCLUDED_)
