#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "linha.h"

typedef struct LinhaStruct {
    int    id;
    double x1;
    double y1;
    double x2;
    double y2;
    char  *cor;
} LinhaStruct;

static char *dupString(const char *s) {
    if (s == NULL) return NULL;
    size_t len = strlen(s) + 1;
    char *copy = (char *)malloc(len);
    if (copy == NULL) return NULL;
    memcpy(copy, s, len);
    return copy;
}

static bool parametrosValidos(int id, const char *cor) {
    return id > 0 && cor != NULL;
}

Linha criaLinha(int id, double x1, double y1, double x2, double y2,
                const char *cor) {
    if (!parametrosValidos(id, cor)) return NULL;

    LinhaStruct *l = (LinhaStruct *)malloc(sizeof(LinhaStruct));
    if (l == NULL) return NULL;

    l->cor = dupString(cor);
    if (l->cor == NULL) {
        free(l);
        return NULL;
    }

    l->id = id;
    l->x1 = x1;
    l->y1 = y1;
    l->x2 = x2;
    l->y2 = y2;
    return (Linha)l;
}

void destroiLinha(Linha l) {
    if (l == NULL) return;
    LinhaStruct *ls = (LinhaStruct *)l;
    free(ls->cor);
    free(ls);
}

int getIdLinha(Linha l) {
    if (l == NULL) return -1;
    return ((LinhaStruct *)l)->id;
}

double getX1Linha(Linha l) {
    if (l == NULL) return 0.0;
    return ((LinhaStruct *)l)->x1;
}

double getY1Linha(Linha l) {
    if (l == NULL) return 0.0;
    return ((LinhaStruct *)l)->y1;
}

double getX2Linha(Linha l) {
    if (l == NULL) return 0.0;
    return ((LinhaStruct *)l)->x2;
}

double getY2Linha(Linha l) {
    if (l == NULL) return 0.0;
    return ((LinhaStruct *)l)->y2;
}

const char *getCorLinha(Linha l) {
    if (l == NULL) return NULL;
    return ((LinhaStruct *)l)->cor;
}

void transladaLinha(Linha l, double dx, double dy) {
    if (l == NULL) return;
    LinhaStruct *ls = (LinhaStruct *)l;
    ls->x1 += dx;
    ls->y1 += dy;
    ls->x2 += dx;
    ls->y2 += dy;
}

void setCorLinha(Linha l, const char *cor) {
    if (l == NULL || cor == NULL) return;

    LinhaStruct *ls = (LinhaStruct *)l;
    char *novaCor = dupString(cor);
    if (novaCor == NULL) return;

    free(ls->cor);
    ls->cor = novaCor;
}

bool contemPontoLinha(Linha l, double px, double py) {
    if (l == NULL) return false;
    LinhaStruct *ls = (LinhaStruct *)l;

    double dx = ls->x2 - ls->x1;
    double dy = ls->y2 - ls->y1;
    double len_sq = dx * dx + dy * dy;
    if (len_sq == 0.0) {
        // Linha degenerada (ponto), verificar se px,py == x1,y1
        return fabs(px - ls->x1) < 1e-6 && fabs(py - ls->y1) < 1e-6;
    }

    // Projeção do ponto na linha
    double t = ((px - ls->x1) * dx + (py - ls->y1) * dy) / len_sq;
    if (t < 0.0 || t > 1.0) return false;

    // Distância perpendicular
    double proj_x = ls->x1 + t * dx;
    double proj_y = ls->y1 + t * dy;
    double dist = hypot(px - proj_x, py - proj_y);
    return dist < 1e-6; // Tolerância
}

bool dentroRegiaoLinha(Linha l, double rx, double ry,
                       double rw, double rh) {
    if (l == NULL) return false;
    LinhaStruct *ls = (LinhaStruct *)l;

    return ls->x1 >= rx && ls->x1 <= rx + rw &&
           ls->y1 >= ry && ls->y1 <= ry + rh &&
           ls->x2 >= rx && ls->x2 <= rx + rw &&
           ls->y2 >= ry && ls->y2 <= ry + rh;
}
