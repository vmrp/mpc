// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#define WM_EDITMESSAGE WM_USER+101//±à¼­¿ò
#define WM_ASYNCSOCKET WM_USER+102
#define WM_APPMESSAGE WM_USER+103
#define WM_RESTARTMESSAGE WM_USER+104
#define WM_OUTMESSAGE WM_USER+105
#define WM_REGTARTMESSAGE WM_USER+106
#if !defined(AFX_MAINFRM_H__6399275F_1E7F_43A0_9E71_A28649C32032__INCLUDED_)
#define AFX_MAINFRM_H__6399275F_1E7F_43A0_9E71_A28649C32032__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "Register.h"
//class CRegister;
class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
    CEdit edit;
    void *regdlg;
// Operations
public:
    void regeditKey();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;

// Generated message map functions

protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnFileOpen();
	afx_msg void OnFileResave();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTrace();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg  LRESULT OnEditMsg( WPARAM wParam, LPARAM lParam );
	afx_msg  LRESULT OnAsyncsocket( WPARAM wParam, LPARAM lParam );
	afx_msg  void OnAppMsg( WPARAM wParam, LPARAM lParam );
	afx_msg  void OnOutMsg( WPARAM wParam, LPARAM lParam );
	afx_msg void OnNomal();
	afx_msg void OnPause();
	afx_msg void OnRegister();
	afx_msg void OnHelpchm();
	afx_msg void OnCatch();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__6399275F_1E7F_43A0_9E71_A28649C32032__INCLUDED_)
