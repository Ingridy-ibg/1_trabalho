#include <stdlib.h>
#include <stdbool.h>
#include "lista.h"



typedef struct No {
    Item    info;
    struct No *ant;
    struct No *prox;
} No;

typedef struct ListaStruct {
    No  *cabeca;   
    No  *cauda;    
    int  tamanho;
} ListaStruct;



static No *criaNo(Item info) {
    No *n = (No *)malloc(sizeof(No));
    if (n == NULL) return NULL;
    n->info = info;
    n->ant  = NULL;
    n->prox = NULL;
    return n;
}


Lista criaLista() {
    ListaStruct *L = (ListaStruct *)malloc(sizeof(ListaStruct));
    if (L == NULL) return NULL;

    L->cabeca = criaNo(NULL);
    L->cauda  = criaNo(NULL);
    if (L->cabeca == NULL || L->cauda == NULL) {
        free(L->cabeca);
        free(L->cauda);
        free(L);
        return NULL;
    }

    L->cabeca->prox = L->cauda;
    L->cauda->ant   = L->cabeca;
    L->tamanho      = 0;
    return (Lista)L;
}



Posic insereFinalLista(Lista L, Item info) {
    if (L == NULL) return NULL; 
    return insertBefore(L, ((ListaStruct *)L)->cauda, info);
}

Posic insereInicioLista(Lista L, Item info) {
    if (L == NULL) return NULL;  
    return insertAfter(L, ((ListaStruct *)L)->cabeca, info);
}

Posic insertBefore(Lista L, Posic p, Item info) {
    if (L == NULL || p == NULL) return NULL;  
    
    ListaStruct *ls = (ListaStruct *)L;
    No *ref  = (No *)p;
    No *novo = criaNo(info);
    if (novo == NULL) return NULL;

    novo->prox = ref;
    novo->ant  = ref->ant;
    ref->ant->prox = novo;
    ref->ant       = novo;

    ls->tamanho++;
    return (Posic)novo;
}

Posic insertAfter(Lista L, Posic p, Item info) {
    if (L == NULL || p == NULL) return NULL;  
    No *ref = (No *)p;
    return insertBefore(L, ref->prox, info);
}



Item removeLista(Lista L, Posic p) {
    if (L == NULL || p == NULL) return NULL; 

    ListaStruct *ls = (ListaStruct *)L;
    No   *no   = (No *)p;
    Item  info = no->info;

    no->ant->prox = no->prox;
    no->prox->ant = no->ant;
    free(no);

    ls->tamanho--;
    return info;
}



int lengthLista(Lista L) {
    if (L == NULL) return 0;
    return ((ListaStruct *)L)->tamanho;
}

bool isEmptyLista(Lista L) {
    if (L == NULL) return true;
    return ((ListaStruct *)L)->tamanho == 0;
}

Item getLista(Lista L, Posic p) {
    if (L == NULL || p == NULL) return NULL;
    return ((No *)p)->info;
}

Posic getFirst(Lista L) {
    if (L == NULL) return NULL;  
    ListaStruct *ls = (ListaStruct *)L;
    if (ls->tamanho == 0) return NULL;
    return (Posic)ls->cabeca->prox;  
}

Posic getLast(Lista L) {
    if (L == NULL) return NULL;  

    ListaStruct *ls = (ListaStruct *)L;
    if (ls->tamanho == 0) return NULL;
    return (Posic)ls->cauda->ant;    
}

Posic getNext(Lista L, Posic p) {
    if (L == NULL || p == NULL) return NULL; 
    ListaStruct *ls = (ListaStruct *)L;
    No *prox = ((No *)p)->prox;
    if (prox == ls->cauda) return NULL;   
    return (Posic)prox;
}

Posic getPrevious(Lista L, Posic p) {
    if (L == NULL || p == NULL) return NULL;  
    ListaStruct *ls = (ListaStruct *)L;
    No *ant = ((No *)p)->ant;
    if (ant == ls->cabeca) return NULL;   
    return (Posic)ant;
}

/* ── busca ── */

Posic buscaLista(Lista L, Item info) {
    if (L == NULL) return NULL;  
    ListaStruct *ls = (ListaStruct *)L;
    No *cur = ls->cabeca->prox;
    while (cur != ls->cauda) {
        if (cur->info == info) return (Posic)cur;
        cur = cur->prox;
    }
    return NULL;
}



void destroiLista(Lista L, void (*destroiItem)(Item)) {
    if (L == NULL) return;
    ListaStruct *ls = (ListaStruct *)L;
    No *cur = ls->cabeca->prox;

    while (cur != ls->cauda) {
        No *prox = cur->prox;
        if (destroiItem != NULL) destroiItem(cur->info);
        free(cur);
        cur = prox;
    }

    free(ls->cabeca);
    free(ls->cauda);
    free(ls);
}