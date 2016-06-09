
// OCamViewerDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "OCamViewer.h"
#include "OCamViewerDlg.h"
#include "afxdialogex.h"
#include "ImgProc.h"
#include <mmsystem.h>
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"") 
#pragma comment(lib, "winmm.lib")

int g_Resolution[][3] = {
	{2592, 1944, 375},
	{2592, 1944, 750},
	{1920,1080, 750},
	{1920,1080, 1500},
	{1280,960, 1500},
	{1280,960, 3000},
	{1280,720, 1500},
	{1280,720, 3000},
	{640,480, 3000},
	{640,480, 6000},
	{640,480, 9000},
	{640,480, 12000},
	{320,240, 3000},
	{320,240, 6000},
	{320,240, 9000},
	{320,240, 12000},
};
int g_Sel;

int g_WIDTH  = 1280;
int g_HEIGHT = 720;
double g_TPF = 30.0;


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// COCamViewerDlg ��ȭ ����

COCamViewerDlg::COCamViewerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(COCamViewerDlg::IDD, pParent)
	, m_Width(g_WIDTH)
	, m_Height(g_HEIGHT)
    , m_Fps(g_TPF)
    , ptrCam(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void COCamViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_WIDTH, m_Width);
	DDX_Text(pDX, IDC_EDIT_HEIGHT, m_Height);
	DDX_Control(pDX, IDC_COMBO_RESOLUTION, m_cbResolution);
}

BEGIN_MESSAGE_MAP(COCamViewerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_PLAY, &COCamViewerDlg::OnBnClickedButtonPlay)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &COCamViewerDlg::OnBnClickedButtonStop)
	ON_CBN_SELCHANGE(IDC_COMBO_RESOLUTION, &COCamViewerDlg::OnCbnSelchangeComboResolution)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_IMAGE, &COCamViewerDlg::OnBnClickedButtonSaveImage)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// COCamViewerDlg �޽��� ó����
static COCamViewerDlg* ptrOCamViewerDlg = NULL;
void CallbackFunction(void* data) 
{
    if (ptrOCamViewerDlg != NULL) {
        ptrOCamViewerDlg->CallbackProc(data);
    }
}

BOOL COCamViewerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
#ifdef _DEBUG
	AllocConsole(); // �ܼ��� Ȱ��ȭ ��Ų��. (printf ���� �⺻ ����� Ȯ���� ����)
    freopen("CONOUT$", "wt", stdout);   // stdout redirection
#endif

	for (int i=0; i<sizeof(g_Resolution)/12; i++)
	{
		CString str;
		if (g_Resolution[i][2]%100 == 0)
			str.Format("%4d x%4d %3.00ffps", g_Resolution[i][0], g_Resolution[i][1], g_Resolution[i][2]/100.0);
		else
			str.Format("%4d x%4d %3.02ffps", g_Resolution[i][0], g_Resolution[i][1], g_Resolution[i][2]/100.0);
		m_cbResolution.AddString(str);
	}

	CWinApp* pApp = AfxGetApp();
	int sel = pApp->GetProfileInt("Set", "Sel", 0);

	m_cbResolution.SetCurSel(sel);
	OnCbnSelchangeComboResolution();

	(CWnd*)GetDlgItem(IDC_BUTTON_PLAY)->EnableWindow(TRUE);
	(CWnd*)GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);

    ptrOCamViewerDlg = this;

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void COCamViewerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void COCamViewerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR COCamViewerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void COCamViewerDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

    ptrOCamViewerDlg = NULL;

	int sel = m_cbResolution.GetCurSel();

	CWinApp* pApp = AfxGetApp();
	pApp->WriteProfileInt("Set", "Sel", sel);

#ifdef _DEBUG
	FreeConsole();  // Debug message�� ���� ����� �ܼ��� ������
#endif
}

void COCamViewerDlg::CallbackProc(void* data) 
{
    DisplayFPS();

    BYTE* src = (BYTE*)data;
    BYTE* dst = (BYTE*)m_Image.GetPtr1D();

    YUV2RGB(src, dst, m_Image.GetWidth()*m_Image.GetHeight(), 0, 1, 2, 3);

    m_Display.Display(m_Image);
    m_Display.ShowWindow(SW_SHOW);
}


void COCamViewerDlg::OnBnClickedButtonPlay()
{
    int camNum = GetConnectedCamNumber();

    for (int i=0; i < camNum; i++) {
        ptrCam = CamOpen(i, m_Width, m_Height, m_Fps, CallbackFunction);
        if (ptrCam != NULL) {
            m_CamID = i;
            break;
        }
    }

    // �̹��� ���� �޸� �Ҵ�
    m_Image.Alloc(m_Width, m_Height, MV_RGB24);

    if (CamStart(ptrCam) == 0) {
        return;
    }

    // TODO: Add your control notification handler code here
	(CWnd*)GetDlgItem(IDC_BUTTON_PLAY)->EnableWindow(FALSE);
	(CWnd*)GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);

	UpdateData(TRUE);

    SetWindowText((std::string("oCam ID: ") + std::to_string(static_cast<long double>(m_CamID))).c_str());
}

void COCamViewerDlg::OnBnClickedButtonStop()
{
	// TODO: Add your control notification handler code here
	(CWnd*)GetDlgItem(IDC_BUTTON_PLAY)->EnableWindow(TRUE);
	(CWnd*)GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);

    CamStop(ptrCam);
    CamClose(ptrCam);
}


void COCamViewerDlg::DisplayFPS()
{
    static DWORD t;
    static int count;

    if (timeGetTime() - t > 1000) {
        std::string temp = std::string("oCam ID: ") + std::to_string(static_cast<long double>(m_CamID));
        temp += " RESOLUTION: " + std::to_string(static_cast<long double>(m_Width)) + " x " + std::to_string(static_cast<long double>(m_Height));
        temp += " FPS: " + std::to_string(static_cast<long double>(count));
        m_Display.SetWindowTextA(temp.c_str()); // �̹����� ����ϴ� â�� fps ǥ��
        t = timeGetTime();
        count = 0;
    }
    else {
        count++;
    }
}

void COCamViewerDlg::OnCbnSelchangeComboResolution()
{
	// TODO: Add your control notification handler code here
	int num = m_cbResolution.GetCurSel();

	m_Width  = g_Resolution[num][0];
	m_Height = g_Resolution[num][1];
	m_Fps    = static_cast<double>(g_Resolution[num][2]) / 100.0;
	UpdateData(FALSE);
}

void COCamViewerDlg::OnBnClickedButtonSaveImage()
{
	// TODO: Add your control notification handler code here
	wImage image;
	image = m_Image;

	CFileDialog dlg(FALSE, _T("bmp"),_T("image"),OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
	_T("bmp FILE (*.bmp)|*.bmp|ALL FILE (*.*)|*.*|") );

	if (dlg.DoModal() == IDOK) 
	{
		CString file = dlg.GetPathName();
		image.Save(file);
	}
}
