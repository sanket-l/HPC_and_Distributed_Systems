#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

int main(int argc, char **argv) {
    if (argc != 3) return 1;
    const char *ip = argv[1];
    int port = atoi(argv[2]);

    int s0 = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &addr.sin_addr);

    int con_err = connect(s0, (struct sockaddr*)&addr, sizeof(addr));
    if(con_err){
        fprintf(stderr, "Error in connection!\n");
        exit(1);
    }

    const char *msg = "Hello Sockets";
    write(s0, msg, strlen(msg));

    close(s0);
    return 0;
}
