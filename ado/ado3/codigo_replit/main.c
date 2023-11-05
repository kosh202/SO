#include <stdio.h>
#include <sys/utsname.h>

int main() {
    struct utsname info;

    if (uname(&info) != 0) {
        perror("uname");
        return 1;
    }

    printf("Sistema: %s\n", info.sysname);
    printf("Nome do Host: %s\n", info.nodename);
    printf("Release do Kernel: %s\n", info.release);
    printf("Versão do Kernel: %s\n", info.version);
    printf("Arquitetura: %s\n", info.machine);

    return 0;
}
