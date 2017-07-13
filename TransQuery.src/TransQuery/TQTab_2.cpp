// TQTab_2.cpp : 实现文件
//

#include "stdafx.h"
#include "TransQuery.h"
#include "TQTab_2.h"
#include "afxdialogex.h"


// CTQTab_2 对话框

IMPLEMENT_DYNAMIC(CTQTab_2, CDialogEx)

CTQTab_2::CTQTab_2(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_TAB_2, pParent)
{

}

CTQTab_2::~CTQTab_2()
{
}

void CTQTab_2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_SRC, m_Combo_Src);
	DDX_Control(pDX, IDC_COMBO_DEST, m_Combo_Dest);
	DDX_Control(pDX, IDC_COMBO_METHOD, m_Combo_Method);
	DDX_Control(pDX, IDC_BUTTON_SEARCH, m_Button_Search);
	DDX_Control(pDX, IDC_LIST_RESULT, m_List_Result);
	DDX_Control(pDX, IDC_BUTTON_DEL, m_Button_Del);
	DDX_Control(pDX, IDC_BUTTON_ADD, m_Button_Add);
	DDX_Control(pDX, IDC_BUTTON_DELCITY, m_Button_DelCity);
	DDX_Control(pDX, IDC_COMBO_DELCITY, m_Combo_DelCity);
}


BEGIN_MESSAGE_MAP(CTQTab_2, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, &CTQTab_2::OnBnClickedButtonSearch)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CTQTab_2::OnBnClickedButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CTQTab_2::OnBnClickedButtonAdd)
	ON_CBN_SELCHANGE(IDC_COMBO_METHOD, &CTQTab_2::OnCbnSelchangeComboMethod)
	ON_BN_CLICKED(IDC_BUTTON_DELCITY, &CTQTab_2::OnBnClickedButtonDelcity)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CTQTab_2 消息处理程序


BOOL CTQTab_2::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitTab();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CTQTab_2::InitTab()
{
	InitCityCombo(m_Combo_Src, TQ_Train);
	InitCityCombo(m_Combo_Dest, TQ_Train);
	InitCityCombo(m_Combo_DelCity, TQ_Train);
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
	m_Combo_Dest.SetCurSel(2);
	m_Combo_Method.SetCurSel(0);
	m_Combo_DelCity.SetCurSel(0);

	m_Add.TQ_Train = TQ_Train;
	m_Add.TQ_Plane = TQ_Plane;

	m_Add.Create(IDD_DIALOG_ADD);
	m_Add.ShowWindow(false);
}

void CTQTab_2::InitCityCombo(CComboBox &m_Combo, CTQCore::Timetable* T)
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

void CTQTab_2::OnBnClickedButtonSearch()
{
	// TODO: 在此添加控件通知处理程序代码
	CTQCore::Timetable* TQ = (m_Combo_Method.GetCurSel() == 0) ? TQ_Train : TQ_Plane;
	m_List_Result.DeleteAllItems();
	for (unsigned int i = 0; i < TQ->GetLineCount(); i++) {
		CTQCore::Line l_Temp = TQ->GetLine(i);
		if (l_Temp.src == TQ->GetCity(m_Combo_Src.GetCurSel()) && l_Temp.dest == TQ->GetCity(m_Combo_Dest.GetCurSel())) {
			unsigned int index = m_List_Result.GetItemCount();
			CString s_Temp;
			s_Temp.Format(_T("%d"), i);
			m_List_Result.InsertItem(index, s_Temp);
			m_List_Result.SetItemText(index, 1, (LPCTSTR)l_Temp.name.c_str());
			m_List_Result.SetItemText(index, 2, (LPCTSTR)TQ->GetCityName(l_Temp.src).c_str());
			m_List_Result.SetItemText(index, 3, (LPCTSTR)CTQCore::uint_to_time(l_Temp.srcTime).c_str());
			m_List_Result.SetItemText(index, 4, (LPCTSTR)TQ->GetCityName(l_Temp.dest).c_str());
			m_List_Result.SetItemText(index, 5, (LPCTSTR)CTQCore::uint_to_time(l_Temp.destTime).c_str());
			m_List_Result.SetItemText(index, 6, (LPCTSTR)CTQCore::int_to_period(l_Temp.period).c_str());
			s_Temp.Format(_T("%.1lf"), l_Temp.price);
			m_List_Result.SetItemText(index, 7, s_Temp);
		}
	}
	if (m_List_Result.GetItemCount() > 0) {
		m_List_Result.SetFocus();
		m_List_Result.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
	}
		
}


void CTQTab_2::OnBnClickedButtonDel()
{
	// TODO: 在此添加控件通知处理程序代码
	CTQCore::Timetable* TQ = (m_Combo_Method.GetCurSel() == 0) ? TQ_Train : TQ_Plane;
	CString s_i;
	POSITION p_Index = m_List_Result.GetFirstSelectedItemPosition();
	if (!p_Index)
		return;
	unsigned int u_Index = m_List_Result.GetNextSelectedItem(p_Index);
	s_i = m_List_Result.GetItemText(u_Index, 0);
	TQ->DeleteLine(_ttoi(s_i));
	TQ->Refresh();
	OnBnClickedButtonSearch();
}


void CTQTab_2::OnBnClickedButtonAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Add.ShowWindow(true);
}


void CTQTab_2::OnCbnSelchangeComboMethod()
{
	// TODO: 在此添加控件通知处理程序代码
	switch (m_Combo_Method.GetCurSel()) {
	case 0:
		InitCityCombo(m_Combo_Src, TQ_Train);
		InitCityCombo(m_Combo_Dest, TQ_Train);
		InitCityCombo(m_Combo_DelCity, TQ_Train);
		break;
	case 1:
		InitCityCombo(m_Combo_Src, TQ_Plane);
		InitCityCombo(m_Combo_Dest, TQ_Plane);
		InitCityCombo(m_Combo_DelCity, TQ_Plane);
		break;
	}
	m_Combo_Src.SetCurSel(0);
	m_Combo_Dest.SetCurSel(2);
	m_Combo_DelCity.SetCurSel(0);
}


void CTQTab_2::OnBnClickedButtonDelcity()
{
	// TODO: 在此添加控件通知处理程序代码
	CTQCore::Timetable* TQ = (m_Combo_Method.GetCurSel() == 0) ? TQ_Train : TQ_Plane;
	TQ->DeleteCity(TQ->GetCity(m_Combo_DelCity.GetCurSel()));
	TQ->Refresh();
	OnCbnSelchangeComboMethod();
}


void CTQTab_2::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 在此处添加消息处理程序代码
	OnCbnSelchangeComboMethod();
}
