// Operation_Page.cpp: 实现文件
//

#include "pch.h"
#include "Robot_Path_Planning_MFC_OpenCV.h"
#include "afxdialogex.h"
#include "Operation_Page.h"
#include <opencv2/opencv.hpp>
#include<stdio.h>
#include<string>
#include<iostream>
#include<fstream>
#include <opencv2/highgui/highgui_c.h>
#include<queue>
#include"GP.h"
#include"WinINet_Downloader.h"
#include<random>

#define G_BORDER 3
#define G_CONTENT 54
#define WALL_COLOR cv::Scalar(128,128,128)
#define WAY_COLOR cv::Scalar(255,255,255)
#define KEYWAY_COLOR cv::Scalar(250,206,136)
#define NEWWALL_COLOR cv::Scalar(0,0,255)
#define MAP_FONT_COLOR cv::Scalar(0,0,255)
#define START_COLOR cv::Scalar(0,255,0)
#define END_COLOR cv::Scalar(255,0,0)
//格子颜色
#define _LEFT 0
#define _UP 1
#define _RIGHT 2
#define _DOWN 3
#define _FRONT 4
#define _BACK 5
#define _SET -1
//路径方向标记
#define RANDOM_DENSITY 0.72
#define RANDOM_LSCALE 1
#define RANDOM_RSCALE 20
// Operation_Page 对话框

IMPLEMENT_DYNAMIC(Operation_Page, CDialogEx)

