/**
 * @file t_main.c
 * @brief Testes unitários para main.c (parseArgs, getters, destroiArgs).
 *
 * Estratégia:
 *   - Monta vetores argv "fake" simulando chamadas de linha de comando.
 *   - Chama parseArgs() e verifica os campos via getters.
 *   - Testa casos de sucesso, erro (faltando obrigatórios) e edge cases (NULL).
 *
 * Cada teste é independente: nao usa estado global persistente.
 * Como parseArgs imprime no stderr em caso de erro, os testes que
 * exercitam erro vao gerar output no stderr — isso e esperado.
 */

#include "unity.h"
#include "../src/main.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* ─────────────────────────────────────────────
   setUp / tearDown
   ───────────────────────────────────────────── */

void setUp(void)    { /* nada a fazer */ }
void tearDown(void) { /* nada a fazer */ }

/* ─────────────────────────────────────────────
   parseArgs — casos de sucesso
   ───────────────────────────────────────────── */

void test_parseArgs_TodosOsParametros(void)
{
    char *argv[] = {
        "ted",
        "-e", "entradas",
        "-f", "cidade.geo",
        "-o", "saidas",
        "-q", "consulta.qry"
    };
    int argc = 9;

    Args a = parseArgs(argc, argv);
    TEST_ASSERT_NOT_NULL_MESSAGE(a, "parseArgs com todos os parametros deve retornar Args.");

    TEST_ASSERT_EQUAL_STRING_MESSAGE("entradas",    getBaseEntrada(a), "-e nao foi capturado.");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("cidade.geo",  getArqGeo(a),      "-f nao foi capturado.");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("saidas",      getBaseSaida(a),   "-o nao foi capturado.");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("consulta.qry",getArqQry(a),      "-q nao foi capturado.");

    destroiArgs(a);
}

void test_parseArgs_SoObrigatorios(void)
{
    char *argv[] = { "ted", "-f", "x.geo", "-o", "out" };
    int argc = 5;

    Args a = parseArgs(argc, argv);
    TEST_ASSERT_NOT_NULL_MESSAGE(a, "parseArgs so com -f e -o deve funcionar.");

    /* -e nao informado: deve usar o padrao "." */
    TEST_ASSERT_EQUAL_STRING_MESSAGE(".",     getBaseEntrada(a), "-e ausente deve cair no padrao '.'");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("x.geo", getArqGeo(a),      "-f deve ser capturado.");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("out",   getBaseSaida(a),   "-o deve ser capturado.");
    /* -q nao informado: deve ser string vazia */
    TEST_ASSERT_EQUAL_STRING_MESSAGE("",      getArqQry(a),      "-q ausente deve ser string vazia.");

    destroiArgs(a);
}

void test_parseArgs_OrdemArbitraria(void)
{
    /* Ordem diferente: -q antes de -f, -o antes de -e */
    char *argv[] = {
        "ted",
        "-q", "qry.qry",
        "-o", "saida",
        "-e", "entrada",
        "-f", "geo.geo"
    };
    int argc = 9;

    Args a = parseArgs(argc, argv);
    TEST_ASSERT_NOT_NULL_MESSAGE(a, "parseArgs deve aceitar parametros em qualquer ordem.");

    TEST_ASSERT_EQUAL_STRING_MESSAGE("entrada", getBaseEntrada(a), "-e em ordem trocada.");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("geo.geo", getArqGeo(a),      "-f em ordem trocada.");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("saida",   getBaseSaida(a),   "-o em ordem trocada.");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("qry.qry", getArqQry(a),      "-q em ordem trocada.");

    destroiArgs(a);
}

/* ─────────────────────────────────────────────
   parseArgs — casos de erro
   ───────────────────────────────────────────── */

void test_parseArgs_FaltaArqGeo(void)
{
    /* Falta -f: deve falhar */
    char *argv[] = { "ted", "-o", "saida" };
    int argc = 3;

    Args a = parseArgs(argc, argv);
    TEST_ASSERT_NULL_MESSAGE(a, "parseArgs sem -f deve retornar NULL.");
}

void test_parseArgs_FaltaBaseSaida(void)
{
    /* Falta -o: deve falhar */
    char *argv[] = { "ted", "-f", "x.geo" };
    int argc = 3;

    Args a = parseArgs(argc, argv);
    TEST_ASSERT_NULL_MESSAGE(a, "parseArgs sem -o deve retornar NULL.");
}

void test_parseArgs_SemArgumentos(void)
{
    char *argv[] = { "ted" };
    int argc = 1;

    Args a = parseArgs(argc, argv);
    TEST_ASSERT_NULL_MESSAGE(a, "parseArgs sem argumentos deve retornar NULL.");
}

