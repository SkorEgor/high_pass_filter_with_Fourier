
// DrawDlg.cpp : ���� ����������
//

#include "stdafx.h"
#include "Draw.h"
#include "DrawDlg.h"
#include "afxdialogex.h"
#include <vector>
#include <complex>
#include "FFT.h"


#define PI 3.1415926535897932384626433832795


#ifdef _DEBUG
#define new DEBUG_NEW
#endif
using namespace Signal;

//������� ������� �������
//���������� ����� ���������� �������, ������� ����� ��������
int NClear(const std::vector<double>& sp, double vost)
{
	double ESp = 0, ECl = 0;
	//������� ����� ������� �������
	for (int i = 0; i < sp.size(); i++)ESp += sp[i] * sp[i];
	int n = 0;
	ECl += sp[n] * sp[n];//������� ���������
	while (ECl < ESp * vost / 200.0)//����� �� 100% � 2 ��-�� ���������
	{
		n++;
		ECl += sp[n] * sp[n];
	}
	return n;
}



CDrawDlg::CDrawDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DRAW_DIALOG, pParent)
	, A1(1)
	, f1(10)
	, phase1(0)
	, A2(2)
	, f2(20)
	, phase2(0)
	, A3(3)
	, f3(30)
	, phase3(0)
	, fd(2048)
	, N(1024)
	, inputPercentageNoise(30)
	, inputPercentageCleaning(70)
	, deviation(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDrawDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, A1);
	DDX_Text(pDX, IDC_EDIT2, f1);
	DDX_Text(pDX, IDC_EDIT3, phase1);
	DDX_Text(pDX, IDC_EDIT4, A2);
	DDX_Text(pDX, IDC_EDIT5, f2);
	DDX_Text(pDX, IDC_EDIT6, phase2);
	DDX_Text(pDX, IDC_EDIT7, A3);
	DDX_Text(pDX, IDC_EDIT8, f3);
	DDX_Text(pDX, IDC_EDIT9, phase3);
	DDX_Text(pDX, IDC_EDIT10, fd);
	DDX_Text(pDX, IDC_EDIT11, N);
	DDX_Text(pDX, IDC_EDIT12, inputPercentageNoise);
	DDX_Text(pDX, IDC_EDIT13, inputPercentageCleaning);
	DDX_Text(pDX, IDC_EDIT14, deviation);
}

BEGIN_MESSAGE_MAP(CDrawDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CDrawDlg::OnBnClickedOk)
	ON_STN_CLICKED(IDC_PICTURE, &CDrawDlg::OnStnClickedPicture)
	ON_EN_CHANGE(IDC_EDIT10, &CDrawDlg::OnEnChangeEdit10)
	ON_EN_CHANGE(IDC_EDIT1, &CDrawDlg::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT2, &CDrawDlg::OnEnChangeEdit2)
	ON_EN_CHANGE(IDC_EDIT3, &CDrawDlg::OnEnChangeEdit3)
	ON_EN_CHANGE(IDC_EDIT11, &CDrawDlg::OnEnChangeEdit11)
	ON_EN_CHANGE(IDC_EDIT4, &CDrawDlg::OnEnChangeEdit4)
	ON_EN_CHANGE(IDC_EDIT5, &CDrawDlg::OnEnChangeEdit5)
	ON_EN_CHANGE(IDC_EDIT6, &CDrawDlg::OnEnChangeEdit6)
	ON_EN_CHANGE(IDC_EDIT7, &CDrawDlg::OnEnChangeEdit7)
	ON_EN_CHANGE(IDC_EDIT8, &CDrawDlg::OnEnChangeEdit8)
	ON_EN_CHANGE(IDC_EDIT9, &CDrawDlg::OnEnChangeEdit9)
	ON_STN_CLICKED(IDC_PICTURE2, &CDrawDlg::OnStnClickedPicture2)
	ON_EN_CHANGE(IDC_EDIT12, &CDrawDlg::OnEnChangeEdit12)
	ON_EN_CHANGE(IDC_EDIT13, &CDrawDlg::OnEnChangeEdit13)
	ON_STN_CLICKED(IDC_PICTURE3, &CDrawDlg::OnStnClickedPicture3)
	ON_EN_CHANGE(IDC_EDIT14, &CDrawDlg::OnEnChangeEdit14)
	ON_STN_CLICKED(IDC_PICTURE4, &CDrawDlg::OnStnClickedPicture4)
END_MESSAGE_MAP()


// ����������� ��������� CDrawDlg

