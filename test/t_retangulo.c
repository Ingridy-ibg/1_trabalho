#include "unity.h"
#include "../src/retangulo.h"

Retangulo R;

void setUp(void)
{
    R = criaRetangulo(1, 10.0, 20.0, 100.0, 50.0, "black", "blue");
}

void tearDown(void)
{
    destroiRetangulo(R);
}

/* ─────────────────────────────────────────────
   criaRetangulo
   ───────────────────────────────────────────── */

void test_criaRetangulo_Valido(void)
{
    TEST_ASSERT_NOT_NULL_MESSAGE(R, "Retangulo criado nao deve ser NULL.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(1,     getIdRetangulo(R),   "ID deve ser 1.");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(10.0,  getXRetangulo(R), "X deve ser 10.0.");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(20.0,  getYRetangulo(R), "Y deve ser 20.0.");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(100.0, getWRetangulo(R), "W deve ser 100.0.");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(50.0,  getHRetangulo(R), "H deve ser 50.0.");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("black", getCorbRetangulo(R), "Cor da borda deve ser black.");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("blue",  getCorpRetangulo(R), "Cor do preenchimento deve ser blue.");
}

void test_criaRetangulo_Nulo_CorNula(void)
{
    Retangulo r = criaRetangulo(2, 0.0, 0.0, 10.0, 10.0, NULL, NULL);
    TEST_ASSERT_NULL_MESSAGE(r, "Criar retangulo com cores NULL deve retornar NULL.");
}

void test_criaRetangulo_DimensaoInvalida(void)
{
    Retangulo r1 = criaRetangulo(2, 0.0, 0.0, 0.0,  10.0, "red", "green");
    Retangulo r2 = criaRetangulo(3, 0.0, 0.0, 10.0,  0.0, "red", "green");
    Retangulo r3 = criaRetangulo(4, 0.0, 0.0, -5.0, 10.0, "red", "green");
    Retangulo r4 = criaRetangulo(5, 0.0, 0.0, 10.0, -5.0, "red", "green");

    TEST_ASSERT_NULL_MESSAGE(r1, "w=0 deve retornar NULL.");
    TEST_ASSERT_NULL_MESSAGE(r2, "h=0 deve retornar NULL.");
    TEST_ASSERT_NULL_MESSAGE(r3, "w negativo deve retornar NULL.");
    TEST_ASSERT_NULL_MESSAGE(r4, "h negativo deve retornar NULL.");
}

void test_criaRetangulo_IdInvalido(void)
{
    Retangulo r = criaRetangulo(0, 0.0, 0.0, 10.0, 10.0, "red", "green");
    TEST_ASSERT_NULL_MESSAGE(r, "ID 0 deve retornar NULL.");
}

/* ─────────────────────────────────────────────
   getters com NULL
   ───────────────────────────────────────────── */

void test_Getters_RetanguloNulo(void)
{
    TEST_ASSERT_EQUAL_INT_MESSAGE(-1,   getIdRetangulo(NULL),   "getId(NULL) deve retornar -1.");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(0.0, getXRetangulo(NULL),  "getX(NULL) deve retornar 0.0.");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(0.0, getYRetangulo(NULL),  "getY(NULL) deve retornar 0.0.");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(0.0, getWRetangulo(NULL),  "getW(NULL) deve retornar 0.0.");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(0.0, getHRetangulo(NULL),  "getH(NULL) deve retornar 0.0.");
    TEST_ASSERT_NULL_MESSAGE(getCorbRetangulo(NULL), "getCorb(NULL) deve retornar NULL.");
    TEST_ASSERT_NULL_MESSAGE(getCorpRetangulo(NULL), "getCorp(NULL) deve retornar NULL.");
}

/* ─────────────────────────────────────────────
   transladaRetangulo
   ───────────────────────────────────────────── */

void test_translada_DeslocamentoPositivo(void)
{
    transladaRetangulo(R, 5.0, 10.0);

    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(15.0, getXRetangulo(R), "X apos translada deve ser 15.0.");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(30.0, getYRetangulo(R), "Y apos translada deve ser 30.0.");
}

void test_translada_DeslocamentoNegativo(void)
{
    transladaRetangulo(R, -5.0, -10.0);

    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(5.0,  getXRetangulo(R), "X apos translada negativa deve ser 5.0.");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(10.0, getYRetangulo(R), "Y apos translada negativa deve ser 10.0.");
}

void test_translada_DeslocamentoZero(void)
{
    transladaRetangulo(R, 0.0, 0.0);

    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(10.0, getXRetangulo(R), "X nao deve mudar com dx=0.");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(20.0, getYRetangulo(R), "Y nao deve mudar com dy=0.");
}

void test_translada_NaoAlteraDimensoes(void)
{
    transladaRetangulo(R, 99.0, 99.0);

    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(100.0, getWRetangulo(R), "W nao deve mudar apos translada.");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(50.0,  getHRetangulo(R), "H nao deve mudar apos translada.");
}

void test_translada_RetanguloNulo(void)
{
    // Nao deve crashar
    transladaRetangulo(NULL, 5.0, 5.0);
    TEST_ASSERT_TRUE(true);
}

/* ─────────────────────────────────────────────
   setCoresRetangulo
   ───────────────────────────────────────────── */

void test_setCores_Valido(void)
{
    setCoresRetangulo(R, "red", "yellow");

    TEST_ASSERT_EQUAL_STRING_MESSAGE("red",    getCorbRetangulo(R), "Cor da borda deve ser red.");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("yellow", getCorpRetangulo(R), "Cor do preenchimento deve ser yellow.");
}

void test_setCores_MesmaCor(void)
{
    setCoresRetangulo(R, "black", "blue");

    TEST_ASSERT_EQUAL_STRING_MESSAGE("black", getCorbRetangulo(R), "Cor da borda deve permanecer black.");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("blue",  getCorpRetangulo(R), "Cor do preenchimento deve permanecer blue.");
}

void test_setCores_UmaCorNula(void)
{
    const char *corbOriginal = getCorbRetangulo(R);
    const char *corpOriginal = getCorpRetangulo(R);

    setCoresRetangulo(R, NULL, "yellow");
    TEST_ASSERT_EQUAL_STRING_MESSAGE(corbOriginal, getCorbRetangulo(R), "Cor da borda nao deve mudar se corb=NULL.");
    TEST_ASSERT_EQUAL_STRING_MESSAGE(corpOriginal, getCorpRetangulo(R), "Cor do preenchimento nao deve mudar se corb=NULL.");

    setCoresRetangulo(R, "red", NULL);
    TEST_ASSERT_EQUAL_STRING_MESSAGE(corbOriginal, getCorbRetangulo(R), "Cor da borda nao deve mudar se corp=NULL.");
    TEST_ASSERT_EQUAL_STRING_MESSAGE(corpOriginal, getCorpRetangulo(R), "Cor do preenchimento nao deve mudar se corp=NULL.");
}

void test_StringsDuplicadas(void)
{
    char corb[] = "red";
    char corp[] = "blue";
    Retangulo r = criaRetangulo(10, 0.0, 0.0, 10.0, 10.0, corb, corp);

    // Modificar as strings originais
    corb[0] = 'g'; // "red" -> "ged"
    corp[0] = 'y'; // "blue" -> "ylue"

    TEST_ASSERT_EQUAL_STRING_MESSAGE("red", getCorbRetangulo(r), "Strings devem ser duplicadas, nao compartilhadas.");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("blue", getCorpRetangulo(r), "Strings devem ser duplicadas, nao compartilhadas.");

    destroiRetangulo(r);
}

/* ─────────────────────────────────────────────
   contemPontoRetangulo
   ───────────────────────────────────────────── */
/*
 * Retangulo R: ancora (10, 20), w=100, h=50
 * Ocupa a regiao: x em [10, 110], y em [20, 70]
 */

void test_contemPonto_Dentro(void)
{
    TEST_ASSERT_TRUE_MESSAGE(contemPontoRetangulo(R, 50.0, 40.0),
        "Ponto (50, 40) deve estar dentro.");
}

void test_contemPonto_NasBordas(void)
{
    TEST_ASSERT_TRUE_MESSAGE(contemPontoRetangulo(R, 10.0, 20.0),
        "Canto inferior esquerdo deve estar dentro.");
    TEST_ASSERT_TRUE_MESSAGE(contemPontoRetangulo(R, 110.0, 70.0),
        "Canto superior direito deve estar dentro.");
    TEST_ASSERT_TRUE_MESSAGE(contemPontoRetangulo(R, 10.0, 70.0),
        "Canto superior esquerdo deve estar dentro.");
    TEST_ASSERT_TRUE_MESSAGE(contemPontoRetangulo(R, 110.0, 20.0),
        "Canto inferior direito deve estar dentro.");
}

void test_contemPonto_Fora(void)
{
    TEST_ASSERT_FALSE_MESSAGE(contemPontoRetangulo(R, 5.0, 40.0),
        "Ponto a esquerda nao deve estar dentro.");
    TEST_ASSERT_FALSE_MESSAGE(contemPontoRetangulo(R, 120.0, 40.0),
        "Ponto a direita nao deve estar dentro.");
    TEST_ASSERT_FALSE_MESSAGE(contemPontoRetangulo(R, 50.0, 10.0),
        "Ponto abaixo nao deve estar dentro.");
    TEST_ASSERT_FALSE_MESSAGE(contemPontoRetangulo(R, 50.0, 80.0),
        "Ponto acima nao deve estar dentro.");
}

void test_contemPonto_RetanguloNulo(void)
{
    TEST_ASSERT_FALSE_MESSAGE(contemPontoRetangulo(NULL, 50.0, 40.0),
        "NULL deve retornar false.");
}

/* ─────────────────────────────────────────────
   dentroRegiaoRetangulo
   ───────────────────────────────────────────── */
/*
 * Retangulo R: ancora (10, 20), w=100, h=50
 * Ocupa a regiao: x em [10, 110], y em [20, 70]
 */

void test_dentroRegiao_TotalmenteContido(void)
{
    // Regiao maior que o retangulo
    TEST_ASSERT_TRUE_MESSAGE(dentroRegiaoRetangulo(R, 0.0, 0.0, 200.0, 200.0),
        "Retangulo deve estar dentro de uma regiao maior.");
}

void test_dentroRegiao_ExatamenteIgual(void)
{
    // Regiao com exatamente as mesmas dimensoes e posicao
    TEST_ASSERT_TRUE_MESSAGE(dentroRegiaoRetangulo(R, 10.0, 20.0, 100.0, 50.0),
        "Retangulo deve estar dentro de uma regiao identica.");
}

void test_dentroRegiao_Fora(void)
{
    // Regiao menor — retangulo nao cabe inteiramente
    TEST_ASSERT_FALSE_MESSAGE(dentroRegiaoRetangulo(R, 15.0, 20.0, 100.0, 50.0),
        "Retangulo nao deve caber em regiao que corta a esquerda.");
    TEST_ASSERT_FALSE_MESSAGE(dentroRegiaoRetangulo(R, 10.0, 25.0, 100.0, 50.0),
        "Retangulo nao deve caber em regiao que corta abaixo.");
    TEST_ASSERT_FALSE_MESSAGE(dentroRegiaoRetangulo(R, 10.0, 20.0, 90.0, 50.0),
        "Retangulo nao deve caber em regiao que corta a direita.");
    TEST_ASSERT_FALSE_MESSAGE(dentroRegiaoRetangulo(R, 10.0, 20.0, 100.0, 40.0),
        "Retangulo nao deve caber em regiao que corta acima.");
}

void test_dentroRegiao_RetanguloNulo(void)
{
    TEST_ASSERT_FALSE_MESSAGE(dentroRegiaoRetangulo(NULL, 0.0, 0.0, 200.0, 200.0),
        "NULL deve retornar false.");
}

void test_dentroRegiao_RegiaoZero(void)
{
    // Regiao com largura zero
    TEST_ASSERT_FALSE_MESSAGE(dentroRegiaoRetangulo(R, 10.0, 20.0, 0.0, 50.0),
        "Retangulo nao deve caber em regiao com w=0.");
    // Regiao com altura zero
    TEST_ASSERT_FALSE_MESSAGE(dentroRegiaoRetangulo(R, 10.0, 20.0, 100.0, 0.0),
        "Retangulo nao deve caber em regiao com h=0.");
}

/* ─────────────────────────────────────────────
   main
   ───────────────────────────────────────────── */

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_criaRetangulo_Valido);
    RUN_TEST(test_criaRetangulo_Nulo_CorNula);
    RUN_TEST(test_criaRetangulo_DimensaoInvalida);
    RUN_TEST(test_criaRetangulo_IdInvalido);

    RUN_TEST(test_Getters_RetanguloNulo);

    RUN_TEST(test_translada_DeslocamentoPositivo);
    RUN_TEST(test_translada_DeslocamentoNegativo);
    RUN_TEST(test_translada_DeslocamentoZero);
    RUN_TEST(test_translada_NaoAlteraDimensoes);
    RUN_TEST(test_translada_RetanguloNulo);

    RUN_TEST(test_setCores_Valido);
    RUN_TEST(test_setCores_MesmaCor);
    RUN_TEST(test_setCores_UmaCorNula);
   

    RUN_TEST(test_StringsDuplicadas);

    RUN_TEST(test_contemPonto_Dentro);
    RUN_TEST(test_contemPonto_NasBordas);
    RUN_TEST(test_contemPonto_Fora);
    RUN_TEST(test_contemPonto_RetanguloNulo);

    RUN_TEST(test_dentroRegiao_TotalmenteContido);
    RUN_TEST(test_dentroRegiao_ExatamenteIgual);
    RUN_TEST(test_dentroRegiao_Fora);
    RUN_TEST(test_dentroRegiao_RegiaoZero);
    RUN_TEST(test_dentroRegiao_RetanguloNulo);

    return UNITY_END();
}