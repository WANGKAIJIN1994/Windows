
// MFCServerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCServer.h"
#include "MFCServerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
CString sIP;
//定义网络事件通知消息
#define WM_SOCKET WM_USER + 1	
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


// CMFCServerDlg 对话框



CMFCServerDlg::CMFCServerDlg(CWnd* pParent /*=NULL*/)
: CDialogEx(CMFCServerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_EXIT, &CMFCServerDlg::OnBnClickedExit)
	ON_BN_CLICKED(IDC_CLEAR, &CMFCServerDlg::OnBnClickedClear)
	ON_BN_CLICKED(IDC_TCP, &CMFCServerDlg::OnBnClickedTcp)
	ON_MESSAGE(WM_SOCKET, OnSocket)
	ON_BN_CLICKED(IDC_UDP, &CMFCServerDlg::OnBnClickedUdp)
END_MESSAGE_MAP()


// CMFCServerDlg 消息处理程序

BOOL CMFCServerDlg::OnInitDialog()
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

	m_bar.Create(WS_CHILD | WS_VISIBLE | SBS_SIZEGRIP, CRect(0, 0, 0, 0), this, 101);
	m_bar.SetBkColor(RGB(0x66, 0xcc, 0xff));		// 背景色
	int arWidth[3] = { 300, 500, -1};
	m_bar.SetParts(3, arWidth);				// 分栏
	m_bar.SetText(" 空闲", 0, 0);
	m_bar.SetText(" UDP服务未开启！", 2, 0);
	m_bar.SetText(" TCP服务未开启！", 1, 0);
	// 设置列表框控件到m_list对象的关联
	m_listInfo.SubclassDlgItem(IDC_LIST1, this);
	m_list.SubclassDlgItem(IDC_LIST2, this);

	// 初始化监听套节字和连接列表
	m_socket = INVALID_SOCKET;
	udp_socket = INVALID_SOCKET;
	m_nClient = 0;

	// 下面是取得本地IP地址的过程，将它显示在状态栏的第一个分栏中
	// 取得本机名称	
	char szHost[256];
	::gethostname(szHost, 256);
	// 通过本机名称取得地址信息
	HOSTENT* pHost = gethostbyname(szHost);
	if (pHost != NULL)
	{
		// 得到第一个IP地址
		in_addr *addr = (in_addr*)*(pHost->h_addr_list);

		// 显示给用户
		sIP.Format(" %s", inet_ntoa(addr[0]));
		m_bar.SetText("本机IP：" + sIP, 0, 0);
	}

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMFCServerDlg::OnPaint()
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
HCURSOR CMFCServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCServerDlg::OnBnClickedExit()
{
	CloseTCPSocket();
	CDialogEx::OnCancel();
}


BOOL CMFCServerDlg::TCPCreateAndListen(int nPort)
{
	if (m_socket == INVALID_SOCKET)
		::closesocket(m_socket);

	// 创建套节字
	m_socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_socket == INVALID_SOCKET)
		return FALSE;

	// 填写要关联的本地地址
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(nPort);
	sin.sin_addr.s_addr = INADDR_ANY;
	// 绑定端口
	if (::bind(m_socket, (sockaddr*)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		return FALSE;
	}

	// 设置socket为窗口通知消息类型
	::WSAAsyncSelect(m_socket, m_hWnd, WM_SOCKET, FD_ACCEPT | FD_CLOSE);
	// 进入监听模式
	::listen(m_socket, 5);

	return TRUE;
}


void CMFCServerDlg::OnBnClickedClear()
{
	m_listInfo.ResetContent();
}


