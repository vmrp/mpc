////////////////////////////////////////////////////////////////
// TraceWin Copyright 1995-2003 Paul DiLascia
// If this program works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
//
// ***************************************************************************
// TraceWin is a tool that displays MFC diagnostic (afxDump, TRACE) output
// in the window of the TraceWin applet.
//
// To use TraceWin, you must #include this file in your main program file
// (typically where you implement your CWinApp). Since this file contains code,
// you should #include it in only once--i.e. NOT in StdAfx.h--or you'll get
// multiply-defined symbol errors in the linker.
//
// To see the output, you also need the TraceWin applet, TraceWin.exe, which
// you can download free from http://pobox.com/~dilascia
// 
// ***************************************************************************
//
// Modification history
// 4-1-95:   Created
// 10-12-95: Use WM_COPYDATA instead of atoms. Also note the name of this 
//           file is changed to TraceWin.H.
// 11-10-95: Added "Keep Window on Top" feature and fixed a bug 
//           relating to saved window pos.
// 11-15-95: Save/Restore position with CWindowPlacement.
// 12-05-95: Implemented tracer object with "new" instead of static, so
//           it remains around for reporting memory leaks, if any.
// 8-27-96:  Made unicode compatible (I hope!)
// 9-14-96:  Added font UI
// 1-1-97:   New DLL scheme. TraceWin.h and MfxTraceInit no longer required
//           for apps that use DLL MFC.
// 5-2-97:   Added autoInit object which works now in VC 5.0 with init_seg
// 8-25-97:  * Removed DLL scheme because of problems with Visual Studio 5.0,
//             failure to capture OnInitInstance messages, unfreed objects,
//					TRACE messages from NT services that don't create a window, etc.
//           * Now shows output in debugger as well as TraceWin
//           * TraceWin applet can still be started after program
// 

// Window class name used by the main window of the TRACEWIN applet.
#define TRACEWND_CLASSNAME "MfxTraceWindow"

// ID sent as COPYDATASRUCT::dwData to identify the WM_COPYDATA message
// as coming from an app using TraceWin.
#define ID_COPYDATA_TRACEMSG MAKELONG(MAKEWORD('t','w'),MAKEWORD('i','n'))

#ifdef _DEBUG

//////////////////
// CFileTrace is a CFile that "writes" to the trace window
//
class CFileTrace : public CFile {
	DECLARE_DYNAMIC(CFileTrace)
	CFileTrace() { m_strFileName = "Mfx File Tracer"; }
	static BOOL bInitialized;
	virtual void Write(const void* lpBuf, UINT nCount);
public:
	static  BOOL Init();	
};
IMPLEMENT_DYNAMIC(CFileTrace, CFile)

//////////////////
// Override to write to TraceWin applet instead of file.
//
void CFileTrace::Write(const void* lpBuf, UINT nCount)
{
	if (!afxTraceEnabled)
		return;	// MFC tracing not enabled

	HWND hTraceWnd = ::FindWindow(TRACEWND_CLASSNAME, NULL);

	if (hTraceWnd) {
		// Found Trace window: send string with WM_COPYDATA
		// Must copy to make me the owner of the string; otherwise
		// barfs when called from MFC with traceMultiApp on
		//
		static char mybuf[1024];
		if (nCount < sizeof(mybuf)) {

			memcpy(mybuf, lpBuf, nCount);
			COPYDATASTRUCT cds;
			cds.dwData = ID_COPYDATA_TRACEMSG;
			cds.cbData = nCount;
			cds.lpData = mybuf;
			CWinApp* pApp = AfxGetApp();
			HWND hWnd = pApp ? pApp->m_pMainWnd->GetSafeHwnd() : NULL;
			::SendMessage(hTraceWnd, WM_COPYDATA, (WPARAM)hWnd, (LPARAM)&cds);
		}
	}
	// Also do normal debug thing
	::OutputDebugString((LPCTSTR)lpBuf);
}

/////////////////
// Initialize tracing. Replace global afxDump.m_pFile with CFileTrace object.
// In VC 5.0, you shouldn't need to call this, since it's called from an
// auto-initializing static object _doTraceInit below. But if you don't see
// any TRACE output in the TraceWin window, you should try calling
// MfxTraceInit any time before your first TRACE message.
//
BOOL CFileTrace::Init()
{
//		SetWindowPos(&wndTopMost,   0,   0,   0,   0,   SWP_NOMOVE | SWP_NOSIZE); 
	HWND hTraceWnd = ::FindWindow(TRACEWND_CLASSNAME, NULL);
    SetWindowPos( hTraceWnd, HWND_TOPMOST, 300, 0, 1000, 1000, SWP_NOMOVE||SWP_NOSIZE|SWP_NOZORDER);//维持当前Z序（忽略hWndlnsertAfter参数）。
	if (!bInitialized && afxDump.m_pFile==NULL) {
		// Initialize tracing: replace afxDump.m_pFile w/my own CFile object
		//
		// It's important to allocate with "new" here, not a static object,
		// because CFileTrace is virtual--i.e., called through a pointer in
		// the object's vtbl--and the compiler will zero out the virtual
		// function table with a NOP function when a static object
		// goes out of scope. But I want my CFileTrace to stay around to
		// display memory leak diagnostics even after all static objects
		// have been destructed. So I allocate the object with new and
		// never delete it. I don't want this allocation to itself generate
		// a reported memory leak, so I turn off memory tracking before I
		// allocate, then on again after.
		//
		BOOL bEnable = AfxEnableMemoryTracking(FALSE);
		afxDump.m_pFile = new CFileTrace;
		AfxEnableMemoryTracking(bEnable);
		bInitialized = TRUE;
	}
	
//	HWND hTraceWnd = ::FindWindow(TRACEWND_CLASSNAME, NULL);
//	SetForegroundWindow(hTraceWnd);
 //   SetWindowPos( hTraceWnd, HWND_TOPMOST, 300, 0, 1000, 1000, SWP_NOMOVE||SWP_NOSIZE);
	return bInitialized;
}

//////////////////
// This object does nothing but call CFileTrace::Init, so all you have to
// do is #include this file. Because afxDump is defined in a module with
//
// #pragma init_seg(lib)
//
// afxDump gets initialized before the "user" segment which is where your
// app (and bInitialized) is by default. If you need to use init_seg(lib),
// or you have other objects whose constructors call TRACE that get
// initialized before CFileTrace::bInitialized, you will have to call
// CFileTrace::Init yourself, before your first TRACE statement.
//
BOOL CFileTrace::bInitialized = CFileTrace::Init();

// This symbol is defined for backward compatibility with
// older versions of TraceWin.
//
#define MfxTraceInit() CFileTrace::Init()

#else

#define MfxTraceInit()

#endif // _DEBUG
