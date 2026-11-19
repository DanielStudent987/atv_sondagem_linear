//
//  TesteBuscar.cpp
//
//  Criado por Eiji Adachi Medeiros Barbosa
//

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "TabelaHash.h"
#include "TabelaHashTestHelper.h"
#include <iostream>

using namespace std;

// Definição global das variáveis usadas por diferentes casos de teste
const string c1 = "CHAVE";
const string v1 = "Valor1";
const string c2 = "VECHA";
const string v2 = "Valor2";
const string c3 = "ECHAV";
const string v3 = "Valor3";

// Teste para buscar um elemento existente sem colisão
TEST_CASE("Buscar elemento existente sem colisão") {
    INFO("Cenário: Buscar elemento existente sem colisão.");
    TabelaHash tabela;
    auto pos1 = TabelaHashTestHelper::hash(tabela, c1);
    CAPTURE(c1);
    CAPTURE(pos1);

    TabelaHashTestHelper::setElementoNaPosicao(tabela, pos1, c1, v1);
    TabelaHashTestHelper::setQuantidade(tabela, 1);

    // Act
    INFO("Operação: buscar chave '", c1, "'. Esperado: retornar o valor associado ou std::nullopt, sem lançar exceção.");
    auto resultado = tabela.buscar(c1);

    // Assert
    const auto consistencia = tabela.checarConsistencia();
    CHECK_MESSAGE(consistencia == ConsistenciaStatus::OK,
        TabelaHashTestHelper::mensagemConsistencia(consistencia, "a busca"));
    CHECK_MESSAGE(resultado == v1,
        "Ao buscar a chave '", c1, "', era esperado obter o valor '", v1,
        "'. Resultado observado: ", TabelaHashTestHelper::nomeResultado(resultado), ".");
}

// Teste para buscar um elemento existente com colisão
TEST_CASE("Buscar elemento existente com colisão") {
    INFO("Cenário: Buscar elemento existente com colisão.");
    // Arrange
    TabelaHash tabela;
    auto pos1 = TabelaHashTestHelper::hash(tabela, c1);
    CAPTURE(c1);
    CAPTURE(pos1);

    TabelaHashTestHelper::setElementoNaPosicao(tabela, pos1, c1, v1);
    TabelaHashTestHelper::setElementoNaPosicao(tabela, pos1 + 1, c2, v2);
    TabelaHashTestHelper::setElementoNaPosicao(tabela, pos1 + 2, c3, v3);
    TabelaHashTestHelper::setQuantidade(tabela, 3);

    // Act
    INFO("Operação: buscar chave '", c1, "'. Esperado: retornar o valor associado ou std::nullopt, sem lançar exceção.");
    auto resultado1 = tabela.buscar(c1);
    INFO("Operação: buscar chave '", c2, "'. Esperado: retornar o valor associado ou std::nullopt, sem lançar exceção.");
    auto resultado2 = tabela.buscar(c2);
    INFO("Operação: buscar chave '", c3, "'. Esperado: retornar o valor associado ou std::nullopt, sem lançar exceção.");
    auto resultado3 = tabela.buscar(c3);

    // Assert
    const auto consistencia = tabela.checarConsistencia();
    CHECK_MESSAGE(consistencia == ConsistenciaStatus::OK,
        TabelaHashTestHelper::mensagemConsistencia(consistencia, "a busca"));
    CHECK_MESSAGE(resultado1 == v1,
        "Ao buscar a chave '", c1, "', era esperado obter o valor '", v1,
        "'. Resultado observado: ", TabelaHashTestHelper::nomeResultado(resultado1), ".");

    CHECK_MESSAGE(resultado2 == v2,
        "Ao buscar a chave '", c2, "', era esperado obter o valor '", v2,
        "'. Resultado observado: ", TabelaHashTestHelper::nomeResultado(resultado2), ".");

    CHECK_MESSAGE(resultado3 == v3,
        "Ao buscar a chave '", c3, "', era esperado obter o valor '", v3,
        "'. Resultado observado: ", TabelaHashTestHelper::nomeResultado(resultado3), ".");
}

