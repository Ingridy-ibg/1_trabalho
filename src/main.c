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
 *        processaQry    → modifica banco (dels/mcs/pol) e
 *                         coleta anotações em arquivo temporário
 *        svgInicia + svgEscreveFormas → escreve estado FINAL do banco
 *        copia anotações do temp para o SVG
 *        svgFinaliza                  → fecha arq-qry.svg
 *   6. destroiFormas / destroiPoligonos / destroiArgs
 *
 * A implementação de parseArgs e dos getters está em args.c.
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
   Auxiliar: extrai "stem" (nome sem extensão e sem diretório)
   Ex.: "entrada/cidade.geo" → "cidade"
   ───────────────────────────────────────────── */

static void stem(const char *nome, char *saida, size_t tam) {
    const char *base = strrchr(nome, '/');
    base = (base != NULL) ? base + 1 : nome;

    strncpy(saida, base, tam - 1);
    saida[tam - 1] = '\0';

    char *ponto = strrchr(saida, '.');
    if (ponto) *ponto = '\0';
}

/* ─────────────────────────────────────────────
   Auxiliar: copia conteúdo de 'src' para 'dst'
   ───────────────────────────────────────────── */

static void copiaArquivo(FILE *src, FILE *dst) {
    rewind(src);
    int c;
    while ((c = fgetc(src)) != EOF)
        fputc(c, dst);
}

/* ─────────────────────────────────────────────
   main
   ───────────────────────────────────────────── */

int main(int argc, char *argv[]) {

    /* ── 1. Argumentos ── */
    Args args = parseArgs(argc, argv);
    if (args == NULL) return 1;

    /* ── 2. Estruturas ── */
    inicializaPoligonos();
    Formas fs = criaFormas();
    if (fs == NULL) {
        fprintf(stderr, "ted: falha ao criar banco de formas\n");
        destroiPoligonos();
        destroiArgs(args);
        return 1;
    }

    /* ── 3. Lê o .geo ── */
    char caminho_geo[MAX_PATH];
    snprintf(caminho_geo, MAX_PATH, "%s/%s",
             getBaseEntrada(args), getArqGeo(args));

    if (!processaGeo(caminho_geo, fs)) {
        destroiFormas(fs);
        destroiPoligonos();
        destroiArgs(args);
        return 1;
    }

    /* ── 4. Gera arq.svg (sem queries) ── */
    char stem_geo[MAX_PATH];
    stem(getArqGeo(args), stem_geo, MAX_PATH);

    char caminho_svg[MAX_PATH];
    snprintf(caminho_svg, MAX_PATH, "%s/%s.svg",
             getBaseSaida(args), stem_geo);

    FILE *f_svg = fopen(caminho_svg, "w");
    if (f_svg == NULL) {
        fprintf(stderr, "ted: nao foi possivel criar '%s'\n", caminho_svg);
        destroiFormas(fs);
        destroiPoligonos();
        destroiArgs(args);
        return 1;
    }
    svgInicia(f_svg, fs);
    svgEscreveFormas(f_svg, fs);
    svgFinaliza(f_svg);
    fclose(f_svg);

    /* ── 5. Processa .qry (se fornecido) ── */
    const char *arq_qry = getArqQry(args);
    if (arq_qry[0] != '\0') {

        char caminho_qry[MAX_PATH];
        snprintf(caminho_qry, MAX_PATH, "%s/%s",
                 getBaseEntrada(args), arq_qry);

        char stem_qry[MAX_PATH];
        stem(arq_qry, stem_qry, MAX_PATH);

        char caminho_svg_qry[MAX_PATH];
        snprintf(caminho_svg_qry, MAX_PATH, "%s/%s-%s.svg",
                 getBaseSaida(args), stem_geo, stem_qry);

        char caminho_txt_qry[MAX_PATH];
        snprintf(caminho_txt_qry, MAX_PATH, "%s/%s-%s.txt",
                 getBaseSaida(args), stem_geo, stem_qry);

        FILE *f_svg_qry = fopen(caminho_svg_qry, "w");
        if (f_svg_qry == NULL) {
            fprintf(stderr, "ted: nao foi possivel criar '%s'\n", caminho_svg_qry);
            destroiFormas(fs);
            destroiPoligonos();
            destroiArgs(args);
            return 1;
        }

        FILE *f_txt_qry = fopen(caminho_txt_qry, "w");
        if (f_txt_qry == NULL) {
            fprintf(stderr, "ted: nao foi possivel criar '%s'\n", caminho_txt_qry);
            fclose(f_svg_qry);
            destroiFormas(fs);
            destroiPoligonos();
            destroiArgs(args);
            return 1;
        }

        FILE *f_ann = tmpfile();
        if (f_ann == NULL) {
            fprintf(stderr, "ted: nao foi possivel criar arquivo temporario\n");
            fclose(f_svg_qry);
            fclose(f_txt_qry);
            destroiFormas(fs);
            destroiPoligonos();
            destroiArgs(args);
            return 1;
        }

        /* Passo A: processa queries. */
        processaQry(caminho_qry, fs, f_ann, f_txt_qry);

        /* Passo B: grava o estado FINAL do banco no SVG. */
        svgInicia(f_svg_qry, fs);
        svgEscreveFormas(f_svg_qry, fs);

        /* Passo C: copia as anotações (sel, x) para depois das formas. */
        copiaArquivo(f_ann, f_svg_qry);
        fclose(f_ann);

        svgFinaliza(f_svg_qry);
        fclose(f_svg_qry);
        fclose(f_txt_qry);
    }

    /* ── 6. Libera recursos ── */
    destroiFormas(fs);
    destroiPoligonos();
    destroiArgs(args);

    return 0;
}