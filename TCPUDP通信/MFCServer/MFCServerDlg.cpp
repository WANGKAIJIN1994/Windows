
// MFCServerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFCServer.h"
#include "MFCServerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
CString sIP;
//���������¼�֪ͨ��Ϣ
#define WM_SOCKET WM_USER + 1	
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	// ʵ��
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


// CMFCServerDlg �Ի���



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


// CMFCServerDlg ��Ϣ�������

BOOL CMFCServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	m_bar.Create(WS_CHILD | WS_VISIBLE | SBS_SIZEGRIP, CRect(0, 0, 0, 0), this, 101);
	m_bar.SetBkColor(RGB(0x66, 0xcc, 0xff));		// ����ɫ
	int arWidth[3] = { 300, 500, -1};
	m_bar.SetParts(3, arWidth);				// ����
	m_bar.SetText(" ����", 0, 0);
	m_bar.SetText(" UDP����δ������", 2, 0);
	m_bar.SetText(" TCP����δ������", 1, 0);
	// �����б��ؼ���m_list����Ĺ���
	m_listInfo.SubclassDlgItem(IDC_LIST1, this);
	m_list.SubclassDlgItem(IDC_LIST2, this);

	// ��ʼ�������׽��ֺ������б�
	m_socket = INVALID_SOCKET;
	udp_socket = INVALID_SOCKET;
	m_nClient = 0;

	// ������ȡ�ñ���IP��ַ�Ĺ��̣�������ʾ��״̬���ĵ�һ��������
	// ȡ�ñ�������	
	char szHost[256];
	::gethostname(szHost, 256);
	// ͨ����������ȡ�õ�ַ��Ϣ
	HOSTENT* pHost = gethostbyname(szHost);
	if (pHost != NULL)
	{
		// �õ���һ��IP��ַ
		in_addr *addr = (in_addr*)*(pHost->h_addr_list);

		// ��ʾ���û�
		sIP.Format(" %s", inet_ntoa(addr[0]));
		m_bar.SetText("����IP��" + sIP, 0, 0);
	}

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMFCServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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

	// �����׽���
	m_socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_socket == INVALID_SOCKET)
		return FALSE;

	// ��дҪ�����ı��ص�ַ
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(nPort);
	sin.sin_addr.s_addr = INADDR_ANY;
	// �󶨶˿�
	if (::bind(m_socket, (sockaddr*)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		return FALSE;
	}

	// ����socketΪ����֪ͨ��Ϣ����
	::WSAAsyncSelect(m_socket, m_hWnd, WM_SOCKET, FD_ACCEPT | FD_CLOSE);
	// �������ģʽ
	::listen(m_socket, 5);

	return TRUE;
}


void CMFCServerDlg::OnBnClickedClear()
{
	m_listInfo.ResetContent();
}


void CMFCServerDlg::OnBnClickedTcp()
{
	if (m_socket == INVALID_SOCKET)  // ��������
	{
		// ȡ�ö˿ں�
		CString sPort;
		GetDlgItem(IDC_PORT)->GetWindowText(sPort);
		int nPort = atoi(sPort);
		if (nPort < 1 || nPort > 65535)
		{
			MessageBox("�˿ںŴ���");
			return;
		}

		// ���������׽��֣�ʹ���������״̬
		if (!TCPCreateAndListen(nPort))
		{
			MessageBox("����TCP�������");
			return;
		}

		// ��������Ӵ��ڿؼ�״̬
		GetDlgItem(IDC_TCP)->SetWindowText("ֹͣTCP����");
		m_bar.SetText("���ڼ���TCP���� �˿ڣ�" + sPort, 1, 0);
	}
	else				// ֹͣ����
	{
		// �ر���������
		CloseTCPSocket();

		// ��������Ӵ��ڿؼ�״̬
		GetDlgItem(IDC_TCP)->SetWindowText("����TCP����");
		m_bar.SetText("TCP����δ����", 1, 0);
	}
}

void CMFCServerDlg::OnBnClickedUdp()
{
	if (udp_socket == INVALID_SOCKET)  // ��������
	{
		// ȡ�ö˿ں�
		CString sPort;
		GetDlgItem(IDC_PORT)->GetWindowText(sPort);
		int nPort = atoi(sPort);
		if (nPort < 1 || nPort > 65535)
		{
			MessageBox("�˿ںŴ���");
			return;
		}
		// �����׽��֣��󶨶˿�
		udp_socket = ::socket(AF_INET, SOCK_DGRAM, 0);
		// ��дҪ�����ı��ص�ַ
		sockaddr_in sin;
		sin.sin_family = AF_INET;
		sin.sin_port = htons(nPort);
		sin.sin_addr.s_addr = INADDR_ANY;
		// �󶨶˿�
		if (::bind(udp_socket, (sockaddr*)&sin, sizeof(sin)) == SOCKET_ERROR)
		{
			MessageBox("����UDP�������");
			return;
		}
		// ����socketΪ����֪ͨ��Ϣ����
		::WSAAsyncSelect(udp_socket, m_hWnd, WM_SOCKET, FD_READ | FD_CLOSE);
		// ��������Ӵ��ڿؼ�״̬
		GetDlgItem(IDC_UDP)->SetWindowText("ֹͣUDP����");
		m_bar.SetText("���ڵȴ�UDP���� �˿ڣ�" + sPort, 2, 0);
	}

	else				// ֹͣ����
	{
		// �ر���������
		CloseUDPSocket();
		// ��������Ӵ��ڿؼ�״̬
		GetDlgItem(IDC_UDP)->SetWindowText("����UDP����");
		m_bar.SetText("UDP����δ����", 2, 0);
	}
}

