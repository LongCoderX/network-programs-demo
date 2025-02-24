#include <cstdio>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <cstring>

const char *SERVER_PREFIX_STR = "Server receive message: "; 

void onProcessMessage(int client_fd) {
    printf("start process %d.\n", client_fd);

    while (true) {
        char recv_buf[1024] = { 0 };
        ssize_t read_context_size = read(client_fd, recv_buf, sizeof(recv_buf));
        if (read_context_size == 0) {
            perror("client disconnect.");
            break;
        }
        if (read_context_size < 0) {
            perror("read failed.");
            break;
        }

        if (strcmp(recv_buf, "quit\r\n") == 0) {
            break;
        }

        char write_buf[1024] = { 0 };
        std::strcat(write_buf, SERVER_PREFIX_STR);
        std::strcat(write_buf, recv_buf);
        write(client_fd, write_buf, strlen(write_buf));
        printf("%s", write_buf);
    }
    close(client_fd);
}

int main() {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        perror("socket");
        return 0;
    }

    struct sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8888);
    addr.sin_addr.s_addr = INADDR_ANY;
    
    int ret = bind(fd, (sockaddr *)&addr, sizeof(sockaddr_in));
    if (ret < 0) {
        perror("bind");
        return 0;
    }

    ret = listen(fd, 5);
    if (ret < 0) {
        perror("listen");
        return 0;
    }

    int count = 0;
    while (true) {
        int client_fd = accept(fd, nullptr, nullptr);
        onProcessMessage(client_fd);
    }

    return 0;
}
