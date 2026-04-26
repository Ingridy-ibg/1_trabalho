/**
 * @file svg.c
 * @brief Implementação do módulo de geração SVG.
 *
 * Coordenadas: usadas diretamente do plano cartesiano, sem inversão de Y.
 * O viewBox é calculado a partir do bounding-box das formas com margem.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#include "svg.h"
#include "formas.h"
#include "circulo.h"
#include "retangulo.h"
#include "linha.h"
#include "texto.h"

/* ─────────────────────────────────────────────
   Constantes
   ───────────────────────────────────────────── */

#define MARGEM          15.0   /* espaço ao redor do bounding-box     */
#define ANELO_R          6.0   /* raio do anel de seleção             */
#define ANELO_SW         2.0   /* stroke-width do anel                */
#define X_BRACO          5.0   /* metade do comprimento do braço do x */
#define X_SW             1.5   /* stroke-width do x de remoção        */
#define SEL_SW           1.5   /* stroke-width da borda de seleção    */

/* ─────────────────────────────────────────────
   Bounding-box
   ───────────────────────────────────────────── */

typedef struct { double xmin, ymin, xmax, ymax; } BBox;

static BBox calculaBBox(Formas fs) {
    BBox bb = { 1e18, 1e18, -1e18, -1e18 };

    PosicForma p = getPrimeiraForma(fs);
    while (p != NULL) {
        const char *tipo = getTipoForma(fs, p);
        void       *d    = getDadosForma(fs, p);

        if (strcmp(tipo, "circulo") == 0) {
            double cx = getXCirculo(d), cy = getYCirculo(d), r = getRCirculo(d);
            if (cx - r < bb.xmin) bb.xmin = cx - r;
            if (cy - r < bb.ymin) bb.ymin = cy - r;
            if (cx + r > bb.xmax) bb.xmax = cx + r;
            if (cy + r > bb.ymax) bb.ymax = cy + r;

        } else if (strcmp(tipo, "retangulo") == 0) {
            double rx = getXRetangulo(d), ry = getYRetangulo(d);
            double rw = getWRetangulo(d), rh = getHRetangulo(d);
            if (rx      < bb.xmin) bb.xmin = rx;
            if (ry      < bb.ymin) bb.ymin = ry;
            if (rx + rw > bb.xmax) bb.xmax = rx + rw;
            if (ry + rh > bb.ymax) bb.ymax = ry + rh;

        } else if (strcmp(tipo, "linha") == 0) {
            double x1 = getX1Linha(d), y1 = getY1Linha(d);
            double x2 = getX2Linha(d), y2 = getY2Linha(d);
            if (x1 < bb.xmin) bb.xmin = x1;  if (y1 < bb.ymin) bb.ymin = y1;
            if (x2 < bb.xmin) bb.xmin = x2;  if (y2 < bb.ymin) bb.ymin = y2;
            if (x1 > bb.xmax) bb.xmax = x1;  if (y1 > bb.ymax) bb.ymax = y1;
            if (x2 > bb.xmax) bb.xmax = x2;  if (y2 > bb.ymax) bb.ymax = y2;

        } else if (strcmp(tipo, "texto") == 0) {
            double tx = getXTexto(d), ty = getYTexto(d);
            if (tx < bb.xmin) bb.xmin = tx;  if (ty < bb.ymin) bb.ymin = ty;
            if (tx > bb.xmax) bb.xmax = tx;  if (ty > bb.ymax) bb.ymax = ty;
        }

        p = getProximaForma(fs, p);
    }

    /* Banco vazio: janela mínima */
    if (bb.xmin >= bb.xmax) { bb.xmin = 0; bb.xmax = 200; }
    if (bb.ymin >= bb.ymax) { bb.ymin = 0; bb.ymax = 200; }
    return bb;
}

/* ─────────────────────────────────────────────
   Helpers de conversão de atributos de texto
   ───────────────────────────────────────────── */

/* Âncora 'i','m','f' → text-anchor SVG */
static const char *ancoraParaSvg(char a) {
    if (a == 'm') return "middle";
    if (a == 'f') return "end";
    return "start";
}