long CMFCServerDlg::OnSocket(WPARAM wParam, LPARAM lParam)
{
	// ȡ�����¼��������׽��־��
	SOCKET s = wParam;
	// �鿴�Ƿ����
	if (WSAGETSELECTERROR(lParam))
	{
		RemoveClient(s);
		::closesocket(s);
		return 0;
	}
	// ���������¼�
	switch (WSAGETSELECTEVENT(lParam))
	{
	case FD_ACCEPT:		// �����е��׽��ּ�⵽�����ӽ���
	{
							if (m_nClient < MAX_SOCKET)
							{
								// �������������µ��׽���client�������ӵ��׽���
								SOCKET client = ::accept(s, NULL, NULL);
								// �����µ��׽���Ϊ����֪ͨ��Ϣ����
								int i = ::WSAAsyncSelect(client,
									m_hWnd, WM_SOCKET, FD_READ | FD_WRITE | FD_CLOSE);
								AddClient(client);
								DisplayClient();

							}
							else
							{
								MessageBox("���ӿͻ�̫�࣡");
							}
	}
		break;

	case FD_CLOSE:		// ��⵽�׽��ֶ�Ӧ�����ӱ��رա�
	{
							RemoveClient(s);

							DisplayClient();


							::closesocket(s);
	}
		break;

	case FD_READ:		// �׽��ֽ��ܵ��Է����͹��������ݰ�
	{																												
								// ȡ�öԷ���IP��ַ�Ͷ˿ںţ�ʹ��getpeername������
								// Peer�Է��ĵ�ַ��Ϣ
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
									// ȡ�öԷ�����������
									// ȡ�������ֽ�˳���IPֵ
									DWORD dwIP = ::inet_addr(sPeerIP);
									// ��ȡ�������ƣ�ע�����е�һ��������ת��
									pHost = ::gethostbyaddr((LPSTR)&dwIP, 4, AF_INET);								
									strncpy_s(szHostName, pHost->h_name, 256);
									// ������������������					
									::recv(s, szText, 1024, 0); 									
								}
								else
								{
									::recvfrom(s, szText, 1024, 0, (sockaddr*)&sockAddr, (int*)&nSockAddrLen) > 0;
									sPeerIP = ::inet_ntoa(sockAddr.sin_addr);

									// ȡ�öԷ�����������
									// ȡ�������ֽ�˳���IPֵ
									DWORD dwIP = ::inet_addr(sPeerIP);
									// ��ȡ�������ƣ�ע�����е�һ��������ת��
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
		// ����µĳ�Ա
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

	// ����ҵ��ͽ��˳�Ա���б����Ƴ�
	if (bFind)
	{

		// ���˳�Ա����ĳ�Ա����ǰ�ƶ�һ����λ
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

		// ȡ�öԷ���IP��ַ�Ͷ˿ںţ�ʹ��getpeername������
		// Peer�Է��ĵ�ַ��Ϣ
		sockaddr_in sockAddr;
		memset(&sockAddr, 0, sizeof(sockAddr));
		int nSockAddrLen = sizeof(sockAddr);
		::getpeername(m_arClient[i], (SOCKADDR*)&sockAddr, &nSockAddrLen);
		// ת��Ϊ�����ֽ�˳��
		int nPeerPort = ::ntohs(sockAddr.sin_port);
		// ת��Ϊ�ַ���IP
		CString sPeerIP = ::inet_ntoa(sockAddr.sin_addr);

		// ȡ�öԷ�����������
		// ȡ�������ֽ�˳���IPֵ
		DWORD dwIP = ::inet_addr(sPeerIP);

		// ��ʾ���û�
		CString strItem = sPeerIP;

		m_list.InsertString(-1, strItem);
	}
}

void CMFCServerDlg::CloseUDPSocket()
{
	//�ر�UDP�ȴ�����
	if (udp_socket != INVALID_SOCKET)
	{
		::closesocket(udp_socket);
		udp_socket = INVALID_SOCKET;
	}
}

void CMFCServerDlg::CloseTCPSocket()
{
	// �رռ����׽���
	if (m_socket != INVALID_SOCKET)
	{
		::closesocket(m_socket);
		m_socket = INVALID_SOCKET;
	}

	// �ر����пͻ�������
	for (int i = 0; i < m_nClient; i++)
	{
		::closesocket(m_arClient[i]);
	}
	m_nClient = 0;
}