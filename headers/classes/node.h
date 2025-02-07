#include <winsock2.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")  // Link with Winsock library

#define BROADCAST_PORT 12345
#define BROADCAST_IP "255.255.255.255"


class Node{
private:
    std::string name;
public:
    Node(std::string name);
    bool broadcast_eth();
    bool listen_eth();
    void print(const std::string& message);
    bool windowSetupSuccess();
    bool enableBroadcastSuccess(SOCKET sock);
    bool enableMultipleNodeBindToSamePortSuccess(SOCKET sock);
};