// Teste para buscar um elemento inexistente
TEST_CASE("Buscar elemento inexistente") {
    INFO("Cenário: Buscar elemento inexistente.");
    // Arrange
    TabelaHash tabela;
    auto pos1 = TabelaHashTestHelper::hash(tabela, c1);
    CAPTURE(c1);
    CAPTURE(pos1);

    TabelaHashTestHelper::setElementoNaPosicao(tabela, pos1, c1, v1);
    TabelaHashTestHelper::setElementoNaPosicao(tabela, pos1 + 1, c2, v2);
    TabelaHashTestHelper::setElementoNaPosicao(tabela, pos1 + 2, c3, v3);
    TabelaHashTestHelper::setQuantidade(tabela, 3);

    // Act
    INFO("Operação: buscar chave '", std::string("chave_qualquer"), "'. Esperado: retornar o valor associado ou std::nullopt, sem lançar exceção.");
    auto resultado = tabela.buscar("chave_qualquer");

    // Assert
    const auto consistencia = tabela.checarConsistencia();
    CHECK_MESSAGE(consistencia == ConsistenciaStatus::OK,
        TabelaHashTestHelper::mensagemConsistencia(consistencia, "a busca"));
    CHECK_MESSAGE(resultado == std::nullopt,
        "Ao buscar a chave inexistente 'chave_qualquer', era esperado retornar std::nullopt. Resultado observado: ",
        TabelaHashTestHelper::nomeResultado(resultado), ".");
}

// Teste para buscar um elemento inexistente numa tabela cheia
TEST_CASE("Buscar elemento inexistente numa tabela cheia") {
    INFO("Cenário: Buscar elemento inexistente numa tabela cheia.");
    // Arrange
    TabelaHash tabela;
    TabelaHashTestHelper::preencherTabelaComOcupado(tabela);

    // Act
    INFO("Operação: buscar chave '", std::string("chave_qualquer"), "'. Esperado: retornar o valor associado ou std::nullopt, sem lançar exceção.");
    auto resultado = tabela.buscar("chave_qualquer");

    // Assert
    const auto consistencia = tabela.checarConsistencia();
    CHECK_MESSAGE(consistencia == ConsistenciaStatus::OK,
        TabelaHashTestHelper::mensagemConsistencia(consistencia, "a busca"));
    CHECK_MESSAGE(resultado == std::nullopt,
        "Ao buscar a chave inexistente 'chave_qualquer', era esperado retornar std::nullopt. Resultado observado: ",
        TabelaHashTestHelper::nomeResultado(resultado), ".");
}

// Teste para buscar um elemento após posição REMOVIDO
TEST_CASE("Buscar elemento após posição REMOVIDO") {
    INFO("Cenário: Buscar elemento após posição REMOVIDO.");
    // Arrange
    TabelaHash tabela;
    auto pos1 = TabelaHashTestHelper::hash(tabela, c1);
    CAPTURE(c1);
    CAPTURE(pos1);

    Elemento e1;
    e1.estado = Estado::REMOVIDO;

    Elemento e2;
    e2.estado = Estado::REMOVIDO;

    TabelaHashTestHelper::setElementoNaPosicao(tabela, pos1, e1);
    TabelaHashTestHelper::setElementoNaPosicao(tabela, pos1 + 1, e2);
    TabelaHashTestHelper::setElementoNaPosicao(tabela, pos1 + 2, c3, v3);
    TabelaHashTestHelper::setQuantidade(tabela, 1);

    // Act
    INFO("Operação: buscar chave '", c3, "'. Esperado: retornar o valor associado ou std::nullopt, sem lançar exceção.");
    auto resultado = tabela.buscar(c3);

    // Assert
    const auto consistencia = tabela.checarConsistencia();
    CHECK_MESSAGE(consistencia == ConsistenciaStatus::OK,
        TabelaHashTestHelper::mensagemConsistencia(consistencia, "a busca"));
    CHECK_MESSAGE(resultado == v3,
        "Ao buscar a chave '", c3, "' após duas posições REMOVIDO, era esperado obter o valor '", v3,
        "'. Resultado observado: ", TabelaHashTestHelper::nomeResultado(resultado),
        ". Verifique se a sondagem continua ao encontrar posições REMOVIDO.");
}
