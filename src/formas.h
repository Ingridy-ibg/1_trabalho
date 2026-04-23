#ifndef FORMAS_H
#define FORMAS_H

#include <stdbool.h>

/**
 * @file formas.h
 * @brief Módulo central de gerenciamento das formas geométricas.
 *
 * Mantém o "banco de dados" de todas as figuras criadas pelos
 * comandos do arquivo .geo e manipuladas pelo arquivo .qry.
 * Internamente usa a Lista duplamente encadeada para armazenar
 * as figuras. Cada figura é identificada por um inteiro único.
 *
 * Também mantém o estado de seleção corrente (comando sel),
 * usada pelos comandos dels e mcs.
 *
 * Tipos de figura suportados: circulo, retangulo, linha, texto.
 */

/**
 * @typedef Formas
 * @brief Ponteiro opaco para o banco de dados de formas.
 */
typedef void *Formas;

/**
 * @typedef PosicForma
 * @brief Ponteiro opaco para a posição de uma forma na lista interna.
 * Usado para iterar sobre as formas sem expor a estrutura interna.
 */
typedef void *PosicForma;

/**
 * @brief Cria um novo banco de dados de formas vazio.
 * @return Ponteiro (Formas) para a estrutura alocada.
 * Retorna NULL em caso de falha de alocação.
 */
Formas criaFormas(void);

/**
 * @brief Libera toda a memória do banco de dados e de todas as formas.
 * @param fs Ponteiro para o banco de dados.
 */
void destroiFormas(Formas fs);

/* ─────────────────────────────────────────────
   Inserção de formas (comandos do .geo)
   ───────────────────────────────────────────── */

/**
 * @brief Insere um círculo no banco de dados (comando c).
 * @param fs   Banco de dados.
 * @param id   Identificador único (>= 1).
 * @param x    Coordenada x do centro.
 * @param y    Coordenada y do centro.
 * @param r    Raio (> 0).
 * @param corb Cor da borda (padrão SVG).
 * @param corp Cor do preenchimento (padrão SVG).
 * @return true se a inserção foi bem-sucedida, false caso contrário.
 */
bool insereCirculo(Formas fs, int id, double x, double y, double r,
                   const char *corb, const char *corp);

/**
 * @brief Insere um retângulo no banco de dados (comando r).
 * @param fs   Banco de dados.
 * @param id   Identificador único (>= 1).
 * @param x    Coordenada x do canto inferior esquerdo.
 * @param y    Coordenada y do canto inferior esquerdo.
 * @param w    Largura (> 0).
 * @param h    Altura (> 0).
 * @param corb Cor da borda (padrão SVG).
 * @param corp Cor do preenchimento (padrão SVG).
 * @return true se a inserção foi bem-sucedida, false caso contrário.
 */
bool insereRetangulo(Formas fs, int id, double x, double y, double w, double h,
                     const char *corb, const char *corp);

/**
 * @brief Insere uma linha no banco de dados (comando l).
 * @param fs  Banco de dados.
 * @param id  Identificador único (>= 1).
 * @param x1  Coordenada x do ponto inicial.
 * @param y1  Coordenada y do ponto inicial.
 * @param x2  Coordenada x do ponto final.
 * @param y2  Coordenada y do ponto final.
 * @param cor Cor da linha (padrão SVG).
 * @return true se a inserção foi bem-sucedida, false caso contrário.
 */
bool insereLinha(Formas fs, int id, double x1, double y1,
                 double x2, double y2, const char *cor);

/**
 * @brief Insere um texto no banco de dados (comando t).
 * @param fs    Banco de dados.
 * @param id    Identificador único (>= 1).
 * @param x     Coordenada x da âncora.
 * @param y     Coordenada y da âncora.
 * @param corb  Cor da borda (padrão SVG).
 * @param corp  Cor do preenchimento (padrão SVG).
 * @param ancora Posição da âncora: 'i', 'm' ou 'f'.
 * @param txto  Conteúdo textual.
 * @return true se a inserção foi bem-sucedida, false caso contrário.
 */
bool insereTexto(Formas fs, int id, double x, double y,
                 const char *corb, const char *corp,
                 char ancora, const char *txto);

/* ─────────────────────────────────────────────
   Consulta por identificador
   ───────────────────────────────────────────── */

/**
 * @brief Retorna a posição da forma com o identificador dado.
 * @param fs Banco de dados.
 * @param id Identificador da forma.
 * @return PosicForma da forma encontrada, ou NULL se não existir.
 */
