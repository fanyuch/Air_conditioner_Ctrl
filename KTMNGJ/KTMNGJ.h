// KTMNGJ.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CKTMNGJApp:
// �йش����ʵ�֣������ KTMNGJ.cpp
//

class CKTMNGJApp : public CWinApp
{
public:
	CKTMNGJApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CKTMNGJApp theApp;
