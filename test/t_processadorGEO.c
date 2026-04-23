/**
 * @file t_processadorGEO.c
 * @brief Testes unitários para processadorGEO.c
 *
 * Estratégia: escreve arquivos .geo temporários em /tmp,
 * chama processaGeo() e verifica o estado do banco de formas.
 */

#include "unity.h"
#include "../src/processadorGEO.h"
#include "../src/formas.h"
#include "../src/circulo.h"
#include "../src/retangulo.h"
#include "../src/linha.h"
#include "../src/texto.h"

#include <stdio.h>
#include <string.h>

/* ─────────────────────────────────────────────
   Helpers
   ───────────────────────────────────────────── */

static Formas F;

/** Escreve conteúdo em /tmp/<nome> e retorna o caminho. */
static const char *geo(const char *nome, const char *conteudo) {
    static char cam[256];
    snprintf(cam, sizeof(cam), "/tmp/%s", nome);
    FILE *f = fopen(cam, "w");
    if (f) { fputs(conteudo, f); fclose(f); }
    return cam;
}

void setUp(void) {
    F = criaFormas();
    /* Reseta o estilo para o padrão (sans, n, 12.0) para garantir testes independentes */
    setEstiloTexto("sans", "n", 12.0);
}
void tearDown(void) { destroiFormas(F); }

/* ─────────────────────────────────────────────
   Arquivo inexistente
   ───────────────────────────────────────────── */

void test_geo_ArquivoInexistente_RetornaFalse(void) {
    bool ok = processaGeo("/tmp/__nao_existe__.geo", F);
    TEST_ASSERT_FALSE_MESSAGE(ok, "Deve retornar false para arquivo inexistente.");
    TEST_ASSERT_EQUAL_INT(0, totalFormas(F));
}

void test_geo_ArquivoNulo_RetornaFalse(void) {
    bool ok = processaGeo(NULL, F);
    TEST_ASSERT_FALSE(ok);
}

void test_geo_BancoNulo_RetornaFalse(void) {
    const char *c = geo("geo_nulo.geo", "c 1 0 0 5 black red\n");
    bool ok = processaGeo(c, NULL);
    TEST_ASSERT_FALSE(ok);
}

/* ─────────────────────────────────────────────
   Arquivo vazio / só comentários
   ───────────────────────────────────────────── */

void test_geo_ArquivoVazio(void) {
    const char *c = geo("geo_vazio.geo", "");
    TEST_ASSERT_TRUE(processaGeo(c, F));
    TEST_ASSERT_EQUAL_INT(0, totalFormas(F));
}

void test_geo_SoComentarios(void) {
    const char *c = geo("geo_coments.geo",
        "# linha 1\n"
        "# linha 2\n"
        "\n");
    TEST_ASSERT_TRUE(processaGeo(c, F));
    TEST_ASSERT_EQUAL_INT(0, totalFormas(F));
}

/* ─────────────────────────────────────────────
   Comando c (círculo)
   ───────────────────────────────────────────── */

void test_geo_Circulo_InsereUm(void) {
    const char *c = geo("geo_c1.geo",
        "c 1 50.0 60.0 10.0 black red\n");
    processaGeo(c, F);
    TEST_ASSERT_EQUAL_INT(1, totalFormas(F));
    PosicForma p = getPrimeiraForma(F);
    TEST_ASSERT_EQUAL_STRING("circulo", getTipoForma(F, p));
}

void test_geo_Circulo_Valores(void) {
    const char *c = geo("geo_c2.geo",
        "c 7 12.5 33.0 4.0 blue yellow\n");
    processaGeo(c, F);
    PosicForma p = getPrimeiraForma(F);
    Circulo ci = (Circulo)getDadosForma(F, p);
    TEST_ASSERT_EQUAL_INT(7,       getIdCirculo(ci));
    TEST_ASSERT_EQUAL_DOUBLE(12.5, getXCirculo(ci));
    TEST_ASSERT_EQUAL_DOUBLE(33.0, getYCirculo(ci));
    TEST_ASSERT_EQUAL_DOUBLE(4.0,  getRCirculo(ci));
    TEST_ASSERT_EQUAL_STRING("blue",   getCorbCirculo(ci));
    TEST_ASSERT_EQUAL_STRING("yellow", getCorpCirculo(ci));
}

/* ─────────────────────────────────────────────
   Comando r (retângulo)
   ───────────────────────────────────────────── */

void test_geo_Retangulo_InsereUm(void) {
    const char *c = geo("geo_r1.geo",
        "r 2 0.0 0.0 100.0 50.0 black blue\n");
    processaGeo(c, F);
    TEST_ASSERT_EQUAL_INT(1, totalFormas(F));
    TEST_ASSERT_EQUAL_STRING("retangulo",
        getTipoForma(F, getPrimeiraForma(F)));
}

