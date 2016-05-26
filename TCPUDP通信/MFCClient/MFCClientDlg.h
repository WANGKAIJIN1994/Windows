
// MFCClientDlg.h : 头文件
//
#pragma once


// CMFCClientDlg 对话框
class CMFCClientDlg : public CDialogEx
{
// 构造
public:
	CMFCClientDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MFCCLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

protected:
	// 状态栏子窗口控件
	CStatusBarCtrl m_bar;
	// 用于与服务器取得连接的套节字句柄
	SOCKET m_socket;
	SOCKET udp_socket;
	CListBox m_list;

// 实现

protected:
	// 连接服务器
	BOOL Connect(LPCTSTR pszRemoteAddr, u_short nPort);
	// 向文本框中添加文本
	void AddStringToList(LPCTSTR pszText, BOOL bRecv = TRUE);

protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnBnClickedClear();
	afx_msg void OnBnClickedExit();
	afx_msg void OnBnClickedConnect();
	afx_msg long OnSocket(WPARAM wParam, LPARAM lParam);
	afx_msg void OnEnChangeInput();	
	afx_msg void OnBnClickedSend();
	afx_msg void OnBnClickedUdp();
};
