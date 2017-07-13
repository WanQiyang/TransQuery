
// TransQueryDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TransQuery.h"
#include "TransQueryDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTQDlg �Ի���



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


// CTQDlg ��Ϣ�������

BOOL CTQDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	InitDlg();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CTQDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CTQDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTQDlg::InitDlg()
{
	TQ_Train = new CTQCore::Timetable;
	if (!TQ_Train) {
		MessageBox(_T("��ʼ������"), _T("����"), MB_ICONEXCLAMATION | MB_OK);
		CDialogEx::OnOK();
	}
	if (TQ_Train->LoadData(_T("./Train.data")) != 0) {
		MessageBox(_T("�����ݶ�ȡ������ȷ�������ļ�Train.data���ڣ�"), _T("����"), MB_ICONEXCLAMATION | MB_OK);
		CDialogEx::OnOK();
	}
	TQ_Train->Refresh();
	m_Tab_1.TQ_Train = m_Tab_2.TQ_Train = TQ_Train;

	TQ_Plane = new CTQCore::Timetable;
	if (!TQ_Plane) {
		MessageBox(_T("��ʼ������"), _T("����"), MB_ICONEXCLAMATION | MB_OK);
		CDialogEx::OnOK();
	}
	if (TQ_Plane->LoadData(_T("./Plane.data")) != 0) {
		MessageBox(_T("�ɻ����ݶ�ȡ������ȷ�������ļ�Plane.data���ڣ�"), _T("����"), MB_ICONEXCLAMATION | MB_OK);
		CDialogEx::OnOK();
	}
	TQ_Plane->Refresh();
	m_Tab_1.TQ_Plane = m_Tab_2.TQ_Plane = TQ_Plane;

	m_Tab.InsertItem(0, _T("��ͨ��ѯ"));
	m_Tab.InsertItem(1, _T("�༭ʱ�̱�"));
	m_Tab.InsertItem(2, _T("����"));
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	
	// TODO: �ڴ˴������Ϣ����������
	if (TQ_Train) delete TQ_Train;
	if (TQ_Plane) delete TQ_Plane;
}
