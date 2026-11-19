//
//  TesteRemover.cpp
//
//  Criado por Eiji Adachi Medeiros Barbosa
//

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "TabelaHash.h"
#include "TabelaHashTestHelper.h"

using namespace std;

// Variáveis globais reusadas pelos casos de teste
const string c1 = "CHAVE";
const string v1 = "Valor1";

const string c2 = "VECHA";
const string v2 = "Valor2";
    
const string c3 = "ECHAV";
const string v3 = "Valor3";
    

// Teste para remover um elemento existente sem colisão
TEST_CASE("Remover elemento existente sem colisão") {
    INFO("Remover a única chave da tabela: deve retornar true, marcar REMOVIDO e zerar a quantidade.");
    // Arrange
    TabelaHash tabela;  
    auto pos1 = TabelaHashTestHelper::hash(tabela, c1);
    CAPTURE(c1);
    CAPTURE(pos1);

    TabelaHashTestHelper::setElementoNaPosicao(tabela, pos1, c1, v1);
    TabelaHashTestHelper::setQuantidade(tabela, 1);
    REQUIRE_MESSAGE(tabela.checarConsistencia() == ConsistenciaStatus::OK,
        "Pré-condição: a tabela preparada pelo teste deve estar consistente antes da remoção.");

    // Act
    auto removeu = tabela.remover(c1);

    // Assert
    const auto consistencia = tabela.checarConsistencia();
    CHECK_MESSAGE(consistencia == ConsistenciaStatus::OK,
        TabelaHashTestHelper::mensagemConsistencia(consistencia, "a remoção"));

    auto elemento = TabelaHashTestHelper::getElementoNaPosicao(tabela, pos1);

    CHECK_MESSAGE(removeu == true, "remover deve retornar true quando encontra e remove a chave existente.");
    CHECK_MESSAGE(tabela.tamanho() == 0, "Após remover todos os elementos, tamanho() deve retornar 0.");
    CHECK_MESSAGE(elemento.estado == Estado::REMOVIDO,
        "Após remover a chave '", c1, "', era esperado atualizar array[", pos1,
        "] para REMOVIDO. Estado observado no array: ", TabelaHashTestHelper::nomeEstado(elemento.estado),
        ". A posição precisa ser marcada como REMOVIDO para preservar a sondagem das chaves que colidiram.");
}

// Teste para remover um elemento existente com colisão
TEST_CASE("Remover elemento existente com colisão") {
    INFO("Remover a terceira chave de uma sequência de colisões, preservando as duas anteriores.");
    // Arrange
    TabelaHash tabela;  
    auto pos1 = TabelaHashTestHelper::hash(tabela, c1);
    CAPTURE(c1);
    CAPTURE(pos1);
    auto pos2 = TabelaHashTestHelper::hash(tabela, c2);
    auto pos3 = TabelaHashTestHelper::hash(tabela, c3);
    CAPTURE(c2);
    CAPTURE(pos2);
    CAPTURE(c3);
    CAPTURE(pos3);

    REQUIRE_MESSAGE(pos1 == pos2, "Pré-condição: CHAVE e VECHA devem ter o mesmo índice hash inicial para testar colisões.");
    REQUIRE_MESSAGE(pos2 == pos3, "Pré-condição: VECHA e ECHAV devem ter o mesmo índice hash inicial para testar colisões.");

    TabelaHashTestHelper::setElementoNaPosicao(tabela, pos1, c1, v1);
    TabelaHashTestHelper::setElementoNaPosicao(tabela, pos1 + 1, c2, v2);
    TabelaHashTestHelper::setElementoNaPosicao(tabela, pos1 + 2, c3, v3);
    TabelaHashTestHelper::setQuantidade(tabela, 3);
    REQUIRE_MESSAGE(tabela.checarConsistencia() == ConsistenciaStatus::OK,
        "Pré-condição: a tabela preparada pelo teste deve estar consistente antes da remoção.");

    // Act
    auto removeu = tabela.remover(c3);

    // Assert
    const auto consistencia = tabela.checarConsistencia();
    CHECK_MESSAGE(consistencia == ConsistenciaStatus::OK,
        TabelaHashTestHelper::mensagemConsistencia(consistencia, "a remoção"));

    auto elemento1 = TabelaHashTestHelper::getElementoNaPosicao(tabela, pos1);
    auto elemento2 = TabelaHashTestHelper::getElementoNaPosicao(tabela, pos1 + 1);
    auto elemento3 = TabelaHashTestHelper::getElementoNaPosicao(tabela, pos1 + 2);

    CHECK_MESSAGE(removeu == true, "remover deve retornar true quando encontra e remove a chave existente.");
    CHECK_MESSAGE(tabela.tamanho() == 2, "Ao remover uma das três chaves, a quantidade deve diminuir exatamente de 3 para 2.");

    CHECK_MESSAGE(elemento1.estado == Estado::OCUPADO, "A chave '", c1, "' não foi removida: era esperado manter array[", pos1,
        "] como OCUPADO. Estado observado no array: ", TabelaHashTestHelper::nomeEstado(elemento1.estado), ".");
    CHECK_MESSAGE(elemento1.chave == c1, "Remover outra chave não deve alterar a chave armazenada nesta posição. Esperado: ", c1);
    CHECK_MESSAGE(elemento1.valor == v1, "Remover outra chave não deve alterar o valor armazenado nesta posição. Esperado: ", v1);

    CHECK_MESSAGE(elemento2.estado == Estado::OCUPADO, "A chave '", c2, "' não foi removida: era esperado manter array[", pos1 + 1,
        "] como OCUPADO. Estado observado no array: ", TabelaHashTestHelper::nomeEstado(elemento2.estado), ".");
    CHECK_MESSAGE(elemento2.chave == c2, "Remover outra chave não deve alterar a chave armazenada nesta posição. Esperado: ", c2);
    CHECK_MESSAGE(elemento2.valor == v2, "Remover outra chave não deve alterar o valor armazenado nesta posição. Esperado: ", v2);

    CHECK_MESSAGE(elemento3.estado == Estado::REMOVIDO, "Após remover a chave '", c3, "', era esperado atualizar array[", pos1 + 2,
        "] para REMOVIDO. Estado observado no array: ", TabelaHashTestHelper::nomeEstado(elemento3.estado), ".");
}

