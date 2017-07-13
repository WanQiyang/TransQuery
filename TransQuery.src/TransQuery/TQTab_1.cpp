// TQTab_1.cpp : 实现文件
//

#include "stdafx.h"
#include "TransQuery.h"
#include "TQTab_1.h"
#include "afxdialogex.h"


// CTQTab_1 对话框

IMPLEMENT_DYNAMIC(CTQTab_1, CDialogEx)

CTQTab_1::CTQTab_1(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_TAB_1, pParent)
	, m_Radio(0)
{

}

CTQTab_1::~CTQTab_1()
{
}

void CTQTab_1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_SRC, m_Combo_Src);
	DDX_Control(pDX, IDC_COMBO_DEST, m_Combo_Dest);
	DDX_Control(pDX, IDC_EDIT_H, m_Edit_H);
	DDX_Control(pDX, IDC_EDIT_M, m_Edit_M);
	DDX_Control(pDX, IDC_SPIN_H, m_Spin_H);
	DDX_Control(pDX, IDC_SPIN_M, m_Spin_M);
	DDX_Control(pDX, IDC_COMBO_METHOD, m_Combo_Method);
	DDX_Radio(pDX, IDC_RADIO_PRICE, m_Radio);
	DDX_Control(pDX, IDC_BUTTON_SEARCH, m_Button_Search);
	DDX_Control(pDX, IDC_LIST_RESULT, m_List_Result);
	DDX_Control(pDX, IDC_STATIC_RESULT, m_Static_Result);
}


BEGIN_MESSAGE_MAP(CTQTab_1, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO_PRICE, &CTQTab_1::OnBnClickedRadio)
	ON_BN_CLICKED(IDC_RADIO_TIME, &CTQTab_1::OnBnClickedRadio)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_H, &CTQTab_1::OnDeltaposSpinH)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_M, &CTQTab_1::OnDeltaposSpinM)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, &CTQTab_1::OnBnClickedButtonSearch)
	ON_CBN_SELCHANGE(IDC_COMBO_METHOD, &CTQTab_1::OnCbnSelchangeComboMethod)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CTQTab_1 消息处理程序


BOOL CTQTab_1::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitTab();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CTQTab_1::InitTab()
{
	InitCityCombo(m_Combo_Src, TQ_Train);
	InitCityCombo(m_Combo_Dest, TQ_Train);
	m_Combo_Method.AddString(_T("火车"));
	m_Combo_Method.AddString(_T("飞机"));
	CRect r_Temp;
	m_List_Result.GetClientRect(r_Temp);
	unsigned int u_Width = r_Temp.right / 416;
	m_List_Result.InsertColumn(0, _T("序号"), LVCFMT_CENTER, 36 * u_Width);
	m_List_Result.InsertColumn(1, _T("班次"), LVCFMT_CENTER, 50 * u_Width);
	m_List_Result.InsertColumn(2, _T("出发地"), LVCFMT_CENTER, 60 * u_Width);
	m_List_Result.InsertColumn(3, _T("出发时间"), LVCFMT_CENTER, 60 * u_Width);
	m_List_Result.InsertColumn(4, _T("到达地"), LVCFMT_CENTER, 60 * u_Width);
	m_List_Result.InsertColumn(5, _T("到达时间"), LVCFMT_CENTER, 60 * u_Width);
	m_List_Result.InsertColumn(6, _T("历时"), LVCFMT_CENTER, 50 * u_Width);
	m_List_Result.InsertColumn(7, _T("费用"), LVCFMT_CENTER, 40 * u_Width);

	m_Combo_Src.SetCurSel(0);
	m_Combo_Dest.SetCurSel(1);
	m_Edit_H.SetWindowText(_T("08"));
	m_Edit_M.SetWindowText(_T("00"));
	m_Combo_Method.SetCurSel(0);
	m_Radio = 0;
	UpdateData(FALSE);
}

void CTQTab_1::InitCityCombo(CComboBox & m_Combo, CTQCore::Timetable* T)
{
	CTQCore::Timetable* TQ = T;
	m_Combo.ResetContent();
	for (unsigned int i = 0; i < TQ->GetCityCount(); i++) {
		CString str;
		str += (LPCTSTR)TQ->GetCityName(TQ->GetCity(i)).c_str();
		str += _T(" ");
		str += (LPCTSTR)TQ->GetCity(i).c_str();
		m_Combo.AddString(str);
	}
		
}


void CTQTab_1::OnBnClickedRadio()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
}

