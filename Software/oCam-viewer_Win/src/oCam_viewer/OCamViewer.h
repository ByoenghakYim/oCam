
// OCamViewer.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// COCamViewerApp:
// �� Ŭ������ ������ ���ؼ��� OCamViewer.cpp�� �����Ͻʽÿ�.
//

class COCamViewerApp : public CWinApp
{
public:
	COCamViewerApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern COCamViewerApp theApp;