#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "retangulo.h"

typedef struct RetanguloStruct {
    int    id;
    double x;
    double y;
    double w;
    double h;
    char  *corb;
    char  *corp;
} RetanguloStruct;

static char *dupString(const char *s) {
    if (s == NULL) return NULL;
    size_t len = strlen(s) + 1;
    char *copy = (char *)malloc(len);
    if (copy == NULL) return NULL;
    memcpy(copy, s, len);
    return copy;
}

static bool parametrosValidos(int id, double w, double h,
                             const char *corb, const char *corp) {
    return id > 0 && w > 0.0 && h > 0.0 && corb != NULL && corp != NULL;
}

Retangulo criaRetangulo(int id, double x, double y, double w, double h,
                        const char *corb, const char *corp) {
    if (!parametrosValidos(id, w, h, corb, corp)) return NULL;

    RetanguloStruct *r = (RetanguloStruct *)malloc(sizeof(RetanguloStruct));
    if (r == NULL) return NULL;

    r->corb = dupString(corb);
    r->corp = dupString(corp);
    if (r->corb == NULL || r->corp == NULL) {
        free(r->corb);
        free(r->corp);
        free(r);
        return NULL;
    }

    r->id = id;
    r->x  = x;
    r->y  = y;
    r->w  = w;
    r->h  = h;
    return (Retangulo)r;
}

void destroiRetangulo(Retangulo r) {
    if (r == NULL) return;
    RetanguloStruct *rs = (RetanguloStruct *)r;
    free(rs->corb);
    free(rs->corp);
    free(rs);
}

int getIdRetangulo(Retangulo r) {
    if (r == NULL) return -1;
    return ((RetanguloStruct *)r)->id;
}

double getXRetangulo(Retangulo r) {
    if (r == NULL) return 0.0;
    return ((RetanguloStruct *)r)->x;
}

double getYRetangulo(Retangulo r) {
    if (r == NULL) return 0.0;
    return ((RetanguloStruct *)r)->y;
}

double getWRetangulo(Retangulo r) {
    if (r == NULL) return 0.0;
    return ((RetanguloStruct *)r)->w;
}

double getHRetangulo(Retangulo r) {
    if (r == NULL) return 0.0;
    return ((RetanguloStruct *)r)->h;
}

const char *getCorbRetangulo(Retangulo r) {
    if (r == NULL) return NULL;
    return ((RetanguloStruct *)r)->corb;
}

const char *getCorpRetangulo(Retangulo r) {
    if (r == NULL) return NULL;
    return ((RetanguloStruct *)r)->corp;
}

void transladaRetangulo(Retangulo r, double dx, double dy) {
    if (r == NULL) return;
    RetanguloStruct *rs = (RetanguloStruct *)r;
    rs->x += dx;
    rs->y += dy;
}

void setCoresRetangulo(Retangulo r, const char *corb, const char *corp) {
    if (r == NULL || corb == NULL || corp == NULL) return;

    RetanguloStruct *rs = (RetanguloStruct *)r;
    char *novaCorb = dupString(corb);
    char *novaCorp = dupString(corp);
    if (novaCorb == NULL || novaCorp == NULL) {
        free(novaCorb);
        free(novaCorp);
        return;
    }

    free(rs->corb);
    free(rs->corp);
    rs->corb = novaCorb;
    rs->corp = novaCorp;
}

bool contemPontoRetangulo(Retangulo r, double px, double py) {
    if (r == NULL) return false;
    RetanguloStruct *rs = (RetanguloStruct *)r;
    return px >= rs->x && px <= rs->x + rs->w &&
           py >= rs->y && py <= rs->y + rs->h;
}

bool dentroRegiaoRetangulo(Retangulo r, double rx, double ry,
                           double rw, double rh) {
    if (r == NULL) return false;
    RetanguloStruct *rs = (RetanguloStruct *)r;

    return rs->x >= rx && rs->y >= ry &&
           rs->x + rs->w <= rx + rw &&
           rs->y + rs->h <= ry + rh;
}
