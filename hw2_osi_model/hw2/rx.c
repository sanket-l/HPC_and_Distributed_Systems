#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

int main(int argc, char **argv) {
    if (argc != 2) return 1;
    int port = atoi(argv[1]);

    // Create TCP socket
    int s = socket(AF_INET, SOCK_STREAM, 0);
    if(s < 0) {
        fprintf(stderr, "Error creating socket!\n");
        exit(1);
    }

    // Configure server address
    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    // Bind socket to port
    int bind_err = bind(s, (struct sockaddr*)&addr, sizeof(addr));
    if(bind_err) {
        fprintf(stderr, "Error binding!\n");
        exit(1);
    }

    // Listen for incoming connections
    int listen_err = listen(s, 1);
    if(listen_err < 0) {
        fprintf(stderr, "Error listening!\n");
        close(s);
        exit(1);
    }

    // Accept client connection
    int c = accept(s, NULL, NULL);
    if(c < 0) {
        fprintf(stderr, "Error accepting connection!\n");
        close(s);
        exit(1);
    }

    // Receive and display message
    char buf[64] = {0};
    int read_err = read(c, buf, sizeof(buf)-1);
    if(read_err < 0) {
        fprintf(stderr, "Error reading message!\n");
        close(c);
        close(s);
        exit(1);
    }
    puts(buf);

    // Cleanup
    close(c);
    close(s);
    return 0;
}
