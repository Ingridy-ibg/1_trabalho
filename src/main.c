/**
 * @file main.c
 * @brief Ponto de entrada do TED — coordena GEO, SVG e QRY.
 *
 * Fluxo:
 *   1. parseArgs        → valida e extrai -e -f -o -q
 *   2. inicializaPoligonos / criaFormas
 *   3. processaGeo      → popula banco
 *   4. svgInicia + svgEscreveFormas + svgFinaliza  → arq.svg
 *   5. Se -q fornecido:
 *        svgInicia + svgEscreveFormas               → arq-qry.svg (base)
 *        processaQry (escreve no svg e no txt)
 *        svgFinaliza                                → fecha arq-qry.svg
 *   6. destroiFormas / destroiPoligonos
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "main.h"
#include "formas.h"
#include "poligono.h"
#include "processadorGEO.h"
#include "processadorQRY.h"
#include "svg.h"

/* ─────────────────────────────────────────────
   parseArgs
   ───────────────────────────────────────────── */

bool parseArgs(int argc, char *argv[], Args *args) {
    if (args == NULL) return false;

    /* Valores padrão */
    strncpy(args->base_entrada, ".", MAX_PATH - 1);
    args->base_entrada[MAX_PATH - 1] = '\0';
    args->arq_geo[0]    = '\0';
    args->base_saida[0] = '\0';
    args->arq_qry[0]    = '\0';

    for (int i = 1; i < argc - 1; i++) {
        if      (strcmp(argv[i], "-e") == 0) {
            strncpy(args->base_entrada, argv[++i], MAX_PATH - 1);
            args->base_entrada[MAX_PATH - 1] = '\0';
        } else if (strcmp(argv[i], "-f") == 0) {
            strncpy(args->arq_geo, argv[++i], MAX_PATH - 1);
            args->arq_geo[MAX_PATH - 1] = '\0';
        } else if (strcmp(argv[i], "-o") == 0) {
            strncpy(args->base_saida, argv[++i], MAX_PATH - 1);
            args->base_saida[MAX_PATH - 1] = '\0';
        } else if (strcmp(argv[i], "-q") == 0) {
            strncpy(args->arq_qry, argv[++i], MAX_PATH - 1);
            args->arq_qry[MAX_PATH - 1] = '\0';
        }
    }

    if (args->arq_geo[0] == '\0' || args->base_saida[0] == '\0') {
        fprintf(stderr,
            "Uso: ted -f arq.geo -o dir_saida [-e dir_entrada] [-q arq.qry]\n");
        return false;
    }
    return true;
}

/* ─────────────────────────────────────────────
   Auxiliar: extrai "stem" (nome sem extensão e sem diretório)
   Ex.: "entrada/cidade.geo" → "cidade"
   ───────────────────────────────────────────── */

static void stem(const char *nome, char *saida, size_t tam) {
    /* Pega só o basename */
    const char *base = strrchr(nome, '/');
    base = (base != NULL) ? base + 1 : nome;

    strncpy(saida, base, tam - 1);
    saida[tam - 1] = '\0';

    /* Remove extensão */
    char *ponto = strrchr(saida, '.');
    if (ponto) *ponto = '\0';
}

/* ─────────────────────────────────────────────
   main
   ───────────────────────────────────────────── */

int main(int argc, char *argv[]) {

    /* ── 1. Argumentos ── */
    Args args;
    if (!parseArgs(argc, argv, &args)) return 1;

    /* ── 2. Estruturas ── */
    inicializaPoligonos();
    Formas fs = criaFormas();
    if (fs == NULL) {
        fprintf(stderr, "ted: falha ao criar banco de formas\n");
        destroiPoligonos();
        return 1;
    }

    /* ── 3. Lê o .geo ── */
    char caminho_geo[MAX_PATH];
    snprintf(caminho_geo, MAX_PATH, "%s/%s", args.base_entrada, args.arq_geo);

    if (!processaGeo(caminho_geo, fs)) {
        destroiFormas(fs);
        destroiPoligonos();
        return 1;
    }

    /* ── 4. Gera arq.svg (sem queries) ── */
    char stem_geo[MAX_PATH];
    stem(args.arq_geo, stem_geo, MAX_PATH);

    char caminho_svg[MAX_PATH];
    snprintf(caminho_svg, MAX_PATH, "%s/%s.svg", args.base_saida, stem_geo);

    FILE *f_svg = fopen(caminho_svg, "w");
    if (f_svg == NULL) {
        fprintf(stderr, "ted: nao foi possivel criar '%s'\n", caminho_svg);
        destroiFormas(fs);
        destroiPoligonos();
        return 1;
    }
    svgInicia(f_svg, fs);
    svgEscreveFormas(f_svg, fs);
    svgFinaliza(f_svg);
    fclose(f_svg);

    /* ── 5. Processa .qry (se fornecido) ── */
    if (args.arq_qry[0] != '\0') {

        char caminho_qry[MAX_PATH];
        snprintf(caminho_qry, MAX_PATH, "%s/%s", args.base_entrada, args.arq_qry);

        char stem_qry[MAX_PATH];
        stem(args.arq_qry, stem_qry, MAX_PATH);

        /* arq-qry.svg */
        char caminho_svg_qry[MAX_PATH];
        snprintf(caminho_svg_qry, MAX_PATH, "%s/%s-%s.svg",
                 args.base_saida, stem_geo, stem_qry);

        /* arq-qry.txt */
        char caminho_txt_qry[MAX_PATH];
        snprintf(caminho_txt_qry, MAX_PATH, "%s/%s-%s.txt",
                 args.base_saida, stem_geo, stem_qry);

        FILE *f_svg_qry = fopen(caminho_svg_qry, "w");
        if (f_svg_qry == NULL) {
            fprintf(stderr, "ted: nao foi possivel criar '%s'\n", caminho_svg_qry);
            destroiFormas(fs);
            destroiPoligonos();
            return 1;
        }

        FILE *f_txt_qry = fopen(caminho_txt_qry, "w");
        if (f_txt_qry == NULL) {
            fprintf(stderr, "ted: nao foi possivel criar '%s'\n", caminho_txt_qry);
            fclose(f_svg_qry);
            destroiFormas(fs);
            destroiPoligonos();
            return 1;
        }

        /* SVG de query começa com todas as formas do .geo */
        svgInicia(f_svg_qry, fs);
        svgEscreveFormas(f_svg_qry, fs);

        /* processaQry escreve marcações no svg e relatório no txt */
        processaQry(caminho_qry, fs, f_svg_qry, f_txt_qry);

        svgFinaliza(f_svg_qry);
        fclose(f_svg_qry);
        fclose(f_txt_qry);
    }

    /* ── 6. Libera recursos ── */
    destroiFormas(fs);
    destroiPoligonos();

    return 0;
}