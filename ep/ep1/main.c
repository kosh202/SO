#include <stdio.h>
#include <stdlib.h>

//define tamanho max pra um bloco estatico
#define TAMANHO_MAX 100000
static char bloco_de_memoria[TAMANHO_MAX];

typedef struct BlocoDeMemoria {
    size_t tamanho;
    struct BlocoDeMemoria *proximo;
} BlocoDeMemoria;

static BlocoDeMemoria *bloco_livre = NULL;

void *aloca(size_t size) {
    if (size <= 0 || size > TAMANHO_MAX) {
        return NULL; // Verificação de tamanho inválido
    }

  
    if (bloco_livre == NULL) {
        bloco_livre = (BlocoDeMemoria *)bloco_de_memoria;
        bloco_livre->tamanho = TAMANHO_MAX - sizeof(BlocoDeMemoria);
        bloco_livre->proximo = NULL;
    }

    BlocoDeMemoria *atual = bloco_livre;
    BlocoDeMemoria *anterior = NULL;

    while (atual) {
        if (atual->tamanho >= size) {
            void *ptr = (char *)atual + sizeof(BlocoDeMemoria);
            if (atual->tamanho > size + sizeof(BlocoDeMemoria)) {
                // Divide o bloco se houver espaço suficiente
                BlocoDeMemoria *novo_bloco = (BlocoDeMemoria *)((char *)atual + size + sizeof(BlocoDeMemoria));
                novo_bloco->tamanho = atual->tamanho - size - sizeof(BlocoDeMemoria);
                novo_bloco->proximo = atual->proximo;
                atual->tamanho = size;
                atual->proximo = novo_bloco;
            } else {
                // Remove o bloco livre da lista
                if (anterior == NULL) {
                    bloco_livre = atual->proximo;
                } else {
                    anterior->proximo = atual->proximo;
                }
            }
            return ptr;
        }
        anterior = atual;
        atual = atual->proximo;
    }

    return NULL; // Não há memória disponível
}

void libera(void *ptr) {
    if (ptr == NULL) {
        return; // Verificação de ponteiro nulo
    }

    // Encontre o bloco de memória correspondente
    BlocoDeMemoria *bloco = (BlocoDeMemoria *)((char *)ptr - sizeof(BlocoDeMemoria));
    bloco->proximo = bloco_livre;
    bloco_livre = bloco;
}

int main(void) {
    int *ptr1;
    int *ptr2;

    // Aloca memória para dois inteiros
    ptr1 = (int *)aloca(sizeof(int));
    ptr2 = (int *)aloca(sizeof(int));

    if (ptr1 == NULL || ptr2 == NULL) {
        printf("Erro na alocação de memória.\n");
        return 1;
    }

    *ptr1 = 42;
    *ptr2 = 100;

    printf("Valor 1: %d\n", *ptr1);
    printf("Valor 2: %d\n", *ptr2);

    // Libera a memória alocada
    libera(ptr1);
    libera(ptr2);

    // Após a liberação, a memória está disponível para reutilização
    int *ptr3 = (int *)aloca(sizeof(int));
    if (ptr3 == NULL) {
        printf("Erro na alocação de memória após a liberação.\n");
        return 1;
    }

    *ptr3 = 200;

    printf("Valor 3: %d\n", *ptr3);

    return 0;
}
