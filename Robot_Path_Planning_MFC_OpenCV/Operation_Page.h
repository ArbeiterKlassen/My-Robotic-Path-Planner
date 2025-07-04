#pragma once
#include "afxdialogex.h"
#include "Robot_Path_Planning_MFC_OpenCV.h"
#include<opencv2/opencv.hpp>
#include "CMyButton.h"
// Operation_Page 对话框

class Operation_Page : public CDialogEx
{
	DECLARE_DYNAMIC(Operation_Page)

public:
	Operation_Page(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~Operation_Page();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OPRE_DIALOG };
#endif

protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
public:
	CBrush m_brush;
	CRect m_rtBtnfile;
	CRect m_rtBtnSelect;
	CRect m_rtBtnHelp;
	CMyButton m_btnExit;
	CMyButton m_btnMax;
	CMyButton m_btnMin;

	int mode = 0;
	CString url = _T(""), port = _T(""), ip = _T(""), path = _T("");
	int width = 0, length = 0, height = 0;

	afx_msg void init(int _m, CString _path, CString _ip, CString _port, int _width, int _height, int _length);
	afx_msg void Loadmode_1(CString _path);
	afx_msg void Loadmode_2(CString _url, CString _port);
	afx_msg void Loadmode_3(int _w,int _h, int _l);
	afx_msg std::vector<std::vector<std::vector<double>>> read_csv_to_Mat(CString _path);
	afx_msg void CVMat_to_Pic(cv::Mat& img, UINT nID);
	DECLARE_MESSAGE_MAP()
	CEdit Info_Bar_Cedit;
	void Info_append(std::string cstr);
	afx_msg void OnBnClicked_Start_Calc();
	afx_msg void OnBnClicked_Export_CSV();
	afx_msg void OnBnClicked_Export_BMP();
	CEdit Export_CSV_Path;
	CEdit Export_BMP_Path;
	afx_msg void OnBnClicked_Export_CSV_Button();
	afx_msg void OnBnClicked_Export_BMP_Button();
	afx_msg void Page_draw_pic(unsigned int page, unsigned int Box_Width, unsigned int Box_Height);
	CEdit Start_X_Cedit;
	CEdit Start_Y_Cedit;
	CEdit End_X_Cedit;
	CEdit End_Y_Cedit;
	CEdit Start_Z_Cedit;
	CEdit End_Z_Cedit;
	CEdit Page_Value;
	afx_msg void OnBnClicked_ChooseFormerPage();
	afx_msg void OnBnClicked_ChooseNextPage();
	CEdit Add_Wall_X;
	CEdit Add_Wall_Y;
	CEdit Add_Wall_Z;
	afx_msg void OnBnClicked_AddWall();
	void draw_box_color(unsigned int x, unsigned int y, unsigned int z, cv::Scalar Color);
	void Info_clear();
	//网络下载服务
	bool DownloadFile(const std::string& url, const std::string& filename);
	afx_msg void OnBnClicked_DisplayResult();
	CEdit POINTLIST_NUM; //编号
	CEdit POINT_END; //实际是终点，显示为起点
	CEdit POINT_START;//实际是起点
	CEdit POINT_DIS;//距离
	afx_msg void OnBnClicked_AddPoint();
	CMyButton m_button_exit;
	afx_msg void OnBnClickedButton13();
};
