
// OCamViewerDlg.h : ��� ����
//

#pragma once

#include "wDisplay.h"
#include "wImage.h"
#include "afxwin.h"

#include "libCamCap.h"

// COCamViewerDlg ��ȭ ����
class COCamViewerDlg : public CDialogEx
{
// �����Դϴ�.
public:
	COCamViewerDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.
	
    int			m_CamID;
	int			m_Width;
	int			m_Height;
    double		m_Fps;

	CComboBox	m_cbResolution;

    wImage		m_Image;
    wDisplay	m_Display;

    CAMPTR		ptrCam;

    void		CallbackProc(void* data);
    void		DisplayFPS();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_OCAMVIEWER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonPlay();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnCbnSelchangeComboResolution();
	afx_msg void OnBnClickedButtonSaveImage();
	afx_msg void OnDestroy();
};
