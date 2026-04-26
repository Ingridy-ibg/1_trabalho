#ifndef SVG_H
#define SVG_H

/**
 * @file svg.h
 * @brief Módulo de geração de arquivos SVG para o TED.
 *
 * Responsabilidades:
 *   - Converter coordenadas do plano cartesiano (origem inferior-esquerda,
 *     Y crescente para cima) para o sistema SVG (origem superior-esquerda,
 *     Y crescente para baixo).
 *   - Escrever o cabeçalho e o rodapé do documento SVG.
 *   - Serializar cada tipo de forma geométrica como elemento SVG.
 *   - Emitir marcações visuais dos comandos .qry:
 *       sel  → retângulo pontilhado vermelho + anel nas âncoras
 *       dels → "x" vermelho na âncora de cada forma removida
 *
 * Fluxo de uso esperado pelo main:
 * @code
 *   FILE *f = fopen("saida.svg", "w");
 *   svgInicia(f, fs);           // cabeçalho + viewBox automático
 *   svgEscreveFormas(f, fs);    // todas as formas do banco
 *   // ... comandos qry emitem marcações via svgSel / svgDels ...
 *   svgFinaliza(f);             // </svg>
 *   fclose(f);
 * @endcode
 *
 * Nota sobre coordenadas:
 *   Todas as funções deste módulo recebem coordenadas no plano cartesiano
 *   (.geo / .qry). A conversão para SVG é feita internamente usando o
 *   bounding-box calculado em svgInicia().
 */

#include <stdio.h>
#include <stdbool.h>
#include "formas.h"

/* ─────────────────────────────────────────────
   Ciclo de vida do documento SVG
   ───────────────────────────────────────────── */

/**
 * @brief Abre o documento SVG e escreve o cabeçalho.
 *
 * Calcula o bounding-box de todas as formas em fs para determinar
 * o viewBox. Adiciona uma margem de 20 unidades em todos os lados.
 * Registra internamente a altura e o ymin do viewport para que as
 * demais funções possam converter coordenadas.
 *
 * Deve ser chamada uma única vez por arquivo, antes de qualquer outra
 * função deste módulo que escreva no mesmo FILE*.
 *
 * @param f  Arquivo SVG aberto para escrita (não pode ser NULL).
 * @param fs Banco de formas (usado para calcular o bounding-box).
 *           Se fs for NULL ou estiver vazio, usa janela padrão 200×200.
 */
void svgInicia(FILE *f, Formas fs);

/**
 * @brief Fecha o documento SVG escrevendo a tag de fechamento </svg>.
 *
 * @param f Arquivo SVG aberto para escrita (não pode ser NULL).
 */
void svgFinaliza(FILE *f);

/* ─────────────────────────────────────────────
   Serialização das formas
   ───────────────────────────────────────────── */

/**
 * @brief Serializa todas as formas do banco como elementos SVG.
 *
 * Itera sobre o banco na ordem de inserção e emite o elemento
 * SVG correspondente a cada forma:
 *   circulo   → <circle>
 *   retangulo → <rect>
 *   linha     → <line>
 *   texto     → <text> (com font-family, font-weight, font-size e text-anchor)
 *
 * Coordenadas são convertidas automaticamente para o sistema SVG.
 *
 * @param f  Arquivo SVG aberto para escrita.
 * @param fs Banco de formas.
 */
void svgEscreveFormas(FILE *f, Formas fs);

/* ─────────────────────────────────────────────
   Marcações visuais dos comandos .qry
   ───────────────────────────────────────────── */

/**
 * @brief Emite a marcação visual do comando sel.
 *
 * Desenha:
 *   1. Um retângulo de borda vermelha pontilhada delimitando a região.
 *   2. Um anel (círculo) vermelho em volta da âncora de cada forma
 *      atualmente selecionada no banco.
 *
 * Coordenadas recebidas no plano cartesiano; conversão feita internamente.
 *
 * @param f  Arquivo SVG aberto para escrita.
 * @param fs Banco de formas (consultado para obter as âncoras selecionadas).
 * @param x  Coordenada x do canto inferior esquerdo da região (plano cart.).
 * @param y  Coordenada y do canto inferior esquerdo da região (plano cart.).
 * @param w  Largura da região.
 * @param h  Altura da região.
 */
void svgSel(FILE *f, Formas fs, double x, double y, double w, double h);

/**
 * @brief Emite um pequeno "x" vermelho na âncora de uma forma removida.
 *
 * Deve ser chamada para cada forma antes de ela ser removida do banco,
 * pois usa a âncora ainda disponível no banco.
 *
 * Coordenadas recebidas no plano cartesiano; conversão feita internamente.
 *
 * @param f  Arquivo SVG aberto para escrita.
 * @param ax Coordenada x da âncora da forma removida (plano cart.).
 * @param ay Coordenada y da âncora da forma removida (plano cart.).
 */
void svgDels(FILE *f, double ax, double ay);

void svgEscreveForma(FILE *f, Formas fs, int id);

#endif /* SVG_H */