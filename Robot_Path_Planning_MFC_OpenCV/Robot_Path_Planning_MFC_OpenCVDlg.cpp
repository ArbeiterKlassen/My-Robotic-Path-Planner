
// Robot_Path_Planning_MFC_OpenCVDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "Robot_Path_Planning_MFC_OpenCV.h"
#include "Robot_Path_Planning_MFC_OpenCVDlg.h"
#include "afxdialogex.h"
#include "Resource.h"
#include <opencv2/opencv.hpp>
#include "Operation_Page.h"
#include "CMyButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP();
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CRobotPathPlanningMFCOpenCVDlg 对话框

LRESULT CRobotPathPlanningMFCOpenCVDlg::OnNcHitTest(CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	UINT nHitTest = CDialogEx::OnNcHitTest(point);
	CRect rect;
	GetClientRect(&rect);
	rect.bottom = 70;
	//函数参数point是相对于屏幕坐标的，需要将其转换为
	//客户区坐标才能使用PtInRect()，否则会因为坐标的不同使判断失误
	//rect.left = rect.left + 200;
	ScreenToClient(&point);
	if (rect.PtInRect(point))
	{
		if (HTCLIENT == nHitTest)
			nHitTest = HTCAPTION;
		//如果鼠标点中的是关闭按钮的位置，需要将上一步的设置还原，
		if (m_rtBtnfile.PtInRect(point) || m_rtBtnSelect.PtInRect(point) || m_rtBtnHelp.PtInRect(point))
		{
			nHitTest = HTCLIENT;
		}
	}
	return nHitTest;
}

CRobotPathPlanningMFCOpenCVDlg::CRobotPathPlanningMFCOpenCVDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ROBOT_PATH_PLANNING_MFC_OPENCV_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRobotPathPlanningMFCOpenCVDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT3, Random_Width);
	DDX_Control(pDX, IDC_EDIT4, Random_Height);
	DDX_Control(pDX, IDC_EDIT5, Random_Length);
	DDX_Control(pDX, IDC_EDIT1, Import_Local_File);
	DDX_Control(pDX, IDC_EDIT2, Import_URL_File_URL);
	DDX_Control(pDX, IDC_EDIT6, Import_URL_File_Port);
	DDX_Control(pDX, IDC_BUTTON8, m_button_exit);
}

BEGIN_MESSAGE_MAP(CRobotPathPlanningMFCOpenCVDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_NCHITTEST()
	ON_BN_CLICKED(IDC_BUTTON4, &CRobotPathPlanningMFCOpenCVDlg::OnBnClicked_RandomSummon)
	ON_BN_CLICKED(IDC_BUTTON1, &CRobotPathPlanningMFCOpenCVDlg::OnBnClicked_Start_Import_Local)
	ON_BN_CLICKED(IDC_BUTTON2, &CRobotPathPlanningMFCOpenCVDlg::OnBnClicked_Start_Import_URL)
	ON_BN_CLICKED(IDC_BUTTON3, &CRobotPathPlanningMFCOpenCVDlg::OnBnClicked_Confirm_Start)
	ON_BN_CLICKED(IDC_BUTTON6, &CRobotPathPlanningMFCOpenCVDlg::OnBnClicked_Rechoose_File)
	ON_BN_CLICKED(IDC_BUTTON8, &CRobotPathPlanningMFCOpenCVDlg::OnBnClickedButton8)
END_MESSAGE_MAP()


// CRobotPathPlanningMFCOpenCVDlg 消息处理程序
BOOL CRobotPathPlanningMFCOpenCVDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//退出按钮绘制
	CRect rtBtnClo;
	GetClientRect(&rtBtnClo);
	rtBtnClo.left = rtBtnClo.right - 20;
	m_button_exit.SetImagePath(_T(".\\res\\icon_popup_off.png"), _T(".\\res\\icon_popup_off.png"), _T(".\\res\\icon_popup_off.png"));
	m_button_exit.InitMyButton(rtBtnClo.left, 5, 16, 16, true);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}



void CRobotPathPlanningMFCOpenCVDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CRobotPathPlanningMFCOpenCVDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this);
		CRect rect;
		//------------------
		//修改字体
		HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);  //获取系统默认GUI字体
		LOGFONT logfont;
		GetObject(hFont, sizeof(LOGFONT), &logfont);
		wcscpy_s(logfont.lfFaceName, L"黑体");//改变为宋体
		//logfont.lfWeight = 60;
		logfont.lfHeight = 15;
		HFONT hNewFont = NULL;
		hNewFont = CreateFontIndirect(&logfont); //改变系统默认的字体，设为宋体，创建了一个新的HFONT 
		HFONT loldfont = (HFONT)(SelectObject(dc, hNewFont));//选上新创建的这个，返回的是旧的
		CRect tmprect;
		GetClientRect(&rect);
		//rect.bottom = 60;
		rect.bottom *= 0.1;
		/*int nCount = 165 - 115 + 186 - 158 + 190 - 115;
		int nIncrecs = (rect.right - rect.left) / nCount;*/
		dc.FillSolidRect(rect, RGB(45, 51, 60));
		dc.SetBkMode(TRANSPARENT);
	}
}

HBRUSH CRobotPathPlanningMFCOpenCVDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	switch (nCtlColor)
	{
	case CTLCOLOR_DLG:
		HBRUSH aBrush;
		aBrush = CreateSolidBrush(RGB(30, 34, 39));
		hbr = aBrush;
		break;
	}

	if (nCtlColor == CTLCOLOR_STATIC)
	{
		if (pWnd->GetDlgCtrlID() == IDC_STATIC)//如果是静态编辑框
		{
			pDC->SetTextColor(RGB(255, 255, 255));//修改字体的颜色
			pDC->SetBkColor(RGB(30, 34, 39));//把字体的背景变成透明的
			return (HBRUSH)::GetStockObject(NULL_BRUSH);
		}
	}
	return hbr;
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CRobotPathPlanningMFCOpenCVDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

namespace mymethod {
	unsigned int str_to_int(CString str) {
		std::string sstr = CT2A(str.GetString());
		unsigned int length = str.GetLength();
		unsigned int result = 0;
		for (int i = 0; i < length; i++)
		{
			result += (sstr[length - i - 1] - 48) * pow(10, i);
		}
		return result;
	}
}


CString Out_Import_URL_Port = _T("");
CString Out_Import_URL_IP = _T("");
CString Out_Import_Local_File = _T("");
unsigned int Out_W_Random = 0, Out_H_Random = 0, Out_L_Random = 0;
unsigned int confirm_mode = 0;
// 用于Confirm的宏变量

void CRobotPathPlanningMFCOpenCVDlg::OnBnClicked_Start_Import_Local()
{
	// TODO: Add your control notification handler code here   
// 设置过滤器   
	TCHAR szFilter[] = _T("CSV文件(*.csv)|*.csv|所有文件(*.*)|*.*||");
	// 构造打开文件对话框
	CString filePath = _T("");
	Out_Import_Local_File = filePath;
	CFileDialog fileDlg(TRUE, _T("csv"), NULL, 0, szFilter, this);
	// 显示打开文件对话框   
	if (IDOK == fileDlg.DoModal())
	{
		// 如果点击了文件对话框上的“打开”按钮，则将选择的文件路径显示到编辑框里   
		filePath = fileDlg.GetPathName();
		Out_Import_Local_File = filePath;
		Import_Local_File.SetWindowTextW(filePath);
		MessageBox(_T("导入本地文件成功：")+filePath);

		Random_Height.SetWindowTextW(_T(""));
		Random_Width.SetWindowTextW(_T(""));
		Random_Length.SetWindowTextW(_T(""));
		Random_Height.SetReadOnly(true);
		Random_Width.SetReadOnly(true);
		Random_Length.SetReadOnly(true);
		GetDlgItem(IDC_BUTTON4)->EnableWindow(false);

		Import_URL_File_URL.SetWindowTextW(_T(""));
		Import_URL_File_Port.SetWindowTextW(_T(""));
		Import_URL_File_URL.SetReadOnly(true);
		Import_URL_File_Port.SetReadOnly(true);
		GetDlgItem(IDC_BUTTON2)->EnableWindow(false);
		confirm_mode = 1;
		GetDlgItem(IDC_BUTTON1)->EnableWindow(false);
	}
}



