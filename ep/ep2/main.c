#include <stdio.h>
#include <stdlib.h>

#define TAMANHO_MBR 512

struct entradaParticao {
    unsigned char stat;
    unsigned char chsStart[3];
    unsigned char type;
    unsigned char chsEnd[3];
    unsigned int lbaStart;
    unsigned int sectors;
};

int main() {
    FILE *arquivo;
    arquivo = fopen("mbr.bin", "rb");

    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }

    unsigned char buffer[TAMANHO_MBR];
    fread(buffer, 1, TAMANHO_MBR, arquivo);
    fclose(arquivo);

    printf("Device Boot Start End Sectors Size Id Type\n");

    struct entradaParticao *partitions = (struct entradaParticao *)(buffer + 446);

    for (int i = 0; i < 4; i++) {
        if (partitions[i].type == 0)
            continue;

        printf("/dev/sda%d ", i + 1);

        if (partitions[i].stat == 0x80)
            printf("* ");
        else
            printf("  ");

        printf("%u ", partitions[i].lbaStart);
        printf("%u ", partitions[i].lbaStart + partitions[i].sectors - 1);
        printf("%u ", partitions[i].sectors);
        printf("%u", partitions[i].sectors * 512 / (1024 * 1024)); // Size in MB

        printf(" %02X ", partitions[i].type);

        switch (partitions[i].type) {
            case 0x01:
                printf("FAT12\n");
                break;
            case 0x04:
            case 0x06:
            case 0x0E:
                printf("FAT16\n");
                break;
            case 0x05:
            case 0x07:
            case 0x0F:
                printf("NTFS\n");
                break;
            default:
                printf("Unknown\n");
        }
    }

    return 0;
}
