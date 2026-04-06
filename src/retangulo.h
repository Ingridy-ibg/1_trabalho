#ifndef RETANGULO_H
#define RETANGULO_H

#include <stdbool.h>

/**
 * @typedef Retangulo
 * @brief Ponteiro opaco para a estrutura interna do retângulo.
 */
typedef void *Retangulo;

/**
 * @brief Cria um novo retângulo com os parâmetros fornecidos.
 * @param id   Identificador inteiro único do retângulo.
 * @param x    Coordenada x do canto inferior esquerdo (âncora).
 * @param y    Coordenada y do canto inferior esquerdo (âncora).
 * @param w    Largura do retângulo (> 0).
 * @param h    Altura do retângulo (> 0).
 * @param corb String com a cor da borda (padrão SVG).
 * @param corp String com a cor do preenchimento (padrão SVG).
 * @return Ponteiro (Retangulo) para a estrutura alocada.
 * Retorna NULL em caso de falha de alocação ou parâmetros inválidos.
 */
Retangulo criaRetangulo(int id, double x, double y, double w, double h,
                        const char *corb, const char *corp);

/**
 * @brief Libera a memória alocada pelo retângulo.
 * @param r Ponteiro para o retângulo a ser destruído.
 */
void destroiRetangulo(Retangulo r);

/**
 * @brief Retorna o identificador do retângulo.
 * @param r Ponteiro para o retângulo.
 * @return Identificador inteiro. Retorna -1 se r for NULL.
 */
int getIdRetangulo(Retangulo r);

/**
 * @brief Retorna a coordenada x da âncora (canto inferior esquerdo).
 * @param r Ponteiro para o retângulo.
 * @return Valor de x. Retorna 0.0 se r for NULL.
 */
double getXRetangulo(Retangulo r);

/**
 * @brief Retorna a coordenada y da âncora (canto inferior esquerdo).
 * @param r Ponteiro para o retângulo.
 * @return Valor de y. Retorna 0.0 se r for NULL.
 */
double getYRetangulo(Retangulo r);

/**
 * @brief Retorna a largura do retângulo.
 * @param r Ponteiro para o retângulo.
 * @return Largura (w). Retorna 0.0 se r for NULL.
 */
double getWRetangulo(Retangulo r);

/**
 * @brief Retorna a altura do retângulo.
 * @param r Ponteiro para o retângulo.
 * @return Altura (h). Retorna 0.0 se r for NULL.
 */
double getHRetangulo(Retangulo r);

/**
 * @brief Retorna a cor da borda do retângulo.
 * @param r Ponteiro para o retângulo.
 * @return String com a cor da borda. Retorna NULL se r for NULL.
 */
const char *getCorbRetangulo(Retangulo r);

/**
 * @brief Retorna a cor do preenchimento do retângulo.
 * @param r Ponteiro para o retângulo.
 * @return String com a cor do preenchimento. Retorna NULL se r for NULL.
 */
const char *getCorpRetangulo(Retangulo r);

/**
 * @brief Translada o retângulo somando dx e dy à sua âncora.
 * @param r  Ponteiro para o retângulo.
 * @param dx Deslocamento em x.
 * @param dy Deslocamento em y.
 */
void transladaRetangulo(Retangulo r, double dx, double dy);

/**
 * @brief Altera as cores do retângulo.
 * @param r    Ponteiro para o retângulo.
 * @param corb Nova cor da borda (padrão SVG).
 * @param corp Nova cor do preenchimento (padrão SVG).
 */
void setCoresRetangulo(Retangulo r, const char *corb, const char *corp);

/**
 * @brief Verifica se o ponto (px, py) está dentro do retângulo.
 * Considera os limites inclusive (ponto na borda conta como dentro).
 * @param r  Ponteiro para o retângulo.
 * @param px Coordenada x do ponto.
 * @param py Coordenada y do ponto.
 * @return true se o ponto está dentro, false caso contrário ou se r for NULL.
 */
bool contemPontoRetangulo(Retangulo r, double px, double py);

/**
 * @brief Verifica se o retângulo está inteiramente contido
 * na região definida por (rx, ry, rw, rh).
 * @param r  Ponteiro para o retângulo.
 * @param rx Coordenada x do canto inferior esquerdo da região.
 * @param ry Coordenada y do canto inferior esquerdo da região.
 * @param rw Largura da região.
 * @param rh Altura da região.
 * @return true se o retângulo estiver inteiramente dentro da região.
 * false caso contrário ou se r for NULL.
 */
bool dentroRegiaoRetangulo(Retangulo r, double rx, double ry,
                           double rw, double rh);

#endif // RETANGULO_H