
// MPSIP3Dlg.cpp: 实现文件
//

#include "pch.h"
#include "ConstSet.h"
#include "BloomFilter.h"
#include "tel.h"
#include "trans.h"
#include <algorithm> // std::move_backward
#include <random> // std::default_random_engine
#include <chrono> // std::chrono::system_clock
#include "framework.h"
#include "MPSIP3.h"
#include "MPSIP3Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMPSIP3Dlg 对话框



CMPSIP3Dlg::CMPSIP3Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MPSIP3_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMPSIP3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDITPath, m_path);
	DDX_Control(pDX, IDC_FPP, m_positive);
	DDX_Control(pDX, IDC_IPADdress, m_ip);
	DDX_Control(pDX, IDC_MaxSize, m_maxsize);
	DDX_Control(pDX, IDC_NumParty, m_numparty);
	DDX_Control(pDX, IDC_Secure, m_security);
	DDX_Control(pDX, IDC_time, m_time);
}

BEGIN_MESSAGE_MAP(CMPSIP3Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_OpenFile, &CMPSIP3Dlg::OnBnClickedOpenfile)
	ON_BN_CLICKED(IDC_Begin, &CMPSIP3Dlg::OnBnClickedBegin)
END_MESSAGE_MAP()


// CMPSIP3Dlg 消息处理程序

BOOL CMPSIP3Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_maxsize.SetWindowTextA("10000");
	m_positive.SetWindowTextA("0.01");
	m_security.SetWindowTextA("12");
	m_numparty.SetWindowTextA("3");
	m_ip.SetWindowTextA("");

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMPSIP3Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMPSIP3Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMPSIP3Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMPSIP3Dlg::OnBnClickedOpenfile()
{
	// TODO: 在此添加控件通知处理程序代码
	TCHAR szFilter[] = _T("文本文件(*.txt)|*.txt|所有文件(*.*)|*.*||");
	// 构造打开文件对话框   
	CFileDialog fileDlg(TRUE, _T("txt"), NULL, OFN_ALLOWMULTISELECT, szFilter, this);
	fileDlg.m_ofn.lpstrTitle = _T("请选择本方的输入集");
	CString strFilePath;
	//CArray<CString, CString> aryFilename;//索引从0开始，aryFilename[0]是对第一个字符串的引用
	// 显示打开文件对话框
	if (IDOK == fileDlg.DoModal())
	{
		POSITION posFile = fileDlg.GetStartPosition();
		while (posFile != NULL)
		{
			CString str_now, str_before;
			str_now = fileDlg.GetNextPathName(posFile);
			//aryFilename.Add(str_now);
			m_path.GetWindowText(str_before);
			m_path.SetWindowText(str_before + str_now + "\r\n");
		}
	}
}


void CMPSIP3Dlg::OnBnClickedBegin()
{
	// TODO: 在此添加控件通知处理程序代码
	clock_t TStart, TEnd;
	double RunTime = 0;
	CString str1, str2, str3, str4, str6, str7;
	m_security.GetWindowText(str1);
	int l = _ttoi(str1);
	m_positive.GetWindowText(str2);
	const double FPP = _tstof(str2);//假阳性概率
	m_path.GetWindowText(str3);
	m_maxsize.GetWindowText(str4);
	const int MaxSize = _ttoi(str4);
	m_ip.GetWindowTextA(str6);
	m_numparty.GetWindowTextA(str7);
	const int NumParty = _ttoi(str7);
	char** IPADdress = new char* [NumParty];
	int pos = 0;
	for (int i = 0; i < NumParty; i++)
	{
		if (i == Me) {
			continue;
		}
		IPADdress[i] = new char[20];
		strcpy(IPADdress[i], str6.Tokenize(_T("\r\n"), pos));
	}

	TStart = clock();
	Mul* PartiesELPK;
	PartiesELPK = new Mul[NumParty - 1];
	PartiesELPK[0].port = 3778;
	PartiesELPK[1].port = 3779;
	HANDLE* hThread;
	hThread = new HANDLE[NumParty - 1];
	for (int i = 0; i < NumParty - 1; i++) {
		hThread[i] = CreateThread(NULL, 0, getELPK, (LPVOID*)&PartiesELPK[i], 0, NULL);
	}
	//初始化
	Party party;
	CBloomFilter BF = CBloomFilter(MaxSize, FPP);
	party.filename = new char[Filename_MAX];
	pos = 0;
	strcpy(party.filename, str3.Tokenize(_T("\r\n"), pos));
	ZZ p, q, g;
	TElSetup(l, &p, &q, &g);
	RandomBnd(party.sk, q);
	party.pk = PowerMod(g, party.sk, p);
	char* zzstring = strdup(zToString(party.pk).c_str());
	TEnd = clock();
	RunTime += (TEnd - TStart);
	if (MessageBox("公开己方公钥", "通信提示", MB_OKCANCEL | MB_SYSTEMMODAL) == IDOK) {
		boradcast(zzstring, 3780);
	}
	TStart = clock();
	//WaitForSingleObject(hThread, INFINITE);
	WaitForMultipleObjects(NumParty - 1, hThread, TRUE, INFINITE);
	ZZ PK;
	PK = TELKGen(p, q, g, party.pk, PartiesELPK, NumParty);
	TEnd = clock();
	RunTime += (TEnd - TStart);
	MessageBox(zToString(PK).c_str(), "公钥");
	//request
	TStart = clock();
	long long bitsize = BF.arraysize;
	BF.InitialBloomFilter(party.filename);
	party.ciphertext1 = new ZZ[bitsize];
	party.ciphertext2 = new ZZ[bitsize];
	for (int j = 0; j < bitsize; j++)
	{
		ZZ m, c1, c2;
		if ((BF.bitarray[j / 8] & (128 >> (j % 8))) == (128 >> (j % 8)))
		{
			m = 1;
			//cout << m ;
		}
		else
		{
			m = 0;
		}
		TELEnc(p, q, g, PK, m, &c1, &c2);
		party.ciphertext1[j] = c1;
		party.ciphertext2[j] = c2;
	}
	char* bitarray1 = strdup(zToString(party.ciphertext1, (ZZ)bitsize).c_str());
	char* bitarray2 = strdup(zToString(party.ciphertext2, (ZZ)bitsize).c_str());
	//MessageBox("准备发送加密的布隆过滤器" );
	SendBitString(bitarray1, (ZZ)bitsize);
	SendBitString(bitarray2, (ZZ)bitsize);
	//response
	long long SizeX1;
	ReceiveBits(party.ciphertext1, IPADdress[0], &SizeX1);

	//computation
	for (int j = 0; j < SizeX1; j++) {
		party.ciphertext1[j] = PowerMod(party.ciphertext1[j], party.sk, p);
	}
	bitarray1 = strdup(zToString(party.ciphertext1, (ZZ)SizeX1).c_str());
	SendBitString(bitarray1, (ZZ)SizeX1);
	TEnd = clock();
	RunTime += (TEnd - TStart);
	stringstream ss;
	ss << RunTime / 1000;
	m_time.SetWindowTextA(ss.str().c_str());
}
