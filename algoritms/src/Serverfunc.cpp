#pragma once

#include "Server.h"
#include "DataBase.h"

#pragma comment(lib, "WS2_32.lib")


int main() {
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cerr << "WSAStartup failed: " << iResult << std::endl;
        return 1;
    }

    
    SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSocket == INVALID_SOCKET) {
        std::cerr << "socket failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.S_un.S_addr = INADDR_ANY;
    serverAddress.sin_port = htons(atoi(DEFAULT_PORT));

    
    iResult = bind(listenSocket, (sockaddr*)&serverAddress, sizeof(serverAddress));
    if (iResult == SOCKET_ERROR) {
        std::cerr << "bind failed: " << WSAGetLastError() << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    
    iResult = listen(listenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        std::cerr << "listen failed: " << WSAGetLastError() << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    while (true) {
        std::vector<Agent> all_agents;

        size_t counter = 0;

        while (counter != 20) {
            SOCKET clientSocket = accept(listenSocket, NULL, NULL);
            if (clientSocket == INVALID_SOCKET) {
                std::cerr << "accept failed: " << WSAGetLastError() << std::endl;
                closesocket(listenSocket);
                WSACleanup();
                return 1;
            }

            // Прием данных от клиента
            char recvbuf[DEFAULT_BUFLEN];
            int recvbuflen = DEFAULT_BUFLEN;
            int iResult = recv(clientSocket, recvbuf, recvbuflen, 0);
            if (iResult > 0) {
                Agent agent;

                // Извлечение значений A и B 
                std::istringstream iss(recvbuf);
                iss >> agent.id >> agent.A >> agent.B;

                agent.id = clientSocket;

                // Добавление агента в вектор
                all_agents.push_back(agent);

                std::cout << "Agent " << agent.id << ": Received values: " << agent.A << ", " << agent.B << std::endl;

            }
            else if (iResult == 0) {
                std::cout << "Connection closed gracefully." << std::endl;
            }
            else {
                std::cerr << "recv failed: " << WSAGetLastError() << std::endl;
            }
            ++counter;
        }

        counter = 0;

        server_log("../data/log.csv", all_agents);

        DataBase db(all_agents);
        std::unordered_map<int, bool> results = db.verdict();

        for (auto const& iter : results) {
            std::string sendBuf = iter.second ? "true" : "false";
            send(iter.first, sendBuf.c_str(), sizeof(bool), 0);
            
            std::cout << iter.first << "  -  " << sendBuf << std::endl;
        }
    }
    closesocket(listenSocket);
    WSACleanup();

    return 0;
}