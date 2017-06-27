/*
Module : SPLASHER.CPP
Purpose: A splash screen component for MFC 4.x which uses a DIB bitmap
         instead of a resource. Palette management code is also included
         so that the bitmap will display using its own optimized palette.
         It also used as a UI thread so that UI of the splash screen 
         remains responsive.
Created: PJN / 15-11-1996
History: PJN / 11-12-1997 1) Incoporation of new DIB code provided by the authors own
                          CDibImage class. A static library version is used by 
                          splasher here. If you want to use CDIbImage for anything
                          other than CSplashThread, then you need to download
                          CDibImage on its own.
                          2) Can now load from resource or bitmap
                          3) General tidy up of the code.
         PJN / 22-3-1998  1) Palette is now correctly initialised on startup
                          2) Code now protects itself if m_SplashScreen cannot be created
         PJN / 22-12-1998 1) Now no longer dependent on CDibImage.
                          2) Unicode Enabled the code,
                          3) General tidy up of the code
                          4) Removed the unnecessary variable m_bCreated
                          5) Fixed a potential race condition in CSplashThread::HideSplash()
         PJN / 01-03-2000 1) Fixed a problem with bitmaps which do not have a palette
                          2) Fixed a problem in Win 98 and Win2000 when the splash screen is
                          closed but the main window of your app fails to activate. The code 
                          now uses AttachThreadInput to synchronise the UI activities of the
                          main GUI thread and the splash screen thread.

Copyright (c) 1996 - 2000 by PJ Naughter.  
All rights reserved.

Modified by: jingzhou xu
*/


//////////////////// Includes ///////////////////
#include "stdafx.h"
#include "resource.h"
#include "Splash.h"



//////////////////// Defines ////////////////////
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define MOUSE_MOVE 0xF012

BEGIN_MESSAGE_MAP(CSplashThread, CWinThread)
	//{{AFX_MSG_MAP(CSplashThread)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

IMPLEMENT_DYNCREATE(CSplashThread, CWinThread)

CSplashThread::CSplashThread()
{
}

CSplashThread::~CSplashThread()
{
}

BOOL CSplashThread::InitInstance()
{
  //Attach this threads UI state to the main one, This will ensure that 
  //the activation state is managed consistenly across the two threads
  ASSERT(AfxGetApp());
  BOOL bSuccess = AttachThreadInput(m_nThreadID, AfxGetApp()->m_nThreadID, TRUE);
  if (!bSuccess)
    TRACE(_T("Failed in call to AttachThredInput, GetLastError:%d\n"), ::GetLastError());

  //Create the Splash Screen UI
  BOOL bCreated = m_SplashScreen.Create();
	VERIFY(bCreated);
  m_pMainWnd = &m_SplashScreen;
	return bCreated;
}

void CSplashThread::HideSplash()
{
  //Wait until the splash screen has been created
  //before trying to close it
  while (!m_SplashScreen.GetSafeHwnd());

  m_SplashScreen.SetOKToClose();
  m_SplashScreen.SendMessage(WM_CLOSE);

}

void CSplashThread::SetBitmapToUse(const CString& sFilename)
{
  m_SplashScreen.SetBitmapToUse(sFilename);
}

void CSplashThread::SetBitmapToUse(UINT nResourceID)
{
  m_SplashScreen.SetBitmapToUse(nResourceID);
}

void CSplashThread::SetBitmapToUse(LPCTSTR pszResourceName)
{
  m_SplashScreen.SetBitmapToUse(pszResourceName);
}

// 设置菲屏显示效果(-1--随机效果显示，0--水平交错效果，1--水平百叶窗效果，2--垂直百叶窗效果，3--随机积木效果，其它--正常显示), jingzhou xu
void CSplashThread::SetShowMode(int nShowMode)   
{
	m_SplashScreen.SetShowMode(nShowMode);
}

BEGIN_MESSAGE_MAP(CSplashWnd, CWnd)
  //{{AFX_MSG_MAP(CSplashWnd)
  ON_WM_CREATE()
  ON_WM_PAINT()
  ON_WM_LBUTTONDOWN()
  ON_WM_CLOSE()
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

