#pragma once
#include <windows.h>
#include <wininet.h>
#include <iostream>
#include <fstream>
#include <string>
#include "Operation_Page.h"
#pragma comment(lib, "wininet.lib")  // 链接 WinINet 库

// 下载文件的函数
bool Operation_Page::DownloadFile(const std::string& url, const std::string& filename) {
    //URL: 需要下载的文件URL
    //filename：本地保存的临时文件路径

    HINTERNET hInternet, hConnect;
    DWORD bytesRead;
    char buffer[1024];  // 缓冲区

    // 初始化 WinINet 会话
    hInternet = InternetOpen(L"FileDownloader", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (hInternet == NULL) {
        MessageBox(_T("Failed to initialize WinINet."));
        return false;
    }

    // 打开指定的 URL
    hConnect = InternetOpenUrlA(hInternet, url.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (hConnect == NULL) {
        MessageBox(_T("Failed to open URL."));
        InternetCloseHandle(hInternet);
        return false;
    }

    // 打开本地文件以写入下载内容
    std::ofstream outFile(filename, std::ios::binary);
    if (!outFile) {
        MessageBox(_T("Failed to open local file."));
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return false;
    }

    // 读取远程文件并写入本地文件
    while (true) {
        if (InternetReadFile(hConnect, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0) {
            outFile.write(buffer, bytesRead);  // 将数据写入本地文件
        }
        else {
            break;  // 下载完成
        }
    }

    // 关闭文件和连接
    outFile.close();
    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);

    std::cout << "File downloaded successfully." << std::endl;
    return true;
}