Operation_Page::Operation_Page(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_OPRE_DIALOG, pParent)
{

}
Operation_Page::~Operation_Page()
{
}
void Operation_Page::init(int _m, CString _path, CString _ip, CString _port, int _width, int _height, int _length) {
	this->mode = _m;
	this->path = _path;
	this->ip = _ip;
	this->port = _port;
	this->width = _width;
	this->height = _height;
	this->length = _length;
	//从Main到Operation_Page的传参
	
	switch (mode) {
	case 1: Loadmode_1(path); break;
	case 2: Loadmode_2(ip, port); break;
	case 3: Loadmode_3(width, height, length); break;
	default: break;
	}
	return;
}
void Operation_Page::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_INFOBAR, Info_Bar_Cedit);
	DDX_Control(pDX, IDC_EDIT3, Export_CSV_Path);
	DDX_Control(pDX, IDC_EDIT4, Export_BMP_Path);
	DDX_Control(pDX, IDC_EDIT10, Start_X_Cedit);
	DDX_Control(pDX, IDC_EDIT8, Start_Y_Cedit);
	DDX_Control(pDX, IDC_EDIT13, End_X_Cedit);
	DDX_Control(pDX, IDC_EDIT11, End_Y_Cedit);
	DDX_Control(pDX, IDC_EDIT9, Start_Z_Cedit);
	DDX_Control(pDX, IDC_EDIT12, End_Z_Cedit);
	DDX_Control(pDX, IDC_EDIT1, Page_Value);
	DDX_Control(pDX, IDC_EDIT5, Add_Wall_X);
	DDX_Control(pDX, IDC_EDIT6, Add_Wall_Y);
	DDX_Control(pDX, IDC_EDIT7, Add_Wall_Z);
}
void Operation_Page::Info_append(std::string str) {
	CString now;
	CString cstr = CA2T(str.c_str());
	Info_Bar_Cedit.GetWindowTextW(now);
	now = now + _T("\r\n") + cstr;
	Info_Bar_Cedit.SetWindowTextW(now);
	Info_Bar_Cedit.RedrawWindow();
	Info_Bar_Cedit.SetSel(1, 1, FALSE);
	Info_Bar_Cedit.SetFocus();
}
void Operation_Page::Info_clear() {
	Info_Bar_Cedit.SetWindowTextW(_T(""));
	return;
}
std::vector<std::vector<std::vector<double>>> Vmap;//Vector矩阵
cv::Mat Gmap;//Graphic Mat
unsigned int Start_x, Start_y, Start_z;
unsigned int End_x, End_y, End_z;
int default_page = 0;
void Operation_Page::Page_draw_pic(unsigned int page, unsigned int Box_Width, unsigned int Box_Height) {
	int G_Width = Box_Width * width;
	int G_Height = Box_Height * height;
	Gmap = cv::Mat::zeros(cv::Size(G_Height, G_Width), CV_8UC3);
	std::vector<std::vector<double>> page_Vmap = Vmap[default_page];
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++) {
			if (page_Vmap[i][j] > 0) {
				cv::rectangle(
					Gmap,
					cv::Point(j * Box_Width + G_BORDER, i * Box_Height + G_BORDER),
					cv::Point((j + 1) * Box_Width - G_BORDER, (i + 1) * Box_Height - G_BORDER),
					WAY_COLOR,
					-1
				);
				cv::putText(
					Gmap,
					std::to_string((int)page_Vmap[i][j]),
					cv::Point(j * Box_Height + G_CONTENT * 0.4, i * Box_Width + G_CONTENT * 0.7),
					cv::FONT_ITALIC,
					1.2,
					MAP_FONT_COLOR
				);
			}
			else {
				cv::rectangle(
					Gmap,
					cv::Point(j * Box_Width + G_BORDER, i * Box_Height + G_BORDER),
					cv::Point((j + 1) * Box_Width - G_BORDER, (i + 1) * Box_Height - G_BORDER),
					WALL_COLOR,
					-1
				);
			}
		}
	}
	CVMat_to_Pic(Gmap, IDC_STATIC_PIC);
	Info_append("Displaying Successfully!");
}
void Operation_Page::Loadmode_1(CString _path) {
	this->SetWindowTextW(_T("本地导入文件：") + _path);
	Info_append("Loading Successfully!");
	Vmap = read_csv_to_Mat(_path);
	width = Vmap[0][0].size();
	height = Vmap[0].size();
	length = Vmap.size();
	Info_append("width: " + std::to_string(width));
	Info_append("height: " + std::to_string(height));
	Info_append("length: " + std::to_string(length));
	//开始绘图 -> 分层图
	int Box_Width = (2 * G_BORDER + G_CONTENT);
	int Box_Height = (2 * G_BORDER + G_CONTENT);
	Page_draw_pic(default_page, Box_Width, Box_Height);
	return;
};
void Operation_Page::Loadmode_2(CString _url, CString _port) {
	std::string temp_filename = "D:/temp_csv.csv";
	CString linkfile = _url + ":" + _port;
	std::string linkfile_str = CT2A(linkfile.GetBuffer());
	if (!DownloadFile(linkfile_str, temp_filename)) {
		Info_append("Downloading Failed!");
		MessageBox(_T("Downloading Failed!"));
		return;
	}
	CString temp_cfname = CA2T(temp_filename.c_str());
	this->SetWindowTextW(_T("从网络导入：") + _url + _T(":") + _port);
	Info_append("Loading Successfully!");
	Vmap = read_csv_to_Mat(temp_cfname);
	width = Vmap[0][0].size();
	height = Vmap[0].size();
	length = Vmap.size();
	Info_append("Width: " + std::to_string(width));
	Info_append("height: " + std::to_string(height));
	Info_append("length: " + std::to_string(height));
	int Box_Width = (2 * G_BORDER + G_CONTENT);
	int Box_Height = (2 * G_BORDER + G_CONTENT);
	Page_draw_pic(default_page, Box_Width, Box_Height);
	return;
};
void Operation_Page::Loadmode_3(int _w, int _h, int _l) {
	this->SetWindowTextW(_T("随机生成地图"));
	Info_append("Loading Randomization Successfully!");
	Vmap = std::vector<std::vector<std::vector<double>>>(length, std::vector<std::vector<double>>(height, std::vector<double>(width, 0)));
	//初始化矩阵
	width = _w;
	height = _h;
	length = _l;
	std::default_random_engine e;
	std::uniform_int_distribution<int> u(RANDOM_LSCALE, RANDOM_RSCALE); // 左闭右闭区间
	std::uniform_real_distribution<double> c(0, 1);
	e.seed(time(0));
	for (int i = 1; i < length-1; i++)for(int j = 1; j < height - 1; j++)for (int k = 1; k < width - 1; k++) {
		if (c(e) < RANDOM_DENSITY)Vmap[i][j][k] = u(e);
	}
	Info_append("Width: " + std::to_string(width));
	Info_append("height: " + std::to_string(height));
	Info_append("length: " + std::to_string(height));
	int Box_Width = (2 * G_BORDER + G_CONTENT);
	int Box_Height = (2 * G_BORDER + G_CONTENT);
	Page_draw_pic(default_page, Box_Width, Box_Height);
	return;
};
inline unsigned int qread() {
	unsigned int x = 0;int f = 1; char ch = getchar();
	while (ch < 48 || ch > 57)if (ch == 45)f = -1;
	while (ch >= 48 && ch <= 57) {
		x = (x << 3) + (x << 1) + ch - 48;
		ch = getchar();
	}
	return x * f;
}

