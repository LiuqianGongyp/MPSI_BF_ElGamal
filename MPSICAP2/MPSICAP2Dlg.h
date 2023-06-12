
// MPSICAP2Dlg.h: 头文件
//

#pragma once


// CMPSICAP2Dlg 对话框
class CMPSICAP2Dlg : public CDialogEx
{
// 构造
public:
	CMPSICAP2Dlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MPSICAP2_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOpenfile();
	CEdit m_path;
	afx_msg void OnBnClickedBegin();
	CEdit m_positive;
	CEdit m_maxsize;
	CEdit m_security;
	CEdit m_time;
	CEdit m_numparty;
	CEdit m_ip;
};
