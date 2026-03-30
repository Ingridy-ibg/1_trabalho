#include <stdlib.h>
#include <stdbool.h>
#include "fila.h"

/* ─────────────────────────────────────────────
   Estrutura interna (proibido expor no .h)
   ───────────────────────────────────────────── */

typedef struct FilaStruct {
    Item *dados;       /* array estático de itens         */
    int   capacidade;  /* tamanho máximo do array         */
    int   inicio;      /* índice do elemento mais antigo  */
    int   tamanho;     /* quantidade de elementos         */
} FilaStruct;

/* ─────────────────────────────────────────────
   Implementação da API pública
   ───────────────────────────────────────────── */

Fila criaFila(int capacidade) {
    if (capacidade <= 0) return NULL;

    FilaStruct *F = (FilaStruct *)malloc(sizeof(FilaStruct));
    if (F == NULL) return NULL;

    F->dados = (Item *)malloc(capacidade * sizeof(Item));
    if (F->dados == NULL) {
        free(F);
        return NULL;
    }

    F->capacidade = capacidade;
    F->inicio     = 0;
    F->tamanho    = 0;
    return (Fila)F;
}

/* ── enqueue ── */

bool enqueue(Fila F, Item info) {
    if (F == NULL) return false;
    FilaStruct *fs = (FilaStruct *)F;

    if (fs->tamanho == fs->capacidade) return false;  /* fila cheia */

    /* índice do próximo slot livre (comportamento circular) */
    int fim = (fs->inicio + fs->tamanho) % fs->capacidade;
    fs->dados[fim] = info;
    fs->tamanho++;
    return true;
}

/* ── dequeue ── */

Item dequeue(Fila F) {
    if (F == NULL) return NULL;
    FilaStruct *fs = (FilaStruct *)F;

    if (fs->tamanho == 0) return NULL;  /* fila vazia */

    Item removido = fs->dados[fs->inicio];
    fs->inicio    = (fs->inicio + 1) % fs->capacidade;  /* avança circularmente */
    fs->tamanho--;
    return removido;
}

/* ── peek ── */

Item peek(Fila F) {
    if (F == NULL) return NULL;
    FilaStruct *fs = (FilaStruct *)F;

    if (fs->tamanho == 0) return NULL;
    return fs->dados[fs->inicio];
}

/* ── consultas ── */

int lengthFila(Fila F) {
    if (F == NULL) return 0;
    return ((FilaStruct *)F)->tamanho;
}

int capacidadeFila(Fila F) {
    if (F == NULL) return 0;
    return ((FilaStruct *)F)->capacidade;
}

bool isEmptyFila(Fila F) {
    if (F == NULL) return true;
    return ((FilaStruct *)F)->tamanho == 0;
}

bool isFullFila(Fila F) {
    if (F == NULL) return false;
    FilaStruct *fs = (FilaStruct *)F;
    return fs->tamanho == fs->capacidade;
}

/* ── clearFila ── */

void clearFila(Fila F, void (*destroiItem)(Item)) {
    if (F == NULL) return;
    FilaStruct *fs = (FilaStruct *)F;

    if (destroiItem != NULL) {
        for (int i = 0; i < fs->tamanho; i++) {
            int idx = (fs->inicio + i) % fs->capacidade;
            destroiItem(fs->dados[idx]);
        }
    }

    fs->inicio   = 0;
    fs->tamanho  = 0;
}

/* ── destroiFila ── */

void destroiFila(Fila F, void (*destroiItem)(Item)) {
    if (F == NULL) return;
    FilaStruct *fs = (FilaStruct *)F;

    if (destroiItem != NULL) {
        for (int i = 0; i < fs->tamanho; i++) {
            int idx = (fs->inicio + i) % fs->capacidade;
            destroiItem(fs->dados[idx]);
        }
    }

    free(fs->dados);
    free(fs);
}