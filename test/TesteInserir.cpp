//
//  TesteInserir.cpp
//
//  Criado por Eiji Adachi Medeiros Barbosa
//

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "TabelaHash.h"
#include "TabelaHashTestHelper.h"

using namespace std;

// Teste para inserir um único elemento em uma tabela vazia
TEST_CASE("Inserir um elemento em uma tabela vazia") {
    INFO("Cenário: Inserir um elemento em uma tabela vazia.");
    // Arrange
    TabelaHash tabela;
    const std::string c1 = "CHAVE";
    const std::string v1 = "Valor1";
    auto pos1 = TabelaHashTestHelper::hash(tabela, c1);
    CAPTURE(c1);
    CAPTURE(pos1);

    // Act
    INFO("Operação: inserir chave '", c1, "' com valor '", v1, "'. Esperado: retornar true sem lançar exceção.");
    bool inseriu = tabela.inserir(c1, v1);

    // Assert
    const auto consistencia = tabela.checarConsistencia();
    CHECK_MESSAGE(consistencia == ConsistenciaStatus::OK,
        TabelaHashTestHelper::mensagemConsistencia(consistencia, "a inserção/atualização"));

    INFO("Posição verificada no array: ", pos1);
    auto elemento = TabelaHashTestHelper::getElementoNaPosicao(tabela, pos1);

    CHECK_MESSAGE(elemento.estado == Estado::OCUPADO,
        "Após inserir/atualizar, era esperado que a posição verificada estivesse OCUPADO. Estado observado: ",
        TabelaHashTestHelper::nomeEstado(elemento.estado), ". Verifique se o estado foi atualizado no próprio array.");

    CHECK_MESSAGE(elemento.chave == c1,
        "Na posição verificada, era esperado chave '", c1, "'. Observado: '", elemento.chave, "'.");
    CHECK_MESSAGE(elemento.valor == v1,
        "Na posição verificada, era esperado valor '", v1, "'. Observado: '", elemento.valor, "'.");
    CHECK_MESSAGE(tabela.tamanho() == 1,
        "Após a inserção, era esperado quantidade = 1. Observado: ", tabela.tamanho(), ".");
    CHECK_MESSAGE(inseriu == true, "Era esperado que inserir retornasse true ao inserir ou atualizar a chave. Retorno observado: ", (inseriu ? "true" : "false"), ".");
}

