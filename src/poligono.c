#include <stdlib.h>
#include <stdbool.h>
#include "poligono.h"
#include "fila.h"



typedef struct {
    double x;
    double y;
} Ponto;


static Fila poligonos[MAX_POLIGONOS];



static bool idValido(int p) {
    return p >= 1 && p <= MAX_POLIGONOS;
}

static int idx(int p) {
    return p - 1;
}



void inicializaPoligonos(void) {
    for (int i = 0; i < MAX_POLIGONOS; i++) {
        poligonos[i] = criaFila(MAX_PONTOS_POLIGONO);
    }
}

void destroiPoligonos(void) {
    for (int i = 0; i < MAX_POLIGONOS; i++) {
        /* cada Item na fila é um Ponto* alocado por inserePoligono */
        destroiFila(poligonos[i], free);
        poligonos[i] = NULL;
    }
}


bool inserePoligono(int p, double x, double y) {
    if (!idValido(p)) return false;

    Ponto *pt = (Ponto *)malloc(sizeof(Ponto));
    if (pt == NULL) return false;

    pt->x = x;
    pt->y = y;

    if (!enqueue(poligonos[idx(p)], pt)) {
        free(pt);
        return false;
    }
    return true;
}



bool removePoligono(int p, double *x, double *y) {
    if (!idValido(p)) return false;

    Ponto *pt = (Ponto *)dequeue(poligonos[idx(p)]);
    if (pt == NULL) return false;

    *x = pt->x;
    *y = pt->y;
    free(pt);
    return true;
}



bool peekPoligono(int p, double *x, double *y) {
    if (!idValido(p)) return false;

    Ponto *pt = (Ponto *)peek(poligonos[idx(p)]);
    if (pt == NULL) return false;

    *x = pt->x;
    *y = pt->y;
    return true;
}



bool limpaPoligono(int p) {
    if (!idValido(p)) return false;

    clearFila(poligonos[idx(p)], free);
    return true;
}



int lengthPoligono(int p) {
    if (!idValido(p)) return -1;
    return lengthFila(poligonos[idx(p)]);
}

bool isEmptyPoligono(int p) {
    if (!idValido(p)) return false;
    return isEmptyFila(poligonos[idx(p)]);
}

bool isFullPoligono(int p) {
    if (!idValido(p)) return false;
    return isFullFila(poligonos[idx(p)]);
}



bool getPontosPoligono(int p, double xs[], double ys[], int *n) {
    if (!idValido(p)) return false;

    Fila f  = poligonos[idx(p)];
    int  tam = lengthFila(f);
    *n = 0;

    /* Para iterar sem consumir a fila: faz dequeue de todos,
       copia as coordenadas e reenfileira na mesma ordem. */
    Ponto *pts[MAX_PONTOS_POLIGONO];

    for (int i = 0; i < tam; i++) {
        pts[i] = (Ponto *)dequeue(f);
        xs[i]  = pts[i]->x;
        ys[i]  = pts[i]->y;
    }

    for (int i = 0; i < tam; i++) {
        enqueue(f, pts[i]);
    }

    *n = tam;
    return true;
}