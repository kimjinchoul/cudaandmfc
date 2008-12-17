// TestDialogDlg.h : 头文件
//

#pragma once
//#include "RenderWnd.h"
#include "CUDAD3DDisplay.h"
#include "afxwin.h"


// CTestDialogDlg 对话框
class CTestDialogDlg : public CDialog
{
// 构造
public:
	CTestDialogDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TESTDIALOG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CCUDAD3DDisplay m_cddDisplay;
	afx_msg void OnBnClickedOk();
	CStatic m_stcResult;
	CStatic m_wndDisplayer;
	float m_fTime;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
