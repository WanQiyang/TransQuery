#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CTQTab_1 对话框

class CTQTab_1 : public CDialogEx
{
	DECLARE_DYNAMIC(CTQTab_1)

public:
	CTQTab_1(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTQTab_1();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_TAB_1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadio();
	afx_msg void OnDeltaposSpinH(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinM(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonSearch();
	afx_msg void OnCbnSelchangeComboMethod();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	DECLARE_MESSAGE_MAP()

public:
	CComboBox m_Combo_Src;
	CComboBox m_Combo_Dest;
	CEdit m_Edit_H;
	CEdit m_Edit_M;
	CSpinButtonCtrl m_Spin_H;
	CSpinButtonCtrl m_Spin_M;
	CComboBox m_Combo_Method;
	CButton m_Button_Search;
	CListCtrl m_List_Result;
	CStatic m_Static_Result;
	int m_Radio;

	CTQCore::Timetable* TQ_Train;
	CTQCore::Timetable* TQ_Plane;

	void InitTab();
	void InitCityCombo(CComboBox &m_Combo, CTQCore::Timetable* T);
	
};
