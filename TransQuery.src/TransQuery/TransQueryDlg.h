
// TransQueryDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "TQTab_1.h"
#include "TQTab_2.h"
#include "TQTab_3.h"

// CTQDlg 对话框
class CTQDlg : public CDialogEx
{
// 构造
public:
	CTQDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TRANSQUERY_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()

public:
	CTabCtrl m_Tab;
	CTQTab_1 m_Tab_1;
	CTQTab_2 m_Tab_2;
	CTQTab_3 m_Tab_3;
	
	CTQCore::Timetable* TQ_Train;
	CTQCore::Timetable* TQ_Plane;

	void InitDlg();
	afx_msg void OnDestroy();
};
