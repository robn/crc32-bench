#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

uint32_t crc32_lookup[16][256];

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("usage: mkslicetables <polynomial>\n");
        return 1;
    }

    uint32_t polynomial = strtol(argv[1], NULL, 0);

    for (unsigned int i = 0; i <= 0xff; i++) {
        uint32_t crc = i;
        for (unsigned int j = 0; j < 8; j++)
            crc = (crc >> 1) ^ ((crc & 1) * polynomial);
        crc32_lookup[0][i] = crc;
    }

    for (unsigned int i = 0; i <= 0xff; i++) {
        crc32_lookup[1][i]  = (crc32_lookup[0][i]  >> 8) ^ crc32_lookup[0][crc32_lookup[0][i]  & 0xff];
        crc32_lookup[2][i]  = (crc32_lookup[1][i]  >> 8) ^ crc32_lookup[0][crc32_lookup[1][i]  & 0xff];
        crc32_lookup[3][i]  = (crc32_lookup[2][i]  >> 8) ^ crc32_lookup[0][crc32_lookup[2][i]  & 0xff];
        crc32_lookup[4][i]  = (crc32_lookup[3][i]  >> 8) ^ crc32_lookup[0][crc32_lookup[3][i]  & 0xff];
        crc32_lookup[5][i]  = (crc32_lookup[4][i]  >> 8) ^ crc32_lookup[0][crc32_lookup[4][i]  & 0xff];
        crc32_lookup[6][i]  = (crc32_lookup[5][i]  >> 8) ^ crc32_lookup[0][crc32_lookup[5][i]  & 0xff];
        crc32_lookup[7][i]  = (crc32_lookup[6][i]  >> 8) ^ crc32_lookup[0][crc32_lookup[6][i]  & 0xff];
        crc32_lookup[8][i]  = (crc32_lookup[7][i]  >> 8) ^ crc32_lookup[0][crc32_lookup[7][i]  & 0xff];
        crc32_lookup[9][i]  = (crc32_lookup[8][i]  >> 8) ^ crc32_lookup[0][crc32_lookup[8][i]  & 0xff];
        crc32_lookup[10][i] = (crc32_lookup[9][i]  >> 8) ^ crc32_lookup[0][crc32_lookup[9][i]  & 0xff];
        crc32_lookup[11][i] = (crc32_lookup[10][i] >> 8) ^ crc32_lookup[0][crc32_lookup[10][i] & 0xff];
        crc32_lookup[12][i] = (crc32_lookup[11][i] >> 8) ^ crc32_lookup[0][crc32_lookup[11][i] & 0xff];
        crc32_lookup[13][i] = (crc32_lookup[12][i] >> 8) ^ crc32_lookup[0][crc32_lookup[12][i] & 0xff];
        crc32_lookup[14][i] = (crc32_lookup[13][i] >> 8) ^ crc32_lookup[0][crc32_lookup[13][i] & 0xff];
        crc32_lookup[15][i] = (crc32_lookup[14][i] >> 8) ^ crc32_lookup[0][crc32_lookup[14][i] & 0xff];
    }

    printf("/* polynomial: 0x%08x */\n", polynomial);

    printf("static const uint32_t crc32_lookup[16][256] = {\n");

    for (int t = 0; t < 16; t++) {
        printf("    {\n");
        for (int i = 0; i < 0x100; i += 8) {
            printf("        0x%08x, 0x%08x, 0x%08x, 0x%08x, 0x%08x, 0x%08x, 0x%08x, 0x%08x%s\n",
                crc32_lookup[t][i],   crc32_lookup[t][i+1], crc32_lookup[t][i+2], crc32_lookup[t][i+3],
                crc32_lookup[t][i+4], crc32_lookup[t][i+5], crc32_lookup[t][i+6], crc32_lookup[t][i+7],
                (i == 0xf8 ? "" : ","));
        }
        printf("    }%s\n", (t == 15 ? "" : ","));
    }

    printf("};\n");

    return 0;
}
