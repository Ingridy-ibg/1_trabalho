#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "circulo.h"

typedef struct CirculoStruct {
    int    id;
    double x;
    double y;
    double r;
    char  *corb;
    char  *corp;
} CirculoStruct;

static char *dupString(const char *s) {
    if (s == NULL) return NULL;
    size_t len = strlen(s) + 1;
    char *copy = (char *)malloc(len);
    if (copy == NULL) return NULL;
    memcpy(copy, s, len);
    return copy;
}

static bool parametrosValidos(int id, double r,
                             const char *corb, const char *corp) {
    return id > 0 && r > 0.0 && corb != NULL && corp != NULL;
}

Circulo criaCirculo(int id, double x, double y, double r,
                    const char *corb, const char *corp) {
    if (!parametrosValidos(id, r, corb, corp)) return NULL;

    CirculoStruct *c = (CirculoStruct *)malloc(sizeof(CirculoStruct));
    if (c == NULL) return NULL;

    c->corb = dupString(corb);
    c->corp = dupString(corp);
    if (c->corb == NULL || c->corp == NULL) {
        free(c->corb);
        free(c->corp);
        free(c);
        return NULL;
    }

    c->id = id;
    c->x  = x;
    c->y  = y;
    c->r  = r;
    return (Circulo)c;
}

void destroiCirculo(Circulo c) {
    if (c == NULL) return;
    CirculoStruct *cs = (CirculoStruct *)c;
    free(cs->corb);
    free(cs->corp);
    free(cs);
}

int getIdCirculo(Circulo c) {
    if (c == NULL) return -1;
    return ((CirculoStruct *)c)->id;
}

double getXCirculo(Circulo c) {
    if (c == NULL) return 0.0;
    return ((CirculoStruct *)c)->x;
}

double getYCirculo(Circulo c) {
    if (c == NULL) return 0.0;
    return ((CirculoStruct *)c)->y;
}

double getRCirculo(Circulo c) {
    if (c == NULL) return 0.0;
    return ((CirculoStruct *)c)->r;
}

const char *getCorbCirculo(Circulo c) {
    if (c == NULL) return NULL;
    return ((CirculoStruct *)c)->corb;
}

const char *getCorpCirculo(Circulo c) {
    if (c == NULL) return NULL;
    return ((CirculoStruct *)c)->corp;
}

void transladaCirculo(Circulo c, double dx, double dy) {
    if (c == NULL) return;
    CirculoStruct *cs = (CirculoStruct *)c;
    cs->x += dx;
    cs->y += dy;
}

void setCoresCirculo(Circulo c, const char *corb, const char *corp) {
    if (c == NULL || corb == NULL || corp == NULL) return;

    CirculoStruct *cs = (CirculoStruct *)c;
    char *novaCorb = dupString(corb);
    char *novaCorp = dupString(corp);
    if (novaCorb == NULL || novaCorp == NULL) {
        free(novaCorb);
        free(novaCorp);
        return;
    }

    free(cs->corb);
    free(cs->corp);
    cs->corb = novaCorb;
    cs->corp = novaCorp;
}

bool contemPontoCirculo(Circulo c, double px, double py) {
    if (c == NULL) return false;
    CirculoStruct *cs = (CirculoStruct *)c;
    
    double dx = px - cs->x;
    double dy = py - cs->y;
    double distancia_sq = dx * dx + dy * dy;
    double raio_sq = cs->r * cs->r;
    
    return distancia_sq <= raio_sq;
}

bool dentroRegiaoCirculo(Circulo c, double rx, double ry,
                         double rw, double rh) {
    if (c == NULL) return false;
    CirculoStruct *cs = (CirculoStruct *)c;

    return cs->x - cs->r >= rx && cs->y - cs->r >= ry &&
           cs->x + cs->r <= rx + rw &&
           cs->y + cs->r <= ry + rh;
}
