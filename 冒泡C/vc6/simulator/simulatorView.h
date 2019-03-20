// simulatorView.h : interface of the CSimulatorView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIMULATORVIEW_H__2225CF7C_F7BD_4F71_96CE_E177D0AB5157__INCLUDED_)
#define AFX_SIMULATORVIEW_H__2225CF7C_F7BD_4F71_96CE_E177D0AB5157__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CSimulatorView : public CView
{
protected: // create from serialization only
	CSimulatorView();
	DECLARE_DYNCREATE(CSimulatorView)

// Attributes
public:
	CSimulatorDoc* GetDocument();
	int bmpid[2];
// Operations
public:
     void initShow();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSimulatorView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSimulatorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CSimulatorView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg  void OnRestartMsg( WPARAM wParam, LPARAM lParam );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in simulatorView.cpp
inline CSimulatorDoc* CSimulatorView::GetDocument()
   { return (CSimulatorDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIMULATORVIEW_H__2225CF7C_F7BD_4F71_96CE_E177D0AB5157__INCLUDED_)
