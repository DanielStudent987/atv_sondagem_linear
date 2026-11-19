//
//  TabelaHash.h
//
//  Created by Eiji Adachi Medeiros Barbosa
//

#ifndef TABELA_HASH_H
#define TABELA_HASH_H

#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_set>
#include <iostream>

enum class Estado 
{ 
    LIVRE = 0, 
    OCUPADO, 
    REMOVIDO 
};

struct Elemento 
{
        std::string chave;
        std::string valor;
        Estado estado = Estado::LIVRE;
};

enum class ConsistenciaStatus {
    OK = 0,
    QUANTIDADE_INCORRETA,
    CHAVE_DUPLICADA,
};

class TabelaHash 
{
    friend class TabelaHashTestHelper;

private:
    Elemento* array;
    int capacidade;
    int quantidade;    
    
    static constexpr int CAPACIDADE_PADRAO = 17;
    static constexpr float CARGA_LIMITE_INFERIOR = 0.125f;
    static constexpr float CARGA_LIMITE_SUPERIOR = 0.5f;

    size_t valorHash(const std::string& chave) const 
    {
        size_t valorHash = 0;
		for (char c : chave) {
			// Função simples para facilitar colisões em testes
			valorHash += static_cast<unsigned int>(c);
			// Forma melhor de calcular valor hash que gera valores mais distribuídos
			// valorHash = 31 * valorHash + static_cast<unsigned int>(c);
		}
        return valorHash;
    }

    int hash(const std::string& chave) const 
    {
        return this->valorHash(chave) % this->capacidade; 
    }

    float fatorDeCarga() 
    {
        return (float)quantidade / (float)capacidade;
    }

    void aumentar()
    {
        throw std::runtime_error("Método ainda não implementado.");
    }

    void diminuir()
    {
        throw std::runtime_error("Método ainda não implementado.");
    }

    void redimensionar([[maybe_unused]] size_t tamanhoNovo)
    {
        throw std::runtime_error("Método ainda não implementado.");
    }

public:
    TabelaHash(int capacidadeInicial = CAPACIDADE_PADRAO) : capacidade(capacidadeInicial) , quantidade(0)
    {
        this->array = new Elemento[this->capacidade];
    }

    ~TabelaHash()
    {
        delete [] this->array;
    }

    bool inserir([[maybe_unused]] const std::string& chave, [[maybe_unused]] const std::string& valor)
    {
        throw std::runtime_error("Método ainda não implementado.");
    }

    bool remover([[maybe_unused]] const std::string& chave)
    {
        throw std::runtime_error("Método ainda não implementado.");
    }

    std::optional<std::string> buscar([[maybe_unused]] const std::string& chave) const
    {
        auto valorHash = this->valorHash(chave);

        for (int delta = 0; delta<capacidade; delta++) {

            int indice = (valorHash+delta) % capacidade;
            auto& elemento = this->array[indice];

            if (elemento.estado == Estado::OCUPADO && elemento.chave == chave) {
                return elemento.valor; 

            } else if (elemento.estado == Estado::LIVRE) {
                return std::nullopt; 

            }
        }
        return std::nullopt;

    }

    ConsistenciaStatus checarConsistencia() const
    {
        int contador = 0;
        std::unordered_set<std::string> chaves;

        for (int i = 0; i < this->capacidade; ++i)
        {
            const Elemento& e = this->array[i];

            if (e.estado == Estado::OCUPADO)
            {
                contador++;

                // Verifica duplicatas
                if (chaves.count(e.chave) > 0)
                {
                    return ConsistenciaStatus::CHAVE_DUPLICADA;
                }
                chaves.insert(e.chave);
            }
        }

        if (contador != this->quantidade)
        {
            return ConsistenciaStatus::QUANTIDADE_INCORRETA;
        }

        return ConsistenciaStatus::OK;
    }

    int tamanho() const 
    {
        return quantidade;
    }

    bool vazia() const
    {
        return this->quantidade == 0;
    }

    bool cheia() const 
    {
        return this->capacidade == this->quantidade;
    }

    void imprimir() const
    {
        for (int i = 0; i < this->capacidade; ++i)
        {
            const Elemento& e = this->array[i];

            std::cout << "[" << i << "]: ";

            switch (e.estado)
            {
                case Estado::LIVRE:
                    std::cout << "LIVRE";
                    break;
                case Estado::REMOVIDO:
                    std::cout << "REMOVIDO";
                    break;
                case Estado::OCUPADO:
                    std::cout << "OCUPADO (" << e.chave << ", " << e.valor << ")";
                    break;
            }

            std::cout << std::endl;
        }
    }
};

#endif /* TABELA_HASH_H */
