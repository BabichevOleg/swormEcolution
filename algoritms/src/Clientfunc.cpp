#pragma once

#include "Client.h"
#include "Evolution.h"
#include "Logger.h"

#pragma comment(lib, "WS2_32.lib")


std::pair<double, double> getting_value() {


    std::ifstream check_file("..\\data\\tmp_data.csv");
    if (!check_file.is_open()) throw std::logic_error("Open file error");

    std::string file_path;
    if (check_file.peek() == EOF) {
        file_path = "..\\data\\data.csv";
    }
    else {
        file_path = "..\\data\\tmp_data.csv";
    }

    check_file.close();
    std::ifstream file(file_path);
    if (!file.is_open()) throw std::logic_error("Open file error");

    std::vector<double> values1;
    std::vector<double> values2;
    std::string line;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string data;
        while (std::getline(ss, data, '|')) {
            values1.push_back(std::stod(data));
        }

        getline(file, line);
        ss.str(line);
        ss.clear();
        while (std::getline(ss, data, '|')) {
            values2.push_back(std::stod(data));
        }
    }

    file.close();

    int index1 = rand() % values1.size();
    int index2 = rand() % values2.size();

    std::pair<double, double> result = { values1[index1], values2[index2] };

    std::cout << getpid() << " - " << result.first << ", " << result.second << std::endl;

    values1.erase(values1.begin() + index1);
    values2.erase(values2.begin() + index2);


    if (values1.size() == 0 && values2.size() == 0) {
        std::ofstream file1("..\\data\\tmp_data.csv");
        if (!file1.is_open()) throw std::logic_error("Open file error");
        file1.close();

        return result;
    }

    std::ofstream file1("..\\data\\tmp_data.csv");
    if (!file1.is_open()) throw std::logic_error("Open file error");

    std::stringstream csv;
    for (const auto& iter : values1) {
        csv << iter << " | ";
    }
    if (!csv.str().empty()) {
        csv.seekp(-3, std::ios_base::end);
        csv << "  ";
    }
    csv << std::endl;

    for (const auto& iter : values2) {
        csv << iter << " | ";
    }
    if (!csv.str().empty()) {
        csv.seekp(-3, std::ios_base::end);
        csv << "  ";
    }
    csv << std::endl;

    file1 << csv.str();

    file1.close();

    return result;
}

int main() {

    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cerr << "WSAStartup failed: " << iResult << std::endl;
        return 1;
    }

    // Создание сокета
    SOCKET connectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (connectSocket == INVALID_SOCKET) {
        std::cerr << "socket failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return NULL;
    }

    // Настройка адреса сервера
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(atoi(DEFAULT_PORT));
    // Преобразование IP-адреса в сетевой байтовый порядок
    if (inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr) != 1) {
        std::cerr << "inet_pton failed: " << WSAGetLastError() << std::endl;
    }

    // Подключение к серверу
    iResult = connect(connectSocket, (sockaddr*)&serverAddress, sizeof(serverAddress));
    if (iResult == SOCKET_ERROR) {
        std::cerr << "connect failed: " << WSAGetLastError() << std::endl;
        closesocket(connectSocket);
        WSACleanup();
        return 1;
    }

    int sockid = socket(AF_INET, SOCK_STREAM, 0);

    HANDLE semaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, "SemaphoreToGetValue");
    WaitForSingleObject(semaphore, INFINITE);

    std::pair<double, double> values = getting_value();

    ReleaseSemaphore(semaphore, 1, NULL);
    CloseHandle(semaphore);

    // Отправка  значений на сервер
    std::string sendData = std::to_string(sockid) + " " + std::to_string(values.first) + " " + std::to_string(values.second);
    iResult = send(connectSocket, sendData.c_str(), (int)sendData.size(), 0);
    if (iResult == SOCKET_ERROR) {
        std::cerr << "send failed: " << WSAGetLastError() << std::endl;
        closesocket(connectSocket);
        WSACleanup();
        return 1;
    }

    std::ofstream file("../data/evolution_values.csv", std::ios::app);
    if (!file.is_open()) throw std::logic_error("Open file error");

    char buffer[256];
    int bytes_received = recv(connectSocket, buffer, sizeof(buffer), 0);
    if (bytes_received == SOCKET_ERROR) {
        std::cerr << "recv failed" << WSAGetLastError() << std::endl;
        closesocket(connectSocket);
        WSACleanup();
        return 1;
    }
    else if (bytes_received == 0) {
        std::cerr << "Сonnection closed gracefully" << std::endl;
    }
    else {
        std::string response(buffer, bytes_received);

        //std::cout << std::endl;
        //std::cout << response << std::endl;

        std::stringstream csv;
        if (response == "t") {
            csv << "0 | 0";
        }
        else {
            csv << values.first << " | " << values.second;
        }
        csv << std::endl;
        file << csv.str();
    }
    file.close();

    // Закрытие сокета
    closesocket(connectSocket);
    WSACleanup();

    return 0;
}