cv::Mat Vec2Mat(std::vector<std::vector<double>> vec)    //二维Vector转Mat
{
	cv::Mat m(vec.size(), vec[1].size(), CV_8UC1);
	for (int i = 0; i < vec.size(); ++i)
		m.row(i) = cv::Mat(vec[i]).t();
	return m;
}

std::vector<std::vector<std::vector<double>>> Operation_Page::read_csv_to_Mat(CString _path) {
	std::string strpath = CT2A(_path.GetBuffer());
	const char* charpath = strpath.c_str();
	std::string tmp;
	std::ifstream readfile;
	readfile.open(charpath,std::ios::in);
	int nline = 0;//行
	std::vector<double> nullvec{};
	int number;
	Info_append("Map Information: ");
	std::getline(readfile, tmp, '\n');//读入width，height，length
	std::vector<int> sizes;
	std::istringstream iss(tmp);
	int num;
	while (iss >> num) {
		sizes.push_back(num);
	}
	width = sizes[0];
	height = sizes[1];
	length = sizes[2];
	std::vector<std::vector<std::vector<double>>> numbers(length, std::vector<std::vector<double>>(height, std::vector<double>(width, 0)));
	for (int i = 0; i < length; i++)//按Page读入
	{
		for (int j = 0; j < height; j++)
		{
			std::getline(readfile, tmp, '\n');
			std::stringstream ss(tmp);
			Info_append(tmp);
			int col = 0;
			while (ss >> number) numbers[i][j][col++] = number;
		}
	}
	CString str;
	str.Format(_T("%d"), default_page + 1);
	Page_Value.SetWindowTextW(str);
	readfile.close();
	return numbers;
}

void Operation_Page::CVMat_to_Pic(cv::Mat& img, UINT nID)
{
	cv::Mat imgTmp;
	CRect rect;
	GetDlgItem(nID)->GetClientRect(&rect);  // 获取控件大小
	cv::resize(img, imgTmp, cv::Size(rect.Width(), rect.Height()));// 缩放Mat并备份
	// 转一下格式 ,这段可以放外面,
	switch (imgTmp.channels())
	{
	case 1:
		cv::cvtColor(imgTmp, imgTmp, CV_GRAY2BGRA); // GRAY单通道
		break;
	case 3:
		cv::cvtColor(imgTmp, imgTmp, CV_BGR2BGRA);  // BGR三通道
		break;
	default:
		break;
	}
	int pixelBytes = imgTmp.channels() * (imgTmp.depth() + 1); // 计算一个像素多少个字节
	// 制作bitmapinfo(数据头)
	BITMAPINFO bitInfo;
	bitInfo.bmiHeader.biBitCount = 8 * pixelBytes;
	bitInfo.bmiHeader.biWidth = imgTmp.cols;
	bitInfo.bmiHeader.biHeight = -imgTmp.rows;
	bitInfo.bmiHeader.biPlanes = 1;
	bitInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitInfo.bmiHeader.biCompression = BI_RGB;
	bitInfo.bmiHeader.biClrImportant = 0;
	bitInfo.bmiHeader.biClrUsed = 0;
	bitInfo.bmiHeader.biSizeImage = 0;
	bitInfo.bmiHeader.biXPelsPerMeter = 0;
	bitInfo.bmiHeader.biYPelsPerMeter = 0;
	// Mat.data + bitmap数据头 -> MFC
	CDC* pDC = GetDlgItem(nID)->GetDC();
	::StretchDIBits(
		pDC->GetSafeHdc(),
		0, 0, rect.Width(), rect.Height(),
		0, 0, rect.Width(), rect.Height(),
		imgTmp.data,
		&bitInfo,
		DIB_RGB_COLORS,
		SRCCOPY
	);
	ReleaseDC(pDC);
}