void CMFCServerDlg::OnBnClickedTcp()
{
	if (m_socket == INVALID_SOCKET)  // 开启服务
	{
		// 取得端口号
		CString sPort;
		GetDlgItem(IDC_PORT)->GetWindowText(sPort);
		int nPort = atoi(sPort);
		if (nPort < 1 || nPort > 65535)
		{
			MessageBox("端口号错误！");
			return;
		}

		// 创建监听套节字，使它进入监听状态
		if (!TCPCreateAndListen(nPort))
		{
			MessageBox("启动TCP服务出错！");
			return;
		}

		// 设置相关子窗口控件状态
		GetDlgItem(IDC_TCP)->SetWindowText("停止TCP服务");
		m_bar.SetText("正在监听TCP连接 端口：" + sPort, 1, 0);
	}
	else				// 停止服务
	{
		// 关闭所有连接
		CloseTCPSocket();

		// 设置相关子窗口控件状态
		GetDlgItem(IDC_TCP)->SetWindowText("开启TCP服务");
		m_bar.SetText("TCP服务未开启", 1, 0);
	}
}

void CMFCServerDlg::OnBnClickedUdp()
{
	if (udp_socket == INVALID_SOCKET)  // 开启服务
	{
		// 取得端口号
		CString sPort;
		GetDlgItem(IDC_PORT)->GetWindowText(sPort);
		int nPort = atoi(sPort);
		if (nPort < 1 || nPort > 65535)
		{
			MessageBox("端口号错误！");
			return;
		}
		// 创建套节字，绑定端口
		udp_socket = ::socket(AF_INET, SOCK_DGRAM, 0);
		// 填写要关联的本地地址
		sockaddr_in sin;
		sin.sin_family = AF_INET;
		sin.sin_port = htons(nPort);
		sin.sin_addr.s_addr = INADDR_ANY;
		// 绑定端口
		if (::bind(udp_socket, (sockaddr*)&sin, sizeof(sin)) == SOCKET_ERROR)
		{
			MessageBox("启动UDP服务出错！");
			return;
		}
		// 设置socket为窗口通知消息类型
		::WSAAsyncSelect(udp_socket, m_hWnd, WM_SOCKET, FD_READ | FD_CLOSE);
		// 设置相关子窗口控件状态
		GetDlgItem(IDC_UDP)->SetWindowText("停止UDP服务");
		m_bar.SetText("正在等待UDP数据 端口：" + sPort, 2, 0);
	}

	else				// 停止服务
	{
		// 关闭所有连接
		CloseUDPSocket();
		// 设置相关子窗口控件状态
		GetDlgItem(IDC_UDP)->SetWindowText("开启UDP服务");
		m_bar.SetText("UDP服务未开启", 2, 0);
	}
}

long CMFCServerDlg::OnSocket(WPARAM wParam, LPARAM lParam)
{
	// 取得有事件发生的套节字句柄
	SOCKET s = wParam;
	// 查看是否出错
	if (WSAGETSELECTERROR(lParam))
	{
		RemoveClient(s);
		::closesocket(s);
		return 0;
	}
	// 处理发生的事件
	switch (WSAGETSELECTEVENT(lParam))
	{
	case FD_ACCEPT:		// 监听中的套接字检测到有连接进入
	{
							if (m_nClient < MAX_SOCKET)
							{
								// 接受连接请求，新的套节字client是新连接的套节字
								SOCKET client = ::accept(s, NULL, NULL);
								// 设置新的套节字为窗口通知消息类型
								int i = ::WSAAsyncSelect(client,
									m_hWnd, WM_SOCKET, FD_READ | FD_WRITE | FD_CLOSE);
								AddClient(client);
								DisplayClient();

							}
							else
							{
								MessageBox("连接客户太多！");
							}
	}
		break;

	case FD_CLOSE:		// 检测到套接字对应的连接被关闭。
	{
							RemoveClient(s);

							DisplayClient();


							::closesocket(s);
	}
		break;

	case FD_READ:		// 套接字接受到对方发送过来的数据包
	{																												
								// 取得对方的IP地址和端口号（使用getpeername函数）
								// Peer对方的地址信息
								sockaddr_in sockAddr;
								memset(&sockAddr, 0, sizeof(sockAddr));
								int nSockAddrLen = sizeof(sockAddr);
								char szText[1024] = { 0 };
								hostent* pHost;
								CString sPeerIP;
								char szHostName[256];

								if (!::getpeername(s, (SOCKADDR*)&sockAddr, &nSockAddrLen))
								{
									sPeerIP = ::inet_ntoa(sockAddr.sin_addr);
									// 取得对方的主机名称
									// 取得网络字节顺序的IP值
									DWORD dwIP = ::inet_addr(sPeerIP);
									// 获取主机名称，注意其中第一个参数的转化
									pHost = ::gethostbyaddr((LPSTR)&dwIP, 4, AF_INET);								
									strncpy_s(szHostName, pHost->h_name, 256);
									// 接受真正的网络数据					
									::recv(s, szText, 1024, 0); 									
								}
								else
								{
									::recvfrom(s, szText, 1024, 0, (sockaddr*)&sockAddr, (int*)&nSockAddrLen) > 0;
									sPeerIP = ::inet_ntoa(sockAddr.sin_addr);

									// 取得对方的主机名称
									// 取得网络字节顺序的IP值
									DWORD dwIP = ::inet_addr(sPeerIP);
									// 获取主机名称，注意其中第一个参数的转化
									pHost = ::gethostbyaddr((LPSTR)&dwIP, 4, AF_INET);				
									strncpy_s(szHostName, pHost->h_name, 256);
								}		
								
								CString strItem = CString(szHostName) + "[" + sPeerIP + "]: " + CString(szText);
								m_listInfo.InsertString(-1, strItem);
							
	}
		break;
	}
	return 0;
}


