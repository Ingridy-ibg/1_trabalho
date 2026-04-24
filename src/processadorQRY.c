/**
 * @file processadorQRY.c
 * @brief Implementação do processador de arquivos .qry.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#include "processadorQRY.h"
#include "formas.h"
#include "poligono.h"
#include "circulo.h"
#include "retangulo.h"
#include "linha.h"
#include "texto.h"
#include "svg.h"

/* ─────────────────────────────────────────────
   Constantes internas
   ───────────────────────────────────────────── */

#define MAX_LINHA    4096
#define MAX_REMOVIDAS 4096   /* limite prático de formas selecionadas */

/* ─────────────────────────────────────────────
   TXT helpers — formato dos relatórios
   ───────────────────────────────────────────── */

/* inp p i: reporta âncora inserida e tipo/id da figura */
static void txt_inp(FILE *txt, int p, int id,
                    double ax, double ay, Formas fs) {
    const char *tipo = "?";
    PosicForma pos = buscaFormaPorId(fs, id);
    if (pos != NULL) tipo = getTipoForma(fs, pos);
    fprintf(txt, "inp %d %d: (%.4f, %.4f) %s\n", p, id, ax, ay, tipo);
}

/* rmp p: reporta a coordenada removida */
static void txt_rmp(FILE *txt, int p, double ax, double ay) {
    fprintf(txt, "rmp %d: (%.4f, %.4f)\n", p, ax, ay);
}

/* sel: reporta id e tipo de cada figura selecionada */
static void txt_sel(FILE *txt, Formas fs) {
    PosicForma p = getPrimeiraForma(fs);
    while (p != NULL) {
        if (isFormaSelecionada(fs, p))
            fprintf(txt, "%d %s\n",
                    getIdForma(fs, p),
                    getTipoForma(fs, p));
        p = getProximaForma(fs, p);
    }
}

/* dels: reporta id, tipo e dados de cada forma removida */
static void txt_dels_forma(FILE *txt, Formas fs, int id) {
    PosicForma pos = buscaFormaPorId(fs, id);
    if (pos == NULL) return;

    const char *tipo  = getTipoForma(fs, pos);
    void       *dados = getDadosForma(fs, pos);

    fprintf(txt, "%d %s", id, tipo);

    if (strcmp(tipo, "circulo") == 0) {
        fprintf(txt, " x=%.4f y=%.4f r=%.4f corb=%s corp=%s",
                getXCirculo(dados), getYCirculo(dados), getRCirculo(dados),
                getCorbCirculo(dados), getCorpCirculo(dados));

    } else if (strcmp(tipo, "retangulo") == 0) {
        fprintf(txt, " x=%.4f y=%.4f w=%.4f h=%.4f corb=%s corp=%s",
                getXRetangulo(dados), getYRetangulo(dados),
                getWRetangulo(dados), getHRetangulo(dados),
                getCorbRetangulo(dados), getCorpRetangulo(dados));

    } else if (strcmp(tipo, "linha") == 0) {
        fprintf(txt, " x1=%.4f y1=%.4f x2=%.4f y2=%.4f cor=%s",
                getX1Linha(dados), getY1Linha(dados),
                getX2Linha(dados), getY2Linha(dados),
                getCorLinha(dados));

    } else if (strcmp(tipo, "texto") == 0) {
        fprintf(txt, " x=%.4f y=%.4f ancora=%c txto=%s",
                getXTexto(dados), getYTexto(dados),
                getAncora(dados), getTxto(dados));
    }
    fprintf(txt, "\n");
}

/* ─────────────────────────────────────────────
   Algoritmo de preenchimento de polígono (scanline)
   ───────────────────────────────────────────── */

static int cmp_double(const void *a, const void *b) {
    double da = *(const double *)a;
    double db = *(const double *)b;
    return (da > db) - (da < db);
}

/**
 * Calcula a interseção do segmento (x1,y1)-(x2,y2) com y=yscan.
 * Usa a regra da metade: inclui borda inferior, exclui superior.
 * Retorna true e preenche *xi se houver interseção.
 */