BOOL CDrawDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ������ ������ ��� ����� ����������� ����.  ����� ������ ��� �������������,
	//  ���� ������� ���� ���������� �� �������� ����������
	SetIcon(m_hIcon, TRUE);			// ������� ������
	SetIcon(m_hIcon, FALSE);		// ������ ������

	drv.Create(GetDlgItem(IDC_PICTURE)->GetSafeHwnd());
	drv2.Create(GetDlgItem(IDC_PICTURE2)->GetSafeHwnd());
	drv3.Create(GetDlgItem(IDC_PICTURE3)->GetSafeHwnd());
	drv4.Create(GetDlgItem(IDC_PICTURE4)->GetSafeHwnd());

	return TRUE;  // ������� �������� TRUE, ���� ����� �� ������� �������� ����������
}

// ��� ���������� ������ ����������� � ���������� ���� ����� ��������������� ����������� ���� �����,
//  ����� ���������� ������.  ��� ���������� MFC, ������������ ������ ���������� ��� �������������,
//  ��� ������������� ����������� ������� ��������.

void CDrawDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �������� ���������� ��� ���������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ������������ ������ �� ������ ����������� ��������������
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		// ��������� ������
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		//drv.Draw();
		CDialogEx::OnPaint();
	}
}

// ������� �������� ��� ������� ��� ��������� ����������� ������� ��� �����������
//  ���������� ����.
HCURSOR CDrawDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDrawDlg::OnStnClickedPicture()
{
	// TODO: �������� ���� ��� ����������� �����������
}


/*			����� ���)						*/		
void CDrawDlg::OnBnClickedOk()
{
	vector<double> noise, signalwnoise;
	double ESignal = 0, ENoise = 0, noiseNormKoef;

	//������ �������� �������
	//����������  ��������� �������
	vector<double> list_t; double min_t = 0;  double max_t = N /fd;	
	vector<double> list_x; double min_x = -1; double max_x = 1;

	//����������� ���������������� ��������
	if (A1 >= A2 && A1 >= A3)	max_x = A1;
	if (A2 >= A1 && A2 >= A3)	max_x = A2;
	if (A3 >= A1 && A3 >= A2)	max_x = A3;
	min_x = -max_x;

	//���������� ��������� �������
	for (int i = 0; i < N; i++)	{
		list_t.push_back(i/fd);
		list_x.push_back(
			A1 * sin(2 * pi * f1 * list_t[i] + phase1) +
			A2 * sin(2 * pi * f2 * list_t[i] + phase2) +
			A3 * sin(2 * pi * f3 * list_t[i] + phase3) );

		if (min_x > list_x[i]) min_x = list_x[i];
		if (max_x < list_x[i]) max_x = list_x[i];

		ESignal += list_x[i] * list_x[i];					//������� �������
	}

	
						/*���*/
	//��� ���������� ����� ������������ ����� ������� ���, �� �������� � ����� ��������� ������������� �� ���� ��������
	//���������� ����� ��� � ��������� [-1, 1] � ������� ��� �������
	signalwnoise = list_x;
	for (int i = 0; i < N; i++)                                 
	{
		//������� gk ����� ������������� �� �����, �������� 12 ��� 
		noise.push_back(((double)rand() * 2.0 / (double)RAND_MAX) - 1);
		for (int j = 0; j < 11; j++)noise[i] += (((double)rand() * 2.0 / (double)RAND_MAX) - 1);
		noise[i] = noise[i] / 12.0;

		ENoise += noise[i] * noise[i];						//������� �����
	}
	
	//�������� ��� ����� ��� ������� ���������� �������� ������� ������� �������
	noiseNormKoef = sqrt((inputPercentageNoise / 100.0)* ESignal / ENoise);
	
	for (int i = 0; i < N; i++)signalwnoise[i] += noiseNormKoef * noise[i];   //��������� ��� � ������
	
	//������� ��� �������� ��� ���������� �������
	double YMax = signalwnoise[0], YMin = signalwnoise[0];
	for (int i = 0; i < N; i++)
	{
		if (YMax < signalwnoise[i])YMax = signalwnoise[i];
		if (YMin > signalwnoise[i])YMin = signalwnoise[i];
	}
	
				/*����� �������*/
	//drv.Draw(list_x, min_x, max_x, list_t, min_t, max_t);
	//drv.Draw(signalwnoise, YMin, YMax, list_t, min_t, max_t, 'R');


	//������ �������
	//���������� �������
	vector<double> list_n;
	vector<double> list_A;

	//������� ������ � ����������� �����
	vector <complex<double>> list_complex_x;
	for (int n = 0; n < N; n++)	{
		list_complex_x.push_back(signalwnoise[n]);
	}
	
	//������� ����� 
	vector<complex<double>> list_A_compl = FFT::fft(list_complex_x);
	//����� ������
	for (int i = 0; i < list_A_compl.size(); i++) list_A.push_back(abs(list_A_compl[i]));

	
	double max_A = 0;
	for (int n = 0; n < N; n++)
	{
		list_n.push_back((fd/N)* n);
		if (max_A < list_A[n]) max_A = list_A[n];//���� ���� ��������
	}

	//������� ������
	drv2.Draw(list_A, 0, max_A*1.25, list_n, 0, fd, 'R');
	//drv2.Draw(absFurKoef, 0, absFurKoefMax, FurKey, 0, fd);*/

	/*������ �������*/
	//������� ������ ���������� � ������������ ��������
	int nCl = NClear(list_A, inputPercentageCleaning);
	//������� ����� �������� �������
	vector<complex<double>> list_A_compl_clean = list_A_compl;
	vector<double> list_A_clean = list_A;
	//�������� �������� �� �� ���������
	for (int i = 0; i < list_A_compl_clean.size(); i++) {
		if (i > nCl && i < list_A_compl_clean.size() - nCl) {
			list_A_compl_clean[i] = (0, 0);
			list_A_clean[i] = 0.0;
		}
	}
	//������ ������ ���������� �������
	drv3.Draw(list_A_clean, 0, max_A * 1.25, list_n, 0, fd, 'G');

	/*������ ��������� ������*/
	//������ �������� ����� �������������� �� ��������� ���������� ������� � ���������� �����
	vector <complex<double>> list_x_compl_clean;
	list_x_compl_clean = FFT::fftRev(list_A_compl_clean);
	//������ �������� �����
	vector<double> list_x_clean;
	for (int i = 0; i < list_x_compl_clean.size(); i++) list_x_clean.push_back(real(list_x_compl_clean[i]));

	/*����� �������1*/
	drv.Draw(signalwnoise, YMin, YMax, list_t, min_t, max_t, 'R', list_x_clean, list_t, 'G');

	/*��������������� �������*/
	vector<double> difS;
	double EdifS = 0;							//��������� ������� ��������� � ��������������� �������
	for (int i = 0; i < list_x.size(); i++)
	{
		difS.push_back(list_x[i] - list_x_clean[i]);
		EdifS += difS[i] * difS[i];
	}
	deviation = EdifS / ESignal;	//�������������� ���������������, ��������� �������� ������� � ������� ��������� �������

	/*����� �������4*/
	drv4.Draw(list_x, YMin, YMax, list_t, min_t, max_t, 'R', list_x_clean, list_t, 'G');

	UpdateData(FALSE);
}



