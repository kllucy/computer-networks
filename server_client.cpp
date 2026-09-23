// client.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#pragma comment (lib,"ws2_32.lib")
#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
#define PORT 8080
int main()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cout << "Winsock Failed: " << result << std::endl;
        return 1;
    }

    SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 
    if (listenSocket == INVALID_SOCKET) {
        std::cout << "create socket failed: " << WSAGetLastError() << std::endl;
        return 1;
    };

    sockaddr_in clientinfo;
    clientinfo.sin_family = AF_INET;
    clientinfo.sin_port = htons(PORT);
    inet_pton(AF_INET, "192.168.52.159", &clientinfo.sin_addr);

    result = connect(listenSocket, (sockaddr*)&clientinfo, sizeof(clientinfo));

    std::cout << "Подключено к серверу " << std::endl;
    char messageBuffer[512];
    int byteReceived;

    std::string message;
    while (true) {
        std::cout << "введите сообщение: ";
        std::getline(std::cin, message);

        int resultSend = send(listenSocket, message.c_str(), message.size(), 0);

        byteReceived = recv(listenSocket, messageBuffer, sizeof(messageBuffer), 0);
        if (byteReceived > 0) {
            messageBuffer[byteReceived] = "\n";
            std::cout << "Ответ от сервера: " << messageBuffer;

        }
        else {
            std::cout << "Соединение закрыто" << std::endl;
            break;
        }
        closesocket(listenSocket);
    }


}

//////
//server
#pragma comment (lib,"ws2_32.lib")
#include <iostream>
#include <WinSock2.h>
#include <string>
#define PORT 8080

int main()
{
    SetConsoleOutputCP(CP_UTF8);

    setlocale(LC_ALL, "RU");
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cout << "Winsock Failed: " << result << std::endl;
        return 1;

    }
    SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSocket == INVALID_SOCKET) {
        std::cout << "create socket failed: " << WSAGetLastError() << std::endl;
        return 1;
    }

    //структура адреса
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    result = bind(listenSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    if (result != 0) {
        std::cout << "bind failed: " << result << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }
    result = listen(listenSocket, SOMAXCONN);
    if (result == SOCKET_ERROR) {
        std::cout << "listen failed!: " << WSAGetLastError() << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }
    std::cout << "Сервер запущен: " << PORT << std::endl;

    SOCKET clientSocket = accept(listenSocket, NULL, NULL);
    if (clientSocket == INVALID_SOCKET) {
        std::cout << "accpet failed!: " << WSAGetLastError() << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Клиент подключился" << std::endl;

    closesocket(listenSocket);

    char messageBuffer[512];
    int bytesReceived;

    do {
        bytesReceived = recv(clientSocket, messageBuffer, sizeof(messageBuffer), 0);
        if (bytesReceived > 0) {
            messageBuffer[bytesReceived] = '\0';
            std::cout << "Получено от клиента: " << messageBuffer << std::endl;
            std::string response = "Сервер получит сообщение: ";
            response += messageBuffer;
            int sendResult= send(clientSocket, response.c_str(), response.size(), 0);
            if (sendResult != 0) {
                std::cout << "send failed: " << GetLastError() << std::endl;

            }
        }
        else if (bytesReceived == 0) {
            std::cout << "Соединение закрыто сервером" << std::endl;
        }
    } while (bytesReceived > 0);
    closesocket(clientSocket);
    WSACleanup();

}
