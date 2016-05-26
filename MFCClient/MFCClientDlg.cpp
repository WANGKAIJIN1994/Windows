
// MFCClientDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFCClient.h"
#include "MFCClientDlg.h"
#include "afxdialogex.h"

// ���������¼�֪ͨ��Ϣ
#define WM_SOCKET WM_USER + 1




#ifdef _DEBUG
#define new DEBUG_NEW
#endif

sockaddr_in sin_socket;
BOOL isConnect = false;
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


// CMFCClientDlg �Ի���



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


// CMFCClientDlg ��Ϣ�������

BOOL CMFCClientDlg::OnInitDialog()
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

	// ����״̬����������������
	m_bar.Create(WS_CHILD | WS_VISIBLE | SBS_SIZEGRIP, CRect(0, 0, 0, 0), this, 101);
	m_bar.SetBkColor(RGB(0x66, 0xcc, 0xff));		// ����ɫ
	int arWidth[] = { 350, -1 };
	m_bar.SetParts(2, arWidth);				// ����
	m_bar.SetText(" UDP����", 1, 0);	// ��һ�������ı�
	m_bar.SetText(" TCP����", 0, 0);				// �ڶ��������ı�
	// ��ʼ�����Ͱ�ť�ͷ��ͱ༭���״̬
	GetDlgItem(IDC_SEND)->EnableWindow(FALSE);
	GetDlgItem(IDC_INPUT)->EnableWindow(FALSE);

	m_list.SubclassDlgItem(IDC_LIST1, this);

	// ��ʼ�������׽���
	m_socket = INVALID_SOCKET;
	udp_socket = INVALID_SOCKET;

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMFCClientDlg::OnPaint()
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
	if (m_socket == INVALID_SOCKET)  // ���ӷ�����
	{
		// ȡ�÷�������ַ
		CString sAddr;
		GetDlgItem(IDC_ADDR)->GetWindowText(sAddr);
		if (sAddr.IsEmpty())
		{
			MessageBox("�������������ַ��");
			return;
		}

		// ȡ�ö˿ں�
		CString sPort;
		GetDlgItem(IDC_PORT)->GetWindowText(sPort);
		int nPort = atoi(sPort);
		if (nPort < 1 || nPort > 65535)
		{
			MessageBox("�˿ںŴ���");
			return;
		}
		// ��ͼ���ӷ�����
		if (!Connect(sAddr, nPort))
		{
			MessageBox("���ӷ���������");
			return;
		}

		// �����û�����
		isConnect = true;
		GetDlgItem(IDC_CONNECT)->SetWindowText("ȡ��");
		m_bar.SetText(" �������ӡ���", 0, 0);
	}
	else		// �Ͽ�������
	{
		// �ر��׽���
		::closesocket(m_socket);
		m_socket = INVALID_SOCKET;

		// �����û�����
		GetDlgItem(IDC_CONNECT)->SetWindowText("���ӷ�����");
		m_bar.SetText(" ����", 0, 0);
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
	// �����׽���
	m_socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_socket == INVALID_SOCKET)
	{
		return FALSE;
	}

	// ����socketΪ����֪ͨ��Ϣ����
	::WSAAsyncSelect(m_socket, m_hWnd,
		WM_SOCKET, FD_CONNECT | FD_CLOSE | FD_WRITE | FD_READ);

	// �ٶ�szAddr��IP��ַ
	ULONG uAddr = ::inet_addr(pszRemoteAddr);

	// ��д��������ַ��Ϣ
	sockaddr_in remote;
	remote.sin_addr.S_un.S_addr = uAddr;
	remote.sin_family = AF_INET;
	remote.sin_port = htons(nPort);

	// ���ӵ�Զ�̻�
	::connect(m_socket, (sockaddr*)&remote, sizeof(sockaddr));

	return TRUE;
}

