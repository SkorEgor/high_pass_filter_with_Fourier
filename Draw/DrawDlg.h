
// DrawDlg.h : ���� ���������
//

#pragma once
#include "Drawer.h"

// ���������� ���� CDrawDlg
class CDrawDlg : public CDialogEx
{
// ��������
public:
	CDrawDlg(CWnd* pParent = NULL);	// ����������� �����������

// ������ ����������� ����
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DRAW_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// ��������� DDX/DDV


// ����������
protected:
	HICON m_hIcon;

	// ��������� ������� ����� ���������
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	Drawer2 drv;
	Drawer drv2;
	Drawer drv3;
	Drawer2 drv4;
	afx_msg void OnStnClickedPicture();
	double A1;
	double f1;
	double phase1;
	double A2;
	double f2;
	double phase2;
	double A3;
	double f3;
	double phase3;
	double fd;
	double N;
	afx_msg void OnEnChangeEdit10();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnEnChangeEdit3();
	afx_msg void OnEnChangeEdit11();
	afx_msg void OnEnChangeEdit4();
	afx_msg void OnEnChangeEdit5();
	afx_msg void OnEnChangeEdit6();
	afx_msg void OnEnChangeEdit7();
	afx_msg void OnEnChangeEdit8();
	afx_msg void OnEnChangeEdit9();
	afx_msg void OnStnClickedPicture2();
	afx_msg void OnEnChangeEdit12();
	float inputPercentageNoise;
	afx_msg void OnEnChangeEdit13();
	float inputPercentageCleaning;
	afx_msg void OnStnClickedPicture3();
	double deviation;
	afx_msg void OnEnChangeEdit14();
	afx_msg void OnStnClickedPicture4();
};
