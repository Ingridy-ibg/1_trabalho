#include "unity.h"
#include "../src/formas.h"
#include "../src/circulo.h"
#include "../src/retangulo.h"
#include "../src/linha.h"
#include "../src/texto.h"

Formas F;

void setUp(void)
{
    F = criaFormas();
}

void tearDown(void)
{
    destroiFormas(F);
}

/* ─────────────────────────────────────────────
   criaFormas
   ───────────────────────────────────────────── */

void test_criaFormas_Valido(void)
{
    TEST_ASSERT_NOT_NULL_MESSAGE(F, "Banco de dados nao deve ser NULL.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, totalFormas(F),            "Total inicial deve ser 0.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, totalFormasSelecionadas(F),"Total selecionadas inicial deve ser 0.");
    TEST_ASSERT_NULL_MESSAGE(getPrimeiraForma(F),               "PrimeiraForma de banco vazio deve ser NULL.");
}

/* ─────────────────────────────────────────────
   Inserção
   ───────────────────────────────────────────── */

void test_insereCirculo_Valido(void)
{
    bool ok = insereCirculo(F, 1, 50.0, 50.0, 10.0, "black", "red");
    TEST_ASSERT_TRUE_MESSAGE(ok, "insereCirculo deve retornar true.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, totalFormas(F), "Total deve ser 1.");
}

void test_insereRetangulo_Valido(void)
{
    bool ok = insereRetangulo(F, 2, 0.0, 0.0, 100.0, 50.0, "black", "blue");
    TEST_ASSERT_TRUE_MESSAGE(ok, "insereRetangulo deve retornar true.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, totalFormas(F), "Total deve ser 1.");
}

void test_insereLinha_Valido(void)
{
    bool ok = insereLinha(F, 3, 0.0, 0.0, 100.0, 100.0, "green");
    TEST_ASSERT_TRUE_MESSAGE(ok, "insereLinha deve retornar true.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, totalFormas(F), "Total deve ser 1.");
}

void test_insereTexto_Valido(void)
{
    bool ok = insereTexto(F, 4, 10.0, 20.0, "black", "white", 'i', "Ola Mundo");
    TEST_ASSERT_TRUE_MESSAGE(ok, "insereTexto deve retornar true.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, totalFormas(F), "Total deve ser 1.");
}

void test_insere_TodasAsFormas(void)
{
    insereCirculo(F,   1, 50.0, 50.0, 10.0, "black", "red");
    insereRetangulo(F, 2, 0.0,  0.0, 100.0, 50.0, "black", "blue");
    insereLinha(F,     3, 0.0,  0.0, 100.0, 100.0, "green");
    insereTexto(F,     4, 10.0, 20.0, "black", "white", 'i', "Ola");

    TEST_ASSERT_EQUAL_INT_MESSAGE(4, totalFormas(F), "Total deve ser 4.");
}

void test_insere_FormaNula(void)
{
    TEST_ASSERT_FALSE_MESSAGE(insereCirculo(NULL, 1, 0.0, 0.0, 10.0, "black", "red"),
        "Inserir em banco NULL deve retornar false.");
}

/* ─────────────────────────────────────────────
   buscaFormaPorId / getTipo / getId / getDados
   ───────────────────────────────────────────── */

void test_buscaFormaPorId_Encontra(void)
{
    insereCirculo(F, 1, 50.0, 50.0, 10.0, "black", "red");
    insereRetangulo(F, 2, 0.0, 0.0, 100.0, 50.0, "black", "blue");

    PosicForma p1 = buscaFormaPorId(F, 1);
    PosicForma p2 = buscaFormaPorId(F, 2);

    TEST_ASSERT_NOT_NULL_MESSAGE(p1, "Deve encontrar forma com id 1.");
    TEST_ASSERT_NOT_NULL_MESSAGE(p2, "Deve encontrar forma com id 2.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, getIdForma(F, p1), "Id da posicao 1 deve ser 1.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(2, getIdForma(F, p2), "Id da posicao 2 deve ser 2.");
}

void test_buscaFormaPorId_NaoEncontra(void)
{
    insereCirculo(F, 1, 50.0, 50.0, 10.0, "black", "red");
    PosicForma p = buscaFormaPorId(F, 99);
    TEST_ASSERT_NULL_MESSAGE(p, "Busca por id inexistente deve retornar NULL.");
}

void test_getTipoForma(void)
{
    insereCirculo(F,   1, 50.0, 50.0, 10.0, "black", "red");
    insereRetangulo(F, 2, 0.0,  0.0, 100.0, 50.0, "black", "blue");
    insereLinha(F,     3, 0.0,  0.0, 100.0, 100.0, "green");
    insereTexto(F,     4, 10.0, 20.0, "black", "white", 'i', "Ola");

    TEST_ASSERT_EQUAL_STRING_MESSAGE("circulo",   getTipoForma(F, buscaFormaPorId(F, 1)), "Tipo deve ser circulo.");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("retangulo", getTipoForma(F, buscaFormaPorId(F, 2)), "Tipo deve ser retangulo.");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("linha",     getTipoForma(F, buscaFormaPorId(F, 3)), "Tipo deve ser linha.");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("texto",     getTipoForma(F, buscaFormaPorId(F, 4)), "Tipo deve ser texto.");
}

void test_getDadosForma(void)
{
    insereCirculo(F, 1, 50.0, 50.0, 10.0, "black", "red");
    PosicForma p = buscaFormaPorId(F, 1);

    void *dados = getDadosForma(F, p);
    TEST_ASSERT_NOT_NULL_MESSAGE(dados, "getDadosForma nao deve retornar NULL.");

    // Verifica que os dados correspondem ao circulo criado
    Circulo c = (Circulo)dados;
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(50.0, getXCirculo(c), "X do circulo deve ser 50.0.");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(10.0, getRCirculo(c), "Raio do circulo deve ser 10.0.");
}

/* ─────────────────────────────────────────────
   getAncoraPorId
   ───────────────────────────────────────────── */

void test_getAncora_Circulo(void)
{
    insereCirculo(F, 1, 30.0, 40.0, 10.0, "black", "red");
    double x, y;
    bool ok = getAncoraPorId(F, 1, &x, &y);
    TEST_ASSERT_TRUE_MESSAGE(ok, "Deve encontrar a ancora.");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(30.0, x, "Ancora x do circulo deve ser 30.0.");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(40.0, y, "Ancora y do circulo deve ser 40.0.");
}

void test_getAncora_Retangulo(void)
{
    insereRetangulo(F, 2, 5.0, 15.0, 100.0, 50.0, "black", "blue");
    double x, y;
    bool ok = getAncoraPorId(F, 2, &x, &y);
    TEST_ASSERT_TRUE_MESSAGE(ok, "Deve encontrar a ancora.");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(5.0,  x, "Ancora x do retangulo deve ser 5.0.");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(15.0, y, "Ancora y do retangulo deve ser 15.0.");
}

void test_getAncora_Linha_MenorX(void)
{
    // extremidade de menor x
    insereLinha(F, 3, 20.0, 10.0, 5.0, 30.0, "green");
    double x, y;
    bool ok = getAncoraPorId(F, 3, &x, &y);
    TEST_ASSERT_TRUE_MESSAGE(ok, "Deve encontrar a ancora.");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(5.0,  x, "Ancora x da linha deve ser o menor x (5.0).");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(30.0, y, "Ancora y da linha deve ser 30.0.");
}

void test_getAncora_Linha_Empate(void)
{
    // empate em x: usar menor y
    insereLinha(F, 4, 10.0, 50.0, 10.0, 20.0, "green");
    double x, y;
    getAncoraPorId(F, 4, &x, &y);
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(10.0, x, "X deve ser 10.0.");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(20.0, y, "Y deve ser o menor (20.0) no empate.");
}

void test_getAncora_Texto(void)
{
    insereTexto(F, 5, 7.0, 8.0, "black", "white", 'i', "Ola");
    double x, y;
    bool ok = getAncoraPorId(F, 5, &x, &y);
    TEST_ASSERT_TRUE_MESSAGE(ok, "Deve encontrar a ancora.");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(7.0, x, "Ancora x do texto deve ser 7.0.");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(8.0, y, "Ancora y do texto deve ser 8.0.");
}

void test_getAncora_IdInexistente(void)
{
    double x, y;
    bool ok = getAncoraPorId(F, 99, &x, &y);
    TEST_ASSERT_FALSE_MESSAGE(ok, "Id inexistente deve retornar false.");
}

/* ─────────────────────────────────────────────
   Iteração
   ───────────────────────────────────────────── */

void test_Iteracao_OrdemInsercao(void)
{
    insereCirculo(F,   1, 0.0, 0.0, 5.0,  "black", "red");
    insereRetangulo(F, 2, 0.0, 0.0, 10.0, 10.0, "black", "blue");
    insereLinha(F,     3, 0.0, 0.0, 10.0, 10.0, "green");

    PosicForma p = getPrimeiraForma(F);
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, getIdForma(F, p), "Primeira deve ser id 1.");
    p = getProximaForma(F, p);
    TEST_ASSERT_EQUAL_INT_MESSAGE(2, getIdForma(F, p), "Segunda deve ser id 2.");
    p = getProximaForma(F, p);
    TEST_ASSERT_EQUAL_INT_MESSAGE(3, getIdForma(F, p), "Terceira deve ser id 3.");
    p = getProximaForma(F, p);
    TEST_ASSERT_NULL_MESSAGE(p, "Depois da ultima deve ser NULL.");
}

/* ─────────────────────────────────────────────
   selecionaFormas
   ───────────────────────────────────────────── */

void test_seleciona_FormasNaRegiao(void)
{
    // Circulo em (50,50) r=5 -> totalmente dentro de (0,0,200,200)
    insereCirculo(F,   1, 50.0, 50.0,  5.0, "black", "red");
    // Retangulo (10,10) w=20 h=20 -> totalmente dentro
    insereRetangulo(F, 2, 10.0, 10.0, 20.0, 20.0, "black", "blue");
    // Circulo em (500,500) -> fora
    insereCirculo(F,   3, 500.0, 500.0, 5.0, "black", "red");

    selecionaFormas(F, 0.0, 0.0, 200.0, 200.0);

    TEST_ASSERT_EQUAL_INT_MESSAGE(2, totalFormasSelecionadas(F), "Deve ter 2 selecionadas.");
    TEST_ASSERT_TRUE_MESSAGE(isFormaSelecionada(F, buscaFormaPorId(F, 1)), "Id 1 deve estar selecionado.");
    TEST_ASSERT_TRUE_MESSAGE(isFormaSelecionada(F, buscaFormaPorId(F, 2)), "Id 2 deve estar selecionado.");
    TEST_ASSERT_FALSE_MESSAGE(isFormaSelecionada(F, buscaFormaPorId(F, 3)), "Id 3 nao deve estar selecionado.");
}

void test_seleciona_DescartaSelecaoAnterior(void)
{
    insereCirculo(F, 1, 50.0, 50.0, 5.0, "black", "red");
    insereCirculo(F, 2, 500.0, 500.0, 5.0, "black", "red");

    selecionaFormas(F, 0.0, 0.0, 200.0, 200.0);
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, totalFormasSelecionadas(F), "Deve ter 1 selecionada.");

    // Nova selecao deve descartar a anterior
    selecionaFormas(F, 400.0, 400.0, 200.0, 200.0);
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, totalFormasSelecionadas(F), "Deve ter 1 selecionada na nova selecao.");
    TEST_ASSERT_FALSE_MESSAGE(isFormaSelecionada(F, buscaFormaPorId(F, 1)), "Id 1 nao deve estar selecionado.");
    TEST_ASSERT_TRUE_MESSAGE(isFormaSelecionada(F, buscaFormaPorId(F, 2)),  "Id 2 deve estar selecionado.");
}

void test_seleciona_NenhumaForma(void)
{
    insereCirculo(F, 1, 500.0, 500.0, 5.0, "black", "red");
    selecionaFormas(F, 0.0, 0.0, 10.0, 10.0);
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, totalFormasSelecionadas(F), "Nenhuma deve ser selecionada.");
}

/* ─────────────────────────────────────────────
   removeFormasSelecionadas (dels)
   ───────────────────────────────────────────── */

void test_dels_RemoveSelecionadas(void)
{
    insereCirculo(F,   1, 50.0,  50.0,  5.0,  "black", "red");
    insereRetangulo(F, 2, 10.0,  10.0,  20.0, 20.0, "black", "blue");
    insereCirculo(F,   3, 500.0, 500.0, 5.0,  "black", "red");

    selecionaFormas(F, 0.0, 0.0, 200.0, 200.0);
    removeFormasSelecionadas(F);

    TEST_ASSERT_EQUAL_INT_MESSAGE(1, totalFormas(F), "Deve restar 1 forma.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, totalFormasSelecionadas(F), "Nenhuma deve estar selecionada.");
    TEST_ASSERT_NULL_MESSAGE(buscaFormaPorId(F, 1), "Id 1 deve ter sido removido.");
    TEST_ASSERT_NULL_MESSAGE(buscaFormaPorId(F, 2), "Id 2 deve ter sido removido.");
    TEST_ASSERT_NOT_NULL_MESSAGE(buscaFormaPorId(F, 3), "Id 3 deve continuar no banco.");
}

void test_dels_SemSelecao(void)
{
    insereCirculo(F, 1, 50.0, 50.0, 5.0, "black", "red");
    removeFormasSelecionadas(F); // Nenhuma selecionada, nao deve crashar
    TEST_ASSERT_EQUAL_INT_MESSAGE(1, totalFormas(F), "Total nao deve mudar.");
}

/* ─────────────────────────────────────────────
   modificaFormasSelecionadas (mcs)
   ───────────────────────────────────────────── */

void test_mcs_TransladaERecolore(void)
{
    insereCirculo(F,   1, 50.0, 50.0, 5.0,  "black", "red");
    insereRetangulo(F, 2, 10.0, 10.0, 20.0, 20.0, "black", "blue");

    selecionaFormas(F, 0.0, 0.0, 200.0, 200.0);
    modificaFormasSelecionadas(F, 10.0, 20.0, "green", "yellow");

    // Verifica circulo
    Circulo c = (Circulo)getDadosForma(F, buscaFormaPorId(F, 1));
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(60.0,    getXCirculo(c),    "X do circulo deve ser 60.0.");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(70.0,    getYCirculo(c),    "Y do circulo deve ser 70.0.");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("green",  getCorbCirculo(c), "Corb do circulo deve ser green.");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("yellow", getCorpCirculo(c), "Corp do circulo deve ser yellow.");

    // Verifica retangulo
    Retangulo r = (Retangulo)getDadosForma(F, buscaFormaPorId(F, 2));
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(20.0,    getXRetangulo(r),    "X do retangulo deve ser 20.0.");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(30.0,    getYRetangulo(r),    "Y do retangulo deve ser 30.0.");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("green",  getCorbRetangulo(r), "Corb do retangulo deve ser green.");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("yellow", getCorpRetangulo(r), "Corp do retangulo deve ser yellow.");
}

void test_mcs_NaoAfetaNaoSelecionadas(void)
{
    insereCirculo(F, 1, 50.0,  50.0,  5.0, "black", "red");
    insereCirculo(F, 2, 500.0, 500.0, 5.0, "black", "red");

    selecionaFormas(F, 0.0, 0.0, 200.0, 200.0);
    modificaFormasSelecionadas(F, 10.0, 10.0, "green", "yellow");

    // Id 2 nao deve ter mudado
    Circulo c2 = (Circulo)getDadosForma(F, buscaFormaPorId(F, 2));
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(500.0,   getXCirculo(c2),    "X do circulo 2 nao deve mudar.");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("black",  getCorbCirculo(c2), "Corb do circulo 2 nao deve mudar.");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("red",    getCorpCirculo(c2), "Corp do circulo 2 nao deve mudar.");
}

/* ─────────────────────────────────────────────
   Defensivos
   ───────────────────────────────────────────── */

void test_Defensivos_BancoNulo(void)
{
    TEST_ASSERT_EQUAL_INT_MESSAGE(0,    totalFormas(NULL),              "totalFormas(NULL) deve ser 0.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0,    totalFormasSelecionadas(NULL),  "totalSel(NULL) deve ser 0.");
    TEST_ASSERT_NULL_MESSAGE(getPrimeiraForma(NULL),                    "getPrimeira(NULL) deve ser NULL.");
    TEST_ASSERT_NULL_MESSAGE(buscaFormaPorId(NULL, 1),                  "busca(NULL) deve ser NULL.");
    TEST_ASSERT_NULL_MESSAGE(getTipoForma(NULL, NULL),                  "getTipo(NULL,NULL) deve ser NULL.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(-1,   getIdForma(NULL, NULL),         "getId(NULL,NULL) deve ser -1.");
    destroiFormas(NULL); // nao deve crashar
}

/* ─────────────────────────────────────────────
   main
   ───────────────────────────────────────────── */

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_criaFormas_Valido);

    RUN_TEST(test_insereCirculo_Valido);
    RUN_TEST(test_insereRetangulo_Valido);
    RUN_TEST(test_insereLinha_Valido);
    RUN_TEST(test_insereTexto_Valido);
    RUN_TEST(test_insere_TodasAsFormas);
    RUN_TEST(test_insere_FormaNula);

    RUN_TEST(test_buscaFormaPorId_Encontra);
    RUN_TEST(test_buscaFormaPorId_NaoEncontra);
    RUN_TEST(test_getTipoForma);
    RUN_TEST(test_getDadosForma);

    RUN_TEST(test_getAncora_Circulo);
    RUN_TEST(test_getAncora_Retangulo);
    RUN_TEST(test_getAncora_Linha_MenorX);
    RUN_TEST(test_getAncora_Linha_Empate);
    RUN_TEST(test_getAncora_Texto);
    RUN_TEST(test_getAncora_IdInexistente);

    RUN_TEST(test_Iteracao_OrdemInsercao);

    RUN_TEST(test_seleciona_FormasNaRegiao);
    RUN_TEST(test_seleciona_DescartaSelecaoAnterior);
    RUN_TEST(test_seleciona_NenhumaForma);

    RUN_TEST(test_dels_RemoveSelecionadas);
    RUN_TEST(test_dels_SemSelecao);

    RUN_TEST(test_mcs_TransladaERecolore);
    RUN_TEST(test_mcs_NaoAfetaNaoSelecionadas);

    RUN_TEST(test_Defensivos_BancoNulo);

    return UNITY_END();
}