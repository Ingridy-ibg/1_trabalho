/**
 * @file svg.c
 * @brief Implementação do módulo de geração SVG.
 *
 * Conversão de coordenadas:
 *   O plano cartesiano tem y=0 embaixo, y crescendo para cima.
 *   O SVG tem y=0 em cima, y crescendo para baixo.
 *
 *   Fórmula:  y_svg = altura_total - (y_cart - y_cart_min) - MARGEM
 *
 *   Os valores g_altura e g_ymin_cart são calculados em svgInicia()
 *   a partir do bounding-box das formas e são usados por todo o módulo.
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

#define MARGEM          20.0   /* espaço ao redor do bounding-box     */
#define ANELO_R          6.0   /* raio do anel de seleção             */
#define ANELO_SW         2.0   /* stroke-width do anel                */
#define X_BRACO          5.0   /* metade do comprimento do braço do x */
#define X_SW             1.5   /* stroke-width do x de remoção        */
#define SEL_SW           1.5   /* stroke-width da borda de seleção    */

/* ─────────────────────────────────────────────
   Estado interno do módulo
   ───────────────────────────────────────────── */

/* Calculados em svgInicia(); usados em toSvgY(). */
static double g_altura    = 200.0;  /* altura total do viewport SVG   */
static double g_ymin_cart = 0.0;    /* menor y cartesiano do bbox     */
static double g_xmin_cart = 0.0;    /* menor x cartesiano (para rect) */

/* ─────────────────────────────────────────────
   Conversão de coordenadas
   ───────────────────────────────────────────── */

/**
 * Converte coordenada y do plano cartesiano para y SVG.
 * O ponto y_cart == g_ymin_cart mapeia para y_svg == g_altura - MARGEM
 * (borda inferior do viewport).
 */
static double toSvgY(double y_cart) {
    return g_altura - (y_cart - g_ymin_cart) - MARGEM;
}

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
    if (f == NULL)              return "sans-serif";
    if (strcmp(f, "sans") == 0) return "sans-serif";
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

    double largura = (bb.xmax - bb.xmin) + 2 * MARGEM;
    double altura  = (bb.ymax - bb.ymin) + 2 * MARGEM;

    /* Salva estado para conversões posteriores */
    g_altura    = altura;
    g_ymin_cart = bb.ymin;
    g_xmin_cart = bb.xmin;

    fprintf(f, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    fprintf(f,
        "<svg xmlns=\"http://www.w3.org/2000/svg\""
        " width=\"%.4f\" height=\"%.4f\""
        " viewBox=\"%.4f 0 %.4f %.4f\">\n",
        largura, altura,
        bb.xmin - MARGEM,
        largura, altura);
}

/* ─────────────────────────────────────────────
   svgFinaliza
   ───────────────────────────────────────────── */

void svgFinaliza(FILE *f) {
    if (f == NULL) return;
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

        if (strcmp(tipo, "circulo") == 0) {
            fprintf(f,
                "  <circle cx=\"%.4f\" cy=\"%.4f\" r=\"%.4f\""
                " stroke=\"%s\" fill=\"%s\"/>\n",
                getXCirculo(d),
                toSvgY(getYCirculo(d)),
                getRCirculo(d),
                getCorbCirculo(d),
                getCorpCirculo(d));

        } else if (strcmp(tipo, "retangulo") == 0) {
            double rx   = getXRetangulo(d);
            double ry_c = getYRetangulo(d);
            double rw   = getWRetangulo(d);
            double rh   = getHRetangulo(d);
            /* Canto sup. esq. em SVG = y cartesiano do canto inf. esq. + h */
            double ry_svg = toSvgY(ry_c + rh);
            fprintf(f,
                "  <rect x=\"%.4f\" y=\"%.4f\" width=\"%.4f\" height=\"%.4f\""
                " stroke=\"%s\" fill=\"%s\"/>\n",
                rx, ry_svg, rw, rh,
                getCorbRetangulo(d),
                getCorpRetangulo(d));

        } else if (strcmp(tipo, "linha") == 0) {
            fprintf(f,
                "  <line x1=\"%.4f\" y1=\"%.4f\" x2=\"%.4f\" y2=\"%.4f\""
                " stroke=\"%s\"/>\n",
                getX1Linha(d), toSvgY(getY1Linha(d)),
                getX2Linha(d), toSvgY(getY2Linha(d)),
                getCorLinha(d));

        } else if (strcmp(tipo, "texto") == 0) {
            double fsize = getSizeTexto(d);
            if (fsize <= 0.0) fsize = 12.0;

            fprintf(f,
                "  <text x=\"%.4f\" y=\"%.4f\""
                " stroke=\"%s\" fill=\"%s\""
                " font-family=\"%s\" font-weight=\"%s\" font-size=\"%.2f\""
                " text-anchor=\"%s\">%s</text>\n",
                getXTexto(d),
                toSvgY(getYTexto(d)),
                getCorbTexto(d),
                getCorpTexto(d),
                familiaParaSvg(getFamilyTexto(d)),
                pesoParaSvg(getWeightTexto(d)),
                fsize,
                ancoraParaSvg(getAncora(d)),
                getTxto(d));
        }

        p = getProximaForma(fs, p);
    }
}

/* ─────────────────────────────────────────────
   svgSel
   ───────────────────────────────────────────── */

void svgSel(FILE *f, Formas fs, double x, double y, double w, double h) {
    if (f == NULL || fs == NULL) return;

    /* Retângulo pontilhado:
       O canto superior em SVG corresponde ao y cartesiano + h */
    double ry_svg = toSvgY(y + h);
    fprintf(f,
        "  <!-- sel -->\n"
        "  <rect x=\"%.4f\" y=\"%.4f\" width=\"%.4f\" height=\"%.4f\""
        " stroke=\"red\" fill=\"transparent\""
        " stroke-dasharray=\"5,3\" stroke-width=\"%.1f\"/>\n",
        x, ry_svg, w, h, SEL_SW);

    /* Anel em cada âncora selecionada */
    PosicForma p = getPrimeiraForma(fs);
    while (p != NULL) {
        if (isFormaSelecionada(fs, p)) {
            double ax, ay;
            if (getAncoraPorId(fs, getIdForma(fs, p), &ax, &ay)) {
                fprintf(f,
                    "  <circle cx=\"%.4f\" cy=\"%.4f\" r=\"%.1f\""
                    " stroke=\"red\" fill=\"none\" stroke-width=\"%.1f\"/>\n",
                    ax, toSvgY(ay), ANELO_R, ANELO_SW);
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

    double sx = ax, sy = toSvgY(ay);
    fprintf(f,
        "  <!-- dels -->\n"
        "  <line x1=\"%.4f\" y1=\"%.4f\" x2=\"%.4f\" y2=\"%.4f\""
        " stroke=\"red\" stroke-width=\"%.1f\"/>\n"
        "  <line x1=\"%.4f\" y1=\"%.4f\" x2=\"%.4f\" y2=\"%.4f\""
        " stroke=\"red\" stroke-width=\"%.1f\"/>\n",
        sx - X_BRACO, sy - X_BRACO, sx + X_BRACO, sy + X_BRACO, X_SW,
        sx + X_BRACO, sy - X_BRACO, sx - X_BRACO, sy + X_BRACO, X_SW);
}