// Teste para inserir dois elementos que colidem
TEST_CASE("Inserir dois elementos que colidem na tabela") {
    INFO("Cenário: Inserir dois elementos que colidem na tabela.");
    // Arrange
    TabelaHash tabela;
    const std::string c1 = "CHAVE";
    const std::string v1 = "Valor1";
    auto pos1 = TabelaHashTestHelper::hash(tabela, c1);
    CAPTURE(c1);
    CAPTURE(pos1);

    const std::string c2 = "VECHA";
    const std::string v2 = "Valor2";
    auto pos2 = TabelaHashTestHelper::hash(tabela, c2);
    CAPTURE(c2);
    CAPTURE(pos2);

    REQUIRE_MESSAGE(pos1 == pos2, "Pré-condição: as duas chaves devem ter o mesmo índice hash inicial para testar colisões.");

    // Act
    INFO("Operação: inserir chave '", c1, "' com valor '", v1, "'. Esperado: retornar true sem lançar exceção.");
    bool inseriu1 = tabela.inserir(c1, v1);
    INFO("Operação: inserir chave '", c2, "' com valor '", v2, "'. Esperado: retornar true sem lançar exceção.");
    bool inseriu2 = tabela.inserir(c2, v2);

    // Assert
    INFO("Posição verificada no array: ", pos1);
    auto elemento1 = TabelaHashTestHelper::getElementoNaPosicao(tabela, pos1);
    INFO("Posição verificada no array: ", pos1 + 1);
    auto elemento2 = TabelaHashTestHelper::getElementoNaPosicao(tabela, pos1 + 1);

    const auto consistencia = tabela.checarConsistencia();
    CHECK_MESSAGE(consistencia == ConsistenciaStatus::OK,
        TabelaHashTestHelper::mensagemConsistencia(consistencia, "a inserção/atualização"));

    CHECK_MESSAGE(elemento1.estado == Estado::OCUPADO,
        "Após inserir/atualizar, era esperado que a posição verificada estivesse OCUPADO. Estado observado: ",
        TabelaHashTestHelper::nomeEstado(elemento1.estado), ". Verifique se o estado foi atualizado no próprio array.");
    CHECK_MESSAGE(elemento1.chave == c1,
        "Na posição verificada, era esperado chave '", c1, "'. Observado: '", elemento1.chave, "'.");
    CHECK_MESSAGE(elemento1.valor == v1,
        "Na posição verificada, era esperado valor '", v1, "'. Observado: '", elemento1.valor, "'.");

    CHECK_MESSAGE(elemento2.estado == Estado::OCUPADO,
        "Após inserir/atualizar, era esperado que a posição verificada estivesse OCUPADO. Estado observado: ",
        TabelaHashTestHelper::nomeEstado(elemento2.estado), ". Verifique se o estado foi atualizado no próprio array.");
    CHECK_MESSAGE(elemento2.chave == c2,
        "Na posição verificada, era esperado chave '", c2, "'. Observado: '", elemento2.chave, "'.");
    CHECK_MESSAGE(elemento2.valor == v2,
        "Na posição verificada, era esperado valor '", v2, "'. Observado: '", elemento2.valor, "'.");

    CHECK_MESSAGE(tabela.tamanho() == 2,
        "Após a inserção, era esperado quantidade = 2. Observado: ", tabela.tamanho(), ".");
    CHECK_MESSAGE(inseriu1 == true, "Era esperado que inserir retornasse true ao inserir ou atualizar a chave. Retorno observado: ", (inseriu1 ? "true" : "false"), ".");
    CHECK_MESSAGE(inseriu2 == true, "Era esperado que inserir retornasse true ao inserir ou atualizar a chave. Retorno observado: ", (inseriu2 ? "true" : "false"), ".");
}

// Teste para atualizar um elemento existente na tabela
TEST_CASE("Atualizar um elemento existente na tabela") {
    INFO("Cenário: Atualizar um elemento existente na tabela.");
    // Arrange
    TabelaHash tabela;
    const std::string c1 = "CHAVE";
    const std::string v1 = "Valor1";
    auto pos1 = TabelaHashTestHelper::hash(tabela, c1);
    CAPTURE(c1);
    CAPTURE(pos1);
    const std::string novoValor = "Novo";

    // Act
    INFO("Operação: inserir chave '", c1, "' com valor '", v1, "'. Esperado: retornar true sem lançar exceção.");
    bool inseriu1 = tabela.inserir(c1, v1);
    INFO("Operação: inserir chave '", c1, "' com valor '", novoValor, "'. Esperado: retornar true sem lançar exceção.");
    bool atualizou = tabela.inserir(c1, novoValor); // Atualiza

    // Assert
    const auto consistencia = tabela.checarConsistencia();
    CHECK_MESSAGE(consistencia == ConsistenciaStatus::OK,
        TabelaHashTestHelper::mensagemConsistencia(consistencia, "a inserção/atualização"));

    INFO("Posição verificada no array: ", pos1);
    auto elemento = TabelaHashTestHelper::getElementoNaPosicao(tabela, pos1);
    CHECK_MESSAGE(elemento.estado == Estado::OCUPADO,
        "Após inserir/atualizar, era esperado que a posição verificada estivesse OCUPADO. Estado observado: ",
        TabelaHashTestHelper::nomeEstado(elemento.estado), ". Verifique se o estado foi atualizado no próprio array.");
    CHECK_MESSAGE(elemento.chave == c1,
        "Na posição verificada, era esperado chave '", c1, "'. Observado: '", elemento.chave, "'.");
    CHECK_MESSAGE(elemento.valor == novoValor,
        "Na posição verificada, era esperado valor '", novoValor, "'. Observado: '", elemento.valor, "'.");
    CHECK_MESSAGE(inseriu1 == true, "Era esperado que inserir retornasse true ao inserir ou atualizar a chave. Retorno observado: ", (inseriu1 ? "true" : "false"), ".");
    CHECK_MESSAGE(atualizou == true, "Era esperado que inserir retornasse true ao inserir ou atualizar a chave. Retorno observado: ", (atualizou ? "true" : "false"), ".");
}

