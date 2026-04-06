#include "unity.h"
#include "../src/circulo.h"

Circulo C;

void setUp(void)
{
    C = criaCirculo(1, 10.0, 20.0, 5.0, "black", "blue");
}

void tearDown(void)
{
    destroiCirculo(C);
}

/* ─────────────────────────────────────────────
   criaCirculo
   ───────────────────────────────────────────── */

void test_criaCirculo_Valido(void)
{
    TEST_ASSERT_NOT_NULL_MESSAGE(C, "Circulo criado nao deve ser NULL.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(1,     getIdCirculo(C),   "ID deve ser 1.");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(10.0,  getXCirculo(C), "X deve ser 10.0.");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(20.0,  getYCirculo(C), "Y deve ser 20.0.");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(5.0,   getRCirculo(C), "R deve ser 5.0.");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("black", getCorbCirculo(C), "Cor da borda deve ser black.");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("blue",  getCorpCirculo(C), "Cor do preenchimento deve ser blue.");
}

void test_criaCirculo_Nulo_CorNula(void)
{
    Circulo c = criaCirculo(2, 0.0, 0.0, 10.0, NULL, NULL);
    TEST_ASSERT_NULL_MESSAGE(c, "Criar circulo com cores NULL deve retornar NULL.");
}

void test_criaCirculo_DimensaoInvalida(void)
{
    Circulo c1 = criaCirculo(2, 0.0, 0.0, 0.0, "red", "green");
    Circulo c2 = criaCirculo(3, 0.0, 0.0, -5.0, "red", "green");

    TEST_ASSERT_NULL_MESSAGE(c1, "r=0 deve retornar NULL.");
    TEST_ASSERT_NULL_MESSAGE(c2, "r negativo deve retornar NULL.");
}

void test_criaCirculo_IdInvalido(void)
{
    Circulo c = criaCirculo(0, 0.0, 0.0, 10.0, "red", "green");
    TEST_ASSERT_NULL_MESSAGE(c, "ID 0 deve retornar NULL.");
}

/* ─────────────────────────────────────────────
   getters com NULL
   ───────────────────────────────────────────── */

void test_Getters_CirculoNulo(void)
{
    TEST_ASSERT_EQUAL_INT_MESSAGE(-1,   getIdCirculo(NULL),   "getId(NULL) deve retornar -1.");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(0.0, getXCirculo(NULL),  "getX(NULL) deve retornar 0.0.");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(0.0, getYCirculo(NULL),  "getY(NULL) deve retornar 0.0.");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(0.0, getRCirculo(NULL),  "getR(NULL) deve retornar 0.0.");
    TEST_ASSERT_NULL_MESSAGE(getCorbCirculo(NULL), "getCorb(NULL) deve retornar NULL.");
    TEST_ASSERT_NULL_MESSAGE(getCorpCirculo(NULL), "getCorp(NULL) deve retornar NULL.");
}

/* ─────────────────────────────────────────────
   transladaCirculo
   ───────────────────────────────────────────── */

void test_translada_DeslocamentoPositivo(void)
{
    transladaCirculo(C, 5.0, 10.0);

    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(15.0, getXCirculo(C), "X apos translada deve ser 15.0.");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(30.0, getYCirculo(C), "Y apos translada deve ser 30.0.");
}

void test_translada_DeslocamentoNegativo(void)
{
    transladaCirculo(C, -5.0, -10.0);

    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(5.0,  getXCirculo(C), "X apos translada negativa deve ser 5.0.");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(10.0, getYCirculo(C), "Y apos translada negativa deve ser 10.0.");
}

void test_translada_DeslocamentoZero(void)
{
    transladaCirculo(C, 0.0, 0.0);

    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(10.0, getXCirculo(C), "X nao deve mudar com dx=0.");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(20.0, getYCirculo(C), "Y nao deve mudar com dy=0.");
}

void test_translada_NaoAlteraRaio(void)
{
    transladaCirculo(C, 99.0, 99.0);

    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(5.0, getRCirculo(C), "R nao deve mudar apos translada.");
}

void test_translada_CirculoNulo(void)
{
    // Nao deve crashar
    transladaCirculo(NULL, 5.0, 5.0);
    TEST_ASSERT_TRUE(true);
}

/* ─────────────────────────────────────────────
   setCoresCirculo
   ───────────────────────────────────────────── */

void test_setCores_Valido(void)
{
    setCoresCirculo(C, "red", "yellow");

    TEST_ASSERT_EQUAL_STRING_MESSAGE("red",    getCorbCirculo(C), "Cor da borda deve ser red.");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("yellow", getCorpCirculo(C), "Cor do preenchimento deve ser yellow.");
}

void test_setCores_MesmaCor(void)
{
    setCoresCirculo(C, "black", "blue");

    TEST_ASSERT_EQUAL_STRING_MESSAGE("black", getCorbCirculo(C), "Cor da borda deve permanecer black.");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("blue",  getCorpCirculo(C), "Cor do preenchimento deve permanecer blue.");
}

void test_setCores_UmaCorNula(void)
{
    const char *corbOriginal = getCorbCirculo(C);
    const char *corpOriginal = getCorpCirculo(C);

    setCoresCirculo(C, NULL, "yellow");
    TEST_ASSERT_EQUAL_STRING_MESSAGE(corbOriginal, getCorbCirculo(C), "Cor da borda nao deve mudar se corb=NULL.");
    TEST_ASSERT_EQUAL_STRING_MESSAGE(corpOriginal, getCorpCirculo(C), "Cor do preenchimento nao deve mudar se corb=NULL.");

    setCoresCirculo(C, "red", NULL);
    TEST_ASSERT_EQUAL_STRING_MESSAGE(corbOriginal, getCorbCirculo(C), "Cor da borda nao deve mudar se corp=NULL.");
    TEST_ASSERT_EQUAL_STRING_MESSAGE(corpOriginal, getCorpCirculo(C), "Cor do preenchimento nao deve mudar se corp=NULL.");
}

void test_StringsDuplicadas(void)
{
    char corb[] = "red";
    char corp[] = "blue";
    Circulo c = criaCirculo(10, 0.0, 0.0, 10.0, corb, corp);

    // Modificar as strings originais
    corb[0] = 'g'; // "red" -> "ged"
    corp[0] = 'y'; // "blue" -> "ylue"

    TEST_ASSERT_EQUAL_STRING_MESSAGE("red", getCorbCirculo(c), "Strings devem ser duplicadas, nao compartilhadas.");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("blue", getCorpCirculo(c), "Strings devem ser duplicadas, nao compartilhadas.");

    destroiCirculo(c);
}

/* ─────────────────────────────────────────────
   contemPontoCirculo
   ───────────────────────────────────────────── */
/*
 * Circulo C: centro (10, 20), r=5
 * Ocupa pontos onde distancia <= 5
 */

void test_contemPonto_Dentro(void)
{
    TEST_ASSERT_TRUE_MESSAGE(contemPontoCirculo(C, 10.0, 20.0),
        "Centro deve estar dentro.");
    TEST_ASSERT_TRUE_MESSAGE(contemPontoCirculo(C, 12.0, 20.0),
        "Ponto dentro deve estar dentro.");
}

void test_contemPonto_NasBordas(void)
{
    TEST_ASSERT_TRUE_MESSAGE(contemPontoCirculo(C, 15.0, 20.0),
        "Ponto na borda direita deve estar dentro.");
    TEST_ASSERT_TRUE_MESSAGE(contemPontoCirculo(C, 10.0, 25.0),
        "Ponto na borda superior deve estar dentro.");
    TEST_ASSERT_TRUE_MESSAGE(contemPontoCirculo(C, 5.0, 20.0),
        "Ponto na borda esquerda deve estar dentro.");
    TEST_ASSERT_TRUE_MESSAGE(contemPontoCirculo(C, 10.0, 15.0),
        "Ponto na borda inferior deve estar dentro.");
}

void test_contemPonto_Fora(void)
{
    TEST_ASSERT_FALSE_MESSAGE(contemPontoCirculo(C, 16.0, 20.0),
        "Ponto fora a direita nao deve estar dentro.");
    TEST_ASSERT_FALSE_MESSAGE(contemPontoCirculo(C, 10.0, 26.0),
        "Ponto fora acima nao deve estar dentro.");
    TEST_ASSERT_FALSE_MESSAGE(contemPontoCirculo(C, 4.0, 20.0),
        "Ponto fora a esquerda nao deve estar dentro.");
    TEST_ASSERT_FALSE_MESSAGE(contemPontoCirculo(C, 10.0, 14.0),
        "Ponto fora abaixo nao deve estar dentro.");
}

void test_contemPonto_CirculoNulo(void)
{
    TEST_ASSERT_FALSE_MESSAGE(contemPontoCirculo(NULL, 10.0, 20.0),
        "NULL deve retornar false.");
}

/* ─────────────────────────────────────────────
   dentroRegiaoCirculo
   ───────────────────────────────────────────── */
/*
 * Circulo C: centro (10, 20), r=5
 * Deve estar dentro se 10-5 >= rx, 10+5 <= rx+rw, 20-5 >= ry, 20+5 <= ry+rh
 */

void test_dentroRegiao_TotalmenteContido(void)
{
    // Regiao maior que o circulo
    TEST_ASSERT_TRUE_MESSAGE(dentroRegiaoCirculo(C, 0.0, 0.0, 200.0, 200.0),
        "Circulo deve estar dentro de uma regiao maior.");
}

void test_dentroRegiao_ExatamenteIgual(void)
{
    // Regiao que contem exatamente o circulo
    TEST_ASSERT_TRUE_MESSAGE(dentroRegiaoCirculo(C, 5.0, 15.0, 10.0, 10.0),
        "Circulo deve estar dentro de uma regiao que o contem exatamente.");
}

void test_dentroRegiao_Fora(void)
{
    // Regiao menor — circulo nao cabe inteiramente
    TEST_ASSERT_FALSE_MESSAGE(dentroRegiaoCirculo(C, 6.0, 15.0, 10.0, 10.0),
        "Circulo nao deve caber em regiao que corta a esquerda.");
    TEST_ASSERT_FALSE_MESSAGE(dentroRegiaoCirculo(C, 5.0, 16.0, 10.0, 10.0),
        "Circulo nao deve caber em regiao que corta abaixo.");
    TEST_ASSERT_FALSE_MESSAGE(dentroRegiaoCirculo(C, 5.0, 15.0, 9.0, 10.0),
        "Circulo nao deve caber em regiao que corta a direita.");
    TEST_ASSERT_FALSE_MESSAGE(dentroRegiaoCirculo(C, 5.0, 15.0, 10.0, 9.0),
        "Circulo nao deve caber em regiao que corta acima.");
}

void test_dentroRegiao_RegiaoZero(void)
{
    // Regiao com largura zero
    TEST_ASSERT_FALSE_MESSAGE(dentroRegiaoCirculo(C, 5.0, 15.0, 0.0, 10.0),
        "Circulo nao deve caber em regiao com w=0.");
    // Regiao com altura zero
    TEST_ASSERT_FALSE_MESSAGE(dentroRegiaoCirculo(C, 5.0, 15.0, 10.0, 0.0),
        "Circulo nao deve caber em regiao com h=0.");
}

void test_dentroRegiao_CirculoNulo(void)
{
    TEST_ASSERT_FALSE_MESSAGE(dentroRegiaoCirculo(NULL, 0.0, 0.0, 200.0, 200.0),
        "NULL deve retornar false.");
}

/* ─────────────────────────────────────────────
   main
   ───────────────────────────────────────────── */

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_criaCirculo_Valido);
    RUN_TEST(test_criaCirculo_Nulo_CorNula);
    RUN_TEST(test_criaCirculo_DimensaoInvalida);
    RUN_TEST(test_criaCirculo_IdInvalido);

    RUN_TEST(test_Getters_CirculoNulo);

    RUN_TEST(test_translada_DeslocamentoPositivo);
    RUN_TEST(test_translada_DeslocamentoNegativo);
    RUN_TEST(test_translada_DeslocamentoZero);
    RUN_TEST(test_translada_NaoAlteraRaio);
    RUN_TEST(test_translada_CirculoNulo);

    RUN_TEST(test_setCores_Valido);
    RUN_TEST(test_setCores_MesmaCor);
    RUN_TEST(test_setCores_UmaCorNula);
    RUN_TEST(test_StringsDuplicadas);

    RUN_TEST(test_contemPonto_Dentro);
    RUN_TEST(test_contemPonto_NasBordas);
    RUN_TEST(test_contemPonto_Fora);
    RUN_TEST(test_contemPonto_CirculoNulo);

    RUN_TEST(test_dentroRegiao_TotalmenteContido);
    RUN_TEST(test_dentroRegiao_ExatamenteIgual);
    RUN_TEST(test_dentroRegiao_Fora);
    RUN_TEST(test_dentroRegiao_RegiaoZero);
    RUN_TEST(test_dentroRegiao_CirculoNulo);

    return UNITY_END();
}