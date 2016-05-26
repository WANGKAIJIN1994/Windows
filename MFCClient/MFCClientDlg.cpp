
// MFCClientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCClient.h"
#include "MFCClientDlg.h"
#include "afxdialogex.h"

// 定义网络事件通知消息
#define WM_SOCKET WM_USER + 1




#ifdef _DEBUG
#define new DEBUG_NEW
#endif

sockaddr_in sin_socket;
BOOL isConnect = false;
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCClientDlg 对话框



CMFCClientDlg::CMFCClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFCClientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_SOCKET, OnSocket)
	ON_BN_CLICKED(IDC_CLEAR, &CMFCClientDlg::OnBnClickedClear)
	ON_BN_CLICKED(IDC_EXIT, &CMFCClientDlg::OnBnClickedExit)
	ON_BN_CLICKED(IDC_CONNECT, &CMFCClientDlg::OnBnClickedConnect)	
	ON_BN_CLICKED(IDC_SEND, &CMFCClientDlg::OnBnClickedSend)
	ON_BN_CLICKED(IDC_UDP, &CMFCClientDlg::OnBnClickedUdp)
END_MESSAGE_MAP()


// CMFCClientDlg 消息处理程序

BOOL CMFCClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// 创建状态栏，设置它的属性
	m_bar.Create(WS_CHILD | WS_VISIBLE | SBS_SIZEGRIP, CRect(0, 0, 0, 0), this, 101);
	m_bar.SetBkColor(RGB(0x66, 0xcc, 0xff));		// 背景色
	int arWidth[] = { 350, -1 };
	m_bar.SetParts(2, arWidth);				// 分栏
	m_bar.SetText(" UDP空闲", 1, 0);	// 第一个栏的文本
	m_bar.SetText(" TCP空闲", 0, 0);				// 第二个栏的文本
	// 初始化发送按钮和发送编辑框的状态
	GetDlgItem(IDC_SEND)->EnableWindow(FALSE);
	GetDlgItem(IDC_INPUT)->EnableWindow(FALSE);

	m_list.SubclassDlgItem(IDC_LIST1, this);

	// 初始化连接套节字
	m_socket = INVALID_SOCKET;
	udp_socket = INVALID_SOCKET;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMFCClientDlg::OnPaint()
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
HCURSOR CMFCClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




void CMFCClientDlg::OnBnClickedClear()
{
	m_list.ResetContent();
}


void CMFCClientDlg::OnBnClickedExit()
{
	if (m_socket != INVALID_SOCKET)
		::closesocket(m_socket);

	CDialog::OnCancel();
}




void CMFCClientDlg::OnBnClickedConnect()
{
	if (m_socket == INVALID_SOCKET)  // 连接服务器
	{
		// 取得服务器地址
		CString sAddr;
		GetDlgItem(IDC_ADDR)->GetWindowText(sAddr);
		if (sAddr.IsEmpty())
		{
			MessageBox("请输入服务器地址！");
			return;
		}

		// 取得端口号
		CString sPort;
		GetDlgItem(IDC_PORT)->GetWindowText(sPort);
		int nPort = atoi(sPort);
		if (nPort < 1 || nPort > 65535)
		{
			MessageBox("端口号错误！");
			return;
		}
		// 试图连接服务器
		if (!Connect(sAddr, nPort))
		{
			MessageBox("连接服务器出错！");
			return;
		}

		// 设置用户界面
		isConnect = true;
		GetDlgItem(IDC_CONNECT)->SetWindowText("取消");
		m_bar.SetText(" 正在连接……", 0, 0);
	}
	else		// 断开服务器
	{
		// 关闭套节字
		::closesocket(m_socket);
		m_socket = INVALID_SOCKET;

		// 设置用户界面
		GetDlgItem(IDC_CONNECT)->SetWindowText("连接服务器");
		m_bar.SetText(" 空闲", 0, 0);
		if (IsDlgButtonChecked(IDC_UDP) == 0)
		{
			GetDlgItem(IDC_INPUT)->EnableWindow(FALSE);
			GetDlgItem(IDC_SEND)->EnableWindow(FALSE);
		}
		isConnect = false;
	}
}

BOOL CMFCClientDlg::Connect(LPCTSTR pszRemoteAddr, u_short nPort)
{
	// 创建套节字
	m_socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_socket == INVALID_SOCKET)
	{
		return FALSE;
	}

	// 设置socket为窗口通知消息类型
	::WSAAsyncSelect(m_socket, m_hWnd,
		WM_SOCKET, FD_CONNECT | FD_CLOSE | FD_WRITE | FD_READ);

	// 假定szAddr是IP地址
	ULONG uAddr = ::inet_addr(pszRemoteAddr);

	// 填写服务器地址信息
	sockaddr_in remote;
	remote.sin_addr.S_un.S_addr = uAddr;
	remote.sin_family = AF_INET;
	remote.sin_port = htons(nPort);

	// 连接到远程机
	::connect(m_socket, (sockaddr*)&remote, sizeof(sockaddr));

	return TRUE;
}

