#ifndef MAIN_H
#define MAIN_H

#include <stdbool.h>

/** Tamanho máximo para caminhos de arquivo/diretório. */
#define MAX_PATH 16384

/**
 * @typedef Args
 * @brief Ponteiro opaco para a estrutura de argumentos parseados.
 *
 * A definição interna está em main.c. Este módulo só expõe a API
 * de criação/destruição e os getters dos campos.
 */
typedef void *Args;

/**
 * @brief Parseia os argumentos da linha de comando.
 *
 * Parâmetros aceitos (qualquer ordem):
 *   -e path    diretório-base de entrada   (opcional; padrão ".")
 *   -f arq.geo arquivo de formas           (obrigatório)
 *   -o path    diretório-base de saída     (obrigatório)
 *   -q arq.qry arquivo de consultas        (opcional)
 *
 * @param argc Número de argumentos de main().
 * @param argv Vetor de argumentos de main().
 * @return Args alocado e preenchido em caso de sucesso.
 *         NULL se algum parâmetro obrigatório estiver ausente
 *         (mensagem de uso é impressa em stderr) ou se a alocação falhar.
 */
Args parseArgs(int argc, char *argv[]);

/**
 * @brief Libera a memória de um Args criado por parseArgs().
 * @param args Ponteiro retornado por parseArgs (pode ser NULL).
 */
void destroiArgs(Args args);

/**
 * @brief Retorna o diretório-base de entrada (-e).
 * @return Sempre uma string válida; padrão "." se -e não foi informado.
 *         Retorna NULL apenas se args for NULL.
 */
const char *getBaseEntrada(Args args);

/**
 * @brief Retorna o nome do arquivo .geo (-f).
 * @return String não-vazia (campo obrigatório). NULL se args for NULL.
 */
const char *getArqGeo(Args args);

/**
 * @brief Retorna o diretório-base de saída (-o).
 * @return String não-vazia (campo obrigatório). NULL se args for NULL.
 */
const char *getBaseSaida(Args args);

/**
 * @brief Retorna o nome do arquivo .qry (-q).
 * @return String vazia ("") se -q não foi informado.
 *         NULL apenas se args for NULL.
 */
const char *getArqQry(Args args);

#endif /* MAIN_H */