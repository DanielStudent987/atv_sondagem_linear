# Tabela hash com sondagem linear

## Informações do projeto

- **Instituição:** IMD
- **Disciplina:** EDB1
- **Professor:** Prof. Eiji Adachi

## Objetivo

Implementar uma tabela hash de pares de strings com sondagem linear, estados de ocupação e redimensionamento automático.

Este projeto é uma atividade de Estruturas de Dados em C++20. O código base contém operações pendentes para implementação pelo aluno.

## Estrutura e operações esperadas

A implementação esperada é uma tabela hash que armazena pares chave-valor e oferece inserção, busca e remoção. Para resolver colisões, deve usar **endereçamento aberto por sondagem linear**, percorrendo as posições seguintes de forma circular, com retorno ao início do array.

### Objetivo da Tabela Hash

A Tabela Hash tem como objetivo fornecer:
- Inserção eficiente de pares chave/valor.
- Atualização de valor caso a chave já exista.
- Detecção e uso inteligente de posições **REMOVIDAS**.
- Redimensionamento automático da tabela conforme o fator de carga.

### Estrutura de Dados

A estrutura `TabelaHash` contém:

- `Elemento* array`: array dinâmico de estruturas com chave, valor e estado.
- `int capacidade`: tamanho do array.
- `int quantidade`: número real de elementos ativos (estado `OCUPADO`).

Cada `Elemento` possui:
- `std::string chave`
- `std::string valor`
- `Estado estado` (`LIVRE`, `OCUPADO` ou `REMOVIDO`)

### Por que usar um campo estado?

Para implementar a sondagem linear corretamente, é necessário que cada posição do array tenha um indicador de estado. O campo ``estado`` do ``Elemento`` permite distinguir entre:
- LIVRE: a posição nunca foi usada (pode encerrar buscas).
- OCUPADO: há um par chave-valor válido.
- REMOVIDO: havia uma chave, mas foi removida. A posição precisa ser ignorada na busca, mas pode ser reutilizada em futuras inserções.

Sem esse campo, não seria possível diferenciar uma posição nunca usada de uma posição que foi esvaziada por uma remoção — o que afetaria a correção da busca e da inserção.

### Interface da Classe `TabelaHash`

```cpp
class TabelaHash {
public:
	TabelaHash(int capacidadeInicial = 17);
	~TabelaHash();

	bool inserir(const std::string& chave, const std::string& valor);
	bool remover(const std::string& chave);
	std::optional<std::string> buscar(const std::string& chave) const;

	int tamanho() const;
	bool vazia() const;
	bool cheia() const;
	void imprimir() const;

	ConsistenciaStatus checarConsistencia() const;
};
```

### Funcionamento das Operações

### Inserção

- Usa `hash(chave)` para obter o índice base.
- Aplica **sondagem linear** para encontrar posição livre ou atualiza se a chave já existir.
- Se encontrar uma posição `REMOVIDO`, registra o primeiro índice disponível e continua a sondagem para verificar se a chave já existe adiante, evitando duplicatas.
- Retorna `true` ao inserir ou atualizar. Incrementa `quantidade` apenas ao inserir uma chave nova.
- Nos testes de inserção em capacidade fixa, lança `std::overflow_error` ao tentar inserir uma chave nova em uma tabela cheia; atualizar uma chave existente continua permitido.
- O crescimento automático pelo fator de carga faz parte da etapa de redimensionamento descrita abaixo.

### Remoção

- Localiza a chave e marca a posição como `REMOVIDO`.
- Reduz `quantidade` apenas quando encontra e remove uma chave, retornando `true`. Retorna `false` se a chave não existir.
- Continua a sondagem ao passar por posições `REMOVIDO`; encerra ao encontrar `LIVRE` ou após percorrer toda a capacidade.
- A redução automática pelo fator de carga faz parte da etapa de redimensionamento descrita abaixo.

### Busca

- Aplica sondagem linear a partir do índice hashado.
- Retorna o valor associado à chave, inclusive se for uma string vazia.
- Continua ao encontrar `REMOVIDO`. Retorna `std::nullopt` ao encontrar `LIVRE` ou percorrer toda a capacidade sem localizar a chave.

