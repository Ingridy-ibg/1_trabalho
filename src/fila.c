#include <stdlib.h>
#include <stdbool.h>
#include "fila.h"

/* ─────────────────────────────────────────────
   Capacidade máxima do array estático interno.
   A fila lógica pode ter capacidade ≤ FILA_CAPACIDADE_MAX,
   definida em criaFila. Para os polígonos do trabalho,
   200 é suficiente (MAX_PONTOS_POLIGONO).
   ───────────────────────────────────────────── */

#define FILA_CAPACIDADE_MAX 200

/* ─────────────────────────────────────────────
   Estrutura interna (proibido expor no .h)
   Implementação ESTÁTICA circular:
   o array `dados` tem tamanho fixo definido em
   tempo de compilação — sem alocação dinâmica
   para o conteúdo da fila.
   ───────────────────────────────────────────── */

typedef struct FilaStruct {
    Item dados[FILA_CAPACIDADE_MAX];  /* array estático de itens */
    int  capacidade;                   /* capacidade lógica (≤ FILA_CAPACIDADE_MAX) */
    int  inicio;                       /* índice do elemento mais antigo */
    int  tamanho;                      /* quantidade de elementos */
} FilaStruct;

/* ─────────────────────────────────────────────
   Implementação da API pública
   ───────────────────────────────────────────── */

Fila criaFila(int capacidade) {
    if (capacidade <= 0 || capacidade > FILA_CAPACIDADE_MAX) return NULL;

    FilaStruct *F = (FilaStruct *)malloc(sizeof(FilaStruct));
    if (F == NULL) return NULL;

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
    fs->inicio    = (fs->inicio + 1) % fs->capacidade;
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

    fs->inicio  = 0;
    fs->tamanho = 0;
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

    /* Não há `free(fs->dados)`: o array é parte da struct. */
    free(fs);
}