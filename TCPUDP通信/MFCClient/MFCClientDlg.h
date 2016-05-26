
// MFCClientDlg.h : ͷ�ļ�
//
#pragma once


// CMFCClientDlg �Ի���
class CMFCClientDlg : public CDialogEx
{
// ����
public:
	CMFCClientDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MFCCLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

protected:
	// ״̬���Ӵ��ڿؼ�
	CStatusBarCtrl m_bar;
	// �����������ȡ�����ӵ��׽��־��
	SOCKET m_socket;
	SOCKET udp_socket;
	CListBox m_list;

// ʵ��

protected:
	// ���ӷ�����
	BOOL Connect(LPCTSTR pszRemoteAddr, u_short nPort);
	// ���ı���������ı�
	void AddStringToList(LPCTSTR pszText, BOOL bRecv = TRUE);

protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
