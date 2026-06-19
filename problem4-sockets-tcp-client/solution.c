#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>


int main(int argc, char *argv[]) {
    char *ip = argv[1];
    int port = atoi(argv[2]);

    int sock = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &addr.sin_addr);
    connect(sock, (struct sockaddr*)&addr, sizeof(addr));

    int x;

    while (scanf("%d", &x) == 1) {
        unsigned char buf[4];
        buf[0] = x & 0xFF;
        buf[1] = (x >> 8) & 0xFF;
        buf[2] = (x >> 16) & 0xFF;
        buf[3] = (x >> 24) & 0xFF;

        send(sock, buf, 4, 0);

        unsigned char ans[4];
        int r = recv(sock, ans, 4, 0);

        if (r <= 0) {
            break;
        }

        int y =
            ans[0] |
            (ans[1] << 8) |
            (ans[2] << 16) |
            (ans[3] << 24);

        printf("%d\n", y);
    }

    close(sock);
    return 0;
}