### Redimensionamento

- Usado para aumentar ou diminuir dinamicamente a tabela.
- Realiza nova alocação e reinserção direta dos elementos `OCUPADO`.
- O teste de crescimento espera capacidade nova igual a `2 * capacidadeAnterior + 1`.
- O objetivo da etapa automática é crescer quando o fator de carga ultrapassar 50% e diminuir quando ficar abaixo de 12,5%. Essa etapa deve ser conciliada com os testes de capacidade fixa, que preparam tabelas cheias pelo helper; o conjunto atual não testa diretamente os gatilhos automáticos de crescimento e redução.

### Sobre `std::optional`

`std::optional<std::string>` representa um resultado que pode conter uma string ou não conter nenhum valor. Em `buscar`, uma chave encontrada deve produzir o valor associado; uma chave inexistente deve produzir `std::nullopt`.

Após implementar `buscar`, você pode consultar o resultado assim:

```cpp
auto resultado = tabela.buscar("CHAVE");

if (resultado.has_value()) {
    std::cout << "Valor encontrado: " << resultado.value() << '\n';
} else {
    std::cout << "Chave não encontrada\n";
}
```

`has_value()` informa se há um valor, e `value()` acessa a string armazenada. Chamar `value()` sem um valor lança `std::bad_optional_access`; por isso, o exemplo verifica sua presença antes do acesso.

Uma string vazia (`""`) ainda é um valor encontrado: nesse caso, `has_value()` retorna `true`. Já `std::nullopt` indica ausência da chave, e `has_value()` retorna `false`.

## Atividade de implementação

Implemente as operações pendentes em `header/TabelaHash.h`, respeitando a interface pública e os contratos descritos acima. Os testes em `test/` definem os cenários que a implementação deve atender.

Métodos pendentes: `inserir`, `remover`, `buscar`, `aumentar`, `diminuir`, `redimensionar`.

No código base, esses métodos lançam `std::runtime_error("Método ainda não implementado.")`. Substitua essa exceção pela implementação de cada operação. As descrições deste README representam o **resultado esperado da implementação do aluno**, não funcionalidades já concluídas.

Os métodos auxiliares já implementados servem de apoio à atividade. Preserve suas interfaces e mantenha os testes fornecidos como referência. É esperado que os testes das operações pendentes falhem com essa mensagem antes da implementação.

## Complexidades esperadas

| Operação  | Complexidade Média | Complexidade no Pior Caso |
|-----------|--------------------|----------------------------|
| Inserir   | O(1)               | O(N)                       |
| Remover   | O(1)               | O(N)                       |
| Buscar    | O(1)               | O(N)                       |
| Imprimir  | O(N)               | O(N)                       |
| Redimensionar | O(N)           | O(N²)                      |

> O pior caso ocorre quando a tabela está muito cheia ou com muitos elementos `REMOVIDO`.

A inserção com crescimento tem custo amortizado O(1) sob boa dispersão das chaves. O array inteiro é percorrido na impressão.

> No redimensionamento, se as N chaves tiverem o mesmo índice hash na tabela nova, suas reinserções exigirão 1, 2, ..., N tentativas. A soma é N(N + 1)/2, resultando em O(N²) no pior caso. Com boa distribuição das chaves e capacidades proporcionais a N, o custo esperado é O(N).

## Estrutura de diretórios

```text
.
├── .vscode/   # Configurações do editor
├── bin/       # Executáveis e objetos gerados; objetos em bin/obj/
├── header/    # TabelaHash.h: classe e implementação da atividade
├── lib/       # Biblioteca de testes doctest
├── src/       # main.cpp: programa de exemplo
├── test/      # Testes automatizados e TabelaHashTestHelper.h
├── .gitignore # Arquivos locais e gerados ignorados pelo Git
├── makefile   # Compilação, execução e limpeza
└── README.md  # Orientações da atividade
```