TEST_CASE("Remover sucessivamente todos os elementos, forçando posições Estado::REMOVIDO") {
    INFO("Remover três chaves que colidem: posições REMOVIDO não devem interromper a sondagem.");
    // Arrange
    TabelaHash tabela;  
    auto pos1 = TabelaHashTestHelper::hash(tabela, c1);
    CAPTURE(c1);
    CAPTURE(pos1);
    auto pos2 = TabelaHashTestHelper::hash(tabela, c2);
    auto pos3 = TabelaHashTestHelper::hash(tabela, c3);
    CAPTURE(c2);
    CAPTURE(pos2);
    CAPTURE(c3);
    CAPTURE(pos3);

    REQUIRE_MESSAGE(pos1 == pos2, "Pré-condição: CHAVE e VECHA devem ter o mesmo índice hash inicial para testar colisões.");
    REQUIRE_MESSAGE(pos2 == pos3, "Pré-condição: VECHA e ECHAV devem ter o mesmo índice hash inicial para testar colisões.");

    TabelaHashTestHelper::setElementoNaPosicao(tabela, pos1, c1, v1);
    TabelaHashTestHelper::setElementoNaPosicao(tabela, pos1 + 1, c2, v2);
    TabelaHashTestHelper::setElementoNaPosicao(tabela, pos1 + 2, c3, v3);
    TabelaHashTestHelper::setQuantidade(tabela, 3);
    REQUIRE_MESSAGE(tabela.checarConsistencia() == ConsistenciaStatus::OK,
        "Pré-condição: a tabela preparada pelo teste deve estar consistente antes da remoção.");

    // Act
    auto removeu1 = tabela.remover(c1);
    auto removeu2 = tabela.remover(c2);
    auto removeu3 = tabela.remover(c3);

    // Assert
    const auto consistencia = tabela.checarConsistencia();
    CHECK_MESSAGE(consistencia == ConsistenciaStatus::OK,
        TabelaHashTestHelper::mensagemConsistencia(consistencia, "a remoção"));

    auto elemento1 = TabelaHashTestHelper::getElementoNaPosicao(tabela, pos1);
    auto elemento2 = TabelaHashTestHelper::getElementoNaPosicao(tabela, pos1 + 1);
    auto elemento3 = TabelaHashTestHelper::getElementoNaPosicao(tabela, pos1 + 2);

    CHECK_MESSAGE(removeu1 == true, "A remoção de ", c1, " deve retornar true. A sondagem deve continuar ao encontrar posições REMOVIDO.");
    CHECK_MESSAGE(removeu2 == true, "A remoção de ", c2, " deve retornar true. A sondagem deve continuar ao encontrar posições REMOVIDO.");
    CHECK_MESSAGE(removeu3 == true, "A remoção de ", c3, " deve retornar true. A sondagem deve continuar ao encontrar posições REMOVIDO.");
    CHECK_MESSAGE(tabela.tamanho() == 0, "Após remover todos os elementos, tamanho() deve retornar 0.");
    
    CHECK_MESSAGE(elemento1.estado == Estado::REMOVIDO, "Após remover a chave '", c1, "', era esperado atualizar array[", pos1,
        "] para REMOVIDO. Estado observado no array: ", TabelaHashTestHelper::nomeEstado(elemento1.estado), ".");
    CHECK_MESSAGE(elemento2.estado == Estado::REMOVIDO, "Após remover a chave '", c2, "', era esperado atualizar array[", pos1 + 1,
        "] para REMOVIDO. Estado observado no array: ", TabelaHashTestHelper::nomeEstado(elemento2.estado), ".");
    CHECK_MESSAGE(elemento3.estado == Estado::REMOVIDO, "Após remover a chave '", c3, "', era esperado atualizar array[", pos1 + 2,
        "] para REMOVIDO. Estado observado no array: ", TabelaHashTestHelper::nomeEstado(elemento3.estado), ".");
}

