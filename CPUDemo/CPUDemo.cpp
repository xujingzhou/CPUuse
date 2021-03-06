//////////////////////////////////////////////////////////////////////////////
// 类名：CCPUDemo
// 功能：应用程序的初始化
// 说明：在此加入动态菲屏显示(每次启动程序时菲屏以不同方式显示)
// 作者：徐景周(johnny Xu, xujingzhou2016@gmail.com)
// 组织：未来工作室(Future Studio)
// 日期：2003.1.1
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CPUDemo.h"
#include "CPUDemoDlg.h"
#include "Splash.h"					// 加入屝屏显示

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CComModule _Module;                // COM模块声明
/////////////////////////////////////////////////////////////////////////////
// CCPUDemoApp

BEGIN_MESSAGE_MAP(CCPUDemoApp, CWinApp)
	//{{AFX_MSG_MAP(CCPUDemoApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCPUDemoApp construction

CCPUDemoApp::CCPUDemoApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CCPUDemoApp object

CCPUDemoApp theApp;

/////////////////////////////////////////////////////////////////////////////
// 名称：InitInstance
// 功能：在此加入动态菲屏显示(每次启动程序时菲屏以不同方式显示)
// 作者：徐景周(jingzhou_xu@163.net)
// 组织：未来工作室(Future Studio)
// 日期：2003.1.1
/////////////////////////////////////////////////////////////////////////////
BOOL CCPUDemoApp::InitInstance()
{
	// 初始化COM,jingzhou xu
	CoInitialize(0);
	_Module.Init(ObjectMap,AfxGetInstanceHandle());

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	//此程序只能运行一次，用互斥量来判断程序是否已运行
    HANDLE m_hMutex=CreateMutex(NULL,TRUE, m_pszAppName); 
    if(GetLastError()==ERROR_ALREADY_EXISTS) { return FALSE; }

	//设置对话框背景和文本颜色
	SetDialogBkColor(RGB(160,180,220),RGB(0,0,0));

	//显示菲屏，jingzhou xu
	CSplashThread* pSplashThread = (CSplashThread*) AfxBeginThread(RUNTIME_CLASS(CSplashThread), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	if (pSplashThread == NULL)
	{
	AfxMessageBox(_T("扉屏显示线程创建失败!"), MB_OK | MB_ICONSTOP);
	return FALSE;
	}
	ASSERT(pSplashThread->IsKindOf(RUNTIME_CLASS(CSplashThread)));

	pSplashThread->SetBitmapToUse(IDB_SPLASH);  
	pSplashThread->SetShowMode(-1);    // 设置菲屏显示效果(-1--随机效果显示，0--水平交错效果，1--水平百叶窗效果，2--垂直百叶窗效果，3--随机积木效果，其它--正常显示), jingzhou xu
	pSplashThread->ResumeThread();     // 激活菲屏显示线程

	// 延时1秒
	Sleep(1000);   
	//隐藏显示的菲屏
	if (pSplashThread != NULL)
	{
		pSplashThread->HideSplash();
	}

	CCPUDemoDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// 名称：ExitInstance
// 功能：程序退出时清理工作
// 作者：徐景周(jingzhou_xu@163.net)
// 组织：未来工作室(Future Studio)
// 日期：2003.1.1
/////////////////////////////////////////////////////////////////////////////
int CCPUDemoApp::ExitInstance() 
{
	// 退出COM
	CoUninitialize();
	
	return CWinApp::ExitInstance();
}
