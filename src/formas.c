#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "formas.h"
#include "lista.h"
#include "circulo.h"
#include "retangulo.h"
#include "linha.h"
#include "texto.h"


typedef enum {
    TIPO_CIRCULO,
    TIPO_RETANGULO,
    TIPO_LINHA,
    TIPO_TEXTO
} TipoForma;

typedef struct FormaItem {
    TipoForma tipo;
    int       id;
    void     *dados;
    bool      selecionada;   /* cache p/ isFormaSelecionada O(1) */
} FormaItem;

typedef struct FormasStruct {
    Lista banco;          /* Lista de FormaItem*                       */
    Lista selecionadas;   /* Lista de Posic do banco (figuras sel.)    */
    int   total;
} FormasStruct;

/* ─────────────────────────────────────────────
   Auxiliares internos
   ───────────────────────────────────────────── */

static const char *tipoParaString(TipoForma tipo) {
    switch (tipo) {
        case TIPO_CIRCULO:   return "circulo";
        case TIPO_RETANGULO: return "retangulo";
        case TIPO_LINHA:     return "linha";
        case TIPO_TEXTO:     return "texto";
        default:             return NULL;
    }
}

static void destroiItem(Item info) {
    FormaItem *fi = (FormaItem *)info;
    if (fi == NULL) return;
    switch (fi->tipo) {
        case TIPO_CIRCULO:   destroiCirculo(fi->dados);   break;
        case TIPO_RETANGULO: destroiRetangulo(fi->dados); break;
        case TIPO_LINHA:     destroiLinha(fi->dados);     break;
        case TIPO_TEXTO:     destroiTexto(fi->dados);     break;
    }
    free(fi);
}

static FormaItem *criaFormaItem(TipoForma tipo, int id, void *dados) {
    FormaItem *fi = (FormaItem *)malloc(sizeof(FormaItem));
    if (fi == NULL) return NULL;
    fi->tipo        = tipo;
    fi->id          = id;
    fi->dados       = dados;
    fi->selecionada = false;
    return fi;
}

static Posic posicDe(PosicForma p) {
    return (Posic)p;
}

static FormaItem *itemDe(Formas fs, PosicForma p) {
    if (fs == NULL || p == NULL) return NULL;
    FormasStruct *fss = (FormasStruct *)fs;
    return (FormaItem *)getLista(fss->banco, posicDe(p));
}

static bool dentroRegiao(FormaItem *fi, double x, double y, double w, double h) {
    switch (fi->tipo) {
        case TIPO_CIRCULO:   return dentroRegiaoCirculo(fi->dados, x, y, w, h);
        case TIPO_RETANGULO: return dentroRegiaoRetangulo(fi->dados, x, y, w, h);
        case TIPO_LINHA:     return dentroRegiaoLinha(fi->dados, x, y, w, h);
        case TIPO_TEXTO:     return dentroRegiaoTexto(fi->dados, x, y, w, h);
        default:             return false;
    }
}

static void transladaForma(FormaItem *fi, double dx, double dy) {
    switch (fi->tipo) {
        case TIPO_CIRCULO:   transladaCirculo(fi->dados, dx, dy);   break;
        case TIPO_RETANGULO: transladaRetangulo(fi->dados, dx, dy); break;
        case TIPO_LINHA:     transladaLinha(fi->dados, dx, dy);     break;
        case TIPO_TEXTO:     transladaTexto(fi->dados, dx, dy);     break;
    }
}

static void setCoresForma(FormaItem *fi, const char *corb, const char *corp) {
    switch (fi->tipo) {
        case TIPO_CIRCULO:   setCoresCirculo(fi->dados, corb, corp);   break;
        case TIPO_RETANGULO: setCoresRetangulo(fi->dados, corb, corp); break;
        case TIPO_LINHA:     setCorLinha(fi->dados, corb);             break;
        case TIPO_TEXTO:     setCoresTexto(fi->dados, corb, corp);     break;
    }
}

/* Esvazia a lista de selecionadas SEM liberar os Posic (são apenas
   referências para nós do banco — não são heap-alocados aqui). */
static void limpaListaSelecionadas(Lista L) {
    while (!isEmptyLista(L))
        removeLista(L, getFirst(L));   /* descarta o Posic retornado */
}

/* ─────────────────────────────────────────────
   Ciclo de vida
   ───────────────────────────────────────────── */

