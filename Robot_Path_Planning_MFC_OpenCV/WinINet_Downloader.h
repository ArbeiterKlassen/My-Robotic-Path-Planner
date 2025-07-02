#pragma once
#include <windows.h>
#include <wininet.h>
#include <iostream>
#include <fstream>
#include <string>
#include "Operation_Page.h"
#pragma comment(lib, "wininet.lib")  // ���� WinINet ��

// �����ļ��ĺ���
bool Operation_Page::DownloadFile(const std::string& url, const std::string& filename) {
    //URL: ��Ҫ���ص��ļ�URL
    //filename�����ر������ʱ�ļ�·��

    HINTERNET hInternet, hConnect;
    DWORD bytesRead;
    char buffer[1024];  // ������

    // ��ʼ�� WinINet �Ự
    hInternet = InternetOpen(L"FileDownloader", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (hInternet == NULL) {
        MessageBox(_T("Failed to initialize WinINet."));
        return false;
    }

    // ��ָ���� URL
    hConnect = InternetOpenUrlA(hInternet, url.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (hConnect == NULL) {
        MessageBox(_T("Failed to open URL."));
        InternetCloseHandle(hInternet);
        return false;
    }

    // �򿪱����ļ���д����������
    std::ofstream outFile(filename, std::ios::binary);
    if (!outFile) {
        MessageBox(_T("Failed to open local file."));
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return false;
    }

    // ��ȡԶ���ļ���д�뱾���ļ�
    while (true) {
        if (InternetReadFile(hConnect, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0) {
            outFile.write(buffer, bytesRead);  // ������д�뱾���ļ�
        }
        else {
            break;  // �������
        }
    }

    // �ر��ļ�������
    outFile.close();
    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);

    std::cout << "File downloaded successfully." << std::endl;
    return true;
}