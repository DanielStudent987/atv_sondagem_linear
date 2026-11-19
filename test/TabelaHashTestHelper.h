//
//  TabelaHashTestHelper.h
//
//  Criado por Eiji Adachi Medeiros Barbosa
//

#ifndef TABELA_HASH_TEST_HELPER_H
#define TABELA_HASH_TEST_HELPER_H

#include "TabelaHash.h"
#include <tuple>
#include <utility>
#include <vector>

class TabelaHashTestHelper {
public:
    static std::string nomeEstado(Estado estado) {
        switch (estado) {
            case Estado::LIVRE: return "LIVRE";
            case Estado::OCUPADO: return "OCUPADO";
            case Estado::REMOVIDO: return "REMOVIDO";
        }
        return "ESTADO INVÁLIDO";
    }

    static std::string mensagemConsistencia(ConsistenciaStatus status, const std::string& operacao) {
        switch (status) {
            case ConsistenciaStatus::QUANTIDADE_INCORRETA:
                return "Na checagem de consistência após " + operacao + ", era esperado que o atributo quantidade "
                       "fosse igual ao número de posições com estado OCUPADO. Foi observado que esses valores são diferentes. "
                       "Verifique se o estado foi atualizado no próprio array e se quantidade foi ajustada corretamente.";
            case ConsistenciaStatus::CHAVE_DUPLICADA:
                return "Na checagem de consistência após " + operacao + ", era esperado que cada chave aparecesse "
                       "em apenas uma posição OCUPADO. Foi observada uma chave repetida em posições OCUPADO. "
                       "Verifique se a operação alterou indevidamente as chaves armazenadas.";
            case ConsistenciaStatus::OK:
                return "A checagem de consistência não encontrou inconsistências após " + operacao + ".";
        }
        return "A checagem de consistência retornou um status desconhecido.";
    }

    static std::string nomeResultado(const std::optional<std::string>& resultado) {
        return resultado ? "valor '" + *resultado + "'" : "std::nullopt (chave não encontrada)";
    }

    static inline size_t capacidade(TabelaHash& tabela){
        return tabela.capacidade;
    }

    static inline int hash(TabelaHash& tabela, std::string chave){
        return tabela.hash(chave);
    }

    static inline Elemento getElementoNaPosicao(const TabelaHash& tabela, int posicao) {
        return tabela.array[posicao]; 
    }

    static inline void setElementoNaPosicao(TabelaHash& tabela, int posicao, Elemento elemento) {
        tabela.array[posicao] = elemento; 
    }

    static inline void setElementoNaPosicao(TabelaHash& tabela, int posicao, std::string chave, std::string valor) {
        tabela.array[posicao].chave = chave;
        tabela.array[posicao].valor = valor;
        tabela.array[posicao].estado = Estado::OCUPADO;
    }

    static inline void setQuantidade(TabelaHash& tabela, int qtd){
        tabela.quantidade = qtd;
    }

    static void preencherTabela(TabelaHash& tabela, std::vector<std::pair<int,Elemento>> dados){
        int contador = 0;
        for( auto e : dados ){
            auto posicao = e.first;
            auto elemento = e.second;
            setElementoNaPosicao(tabela, posicao, elemento);

            if(elemento.estado == Estado::OCUPADO){
                contador++;
            }
        }
        setQuantidade(tabela, contador);
    }

    static void preencherTabela(TabelaHash& tabela, std::vector<std::tuple<int, std::string, std::string>> dados){
        for( auto e : dados ){
            auto posicao = std::get<0>(e);
            auto chave = std::get<1>(e);
            auto valor = std::get<2>(e);
            setElementoNaPosicao(tabela, posicao, chave, valor);
        }
        setQuantidade(tabela, dados.size());
    }

    static void preencherTabelaComOcupado(TabelaHash& tabela){
        for (int i = 0; i < tabela.capacidade; ++i) {
            tabela.array[i].estado = Estado::OCUPADO;
            tabela.array[i].chave = "Chave-" + std::to_string(i);
            tabela.array[i].valor = "Valor-" + std::to_string(i);
        }
        setQuantidade(tabela, tabela.capacidade);
    }

    static void preencherTabelaComRemovido(TabelaHash& tabela){
        for (int i = 0; i < tabela.capacidade; ++i) {
            tabela.array[i].estado = Estado::REMOVIDO;
        }
        setQuantidade(tabela, 0);
    }

    static void aumentar(TabelaHash& tabela){
        tabela.aumentar();
    }

    static bool todosPermanecem(TabelaHash& tabela, std::vector<std::tuple<int, std::string, std::string>> dados)
    {
        for(auto dado : dados)
        {
            auto chave = std::get<1>(dado);
            auto valor = std::get<2>(dado);
            
            bool achou = false;
            
            for(int i = 0; i < tabela.capacidade; i++)
            {
                if( tabela.array[i].estado == Estado::OCUPADO &&
                    tabela.array[i].chave == chave &&
                    tabela.array[i].valor == valor )
                {
                    achou = true;
                    break;
                }
            }
            if(!achou)
            {
                return false;
            }
        }
        return true;
    }

    static bool todosJaEstavam(TabelaHash& tabela, std::vector<std::tuple<int, std::string, std::string>> dados)
    {
        for(int i = 0; i < tabela.capacidade; i++)
        {
            if(tabela.array[i].estado == Estado::OCUPADO)
            {
                bool achou = false;
                for(auto dado : dados)
                {
                    auto chave = std::get<1>(dado);
                    auto valor = std::get<2>(dado);

                    if( tabela.array[i].chave == chave &&
                        tabela.array[i].valor == valor )
                    {
                        achou = true;
                        break;
                    }
                }
                if(!achou){
                    return false;
                }
            }
        }

        return true;
    }
};

#endif // TABELA_HASH_TEST_HELPER_H
