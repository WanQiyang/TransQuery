#pragma once


// CTQTab_3 �Ի���

class CTQTab_3 : public CDialogEx
{
	DECLARE_DYNAMIC(CTQTab_3)

public:
	CTQTab_3(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTQTab_3();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_TAB_3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
