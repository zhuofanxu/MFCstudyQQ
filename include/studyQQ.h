
// studyQQ.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CstudyQQApp:
// �йش����ʵ�֣������ studyQQ.cpp
//

class CstudyQQApp : public CWinApp
{
public:
	CstudyQQApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CstudyQQApp theApp;