A classe `TabelaHash` é implementada diretamente em `header/TabelaHash.h` e não usa templates. O helper fica em `test/TabelaHashTestHelper.h`, junto dos testes, e reúne a preparação e inspeção da tabela e as funções compartilhadas de diagnóstico. Os objetos do programa são gerados em `bin/obj/src/`; os objetos dos testes, em `bin/obj/test/`.

## Pré-requisitos

- GNU Make.
- Compilador C++ com suporte a C++20 e aos sanitizadores AddressSanitizer e UndefinedBehaviorSanitizer.
- Terminal com comandos `mkdir`, `find` e `rm`, como em Linux, macOS ou um ambiente compatível no Windows.

O doctest já está incluído em `lib/doctest.h`. O Makefile usa `g++` por padrão; para escolher outro compilador, use, por exemplo, `make CXX=clang++`.

A compilação habilita avisos, símbolos de depuração e sanitizadores para ajudar a detectar erros de memória e comportamento indefinido.

## Compilação e execução

| Comando | Descrição |
|---|---|
| `make` ou `make all` | Compila o programa principal e os testes funcionais. |
| `make run` | Compila, se necessário, e executa o programa principal (`bin/main`). |
| `make test` | Compila os testes funcionais, sem executá-los. |
| `make run-test` | Compila, se necessário, e executa os testes funcionais. |
| `make clean` | Remove os arquivos gerados, preservando `bin/` e seu `.gitkeep`. |

```bash
make
make run
make run-test
make run-test ARGS="--list-test-cases"
make clean
```

`ARGS` permite passar argumentos ao programa ou ao doctest. A execução do programa de exemplo também pode lançar a exceção de implementação pendente ao chamar uma operação ainda não concluída.

## Testes

Os testes funcionais são compilados em executáveis independentes por operação. `make run-test` executa esses testes em sequência e interrompe a execução se um executável falhar.

| Comando | Executável / finalidade |
|---|---|
| `make run-test-inserir` | `bin/test-inserir` |
| `make run-test-buscar` | `bin/test-buscar` |
| `make run-test-remover` | `bin/test-remover` |
| `make run-test-redimensionar` | `bin/test-redimensionar` |
| `make run-test-desempenho` | Compila e executa `bin/test-desempenho`. |

### Como interpretar as falhas

Os testes de inserção, busca e remoção incluem contexto com `INFO` e `CAPTURE` e mensagens em português com `CHECK_MESSAGE` e `REQUIRE_MESSAGE`. As mensagens indicam o comportamento esperado e o resultado observado, como o estado `OCUPADO` quando era esperado `REMOVIDO`, ou `std::nullopt` quando era esperado um valor.

O helper compartilha os nomes dos estados, a descrição dos resultados de busca e as mensagens específicas de consistência. `QUANTIDADE_INCORRETA` indica divergência entre o atributo `quantidade` e o número de posições `OCUPADO`; `CHAVE_DUPLICADA` indica repetição de chave em posições `OCUPADO`. A mensagem menciona apenas a inconsistência retornada pela checagem.

As verificações de pré-condição com `REQUIRE_MESSAGE` interrompem o caso de teste se a preparação estiver incorreta. As verificações com `CHECK_MESSAGE` permitem continuar e mostrar outras falhas do mesmo cenário.

Para executar uma operação isoladamente ou exibir também as verificações que passam:

```bash
make run-test-remover
make run-test-inserir ARGS="--success"
make run-test-buscar ARGS="--test-case=*REMOVIDO*"
```

Os métodos pendentes continuam lançando a exceção de método não implementado. Uma compilação bem-sucedida não significa que os testes passarão antes de esses métodos serem implementados.

### Teste de desempenho

O teste de desempenho é separado: `make`, `make test` e `make run-test` não o compilam nem o executam. Execute-o explicitamente após implementar e validar as operações funcionais.

```bash
make run-test-inserir
make run-test-desempenho
```

O tempo exibido pelo teste de desempenho é medido com a configuração de depuração e os sanitizadores habilitados; serve como observação nesse ambiente, não como medição de uma compilação otimizada.
