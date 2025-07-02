
// Robot_Path_Planning_MFC_OpenCVDlg.h: 头文件
//

#pragma once
#include"Robot_Path_Planning_MFC_OpenCV.h"
#include"Robot_Path_Planning_MFC_OpenCVDlg.h"
#include<opencv2/opencv.hpp>
#include"Resource.h"
// CRobotPathPlanningMFCOpenCVDlg 对话框
class CRobotPathPlanningMFCOpenCVDlg : public CDialogEx
{
// 构造
public:
	CRobotPathPlanningMFCOpenCVDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ROBOT_PATH_PLANNING_MFC_OPENCV_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CEdit Random_Width;
	CEdit Random_Height;
	CEdit Random_Length;
	CEdit Import_Local_File;
	CEdit Import_URL_File_URL;
	CEdit Import_URL_File_Port;
	afx_msg void OnBnClicked_RandomSummon();
	afx_msg void OnBnClicked_Start_Import_Local();
	afx_msg void OnBnClicked_Start_Import_URL();
	afx_msg void OnBnClicked_Confirm_Start();
	afx_msg void OnBnClicked_Rechoose_File();
};