static bool intersecao_h(double x1, double y1, double x2, double y2,
                          double yscan, double *xi) {
    if (y1 == y2) return false;
    double ymin = (y1 < y2) ? y1 : y2;
    double ymax = (y1 > y2) ? y1 : y2;
    if (yscan < ymin || yscan >= ymax) return false;
    *xi = x1 + (yscan - y1) * (x2 - x1) / (y2 - y1);
    return true;
}

/**
 * Executa o comando pol:
 * gera linhas de borda (n vértices → n lados) e linhas de preenchimento
 * horizontal com distância d, inserindo tudo no banco com IDs a partir
 * de id_inicio. Retorna o número total de linhas inseridas, ou -1 em erro.
 */
static int executa_pol(Formas fs, int p, int id_inicio, double d,
                       const char *corb, const char *corp) {
    double xs[MAX_PONTOS_POLIGONO], ys[MAX_PONTOS_POLIGONO];
    int n = 0;

    if (!getPontosPoligono(p, xs, ys, &n) || n < 2) return -1;

    int prox_id = id_inicio;

    /* ── Bordas: n lados fechando o polígono ── */
    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        if (!insereLinha(fs, prox_id++,
                         xs[i], ys[i], xs[j], ys[j], corb))
            return -1;
    }

    /* ── Preenchimento: scanline horizontal ── */
    double ymin = ys[0], ymax = ys[0];
    for (int i = 1; i < n; i++) {
        if (ys[i] < ymin) ymin = ys[i];
        if (ys[i] > ymax) ymax = ys[i];
    }

    for (double yscan = ymin + d; yscan < ymax; yscan += d) {
        double xis[MAX_PONTOS_POLIGONO];
        int    ni = 0;

        for (int i = 0; i < n; i++) {
            int j = (i + 1) % n;
            double xi;
            if (intersecao_h(xs[i], ys[i], xs[j], ys[j], yscan, &xi))
                xis[ni++] = xi;
        }

        if (ni < 2) continue;
        qsort(xis, ni, sizeof(double), cmp_double);

        for (int k = 0; k + 1 < ni; k += 2) {
            if (!insereLinha(fs, prox_id++,
                             xis[k], yscan, xis[k+1], yscan, corp))
                return -1;
        }
    }

    return prox_id - id_inicio;
}

/* ─────────────────────────────────────────────
   Handlers por comando
   ───────────────────────────────────────────── */

/* inp p i */
static void cmd_inp(char *linha, Formas fs, FILE *svg, FILE *txt) {
    (void)svg;
    int p, id;
    if (sscanf(linha, "%*s %d %d", &p, &id) != 2) return;

    double ax, ay;
    if (!getAncoraPorId(fs, id, &ax, &ay)) return;

    inserePoligono(p, ax, ay);
    txt_inp(txt, p, id, ax, ay, fs);
}

/* rmp p */
static void cmd_rmp(char *linha, FILE *txt) {
    int p;
    if (sscanf(linha, "%*s %d", &p) != 1) return;

    double ax, ay;
    if (removePoligono(p, &ax, &ay))
        txt_rmp(txt, p, ax, ay);
}

/* pol p i d corb corp */
static void cmd_pol(char *linha, Formas fs) {
    int    p, id_inicio;
    double d;
    char   corb[64], corp[64];

    if (sscanf(linha, "%*s %d %d %lf %63s %63s",
               &p, &id_inicio, &d, corb, corp) != 5)
        return;

    executa_pol(fs, p, id_inicio, d, corb, corp);
}

/* clp p */
static void cmd_clp(char *linha) {
    int p;
    if (sscanf(linha, "%*s %d", &p) == 1)
        limpaPoligono(p);
}

