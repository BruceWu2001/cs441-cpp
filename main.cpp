#include <iostream>
#include <thread>
#include <vector>
#include "headers/classes/node.h"

int main (){
    Node n2 = Node("N2");
    Node n3 = Node("N3");

    std::thread t1 = std::thread(&Node::listen_eth, &n2);
    std::thread t2 = std::thread(&Node::listen_eth, &n3);


    std::vector<std::thread> threads;
    threads.emplace_back(std::move(t1));
    threads.emplace_back(std::move(t2));

    for (auto& th : threads) 
        th.join();
    return 0;
}