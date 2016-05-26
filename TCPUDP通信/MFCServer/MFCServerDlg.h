
// MFCServerDlg.h : ͷ�ļ�
//

#pragma once
#define MAX_SOCKET 56	// ����˷��������ܽ��ܵ����ͻ���

// CMFCServerDlg �Ի���
class CMFCServerDlg : public CDialogEx
{
// ����
public:
	CMFCServerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MFCSERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	// ����TCP�׽��֣�������Ϊ����״̬��׼�����ܿͻ�������
	BOOL TCPCreateAndListen(int nPort);
	// �ر������׽��֣����������׽��ֺ�����accept�������ص��׽���
	void CloseTCPSocket();
	// ��ͻ������б������һ���ͻ�
	void CloseUDPSocket();
	// ��ͻ������б������һ���ͻ�
	BOOL AddClient(SOCKET s);
	// �ӿͻ������б����ƴ�һ���ͻ�
	void RemoveClient(SOCKET s);
	//��ʾ��ǰ���ӵ����пͻ���
	void DisplayClient();

protected:
	// �����Ӵ��ڿؼ���һ����״̬�����������б��
	CStatusBarCtrl m_bar;
	CListBox m_listInfo;
	CListBox m_list;

	// �����׽��־��
	SOCKET m_socket;
	SOCKET udp_socket;

	// �ͻ������б�
	SOCKET m_arClient[MAX_SOCKET];	// �׽�������
	int m_nClient;			// ��������Ĵ�С


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	// ������ֹͣTCP����
	afx_msg void OnTCPStart();
	// ������ֹͣUDP����
	afx_msg void OnUDPStart();
	// �����Ϣ
	afx_msg void OnClear();
	// �׽���֪ͨ�¼�
	afx_msg long OnSocket(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedExit();
	afx_msg void OnBnClickedClear();
	afx_msg void OnBnClickedTcp();
	afx_msg void OnBnClickedUdp();
};
