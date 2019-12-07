#include "csapp.h"

int main(int argc, char **argv) {
    struct in_addr ip_address;
    uint32_t addr;
    char buf[MAXBUF];

    if (argc != 2) {
        fprintf(stderr, "Usage:\n./hex2add [hex number]\n");
    }
    
    sscanf(argv[1], "%x", &addr);
    ip_address.s_addr = htonl(addr);
    
    inet_ntop(AF_INET, &ip_address, buf, MAXBUF);
    printf("%s\n", buf);
    return 0;
}

