#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <functional>

//свободный порт
#define PORT 10105
//код выхода
#define CLOSE_MSG "exit"

void handler(int socket)
{
    std::cout << "User #" << socket << " connected!" << std::endl;
    while (true) {
        char buf[512];
        size_t bytes_read = recv(socket, buf, 512, 0);
        if (bytes_read <= 0) {
            break;
        }
        std::string msg(buf, bytes_read);
    
        std::cout << socket << ": " << msg << std::endl;
        send(socket, msg.c_str(), msg.size(), 0);
        if (msg == CLOSE_MSG) {
            break;
        }
    }
    std::cout << "User #" << socket << " disconnected!" << std::endl;
    close(socket);
}

int main(int argc, char** argv)
{
    int listener;
    struct sockaddr_in server_sock_addr{};
    
    listener = socket(AF_INET, SOCK_STREAM, 0);
    if (listener < 0) {
        perror("socket");
        exit(1);
    }
    
    server_sock_addr.sin_family = AF_INET;
    //задаем порт
    server_sock_addr.sin_port = htons(PORT);
    //задем локальный сервер
    server_sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(listener, (struct sockaddr*) &server_sock_addr, sizeof(server_sock_addr)) < 0) {
        perror("bind");
        exit(2);
    }
    
    listen(listener, 5);
    
    for (size_t i = 0; i < 10; ++i) {
        int socket = accept(listener, nullptr, nullptr);
        if (socket < 0) {
            perror("accept");
            exit(3);
        }
        std::thread(handler, socket).detach();
    }
    return 0;
}

