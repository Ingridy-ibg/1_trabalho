#include "unity.h"
#include "../src/texto.h"

Texto T;

void setUp(void)
{
    // Garante estilo padrão antes de cada teste
    setEstiloTexto("sans", "n", 12.0);
    T = criaTexto(1, 10.0, 20.0, "black", "red", 'i', "Hello World");
}

void tearDown(void)
{
    destroiTexto(T);
}

/* ─────────────────────────────────────────────
   setEstiloTexto
   ───────────────────────────────────────────── */

void test_setEstilo_AplicadoAoCriar(void)
{
    setEstiloTexto("serif", "b", 16.0);
    Texto t = criaTexto(2, 0.0, 0.0, "black", "blue", 'i', "Teste");

    TEST_ASSERT_EQUAL_STRING_MESSAGE("serif", getFamilyTexto(t), "Familia deve ser serif.");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("b",     getWeightTexto(t), "Peso deve ser b.");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(16.0,    getSizeTexto(t),   "Tamanho deve ser 16.0.");

    destroiTexto(t);
}

void test_setEstilo_NaoAfetaTextosJaCriados(void)
{
    // T foi criado com estilo "sans", "n", 12.0 no setUp
    setEstiloTexto("cursive", "b+", 24.0);

    // T não deve ter mudado
    TEST_ASSERT_EQUAL_STRING_MESSAGE("sans", getFamilyTexto(T), "Family de T nao deve mudar.");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("n",    getWeightTexto(T), "Weight de T nao deve mudar.");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(12.0,   getSizeTexto(T),   "Size de T nao deve mudar.");
}

void test_setEstilo_Invalido(void)
{
    // Não deve crashar nem alterar o estilo
    setEstiloTexto(NULL, "n", 12.0);
    setEstiloTexto("sans", NULL, 12.0);
    setEstiloTexto("sans", "n", 0.0);
    setEstiloTexto("sans", "n", -5.0);

    // Estilo deve continuar o anterior (sans, n, 12.0)
    Texto t = criaTexto(2, 0.0, 0.0, "black", "blue", 'i', "Teste");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("sans", getFamilyTexto(t), "Family deve permanecer sans.");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("n",    getWeightTexto(t), "Weight deve permanecer n.");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(12.0,   getSizeTexto(t),   "Size deve permanecer 12.0.");
    destroiTexto(t);
}

/* ─────────────────────────────────────────────
   criaTexto
   ───────────────────────────────────────────── */

