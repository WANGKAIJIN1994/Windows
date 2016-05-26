
// MFCClient.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include <afxwin.h>	
#include <afxcmn.h>
#include <winsock2.h> 

// 告诉连接器与WS2_32库连接
#pragma comment(lib,"WS2_32.lib")

// CMFCClientApp: 
// 有关此类的实现，请参阅 MFCClient.cpp
//

class CMFCClientApp : public CWinApp
{
public:
	CMFCClientApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CMFCClientApp theApp;