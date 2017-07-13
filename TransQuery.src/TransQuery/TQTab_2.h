#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "TQAdd.h"

// CTQTab_2 对话框

class CTQTab_2 : public CDialogEx
{
	DECLARE_DYNAMIC(CTQTab_2)

public:
	CTQTab_2(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTQTab_2();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_TAB_2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonDel();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonSearch();
	afx_msg void OnCbnSelchangeComboMethod();
	afx_msg void OnBnClickedButtonDelcity();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	DECLARE_MESSAGE_MAP()

public:
	CComboBox m_Combo_Src;
	CComboBox m_Combo_Dest;
	CComboBox m_Combo_Method;
	CButton m_Button_Search;
	CListCtrl m_List_Result;
	CButton m_Button_Del;
	CButton m_Button_Add;
	CButton m_Button_DelCity;
	CComboBox m_Combo_DelCity;

	CTQAdd m_Add;

	CTQCore::Timetable* TQ_Train;
	CTQCore::Timetable* TQ_Plane;

	void InitTab();
	void InitCityCombo(CComboBox &m_Combo, CTQCore::Timetable* T);
	
};