// Teste para inserir em uma tabela já cheia
TEST_CASE("Inserir em uma tabela já cheia deve lançar exceção") {
    INFO("Cenário: Inserir em uma tabela já cheia deve lançar exceção.");
     // Arrange
    TabelaHash tabela;

    TabelaHashTestHelper::preencherTabelaComOcupado(tabela);
    REQUIRE_MESSAGE(tabela.checarConsistencia() == ConsistenciaStatus::OK,
        "Pré-condição: a tabela preparada pelo teste deve estar consistente antes da operação.");
    REQUIRE_MESSAGE(tabela.cheia(), "Pré-condição: a tabela deve estar cheia antes desta operação.");

    // Act & Assert
    CHECK_THROWS_AS_MESSAGE(tabela.inserir("chave", "Valor"), std::overflow_error,
        "Ao inserir uma chave nova numa tabela cheia, era esperado lançar std::overflow_error.");
    const auto consistencia = tabela.checarConsistencia();
    CHECK_MESSAGE(consistencia == ConsistenciaStatus::OK,
        TabelaHashTestHelper::mensagemConsistencia(consistencia, "a inserção/atualização"));
}

// Teste para atualizar um elemento em uma tabela já cheia
TEST_CASE("Atualizar valor em uma tabela com todas posições OCUPADO") {
    INFO("Cenário: Atualizar valor em uma tabela com todas posições OCUPADO.");
    // Arrange
    TabelaHash tabela;
    TabelaHashTestHelper::preencherTabelaComOcupado(tabela);

    REQUIRE_MESSAGE(tabela.checarConsistencia() == ConsistenciaStatus::OK,
        "Pré-condição: a tabela preparada pelo teste deve estar consistente antes da operação.");
    REQUIRE_MESSAGE(tabela.cheia(), "Pré-condição: a tabela deve estar cheia antes desta operação.");

    auto chave = TabelaHashTestHelper::getElementoNaPosicao(tabela, 0).chave;

    // Act
    INFO("Operação: atualizar chave existente '", chave, "' para o valor 'Novo', mesmo com a tabela cheia. Esperado: retornar true sem lançar exceção.");
    bool atualizou = tabela.inserir(chave, "Novo");

    // Assert
    const auto consistencia = tabela.checarConsistencia();
    CHECK_MESSAGE(consistencia == ConsistenciaStatus::OK,
        TabelaHashTestHelper::mensagemConsistencia(consistencia, "a inserção/atualização"));

    const auto elemento = TabelaHashTestHelper::getElementoNaPosicao(tabela, 0);
    CHECK_MESSAGE(elemento.chave == chave, "Ao atualizar, era esperado preservar a chave '", chave,
        "'. Observado: '", elemento.chave, "'.");
    CHECK_MESSAGE(elemento.valor == "Novo", "Após atualizar, era esperado o valor 'Novo'. Observado: '", elemento.valor, "'.");
    CHECK_MESSAGE(atualizou == true, "Era esperado que inserir retornasse true ao inserir ou atualizar a chave. Retorno observado: ", (atualizou ? "true" : "false"), ".");
}

