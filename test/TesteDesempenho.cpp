#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "TabelaHash.h"
#include <vector>
#include <chrono>

TEST_CASE("Teste de Desempenho") {
    using namespace std::chrono;

    const int capacidade = 100003;
    const int totalEntradas = 180000;

    TabelaHash tabela(capacidade);

    std::vector<std::pair<std::string, std::string>> entradas;

    for (int i = 0; i < totalEntradas; ++i) {
        auto chave = "chave" + std::to_string(i);
        auto valor = "valor" + std::to_string(i);
        entradas.push_back(std::make_pair(chave, valor));
    }

    int totalInsercoes = 0;
    bool tabelaFicouCheia = false;
    auto inicio = steady_clock::now();
    
    try
    {
        for(const auto& entrada : entradas)
        {
            if (tabela.inserir(entrada.first, entrada.second))
            {
                totalInsercoes++;
            }
        }
    }
    catch (const std::overflow_error&)
    {
        tabelaFicouCheia = true;
    }

    auto fim = steady_clock::now();
    auto duracao = duration_cast<milliseconds>(fim - inicio);

    if (tabelaFicouCheia)
    {
        std::cout << "Teste finalizado porque a tabela ficou cheia." << std::endl;
        CHECK(tabela.cheia());
    }
    else
    {
        CHECK(totalInsercoes == totalEntradas);
    }

    std::cout << "Tempo para " << totalInsercoes << " inserções: "
              << duracao.count() << " ms" << std::endl;

    CHECK(tabela.tamanho() == totalInsercoes);
    CHECK(tabela.checarConsistencia() == ConsistenciaStatus::OK);
}
