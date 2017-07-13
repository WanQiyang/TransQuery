
// TransQueryDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TransQuery.h"
#include "TransQueryDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTQDlg 对话框



CTQDlg::CTQDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TRANSQUERY_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTQDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB, m_Tab);
}

BEGIN_MESSAGE_MAP(CTQDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, &CTQDlg::OnTcnSelchangeTab)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CTQDlg 消息处理程序

BOOL CTQDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	InitDlg();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTQDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTQDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTQDlg::InitDlg()
{
	TQ_Train = new CTQCore::Timetable;
	if (!TQ_Train) {
		MessageBox(_T("初始化错误！"), _T("错误"), MB_ICONEXCLAMATION | MB_OK);
		CDialogEx::OnOK();
	}
	if (TQ_Train->LoadData(_T("./Train.data")) != 0) {
		MessageBox(_T("火车数据读取错误，请确认数据文件Train.data存在！"), _T("错误"), MB_ICONEXCLAMATION | MB_OK);
		CDialogEx::OnOK();
	}
	TQ_Train->Refresh();
	m_Tab_1.TQ_Train = m_Tab_2.TQ_Train = TQ_Train;

	TQ_Plane = new CTQCore::Timetable;
	if (!TQ_Plane) {
		MessageBox(_T("初始化错误！"), _T("错误"), MB_ICONEXCLAMATION | MB_OK);
		CDialogEx::OnOK();
	}
	if (TQ_Plane->LoadData(_T("./Plane.data")) != 0) {
		MessageBox(_T("飞机数据读取错误，请确认数据文件Plane.data存在！"), _T("错误"), MB_ICONEXCLAMATION | MB_OK);
		CDialogEx::OnOK();
	}
	TQ_Plane->Refresh();
	m_Tab_1.TQ_Plane = m_Tab_2.TQ_Plane = TQ_Plane;

	m_Tab.InsertItem(0, _T("交通咨询"));
	m_Tab.InsertItem(1, _T("编辑时刻表"));
	m_Tab.InsertItem(2, _T("关于"));
	m_Tab_1.Create(IDD_DIALOG_TAB_1, GetDlgItem(IDC_TAB));
	m_Tab_2.Create(IDD_DIALOG_TAB_2, GetDlgItem(IDC_TAB));
	m_Tab_3.Create(IDD_DIALOG_TAB_3, GetDlgItem(IDC_TAB));

	CRect tabRect, childRect;
	/*
	m_Tab_1.GetClientRect(&childRect);
	m_Tab_1.MoveWindow(&childRect);
	m_Tab_2.GetClientRect(&childRect);
	m_Tab_2.MoveWindow(&childRect);
	m_Tab_3.GetClientRect(&childRect);
	m_Tab_3.MoveWindow(&childRect);
	*/
	m_Tab_1.GetWindowRect(&childRect);
	m_Tab.SetWindowPos(NULL, 0, 0, childRect.right - childRect.left + 1, childRect.bottom - childRect.top + 31, SWP_NOMOVE | SWP_NOZORDER);
	m_Tab.GetWindowRect(&tabRect);
	this->SetWindowPos(NULL, 0, 0, tabRect.right + tabRect.left, tabRect.bottom + tabRect.top, SWP_NOMOVE | SWP_NOZORDER);

	m_Tab.SetItemSize({ (tabRect.right - tabRect.left - 6) / 3 , 30 });
	m_Tab.SetPadding({ 3, 3 });

	m_Tab_1.ShowWindow(true);
	m_Tab_2.ShowWindow(false);
	m_Tab_3.ShowWindow(false);

	m_Tab.SetCurSel(0);

}



void CTQDlg::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	const int CurSel = m_Tab.GetCurSel();
	switch (CurSel) {
	case 0:
		m_Tab_1.ShowWindow(true);
		m_Tab_2.ShowWindow(false);
		m_Tab_3.ShowWindow(false);
		break;

	case 1:
		m_Tab_1.ShowWindow(false);
		m_Tab_2.ShowWindow(true);
		m_Tab_3.ShowWindow(false);
		break;

	case 2:
		m_Tab_1.ShowWindow(false);
		m_Tab_2.ShowWindow(false);
		m_Tab_3.ShowWindow(true);
		break;
	}
	*pResult = 0;
}


void CTQDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	
	// TODO: 在此处添加消息处理程序代码
	if (TQ_Train) delete TQ_Train;
	if (TQ_Plane) delete TQ_Plane;
}
