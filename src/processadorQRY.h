#ifndef PROCESSADORQRY_H
#define PROCESSADORQRY_H

/**
 * @file processadorQRY.h
 * @brief Leitura e processamento do arquivo .qry.
 *
 * Responsabilidade única: abrir o arquivo .qry, parsear linha a linha
 * e executar cada consulta sobre o banco de formas, escrevendo as
 * saídas nos arquivos SVG e TXT fornecidos.
 *
 * Comandos suportados:
 *   inp p i             — insere âncora da figura i no polígono p
 *   rmp p               — remove a coordenada mais antiga do polígono p
 *   pol p i d corb corp — gera as linhas de borda e preenchimento do polígono p
 *   clp p               — remove todas as coordenadas do polígono p
 *   sel x y w h         — seleciona figuras inteiramente dentro da região
 *   dels                — remove do banco todas as figuras selecionadas
 *   mcs dx dy corb corp — translada e recolore as figuras selecionadas
 *
 * Linhas em branco e linhas iniciadas por '#' são ignoradas.
 *
 * O arquivo SVG recebido já deve ter o cabeçalho escrito pelo chamador.
 * O arquivo TXT é aberto sem cabeçalho; cada comando que produz saída
 * escreve suas próprias linhas.
 */

#include <stdio.h>
#include <stdbool.h>
#include "formas.h"

/**
 * @brief Processa o arquivo .qry e executa todas as consultas.
 *
 * Para cada comando reconhecido:
 *  - Atualiza o banco de formas (fs) conforme necessário.
 *  - Escreve marcações visuais no arquivo SVG (svg_out).
 *  - Escreve o relatório textual no arquivo TXT (txt_out).
 *
 * @param caminho  Caminho completo do arquivo .qry a ser lido.
 * @param fs       Banco de dados de formas já populado (não pode ser NULL).
 * @param svg_out  Arquivo SVG aberto para escrita (não pode ser NULL).
 * @param txt_out  Arquivo TXT aberto para escrita (não pode ser NULL).
 * @return true  se o arquivo foi aberto e lido sem erros fatais.
 *         false se o arquivo não pôde ser aberto (mensagem no stderr).
 */
bool processaQry(const char *caminho, Formas fs,
                 FILE *svg_out, FILE *txt_out);

#endif /* PROCESSADORQRY_H */