BEGIN_MESSAGE_MAP(Operation_Page, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &Operation_Page::OnBnClicked_Start_Calc)
	ON_BN_CLICKED(IDC_BUTTON2, &Operation_Page::OnBnClicked_Export_CSV)
	ON_BN_CLICKED(IDC_BUTTON5, &Operation_Page::OnBnClicked_Export_BMP)
	ON_BN_CLICKED(IDC_BUTTON7, &Operation_Page::OnBnClicked_Export_CSV_Button)
	ON_BN_CLICKED(IDC_BUTTON8, &Operation_Page::OnBnClicked_Export_BMP_Button)
	ON_BN_CLICKED(IDC_BUTTON9, &Operation_Page::OnBnClicked_ChooseFormerPage)
	ON_BN_CLICKED(IDC_BUTTON10, &Operation_Page::OnBnClicked_ChooseNextPage)
	ON_BN_CLICKED(IDC_BUTTON3, &Operation_Page::OnBnClicked_AddWall)
	ON_BN_CLICKED(IDC_BUTTON11, &Operation_Page::OnBnClicked_DisplayResult)
END_MESSAGE_MAP()


// Operation_Page 消息处理程序

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


struct Node {
	int x;
	int y;
	int z;
	int from;
	Node(int _x, int _y, int _z, int _f) {
		x = _x;
		y = _y;
		z = _z;
		from = _f;
	}
	bool friend operator<(Node a, Node b) {
		return Vmap[a.z][a.y][a.x] > Vmap[b.z][b.y][b.x];
	}
};

#define MAX_GSIZE 10001
#define INF 10001
//初始化邻接数组
//后续可以用邻接表/链式前向星优化

int min(int x, int y) {
	return x < y ? x : y;
}

