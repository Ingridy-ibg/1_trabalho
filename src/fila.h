#ifndef FILA_H
#define FILA_H

#include <stdbool.h>

/**
 * @typedef Item
 * @brief Ponteiro genérico para a informação armazenada em um nó da fila.
 */
typedef void *Item;

/**
 * @typedef Fila
 * @brief Ponteiro opaco que representa a estrutura de controle da fila.
 * A implementação interna é estática e circular.
 */
typedef void *Fila;

/**
 * @brief Cria uma nova fila vazia com capacidade máxima definida.
 * @param capacidade Número máximo de elementos que a fila pode armazenar.
 * @return Retorna um ponteiro (Fila) para a nova estrutura alocada.
 * Retorna NULL caso não haja memória disponível ou capacidade inválida.
 */
Fila criaFila(int capacidade);

/**
 * @brief Insere um novo item no final da fila (enqueue).
 * @param F Ponteiro para a fila onde o item será inserido.
 * @param info Ponteiro genérico (Item) contendo os dados a serem armazenados.
 * @return true se a inserção foi bem-sucedida, false se a fila estiver cheia ou for nula.
 */
bool enqueue(Fila F, Item info);

/**
 * @brief Remove e retorna o item mais antigo da fila (dequeue).
 * @param F Ponteiro para a fila.
 * @return Retorna o Item removido da frente da fila.
 * Retorna NULL se a fila estiver vazia ou for nula.
 * O Item em si NÃO é liberado — apenas o slot interno é liberado.
 */
Item dequeue(Fila F);

/**
 * @brief Consulta o item mais antigo da fila sem removê-lo.
 * @param F Ponteiro para a fila.
 * @return Retorna o Item na frente da fila, sem removê-lo.
 * Retorna NULL se a fila estiver vazia ou for nula.
 */
Item peek(Fila F);

/**
 * @brief Retorna a quantidade de elementos atualmente na fila.
 * @param F Ponteiro para a fila.
 * @return Inteiro representando o número de elementos. Retorna 0 se vazia ou nula.
 */
int lengthFila(Fila F);

/**
 * @brief Retorna a capacidade máxima da fila.
 * @param F Ponteiro para a fila.
 * @return Inteiro representando a capacidade definida na criação. Retorna 0 se nula.
 */
int capacidadeFila(Fila F);

/**
 * @brief Verifica se a fila está vazia.
 * @param F Ponteiro para a fila.
 * @return true se não houver elementos, false caso contrário.
 */
bool isEmptyFila(Fila F);

/**
 * @brief Verifica se a fila está cheia.
 * @param F Ponteiro para a fila.
 * @return true se o número de elementos atingiu a capacidade máxima, false caso contrário.
 */
bool isFullFila(Fila F);

/**
 * @brief Remove todos os elementos da fila sem destruí-la, tornando-a vazia.
 * @param F Ponteiro para a fila.
 * @param destroiItem Ponteiro para uma função que sabe como liberar a memória de um Item.
 * Se NULL, os itens são descartados sem liberação de memória.
 */
void clearFila(Fila F, void (*destroiItem)(Item));

/**
 * @brief Libera toda a memória ocupada pela fila e, opcionalmente, pelos itens armazenados.
 * @param F Ponteiro para a fila a ser destruída.
 * @param destroiItem Ponteiro para uma função que sabe como liberar a memória de um Item.
 * Se NULL, a fila é destruída mas os itens são preservados.
 */
void destroiFila(Fila F, void (*destroiItem)(Item));

#endif // FILA_H