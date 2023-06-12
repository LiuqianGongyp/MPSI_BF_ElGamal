
// MPSIP1Dlg.cpp: 实现文件
//

#include "pch.h"
#include "ConstSet.h"
#include "BloomFilter.h"
#include "tel.h"
#include "trans.h"
#include <algorithm> // std::move_backward
#include <random> // std::default_random_engine
#include <chrono> // std::chrono::system_clock
#include <queue>
#include "framework.h"
#include "MPSIP1.h"
#include "MPSIP1Dlg.h"
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


// CMPSIP1Dlg 对话框



CMPSIP1Dlg::CMPSIP1Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MPSIP1_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMPSIP1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDITPath, m_path);
	DDX_Control(pDX, IDC_FPP, m_positive);
	DDX_Control(pDX, IDC_InterCA, m_IntersectSize);
	DDX_Control(pDX, IDC_MaxSize, m_maxsize);
	DDX_Control(pDX, IDC_Secure, m_security);
	DDX_Control(pDX, IDC_Size1, m_Size1);
	DDX_Control(pDX, IDC_time, m_time);
	DDX_Control(pDX, IDC_InterSet, m_settext);
	DDX_Control(pDX, IDC_NumParty, m_numparty);
	DDX_Control(pDX, IDC_IPADdress, m_ip);
}

BEGIN_MESSAGE_MAP(CMPSIP1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_OpenFile, &CMPSIP1Dlg::OnBnClickedOpenfile)
	ON_BN_CLICKED(IDC_Begin, &CMPSIP1Dlg::OnBnClickedBegin)
END_MESSAGE_MAP()


// CMPSIP1Dlg 消息处理程序

BOOL CMPSIP1Dlg::OnInitDialog()
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
	m_security.SetWindowTextA("1024");
	m_Size1.SetWindowTextA("10000");
	m_numparty.SetWindowTextA("3");
	m_ip.SetWindowTextA("");
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMPSIP1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMPSIP1Dlg::OnPaint()
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
HCURSOR CMPSIP1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}





void CMPSIP1Dlg::OnBnClickedOpenfile()
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


void CMPSIP1Dlg::OnBnClickedBegin()
{
	// TODO: 在此添加控件通知处理程序代码
	clock_t TStart, TEnd;
	double RunTime = 0;
	CString str1, str2, str3, str4, str5, str6, str7;
	m_security.GetWindowText(str1);
	int l = _ttoi(str1);
	m_positive.GetWindowText(str2);
	const double FPP = _tstof(str2);//假阳性概率
	m_path.GetWindowText(str3);
	m_maxsize.GetWindowText(str4);
	const int MaxSize = _ttoi(str4);
	m_Size1.GetWindowText(str5);
	const int SizeX1 = _ttoi(str5);
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
	PartiesELPK[0].port = 3779;
	PartiesELPK[1].port = 3780;
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
		boradcast(zzstring, 3778);
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
	long bitsize = BF.arraysize;
	//response
	//MessageBox("接收其余参与方的加密的布隆过滤器", "通信提示");
	int HashNum = BF.hasFunsNum;
	int* HashValue;
	HashValue = new int[HashNum];
	party.ciphertext1 = new ZZ[SizeX1];
	party.ciphertext2 = new ZZ[SizeX1];
	ZZ** Cipher1;
	ZZ** Cipher2;
	Cipher1 = new ZZ * [NumParty - 1];
	Cipher2 = new ZZ * [NumParty - 1];
	for (int i = 0; i < NumParty - 1; i++) {
		Cipher1[i] = new ZZ[bitsize];
		Cipher2[i] = new ZZ[bitsize];
		ReceiveBits(Cipher1[i], IPADdress[i + 1]);
		ReceiveBits(Cipher2[i], IPADdress[i + 1]);
	}
	FILE* fin;
	fopen_s(&fin, party.filename, "r");
	//assert(fin);
	char ch;
	char str[1000] = { '\0' };
	int  iCnt = 0;
	int add = 0;
	int Final;
	do
	{
		Final = fscanf_s(fin, "%c", &ch);
		if (ch != ' ' && Final != EOF)
		{
			str[iCnt] = ch;
			iCnt++;
		}
		else
		{
			str[iCnt] = '\0';/*
			string S;
			S = str;
			cout <<"a"<< S <<"a"<< endl;*/
			BF.GetHashValue(str, strlen(str), HashValue);
			iCnt = 0;
			ZZ C1(1), C2(1);
			for (int index = 0; index < NumParty - 1; index++) {
				for (int k = 0; k < HashNum; k++) {
					/*cout << HashValue[k] << endl;
					cout << 'a' << endl;*/
					C1 = MulMod(C1, Cipher1[index][HashValue[k]], p);
					C2 = MulMod(C2, Cipher2[index][HashValue[k]], p);
				}
			}
			party.ciphertext1[add] = C1;
			party.ciphertext2[add] = C2;
			add++;
		}
	} while (Final != EOF && add < SizeX1);
	if (NULL != fin)
	{
		fclose(fin);
		fin = NULL;
	}
	char* bitarray1 = strdup(zToString(party.ciphertext1, (ZZ)SizeX1).c_str());
	//char* bitarray2 = strdup(zToString(party.ciphertext2, (ZZ)SizeX1).c_str());
	SendBitString(bitarray1, (ZZ)SizeX1);
	SendBitString(bitarray1, (ZZ)SizeX1);
	//computation
	ZZ** T;
	T = new ZZ * [NumParty - 1];
	for (int i = 1; i < NumParty; i++) {
		T[i] = new ZZ[SizeX1];
		ReceiveBits(T[i], IPADdress[i]);
	}
	T[0] = new ZZ[SizeX1];
	for (int j = 0; j < SizeX1; j++) {
		T[0][j] = PowerMod(party.ciphertext1[j], party.sk, p);
	}
	ZZ count;//
	queue<int> answer;
	for (int index = 0; index < SizeX1; index++) {
		ZZ rho(1);
		for (int i = 0; i < NumParty; i++) {
			rho = MulMod(rho, T[i][index], p);
		}
		//cout << 'a' << endl;
		if (MulMod(party.ciphertext2[index], InvMod(rho, p), p) == 1) {
			answer.push(index);
			count++;
			//cout << index << endl;
		}
	}
	TEnd = clock();
	RunTime += (TEnd - TStart);
	stringstream ss;
	ss << count;
	m_IntersectSize.SetWindowTextA(ss.str().c_str());
	ss.clear();
	ss.str("");
	ss << RunTime / 1000;
	m_time.SetWindowTextA(ss.str().c_str());


	ss.clear();
	ss.str("");
	fopen_s(&fin, party.filename, "r");
	iCnt = 0;
	add = 0;
	do
	{
		Final = fscanf_s(fin, "%c", &ch);
		if (ch != ' ' && Final != EOF)
		{
			str[iCnt] = ch;
			iCnt++;
		}
		else
		{
			str[iCnt] = '\0';
			iCnt = 0;
			if (answer.front() == add) {
				string S;
				S = str;
				ss << S << "\r\n";
				answer.pop();
			}
			add++;
		}
	} while (Final != EOF && add < SizeX1 && answer.size() > 0);
	if (NULL != fin)
	{
		fclose(fin);
		fin = NULL;
	}
	m_settext.SetWindowTextA(ss.str().c_str());
}