BOOL CMFCServerDlg::AddClient(SOCKET s)
{
	if (m_nClient < MAX_SOCKET)
	{
		// 添加新的成员
		m_arClient[m_nClient++] = s;
		return TRUE;
	}
	return FALSE;
}

void CMFCServerDlg::RemoveClient(SOCKET s)
{
	BOOL bFind = FALSE;
	int i = 0;
	for (; i < m_nClient; i++)
	{
		if (m_arClient[i] == s)
		{
			bFind = TRUE;
			break;
		}
	}

	// 如果找到就将此成员从列表中移除
	if (bFind)
	{

		// 将此成员后面的成员都向前移动一个单位
		for (int j = i; j < m_nClient; j++)
		{
			m_arClient[j] = m_arClient[j + 1];
		}
		m_nClient--;
	}
}

void CMFCServerDlg::DisplayClient(){

	m_list.ResetContent();

	for (int i = 0; i < m_nClient; i++){

		// 取得对方的IP地址和端口号（使用getpeername函数）
		// Peer对方的地址信息
		sockaddr_in sockAddr;
		memset(&sockAddr, 0, sizeof(sockAddr));
		int nSockAddrLen = sizeof(sockAddr);
		::getpeername(m_arClient[i], (SOCKADDR*)&sockAddr, &nSockAddrLen);
		// 转化为主机字节顺序
		int nPeerPort = ::ntohs(sockAddr.sin_port);
		// 转化为字符串IP
		CString sPeerIP = ::inet_ntoa(sockAddr.sin_addr);

		// 取得对方的主机名称
		// 取得网络字节顺序的IP值
		DWORD dwIP = ::inet_addr(sPeerIP);

		// 显示给用户
		CString strItem = sPeerIP;

		m_list.InsertString(-1, strItem);
	}
}

void CMFCServerDlg::CloseUDPSocket()
{
	//关闭UDP等待连接
	if (udp_socket != INVALID_SOCKET)
	{
		::closesocket(udp_socket);
		udp_socket = INVALID_SOCKET;
	}
}

void CMFCServerDlg::CloseTCPSocket()
{
	// 关闭监听套节字
	if (m_socket != INVALID_SOCKET)
	{
		::closesocket(m_socket);
		m_socket = INVALID_SOCKET;
	}

	// 关闭所有客户的连接
	for (int i = 0; i < m_nClient; i++)
	{
		::closesocket(m_arClient[i]);
	}
	m_nClient = 0;
}