//////////////////////////////////////////////////////////////////////////////
//类名：CCPUDemoDlg
//功能：CPU利用率组件示例程序(客户端)
//作者：徐景周(johnny Xu, xujingzhou2016@gmail.com)
//组织：未来工作室(Future Studio)
//日期：2003.1.1
/////////////////////////////////////////////////////////////////////////////
#if !defined(AFX_CPUDEMODLG_H__1E013B27_3301_11D7_AA35_EA02DB2FF40A__INCLUDED_)
#define AFX_CPUDEMODLG_H__1E013B27_3301_11D7_AA35_EA02DB2FF40A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WinXPButtonST.h"			// XP风格按钮
#include "HyperLink.h"				// 加入超链接类
#include "PictureEx.h"				// 加入GIF动态图像显示
#include "GradientProgressCtrl.h"   // 渐变色进度条
/////////////////////////////////////////////////////////////////////////////
// CCPUDemoDlg dialog
class CCPUDemoDlg : public CDialog
{
// Construction
public:
	CCPUDemoDlg(CWnd* pParent = NULL);	// standard constructor

	CString m_strSystemType;			// 当前操作系统版本

// Dialog Data
	//{{AFX_DATA(CCPUDemoDlg)
	enum { IDD = IDD_CPUDEMO_DIALOG };
	CStatic	m_SystemType;
	CGradientProgressCtrl	m_Progress;
	CWinXPButtonST	m_OK;				// XP风格按钮
	CWinXPButtonST	m_Cancel;			// XP风格按钮
	CWinXPButtonST	m_About;			// XP风格按钮
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCPUDemoDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCPUDemoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnAbout();
	virtual void OnOK();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CPUDEMODLG_H__1E013B27_3301_11D7_AA35_EA02DB2FF40A__INCLUDED_)