void test_geo_Retangulo_Valores(void) {
    const char *c = geo("geo_r2.geo",
        "r 3 10.0 20.0 80.0 40.0 green white\n");
    processaGeo(c, F);
    Retangulo r = (Retangulo)getDadosForma(F, getPrimeiraForma(F));
    TEST_ASSERT_EQUAL_INT(3,      getIdRetangulo(r));
    TEST_ASSERT_EQUAL_DOUBLE(10.0, getXRetangulo(r));
    TEST_ASSERT_EQUAL_DOUBLE(20.0, getYRetangulo(r));
    TEST_ASSERT_EQUAL_DOUBLE(80.0, getWRetangulo(r));
    TEST_ASSERT_EQUAL_DOUBLE(40.0, getHRetangulo(r));
    TEST_ASSERT_EQUAL_STRING("green", getCorbRetangulo(r));
    TEST_ASSERT_EQUAL_STRING("white", getCorpRetangulo(r));
}

/* ─────────────────────────────────────────────
   Comando l (linha)
   ───────────────────────────────────────────── */

void test_geo_Linha_InsereUma(void) {
    const char *c = geo("geo_l1.geo",
        "l 4 0.0 0.0 100.0 100.0 green\n");
    processaGeo(c, F);
    TEST_ASSERT_EQUAL_INT(1, totalFormas(F));
    TEST_ASSERT_EQUAL_STRING("linha",
        getTipoForma(F, getPrimeiraForma(F)));
}

void test_geo_Linha_Valores(void) {
    const char *c = geo("geo_l2.geo",
        "l 5 1.0 2.0 3.0 4.0 red\n");
    processaGeo(c, F);
    Linha l = (Linha)getDadosForma(F, getPrimeiraForma(F));
    TEST_ASSERT_EQUAL_INT(5,     getIdLinha(l));
    TEST_ASSERT_EQUAL_DOUBLE(1.0, getX1Linha(l));
    TEST_ASSERT_EQUAL_DOUBLE(2.0, getY1Linha(l));
    TEST_ASSERT_EQUAL_DOUBLE(3.0, getX2Linha(l));
    TEST_ASSERT_EQUAL_DOUBLE(4.0, getY2Linha(l));
    TEST_ASSERT_EQUAL_STRING("red", getCorLinha(l));
}

/* ─────────────────────────────────────────────
   Comando t (texto)
   ───────────────────────────────────────────── */

void test_geo_Texto_InsereUm(void) {
    const char *c = geo("geo_t1.geo",
        "t 6 30.0 40.0 black white i Ola Mundo\n");
    processaGeo(c, F);
    TEST_ASSERT_EQUAL_INT(1, totalFormas(F));
    TEST_ASSERT_EQUAL_STRING("texto",
        getTipoForma(F, getPrimeiraForma(F)));
}

void test_geo_Texto_Valores(void) {
    const char *c = geo("geo_t2.geo",
        "t 8 5.5 7.0 blue red m Texto com espacos\n");
    processaGeo(c, F);
    Texto t = (Texto)getDadosForma(F, getPrimeiraForma(F));
    TEST_ASSERT_EQUAL_INT(8,         getIdTexto(t));
    TEST_ASSERT_EQUAL_DOUBLE(5.5,    getXTexto(t));
    TEST_ASSERT_EQUAL_DOUBLE(7.0,    getYTexto(t));
    TEST_ASSERT_EQUAL_STRING("blue",  getCorbTexto(t));
    TEST_ASSERT_EQUAL_STRING("red",   getCorpTexto(t));
    TEST_ASSERT_EQUAL_CHAR('m',       getAncora(t));
    TEST_ASSERT_EQUAL_STRING("Texto com espacos", getTxto(t));
}

void test_geo_Texto_AncoraMeio(void) {
    const char *c = geo("geo_t3.geo", "t 1 0 0 black white m Centro\n");
    processaGeo(c, F);
    Texto t = (Texto)getDadosForma(F, getPrimeiraForma(F));
    TEST_ASSERT_EQUAL_CHAR('m', getAncora(t));
}

void test_geo_Texto_AncoraFim(void) {
    const char *c = geo("geo_t4.geo", "t 1 0 0 black white f Fim\n");
    processaGeo(c, F);
    Texto t = (Texto)getDadosForma(F, getPrimeiraForma(F));
    TEST_ASSERT_EQUAL_CHAR('f', getAncora(t));
}

/* ─────────────────────────────────────────────
   Comando ts (estilo de texto)
   ───────────────────────────────────────────── */

void test_geo_Ts_AplicaEstiloAoTextoSeguinte(void) {
    const char *c = geo("geo_ts1.geo",
        "ts serif b 18.0\n"
        "t 1 0.0 0.0 black white i Estilizado\n");
    processaGeo(c, F);
    Texto t = (Texto)getDadosForma(F, getPrimeiraForma(F));
    TEST_ASSERT_EQUAL_STRING("serif", getFamilyTexto(t));
    TEST_ASSERT_EQUAL_STRING("b",     getWeightTexto(t));
    TEST_ASSERT_EQUAL_DOUBLE(18.0,    getSizeTexto(t));
}

