#ifndef TEXTO_H
#define TEXTO_H

#include <stdbool.h>

/**
 * @file texto.h
 * @brief Módulo de criação e manipulação de textos.
 *
 * Um texto é definido por:
 *   - identificador inteiro único (id)
 *   - coordenada âncora (x, y)
 *   - cor da borda (corb) e cor do preenchimento (corp)
 *   - posição da âncora (a): 'i' = início, 'm' = meio, 'f' = fim
 *   - conteúdo textual (txto), pode conter espaços
 *   - estilo tipográfico: família (fFamily), peso (fWeight), tamanho (fSize)
 *
 * Corresponde ao comando 't' do arquivo .geo.
 * O estilo tipográfico é definido pelo comando 'ts' e afeta todos os
 * textos criados subsequentemente. O módulo mantém um estilo global
 * que pode ser atualizado via setEstiloTexto().
 *
 * Famílias de fonte válidas : "sans" (sans-serif), "serif", "cursive"
 * Pesos de fonte válidos    : "n" (normal), "b" (bold), "b+" (bolder),
 *                             "l" (lighter)
 */

/**
 * @typedef Texto
 * @brief Ponteiro opaco para a estrutura interna do texto.
 */
typedef void *Texto;

/**
 * @brief Atualiza o estilo tipográfico global (comando ts).
 * Todos os textos criados após esta chamada usarão este estilo.
 * @param fFamily Família da fonte: "sans", "serif" ou "cursive".
 * @param fWeight Peso da fonte: "n", "b", "b+" ou "l".
 * @param fSize   Tamanho da fonte em pontos (> 0).
 */
void setEstiloTexto(const char *fFamily, const char *fWeight, double fSize);

/**
 * @brief Cria um novo texto com os parâmetros fornecidos.
 * O estilo tipográfico aplicado será o estilo global corrente
 * definido pela última chamada a setEstiloTexto().
 * @param id    Identificador inteiro único do texto (>= 1).
 * @param x     Coordenada x da âncora.
 * @param y     Coordenada y da âncora.
 * @param corb  String com a cor da borda (padrão SVG).
 * @param corp  String com a cor do preenchimento (padrão SVG).
 * @param ancora Posição da âncora: 'i' (início), 'm' (meio), 'f' (fim).
 * @param txto  Conteúdo textual (pode conter espaços).
 * @return Ponteiro (Texto) para a estrutura alocada.
 * Retorna NULL em caso de falha de alocação ou parâmetros inválidos.
 */
Texto criaTexto(int id, double x, double y, const char *corb, const char *corp,
                char ancora, const char *txto);

/**
 * @brief Libera a memória alocada pelo texto.
 * @param t Ponteiro para o texto a ser destruído.
 */
void destroiTexto(Texto t);

/**
 * @brief Retorna o identificador do texto.
 * @param t Ponteiro para o texto.
 * @return Identificador inteiro. Retorna -1 se t for NULL.
 */
int getIdTexto(Texto t);

/**
 * @brief Retorna a coordenada x da âncora do texto.
 * @param t Ponteiro para o texto.
 * @return Valor de x. Retorna 0.0 se t for NULL.
 */
double getXTexto(Texto t);

/**
 * @brief Retorna a coordenada y da âncora do texto.
 * @param t Ponteiro para o texto.
 * @return Valor de y. Retorna 0.0 se t for NULL.
 */
double getYTexto(Texto t);

/**
 * @brief Retorna a cor da borda do texto.
 * @param t Ponteiro para o texto.
 * @return String com a cor da borda. Retorna NULL se t for NULL.
 */
const char *getCorbTexto(Texto t);

/**
 * @brief Retorna a cor do preenchimento do texto.
 * @param t Ponteiro para o texto.
 * @return String com a cor do preenchimento. Retorna NULL se t for NULL.
 */
const char *getCorpTexto(Texto t);

/**
 * @brief Retorna a posição da âncora do texto.
 * @param t Ponteiro para o texto.
 * @return Caractere 'i', 'm' ou 'f'. Retorna '\0' se t for NULL.
 */
char getAncora(Texto t);

/**
 * @brief Retorna o conteúdo textual.
 * @param t Ponteiro para o texto.
 * @return String com o conteúdo. Retorna NULL se t for NULL.
 */
const char *getTxto(Texto t);

/**
 * @brief Retorna a família da fonte do texto.
 * @param t Ponteiro para o texto.
 * @return String com a família ("sans", "serif" ou "cursive").
 * Retorna NULL se t for NULL.
 */
const char *getFamilyTexto(Texto t);

/**
 * @brief Retorna o peso da fonte do texto.
 * @param t Ponteiro para o texto.
 * @return String com o peso ("n", "b", "b+" ou "l").
 * Retorna NULL se t for NULL.
 */
const char *getWeightTexto(Texto t);

/**
 * @brief Retorna o tamanho da fonte do texto.
 * @param t Ponteiro para o texto.
 * @return Tamanho em pontos. Retorna 0.0 se t for NULL.
 */
double getSizeTexto(Texto t);

/**
 * @brief Translada o texto somando dx e dy à sua âncora.
 * @param t  Ponteiro para o texto.
 * @param dx Deslocamento em x.
 * @param dy Deslocamento em y.
 */
void transladaTexto(Texto t, double dx, double dy);

/**
 * @brief Altera as cores do texto.
 * @param t    Ponteiro para o texto.
 * @param corb Nova cor da borda (padrão SVG).
 * @param corp Nova cor do preenchimento (padrão SVG).
 */
void setCoresTexto(Texto t, const char *corb, const char *corp);

/**
 * @brief Verifica se o ponto (px, py) coincide com a âncora do texto.
 * @param t  Ponteiro para o texto.
 * @param px Coordenada x do ponto.
 * @param py Coordenada y do ponto.
 * @return true se o ponto está na âncora, false caso contrário ou se t for NULL.
 */
bool contemPontoTexto(Texto t, double px, double py);

/**
 * @brief Verifica se a âncora do texto está inteiramente contida
 * na região definida por (rx, ry, rw, rh).
 * @param t  Ponteiro para o texto.
 * @param rx Coordenada x do canto inferior esquerdo da região.
 * @param ry Coordenada y do canto inferior esquerdo da região.
 * @param rw Largura da região.
 * @param rh Altura da região.
 * @return true se a âncora estiver dentro da região.
 * false caso contrário ou se t for NULL.
 */
bool dentroRegiaoTexto(Texto t, double rx, double ry,
                       double rw, double rh);

#endif // TEXTO_H