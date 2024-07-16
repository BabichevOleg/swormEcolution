#pragma once

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <process.h>
#include <Windows.h>
#include <exception>
#include <random>
#include <mutex>


#define DEFAULT_PORT "27015"
#define DEFAULT_BUFLEN 512

// ��������� ��� �������� ������ ��� ��������
struct ProcessData {
    SOCKET sockfd;  // ����� ��������
    int dataIndex; // ������ ������ � �������
};


std::pair<double, double> getting_value();