/*
 * sel x y w h
 *
 * SVG: retângulo pontilhado vermelho + anel em cada âncora selecionada.
 * TXT: lista id e tipo de cada forma selecionada.
 *
 * Nota: as coordenadas do .qry estão no plano cartesiano (Y para cima).
 * A conversão para SVG (Y para baixo) ficará no módulo svg.c.
 * Aqui passamos as coordenadas SVG diretamente apenas para os helpers
 * inline acima — como o svg.c ainda não existe, usamos y_svg = y para
 * não criar dependência circular. Quando svg.c for criado, basta
 * substituir as chamadas svg_* pelas funções exportadas por svg.h.
 */
static void cmd_sel(char *linha, Formas fs, FILE *svg, FILE *txt) {
    double x, y, w, h;
    if (sscanf(linha, "%*s %lf %lf %lf %lf", &x, &y, &w, &h) != 4) return;

    selecionaFormas(fs, x, y, w, h);
    svgSel(svg, fs, x, y, w, h);
    txt_sel(txt, fs);
}

/* dels */
static void cmd_dels(Formas fs, FILE *svg, FILE *txt) {
    /* Coleta IDs e âncoras ANTES de remover (dados ainda válidos) */
    int    ids[MAX_REMOVIDAS];
    double axs[MAX_REMOVIDAS], ays[MAX_REMOVIDAS];
    int    n = 0;

    PosicForma p = getPrimeiraForma(fs);
    while (p != NULL && n < MAX_REMOVIDAS) {
        if (isFormaSelecionada(fs, p)) {
            int id = getIdForma(fs, p);
            double ax, ay;
            if (getAncoraPorId(fs, id, &ax, &ay)) {
                ids[n] = id; axs[n] = ax; ays[n] = ay;
                n++;
            }
        }
        p = getProximaForma(fs, p);
    }

    /* TXT: reporta antes de remover */
    for (int i = 0; i < n; i++)
        txt_dels_forma(txt, fs, ids[i]);

    /* Remove do banco */
    removeFormasSelecionadas(fs);

    /* SVG: "x" nas posições das âncoras removidas */
    for (int i = 0; i < n; i++)
        svgDels(svg, axs[i], ays[i]);
}

/* mcs dx dy corb corp */
static void cmd_mcs(char *linha, Formas fs) {
    double dx, dy;
    char   corb[64], corp[64];

    if (sscanf(linha, "%*s %lf %lf %63s %63s",
               &dx, &dy, corb, corp) == 4)
        modificaFormasSelecionadas(fs, dx, dy, corb, corp);
}

/* ─────────────────────────────────────────────
   processaQry
   ───────────────────────────────────────────── */

bool processaQry(const char *caminho, Formas fs,
                 FILE *svg_out, FILE *txt_out) {
    if (caminho == NULL || fs == NULL ||
        svg_out == NULL || txt_out == NULL) return false;

    FILE *f = fopen(caminho, "r");
    if (f == NULL) {
        fprintf(stderr, "processaQry: nao foi possivel abrir '%s'\n", caminho);
        return false;
    }

    char linha[MAX_LINHA];
    while (fgets(linha, sizeof(linha), f) != NULL) {
        linha[strcspn(linha, "\r\n")] = '\0';

        /* Pula espaços iniciais */
        char *p = linha;
        while (*p == ' ' || *p == '\t') p++;

        if (*p == '\0' || *p == '#') continue;

        char cmd[16];
        if (sscanf(p, "%15s", cmd) != 1) continue;

        if      (strcmp(cmd, "inp") == 0) cmd_inp(p, fs, svg_out, txt_out);
        else if (strcmp(cmd, "rmp") == 0) cmd_rmp(p, txt_out);
        else if (strcmp(cmd, "pol") == 0) cmd_pol(p, fs);
        else if (strcmp(cmd, "clp") == 0) cmd_clp(p);
        else if (strcmp(cmd, "sel") == 0) cmd_sel(p, fs, svg_out, txt_out);
        else if (strcmp(cmd, "dels") == 0) cmd_dels(fs, svg_out, txt_out);
        else if (strcmp(cmd, "mcs") == 0)  cmd_mcs(p, fs);
        /* Outros comandos são ignorados */
    }

    fclose(f);
    return true;
}