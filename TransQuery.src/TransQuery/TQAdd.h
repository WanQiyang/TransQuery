#pragma once
#include "afxwin.h"

// CTQAdd 对话框

class CTQAdd : public CDialogEx
{
	DECLARE_DYNAMIC(CTQAdd)

public:
	CTQAdd(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTQAdd();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ADD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	DECLARE_MESSAGE_MAP()

public:
	CComboBox m_Combo_Method;
	CComboBox m_Combo_Src;
	CComboBox m_Combo_Dest;
	CEdit m_Edit_Name;
	CEdit m_Edit_SrcTime;
	CEdit m_Edit_DestTime;
	CEdit m_Edit_Period;
	CEdit m_Edit_Price;

	CTQCore::Timetable* TQ_Train;
	CTQCore::Timetable* TQ_Plane;

	void InitDlg();
	void InitCityCombo(CComboBox & m_Combo);
	bool CheckTime(const CString & s_Time);
	bool CheckPeriod(const CString & s_Period);

};