void CRobotPathPlanningMFCOpenCVDlg::OnBnClicked_Start_Import_URL()
{
	CString urlPath, port;
	Import_URL_File_URL.GetWindowTextW(urlPath);
	Import_URL_File_Port.GetWindowTextW(port);
	if (urlPath == _T(""))MessageBox(_T("请输入URL!"));
	if (port == _T(""))MessageBox(_T("请输入端口!"));
	if (urlPath != _T("") && port != _T("")) {
		Out_Import_URL_IP = urlPath;
		Out_Import_URL_Port = port;
		MessageBox(_T("导入网络文件成功：") + Out_Import_URL_IP + _T(":") + Out_Import_URL_Port);
	}

	Random_Height.SetWindowTextW(_T(""));
	Random_Width.SetWindowTextW(_T(""));
	Random_Length.SetWindowTextW(_T(""));
	Random_Height.SetReadOnly(true);
	Random_Width.SetReadOnly(true);
	Random_Length.SetReadOnly(true);
	GetDlgItem(IDC_BUTTON4)->EnableWindow(false);

	Import_Local_File.SetWindowTextW(_T(""));
	Import_Local_File.SetReadOnly(true);
	GetDlgItem(IDC_BUTTON1)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON2)->EnableWindow(false);
	confirm_mode = 2;
}

void CRobotPathPlanningMFCOpenCVDlg::OnBnClicked_RandomSummon()
{
	CString width, height, length;
	Random_Width.GetWindowTextW(width);
	Random_Height.GetWindowTextW(height);
	Random_Length.GetWindowTextW(length);
	if (width == _T("") || height == _T("") || length == _T("")) { MessageBox(_T("请填写有效的长宽高！")); return; }
	else {
		Out_W_Random = mymethod::str_to_int(width);
		Out_H_Random = mymethod::str_to_int(height);
		Out_L_Random = mymethod::str_to_int(length);
	}

	Import_URL_File_URL.SetWindowTextW(_T(""));
	Import_URL_File_Port.SetWindowTextW(_T(""));
	Import_URL_File_URL.SetReadOnly(true);
	Import_URL_File_Port.SetReadOnly(true);
	GetDlgItem(IDC_BUTTON2)->EnableWindow(false);

	Import_Local_File.SetWindowTextW(_T(""));
	Import_Local_File.SetReadOnly(true);
	GetDlgItem(IDC_BUTTON1)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON4)->EnableWindow(false);
	confirm_mode = 3;
}

void CRobotPathPlanningMFCOpenCVDlg::OnBnClicked_Confirm_Start()
{
	switch (confirm_mode) {
	case 1: {
		Operation_Page* opp = new Operation_Page;
		opp->Create(IDD_OPRE_DIALOG,this);
		opp->ShowWindow(SW_SHOW);
		opp->init(1, Out_Import_Local_File, Out_Import_URL_IP, Out_Import_URL_Port, Out_W_Random, Out_H_Random, Out_L_Random); break; 
	};
	case 2: {
		Operation_Page* opp = new Operation_Page;
		opp->Create(IDD_OPRE_DIALOG, this);
		opp->ShowWindow(SW_SHOW);
		opp->init(2, Out_Import_Local_File, Out_Import_URL_IP, Out_Import_URL_Port, Out_W_Random, Out_H_Random, Out_L_Random); break;
	};
	case 3: {
		Operation_Page* opp = new Operation_Page;
		opp->Create(IDD_OPRE_DIALOG, this);
		opp->ShowWindow(SW_SHOW);
		opp->init(3, Out_Import_Local_File, Out_Import_URL_IP, Out_Import_URL_Port, Out_W_Random, Out_H_Random, Out_L_Random); break;
	};
	default:MessageBox(_T("请选择一种模式：从本地打开/从网络打开/指定尺寸随机生成"));
	}
}


void CRobotPathPlanningMFCOpenCVDlg::OnBnClicked_Rechoose_File()
{
	GetDlgItem(IDC_BUTTON1)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON2)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON4)->EnableWindow(true);

	Random_Height.SetReadOnly(false);
	Random_Width.SetReadOnly(false);
	Random_Length.SetReadOnly(false);
	Import_Local_File.SetReadOnly(false);
	Import_URL_File_Port.SetReadOnly(false);
	Import_URL_File_URL.SetReadOnly(false);

	Random_Height.SetWindowTextW(_T(""));
	Random_Width.SetWindowTextW(_T(""));
	Random_Length.SetWindowTextW(_T(""));
	Import_Local_File.SetWindowTextW(_T(""));
	Import_URL_File_URL.SetWindowTextW(_T(""));
	Import_URL_File_Port.SetWindowTextW(_T(""));
	confirm_mode = 0;
}


void CRobotPathPlanningMFCOpenCVDlg::OnBnClickedButton8()
{
	CDialog::OnCancel();
}
