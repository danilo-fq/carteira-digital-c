# Carteira Digital (C)

Simulação de uma carteira digital / banco em linguagem C, desenvolvida como
exercício da disciplina de **Algoritmos** (P2 — UNIPE).

> 🚧 **Status: em desenvolvimento.**
> O programa já compila e executa, mas ainda não implementa todas as
> funcionalidades previstas no enunciado. Veja a seção
> [O que falta](#-o-que-falta).

---

## 📋 Sobre o projeto

O programa simula um banco com `n` clientes. Cada cliente possui nome, número
de conta (começando em `1000`), até **2 contas** e o histórico das **últimas 5
transações**.

As regras de negócio são:

| Regra | Condição |
|---|---|
| **Empréstimo** | Ter conta **corrente + poupança**, saldo positivo na poupança e nunca ter feito empréstimo antes (limite de **1 por cliente**) |
| **Limite do empréstimo** | `25% × 10 × (saldo total das contas)` |
| **Título de capitalização** | Ter conta **salário** com saldo entre **R$ 1.000,00 e R$ 1.500,00** |
| **Saldo negativo** | Conta **corrente** com saldo abaixo de zero |
| **Contas incompatíveis** | Conta corrente e conta salário **não** podem coexistir no mesmo cliente |
| **Número de contas** | até 2 contas, obrigatoriamente de tipos diferentes |

### Saída do relatório

Ao consultar o histórico, o programa exibe:

1. Saldo total do banco;
2. Clientes aptos a empréstimo;
3. Clientes com saldo negativo na conta corrente;
4. Clientes aptos a compra de títulos;
5. Cliente contemplado no sorteio.

---

## 🗂️ Estrutura do projeto

```
carteira-digital-c/
├── main.c        # Ponto de entrada, menu principal e clientes de teste
├── banco.c       # Implementação das regras de negócio
├── meuBanco.h    # Structs, enums e protótipos (contrato entre main.c e banco.c)
├── enunciado.c   # Enunciado do exercício (apenas comentário, não compila nada)
├── build/        # Artefatos de compilação (ignorado pelo Git)
├── .gitignore
└── README.md
```

### Estruturas de dados

```c
typedef enum { CONTA_CORRENTE = 1, CONTA_SALARIO, CONTA_POUPANCA } TipoConta;

typedef struct {
  char tipoTransacao;   // 'D' = depósito, 'S' = saque // 'T' = Titulo de capitalização // 'E' = Emprestimo
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
  Conta contas[2];
  Transacao historico[5];
} Cliente;
```

> ⚠️ **Atenção ao inicializar um `Cliente`:** a ordem dos campos importa.
> `qtdTransacoes` fica **entre** `qtdContas` e `contas`. Omitir esse campo
> desloca todos os inicializadores seguintes e gera erros como
> *"braces around scalar initializer"*.

---

## 🔨 Compilação

Todos os artefatos de compilação devem ir para a pasta **`build/`**, que já
está listada no `.gitignore` (assim nenhum executável vai parar no
repositório).

### Linux

```bash
mkdir -p build
gcc main.c banco.c -o build/carteira
```

### Windows (PowerShell / CMD)

```powershell
mkdir build
gcc main.c banco.c -o build/carteira.exe
```

> O GCC **não** cria diretórios. Se `build/` não existir, a compilação falha
> com `No such file or directory`.

---

## ▶️ Execução

```bash
./build/carteira          # Linux
```

```powershell
.\build\carteira.exe      # Windows
```

### Menu principal

```
a - Abrir Banco
c - Consultar Historico
s - Sair
```

- **`a`** — abre o menu do banco, onde é possível cadastrar clientes
  (`c`) ou voltar ao menu principal (`s`).
- **`c`** — recalcula as listas de clientes (aptos a empréstimo, saldo
  negativo, aptos a título) e imprime o relatório.
- **`s`** — encerra o programa.

---

## 🚧 O que falta

Itens ainda não implementados ou incompletos:

- [ ] **Movimentações financeiras** — não há depósito, saque nem contratação
      de empréstimo/título pela interface. O histórico é apenas populado pelos
      clientes de teste.
- [ ] **Sorteio** — a seção "Cliente Contemplado no Sorteio" é impressa, mas
      nenhum cliente é sorteado.
- [ ] **Persistência em arquivo** — as funções de arquivo estão comentadas em
      `meuBanco.h`; nada é salvo entre execuções.
- [ ] **Atualização do histórico** — `qtdTransacoes` não é atualizado após
      operações; `historico` é um buffer circular ainda não implementado.
- [ ] **`fezEmprestimo`** — o campo existe e é validado, mas nunca é alterado
      para `1` (falta a operação de contratar empréstimo).
- [ ] **Limpeza de tela** — `system("cls")` / `system("clear")` ainda não foi
      aplicado.

---

## 📝 Observações de implementação

- O código-fonte evita acentos nas strings de saída (`Emprestimos`,
  `Titulos`) para não quebrar em terminais com encoding diferente.
- `main.c` e `banco.c` são compilados juntos; `meuBanco.h` funciona como
  contrato único entre eles.
- Os protótipos no header devem sempre acompanhar os nomes definidos em
  `banco.c` — nomes divergentes geram `implicit declaration of function`,
  que é **erro** nas versões recentes do GCC.