/* Peso "n","b","b+","l" → font-weight SVG */
static const char *pesoParaSvg(const char *w) {
    if (w == NULL)            return "normal";
    if (strcmp(w, "b")  == 0) return "bold";
    if (strcmp(w, "b+") == 0) return "bolder";
    if (strcmp(w, "l")  == 0) return "lighter";
    return "normal";
}

/* Família "sans","serif","cursive" → font-family SVG */
static const char *familiaParaSvg(const char *f) {
    if (f == NULL)               return "sans-serif";
    if (strcmp(f, "sans")  == 0) return "sans-serif";
    if (strcmp(f, "serif") == 0) return "serif";
    return "cursive";
}

/* ─────────────────────────────────────────────
   svgInicia
   ───────────────────────────────────────────── */

void svgInicia(FILE *f, Formas fs) {
    if (f == NULL) return;

    BBox bb = (fs != NULL) ? calculaBBox(fs)
                           : (BBox){ 0, 0, 200, 200 };

    double largura = (bb.xmax - bb.xmin) + 2.0 * MARGEM;
    double altura  = (bb.ymax - bb.ymin) + 2.0 * MARGEM;
    double vx      = bb.xmin - MARGEM;
    double vy      = bb.ymin - MARGEM;

    fprintf(f, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    fprintf(f,
        "<svg xmlns=\"http://www.w3.org/2000/svg\""
        " xmlns:xlink=\"http://www.w3.org/1999/xlink\""
        " width=\"%.4f\" height=\"%.4f\""
        " viewBox=\"%.4f %.4f %.4f %.4f\">\n",
        largura, altura, vx, vy, largura, altura);
    fprintf(f, "<g id=\"fig\">\n");
}

/* ─────────────────────────────────────────────
   svgFinaliza
   ───────────────────────────────────────────── */

void svgFinaliza(FILE *f) {
    if (f == NULL) return;
    fprintf(f, "</g>\n");
    fprintf(f, "</svg>\n");
}

/* ─────────────────────────────────────────────
   svgEscreveFormas
   ───────────────────────────────────────────── */

void svgEscreveFormas(FILE *f, Formas fs) {
    if (f == NULL || fs == NULL) return;

    PosicForma p = getPrimeiraForma(fs);
    while (p != NULL) {
        const char *tipo = getTipoForma(fs, p);
        void       *d    = getDadosForma(fs, p);
        int         id   = getIdForma(fs, p);

        if (strcmp(tipo, "circulo") == 0) {
            fprintf(f,
                "   <circle id=\"%d\""
                " r=\"%.3f\" cx=\"%.6f\" cy=\"%.6f\""
                " fill=\"%s\" stroke=\"%s\""
                " fill-opacity=\"0.5\" stroke-width=\"1.0px\"/>\n",
                id,
                getRCirculo(d),
                getXCirculo(d), getYCirculo(d),
                getCorpCirculo(d), getCorbCirculo(d));

        } else if (strcmp(tipo, "retangulo") == 0) {
            fprintf(f,
                "   <rect id=\"%d\""
                " x=\"%.6f\" y=\"%.6f\""
                " width=\"%.6f\" height=\"%.6f\""
                " fill=\"%s\" stroke=\"%s\""
                " fill-opacity=\"0.5\" stroke-width=\"1.0px\"/>\n",
                id,
                getXRetangulo(d), getYRetangulo(d),
                getWRetangulo(d), getHRetangulo(d),
                getCorpRetangulo(d), getCorbRetangulo(d));

        } else if (strcmp(tipo, "linha") == 0) {
            fprintf(f,
                "   <line id=\"%d\""
                " x1=\"%.6f\" y1=\"%.6f\""
                " x2=\"%.6f\" y2=\"%.6f\""
                " stroke=\"%s\" stroke-width=\"2\""
                " stroke-opacity=\"1.000000\"/>\n",
                id,
                getX1Linha(d), getY1Linha(d),
                getX2Linha(d), getY2Linha(d),
                getCorLinha(d));

        } else if (strcmp(tipo, "texto") == 0) {
            double fsize = getSizeTexto(d);
            if (fsize <= 0.0) fsize = 12.0;

            fprintf(f,
                "   <text x=\"%.6f\" y=\"%.6f\" id=\"%d\""
                " text-anchor=\"%s\""
                " fill=\"%s\" stroke=\"%s\""
                " font-family=\"%s\" font-size=\"%.0fpt\""
                " font-weight=\"%s\">%s</text>\n",
                getXTexto(d), getYTexto(d), id,
                ancoraParaSvg(getAncora(d)),
                getCorpTexto(d), getCorbTexto(d),
                familiaParaSvg(getFamilyTexto(d)),
                fsize,
                pesoParaSvg(getWeightTexto(d)),
                getTxto(d));
        }

        p = getProximaForma(fs, p);
    }
}

/* ─────────────────────────────────────────────
   svgEscreveForma  (escreve uma única forma por id)
   Usado por cmd_pol para gravar linhas no SVG à medida que as insere.
   ───────────────────────────────────────────── */

void svgEscreveForma(FILE *f, Formas fs, int id) {
    if (f == NULL || fs == NULL) return;
    PosicForma pos = buscaFormaPorId(fs, id);
    if (pos == NULL) return;

    /* Reutiliza a lógica de svgEscreveFormas para um único elemento.
       Fazemos isso iterando manualmente para não duplicar código. */
    const char *tipo = getTipoForma(fs, pos);
    void       *d    = getDadosForma(fs, pos);

    if (strcmp(tipo, "linha") == 0) {
        fprintf(f,
            "   <line id=\"%d\""
            " x1=\"%.6f\" y1=\"%.6f\""
            " x2=\"%.6f\" y2=\"%.6f\""
            " stroke=\"%s\" stroke-width=\"2\""
            " stroke-opacity=\"1.000000\"/>\n",
            id,
            getX1Linha(d), getY1Linha(d),
            getX2Linha(d), getY2Linha(d),
            getCorLinha(d));
    }
    /* outros tipos podem ser adicionados se pol gerar mais formas */
}

/* ─────────────────────────────────────────────
   svgSel
   ───────────────────────────────────────────── */

void svgSel(FILE *f, Formas fs, double x, double y, double w, double h) {
    if (f == NULL || fs == NULL) return;

    /* Retângulo pontilhado delimitando a região de seleção */
    fprintf(f,
        "   <!-- sel -->\n"
        "   <rect x=\"%.4f\" y=\"%.4f\" width=\"%.4f\" height=\"%.4f\""
        " stroke=\"red\" fill=\"transparent\""
        " stroke-dasharray=\"5,3\" stroke-width=\"%.1f\"/>\n",
        x, y, w, h, SEL_SW);

    /* Anel em cada âncora selecionada */
    PosicForma p = getPrimeiraForma(fs);
    while (p != NULL) {
        if (isFormaSelecionada(fs, p)) {
            double ax, ay;
            if (getAncoraPorId(fs, getIdForma(fs, p), &ax, &ay)) {
                fprintf(f,
                    "   <circle cx=\"%.4f\" cy=\"%.4f\" r=\"%.1f\""
                    " stroke=\"red\" fill=\"none\""
                    " stroke-width=\"%.1f\"/>\n",
                    ax, ay, ANELO_R, ANELO_SW);
            }
        }
        p = getProximaForma(fs, p);
    }
}

/* ─────────────────────────────────────────────
   svgDels
   ───────────────────────────────────────────── */

void svgDels(FILE *f, double ax, double ay) {
    if (f == NULL) return;

    fprintf(f,
        "   <!-- dels -->\n"
        "   <line x1=\"%.4f\" y1=\"%.4f\" x2=\"%.4f\" y2=\"%.4f\""
        " stroke=\"red\" stroke-width=\"%.1f\"/>\n"
        "   <line x1=\"%.4f\" y1=\"%.4f\" x2=\"%.4f\" y2=\"%.4f\""
        " stroke=\"red\" stroke-width=\"%.1f\"/>\n",
        ax - X_BRACO, ay - X_BRACO, ax + X_BRACO, ay + X_BRACO, X_SW,
        ax + X_BRACO, ay - X_BRACO, ax - X_BRACO, ay + X_BRACO, X_SW);
}