CSplashWnd::CSplashWnd()
{
  m_bOKToClose = FALSE;
  m_nHeight = 0;
  m_nWidth = 0;

  m_nShowMode = 1;          // 初始设置菲屏显示效果(-1--随机效果显示，0--水平交错效果，1--水平百叶窗效果，2--垂直百叶窗效果，3--随机积木效果，其它--正常显示), jingzhou xu
}

CSplashWnd::~CSplashWnd()
{
  //destroy our invisible owner when we're done
  if (m_wndOwner.m_hWnd != NULL)
    m_wndOwner.DestroyWindow();
}

BOOL CSplashWnd::LoadBitmap()
{     
  //Use LoadImage to get the image loaded into a DIBSection
  HBITMAP hBitmap;
  if (m_bUseFile)
    hBitmap = (HBITMAP) ::LoadImage(NULL, m_sFilename, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
  else
    hBitmap = (HBITMAP) ::LoadImage(AfxGetResourceHandle(), m_pszResourceName, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE);

  //Check that we could load it up  
  if (hBitmap == NULL)       
    return FALSE;

  //Get the width and height of the DIBSection
  BITMAP bm;
  GetObject(hBitmap, sizeof(BITMAP), &bm);
  m_nHeight = bm.bmHeight;
  m_nWidth = bm.bmWidth;

  //Covert from the SDK bitmap handle to the MFC equivalent
  m_Bitmap.Attach(hBitmap);

  return TRUE;   
}

void CSplashWnd::CreatePaletteFromBitmap()
{
  //Get the color depth of the DIBSection
  BITMAP bm;
  m_Bitmap.GetObject(sizeof(BITMAP), &bm);

  //If the DIBSection is 256 color or less, it has a color table
  if ((bm.bmBitsPixel * bm.bmPlanes) <= 8 )     
  {
    //Create a memory DC and select the DIBSection into it
    CDC memDC;
    memDC.CreateCompatibleDC(NULL);
    CBitmap* pOldBitmap = memDC.SelectObject(&m_Bitmap);

    //Get the DIBSection's color table
    RGBQUAD rgb[256];
    ::GetDIBColorTable(memDC.m_hDC, 0, 256, rgb);

    //Create a palette from the color table
    LPLOGPALETTE pLogPal = (LPLOGPALETTE) new BYTE[sizeof(LOGPALETTE) + (256*sizeof(PALETTEENTRY))];
    pLogPal->palVersion = 0x300;       
    pLogPal->palNumEntries = 256;

    for (WORD i=0; i<256; i++)       
    {
      pLogPal->palPalEntry[i].peRed = rgb[i].rgbRed;
      pLogPal->palPalEntry[i].peGreen = rgb[i].rgbGreen;
      pLogPal->palPalEntry[i].peBlue = rgb[i].rgbBlue;
      pLogPal->palPalEntry[i].peFlags = 0;
    }
    VERIFY(m_Palette.CreatePalette(pLogPal));
    
    //Clean up
    delete pLogPal;
    memDC.SelectObject(pOldBitmap);

	//zwh Adding,2002.6.20 
	memDC.DeleteDC();
  }
  else  //It has no color table, so use a halftone palette     
  {
    CDC* pRefDC = GetDC();
    m_Palette.CreateHalftonePalette(pRefDC);
    ReleaseDC(pRefDC);     
  }     
}

BOOL CSplashWnd::Create()
{                   
  //Load up the bitmap from file or from resource
  VERIFY(LoadBitmap());

  //Modify the owner window of the splash screen to be an invisible WS_POPUP 
  //window so that the splash screen does not appear in the task bar
  LPCTSTR pszWndClass = AfxRegisterWndClass(0);
  VERIFY(m_wndOwner.CreateEx(0, pszWndClass, _T(""), WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, 0));

  //Create this window
  pszWndClass = AfxRegisterWndClass(0, AfxGetApp()->LoadStandardCursor(IDC_ARROW));
  VERIFY(CreateEx(0, pszWndClass, _T(""), WS_POPUP | WS_VISIBLE, 0, 0, m_nWidth, m_nHeight, m_wndOwner.GetSafeHwnd(), NULL));

  //Create the palette, We need to do this after the window is created because
  //we may need to access the DC associated with it
  CreatePaletteFromBitmap();

  //realize the bitmap's palette into the DC
  OnQueryNewPalette();

  return TRUE;
}

int CSplashWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CWnd::OnCreate(lpCreateStruct) == -1)
    return -1;

  //Center the splash window on the screen
  CenterWindow();

  return 0;
}

