
// MFCClient.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include <afxwin.h>	
#include <afxcmn.h>
#include <winsock2.h> 

// ������������WS2_32������
#pragma comment(lib,"WS2_32.lib")

// CMFCClientApp: 
// �йش����ʵ�֣������ MFCClient.cpp
//

class CMFCClientApp : public CWinApp
{
public:
	CMFCClientApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMFCClientApp theApp;