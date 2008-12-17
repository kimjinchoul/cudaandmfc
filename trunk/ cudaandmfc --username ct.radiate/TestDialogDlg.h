// TestDialogDlg.h : ͷ�ļ�
//

#pragma once
//#include "RenderWnd.h"
#include "CUDAD3DDisplay.h"
#include "afxwin.h"


// CTestDialogDlg �Ի���
class CTestDialogDlg : public CDialog
{
// ����
public:
	CTestDialogDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TESTDIALOG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
