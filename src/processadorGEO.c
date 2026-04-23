/**
 * @file processadorGEO.c
 * @brief Implementação do processador de arquivos .geo.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "processadorGEO.h"
#include "formas.h"
#include "texto.h"

/* ─────────────────────────────────────────────
   Constantes internas
   ───────────────────────────────────────────── */

#define MAX_LINHA 4096

/* ─────────────────────────────────────────────
   Helpers internos de parsing
   ───────────────────────────────────────────── */

/**
 * Avança ponteiro p sobre espaços/tabs.
 * Retorna p apontando para o primeiro não-espaço (ou '\0').
 */
static char *pula_espacos(char *p) {
    while (*p == ' ' || *p == '\t') p++;
    return p;
}

/**
 * Avança ponteiro p sobre caracteres não-espaço.
 * Retorna p apontando para o próximo espaço ou '\0'.
 */
static char *pula_token(char *p) {
    while (*p != ' ' && *p != '\t' && *p != '\0') p++;
    return p;
}

/**
 * Extrai o texto restante após N tokens já consumidos na linha original.
 * n_pular: número de tokens a pular (inclui o próprio comando).
 * Retorna ponteiro para o início do texto, ou NULL se a linha acabar antes.
 */
static char *extrai_resto(char *linha, int n_pular) {
    char *p = linha;
    for (int i = 0; i < n_pular; i++) {
        p = pula_espacos(p);
        if (*p == '\0') return NULL;
        p = pula_token(p);
    }
    p = pula_espacos(p);
    return (*p != '\0') ? p : NULL;
}

/* ─────────────────────────────────────────────
   Handlers por comando
   ───────────────────────────────────────────── */

/* c i x y r corb corp */
static void cmd_circulo(char *linha, Formas fs) {
    int    id;
    double x, y, r;
    char   corb[64], corp[64];

    if (sscanf(linha, "%*s %d %lf %lf %lf %63s %63s",
               &id, &x, &y, &r, corb, corp) == 6)
        insereCirculo(fs, id, x, y, r, corb, corp);
}

/* r i x y w h corb corp */
static void cmd_retangulo(char *linha, Formas fs) {
    int    id;
    double x, y, w, h;
    char   corb[64], corp[64];

    if (sscanf(linha, "%*s %d %lf %lf %lf %lf %63s %63s",
               &id, &x, &y, &w, &h, corb, corp) == 7)
        insereRetangulo(fs, id, x, y, w, h, corb, corp);
}

/* l i x1 y1 x2 y2 cor */
static void cmd_linha(char *linha, Formas fs) {
    int    id;
    double x1, y1, x2, y2;
    char   cor[64];

    if (sscanf(linha, "%*s %d %lf %lf %lf %lf %63s",
               &id, &x1, &y1, &x2, &y2, cor) == 6)
        insereLinha(fs, id, x1, y1, x2, y2, cor);
}

/*
 * t i x y corb corp a txto
 *
 * O txto é o último campo e pode conter espaços — vai até o fim da linha.
 * Estrutura da linha (tokens 1-indexados):
 *   1=t  2=i  3=x  4=y  5=corb  6=corp  7=a  8..=txto
 * Pulamos 7 tokens e o restante é o texto.
 */
static void cmd_texto(char *linha, Formas fs) {
    int    id;
    double x, y;
    char   corb[64], corp[64], ancora_s[4];

    if (sscanf(linha, "%*s %d %lf %lf %63s %63s %3s",
               &id, &x, &y, corb, corp, ancora_s) != 6)
        return;

    /* 7 tokens a pular: t i x y corb corp a */
    char *txto = extrai_resto(linha, 7);
    if (txto == NULL) return;

    /* Remove newline residual */
    txto[strcspn(txto, "\r\n")] = '\0';
    if (txto[0] == '\0') return;

    insereTexto(fs, id, x, y, corb, corp, ancora_s[0], txto);
}

/* ts fFamily fWeight fSize */
static void cmd_ts(char *linha) {
    char   fam[32], weight[8];
    double fsize;

    if (sscanf(linha, "%*s %31s %7s %lf", fam, weight, &fsize) == 3)
        setEstiloTexto(fam, weight, fsize);
}

/* ─────────────────────────────────────────────
   processaGeo
   ───────────────────────────────────────────── */

bool processaGeo(const char *caminho, Formas fs) {
    if (caminho == NULL || fs == NULL) return false;

    FILE *f = fopen(caminho, "r");
    if (f == NULL) {
        fprintf(stderr, "processaGeo: nao foi possivel abrir '%s'\n", caminho);
        return false;
    }

    char linha[MAX_LINHA];
    while (fgets(linha, sizeof(linha), f) != NULL) {
        /* Remove newline */
        linha[strcspn(linha, "\r\n")] = '\0';

        /* Ignora vazia e comentário */
        char *p = pula_espacos(linha);
        if (*p == '\0' || *p == '#') continue;

        /* Lê o comando (primeiro token) */
        char cmd[16];
        if (sscanf(p, "%15s", cmd) != 1) continue;

        if      (strcmp(cmd, "c")  == 0) cmd_circulo(p, fs);
        else if (strcmp(cmd, "r")  == 0) cmd_retangulo(p, fs);
        else if (strcmp(cmd, "l")  == 0) cmd_linha(p, fs);
        else if (strcmp(cmd, "t")  == 0) cmd_texto(p, fs);
        else if (strcmp(cmd, "ts") == 0) cmd_ts(p);
        /* Outros comandos são ignorados */
    }

    fclose(f);
    return true;
}