// Teste para remover um elemento inexistente
TEST_CASE("Remover elemento inexistente") {
    INFO("Tentar remover uma chave ausente deve retornar false e preservar a quantidade.");
    // Arrange
    TabelaHash tabela;  
    auto pos1 = TabelaHashTestHelper::hash(tabela, c1);
    CAPTURE(c1);
    CAPTURE(pos1);
    auto pos2 = TabelaHashTestHelper::hash(tabela, c2);
    auto pos3 = TabelaHashTestHelper::hash(tabela, c3);
    CAPTURE(c2);
    CAPTURE(pos2);
    CAPTURE(c3);
    CAPTURE(pos3);

    REQUIRE_MESSAGE(pos1 == pos2, "Pré-condição: CHAVE e VECHA devem ter o mesmo índice hash inicial para testar colisões.");
    REQUIRE_MESSAGE(pos2 == pos3, "Pré-condição: VECHA e ECHAV devem ter o mesmo índice hash inicial para testar colisões.");

    TabelaHashTestHelper::setElementoNaPosicao(tabela, pos1, c1, v1);
    TabelaHashTestHelper::setElementoNaPosicao(tabela, pos1 + 1, c2, v2);
    TabelaHashTestHelper::setElementoNaPosicao(tabela, pos1 + 2, c3, v3);
    TabelaHashTestHelper::setQuantidade(tabela, 3);
    REQUIRE_MESSAGE(tabela.checarConsistencia() == ConsistenciaStatus::OK,
        "Pré-condição: a tabela preparada pelo teste deve estar consistente antes da remoção.");

    // Act
    auto removeu = tabela.remover("chave_qualquer");

    // Assert
    const auto consistencia = tabela.checarConsistencia();
    CHECK_MESSAGE(consistencia == ConsistenciaStatus::OK,
        TabelaHashTestHelper::mensagemConsistencia(consistencia, "a remoção"));
    CHECK_MESSAGE(removeu == false, "remover deve retornar false quando a chave não existe na tabela.");
    CHECK_MESSAGE(tabela.tamanho() == 3, "Uma tentativa de remover chave inexistente deve manter a quantidade em 3.");
}

// Teste para remover um elemento inexistente em uma tabela cheia
TEST_CASE("Remover elemento inexistente numa tabela cheia") {
    INFO("Em tabela cheia, a sondagem deve terminar após no máximo capacidade posições e retornar false para chave ausente.");
    // Arrange
    TabelaHash tabela;  
    auto pos1 = TabelaHashTestHelper::hash(tabela, c1);
    CAPTURE(c1);
    CAPTURE(pos1);
    auto pos2 = TabelaHashTestHelper::hash(tabela, c2);
    auto pos3 = TabelaHashTestHelper::hash(tabela, c3);
    CAPTURE(c2);
    CAPTURE(pos2);
    CAPTURE(c3);
    CAPTURE(pos3);

    REQUIRE_MESSAGE(pos1 == pos2, "Pré-condição: CHAVE e VECHA devem ter o mesmo índice hash inicial para testar colisões.");
    REQUIRE_MESSAGE(pos2 == pos3, "Pré-condição: VECHA e ECHAV devem ter o mesmo índice hash inicial para testar colisões.");

    // Enche a tabela
    TabelaHashTestHelper::preencherTabelaComOcupado(tabela);
    REQUIRE_MESSAGE(tabela.checarConsistencia() == ConsistenciaStatus::OK,
        "Pré-condição: a tabela preparada pelo teste deve estar consistente antes da remoção.");
    REQUIRE_MESSAGE(tabela.cheia(), "Pré-condição: todas as posições devem estar ocupadas antes da tentativa de remoção.");

    // Act
    auto removeu = tabela.remover("chave_qualquer");

    // Assert
    const auto consistencia = tabela.checarConsistencia();
    CHECK_MESSAGE(consistencia == ConsistenciaStatus::OK,
        TabelaHashTestHelper::mensagemConsistencia(consistencia, "a remoção"));
    CHECK_MESSAGE(removeu == false, "remover deve retornar false quando a chave não existe na tabela.");
    CHECK_MESSAGE(tabela.cheia(), "Remover uma chave inexistente deve manter a tabela cheia, sem diminuir a quantidade.");
}
