#if !defined(AFX_ENHPROGRESSCTRL_H__12909D73_C393_11D1_9FAE_8192554015AD__INCLUDED_)
#define AFX_ENHPROGRESSCTRL_H__12909D73_C393_11D1_9FAE_8192554015AD__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// EnhProgressCtrl.h : header file
//
//
/*
Modified by 徐景周 2000.12
功能：渐变色进度条显示效果
*/
//
#include "MemDC.h"

/////////////////////////////////////////////////////////////////////////////
// CGradientProgressCtrl window

class CGradientProgressCtrl : public CProgressCtrl
{
// Construction
public:
	CGradientProgressCtrl();

// Attributes
public:
// Attributes
	
	void SetRange(int nLower, int nUpper);
	void SetRange32( int nLower, int nUpper );
	int SetPos(int nPos);
	int SetStep(int nStep);
	int StepIt(void);

// Operations
public:
	
	// Set Functions
	void SetTextColor(COLORREF color)	{m_clrText = color;}
	void SetBkColor(COLORREF color)		 {m_clrBkGround = color;}
	void SetStartColor(COLORREF color)	{m_clrStart = color;}
	void SetEndColor(COLORREF color)	{m_clrEnd = color;}

	// Show the percent caption
	void ShowPercent(BOOL bShowPercent = TRUE)	{m_bShowPercent = bShowPercent;}
	
	// Get Functions
	COLORREF GetTextColor(void)	{return m_clrText;}
	COLORREF GetBkColor(void)		 {return m_clrBkGround;}
	COLORREF GetStartColor(void)	{return m_clrStart;}
	COLORREF GetEndColor(void)	{return m_clrEnd;}


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGradientProgressCtrl)
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGradientProgressCtrl();

	// Generated message map functions
protected:
	void DrawGradient(CPaintDC *pDC, const RECT &rectClient, const int &nMaxWidth, const BOOL &bVertical);	
	int m_nLower, m_nUpper, m_nStep, m_nCurrentPosition;
	COLORREF	m_clrStart, m_clrEnd, m_clrBkGround, m_clrText;
	BOOL m_bShowPercent;
	//{{AFX_MSG(CGradientProgressCtrl)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ENHPROGRESSCTRL_H__12909D73_C393_11D1_9FAE_8192554015AD__INCLUDED_)
