#include <winsock2.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")  // Link with Winsock library

#define BROADCAST_PORT 12345
#define BROADCAST_IP "255.255.255.255"

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

    // Enable broadcast
    BOOL broadcast = TRUE;
    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char*)&broadcast, sizeof(broadcast)) < 0) {
        std::cerr << "Failed to set broadcast option.\n";
        return 1;
    }

    sockaddr_in broadcastAddr{};
    broadcastAddr.sin_family = AF_INET;
    broadcastAddr.sin_port = htons(BROADCAST_PORT);
    broadcastAddr.sin_addr.s_addr = inet_addr(BROADCAST_IP);

    const char* message = "Hello, Node 2!";
    if (sendto(sock, message, strlen(message), 0, (sockaddr*)&broadcastAddr, sizeof(broadcastAddr)) < 0) {
        std::cerr << "Broadcast send failed.\n";
        return 1;
    }

    std::cout << "Broadcast message sent!\n";
    closesocket(sock);
    WSACleanup();
    return 0;
}
