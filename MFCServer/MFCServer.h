
// MFCServer.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
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

// CMFCServerApp: 
// �йش����ʵ�֣������ MFCServer.cpp
//


class CMFCServerApp : public CWinApp
{
public:
	CMFCServerApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMFCServerApp theApp;