// Teste para inserir em uma tabela com algumas posições REMOVIDO
TEST_CASE("Inserir em uma tabela com algumas posições REMOVIDO") {
    INFO("Cenário: Inserir em uma tabela com algumas posições REMOVIDO.");
    // Arrange
    TabelaHash tabela;

    const std::string c1 = "CHAVE";
    const std::string v1 = "Valor1";
    auto pos1 = TabelaHashTestHelper::hash(tabela, c1);
    CAPTURE(c1);
    CAPTURE(pos1);
    Elemento e1;
    e1.chave = c1;
    e1.valor = v1;
    e1.estado = Estado::OCUPADO;

    const std::string c2 = "VECHA";
    const std::string v2 = "Valor2";
    Elemento e2;
    e2.chave = c2;
    e2.valor = v2;
    e2.estado = Estado::OCUPADO;

    const std::string c3 = "ECHAV";
    const std::string v3 = "Valor3";

    Elemento removido1;
    removido1.estado = Estado::REMOVIDO;

    Elemento removido2;
    removido2.estado = Estado::REMOVIDO;

    std::vector<pair<int, Elemento>> dados = {
        {pos1    , e1},
        {pos1 + 1, removido1},
        {pos1 + 2, removido2},
        {pos1 + 3, e2}
    };
    TabelaHashTestHelper::preencherTabela(tabela, dados);
    REQUIRE_MESSAGE(tabela.checarConsistencia() == ConsistenciaStatus::OK,
        "Pré-condição: a tabela preparada pelo teste deve estar consistente antes da operação.");

    // Act
    INFO("Operação: inserir chave '", c3, "' com valor '", v3, "'. Esperado: retornar true sem lançar exceção.");
    bool inseriu = tabela.inserir(c3, v3);
    // Assert

    const auto consistencia = tabela.checarConsistencia();
    CHECK_MESSAGE(consistencia == ConsistenciaStatus::OK,
        TabelaHashTestHelper::mensagemConsistencia(consistencia, "a inserção/atualização"));

    INFO("Posição verificada no array: ", pos1 + 1);
    auto elemento = TabelaHashTestHelper::getElementoNaPosicao(tabela, pos1 + 1);
    CHECK_MESSAGE(elemento.estado == Estado::OCUPADO,
        "Após inserir/atualizar, era esperado que a posição verificada estivesse OCUPADO. Estado observado: ",
        TabelaHashTestHelper::nomeEstado(elemento.estado), ". Verifique se o estado foi atualizado no próprio array.");
    CHECK_MESSAGE(elemento.chave == c3,
        "Na posição verificada, era esperado chave '", c3, "'. Observado: '", elemento.chave, "'.");
    CHECK_MESSAGE(elemento.valor == v3,
        "Na posição verificada, era esperado valor '", v3, "'. Observado: '", elemento.valor, "'.");
    CHECK_MESSAGE(inseriu == true, "Era esperado que inserir retornasse true ao inserir ou atualizar a chave. Retorno observado: ", (inseriu ? "true" : "false"), ".");
}

