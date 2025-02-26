#include <ostream>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <cstdio>
#include <string>
#include <iostream>

const std::string SERVER_PREFIX_STR = "Server receive message: ";

void onProcessMessage(int client_fd) {
    printf("start process %d.\n", client_fd);

    while (true) {
        std::string recv_buf(nullptr, 1024);
        ssize_t read_context_size = read(client_fd, (void *)recv_buf.c_str(), recv_buf.capacity());
        if (read_context_size == 0) {
            perror("client disconnect.");
            break;
        }
        if (read_context_size < 0) {
            perror("read failed.");
            break;
        }

        if (recv_buf == "quit\r\n") {
            break;
        }

        std::string write_buf = SERVER_PREFIX_STR + recv_buf;

        write(client_fd, write_buf.c_str(), write_buf.size());
        
        std::cout << write_buf;
    }
    close(client_fd);
}

auto main(int argc, char *argv[]) -> int {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        std::cerr << "socket\n";
        return 0;
    }

    struct sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8888);
    addr.sin_addr.s_addr = INADDR_ANY;
    
    int ret = bind(fd, (sockaddr *)&addr, sizeof(sockaddr_in));
    if (ret < 0) {
        std::cerr << "bind\n";
        return 0;
    }

    ret = listen(fd, 5);
    if (ret < 0) {
        std::cerr << "listen\n";
        return 0;
    }

    while (true) {
        int client_fd = accept(fd, nullptr, nullptr);
        onProcessMessage(client_fd);
    }

    return 0;
}
