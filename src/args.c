/**
 * @file args.c
 * @brief Implementação da API de parsing de argumentos da linha de comando.
 *
 * Este módulo é separado de main.c para permitir que os testes unitários
 * (t_main.c) possam ser linkados sem incluir o int main() do programa.
 *
 * Veja main.h para a documentação dos contratos.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "main.h"

/* ─────────────────────────────────────────────
   Estrutura interna (proibida no .h)
   ───────────────────────────────────────────── */

typedef struct ArgsStruct {
    char base_entrada[MAX_PATH];  /**< -e  Diretório-base de entrada. Padrão: "."  */
    char arq_geo[MAX_PATH];       /**< -f  Nome do arquivo .geo (obrigatório)       */
    char base_saida[MAX_PATH];    /**< -o  Diretório-base de saída  (obrigatório)   */
    char arq_qry[MAX_PATH];       /**< -q  Nome do arquivo .qry (opcional)          */
} ArgsStruct;

/* ─────────────────────────────────────────────
   parseArgs
   ───────────────────────────────────────────── */

Args parseArgs(int argc, char *argv[]) {
    ArgsStruct *args = (ArgsStruct *)malloc(sizeof(ArgsStruct));
    if (args == NULL) return NULL;

    /* Valores padrão */
    strncpy(args->base_entrada, ".", MAX_PATH - 1);
    args->base_entrada[MAX_PATH - 1] = '\0';
    args->arq_geo[0]    = '\0';
    args->base_saida[0] = '\0';
    args->arq_qry[0]    = '\0';

    for (int i = 1; i < argc; i++) {
        if      (strcmp(argv[i], "-e") == 0 && i + 1 < argc) {
            strncpy(args->base_entrada, argv[++i], MAX_PATH - 1);
            args->base_entrada[MAX_PATH - 1] = '\0';
        } else if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) {
            strncpy(args->arq_geo, argv[++i], MAX_PATH - 1);
            args->arq_geo[MAX_PATH - 1] = '\0';
        } else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
            strncpy(args->base_saida, argv[++i], MAX_PATH - 1);
            args->base_saida[MAX_PATH - 1] = '\0';
        } else if (strcmp(argv[i], "-q") == 0 && i + 1 < argc) {
            strncpy(args->arq_qry, argv[++i], MAX_PATH - 1);
            args->arq_qry[MAX_PATH - 1] = '\0';
        }
    }

    if (args->arq_geo[0] == '\0' || args->base_saida[0] == '\0') {
        fprintf(stderr,
            "Uso: ted -f arq.geo -o dir_saida [-e dir_entrada] [-q arq.qry]\n");
        free(args);
        return NULL;
    }
    return (Args)args;
}

/* ─────────────────────────────────────────────
   destroiArgs / getters
   ───────────────────────────────────────────── */

void destroiArgs(Args args) {
    if (args != NULL) free(args);
}

const char *getBaseEntrada(Args args) {
    return (args != NULL) ? ((ArgsStruct *)args)->base_entrada : NULL;
}

const char *getArqGeo(Args args) {
    return (args != NULL) ? ((ArgsStruct *)args)->arq_geo : NULL;
}

const char *getBaseSaida(Args args) {
    return (args != NULL) ? ((ArgsStruct *)args)->base_saida : NULL;
}

const char *getArqQry(Args args) {
    return (args != NULL) ? ((ArgsStruct *)args)->arq_qry : NULL;
}