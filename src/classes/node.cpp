#include "../../headers/classes/node.h"
#include "../../headers/utils/printMessage.h"

Node::Node(
    std::string name
): name(name){

}

bool Node::broadcast_eth(){
    if (!Node::windowSetupSuccess()){return 1;}

    SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == INVALID_SOCKET) {
        Node::print("Socket creation failed.");
        return 1;
    }

    if (!Node::enableBroadcastSuccess(sock)){return 1;}

    sockaddr_in broadcastAddr{};
    broadcastAddr.sin_family = AF_INET;
    broadcastAddr.sin_port = htons(BROADCAST_PORT);
    broadcastAddr.sin_addr.s_addr = inet_addr(BROADCAST_IP);

    const char* message = "Hello from Node 1!";
    if (sendto(sock, message, strlen(message), 0, (sockaddr*)&broadcastAddr, sizeof(broadcastAddr)) < 0) {
        Node::print("Broadcast send failed.");
        return 1;
    }

    Node::print("Broadcast message sent!");
    closesocket(sock);
    WSACleanup();
    return 0;
}

bool Node::listen_eth(){
    if (!Node::windowSetupSuccess()){return 1;}

    // AF_INET means IPV4
    // SOCK_DGRAM: DATAGRAM BASED (UDP) alternative is SOCK_STREAM (TCP)
    // 0: protocol --> UDP
    SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);

    sockaddr_in recvAddr{};
    recvAddr.sin_family = AF_INET;
    recvAddr.sin_port = htons(BROADCAST_PORT);
    recvAddr.sin_addr.s_addr = INADDR_ANY;

    if(!Node::enableMultipleNodeBindToSamePortSuccess(sock)){return 1;}


    if (bind(sock, (sockaddr*)&recvAddr, sizeof(recvAddr)) < 0) {
        Node::print("Binding failed.");
        return 1;
    }

    char buffer[1024] = {0};
    sockaddr_in senderAddr;
    int senderAddrSize = sizeof(senderAddr);

    Node::print("Waiting for broadcast messages...");
    while(true){
        int recvLen = recvfrom(sock, buffer, sizeof(buffer) - 1, 0, (sockaddr*)&senderAddr, &senderAddrSize);
        if (recvLen < 0) {
            Node::print("Receiving failed.");
            return 1;
        }
    
        buffer[recvLen] = '\0';
        Node::print("Received message: "+ std::string(buffer));
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}

// UTILS
// UTILS
// UTILS

void Node::print(const std::string& message){
    printMessage(name + ": " + message);
}


// CONFIGURATIONS
// CONFIGURATIONS
// CONFIGURATIONS

bool Node::windowSetupSuccess(){
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        Node::print("WSAStartup failed.");
        return false;
    }
    return true;
}

bool Node::enableMultipleNodeBindToSamePortSuccess(SOCKET sock){
    BOOL optval = TRUE;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(optval)) < 0) {
        Node::print("Failed to set SO_REUSEADDR.");
        return false;
    }
    return true;
}

bool Node::enableBroadcastSuccess(SOCKET sock){
    BOOL broadcast = TRUE;
    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char*)&broadcast, sizeof(broadcast)) < 0) {
        Node::print("Failed to set broadcast option.");
        return false;
    }
    return true;
}