void CMFCClientDlg::AddStringToList(LPCTSTR pszText, BOOL bRecv)
{
	CString strEdit;
	GetDlgItem(IDC_LIST1)->GetWindowText(strEdit);

	if (bRecv)
	{
		strEdit += "UDP服务未开启，发送失败！！！";
	}
	else
	{
		strEdit += "【Send】：";
		strEdit += pszText;
	}
	m_list.InsertString(-1,strEdit);
}

long CMFCClientDlg::OnSocket(WPARAM wParam, LPARAM lParam)
{
	// 取得有事件发生的套节字句柄
	SOCKET s = wParam;
	// 查看是否出错
	if (WSAGETSELECTERROR(lParam))
	{
		if (m_socket != SOCKET_ERROR)
			OnBnClickedConnect();
		m_bar.SetText(" 连接出错！", 0, 0);
		return 0;
	}
	// 处理发生的事件
	switch (WSAGETSELECTEVENT(lParam))
	{
	case FD_CONNECT:	// 套节字正确的连接到服务器
	{
							// 设置用户界面
							GetDlgItem(IDC_CONNECT)->SetWindowText("断开连接");
							GetDlgItem(IDC_INPUT)->EnableWindow(TRUE);
							GetDlgItem(IDC_SEND)->EnableWindow(TRUE);
							m_bar.SetText(" 已经连接到服务器", 0, 0);
	}
		break;

	case FD_READ:		// 套接字接受到对方发送过来的数据包
	{
							// 从服务器接受数据
							char szText[1024] = { 0 };
							::recv(s, szText, 1024, 0);
							// 显示给用户
							AddStringToList(CString(szText) + "\r\n");
	}
		break;

	case FD_CLOSE:
		OnBnClickedConnect();
		break;
	}

	return 0;
}

void CMFCClientDlg::OnBnClickedSend()
{
	if (udp_socket == INVALID_SOCKET && m_socket == INVALID_SOCKET)
		return;
	else
	{
		// 取得要发送的字符串
		CString sText;
		GetDlgItem(IDC_INPUT)->GetWindowText(sText);

		// 添加一个“回车换行”
		// 注意，添加它并不是必须的，但是如果使用本软件作为客户端调试网络协议，
		// 比如SMTP、FTP等，就要添加它了。因为这些协议都要求使用“回车换行”作为一个命令的结束标记
		sText += "\r\n";

		// 发送数据到服务器

		if (IsDlgButtonChecked(IDC_UDP) == 1 && ::sendto(udp_socket, sText, sText.GetLength(), 0, (sockaddr*)&sin_socket, sizeof(sin_socket)) != -1)
		{
			AddStringToList(sText, FALSE);
			GetDlgItem(IDC_INPUT)->SetWindowText("");
		}
		if (IsDlgButtonChecked(IDC_UDP) == 0 && ::send(m_socket, sText, sText.GetLength(), 0) != -1)
		{
			AddStringToList(sText, FALSE);
			GetDlgItem(IDC_INPUT)->SetWindowText("");
		}
	}
}


void CMFCClientDlg::OnBnClickedUdp()
{
	if (IsDlgButtonChecked(IDC_UDP) == 1)
	{
		m_bar.SetText(" UDP已开启", 1, 0);
		GetDlgItem(IDC_SEND)->EnableWindow(TRUE);
		GetDlgItem(IDC_INPUT)->EnableWindow(TRUE);

		udp_socket = ::socket(AF_INET, SOCK_DGRAM, 0);
		::WSAAsyncSelect(udp_socket, m_hWnd,
			WM_SOCKET, FD_CLOSE | FD_READ);

		//取得服务器地址
		CString sAddr;
		GetDlgItem(IDC_ADDR)->GetWindowText(sAddr);

		// 取得端口号
		CString sPort;
		GetDlgItem(IDC_PORT)->GetWindowText(sPort);
		int nPort = atoi(sPort);

		if (nPort < 0 || nPort > 65535 || sAddr.IsEmpty() || sPort.IsEmpty())
		{
			m_bar.SetText(" UDP空闲", 1, 0);	// 第一个栏的文本
			MessageBox("端口或地址错误！\n请重新输入");
			CButton* pBtn = (CButton*)GetDlgItem(IDC_UDP);
			pBtn->SetCheck(0);		
			return;
		}

		// 填写服务器地址信息
		sin_socket.sin_addr.S_un.S_addr = inet_addr(sAddr);
		sin_socket.sin_family = AF_INET;
		sin_socket.sin_port = htons(nPort);
	}
	else if (IsDlgButtonChecked(IDC_UDP) == 0 && isConnect == false){
		GetDlgItem(IDC_SEND)->EnableWindow(false);
		GetDlgItem(IDC_INPUT)->EnableWindow(false);
		m_bar.SetText(" UDP空闲", 1, 0);
	}
	else if (IsDlgButtonChecked(IDC_UDP) == 0)
		m_bar.SetText(" UDP空闲", 1, 0);
}
