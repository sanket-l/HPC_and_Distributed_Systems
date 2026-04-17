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

    // Create TCP socket
    int s0 = socket(AF_INET, SOCK_STREAM, 0);
    if(s0 < 0) {
        fprintf(stderr, "Error creating socket!\n");
        exit(1);
    }

    // Configure server address
    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    
    // Parse IP address
    int pton_err = inet_pton(AF_INET, ip, &addr.sin_addr);
    if(pton_err <= 0) {
        fprintf(stderr, "Error parsing IP address!\n");
        close(s0);
        exit(1);
    }

    // Connect to server
    int con_err = connect(s0, (struct sockaddr*)&addr, sizeof(addr));
    if(con_err) {
        fprintf(stderr, "Error in connection!\n");
        close(s0);
        exit(1);
    }

    // Send message
    const char *msg = "Hello Sockets";
    int write_err = write(s0, msg, strlen(msg));
    if(write_err < 0) {
        fprintf(stderr, "Error sending message!\n");
        close(s0);
        exit(1);
    }

    // Cleanup
    close(s0);
    return 0;
}