void CTQTab_1::OnDeltaposSpinH(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	CString s_H;
	m_Edit_H.GetWindowText(s_H);
	int i_H = _ttoi(s_H);
	s_H.Format(_T("%02d"), (i_H - pNMUpDown->iDelta + 24) % 24);
	m_Edit_H.SetWindowText(s_H);
	*pResult = 0;
}

void CTQTab_1::OnDeltaposSpinM(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	CString s_M;
	m_Edit_M.GetWindowText(s_M);
	int i_M = _ttoi(s_M);
	s_M.Format(_T("%02d"), (i_M - pNMUpDown->iDelta * 10 + 60) % 60);
	m_Edit_M.SetWindowText(s_M);
	*pResult = 0;
}

void CTQTab_1::OnBnClickedButtonSearch()
{
	// TODO: 在此添加控件通知处理程序代码
	CTQCore::Timetable* TQ = (m_Combo_Method.GetCurSel() == 0) ? TQ_Train : TQ_Plane;
	m_List_Result.DeleteAllItems();
	CTQCore::Path p_Result;
	CTQCore::Timetable::QueryType Type = (m_Radio == 0) ? CTQCore::Timetable::QT_PRICE : CTQCore::Timetable::QT_TIME;
	CString s_Src(TQ->GetCity(m_Combo_Src.GetCurSel()).c_str()), s_Dest(TQ->GetCity(m_Combo_Dest.GetCurSel()).c_str()), s_Now, s_H, s_M;
	m_Edit_H.GetWindowText(s_H);
	m_Edit_M.GetWindowText(s_M);
	s_Now = s_H + _T(":") + s_M;
	p_Result = TQ->Query(s_Src.GetString(), s_Dest.GetString(), CTQCore::time_to_uint(s_Now.GetString()), Type);
	if (p_Result.size() == 0) {
		m_Static_Result.SetWindowText(_T("查询结果 - 不存在满足要求的线路"));
		MessageBox(_T("不存在满足要求的线路！"), _T("提示"), MB_ICONINFORMATION | MB_OK);
		return;
	}
	unsigned int u_LastTime = CTQCore::time_to_uint(s_Now.GetString());
	double d_Price = 0;
	int i_Period = 0;
	for (unsigned int i = 0; i < p_Result.size(); i++) {
		CTQCore::Line l_Temp = TQ->GetLine(p_Result[i]);
		CString s_Temp;
		s_Temp.Format(_T("%d"), i);
		m_List_Result.InsertItem(i, s_Temp);
		m_List_Result.SetItemText(i, 1, (LPCTSTR)l_Temp.name.c_str());
		m_List_Result.SetItemText(i, 2, (LPCTSTR)TQ->GetCityName(l_Temp.src).c_str());
		m_List_Result.SetItemText(i, 3, (LPCTSTR)CTQCore::uint_to_time(l_Temp.srcTime).c_str());
		m_List_Result.SetItemText(i, 4, (LPCTSTR)TQ->GetCityName(l_Temp.dest).c_str());
		m_List_Result.SetItemText(i, 5, (LPCTSTR)CTQCore::uint_to_time(l_Temp.destTime).c_str());
		m_List_Result.SetItemText(i, 6, (LPCTSTR)CTQCore::int_to_period(l_Temp.period).c_str());
		s_Temp.Format(_T("%.1lf"), l_Temp.price);
		m_List_Result.SetItemText(i, 7, s_Temp);
		i_Period += CTQCore::time_to_period(u_LastTime, l_Temp.srcTime) + l_Temp.period;
		u_LastTime = l_Temp.destTime;
		d_Price += l_Temp.price;
	}
	CString s_Result;
	s_Result.Format(_T("查询结果 - 费用 %.1lf  历时 %s"), d_Price, CTQCore::int_to_period(i_Period).c_str());
	m_Static_Result.SetWindowText(s_Result);
}


void CTQTab_1::OnCbnSelchangeComboMethod()
{
	// TODO: 在此添加控件通知处理程序代码
	switch (m_Combo_Method.GetCurSel()) {
	case 0:
		InitCityCombo(m_Combo_Src, TQ_Train);
		InitCityCombo(m_Combo_Dest, TQ_Train);
		break;
	case 1:
		InitCityCombo(m_Combo_Src, TQ_Plane);
		InitCityCombo(m_Combo_Dest, TQ_Plane);
		break;
	}
	m_Combo_Src.SetCurSel(0);
	m_Combo_Dest.SetCurSel(1);
}


void CTQTab_1::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 在此处添加消息处理程序代码
	OnCbnSelchangeComboMethod();
}
