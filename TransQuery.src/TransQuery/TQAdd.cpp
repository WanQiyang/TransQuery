// TQAdd.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TransQuery.h"
#include "TQAdd.h"
#include "afxdialogex.h"


// CTQAdd �Ի���

IMPLEMENT_DYNAMIC(CTQAdd, CDialogEx)

CTQAdd::CTQAdd(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_ADD, pParent)
{

}

CTQAdd::~CTQAdd()
{
}

void CTQAdd::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_METHOD, m_Combo_Method);
	DDX_Control(pDX, IDC_COMBO_SRC, m_Combo_Src);
	DDX_Control(pDX, IDC_COMBO_DEST, m_Combo_Dest);
	DDX_Control(pDX, IDC_EDIT_NAME, m_Edit_Name);
	DDX_Control(pDX, IDC_EDIT_SRCTIME, m_Edit_SrcTime);
	DDX_Control(pDX, IDC_EDIT_DESTTIME, m_Edit_DestTime);
	DDX_Control(pDX, IDC_EDIT_PERIOD, m_Edit_Period);
	DDX_Control(pDX, IDC_EDIT_PRICE, m_Edit_Price);
}


BEGIN_MESSAGE_MAP(CTQAdd, CDialogEx)
	ON_BN_CLICKED(IDCANCEL, &CTQAdd::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CTQAdd::OnBnClickedOk)
END_MESSAGE_MAP()


// CTQAdd ��Ϣ�������


BOOL CTQAdd::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	InitDlg();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

void CTQAdd::InitDlg()
{
	CRect r_Dlg;
	GetClientRect(r_Dlg);
	int i_ScreenWidth = GetSystemMetrics(SM_CXSCREEN), i_ScreenHeight = GetSystemMetrics(SM_CYSCREEN);
	SetWindowPos(NULL, (i_ScreenWidth - r_Dlg.right) / 2, (i_ScreenHeight - r_Dlg.bottom) / 2, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	InitCityCombo(m_Combo_Src);
	InitCityCombo(m_Combo_Dest);
	m_Combo_Method.AddString(_T("��"));
	m_Combo_Method.AddString(_T("�ɻ�"));
	m_Edit_Name.SetWindowText(_T("G1"));
	m_Edit_SrcTime.SetWindowText(_T("08:00"));
	m_Edit_DestTime.SetWindowText(_T("09:00"));
	m_Edit_Period.SetWindowText(_T("01:00"));
	m_Edit_Price.SetWindowText(_T("0.0"));
	m_Combo_Src.SetCurSel(0);
	m_Combo_Dest.SetCurSel(1);
	m_Combo_Method.SetCurSel(0);

}

void CTQAdd::InitCityCombo(CComboBox & m_Combo)
{
	CTQCore::Timetable* TQ = TQ_Train;
	for (unsigned int i = 0; i < TQ->GetCityCount(); i++) {
		CString str;
		str += (LPCTSTR)TQ->GetCityName(TQ->GetCity(i)).c_str();
		str += _T(" ");
		str += (LPCTSTR)TQ->GetCity(i).c_str();
		m_Combo.AddString(str);
	}
	
}

bool CTQAdd::CheckTime(const CString & s_Time)
{
	if (s_Time.GetLength() != 5)
		return false;
	if (_ttoi(s_Time.Left(2)) < 0 || _ttoi(s_Time.Left(2)) > 23)
		return false;
	if (_ttoi(s_Time.Right(2)) < 0 || _ttoi(s_Time.Right(2)) > 59)
		return false;

	return true;
}

bool CTQAdd::CheckPeriod(const CString & s_Period)
{
	if (s_Period.GetLength() < 4)
		return false;
	int i_colon = s_Period.Find(_T(":"));
	if (i_colon < 0)
		return false;
	if (_ttoi(s_Period.Right(s_Period.GetLength() - i_colon - 1)) < 0 || _ttoi(s_Period.Right(s_Period.GetLength() - i_colon - 1)) > 59)
		return false;

	return true;
}

void CTQAdd::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}


void CTQAdd::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString s_Name, s_SrcTime, s_DestTime, s_Period, s_Price;

	m_Edit_Name.GetWindowText(s_Name);

	m_Edit_SrcTime.GetWindowText(s_SrcTime);
	if (!CheckTime(s_SrcTime)) {
		MessageBox(_T("����ʱ���ʽ�Ƿ���"), _T("����"), MB_ICONEXCLAMATION | MB_OK);
		return;
	}

	m_Edit_DestTime.GetWindowText(s_DestTime);
	if (!CheckTime(s_DestTime)) {
		MessageBox(_T("����ʱ���ʽ�Ƿ���"), _T("����"), MB_ICONEXCLAMATION | MB_OK);
		return;
	}

	m_Edit_Period.GetWindowText(s_Period);
	if (!CheckPeriod(s_Period)) {
		MessageBox(_T("��ʱ��ʽ�Ƿ���"), _T("����"), MB_ICONEXCLAMATION | MB_OK);
		return;
	}

	m_Edit_Price.GetWindowText(s_Price);
	if (_ttof(s_Price)<0) {
		MessageBox(_T("���÷Ƿ���"), _T("����"), MB_ICONEXCLAMATION | MB_OK);
		return;
	}

	CTQCore::Timetable* TQ = (m_Combo_Method.GetCurSel() == 0) ? TQ_Train : TQ_Plane;

	CTQCore::Line l_Temp;
	l_Temp.name = s_Name.GetString();
	l_Temp.src = TQ->GetCity(m_Combo_Src.GetCurSel());
	l_Temp.srcTime = CTQCore::time_to_uint(s_SrcTime.GetString());
	l_Temp.dest = TQ->GetCity(m_Combo_Dest.GetCurSel());
	l_Temp.destTime = CTQCore::time_to_uint(s_DestTime.GetString());
	l_Temp.period = CTQCore::period_to_int(s_Period.GetString());
	l_Temp.price = _ttof(s_Price);

	TQ->AddLine(l_Temp);
	TQ->Refresh();

	CDialogEx::OnOK();
}
