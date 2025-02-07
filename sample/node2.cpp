#include <winsock2.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

#define BROADCAST_PORT 12345

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed.\n";
        return 1;
    }

    SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Socket creation failed.\n";
        return 1;
    }

    sockaddr_in recvAddr{};
    recvAddr.sin_family = AF_INET;
    recvAddr.sin_port = htons(BROADCAST_PORT);
    recvAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sock, (sockaddr*)&recvAddr, sizeof(recvAddr)) < 0) {
        std::cerr << "Binding failed.\n";
        return 1;
    }

    char buffer[1024] = {0};
    sockaddr_in senderAddr;
    int senderAddrSize = sizeof(senderAddr);

    std::cout << "Waiting for broadcast messages...\n";
    int recvLen = recvfrom(sock, buffer, sizeof(buffer) - 1, 0, (sockaddr*)&senderAddr, &senderAddrSize);
    if (recvLen < 0) {
        std::cerr << "Receiving failed.\n";
        return 1;
    }

    buffer[recvLen] = '\0';
    std::cout << "Received message: " << buffer << "\n";

    closesocket(sock);
    WSACleanup();
    return 0;
}