PosicForma buscaFormaPorId(Formas fs, int id);

/**
 * @brief Obtém a coordenada âncora da forma identificada por id.
 * Para círculo: centro. Para retângulo: canto inf. esq.
 * Para linha: extremidade de menor x (desempate: menor y).
 * Para texto: posição da âncora.
 * @param fs Banco de dados.
 * @param id Identificador da forma.
 * @param x  Saída: coordenada x da âncora.
 * @param y  Saída: coordenada y da âncora.
 * @return true se encontrou e preencheu x e y, false caso contrário.
 */
bool getAncoraPorId(Formas fs, int id, double *x, double *y);

/**
 * @brief Retorna o tipo textual da forma na posição p.
 * @param fs Banco de dados.
 * @param p  Posição da forma.
 * @return String com o tipo: "circulo", "retangulo", "linha" ou "texto".
 * Retorna NULL se fs ou p forem NULL.
 */
const char *getTipoForma(Formas fs, PosicForma p);

/**
 * @brief Retorna o identificador da forma na posição p.
 * @param fs Banco de dados.
 * @param p  Posição da forma.
 * @return Identificador inteiro, ou -1 se fs ou p forem NULL.
 */
int getIdForma(Formas fs, PosicForma p);

/**
 * @brief Retorna o ponteiro interno da forma na posição p.
 * O tipo do ponteiro deve ser inferido via getTipoForma().
 * @param fs Banco de dados.
 * @param p  Posição da forma.
 * @return Ponteiro genérico para a struct interna da forma
 * (Circulo, Retangulo, Linha ou Texto). Retorna NULL se inválido.
 */
void *getDadosForma(Formas fs, PosicForma p);

/* ─────────────────────────────────────────────
   Iteração sobre todas as formas
   ───────────────────────────────────────────── */

/**
 * @brief Retorna a posição da primeira forma do banco de dados.
 * @param fs Banco de dados.
 * @return PosicForma da primeira forma, ou NULL se vazio.
 */
PosicForma getPrimeiraForma(Formas fs);

/**
 * @brief Retorna a posição da forma seguinte a p.
 * @param fs Banco de dados.
 * @param p  Posição atual.
 * @return PosicForma da próxima forma, ou NULL se p for a última.
 */
PosicForma getProximaForma(Formas fs, PosicForma p);

/**
 * @brief Retorna o número total de formas no banco de dados.
 * @param fs Banco de dados.
 * @return Quantidade de formas. Retorna 0 se fs for NULL.
 */
int totalFormas(Formas fs);

/* ─────────────────────────────────────────────
   Seleção (comando sel)
   ───────────────────────────────────────────── */

/**
 * @brief Seleciona as formas inteiramente dentro da região (comando sel).
 * Descarta qualquer seleção anterior.
 * @param fs Banco de dados.
 * @param x  Coordenada x do canto inferior esquerdo da região.
 * @param y  Coordenada y do canto inferior esquerdo da região.
 * @param w  Largura da região.
 * @param h  Altura da região.
 */
void selecionaFormas(Formas fs, double x, double y, double w, double h);

/**
 * @brief Verifica se a forma na posição p está selecionada.
 * @param fs Banco de dados.
 * @param p  Posição da forma.
 * @return true se selecionada, false caso contrário ou se inválido.
 */
bool isFormaSelecionada(Formas fs, PosicForma p);

/**
 * @brief Retorna o número de formas atualmente selecionadas.
 * @param fs Banco de dados.
 * @return Quantidade de formas selecionadas. Retorna 0 se fs for NULL.
 */
int totalFormasSelecionadas(Formas fs);

/* ─────────────────────────────────────────────
   Operações sobre seleção (comandos dels e mcs)
   ───────────────────────────────────────────── */

/**
 * @brief Remove do banco de dados todas as formas selecionadas (comando dels).
 * Após a remoção, não há formas selecionadas.
 * @param fs Banco de dados.
 */
void removeFormasSelecionadas(Formas fs);

/**
 * @brief Translada e recolore todas as formas selecionadas (comando mcs).
 * @param fs   Banco de dados.
 * @param dx   Deslocamento em x.
 * @param dy   Deslocamento em y.
 * @param corb Nova cor da borda (padrão SVG).
 * @param corp Nova cor do preenchimento (padrão SVG).
 */
void modificaFormasSelecionadas(Formas fs, double dx, double dy,
                                const char *corb, const char *corp);

#endif // FORMAS_H