void test_criaTexto_Valido(void)
{
    TEST_ASSERT_NOT_NULL_MESSAGE(T, "Texto criado nao deve ser NULL.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(1,       getIdTexto(T),   "ID deve ser 1.");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(10.0, getXTexto(T),    "X deve ser 10.0.");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(20.0, getYTexto(T),    "Y deve ser 20.0.");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("black", getCorbTexto(T), "Cor da borda deve ser black.");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("red",   getCorpTexto(T), "Cor do preench. deve ser red.");
    TEST_ASSERT_EQUAL_INT_MESSAGE('i',     getAncora(T),    "Ancora deve ser 'i'.");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("Hello World", getTxto(T), "Conteudo deve ser Hello World.");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("sans", getFamilyTexto(T), "Family deve ser sans.");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("n",    getWeightTexto(T), "Weight deve ser n.");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(12.0,   getSizeTexto(T),   "Size deve ser 12.0.");
}

void test_criaTexto_TodasAsAncoras(void)
{
    Texto ti = criaTexto(2, 0.0, 0.0, "black", "blue", 'i', "inicio");
    Texto tm = criaTexto(3, 0.0, 0.0, "black", "blue", 'm', "meio");
    Texto tf = criaTexto(4, 0.0, 0.0, "black", "blue", 'f', "fim");

    TEST_ASSERT_EQUAL_INT_MESSAGE('i', getAncora(ti), "Ancora deve ser 'i'.");
    TEST_ASSERT_EQUAL_INT_MESSAGE('m', getAncora(tm), "Ancora deve ser 'm'.");
    TEST_ASSERT_EQUAL_INT_MESSAGE('f', getAncora(tf), "Ancora deve ser 'f'.");

    destroiTexto(ti);
    destroiTexto(tm);
    destroiTexto(tf);
}

void test_criaTexto_ComEspacos(void)
{
    Texto t = criaTexto(2, 0.0, 0.0, "black", "blue", 'i',
                        "Texto com varios espacos e acentuacao");
    TEST_ASSERT_NOT_NULL_MESSAGE(t, "Texto com espacos deve ser criado.");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("Texto com varios espacos e acentuacao",
                                     getTxto(t), "Conteudo deve ser preservado.");
    destroiTexto(t);
}

void test_criaTexto_IdInvalido(void)
{
    Texto t = criaTexto(0, 0.0, 0.0, "black", "blue", 'i', "teste");
    TEST_ASSERT_NULL_MESSAGE(t, "ID 0 deve retornar NULL.");
}

void test_criaTexto_CorNula(void)
{
    Texto t1 = criaTexto(2, 0.0, 0.0, NULL, "blue", 'i', "teste");
    Texto t2 = criaTexto(3, 0.0, 0.0, "black", NULL, 'i', "teste");
    TEST_ASSERT_NULL_MESSAGE(t1, "corb NULL deve retornar NULL.");
    TEST_ASSERT_NULL_MESSAGE(t2, "corp NULL deve retornar NULL.");
}

void test_criaTexto_AncoraInvalida(void)
{
    Texto t = criaTexto(2, 0.0, 0.0, "black", "blue", 'x', "teste");
    TEST_ASSERT_NULL_MESSAGE(t, "Ancora invalida deve retornar NULL.");
}

void test_criaTexto_TxtoNulo(void)
{
    Texto t = criaTexto(2, 0.0, 0.0, "black", "blue", 'i', NULL);
    TEST_ASSERT_NULL_MESSAGE(t, "Conteudo NULL deve retornar NULL.");
}

/* ─────────────────────────────────────────────
   Getters com NULL
   ───────────────────────────────────────────── */

void test_Getters_TextoNulo(void)
{
    TEST_ASSERT_EQUAL_INT_MESSAGE(-1,    getIdTexto(NULL),     "getId(NULL) deve ser -1.");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(0.0, getXTexto(NULL),     "getX(NULL) deve ser 0.0.");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(0.0, getYTexto(NULL),     "getY(NULL) deve ser 0.0.");
    TEST_ASSERT_NULL_MESSAGE(getCorbTexto(NULL),                "getCorb(NULL) deve ser NULL.");
    TEST_ASSERT_NULL_MESSAGE(getCorpTexto(NULL),                "getCorp(NULL) deve ser NULL.");
    TEST_ASSERT_EQUAL_INT_MESSAGE('\0',  getAncora(NULL),      "getAncora(NULL) deve ser '\\0'.");
    TEST_ASSERT_NULL_MESSAGE(getTxto(NULL),                     "getTxto(NULL) deve ser NULL.");
    TEST_ASSERT_NULL_MESSAGE(getFamilyTexto(NULL),              "getFamily(NULL) deve ser NULL.");
    TEST_ASSERT_NULL_MESSAGE(getWeightTexto(NULL),              "getWeight(NULL) deve ser NULL.");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(0.0, getSizeTexto(NULL),  "getSize(NULL) deve ser 0.0.");
}

/* ─────────────────────────────────────────────
   transladaTexto
   ───────────────────────────────────────────── */

void test_translada_Positivo(void)
{
    transladaTexto(T, 5.0, 10.0);
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(15.0, getXTexto(T), "X deve ser 15.0 apos translada.");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(30.0, getYTexto(T), "Y deve ser 30.0 apos translada.");
}

void test_translada_Negativo(void)
{
    transladaTexto(T, -5.0, -10.0);
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(5.0,  getXTexto(T), "X deve ser 5.0 apos translada negativa.");
    TEST_ASSERT_EQUAL_DOUBLE_MESSAGE(10.0, getYTexto(T), "Y deve ser 10.0 apos translada negativa.");
}

void test_translada_NaoAlteraOutrosCampos(void)
{
    transladaTexto(T, 99.0, 99.0);
    TEST_ASSERT_EQUAL_STRING_MESSAGE("Hello World", getTxto(T),   "Conteudo nao deve mudar.");
    TEST_ASSERT_EQUAL_INT_MESSAGE('i',              getAncora(T), "Ancora nao deve mudar.");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("black",       getCorbTexto(T), "Corb nao deve mudar.");
}

void test_translada_TextoNulo(void)
{
    transladaTexto(NULL, 5.0, 5.0);
    TEST_ASSERT_TRUE(true); // nao deve crashar
}

/* ─────────────────────────────────────────────
   setCoresTexto
   ───────────────────────────────────────────── */

void test_setCores_Valido(void)
{
    setCoresTexto(T, "green", "yellow");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("green",  getCorbTexto(T), "Corb deve ser green.");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("yellow", getCorpTexto(T), "Corp deve ser yellow.");
}

void test_setCores_TextoNulo(void)
{
    setCoresTexto(NULL, "red", "blue");
    TEST_ASSERT_TRUE(true); // nao deve crashar
}

/* ─────────────────────────────────────────────
   contemPontoTexto
   ───────────────────────────────────────────── */

void test_contemPonto_NaAncora(void)
{
    // T tem ancora em (10.0, 20.0)
    TEST_ASSERT_TRUE_MESSAGE(contemPontoTexto(T, 10.0, 20.0),
        "Ponto na ancora deve retornar true.");
}

void test_contemPonto_ForaDaAncora(void)
{
    TEST_ASSERT_FALSE_MESSAGE(contemPontoTexto(T, 11.0, 20.0),
        "Ponto fora da ancora deve retornar false.");
    TEST_ASSERT_FALSE_MESSAGE(contemPontoTexto(T, 10.0, 21.0),
        "Ponto fora da ancora deve retornar false.");
}

void test_contemPonto_TextoNulo(void)
{
    TEST_ASSERT_FALSE_MESSAGE(contemPontoTexto(NULL, 10.0, 20.0),
        "NULL deve retornar false.");
}

/* ─────────────────────────────────────────────
   dentroRegiaoTexto
   ───────────────────────────────────────────── */

void test_dentroRegiao_AncoraContida(void)
{
    // T tem ancora em (10, 20); regiao (0, 0, 100, 100) contem esse ponto
    TEST_ASSERT_TRUE_MESSAGE(dentroRegiaoTexto(T, 0.0, 0.0, 100.0, 100.0),
        "Ancora dentro da regiao deve retornar true.");
}

void test_dentroRegiao_NaBorda(void)
{
    // Regiao exatamente na ancora
    TEST_ASSERT_TRUE_MESSAGE(dentroRegiaoTexto(T, 10.0, 20.0, 0.0, 0.0),
        "Ancora na borda da regiao deve retornar true.");
}

void test_dentroRegiao_ForaDaRegiao(void)
{
    TEST_ASSERT_FALSE_MESSAGE(dentroRegiaoTexto(T, 20.0, 20.0, 50.0, 50.0),
        "Ancora fora da regiao deve retornar false.");
    TEST_ASSERT_FALSE_MESSAGE(dentroRegiaoTexto(T, 0.0, 0.0, 5.0, 5.0),
        "Ancora fora da regiao deve retornar false.");
}

void test_dentroRegiao_TextoNulo(void)
{
    TEST_ASSERT_FALSE_MESSAGE(dentroRegiaoTexto(NULL, 0.0, 0.0, 100.0, 100.0),
        "NULL deve retornar false.");
}

/* ─────────────────────────────────────────────
   main
   ───────────────────────────────────────────── */

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_setEstilo_AplicadoAoCriar);
    RUN_TEST(test_setEstilo_NaoAfetaTextosJaCriados);
    RUN_TEST(test_setEstilo_Invalido);

    RUN_TEST(test_criaTexto_Valido);
    RUN_TEST(test_criaTexto_TodasAsAncoras);
    RUN_TEST(test_criaTexto_ComEspacos);
    RUN_TEST(test_criaTexto_IdInvalido);
    RUN_TEST(test_criaTexto_CorNula);
    RUN_TEST(test_criaTexto_AncoraInvalida);
    RUN_TEST(test_criaTexto_TxtoNulo);

    RUN_TEST(test_Getters_TextoNulo);

    RUN_TEST(test_translada_Positivo);
    RUN_TEST(test_translada_Negativo);
    RUN_TEST(test_translada_NaoAlteraOutrosCampos);
    RUN_TEST(test_translada_TextoNulo);

    RUN_TEST(test_setCores_Valido);
    RUN_TEST(test_setCores_TextoNulo);

    RUN_TEST(test_contemPonto_NaAncora);
    RUN_TEST(test_contemPonto_ForaDaAncora);
    RUN_TEST(test_contemPonto_TextoNulo);

    RUN_TEST(test_dentroRegiao_AncoraContida);
    RUN_TEST(test_dentroRegiao_NaBorda);
    RUN_TEST(test_dentroRegiao_ForaDaRegiao);
    RUN_TEST(test_dentroRegiao_TextoNulo);

    return UNITY_END();
}