void CSplashWnd::OnPaint()
{
	CPaintDC dc(this);

	// 选择调色板及位图到DC中
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	CBitmap* pOldBitmap = memDC.SelectObject(&m_Bitmap);
	CPalette* pOldPalette = dc.SelectPalette(&m_Palette, FALSE);
	dc.RealizePalette();

	//---------------------------------------------------------
	// 以下部分为动态显示菲屏效果代码，jingzhou xu 
	int i,j,step,stepx,stepy,dispnum,x,y; 

	if(m_nShowMode == -1)				// 随机菲屏效果显示
	{
		srand((unsigned)time(NULL)); 
		m_nShowMode = rand()%5;
	}
	switch(m_nShowMode)
	{
	case 0:
		// 一、水平交错效果算法
		for( i=0; i<=m_nHeight; i+=2 ) 
		{
			j = i; 
			while(j>0) 
			{
				dc.StretchBlt( 
					//奇数，由上至下 
					0,j-1, 
					//目标设备逻辑横、纵坐标 
					m_nWidth,1, 
					//显示位图的像素宽、高度 
					&memDC, 
					//源位图设备情境对象 
					0,m_nHeight-(i-j-1), 
					//源位图的起始横、纵坐标 
					m_nWidth,1, 
					//源位图的像素宽、高度 
					SRCCOPY); 
				
				dc.StretchBlt( 
					//偶数，由下至上 
					0,m_nHeight-j, 
					//目标设备逻辑横、纵坐标 
					m_nWidth,1, 
					//显示位图的像素宽、高度 
					&memDC, 
					//源位图设备情境对象 
					0,i-j, 
					//源位图的起始横、纵坐标 
					m_nWidth,1, 
					//源位图的像素宽、高度 
					SRCCOPY); 
				j-=2; 
			} 
			Sleep(10);
		} 
		dc.BitBlt(0, 0, m_nWidth, m_nHeight, &memDC, 0, 0, SRCCOPY);

		break;
	case 1:
		// 二、水平百叶窗效果算法
		step=m_nHeight/15; 
		for ( i=0; i<=step; i++ ) 
		{
			for ( j=0; j<15; j++ ) 
				dc.StretchBlt( 
				0,j*step+i, 
				//目标设备逻辑横、纵坐标 
				m_nWidth,1,
				//显示位图的像素宽、高度 
				&memDC, 
				//源位图设备情境对象 
				0,j*step+i, 
				//源位图的起始横、纵坐标 
				m_nWidth,1, 
				//源位图的像素宽、高度 
				SRCCOPY); 
			
			Sleep(30); 
		}

		break;
	case 2:
		// 三、垂直百叶窗效果算法
		step=m_nWidth/15; 
		for ( i=0; i<=step; i++ ) 
		{
			for ( j=0; j<15; j++ ) 
				dc.StretchBlt( 
				j*step+i,0, 
				//目标设备逻辑横、纵坐标 
				1,m_nHeight,
				//显示位图的像素宽、高度 
				&memDC, 
				//源位图设备情境对象 
				j*step+i,0, 
				//源位图的起始横、纵坐标 
				1, m_nHeight,
				//源位图的像素宽、高度 
				SRCCOPY); 
			
			Sleep(20); 
		}

		break;
	case 3:
		// 四、随机积木效果算法
		int pxy[10][10];             // 碎片数
		for ( i=0; i<10; i++ ) 
			for ( j=0; j<10; j++ ) 
				pxy[i][j]=0; 
		stepx=m_nWidth/10; 
		stepy=m_nHeight/10; 
		srand( (unsigned)time( NULL ) ); 
		dispnum=0; 
		while(1) 
		{ 
			x=rand() % 10; 
			y=rand() % 10; 
			if ( pxy[x][y] )       // 已经显示过，跳过
				continue; 
			pxy[x][y]=1;           // 当前显示的置1
			dc.StretchBlt( 
				x*stepx, y*stepy, 
				//目标设备逻辑横、纵坐标 
				stepx,stepy, 
				//显示位图的像素宽、高度 
				&memDC, 
				//源位图设备情境对象 
				x*stepx, y*stepy, 
				//源位图的起始横、纵坐标 
				stepx,stepy, 
				//源位图的像素宽、高度 
				SRCCOPY); 
			dispnum++; 

			if ( dispnum >=100 )   // 是否显示完
				break; 

			Sleep(10); 
		} 

		break;
	default:
		// 五、正常显示
		dc.BitBlt(0, 0, m_nWidth, m_nHeight, &memDC, 0, 0, SRCCOPY);

		break;
	}
	//---------------------------------------------------------

	memDC.SelectObject(pOldBitmap);         
	dc.SelectPalette(pOldPalette, FALSE);

	memDC.DeleteDC();
}