void Operation_Page::draw_box_color(unsigned int x, unsigned int y, unsigned int z, cv::Scalar Color) {
	int Box_Width = (2 * G_BORDER + G_CONTENT);
	int Box_Height = (2 * G_BORDER + G_CONTENT);
	int dx1 = x * 60 + G_BORDER;
	int dy1 = y * 60 + G_BORDER;
	int dx2 = (x + 1) * 60 - G_BORDER;
	int dy2 = (y + 1) * 60 - G_BORDER;
	cv::rectangle(
		Gmap,
		cv::Point(dx1,dy1),
		cv::Point(dx2,dy2),
		Color,
		-1
	);
}
std::vector<std::vector<std::vector<unsigned int>>> Out_G, Out_F;
std::vector<std::vector<std::vector<bool>>> Out_V;
//传参给display用
void Operation_Page::OnBnClicked_Start_Calc()
{
	//第一层 层
	//第二层 行
	//第三层 列
	std::vector<std::vector<std::vector<unsigned int>>> Graph(length,std::vector<std::vector<unsigned int>>(height,std::vector<unsigned int>(width,INF)));
	std::vector<std::vector<std::vector<unsigned int>>> Face(length, std::vector<std::vector<unsigned int>>(height, std::vector<unsigned int>(width, INF)));
	std::vector<std::vector<std::vector<bool>>> visited(length, std::vector<std::vector<bool>>(height, std::vector<bool>(width, 0)));
	CString sx, sy, sz, ex, ey, ez;
	Start_X_Cedit.GetWindowTextW(sx);
	Start_Y_Cedit.GetWindowTextW(sy);
	Start_Z_Cedit.GetWindowTextW(sz);
	End_X_Cedit.GetWindowTextW(ex);
	End_Y_Cedit.GetWindowTextW(ey);
	End_Z_Cedit.GetWindowTextW(ez);
	if (sx == _T("") || sy == _T("") || sz == _T("") || ex == _T("") || ey == _T("") || ez == _T("")) {
		MessageBox(_T("请输入完整的参数！"));
		return;
	}
	//提取点位
	Start_x = str_to_int(sx);
	Start_y = str_to_int(sy);
	Start_z = str_to_int(sz);
	End_x = str_to_int(ex);
	End_y = str_to_int(ey);
	End_z = str_to_int(ez);
	int x_now, y_now, z_now;
	//思路：BFS扩散
	x_now = Start_x;
	y_now = Start_y;
	z_now = Start_z;
	//std::queue<Node> to;
	std::priority_queue<Node> to;
	//to.push(Node(x_now,y_now))
	to.push(Node(x_now, y_now, z_now, _SET));
	Graph[Start_z][Start_y][Start_x] = Vmap[Start_z][Start_y][Start_x];
	int dis;
	int tx, ty, tz;
	while (!to.empty()) {
		tx = to.top().x;
		ty = to.top().y;
		tz = to.top().z;
		draw_box_color(tx, ty, tz, WAY_COLOR);
		to.pop();
		if (visited[tz][ty][tx])continue;
		visited[tz][ty][tx] = true;
		if (Vmap[tz][ty][tx - 1] > 0 && !visited[tz][ty][tx - 1]) {//LEFT
			to.push(Node(tx - 1, ty, tz, _SET));
			draw_box_color(tx - 1, ty, tz, WAY_COLOR);
		}
		if (Vmap[tz][ty][tx + 1] > 0 && !visited[tz][ty][tx + 1]) {//RIGHT
			to.push(Node(tx + 1, ty, tz, _SET));
			draw_box_color(tx + 1, ty, tz, WAY_COLOR);
		}
		if (Vmap[tz][ty - 1][tx] > 0 && !visited[tz][ty - 1][tx - 1]) {//DOWN
			to.push(Node(tx, ty - 1, tz, _SET));
			draw_box_color(tx, ty - 1, tz, WAY_COLOR);
		}
		if (Vmap[tz][ty + 1][tx] > 0 && !visited[tz][ty + 1][tx + 1]) {//UP
			to.push(Node(tx, ty + 1, tz, _SET));
			draw_box_color(tx, ty + 1, tz, WAY_COLOR);
		}
		if (Vmap[tz - 1][ty][tx] > 0 && !visited[tz - 1][ty][tx]) {//FRONT
			to.push(Node(tx, ty, tz - 1, _SET));
			draw_box_color(tx, ty, tz - 1, WAY_COLOR);
		}
		if (Vmap[tz + 1][ty][tx] > 0 && !visited[tz + 1][ty][tx]) {//BACK
			to.push(Node(tx, ty, tz + 1, _SET));
			draw_box_color(tx, ty, tz + 1, WAY_COLOR);
		}

		//到当前点的距离是自己格上下左右四侧dis的最小值+自己格的vmap
		dis = min(Graph[tz][ty][tx + 1], Graph[tz][ty][tx - 1]);
		dis = min(dis, Graph[tz][ty - 1][tx]);
		dis = min(dis, Graph[tz][ty + 1][tx]);
		dis = min(dis, Graph[tz - 1][ty][tx]);
		dis = min(dis, Graph[tz + 1][ty][tx]);
		int face = 0;
		if (dis == Graph[tz][ty - 1][tx])face = _DOWN;
		if (dis == Graph[tz][ty + 1][tx])face = _UP;
		if (dis == Graph[tz][ty][tx - 1])face = _LEFT;
		if (dis == Graph[tz][ty][tx + 1])face = _RIGHT;
		if (dis == Graph[tz - 1][ty][tx])face = _BACK;
		if (dis == Graph[tz + 1][ty][tx])face = _FRONT;
		if (dis == INF)dis = 0;
		dis += Vmap[tz][ty][tx];
		Graph[tz][ty][tx] = dis;
		Face[tz][ty][tx] = face;
		Info_append("(" + std::to_string(tx) + "," + std::to_string(ty) + "," + std::to_string(tz) + "), " + std::to_string(Graph[tz][ty][tx]));

		if (tx == End_x && ty == End_y && tz == End_z)break;

	}
	std::queue<Node> pathlist;
	pathlist.push(Node(End_x, End_y, End_z, Face[End_z][End_y][End_x]));
	int fx, fy, fz;
	fx = End_x;
	fy = End_y;
	fz = End_z;
	Info_clear();
	std::string ret = std::to_string(Graph[End_z][End_y][End_x]);
	Info_append("从源点到终点的距离为：" + ret);
	Info_append("路径：");
	Info_append("(" + std::to_string(fx) + "," + std::to_string(fy) + "," + std::to_string(fz) + ") ");
	while (!pathlist.empty()) {
		int now = pathlist.front().from;
		draw_box_color(fx, fy, fz, KEYWAY_COLOR);
		switch (now)
		{
		case _DOWN:fy--; break;
		case _UP:fy++; break;
		case _LEFT:fx--; break;
		case _RIGHT:fx++; break;
		case _BACK:fz--; break;
		case _FRONT:fz++; break;
		case _SET:break;
		default:
			break;
		}
		pathlist.push(Node(fx, fy, fz, Face[fz][fy][fx]));
		Info_append("(" + std::to_string(fx) + "," + std::to_string(fy) + "," + std::to_string(fz) + ") ");
		pathlist.pop();
		if (fx == Start_x && fy == Start_y && fz == Start_z)break;
	}
	draw_box_color(Start_x, Start_y, Start_z, START_COLOR);
	draw_box_color(End_x, End_y, End_z, END_COLOR);
	CVMat_to_Pic(Gmap, IDC_STATIC_PIC);
	Out_G = Graph;
	Out_V = visited;
	Out_F = Face;
	GetDlgItem(IDC_BUTTON2)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON5)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON11)->EnableWindow(true);
	return;
}


