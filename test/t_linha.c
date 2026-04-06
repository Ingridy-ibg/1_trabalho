#include "unity.h"
#include "../src/linha.h"

Linha L;

void setUp(void)
{
    L = criaLinha(1, 10.0, 20.0, 50.0, 70.0, "black");
}

void tearDown(void)
{
    destroiLinha(L);
}

/* ─────────────────────────────────────────────
   criaLinha
   ───────────────────────────────────────────── */

void test_criaLinha_Valido(void)
{
    TEST_ASSERT_NOT_NULL_MESSAGE(L, "Linha criada nao deve ser NULL.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(1,     getIdLinha(L),   "ID deve ser 1.");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(10.0,  getX1Linha(L), "X1 deve ser 10.0.");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(20.0,  getY1Linha(L), "Y1 deve ser 20.0.");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(50.0,  getX2Linha(L), "X2 deve ser 50.0.");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(70.0,  getY2Linha(L), "Y2 deve ser 70.0.");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("black", getCorLinha(L), "Cor deve ser black.");
}

void test_criaLinha_CorNula(void)
{
    Linha l = criaLinha(2, 0.0, 0.0, 10.0, 10.0, NULL);
    TEST_ASSERT_NULL_MESSAGE(l, "Criar linha com cor NULL deve retornar NULL.");
}

void test_criaLinha_IdInvalido(void)
{
    Linha l = criaLinha(0, 0.0, 0.0, 10.0, 10.0, "red");
    TEST_ASSERT_NULL_MESSAGE(l, "ID 0 deve retornar NULL.");
}

/* ─────────────────────────────────────────────
   getters com NULL
   ───────────────────────────────────────────── */

void test_Getters_LinhaNulo(void)
{
    TEST_ASSERT_EQUAL_INT_MESSAGE(-1,   getIdLinha(NULL),   "getId(NULL) deve retornar -1.");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(0.0, getX1Linha(NULL),  "getX1(NULL) deve retornar 0.0.");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(0.0, getY1Linha(NULL),  "getY1(NULL) deve retornar 0.0.");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(0.0, getX2Linha(NULL),  "getX2(NULL) deve retornar 0.0.");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(0.0, getY2Linha(NULL),  "getY2(NULL) deve retornar 0.0.");
    TEST_ASSERT_NULL_MESSAGE(getCorLinha(NULL), "getCor(NULL) deve retornar NULL.");
}

/* ─────────────────────────────────────────────
   transladaLinha
   ───────────────────────────────────────────── */

void test_translada_DeslocamentoPositivo(void)
{
    transladaLinha(L, 5.0, 10.0);

    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(15.0, getX1Linha(L), "X1 apos translada deve ser 15.0.");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(30.0, getY1Linha(L), "Y1 apos translada deve ser 30.0.");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(55.0, getX2Linha(L), "X2 apos translada deve ser 55.0.");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(80.0, getY2Linha(L), "Y2 apos translada deve ser 80.0.");
}

void test_translada_DeslocamentoNegativo(void)
{
    transladaLinha(L, -5.0, -10.0);

    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(5.0,  getX1Linha(L), "X1 apos translada negativa deve ser 5.0.");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(10.0, getY1Linha(L), "Y1 apos translada negativa deve ser 10.0.");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(45.0, getX2Linha(L), "X2 apos translada negativa deve ser 45.0.");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(60.0, getY2Linha(L), "Y2 apos translada negativa deve ser 60.0.");
}

void test_translada_DeslocamentoZero(void)
{
    transladaLinha(L, 0.0, 0.0);

    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(10.0, getX1Linha(L), "X1 nao deve mudar com dx=0.");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(20.0, getY1Linha(L), "Y1 nao deve mudar com dy=0.");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(50.0, getX2Linha(L), "X2 nao deve mudar com dx=0.");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(70.0, getY2Linha(L), "Y2 nao deve mudar com dy=0.");
}

void test_translada_LinhaNulo(void)
{
    // Nao deve crashar
    transladaLinha(NULL, 5.0, 5.0);
    TEST_ASSERT_TRUE(true);
}

/* ─────────────────────────────────────────────
   setCorLinha
   ───────────────────────────────────────────── */

void test_setCor_Valido(void)
{
    setCorLinha(L, "red");

    TEST_ASSERT_EQUAL_STRING_MESSAGE("red", getCorLinha(L), "Cor deve ser red.");
}

void test_setCor_MesmaCor(void)
{
    setCorLinha(L, "black");

    TEST_ASSERT_EQUAL_STRING_MESSAGE("black", getCorLinha(L), "Cor deve permanecer black.");
}

void test_setCor_CorNula(void)
{
    const char *corOriginal = getCorLinha(L);

    setCorLinha(L, NULL);
    TEST_ASSERT_EQUAL_STRING_MESSAGE(corOriginal, getCorLinha(L), "Cor nao deve mudar se cor=NULL.");
}

void test_StringsDuplicadas(void)
{
    char cor[] = "red";
    Linha l = criaLinha(10, 0.0, 0.0, 10.0, 10.0, cor);

    // Modificar a string original
    cor[0] = 'g'; // "red" -> "ged"

    TEST_ASSERT_EQUAL_STRING_MESSAGE("red", getCorLinha(l), "String deve ser duplicada, nao compartilhada.");

    destroiLinha(l);
}

/* ─────────────────────────────────────────────
   contemPontoLinha
   ───────────────────────────────────────────── */
/*
 * Linha L: (10,20) a (50,70)
 * Equacao parametrica: x = 10 + 40t, y = 20 + 50t, t em [0,1]
 */

void test_contemPonto_Dentro(void)
{
    TEST_ASSERT_TRUE_MESSAGE(contemPontoLinha(L, 30.0, 45.0),
        "Ponto (30, 45) deve estar na linha.");
}

void test_contemPonto_NasPontas(void)
{
    TEST_ASSERT_TRUE_MESSAGE(contemPontoLinha(L, 10.0, 20.0),
        "Ponto inicial deve estar na linha.");
    TEST_ASSERT_TRUE_MESSAGE(contemPontoLinha(L, 50.0, 70.0),
        "Ponto final deve estar na linha.");
}

void test_contemPonto_Fora(void)
{
    TEST_ASSERT_FALSE_MESSAGE(contemPontoLinha(L, 30.0, 46.0),
        "Ponto fora da linha nao deve estar dentro.");
    TEST_ASSERT_FALSE_MESSAGE(contemPontoLinha(L, 5.0, 15.0),
        "Ponto antes do inicio nao deve estar dentro.");
    TEST_ASSERT_FALSE_MESSAGE(contemPontoLinha(L, 55.0, 75.0),
        "Ponto apos o fim nao deve estar dentro.");
}

void test_contemPonto_LinhaNulo(void)
{
    TEST_ASSERT_FALSE_MESSAGE(contemPontoLinha(NULL, 30.0, 45.0),
        "NULL deve retornar false.");
}

/* ─────────────────────────────────────────────
   dentroRegiaoLinha
   ───────────────────────────────────────────── */
/*
 * Linha L: (10,20) a (50,70)
 */

void test_dentroRegiao_TotalmenteContido(void)
{
    // Regiao maior que a linha
    TEST_ASSERT_TRUE_MESSAGE(dentroRegiaoLinha(L, 0.0, 0.0, 200.0, 200.0),
        "Linha deve estar dentro de uma regiao maior.");
}

void test_dentroRegiao_ExatamenteIgual(void)
{
    // Regiao que contem exatamente a linha
    TEST_ASSERT_TRUE_MESSAGE(dentroRegiaoLinha(L, 10.0, 20.0, 40.0, 50.0),
        "Linha deve estar dentro de uma regiao que a contem exatamente.");
}

void test_dentroRegiao_Fora(void)
{
    // Regiao menor — linha nao cabe inteiramente
    TEST_ASSERT_FALSE_MESSAGE(dentroRegiaoLinha(L, 15.0, 20.0, 40.0, 50.0),
        "Linha nao deve caber em regiao que corta o inicio.");
    TEST_ASSERT_FALSE_MESSAGE(dentroRegiaoLinha(L, 10.0, 25.0, 40.0, 50.0),
        "Linha nao deve caber em regiao que corta abaixo.");
    TEST_ASSERT_FALSE_MESSAGE(dentroRegiaoLinha(L, 10.0, 20.0, 35.0, 50.0),
        "Linha nao deve caber em regiao que corta o fim.");
    TEST_ASSERT_FALSE_MESSAGE(dentroRegiaoLinha(L, 10.0, 20.0, 40.0, 45.0),
        "Linha nao deve caber em regiao que corta acima.");
}

void test_dentroRegiao_RegiaoZero(void)
{
    // Regiao com largura zero
    TEST_ASSERT_FALSE_MESSAGE(dentroRegiaoLinha(L, 10.0, 20.0, 0.0, 50.0),
        "Linha nao deve caber em regiao com w=0.");
    // Regiao com altura zero
    TEST_ASSERT_FALSE_MESSAGE(dentroRegiaoLinha(L, 10.0, 20.0, 40.0, 0.0),
        "Linha nao deve caber em regiao com h=0.");
}

void test_dentroRegiao_LinhaNulo(void)
{
    TEST_ASSERT_FALSE_MESSAGE(dentroRegiaoLinha(NULL, 0.0, 0.0, 200.0, 200.0),
        "NULL deve retornar false.");
}

/* ─────────────────────────────────────────────
   main
   ───────────────────────────────────────────── */

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_criaLinha_Valido);
    RUN_TEST(test_criaLinha_CorNula);
    RUN_TEST(test_criaLinha_IdInvalido);

    RUN_TEST(test_Getters_LinhaNulo);

    RUN_TEST(test_translada_DeslocamentoPositivo);
    RUN_TEST(test_translada_DeslocamentoNegativo);
    RUN_TEST(test_translada_DeslocamentoZero);
    RUN_TEST(test_translada_LinhaNulo);

    RUN_TEST(test_setCor_Valido);
    RUN_TEST(test_setCor_MesmaCor);
    RUN_TEST(test_setCor_CorNula);
    RUN_TEST(test_StringsDuplicadas);

    RUN_TEST(test_contemPonto_Dentro);
    RUN_TEST(test_contemPonto_NasPontas);
    RUN_TEST(test_contemPonto_Fora);
    RUN_TEST(test_contemPonto_LinhaNulo);

    RUN_TEST(test_dentroRegiao_TotalmenteContido);
    RUN_TEST(test_dentroRegiao_ExatamenteIgual);
    RUN_TEST(test_dentroRegiao_Fora);
    RUN_TEST(test_dentroRegiao_RegiaoZero);
    RUN_TEST(test_dentroRegiao_LinhaNulo);

    return UNITY_END();
}