#ifndef CIRCULO_H
#define CIRCULO_H

#include <stdbool.h>

/**
 * @typedef Circulo
 * @brief Ponteiro opaco para a estrutura interna do círculo.
 */
typedef void *Circulo;

/**
 * @brief Cria um novo círculo com os parâmetros fornecidos.
 * @param id   Identificador inteiro único do círculo.
 * @param x    Coordenada x do centro do círculo.
 * @param y    Coordenada y do centro do círculo.
 * @param r    Raio do círculo (> 0).
 * @param corb String com a cor da borda (padrão SVG).
 * @param corp String com a cor do preenchimento (padrão SVG).
 * @return Ponteiro (Circulo) para a estrutura alocada.
 * Retorna NULL em caso de falha de alocação ou parâmetros inválidos.
 */
Circulo criaCirculo(int id, double x, double y, double r,
                    const char *corb, const char *corp);

/**
 * @brief Libera a memória alocada pelo círculo.
 * @param c Ponteiro para o círculo a ser destruído.
 */
void destroiCirculo(Circulo c);

/**
 * @brief Retorna o identificador do círculo.
 * @param c Ponteiro para o círculo.
 * @return Identificador inteiro. Retorna -1 se c for NULL.
 */
int getIdCirculo(Circulo c);

/**
 * @brief Retorna a coordenada x do centro do círculo.
 * @param c Ponteiro para o círculo.
 * @return Valor de x. Retorna 0.0 se c for NULL.
 */
double getXCirculo(Circulo c);

/**
 * @brief Retorna a coordenada y do centro do círculo.
 * @param c Ponteiro para o círculo.
 * @return Valor de y. Retorna 0.0 se c for NULL.
 */
double getYCirculo(Circulo c);

/**
 * @brief Retorna o raio do círculo.
 * @param c Ponteiro para o círculo.
 * @return Raio (r). Retorna 0.0 se c for NULL.
 */
double getRCirculo(Circulo c);

/**
 * @brief Retorna a cor da borda do círculo.
 * @param c Ponteiro para o círculo.
 * @return String com a cor da borda. Retorna NULL se c for NULL.
 */
const char *getCorbCirculo(Circulo c);

/**
 * @brief Retorna a cor do preenchimento do círculo.
 * @param c Ponteiro para o círculo.
 * @return String com a cor do preenchimento. Retorna NULL se c for NULL.
 */
const char *getCorpCirculo(Circulo c);

/**
 * @brief Translada o círculo somando dx e dy ao seu centro.
 * @param c  Ponteiro para o círculo.
 * @param dx Deslocamento em x.
 * @param dy Deslocamento em y.
 */
void transladaCirculo(Circulo c, double dx, double dy);

/**
 * @brief Altera as cores do círculo.
 * @param c    Ponteiro para o círculo.
 * @param corb Nova cor da borda (padrão SVG).
 * @param corp Nova cor do preenchimento (padrão SVG).
 */
void setCoresCirculo(Circulo c, const char *corb, const char *corp);

/**
 * @brief Verifica se o ponto (px, py) está dentro do círculo.
 * Considera os limites inclusive (ponto na borda conta como dentro).
 * @param c  Ponteiro para o círculo.
 * @param px Coordenada x do ponto.
 * @param py Coordenada y do ponto.
 * @return true se o ponto está dentro, false caso contrário ou se c for NULL.
 */
bool contemPontoCirculo(Circulo c, double px, double py);

/**
 * @brief Verifica se o círculo está inteiramente contido
 * na região definida por (rx, ry, rw, rh).
 * @param c  Ponteiro para o círculo.
 * @param rx Coordenada x do canto inferior esquerdo da região.
 * @param ry Coordenada y do canto inferior esquerdo da região.
 * @param rw Largura da região.
 * @param rh Altura da região.
 * @return true se o círculo estiver inteiramente dentro da região.
 * false caso contrário ou se c for NULL.
 */
bool dentroRegiaoCirculo(Circulo c, double rx, double ry,
                         double rw, double rh);

#endif // CIRCULO_H