void CDrawDlg::OnEnChangeEdit10()	{ UpdateData(TRUE); }


void CDrawDlg::OnEnChangeEdit1()	{ UpdateData(TRUE); }


void CDrawDlg::OnEnChangeEdit2()	{ UpdateData(TRUE); }


void CDrawDlg::OnEnChangeEdit3()	{ UpdateData(TRUE); }


void CDrawDlg::OnEnChangeEdit11()	{ UpdateData(TRUE); }


void CDrawDlg::OnEnChangeEdit4()	{ UpdateData(TRUE); }


void CDrawDlg::OnEnChangeEdit5()	{ UpdateData(TRUE); }


void CDrawDlg::OnEnChangeEdit6()
{
	// TODO:  ���� ��� ������� ���������� RICHEDIT, �� ������� ���������� �� �����
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// ������� � ����� CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  �������� ��� �������� ����������
}


void CDrawDlg::OnEnChangeEdit7() { UpdateData(TRUE); }

void CDrawDlg::OnEnChangeEdit8() { UpdateData(TRUE); }

void CDrawDlg::OnEnChangeEdit9() { UpdateData(TRUE); }


void CDrawDlg::OnStnClickedPicture2()
{
	// TODO: �������� ���� ��� ����������� �����������
}

//������� ���� - inputPercentageNoise
void CDrawDlg::OnEnChangeEdit12()	{ UpdateData(TRUE); }
//�������� ������� - inputPercentageCleaning
void CDrawDlg::OnEnChangeEdit13()	{ UpdateData(TRUE); }
//deviation
void CDrawDlg::OnEnChangeEdit14() { UpdateData(TRUE); }

void CDrawDlg::OnStnClickedPicture3()
{
	// TODO: �������� ���� ��� ����������� �����������
}

void CDrawDlg::OnStnClickedPicture4()
{
	// TODO: �������� ���� ��� ����������� �����������
}
