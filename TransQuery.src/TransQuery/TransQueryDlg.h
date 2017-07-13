
// TransQueryDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "TQTab_1.h"
#include "TQTab_2.h"
#include "TQTab_3.h"

// CTQDlg �Ի���
class CTQDlg : public CDialogEx
{
// ����
public:
	CTQDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TRANSQUERY_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()

public:
	CTabCtrl m_Tab;
	CTQTab_1 m_Tab_1;
	CTQTab_2 m_Tab_2;
	CTQTab_3 m_Tab_3;
	
	CTQCore::Timetable* TQ_Train;
	CTQCore::Timetable* TQ_Plane;

	void InitDlg();
	afx_msg void OnDestroy();
};