void test_geo_Ts_NaoAplicaAoTextoAnterior(void) {
    const char *c = geo("geo_ts2.geo",
        "t 1 0.0 0.0 black white i Antes\n"
        "ts cursive b+ 24.0\n"
        "t 2 0.0 0.0 black white i Depois\n");
    processaGeo(c, F);

    PosicForma p1 = buscaFormaPorId(F, 1);
    PosicForma p2 = buscaFormaPorId(F, 2);

    Texto t1 = (Texto)getDadosForma(F, p1);
    Texto t2 = (Texto)getDadosForma(F, p2);

    /* t2 deve ter o novo estilo */
    TEST_ASSERT_EQUAL_STRING("cursive", getFamilyTexto(t2));
    TEST_ASSERT_EQUAL_DOUBLE(24.0,      getSizeTexto(t2));

    /* t1 deve ter estilo padrão (sans, n, 12) */
    TEST_ASSERT_EQUAL_STRING("sans", getFamilyTexto(t1));
}

/* ─────────────────────────────────────────────
   Múltiplas formas e ordem de inserção
   ───────────────────────────────────────────── */

void test_geo_MultiplasForms_ContagemCorreta(void) {
    const char *c = geo("geo_multi.geo",
        "c 1 50.0 50.0 10.0 black red\n"
        "r 2 0.0  0.0 100.0 50.0 black blue\n"
        "l 3 0.0  0.0 100.0 100.0 green\n"
        "t 4 10.0 20.0 black white i Oi\n");
    processaGeo(c, F);
    TEST_ASSERT_EQUAL_INT(4, totalFormas(F));
}

void test_geo_MultiplasForms_OrdemPreservada(void) {
    const char *c = geo("geo_ordem.geo",
        "c 10 0 0 5 black red\n"
        "r 20 0 0 10 10 black blue\n"
        "l 30 0 0 5 5 green\n");
    processaGeo(c, F);

    PosicForma p = getPrimeiraForma(F);
    TEST_ASSERT_EQUAL_INT(10, getIdForma(F, p));
    p = getProximaForma(F, p);
    TEST_ASSERT_EQUAL_INT(20, getIdForma(F, p));
    p = getProximaForma(F, p);
    TEST_ASSERT_EQUAL_INT(30, getIdForma(F, p));
}

void test_geo_IgnoraLinhasVazias(void) {
    const char *c = geo("geo_blanks.geo",
        "\n"
        "# comentario\n"
        "\n"
        "c 1 0.0 0.0 5.0 black red\n"
        "\n");
    processaGeo(c, F);
    TEST_ASSERT_EQUAL_INT(1, totalFormas(F));
}

void test_geo_IgnoraComandoDesconhecido(void) {
    const char *c = geo("geo_unk.geo",
        "c 1 0 0 5 black red\n"
        "xyz 99 ignorado\n"
        "r 2 0 0 10 10 black blue\n");
    processaGeo(c, F);
    TEST_ASSERT_EQUAL_INT(2, totalFormas(F));
}

/* ─────────────────────────────────────────────
   Runner
   ───────────────────────────────────────────── */

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_geo_ArquivoInexistente_RetornaFalse);
    RUN_TEST(test_geo_ArquivoNulo_RetornaFalse);
    RUN_TEST(test_geo_BancoNulo_RetornaFalse);
    RUN_TEST(test_geo_ArquivoVazio);
    RUN_TEST(test_geo_SoComentarios);

    RUN_TEST(test_geo_Circulo_InsereUm);
    RUN_TEST(test_geo_Circulo_Valores);

    RUN_TEST(test_geo_Retangulo_InsereUm);
    RUN_TEST(test_geo_Retangulo_Valores);

    RUN_TEST(test_geo_Linha_InsereUma);
    RUN_TEST(test_geo_Linha_Valores);

    RUN_TEST(test_geo_Texto_InsereUm);
    RUN_TEST(test_geo_Texto_Valores);
    RUN_TEST(test_geo_Texto_AncoraMeio);
    RUN_TEST(test_geo_Texto_AncoraFim);

    RUN_TEST(test_geo_Ts_AplicaEstiloAoTextoSeguinte);
    RUN_TEST(test_geo_Ts_NaoAplicaAoTextoAnterior);

    RUN_TEST(test_geo_MultiplasForms_ContagemCorreta);
    RUN_TEST(test_geo_MultiplasForms_OrdemPreservada);
    RUN_TEST(test_geo_IgnoraLinhasVazias);
    RUN_TEST(test_geo_IgnoraComandoDesconhecido);

    return UNITY_END();
}