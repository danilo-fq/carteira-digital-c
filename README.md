# Carteira Digital (C)

Simulação de uma carteira digital / banco em linguagem C, desenvolvida como
exercício da disciplina de **Algoritmos** (P2 — UNIPE).

---

## 📋 Sobre o projeto

O programa simula um banco com `n` clientes. Cada cliente possui nome, número
de conta (começando em `1000`), até **2 contas** e um histórico das **últimas
5 transações**.

A aplicação tem dois perfis de acesso: **Funcionário** (abre o banco, cadastra
clientes, emite relatório e efetua o sorteio) e **Cliente** (acessa a conta e
verifica se está negativado).

### Regras de negócio

| Regra | Condição |
|---|---|
| **Empréstimo** | Ter conta **corrente + poupança**, saldo positivo na poupança e nunca ter feito empréstimo antes (limite de **1 por cliente**) |
| **Limite do empréstimo** | `25% × 10 × (saldo total das contas)` |
| **Título de capitalização** | Ter conta **salário** com saldo entre **R$ 1.000,00 e R$ 1.500,00** |
| **Saldo negativo** | Conta **corrente** com saldo abaixo de zero |
| **Contas incompatíveis** | Conta corrente e conta salário **não** podem coexistir no mesmo cliente |
| **Número de contas** | 1 ou 2 contas, obrigatoriamente de tipos diferentes |

### Sorteio

Participam do sorteio os clientes com `comprouTitulo == 1`. Entre eles, os até
**5** com maior saldo na conta salário disputam. Para cada participante é
gerada uma aposta aleatória de 1 a 15; calcula-se a **soma de Fibonacci** até
essa posição e, se o resultado for **primo**, o participante concorre. Vence
quem obtiver a maior soma prima. O contemplado recebe um prêmio de
`10 × (saldo da conta salário)`, creditado na própria conta salário.

### Relatório

Ao emitir o relatório, o programa exibe:

1. Saldo total do banco;
2. Clientes aptos a empréstimo (com o limite calculado);
3. Clientes com saldo negativo na conta corrente;
4. Clientes aptos a compra de títulos;
5. Cliente contemplado no sorteio (se o sorteio já tiver sido efetuado).

---

## 🗂️ Estrutura do projeto

```
carteira-digital-c/
├── main.c                       # Ponto de entrada, menus e clientes de teste
├── banco.c                      # Implementação das regras de negócio e do sorteio
├── meuBanco.h                   # Structs, enums e protótipos (contrato entre .c)
├── enunciado.c                  # Enunciado do exercício (comentário, não compila)
├── .gitignore
└── README.md
```

### Estruturas de dados

```c
typedef enum {
  EMPRESTIMO = 1,
  SALDO_NEGATIVO,
  CONTA_SALARIO_FAIXA_SALDO,
} TipoCondicao;

typedef enum { CONTA_CORRENTE = 1, CONTA_SALARIO, CONTA_POUPANCA } TipoConta;

typedef struct {
  char tipoTransacao;   // 'D' depósito, 'S' saque, 'T' título, 'E' empréstimo
  float valor;
} Transacao;

typedef struct {
  TipoConta tipo;
  float saldo;
} Conta;

typedef struct {
  char nome[20];
  int numeroConta;
  int fezEmprestimo;
  int qtdContas;
  int qtdTransacoes;
  int comprouTitulo;
  float premioSorteio;
  Conta contas[2];
  Transacao historico[5];
} Cliente;
```

> ⚠️ **Atenção ao inicializar um `Cliente` por chaves:** a ordem dos campos
> importa. Os campos escalares (`numeroConta`, `fezEmprestimo`, `qtdContas`,
> `qtdTransacoes`, `comprouTitulo`, `premioSorteio`) vêm **antes** de `contas`
> e `historico`. Omitir ou trocar a ordem desloca os inicializadores seguintes
> e gera erros como *"braces around scalar initializer"*. Veja os clientes de
> teste em `main.c` como referência.

---

## 🔨 Compilação

Requer um compilador C (GCC recomendado). `main.c` e `banco.c` são compilados
juntos; `meuBanco.h` é o contrato entre eles.

### Linux

```bash
gcc main.c banco.c -o carteira
```

### Windows (PowerShell / CMD)

```powershell
gcc main.c banco.c -o build/carteira.exe
```

---

## ▶️ Execução

```bash
./carteira          # Linux
```

```powershell
.\carteira.exe      # Windows
```

### Menu principal

```
1 - Funcionario
2 - Cliente
3 - Sair
```

**Funcionário:**

```
a - Abrir Banco        # cadastra clientes (realloc da lista)
r - Emitir Relatorio   # recalcula as listas e imprime o relatório
s - Efetuar Sorteio    # roda o sorteio entre quem comprou título
x - Sair
```

**Cliente:**

```
a - Acessar Conta      # informa o número da conta e avisa se está negativado
x - Sair
```

> O programa inicia já carregado com **clientes de teste** definidos em
> `main.c` (array `clientesTeste`), úteis para exercitar relatório e sorteio
> sem cadastrar nada. Remova esse bloco quando quiser iniciar com o banco
> vazio.

---

## 📝 Observações de implementação

- O código-fonte evita acentos nas strings de saída (`Emprestimos`,
  `Titulos`) para não quebrar em terminais com encoding diferente.
- A lista de clientes e as listas derivadas (aptos a empréstimo, saldo
  negativo, aptos a título) são alocadas dinamicamente e liberadas ao sair
  pela opção `3` do menu principal.
- `atualizarHistoricos` recalcula as três listas derivadas a cada relatório e
  a cada acesso do cliente.
- **Limpeza de tela** entre os menus via `limparTela()`, que seleciona o
  comando por sistema em tempo de compilação (`system("cls")` no Windows,
  `system("clear")` nos demais, através de `#ifdef _WIN32`). A tela é limpa no
  topo de cada menu (principal, funcionário e cliente). Após ações cujo
  resultado precisa ser lido (relatório, sorteio, acesso à conta), `pausar()`
  aguarda o Enter do usuário antes da próxima limpeza. Os `scanf` que precedem
  uma pausa são seguidos de `limparBuffer()` para que o `\n` residual não faça
  a pausa ser ignorada.
- A persistência em arquivo foi prevista (protótipos comentados em
  `meuBanco.h`), mas ainda **não** está implementada; nada é salvo entre
  execuções.
- Os protótipos no header devem sempre acompanhar os nomes definidos em
  `banco.c` — nomes divergentes geram `implicit declaration of function`,
  que é **erro** nas versões recentes do GCC.
```