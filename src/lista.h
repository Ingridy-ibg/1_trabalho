#ifndef LISTA_H
#define LISTA_H

#include <stdbool.h>


/**
 * @typedef Item
 * @brief Ponteiro genérico para a informação armazenada em um nó da lista.
 */
typedef void *Item;

/**
 * @typedef Posic
 * @brief Ponteiro opaco que representa a posição (o nó) de um item na lista.
 */
typedef void *Posic;

/**
 * @typedef Lista
 * @brief Ponteiro opaco que representa a cabeça/controle da lista.
 */
typedef void *Lista;

/**
 * @brief Cria uma nova lista vazia.
 * @return Retorna um ponteiro (Lista) para a nova estrutura alocada.
 * Retorna NULL caso não haja memória disponível.
 */
Lista criaLista();

/**
 * @brief Insere um novo item no final da lista.
 * @param L Ponteiro para a lista onde o item será inserido.
 * @param info Ponteiro genérico (Item) contendo os dados a serem armazenados.
 * @return Retorna a posição do item recém-inserido, ou NULL em caso de falha.
 */
Posic insereFinalLista(Lista L, Item info);

/**
 * @brief Insere um novo item no começo da lista.
 * @param L Ponteiro para a lista onde o item será inserido.
 * @param info Ponteiro genérico (Item) contendo os dados a serem armazenados.
 * @return Retorna a posição do item recém-inserido, ou NULL em caso de falha.
 */
Posic insereInicioLista(Lista L, Item info);

/**
 * @brief Insere um novo item imediatamente ANTES da posição indicada.
 * @param L Ponteiro para a lista.
 * @param p Posição de referência.
 * @param info Ponteiro genérico (Item) para o dado.
 * @return Retorna a posição (Posic) do novo nó inserido.
 */
Posic insertBefore(Lista L, Posic p, Item info);

/**
 * @brief Insere um novo item imediatamente DEPOIS da posição indicada.
 * @param L Ponteiro para a lista.
 * @param p Posição de referência.
 * @param info Ponteiro genérico (Item) para o dado.
 * @return Retorna a posição (Posic) do novo nó inserido.
 */
Posic insertAfter(Lista L, Posic p, Item info);

/**
 * @brief Remove o nó na posição indicada da lista.
 * @param L Ponteiro para a lista.
 * @param p Posição (Posic) do nó a ser removido.
 * @return Retorna o Item que estava armazenado na posição p.
 * O nó interno é liberado (free), mas o Item em si NÃO é liberado.
 */
Item removeLista(Lista L, Posic p);

/**
 * @brief Retorna a quantidade de elementos armazenados na lista.
 * @param L Ponteiro para a lista.
 * @return Inteiro representando o número de nós. Retorna 0 se for vazia ou nula.
 */
int lengthLista(Lista L);

/**
 * @brief Obtém o dado armazenado em uma determinada posição.
 * @param L Ponteiro para a lista.
 * @param p Posição (Posic) do nó.
 * @return Retorna o Item armazenado no nó correspondente a 'p'.
 */
Item getLista(Lista L, Posic p);

/**
 * @brief Retorna a posição do primeiro elemento da lista.
 * @param L Ponteiro para a lista.
 * @return Retorna a posição (Posic) do primeiro nó. Retorna NULL se a lista estiver vazia.
 */
Posic getFirst(Lista L);

/**
 * @brief Retorna a posição do elemento subsequente a 'p'.
 * @param L Ponteiro para a lista.
 * @param p Posição (Posic) atual.
 * @return Retorna a posição (Posic) do próximo nó. Retorna NULL se 'p' for o último.
 */
Posic getNext(Lista L, Posic p);

/**
 * @brief Retorna a posição do último elemento da lista.
 * @param L Ponteiro para a lista.
 * @return Retorna a posição (Posic) do último nó. Retorna NULL se a lista estiver vazia.
 */
Posic getLast(Lista L);

/**
 * @brief Retorna a posição do elemento anterior a 'p'.
 * @param L Ponteiro para a lista.
 * @param p Posição (Posic) atual.
 * @return Retorna a posição (Posic) do nó anterior. Retorna NULL se 'p' for o primeiro.
 */
Posic getPrevious(Lista L, Posic p);

/**
 * @brief Libera toda a memória da lista e, opcionalmente, dos itens armazenados.
 * @param L Ponteiro para a lista a ser destruída.
 * @param destroiItem Ponteiro para uma função que sabe como liberar a memória de um Item. 
 * Se você passar NULL, a lista libera apenas os nós e preserva os itens.
 */
void destroiLista(Lista L, void (*destroiItem)(Item));

/**
 * @brief Verifica se a lista está vazia.
 * @param L Ponteiro para a lista.
 * @return true se a lista estiver vazia, false caso contrário.
 */
bool isEmptyLista(Lista L);

/**
 * @brief Busca um item na lista.
 * @param L Ponteiro para a lista.
 * @param info Ponteiro genérico (Item) que está sendo buscado.
 * @return Retorna a posição do nó encontrado, ou NULL se não encontrar.
 */
Posic buscaLista(Lista L, Item info);

#endif // LISTA_H