// Teste para atualizar um elemento após uma posição REMOVIDO
TEST_CASE("Atualizar um elemento após uma posição REMOVIDO") {
    INFO("Cenário: Atualizar um elemento após uma posição REMOVIDO.");
    // Arrange
    TabelaHash tabela;

    const std::string c1 = "CHAVE";
    const std::string v1 = "Valor1";
    auto pos1 = TabelaHashTestHelper::hash(tabela, c1);
    CAPTURE(c1);
    CAPTURE(pos1);
    Elemento e1;
    e1.chave = c1;
    e1.valor = v1;
    e1.estado = Estado::OCUPADO;

    const std::string c2 = "VECHA";
    const std::string v2 = "Valor2";
    Elemento e2;
    e2.chave = c2;
    e2.valor = v2;
    e2.estado = Estado::OCUPADO;

    const std::string novoValor = "Novo";

    Elemento removido1;
    removido1.estado = Estado::REMOVIDO;

    Elemento removido2;
    removido2.estado = Estado::REMOVIDO;

    std::vector<std::pair<int, Elemento>> dados = {
        {pos1    , e1},
        {pos1 + 1, removido1},
        {pos1 + 2, removido2},
        {pos1 + 3, e2}
    };

    TabelaHashTestHelper::preencherTabela(tabela, dados);
    REQUIRE_MESSAGE(tabela.checarConsistencia() == ConsistenciaStatus::OK,
        "Pré-condição: a tabela preparada pelo teste deve estar consistente antes da operação.");

    // Act
    INFO("Operação: inserir chave '", c2, "' com valor '", novoValor, "'. Esperado: retornar true sem lançar exceção.");
    bool atualizou = tabela.inserir(c2, novoValor);

    // Assert
    INFO("Posição verificada no array: ", pos1 + 3);
    auto elemento = TabelaHashTestHelper::getElementoNaPosicao(tabela, pos1 + 3);
    CHECK_MESSAGE(elemento.estado == Estado::OCUPADO,
        "Após inserir/atualizar, era esperado que a posição verificada estivesse OCUPADO. Estado observado: ",
        TabelaHashTestHelper::nomeEstado(elemento.estado), ". Verifique se o estado foi atualizado no próprio array.");
    CHECK_MESSAGE(elemento.chave == c2,
        "Na posição verificada, era esperado chave '", c2, "'. Observado: '", elemento.chave, "'.");
    CHECK_MESSAGE(elemento.valor == novoValor,
        "Na posição verificada, era esperado valor '", novoValor, "'. Observado: '", elemento.valor, "'.");
    const auto consistencia = tabela.checarConsistencia();
    CHECK_MESSAGE(consistencia == ConsistenciaStatus::OK,
        TabelaHashTestHelper::mensagemConsistencia(consistencia, "a inserção/atualização"));
    CHECK_MESSAGE(atualizou == true, "Era esperado que inserir retornasse true ao inserir ou atualizar a chave. Retorno observado: ", (atualizou ? "true" : "false"), ".");
}

// Teste para inserir em uma tabela com todas posições REMOVIDO
TEST_CASE("Inserir em uma tabela com todas posições REMOVIDO") {
    INFO("Cenário: Inserir em uma tabela com todas posições REMOVIDO.");
    // Arrange
    TabelaHash tabela;

    const std::string c3 = "ECHAV";
    const std::string v3 = "Valor3";
    auto pos3 = TabelaHashTestHelper::hash(tabela, c3);
    CAPTURE(c3);
    CAPTURE(pos3);

    TabelaHashTestHelper::preencherTabelaComRemovido(tabela);
    REQUIRE_MESSAGE(tabela.checarConsistencia() == ConsistenciaStatus::OK,
        "Pré-condição: a tabela preparada pelo teste deve estar consistente antes da operação.");

    // Act
    INFO("Operação: inserir chave '", c3, "' com valor '", v3, "'. Esperado: retornar true sem lançar exceção.");
    bool inseriu = tabela.inserir(c3, v3);

    // Assert
    INFO("Posição verificada no array: ", pos3);
    auto elemento = TabelaHashTestHelper::getElementoNaPosicao(tabela, pos3);
    CHECK_MESSAGE(elemento.estado == Estado::OCUPADO,
        "Após inserir/atualizar, era esperado que a posição verificada estivesse OCUPADO. Estado observado: ",
        TabelaHashTestHelper::nomeEstado(elemento.estado), ". Verifique se o estado foi atualizado no próprio array.");
    CHECK_MESSAGE(elemento.chave == c3,
        "Na posição verificada, era esperado chave '", c3, "'. Observado: '", elemento.chave, "'.");
    CHECK_MESSAGE(elemento.valor == v3,
        "Na posição verificada, era esperado valor '", v3, "'. Observado: '", elemento.valor, "'.");
    const auto consistencia = tabela.checarConsistencia();
    CHECK_MESSAGE(consistencia == ConsistenciaStatus::OK,
        TabelaHashTestHelper::mensagemConsistencia(consistencia, "a inserção/atualização"));
    CHECK_MESSAGE(tabela.tamanho() == 1,
        "Após a inserção, era esperado quantidade = 1. Observado: ", tabela.tamanho(), ".");
    CHECK_MESSAGE(inseriu == true, "Era esperado que inserir retornasse true ao inserir ou atualizar a chave. Retorno observado: ", (inseriu ? "true" : "false"), ".");
}