Formas criaFormas(void) {
    FormasStruct *fs = (FormasStruct *)malloc(sizeof(FormasStruct));
    if (fs == NULL) return NULL;

    fs->banco        = criaLista();
    fs->selecionadas = criaLista();
    fs->total        = 0;

    if (fs->banco == NULL || fs->selecionadas == NULL) {
        if (fs->banco)        destroiLista(fs->banco, NULL);
        if (fs->selecionadas) destroiLista(fs->selecionadas, NULL);
        free(fs);
        return NULL;
    }
    return (Formas)fs;
}

void destroiFormas(Formas fs) {
    if (fs == NULL) return;
    FormasStruct *fss = (FormasStruct *)fs;
    /* selecionadas só guarda Posic do banco — não há nada a liberar */
    destroiLista(fss->selecionadas, NULL);
    destroiLista(fss->banco, destroiItem);
    free(fss);
}

/* ─────────────────────────────────────────────
   Inserção
   ───────────────────────────────────────────── */

static bool insereForma(Formas fs, TipoForma tipo, int id, void *dados) {
    if (fs == NULL || dados == NULL) return false;
    FormasStruct *fss = (FormasStruct *)fs;

    FormaItem *fi = criaFormaItem(tipo, id, dados);
    if (fi == NULL) return false;

    Posic p = insereFinalLista(fss->banco, fi);
    if (p == NULL) {
        free(fi);
        return false;
    }
    fss->total++;
    return true;
}

bool insereCirculo(Formas fs, int id, double x, double y, double r,
                   const char *corb, const char *corp) {
    Circulo c = criaCirculo(id, x, y, r, corb, corp);
    if (c == NULL) return false;
    if (!insereForma(fs, TIPO_CIRCULO, id, c)) {
        destroiCirculo(c);
        return false;
    }
    return true;
}

bool insereRetangulo(Formas fs, int id, double x, double y, double w, double h,
                     const char *corb, const char *corp) {
    Retangulo r = criaRetangulo(id, x, y, w, h, corb, corp);
    if (r == NULL) return false;
    if (!insereForma(fs, TIPO_RETANGULO, id, r)) {
        destroiRetangulo(r);
        return false;
    }
    return true;
}

bool insereLinha(Formas fs, int id, double x1, double y1,
                 double x2, double y2, const char *cor) {
    Linha l = criaLinha(id, x1, y1, x2, y2, cor);
    if (l == NULL) return false;
    if (!insereForma(fs, TIPO_LINHA, id, l)) {
        destroiLinha(l);
        return false;
    }
    return true;
}

bool insereTexto(Formas fs, int id, double x, double y,
                 const char *corb, const char *corp,
                 char ancora, const char *txto) {
    Texto t = criaTexto(id, x, y, corb, corp, ancora, txto);
    if (t == NULL) return false;
    if (!insereForma(fs, TIPO_TEXTO, id, t)) {
        destroiTexto(t);
        return false;
    }
    return true;
}

/* ─────────────────────────────────────────────
   Consulta por identificador
   ───────────────────────────────────────────── */

PosicForma buscaFormaPorId(Formas fs, int id) {
    if (fs == NULL) return NULL;
    FormasStruct *fss = (FormasStruct *)fs;

    Posic p = getFirst(fss->banco);
    while (p != NULL) {
        FormaItem *fi = (FormaItem *)getLista(fss->banco, p);
        if (fi->id == id) return (PosicForma)p;
        p = getNext(fss->banco, p);
    }
    return NULL;
}

bool getAncoraPorId(Formas fs, int id, double *x, double *y) {
    if (fs == NULL || x == NULL || y == NULL) return false;

    PosicForma p = buscaFormaPorId(fs, id);
    if (p == NULL) return false;

    FormaItem *fi = itemDe(fs, p);
    switch (fi->tipo) {
        case TIPO_CIRCULO:
            *x = getXCirculo(fi->dados);
            *y = getYCirculo(fi->dados);
            break;
        case TIPO_RETANGULO:
            *x = getXRetangulo(fi->dados);
            *y = getYRetangulo(fi->dados);
            break;
        case TIPO_LINHA: {
            double x1 = getX1Linha(fi->dados), y1 = getY1Linha(fi->dados);
            double x2 = getX2Linha(fi->dados), y2 = getY2Linha(fi->dados);
            if (x1 < x2 || (x1 == x2 && y1 <= y2)) { *x = x1; *y = y1; }
            else                                   { *x = x2; *y = y2; }
            break;
        }
        case TIPO_TEXTO:
            *x = getXTexto(fi->dados);
            *y = getYTexto(fi->dados);
            break;
        default:
            return false;
    }
    return true;
}

