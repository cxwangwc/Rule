
// FlowMFCCall.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CFlowMFCCallApp:
// �йش����ʵ�֣������ FlowMFCCall.cpp
//

class CFlowMFCCallApp : public CWinApp
{
public:
	CFlowMFCCallApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CFlowMFCCallApp theApp;