void test_parseArgs_FlagSemValor(void)
{
    /* -f no final, sem o nome do arquivo a seguir: deve falhar */
    char *argv[] = { "ted", "-o", "saida", "-f" };
    int argc = 4;

    Args a = parseArgs(argc, argv);
    TEST_ASSERT_NULL_MESSAGE(a, "Flag sem valor deve resultar em arq_geo vazio e retornar NULL.");
}

/* ─────────────────────────────────────────────
   destroiArgs
   ───────────────────────────────────────────── */

void test_destroiArgs_NaoCrashaComNULL(void)
{
    /* Apenas verificar que nao da segfault. */
    destroiArgs(NULL);
    TEST_PASS();
}

void test_destroiArgs_LiberaArgsValido(void)
{
    char *argv[] = { "ted", "-f", "x.geo", "-o", "out" };
    Args a = parseArgs(5, argv);
    TEST_ASSERT_NOT_NULL(a);

    /* Nao deve dar segfault nem leak detectavel pelo asan. */
    destroiArgs(a);
    TEST_PASS();
}

/* ─────────────────────────────────────────────
   Getters — comportamento com NULL
   ───────────────────────────────────────────── */

void test_Getters_RetornamNULLquandoArgsNULL(void)
{
    TEST_ASSERT_NULL_MESSAGE(getBaseEntrada(NULL), "getBaseEntrada(NULL) deve retornar NULL.");
    TEST_ASSERT_NULL_MESSAGE(getArqGeo(NULL),      "getArqGeo(NULL) deve retornar NULL.");
    TEST_ASSERT_NULL_MESSAGE(getBaseSaida(NULL),   "getBaseSaida(NULL) deve retornar NULL.");
    TEST_ASSERT_NULL_MESSAGE(getArqQry(NULL),      "getArqQry(NULL) deve retornar NULL.");
}

/* ─────────────────────────────────────────────
   Casos com caminhos com barras
   ───────────────────────────────────────────── */

void test_parseArgs_CaminhosComBarras(void)
{
    char *argv[] = {
        "ted",
        "-e", "/home/user/inputs",
        "-f", "subdir/cidade.geo",
        "-o", "../outputs",
        "-q", "subdir/q.qry"
    };
    int argc = 9;

    Args a = parseArgs(argc, argv);
    TEST_ASSERT_NOT_NULL(a);

    TEST_ASSERT_EQUAL_STRING("/home/user/inputs",  getBaseEntrada(a));
    TEST_ASSERT_EQUAL_STRING("subdir/cidade.geo",  getArqGeo(a));
    TEST_ASSERT_EQUAL_STRING("../outputs",         getBaseSaida(a));
    TEST_ASSERT_EQUAL_STRING("subdir/q.qry",       getArqQry(a));

    destroiArgs(a);
}

/* ─────────────────────────────────────────────
   Sobrescrita: a ultima ocorrencia de uma flag prevalece
   ───────────────────────────────────────────── */

void test_parseArgs_FlagsRepetidas_UltimaVence(void)
{
    char *argv[] = {
        "ted",
        "-f", "primeiro.geo",
        "-f", "segundo.geo",
        "-o", "out"
    };
    int argc = 7;

    Args a = parseArgs(argc, argv);
    TEST_ASSERT_NOT_NULL(a);
    TEST_ASSERT_EQUAL_STRING_MESSAGE("segundo.geo", getArqGeo(a),
        "Quando uma flag aparece varias vezes, a ultima deve prevalecer.");

    destroiArgs(a);
}

/* ─────────────────────────────────────────────
   main
   ───────────────────────────────────────────── */

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_parseArgs_TodosOsParametros);
    RUN_TEST(test_parseArgs_SoObrigatorios);
    RUN_TEST(test_parseArgs_OrdemArbitraria);
    RUN_TEST(test_parseArgs_CaminhosComBarras);
    RUN_TEST(test_parseArgs_FlagsRepetidas_UltimaVence);

    RUN_TEST(test_parseArgs_FaltaArqGeo);
    RUN_TEST(test_parseArgs_FaltaBaseSaida);
    RUN_TEST(test_parseArgs_SemArgumentos);
    RUN_TEST(test_parseArgs_FlagSemValor);

    RUN_TEST(test_destroiArgs_NaoCrashaComNULL);
    RUN_TEST(test_destroiArgs_LiberaArgsValido);

    RUN_TEST(test_Getters_RetornamNULLquandoArgsNULL);

    return UNITY_END();
}