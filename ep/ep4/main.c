#define FUSE_USE_VERSION 31
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>

// Substitua pelo caminho real do seu arquivo BMP
static const char *bmp_file_path = "/workspaces/SO/ep/ep4/kirbo.bmp";
static const char *root_dir = "/";

// Defina uma estrutura para o cabeçalho BMP
struct BmpHeader {
    // ... defina os campos do cabeçalho BMP ...
};

// Função para verificar se um caminho é um diretório
static int is_directory(const char *path)
{
    return path[strlen(path) - 1] == '/';
}

// Função para verificar se o nome do arquivo tem a extensão ".txt"
static int is_txt_file(const char *name)
{
    const char *ext = ".txt";
    size_t name_len = strlen(name);
    size_t ext_len = strlen(ext);

    return (name_len > ext_len) && (strcmp(name + name_len - ext_len, ext) == 0);
}

// Implementação da leitura de dados de um arquivo BMP
static int read_bmp_data(char *buf, size_t size, off_t offset)
{
    FILE *bmp_file = fopen(bmp_file_path, "rb");
    if (!bmp_file)
        return -errno;

    // Leia o cabeçalho BMP
    struct BmpHeader bmp_header;
    fread(&bmp_header, sizeof(struct BmpHeader), 1, bmp_file);

    // Desloque-se para a posição de leitura correta no arquivo BMP
    if (fseek(bmp_file, offset, SEEK_SET) != 0)
    {
        fclose(bmp_file);
        return -errno;
    }

    // Leia os dados BMP reais
    size_t read_bytes = fread(buf, 1, size, bmp_file);

    fclose(bmp_file);

    return read_bytes;
}

// Implementação da obtenção de atributos
static int bmpfs_getattr(const char *path, struct stat *stbuf)
{
    int res = 0;

    memset(stbuf, 0, sizeof(struct stat));

    if (strcmp(path, root_dir) == 0)
    {
        stbuf->st_mode = S_IFDIR | 0755;
        stbuf->st_nlink = 2;
    }
    else if (is_directory(path))
    {
        stbuf->st_mode = S_IFDIR | 0755;
        stbuf->st_nlink = 2;
    }
    else if (is_txt_file(path))
    {
        stbuf->st_mode = S_IFREG | 0644;
        stbuf->st_nlink = 1;
        stbuf->st_size = 1024; // Tamanho fictício do arquivo .txt
    }
    else
    {
        res = -ENOENT;
    }

    return res;
}

// Implementação da listagem de diretórios
static int bmpfs_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
    (void)offset;
    (void)fi;

    if (strcmp(path, root_dir) != 0)
        return -ENOENT;

    filler(buf, ".", NULL, 0);
    filler(buf, "..", NULL, 0);

    // Adicione seus arquivos/diretórios específicos aqui
    // Neste exemplo, adicionamos apenas arquivos .txt fictícios
    filler(buf, "file1.txt", NULL, 0);
    filler(buf, "file2.txt", NULL, 0);

    return 0;
}

// Implementação da abertura de arquivos
static int bmpfs_open(const char *path, struct fuse_file_info *fi)
{
    // Verifique se o arquivo pode ser aberto (por exemplo, permissões)
    if (!is_txt_file(path))
        return -ENOENT;

    return 0;
}

// Implementação da leitura de arquivos
static int bmpfs_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    // Implemente a lógica para ler dados de um arquivo no diretório virtual
    // Use a função read_bmp_data para obter os dados do arquivo BMP
    int read_bytes = read_bmp_data(buf, size, offset);
    if (read_bytes < 0)
        return read_bytes;

    return read_bytes;
}

// Função para criar um diretório no diretório virtual
static int bmpfs_mkdir(const char *path, mode_t mode)
{
    // Implemente a lógica para criar um diretório no diretório virtual
    // Isso pode envolver a criação de um diretório na imagem BMP, se aplicável.
    // Lembre-se de ajustar as permissões do diretório conforme especificado por 'mode'.

    // Crie o diretório no sistema de arquivos real (o segundo argumento é o valor das permissões)
    if (mkdir(path, 0777) == -1)
        return -errno;

    return 0; // Retorne 0 em caso de sucesso
}

// Estrutura de operações do FUSE
static struct fuse_operations bmpfs_oper = {
    .getattr = bmpfs_getattr,
    .readdir = bmpfs_readdir,
    .open = bmpfs_open,
    .read = bmpfs_read,
    .mkdir = bmpfs_mkdir,
};

int main(int argc, char *argv[])
{
    return fuse_main(argc, argv, &bmpfs_oper, NULL);
}
