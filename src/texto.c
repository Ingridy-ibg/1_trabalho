#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "texto.h"

/* ─────────────────────────────────────────────
   Estrutura interna (proibido expor no .h)
   ───────────────────────────────────────────── */

typedef struct TextoStruct {
    int    id;
    double x;
    double y;
    char  *corb;
    char  *corp;
    char   ancora;   /* 'i', 'm' ou 'f' */
    char  *txto;
    char  *fFamily;
    char  *fWeight;
    double fSize;
} TextoStruct;

/* ─────────────────────────────────────────────
   Estilo global (comando ts)
   ───────────────────────────────────────────── */

static char   estilo_fFamily[32] = "sans";
static char   estilo_fWeight[8]  = "n";
static double estilo_fSize       = 12.0;

/* ─────────────────────────────────────────────
   Auxiliares internos
   ───────────────────────────────────────────── */

static char *dupString(const char *s) {
    if (s == NULL) return NULL;
    size_t len = strlen(s) + 1;
    char *copy = (char *)malloc(len);
    if (copy == NULL) return NULL;
    memcpy(copy, s, len);
    return copy;
}

static bool ancoraValida(char a) {
    return a == 'i' || a == 'm' || a == 'f';
}

static bool parametrosValidos(int id, const char *corb, const char *corp,
                               char ancora, const char *txto) {
    return id > 0 && corb != NULL && corp != NULL &&
           ancoraValida(ancora) && txto != NULL;
}

/* ─────────────────────────────────────────────
   Estilo global
   ───────────────────────────────────────────── */

void setEstiloTexto(const char *fFamily, const char *fWeight, double fSize) {
    if (fFamily == NULL || fWeight == NULL || fSize <= 0.0) return;

    strncpy(estilo_fFamily, fFamily, sizeof(estilo_fFamily) - 1);
    estilo_fFamily[sizeof(estilo_fFamily) - 1] = '\0';

    strncpy(estilo_fWeight, fWeight, sizeof(estilo_fWeight) - 1);
    estilo_fWeight[sizeof(estilo_fWeight) - 1] = '\0';

    estilo_fSize = fSize;
}

/* ─────────────────────────────────────────────
   criaTexto
   ───────────────────────────────────────────── */

Texto criaTexto(int id, double x, double y, const char *corb, const char *corp,
                char ancora, const char *txto) {
    if (!parametrosValidos(id, corb, corp, ancora, txto)) return NULL;

    TextoStruct *t = (TextoStruct *)malloc(sizeof(TextoStruct));
    if (t == NULL) return NULL;

    t->corb    = dupString(corb);
    t->corp    = dupString(corp);
    t->txto    = dupString(txto);
    t->fFamily = dupString(estilo_fFamily);
    t->fWeight = dupString(estilo_fWeight);

    if (t->corb == NULL || t->corp == NULL ||
        t->txto == NULL || t->fFamily == NULL || t->fWeight == NULL) {
        free(t->corb);
        free(t->corp);
        free(t->txto);
        free(t->fFamily);
        free(t->fWeight);
        free(t);
        return NULL;
    }

    t->id     = id;
    t->x      = x;
    t->y      = y;
    t->ancora = ancora;
    t->fSize  = estilo_fSize;
    return (Texto)t;
}

/* ─────────────────────────────────────────────
   destroiTexto
   ───────────────────────────────────────────── */

void destroiTexto(Texto t) {
    if (t == NULL) return;
    TextoStruct *ts = (TextoStruct *)t;
    free(ts->corb);
    free(ts->corp);
    free(ts->txto);
    free(ts->fFamily);
    free(ts->fWeight);
    free(ts);
}

/* ─────────────────────────────────────────────
   Getters
   ───────────────────────────────────────────── */

int getIdTexto(Texto t) {
    if (t == NULL) return -1;
    return ((TextoStruct *)t)->id;
}

double getXTexto(Texto t) {
    if (t == NULL) return 0.0;
    return ((TextoStruct *)t)->x;
}

double getYTexto(Texto t) {
    if (t == NULL) return 0.0;
    return ((TextoStruct *)t)->y;
}

const char *getCorbTexto(Texto t) {
    if (t == NULL) return NULL;
    return ((TextoStruct *)t)->corb;
}

const char *getCorpTexto(Texto t) {
    if (t == NULL) return NULL;
    return ((TextoStruct *)t)->corp;
}

char getAncora(Texto t) {
    if (t == NULL) return '\0';
    return ((TextoStruct *)t)->ancora;
}

const char *getTxto(Texto t) {
    if (t == NULL) return NULL;
    return ((TextoStruct *)t)->txto;
}

const char *getFamilyTexto(Texto t) {
    if (t == NULL) return NULL;
    return ((TextoStruct *)t)->fFamily;
}

const char *getWeightTexto(Texto t) {
    if (t == NULL) return NULL;
    return ((TextoStruct *)t)->fWeight;
}

double getSizeTexto(Texto t) {
    if (t == NULL) return 0.0;
    return ((TextoStruct *)t)->fSize;
}

/* ─────────────────────────────────────────────
   transladaTexto
   ───────────────────────────────────────────── */

void transladaTexto(Texto t, double dx, double dy) {
    if (t == NULL) return;
    TextoStruct *ts = (TextoStruct *)t;
    ts->x += dx;
    ts->y += dy;
}

/* ─────────────────────────────────────────────
   setCoresTexto
   ───────────────────────────────────────────── */

void setCoresTexto(Texto t, const char *corb, const char *corp) {
    if (t == NULL || corb == NULL || corp == NULL) return;
    TextoStruct *ts = (TextoStruct *)t;

    char *novaCorb = dupString(corb);
    char *novaCorp = dupString(corp);
    if (novaCorb == NULL || novaCorp == NULL) {
        free(novaCorb);
        free(novaCorp);
        return;
    }

    free(ts->corb);
    free(ts->corp);
    ts->corb = novaCorb;
    ts->corp = novaCorp;
}

/* ─────────────────────────────────────────────
   contemPontoTexto / dentroRegiaoTexto
   ───────────────────────────────────────────── */

bool contemPontoTexto(Texto t, double px, double py) {
    if (t == NULL) return false;
    TextoStruct *ts = (TextoStruct *)t;
    return fabs(px - ts->x) < 1e-6 && fabs(py - ts->y) < 1e-6;
}

bool dentroRegiaoTexto(Texto t, double rx, double ry,
                       double rw, double rh) {
    if (t == NULL) return false;
    TextoStruct *ts = (TextoStruct *)t;
    return ts->x >= rx && ts->x <= rx + rw &&
           ts->y >= ry && ts->y <= ry + rh;
}