//This message is an optional extra, If you do not want the splash screen
//to be not be dragable then remove this function and its message map entry
void CSplashWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{

}

void CSplashWnd::OnClose() 
{
  if (m_bOKToClose) 
    CWnd::OnClose();

   AfxEndThread(0);
}

BOOL CSplashWnd::SelRelPal(BOOL bForceBkgnd)
{
  // We are going active, so realize our palette.
  CDC* pDC = GetDC();

  CPalette* pOldPal = pDC->SelectPalette(&m_Palette, bForceBkgnd);
  UINT u = pDC->RealizePalette();
  pDC->SelectPalette(pOldPal, TRUE);
  pDC->RealizePalette();

  ReleaseDC(pDC);

  // If any colors have changed or we are in the
  // background, repaint the lot.
  if (u || bForceBkgnd) 
    InvalidateRect(NULL, TRUE); // Repaint.
  
  return (BOOL) u; // TRUE if some colors changed.
}

void CSplashWnd::OnPaletteChanged(CWnd* pFocusWnd)
{
  // See if the change was caused by us and ignore it if not.
  if (pFocusWnd != this) 
    SelRelPal(TRUE); // Realize in the background. 
}

BOOL CSplashWnd::OnQueryNewPalette()
{
  return SelRelPal(FALSE); // Realize in the foreground.
}

void CSplashWnd::SetBitmapToUse(const CString& sFilename)
{
  m_bUseFile = TRUE;
  m_sFilename = sFilename;
}

void CSplashWnd::SetBitmapToUse(UINT nResourceID)
{
  m_bUseFile = FALSE;
  m_pszResourceName = MAKEINTRESOURCE(nResourceID);
}

void CSplashWnd::SetBitmapToUse(LPCTSTR pszResourceName)
{
  m_bUseFile = FALSE;
  m_pszResourceName = pszResourceName;
}


BOOL CSplashWnd::PreTranslateMessage(MSG* pMsg) 
{
	// If we get a keyboard or mouse message, hide the splash screen.
	if (pMsg->message == WM_KEYDOWN ||
	    pMsg->message == WM_SYSKEYDOWN ||
	    pMsg->message == WM_LBUTTONDOWN ||
	    pMsg->message == WM_RBUTTONDOWN ||
	    pMsg->message == WM_MBUTTONDOWN ||
	    pMsg->message == WM_NCLBUTTONDOWN ||
	    pMsg->message == WM_NCRBUTTONDOWN ||
	    pMsg->message == WM_NCMBUTTONDOWN)
	{
//		SendMessage(WM_CLOSE);
//		return TRUE;	// message handled here
	}	

	return CWnd::PreTranslateMessage(pMsg);
}
