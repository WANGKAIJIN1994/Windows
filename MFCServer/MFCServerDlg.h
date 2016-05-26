
// MFCServerDlg.h : 头文件
//

#pragma once
#define MAX_SOCKET 56	// 定义此服务器所能接受的最大客户量

// CMFCServerDlg 对话框
class CMFCServerDlg : public CDialogEx
{
// 构造
public:
	CMFCServerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MFCSERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	// 创建TCP套节字，并设置为监听状态，准备接受客户的连接
	BOOL TCPCreateAndListen(int nPort);
	// 关闭所有套节字，包括监听套节字和所有accept函数返回的套节字
	void CloseTCPSocket();
	// 向客户连接列表中添加一个客户
	void CloseUDPSocket();
	// 向客户连接列表中添加一个客户
	BOOL AddClient(SOCKET s);
	// 从客户连接列表中移处一个客户
	void RemoveClient(SOCKET s);
	//显示当前连接的所有客户端
	void DisplayClient();

protected:
	// 三个子窗口控件，一个是状态栏，两个是列表框
	CStatusBarCtrl m_bar;
	CListBox m_listInfo;
	CListBox m_list;

	// 监听套节字句柄
	SOCKET m_socket;
	SOCKET udp_socket;

	// 客户连接列表
	SOCKET m_arClient[MAX_SOCKET];	// 套节字数组
	int m_nClient;			// 上述数组的大小


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	// 开启或停止TCP服务
	afx_msg void OnTCPStart();
	// 开启或停止UDP服务
	afx_msg void OnUDPStart();
	// 清空信息
	afx_msg void OnClear();
	// 套节字通知事件
	afx_msg long OnSocket(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedExit();
	afx_msg void OnBnClickedClear();
	afx_msg void OnBnClickedTcp();
	afx_msg void OnBnClickedUdp();
};