CString savefolder_filePath;

void Operation_Page::OnBnClicked_Export_CSV()
{
	savefolder_filePath = _T("");
	HWND hwnd = GetSafeHwnd();   //得到窗口句柄
	CString filePath = TEXT(""); //得到文件路径
	LPMALLOC pMalloc;
	if (::SHGetMalloc(&pMalloc) == NOERROR) //取得IMalloc分配器接口
	{
		BROWSEINFO bi;
		TCHAR pszBuffer[MAX_PATH];
		LPITEMIDLIST pidl;
		bi.hwndOwner = hwnd;
		bi.pidlRoot = NULL;
		bi.pszDisplayName = pszBuffer;
		bi.lpszTitle = _T("选择文件夹"); //选择目录对话框的上部分的标题
		//添加新建文件夹按钮 BIF_NEWDIALOGSTYLE
		bi.ulFlags = BIF_NEWDIALOGSTYLE | BIF_RETURNONLYFSDIRS | BIF_RETURNFSANCESTORS;
		bi.lpfn = NULL;
		bi.lParam = 0;
		bi.iImage = 0;
		if ((pidl = ::SHBrowseForFolder(&bi)) != NULL)  //取得IMalloc分配器接口
		{
			if (::SHGetPathFromIDList(pidl, pszBuffer)) //获得一个文件系统路径
			{
				filePath = pszBuffer;
			}
			pMalloc->Free(pidl); //释放内存
			Export_CSV_Path.SetWindowTextW(filePath);
			savefolder_filePath = filePath;
		}
		pMalloc->Release(); //释放接口
	}
}


void Operation_Page::OnBnClicked_Export_BMP()
{
	savefolder_filePath = _T("");
	HWND hwnd = GetSafeHwnd();   //得到窗口句柄
	CString filePath = TEXT(""); //得到文件路径
	LPMALLOC pMalloc;
	if (::SHGetMalloc(&pMalloc) == NOERROR) //取得IMalloc分配器接口
	{
		BROWSEINFO bi;
		TCHAR pszBuffer[MAX_PATH];
		LPITEMIDLIST pidl;
		bi.hwndOwner = hwnd;
		bi.pidlRoot = NULL;
		bi.pszDisplayName = pszBuffer;
		bi.lpszTitle = _T("选择文件夹"); //选择目录对话框的上部分的标题
		//添加新建文件夹按钮 BIF_NEWDIALOGSTYLE
		bi.ulFlags = BIF_NEWDIALOGSTYLE | BIF_RETURNONLYFSDIRS | BIF_RETURNFSANCESTORS;
		bi.lpfn = NULL;
		bi.lParam = 0;
		bi.iImage = 0;
		if ((pidl = ::SHBrowseForFolder(&bi)) != NULL)  //取得IMalloc分配器接口
		{
			if (::SHGetPathFromIDList(pidl, pszBuffer)) //获得一个文件系统路径
			{
				filePath = pszBuffer;
			}
			pMalloc->Free(pidl); //释放内存
			Export_BMP_Path.SetWindowTextW(filePath);
			savefolder_filePath = filePath;
		}
		pMalloc->Release(); //释放接口
	}
}


