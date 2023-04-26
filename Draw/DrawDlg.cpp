
// DrawDlg.cpp : файл реализации
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

//Функция очистки сигнала
//Возвращает конец промежутка сигнала, который нужно оставить
int NClear(const std::vector<double>& sp, double vost)
{
	double ESp = 0, ECl = 0;
	//Считаем всего энергию спектра
	for (int i = 0; i < sp.size(); i++)ESp += sp[i] * sp[i];
	int n = 0;
	ECl += sp[n] * sp[n];//энергия интервала
	while (ECl < ESp * vost / 200.0)//делим на 100% и 2 из-за симметрии
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


// обработчики сообщений CDrawDlg

BOOL CDrawDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	drv.Create(GetDlgItem(IDC_PICTURE)->GetSafeHwnd());
	drv2.Create(GetDlgItem(IDC_PICTURE2)->GetSafeHwnd());
	drv3.Create(GetDlgItem(IDC_PICTURE3)->GetSafeHwnd());
	drv4.Create(GetDlgItem(IDC_PICTURE4)->GetSafeHwnd());

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CDrawDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		//drv.Draw();
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CDrawDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDrawDlg::OnStnClickedPicture()
{
	// TODO: добавьте свой код обработчика уведомлений
}


/*			Здесь код)						*/		
void CDrawDlg::OnBnClickedOk()
{
	vector<double> noise, signalwnoise;
	double ESignal = 0, ENoise = 0, noiseNormKoef;

	//График исходной функции
	//Координаты  исходного графика
	vector<double> list_t; double min_t = 0;  double max_t = N /fd;	
	vector<double> list_x; double min_x = -1; double max_x = 1;

	//Определения предварительного макимума
	if (A1 >= A2 && A1 >= A3)	max_x = A1;
	if (A2 >= A1 && A2 >= A3)	max_x = A2;
	if (A3 >= A1 && A3 >= A2)	max_x = A3;
	min_x = -max_x;

	//Вычисление координат графика
	for (int i = 0; i < N; i++)	{
		list_t.push_back(i/fd);
		list_x.push_back(
			A1 * sin(2 * pi * f1 * list_t[i] + phase1) +
			A2 * sin(2 * pi * f2 * list_t[i] + phase2) +
			A3 * sin(2 * pi * f3 * list_t[i] + phase3) );

		if (min_x > list_x[i]) min_x = list_x[i];
		if (max_x < list_x[i]) max_x = list_x[i];

		ESignal += list_x[i] * list_x[i];					//Энергия сигнала
	}

	
						/*ШУМ*/
	//Для зашумления будем использовать белый Гаусовы шум, он адетивен и имеет одинковое расперделение на всех частотах
	//генерируем белый шум с отсчётами [-1, 1] и считаем его энергию
	signalwnoise = list_x;
	for (int i = 0; i < N; i++)                                 
	{
		//Считаем gk числа распределения по Гаусу, суммирую 12 раз 
		noise.push_back(((double)rand() * 2.0 / (double)RAND_MAX) - 1);
		for (int j = 0; j < 11; j++)noise[i] += (((double)rand() * 2.0 / (double)RAND_MAX) - 1);
		noise[i] = noise[i] / 12.0;

		ENoise += noise[i] * noise[i];						//Энергия шумма
	}
	
	//изменяем шум чтобы его энергия составляла заданный процент энергии сигнала
	noiseNormKoef = sqrt((inputPercentageNoise / 100.0)* ESignal / ENoise);
	
	for (int i = 0; i < N; i++)signalwnoise[i] += noiseNormKoef * noise[i];   //добавляем шум в сигнал
	
	//Находим мак значение для построения графика
	double YMax = signalwnoise[0], YMin = signalwnoise[0];
	for (int i = 0; i < N; i++)
	{
		if (YMax < signalwnoise[i])YMax = signalwnoise[i];
		if (YMin > signalwnoise[i])YMin = signalwnoise[i];
	}
	
				/*Вывод графика*/
	//drv.Draw(list_x, min_x, max_x, list_t, min_t, max_t);
	//drv.Draw(signalwnoise, YMin, YMax, list_t, min_t, max_t, 'R');


	//График спектра
	//Координаты спектра
	vector<double> list_n;
	vector<double> list_A;

	//Заводим сиганл в комлексноую форму
	vector <complex<double>> list_complex_x;
	for (int n = 0; n < N; n++)	{
		list_complex_x.push_back(signalwnoise[n]);
	}
	
	//Считаем фурье 
	vector<complex<double>> list_A_compl = FFT::fft(list_complex_x);
	//Берем модуль
	for (int i = 0; i < list_A_compl.size(); i++) list_A.push_back(abs(list_A_compl[i]));

	
	double max_A = 0;
	for (int n = 0; n < N; n++)
	{
		list_n.push_back((fd/N)* n);
		if (max_A < list_A[n]) max_A = list_A[n];//Ищем макс значение
	}

	//Выводим график
	drv2.Draw(list_A, 0, max_A*1.25, list_n, 0, fd, 'R');
	//drv2.Draw(absFurKoef, 0, absFurKoefMax, FurKey, 0, fd);*/

	/*Чистка спектра*/
	//Находим длинну промежутка с определенной энергией
	int nCl = NClear(list_A, inputPercentageCleaning);
	//Создаем копию значений спектра
	vector<complex<double>> list_A_compl_clean = list_A_compl;
	vector<double> list_A_clean = list_A;
	//Зануляем элементы не из интервала
	for (int i = 0; i < list_A_compl_clean.size(); i++) {
		if (i > nCl && i < list_A_compl_clean.size() - nCl) {
			list_A_compl_clean[i] = (0, 0);
			list_A_clean[i] = 0.0;
		}
	}
	//Строим график очищенного спектра
	drv3.Draw(list_A_clean, 0, max_A * 1.25, list_n, 0, fd, 'G');

	/*НАЙДЕМ ОЧИЩЕННЫЙ СИГНАЛ*/
	//Возмем обратное фурье преобразование от исходного очищенного спектра в комплесной форме
	vector <complex<double>> list_x_compl_clean;
	list_x_compl_clean = FFT::fftRev(list_A_compl_clean);
	//Возмем реальную часть
	vector<double> list_x_clean;
	for (int i = 0; i < list_x_compl_clean.size(); i++) list_x_clean.push_back(real(list_x_compl_clean[i]));

	/*Вывод графика1*/
	drv.Draw(signalwnoise, YMin, YMax, list_t, min_t, max_t, 'R', list_x_clean, list_t, 'G');

	/*Рассогласование сигнала*/
	vector<double> difS;
	double EdifS = 0;							//отношение энергий истинного и отфлитрованного сигнала
	for (int i = 0; i < list_x.size(); i++)
	{
		difS.push_back(list_x[i] - list_x_clean[i]);
		EdifS += difS[i] * difS[i];
	}
	deviation = EdifS / ESignal;	//Характеристика рассогласования, отношение разности энергий к энергии истинного сигнала

	/*Вывод графика4*/
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
	// TODO:  Если это элемент управления RICHEDIT, то элемент управления не будет
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// функция и вызов CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Добавьте код элемента управления
}


void CDrawDlg::OnEnChangeEdit7() { UpdateData(TRUE); }

void CDrawDlg::OnEnChangeEdit8() { UpdateData(TRUE); }

void CDrawDlg::OnEnChangeEdit9() { UpdateData(TRUE); }


void CDrawDlg::OnStnClickedPicture2()
{
	// TODO: добавьте свой код обработчика уведомлений
}

//Процент шума - inputPercentageNoise
void CDrawDlg::OnEnChangeEdit12()	{ UpdateData(TRUE); }
//Процента очистки - inputPercentageCleaning
void CDrawDlg::OnEnChangeEdit13()	{ UpdateData(TRUE); }
//deviation
void CDrawDlg::OnEnChangeEdit14() { UpdateData(TRUE); }

void CDrawDlg::OnStnClickedPicture3()
{
	// TODO: добавьте свой код обработчика уведомлений
}

void CDrawDlg::OnStnClickedPicture4()
{
	// TODO: добавьте свой код обработчика уведомлений
}
