/*
gcc hello.c -o hello

./hello
*/


#define FUSE_USE_VERSION 31
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>

static const char *bmp_file_path = "/path/to/your/image.bmp"; // Substitua pelo caminho real do seu arquivo BMP

static int bmpfs_getattr(const char *path, struct stat *stbuf)
{
    int res = 0;

    memset(stbuf, 0, sizeof(struct stat));

    if (strcmp(path, "/") == 0)
    {
        stbuf->st_mode = S_IFDIR | 0755;
        stbuf->st_nlink = 2;
    }
    else
    {
        // Implemente a lógica para obter atributos de arquivos aqui
        // Use as funções da LibFuse como fuse_get_context() para obter informações do usuário atual
        // e então preencha stbuf de acordo com o arquivo/diretório especificado
        res = -ENOENT;
    }

    return res;
}

static int bmpfs_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                         off_t offset, struct fuse_file_info *fi)
{
    (void)offset;
    (void)fi;

    if (strcmp(path, "/") != 0)
        return -ENOENT;

    filler(buf, ".", NULL, 0);
    filler(buf, "..", NULL, 0);

    // Implemente a lógica para preencher o diretório virtual com os arquivos presentes na imagem BMP
    // Utilize a função filler para adicionar cada arquivo/diretório ao diretório virtual

    return 0;
}

static int bmpfs_open(const char *path, struct fuse_file_info *fi)
{
    // Implemente a lógica para abrir um arquivo no diretório virtual
    // Pode envolver a verificação do caminho, permissões, etc.

    return 0;
}

static int bmpfs_read(const char *path, char *buf, size_t size, off_t offset,
                      struct fuse_file_info *fi)
{
    // Implemente a lógica para ler dados de um arquivo no diretório virtual
    // Use a função pread() para ler dados do arquivo BMP e preencher buf

    return 0; // Retorne o número de bytes lidos
}

static struct fuse_operations bmpfs_oper = {
    .getattr = bmpfs_getattr,
    .readdir = bmpfs_readdir,
    .open = bmpfs_open,
    .read = bmpfs_read,
};

int main(int argc, char *argv[])
{
    return fuse_main(argc, argv, &bmpfs_oper, NULL);
}