const char *getTipoForma(Formas fs, PosicForma p) {
    FormaItem *fi = itemDe(fs, p);
    if (fi == NULL) return NULL;
    return tipoParaString(fi->tipo);
}

int getIdForma(Formas fs, PosicForma p) {
    FormaItem *fi = itemDe(fs, p);
    if (fi == NULL) return -1;
    return fi->id;
}

void *getDadosForma(Formas fs, PosicForma p) {
    FormaItem *fi = itemDe(fs, p);
    if (fi == NULL) return NULL;
    return fi->dados;
}

/* ─────────────────────────────────────────────
   Iteração no banco
   ───────────────────────────────────────────── */

PosicForma getPrimeiraForma(Formas fs) {
    if (fs == NULL) return NULL;
    return (PosicForma)getFirst(((FormasStruct *)fs)->banco);
}

PosicForma getProximaForma(Formas fs, PosicForma p) {
    if (fs == NULL || p == NULL) return NULL;
    return (PosicForma)getNext(((FormasStruct *)fs)->banco, posicDe(p));
}

int totalFormas(Formas fs) {
    if (fs == NULL) return 0;
    return ((FormasStruct *)fs)->total;
}

/* ─────────────────────────────────────────────
   Seleção
   ───────────────────────────────────────────── */

void selecionaFormas(Formas fs, double x, double y, double w, double h) {
    if (fs == NULL) return;
    FormasStruct *fss = (FormasStruct *)fs;

    /* Desconsidera seleções anteriores */
    limpaListaSelecionadas(fss->selecionadas);

    Posic p = getFirst(fss->banco);
    while (p != NULL) {
        FormaItem *fi = (FormaItem *)getLista(fss->banco, p);
        bool dentro = dentroRegiao(fi, x, y, w, h);
        fi->selecionada = dentro;                            /* cache p/ O(1) */
        if (dentro)
            insereFinalLista(fss->selecionadas, (Item)p);    /* fonte canônica */
        p = getNext(fss->banco, p);
    }
}

bool isFormaSelecionada(Formas fs, PosicForma p) {
    FormaItem *fi = itemDe(fs, p);
    if (fi == NULL) return false;
    return fi->selecionada;
}

int totalFormasSelecionadas(Formas fs) {
    if (fs == NULL) return 0;
    return lengthLista(((FormasStruct *)fs)->selecionadas);
}

/* ─────────────────────────────────────────────
   dels e mcs 
   ───────────────────────────────────────────── */

void removeFormasSelecionadas(Formas fs) {
    if (fs == NULL) return;
    FormasStruct *fss = (FormasStruct *)fs;

    /* Itera a lista de selecionadas e remove cada uma do banco */
    Posic ps = getFirst(fss->selecionadas);
    while (ps != NULL) {
        Posic prox_ps = getNext(fss->selecionadas, ps);
        Posic p_banco = (Posic)getLista(fss->selecionadas, ps);

        FormaItem *fi = (FormaItem *)removeLista(fss->banco, p_banco);
        destroiItem(fi);
        fss->total--;

        ps = prox_ps;
    }

    /* Esvazia a lista (Posic já invalidados pela remoção do banco) */
    limpaListaSelecionadas(fss->selecionadas);
}

void modificaFormasSelecionadas(Formas fs, double dx, double dy,
                                const char *corb, const char *corp) {
    if (fs == NULL || corb == NULL || corp == NULL) return;
    FormasStruct *fss = (FormasStruct *)fs;

    /* Itera a lista de selecionadas e modifica cada figura no banco */
    Posic ps = getFirst(fss->selecionadas);
    while (ps != NULL) {
        Posic p_banco = (Posic)getLista(fss->selecionadas, ps);
        FormaItem *fi = (FormaItem *)getLista(fss->banco, p_banco);
        transladaForma(fi, dx, dy);
        setCoresForma(fi, corb, corp);
        ps = getNext(fss->selecionadas, ps);
    }
}