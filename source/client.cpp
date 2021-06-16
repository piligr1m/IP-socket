#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <string>
#include <iostream>

//свободный порт
#define PORT 10105
//код выхода
#define CLOSE_MSG "exit"

int main(int argc, char** argv)
{
    int client_socket;
    struct sockaddr_in server_sock_addr{};
    
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("socket");
        exit(1);
    }
    
    server_sock_addr.sin_family = AF_INET;
    //задаем порт
    server_sock_addr.sin_port = htons(PORT);
    //задем локальный сервер
    server_sock_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    if (connect(client_socket, (struct sockaddr*) &server_sock_addr, sizeof(server_sock_addr)) < 0) {
        perror("connect");
        exit(2);
    }
    std::cout << "Connected!" << std::endl;
    
    while (true) {
        std::string msg;
        std::getline(std::cin, msg);
        send(client_socket, msg.c_str(), msg.size(), 0);
        
        char buf[512];
        size_t bytes_read = recv(client_socket, buf, 512, 0);
        std::string answer(buf, bytes_read);
        std::cout << "Server answer: " << answer << std::endl;
        
        if (answer == CLOSE_MSG) {
            break;
        }
    }
    close(client_socket);
    
    return 0;
}