void CMFCClientDlg::AddStringToList(LPCTSTR pszText, BOOL bRecv)
{
	CString strEdit;
	GetDlgItem(IDC_LIST1)->GetWindowText(strEdit);

	if (bRecv)
	{
		strEdit += "UDP����δ����������ʧ�ܣ�����";
	}
	else
	{
		strEdit += "��Send����";
		strEdit += pszText;
	}
	m_list.InsertString(-1,strEdit);
}

long CMFCClientDlg::OnSocket(WPARAM wParam, LPARAM lParam)
{
	// ȡ�����¼��������׽��־��
	SOCKET s = wParam;
	// �鿴�Ƿ����
	if (WSAGETSELECTERROR(lParam))
	{
		if (m_socket != SOCKET_ERROR)
			OnBnClickedConnect();
		m_bar.SetText(" ���ӳ���", 0, 0);
		return 0;
	}
	// ���������¼�
	switch (WSAGETSELECTEVENT(lParam))
	{
	case FD_CONNECT:	// �׽�����ȷ�����ӵ�������
	{
							// �����û�����
							GetDlgItem(IDC_CONNECT)->SetWindowText("�Ͽ�����");
							GetDlgItem(IDC_INPUT)->EnableWindow(TRUE);
							GetDlgItem(IDC_SEND)->EnableWindow(TRUE);
							m_bar.SetText(" �Ѿ����ӵ�������", 0, 0);
	}
		break;

	case FD_READ:		// �׽��ֽ��ܵ��Է����͹��������ݰ�
	{
							// �ӷ�������������
							char szText[1024] = { 0 };
							::recv(s, szText, 1024, 0);
							// ��ʾ���û�
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
		// ȡ��Ҫ���͵��ַ���
		CString sText;
		GetDlgItem(IDC_INPUT)->GetWindowText(sText);

		// ���һ�����س����С�
		// ע�⣬����������Ǳ���ģ��������ʹ�ñ������Ϊ�ͻ��˵�������Э�飬
		// ����SMTP��FTP�ȣ���Ҫ������ˡ���Ϊ��ЩЭ�鶼Ҫ��ʹ�á��س����С���Ϊһ������Ľ������
		sText += "\r\n";

		// �������ݵ�������

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
		m_bar.SetText(" UDP�ѿ���", 1, 0);
		GetDlgItem(IDC_SEND)->EnableWindow(TRUE);
		GetDlgItem(IDC_INPUT)->EnableWindow(TRUE);

		udp_socket = ::socket(AF_INET, SOCK_DGRAM, 0);
		::WSAAsyncSelect(udp_socket, m_hWnd,
			WM_SOCKET, FD_CLOSE | FD_READ);

		//ȡ�÷�������ַ
		CString sAddr;
		GetDlgItem(IDC_ADDR)->GetWindowText(sAddr);

		// ȡ�ö˿ں�
		CString sPort;
		GetDlgItem(IDC_PORT)->GetWindowText(sPort);
		int nPort = atoi(sPort);

		if (nPort < 0 || nPort > 65535 || sAddr.IsEmpty() || sPort.IsEmpty())
		{
			m_bar.SetText(" UDP����", 1, 0);	// ��һ�������ı�
			MessageBox("�˿ڻ��ַ����\n����������");
			CButton* pBtn = (CButton*)GetDlgItem(IDC_UDP);
			pBtn->SetCheck(0);		
			return;
		}

		// ��д��������ַ��Ϣ
		sin_socket.sin_addr.S_un.S_addr = inet_addr(sAddr);
		sin_socket.sin_family = AF_INET;
		sin_socket.sin_port = htons(nPort);
	}
	else if (IsDlgButtonChecked(IDC_UDP) == 0 && isConnect == false){
		GetDlgItem(IDC_SEND)->EnableWindow(false);
		GetDlgItem(IDC_INPUT)->EnableWindow(false);
		m_bar.SetText(" UDP����", 1, 0);
	}
	else if (IsDlgButtonChecked(IDC_UDP) == 0)
		m_bar.SetText(" UDP����", 1, 0);
}
