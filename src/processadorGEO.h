#ifndef PROCESSADORGEO_H
#define PROCESSADORGEO_H

/**
 * @file processadorGEO.h
 * @brief Leitura e processamento do arquivo .geo.
 *
 * Responsabilidade única: abrir o arquivo .geo, parsear linha a linha
 * e popular o banco de formas (Formas) com os comandos reconhecidos.
 *
 * Comandos suportados:
 *   c  i x y r corb corp          — círculo
 *   r  i x y w h corb corp        — retângulo
 *   l  i x1 y1 x2 y2 cor          — linha
 *   t  i x y corb corp a txto     — texto (txto vai até o fim da linha)
 *   ts fFamily fWeight fSize       — muda estilo dos textos seguintes
 *
 * Linhas em branco e linhas iniciadas por '#' são ignoradas.
 */

#include "formas.h"

/**
 * @brief Processa o arquivo .geo e insere todas as formas no banco.
 *
 * Abre o arquivo em caminho, lê cada linha, identifica o comando e
 * chama a função de inserção correspondente em fs.
 * Comandos desconhecidos são silenciosamente ignorados.
 *
 * @param caminho Caminho completo do arquivo .geo a ser lido.
 * @param fs      Banco de dados de formas já criado (não pode ser NULL).
 * @return true  se o arquivo foi aberto e lido sem erros fatais.
 *         false se o arquivo não pôde ser aberto (mensagem no stderr).
 */
bool processaGeo(const char *caminho, Formas fs);

#endif /* PROCESSADORGEO_H */