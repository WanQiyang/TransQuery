#pragma once


// CTQTab_3 对话框

class CTQTab_3 : public CDialogEx
{
	DECLARE_DYNAMIC(CTQTab_3)

public:
	CTQTab_3(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTQTab_3();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_TAB_3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
