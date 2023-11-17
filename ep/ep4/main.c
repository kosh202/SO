/*
gcc hello.c -o hello

./hello
*/
#define FUSE_USE_VERSION 30
#include <fuse.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "fuse_common.h"
#include <fcntl.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <sys/uio.h>

// Adicione aqui as inclusões necessárias para manipulação de imagens BMP

static const char *bmp_path = "ep\ep4\kirbo.bmp";

// Adicione estruturas ou variáveis globais necessárias para armazenar pastas e arquivos

static void read_bmp(const char *path) {
    // Implemente a leitura da imagem BMP aqui
}

static int bmp_getattr(const char *path, struct stat *stbuf) {
    // Implemente a obtenção de atributos para o BMP e os elementos armazenados nele
    // Utilize stbuf para preencher informações sobre o arquivo ou diretório
}

static int bmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                        off_t offset, struct fuse_file_info *fi) {
    // Implemente a leitura de diretórios
    // Utilize filler para adicionar entradas de diretório ao buffer
}

static int bmp_open(const char *path, struct fuse_file_info *fi) {
    // Implemente a abertura de arquivos
    // Verifique se o arquivo existe e está acessível
}

static int bmp_read(const char *path, char *buf, size_t size, off_t offset,
                    struct fuse_file_info *fi) {
    // Implemente a leitura de dados de arquivos
    // Leitura efetiva dos dados do arquivo
}

static struct fuse_operations bmp_oper = {
    .getattr = bmp_getattr,
    .readdir = bmp_readdir,
    .open = bmp_open,
    .read = bmp_read,
    // Adicione mais operações conforme necessário
};

int main(int argc, char *argv[]) {
    // Leitura da imagem BMP
    read_bmp(bmp_path);

    // Inicialização do FUSE
    return fuse_main(argc, argv, &bmp_oper, NULL);
}
