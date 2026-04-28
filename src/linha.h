#ifndef LINHA_H
#define LINHA_H

#include <stdbool.h>

/**
 * @typedef Linha
 * @brief Ponteiro opaco para a estrutura interna da linha.
 */
typedef void *Linha;

/**
 * @brief Cria uma nova linha com os parâmetros fornecidos.
 *
 * A espessura visual (stroke-width) é inicializada com o valor default
 * (1.0). Use setStrokeWidthLinha para ajustá-la (ex.: linhas de
 * preenchimento de polígono usam d/4 para parecer hachura).
 *
 * @param id   Identificador inteiro único da linha.
 * @param x1   Coordenada x do ponto inicial da linha.
 * @param y1   Coordenada y do ponto inicial da linha.
 * @param x2   Coordenada x do ponto final da linha.
 * @param y2   Coordenada y do ponto final da linha.
 * @param cor  String com a cor da linha (padrão SVG).
 * @return Ponteiro (Linha) para a estrutura alocada.
 * Retorna NULL em caso de falha de alocação ou parâmetros inválidos.
 */
Linha criaLinha(int id, double x1, double y1, double x2, double y2,
                const char *cor);

/**
 * @brief Libera a memória alocada pela linha.
 * @param l Ponteiro para a linha a ser destruída.
 */
void destroiLinha(Linha l);

/**
 * @brief Retorna o identificador da linha.
 * @param l Ponteiro para a linha.
 * @return Identificador inteiro. Retorna -1 se l for NULL.
 */
int getIdLinha(Linha l);

/**
 * @brief Retorna a coordenada x do ponto inicial da linha.
 * @param l Ponteiro para a linha.
 * @return Valor de x1. Retorna 0.0 se l for NULL.
 */
double getX1Linha(Linha l);

/**
 * @brief Retorna a coordenada y do ponto inicial da linha.
 * @param l Ponteiro para a linha.
 * @return Valor de y1. Retorna 0.0 se l for NULL.
 */
double getY1Linha(Linha l);

/**
 * @brief Retorna a coordenada x do ponto final da linha.
 * @param l Ponteiro para a linha.
 * @return Valor de x2. Retorna 0.0 se l for NULL.
 */
double getX2Linha(Linha l);

/**
 * @brief Retorna a coordenada y do ponto final da linha.
 * @param l Ponteiro para a linha.
 * @return Valor de y2. Retorna 0.0 se l for NULL.
 */
double getY2Linha(Linha l);

/**
 * @brief Retorna a cor da linha.
 * @param l Ponteiro para a linha.
 * @return String com a cor da linha. Retorna NULL se l for NULL.
 */
const char *getCorLinha(Linha l);

/**
 * @brief Retorna a espessura visual (stroke-width) da linha.
 * @param l Ponteiro para a linha.
 * @return Espessura em unidades do plano. Retorna 1.0 (default) se l for NULL.
 */
double getStrokeWidthLinha(Linha l);

/**
 * @brief Define a espessura visual (stroke-width) da linha.
 *
 * Usado pelo comando pol para diferenciar bordas (mais grossas) e
 * linhas de preenchimento (mais finas, proporcionais a d).
 *
 * @param l  Ponteiro para a linha.
 * @param sw Nova espessura em unidades do plano. Deve ser > 0.
 */
void setStrokeWidthLinha(Linha l, double sw);

/**
 * @brief Translada a linha somando dx e dy a ambos os pontos.
 * @param l  Ponteiro para a linha.
 * @param dx Deslocamento em x.
 * @param dy Deslocamento em y.
 */
void transladaLinha(Linha l, double dx, double dy);

/**
 * @brief Altera a cor da linha.
 * @param l   Ponteiro para a linha.
 * @param cor Nova cor da linha (padrão SVG).
 */
void setCorLinha(Linha l, const char *cor);

/**
 * @brief Verifica se o ponto (px, py) está na linha.
 * Considera pontos exatamente na linha (com tolerância numérica).
 * @param l  Ponteiro para a linha.
 * @param px Coordenada x do ponto.
 * @param py Coordenada y do ponto.
 * @return true se o ponto está na linha, false caso contrário ou se l for NULL.
 */
bool contemPontoLinha(Linha l, double px, double py);

/**
 * @brief Verifica se a linha está inteiramente contida
 * na região definida por (rx, ry, rw, rh).
 * @param l  Ponteiro para a linha.
 * @param rx Coordenada x do canto inferior esquerdo da região.
 * @param ry Coordenada y do canto inferior esquerdo da região.
 * @param rw Largura da região.
 * @param rh Altura da região.
 * @return true se a linha estiver inteiramente dentro da região.
 * false caso contrário ou se l for NULL.
 */
bool dentroRegiaoLinha(Linha l, double rx, double ry,
                       double rw, double rh);

#endif // LINHA_H