void Operation_Page::OnBnClicked_Export_CSV_Button()
{
	if (savefolder_filePath == "") {
		MessageBox(_T("请选择文件夹路径！"));
	}
	else {
		cv::String strfile = CT2A(savefolder_filePath.GetBuffer());
		std::ofstream file(strfile + "/export_result.csv");
		file << width << " " << height << " " << length;
		file << std::endl;
		for (int k = 0; k < length; k++)
		{
			for (int i = 0; i < Vmap.size(); i++)
			{
				for (int j = 0; j < Vmap[1].size(); j++) {
					file << Vmap[k][i][j] << " ";
				}
				file << std::endl;
			}
		}

		file.close();
	}
}


void Operation_Page::OnBnClicked_Export_BMP_Button()
{
	if (savefolder_filePath == "") {
		MessageBox(_T("请选择文件夹路径！"));
	}
	else {
		cv::String strfile = CT2A(savefolder_filePath.GetBuffer());
		cv::imwrite(strfile+"/export_result.bmp", Gmap);
	}
}


void Operation_Page::OnBnClicked_ChooseFormerPage()
{
	int Box_Width = (2 * G_BORDER + G_CONTENT);
	int Box_Height = (2 * G_BORDER + G_CONTENT);
	if (default_page != 0)default_page--;
	Page_draw_pic(default_page, Box_Width, Box_Height);
	CString str;
	str.Format(_T("%d"), default_page + 1);
	Page_Value.SetWindowTextW(str);
}


void Operation_Page::OnBnClicked_ChooseNextPage()
{
	int Box_Width = (2 * G_BORDER + G_CONTENT);
	int Box_Height = (2 * G_BORDER + G_CONTENT);
	if (default_page != length-1)default_page++;
	Page_draw_pic(default_page, Box_Width, Box_Height);
	CString str;
	str.Format(_T("%d"), default_page + 1);
	Page_Value.SetWindowTextW(str);
}


void Operation_Page::OnBnClicked_AddWall()
{
	CString addx, addy, addz;
	int ax, ay, az;
	Add_Wall_X.GetWindowTextW(addx);
	Add_Wall_Y.GetWindowTextW(addy);
	Add_Wall_Z.GetWindowTextW(addz);
	ax = str_to_int(addx);
	ay = str_to_int(addy);
	az = str_to_int(addz);
	if (addx == _T("") || addy == _T("") || addz == _T("")) {
		MessageBox(_T("请输入完整的参数！"));
		return;
	}
	Vmap[az][ay][ax] = 0;
	int Box_Width = (2 * G_BORDER + G_CONTENT);
	int Box_Height = (2 * G_BORDER + G_CONTENT);
	draw_box_color(ax, ay, az, NEWWALL_COLOR);
	CVMat_to_Pic(Gmap, IDC_STATIC_PIC);
	CString str;
	str.Format(_T("%d"), default_page + 1);
	Page_Value.SetWindowTextW(str);
	Info_append("Added new wall Successfully!");
}


void Operation_Page::OnBnClicked_DisplayResult()
{
	std::queue<Node> pathlist;
	pathlist.push(Node(End_x, End_y, End_z, Out_F[End_z][End_y][End_x]));
	int fx, fy, fz;
	fx = End_x;
	fy = End_y;
	fz = End_z;
	Info_clear();
	std::string ret = std::to_string(Out_G[End_z][End_y][End_x]);
	Info_append("从源点到终点的距离为：" + ret);
	Info_append("路径：");
	Info_append("(" + std::to_string(fx) + "," + std::to_string(fy) + "," + std::to_string(fz) + ") ");
	while (!pathlist.empty()) {
		int now = pathlist.front().from;
		draw_box_color(fx, fy, fz, KEYWAY_COLOR);
		switch (now)
		{
		case _DOWN:fy--; break;
		case _UP:fy++; break;
		case _LEFT:fx--; break;
		case _RIGHT:fx++; break;
		case _BACK:fz--; break;
		case _FRONT:fz++; break;
		case _SET:break;
		default:
			break;
		}
		pathlist.push(Node(fx, fy, fz, Out_F[fz][fy][fx]));
		Info_append("(" + std::to_string(fx) + "," + std::to_string(fy) + "," + std::to_string(fz) + ") ");
		pathlist.pop();
		if (fx == Start_x && fy == Start_y && fz == Start_z)break;
	}
	draw_box_color(Start_x, Start_y, Start_z, START_COLOR);
	draw_box_color(End_x, End_y, End_z, END_COLOR);
	CVMat_to_Pic(Gmap, IDC_STATIC_PIC);
	return;
}
