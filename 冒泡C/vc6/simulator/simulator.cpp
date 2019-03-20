// simulator.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "simulator.h"
#include "tracewin.h"
#include "MainFrm.h"
#include "simulatorDoc.h"
#include "simulatorView.h"
#include   <afxadv.h> //hxh:Has   CRecentFileList   class   definition. 
#include "checkmrp.h"
#include "sim_api.h"
#include "sim_win.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSimulatorApp

BEGIN_MESSAGE_MAP(CSimulatorApp, CWinApp)
	//{{AFX_MSG_MAP(CSimulatorApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	ON_COMMAND_RANGE(ID_FILE_MRU_FILE1,   ID_FILE_MRU_FILE1+20,   MyMRUFileHandler)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSimulatorApp construction

CSimulatorApp::CSimulatorApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSimulatorApp object

CSimulatorApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSimulatorApp initialization

BOOL CSimulatorApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.
    MfxTraceInit();
#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;

	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CSimulatorDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CSimulatorView));
	AddDocTemplate(pDocTemplate);
//	EnableShellOpen();//注意要放在AddDocTemplate之后   
//	RegisterShellFileTypes   ();   
	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	char mrpFullPath[MAX_PATH];
	memset(mrpFullPath,0,sizeof(mrpFullPath));
	strcpy(mrpFullPath,cmdInfo.m_strFileName);
	/*
	int   i = strlen(mrpFullPath);   
	TRACE("!!!!!!!!==%d\n",i);
	while   (i>1&&mrpFullPath[i]!='\\')   
	{
		i--;   
		mrpFullPath[i+1]='\0';  
	}
	*/

	//TRACE("@@@@@@@@@@@@@@@@@@@ == %s == \n",mrpFullPath);
	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	if (strlen(mrpFullPath) > 1) {
		saveCheckMrp(mrpFullPath);
		startMrp();
	}
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CSimulatorApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CSimulatorApp message handlers

void CSimulatorApp::MyMRUFileHandler(UINT i)
{
	ASSERT_VALID(this);   
	ASSERT(m_pRecentFileList   !=   NULL);   
	
	ASSERT(i   >=   ID_FILE_MRU_FILE1);   
	ASSERT(i   <   ID_FILE_MRU_FILE1   +   (UINT)m_pRecentFileList->GetSize());   
	
	CString   strName,   strCurDir,   strMessage;   
	int   nIndex   =   i   -   ID_FILE_MRU_FILE1;   
	ASSERT((*m_pRecentFileList)[nIndex].GetLength()   !=   0);   
	
	strName.Format("MRU:   open   file   (%d)   '%s'.\n",   (nIndex)   +   1,(LPCTSTR)(*m_pRecentFileList)[nIndex]);   
//	TRACE("hahahaha === %s\n",(*m_pRecentFileList)[nIndex]);
	if   (OpenDocumentFile((*m_pRecentFileList)[nIndex])   ==   NULL)   
	{ 
		m_pRecentFileList->Remove(nIndex);   
	}else{
	//	AfxMessageBox(strName);
		 
		if (CheckMrp() == 0) {
			saveCheckMrp((*m_pRecentFileList)[0]);  
			startMrp();
		}
		else
		{
			//AfxMessageBox(strName); 
			char path[MAX_PATH] = {0};
			memcpy(path,(*m_pRecentFileList)[0],MAX_PATH);
//			TRACE("path == %s, == len == %d\n",path,strlen(path));
			::SendMessage(getFrmHandle(),WM_APPMESSAGE,0,(LPARAM)path);
		}
		
	}
	
}

