#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

int main(int argc, char **argv) {
    if (argc != 2) return 1;
    int port = atoi(argv[1]);

    int s = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    int bind_err = bind(s, (struct sockaddr*)&addr, sizeof(addr));
    if(bind_err) {
        fprintf(stderr, "Error binding!\n");
        exit(1);
    }
    listen(s, 1);

    int c = accept(s, NULL, NULL);

    char buf[64] = {0};
    read(c, buf, sizeof(buf)-1);
    puts(buf);

    close(c);
    close(s);
    return 0;
}
