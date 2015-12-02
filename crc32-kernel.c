#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/socket.h>
#include <linux/if_alg.h>
#include <sys/param.h>
#include <string.h>
#include <strings.h>

#ifndef SOL_ALG
#define SOL_ALG 279
#endif

int alg_fd = -1;
int crc_fd = -1;

void crc32_init(void) {
    struct sockaddr_alg sa = {
        .salg_family = AF_ALG,
        .salg_type   = "hash",
        .salg_name   = "crc32"
    };

    if ((alg_fd = socket(AF_ALG, SOCK_SEQPACKET, 0)) == -1) {
        perror("socket");
        abort();
    }

    if (bind(alg_fd, (struct sockaddr *) &sa, sizeof(sa)) != 0) {
        perror("bind");
        abort();
    }

    if ((crc_fd = accept(alg_fd, NULL, 0)) == -1) {
        perror("accept");
        abort();
    }

    int crc = ~0;
    if (setsockopt(alg_fd, SOL_ALG, ALG_SET_KEY, &crc, 4) != 0) {
        perror("setsockopt");
        abort();
    }
}

void crc32_shutdown(void) {
    close(crc_fd);
    close(alg_fd);
}

unsigned long crc32(unsigned long crc, unsigned char *buf, unsigned long len) {
    if (send(crc_fd, buf, len, MSG_DONTWAIT) != len) {
        perror("send");
        abort();
    }

    int crcout = 0;
    if (read(crc_fd, &crcout, 4) != 4) {
        perror("read");
        abort();
    }

    return ~crcout & 0xffffffff;
}
