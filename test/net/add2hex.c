#include "csapp.h"

int main(int argc, char **argv) {
    struct in_addr ip_address;
    int rc;
    char buf[MAXBUF];

    if (argc != 2) {
        fprintf(stderr, "Usage:\n./add2hex <dotted-decimal>\n");
    }

    rc = inet_pton(AF_INET, argv[1], &ip_address);

    printf("0x%x\n", ntohl(ip_address.s_addr));
    return 0;
}

