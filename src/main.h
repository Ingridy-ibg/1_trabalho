#ifndef MAIN_H
#define MAIN_H

/**
 * @file main.h
 * @brief Ponto de entrada e parsing de argumentos do programa TED.
 *
 * O main coordena os três módulos de processamento:
 *   processadorGEO → lê o .geo e popula o banco de formas
 *   svg            → gera os arquivos .svg
 *   processadorQRY → lê o .qry e gera o .svg e .txt de consultas
 *
 * Arquivos produzidos conforme a tabela do enunciado:
 *   Só -f           →  BSD/arq.svg
 *   -f e -q         →  BSD/arq.svg
 *                       BSD/arq-qry.svg
 *                       BSD/arq-qry.txt
 */

#include <stdbool.h>

/** Tamanho máximo para caminhos de arquivo/diretório. */
#define MAX_PATH 16384

/**
 * @struct Args
 * @brief Argumentos de linha de comando já parseados.
 *
 * Campos opcionais ficam com string vazia ("") quando não informados.
 */
typedef struct {
    char base_entrada[MAX_PATH]; /**< -e  Diretório-base de entrada. Padrão: "."  */
    char arq_geo[MAX_PATH];      /**< -f  Nome do arquivo .geo (obrigatório)       */
    char base_saida[MAX_PATH];   /**< -o  Diretório-base de saída  (obrigatório)   */
    char arq_qry[MAX_PATH];      /**< -q  Nome do arquivo .qry (opcional)          */
} Args;

/**
 * @brief Parseia os argumentos de linha de comando e preenche Args.
 *
 * Parâmetros aceitos (qualquer ordem):
 *   -e path    diretório-base de entrada   (opcional; padrão ".")
 *   -f arq.geo arquivo de formas           (obrigatório)
 *   -o path    diretório-base de saída     (obrigatório)
 *   -q arq.qry arquivo de consultas        (opcional)
 *
 * @param argc Número de argumentos de main().
 * @param argv Vetor de argumentos de main().
 * @param args Ponteiro para struct a ser preenchida (não pode ser NULL).
 * @return true  se todos os campos obrigatórios foram encontrados.
 *         false caso contrário (imprime uso no stderr).
 */
bool parseArgs(int argc, char *argv[], Args *args);

#endif /* MAIN_H */