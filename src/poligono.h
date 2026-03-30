#ifndef POLIGONO_H
#define POLIGONO_H

#include <stdbool.h>

/**
 * @file poligono.h
 * @brief Módulo de gerenciamento de polígonos.
 *
 * O programa mantém até MAX_POLIGONOS polígonos simultaneamente,
 * identificados por um inteiro p no intervalo [1, MAX_POLIGONOS].
 * Cada polígono armazena uma sequência de coordenadas (x, y) em
 * uma fila estática circular. As coordenadas são inseridas pelo
 * comando inp e removidas pelo comando rmp, seguindo ordem FIFO.
 * O comando pol consome as coordenadas para gerar as bordas e o
 * preenchimento do polígono como linhas no banco de dados.
 */

/**
 * @brief Número máximo de polígonos simultâneos suportados.
 * Identificadores válidos: [1, MAX_POLIGONOS].
 */
#define MAX_POLIGONOS 10

/**
 * @brief Capacidade máxima de pontos por polígono.
 * Define o tamanho da fila estática circular interna.
 */
#define MAX_PONTOS_POLIGONO 100

/**
 * @brief Inicializa o módulo de polígonos.
 * Deve ser chamada uma única vez antes de qualquer outra função
 * deste módulo. Cria as filas internas de todos os polígonos.
 */
void inicializaPoligonos(void);

/**
 * @brief Libera toda a memória alocada pelo módulo.
 * Deve ser chamada ao final do programa.
 */
void destroiPoligonos(void);

/**
 * @brief Insere uma coordenada no final do polígono p (comando inp).
 * @param p Identificador do polígono [1, MAX_POLIGONOS].
 * @param x Coordenada x do ponto a inserir.
 * @param y Coordenada y do ponto a inserir.
 * @return true se a inserção foi bem-sucedida.
 * false se p for inválido ou o polígono estiver cheio.
 */
bool inserePoligono(int p, double x, double y);

/**
 * @brief Remove a coordenada mais antiga do polígono p (comando rmp).
 * @param p Identificador do polígono [1, MAX_POLIGONOS].
 * @param x Parâmetro de saída: preenchido com a coordenada x removida.
 * @param y Parâmetro de saída: preenchido com a coordenada y removida.
 * @return true se a remoção foi bem-sucedida.
 * false se p for inválido ou o polígono estiver vazio.
 */
bool removePoligono(int p, double *x, double *y);

/**
 * @brief Consulta a coordenada mais antiga do polígono p sem removê-la.
 * @param p Identificador do polígono [1, MAX_POLIGONOS].
 * @param x Parâmetro de saída: preenchido com a coordenada x consultada.
 * @param y Parâmetro de saída: preenchido com a coordenada y consultada.
 * @return true se a consulta foi bem-sucedida.
 * false se p for inválido ou o polígono estiver vazio.
 */
bool peekPoligono(int p, double *x, double *y);

/**
 * @brief Remove todas as coordenadas do polígono p (comando clp).
 * @param p Identificador do polígono [1, MAX_POLIGONOS].
 * @return true se a limpeza foi bem-sucedida, false se p for inválido.
 */
bool limpaPoligono(int p);

/**
 * @brief Retorna a quantidade de coordenadas no polígono p.
 * @param p Identificador do polígono [1, MAX_POLIGONOS].
 * @return Número de pontos na fila. Retorna -1 se p for inválido.
 */
int lengthPoligono(int p);

/**
 * @brief Verifica se o polígono p está vazio.
 * @param p Identificador do polígono [1, MAX_POLIGONOS].
 * @return true se não houver pontos, false caso contrário ou se p for inválido.
 */
bool isEmptyPoligono(int p);

/**
 * @brief Verifica se o polígono p está cheio.
 * @param p Identificador do polígono [1, MAX_POLIGONOS].
 * @return true se atingiu MAX_PONTOS_POLIGONO, false caso contrário ou se p for inválido.
 */
bool isFullPoligono(int p);

/**
 * @brief Retorna todos os pontos do polígono p em ordem FIFO sem removê-los.
 * Útil para o comando pol, que precisa iterar sobre os pontos para
 * gerar bordas e preenchimento sem consumir a fila.
 * @param p  Identificador do polígono [1, MAX_POLIGONOS].
 * @param xs Array de saída preenchido com as coordenadas x na ordem FIFO.
 * Deve ter capacidade mínima de MAX_PONTOS_POLIGONO elementos.
 * @param ys Array de saída preenchido com as coordenadas y na ordem FIFO.
 * Deve ter capacidade mínima de MAX_PONTOS_POLIGONO elementos.
 * @param n  Parâmetro de saída: preenchido com o número de pontos copiados.
 * @return true se a operação foi bem-sucedida, false se p for inválido.
 */
bool getPontosPoligono(int p, double xs[], double ys[], int *n);

#endif // POLIGONO_H