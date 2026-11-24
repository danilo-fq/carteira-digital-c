/*Declarações*/
#include <stdio.h>
#ifndef MEU_BANCO
#define MEU_BANCO

typedef enum {
  EMPRESTIMO = 1,
  SALDO_NEGATIVO,
  CONTA_SALARIO_FAIXA_SALDO,
} TipoCondicao;

typedef enum {
  CONTA_CORRENTE = 1,
  CONTA_SALARIO,
  CONTA_POUPANCA,
} TipoConta;
typedef struct {
  char tipoTransacao;
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

void abrirBanco(Cliente **listaClientes, int *qtdClientes);

void emitirHistorico(
  Cliente *listaClientes,
  int qtdClientes,
  Cliente *listaClientesAptosEmprestimo,
  int qtdClientesAptosEmprestimo,
  Cliente *listaClientesSaldoNegativo,
  int qtdClientesNegativos,
  Cliente *listaClientesAptosCompraTitulos,
  int qtdClientesAptosCompraTitulos,
  float saldoBanco,
  int realizouSorteio
);

int atualizarHistoricos(
  Cliente *listaClientes,
  int qtdClientes,
  Cliente **listaClientesAptosEmprestimo,
  int *qtdClientesAptosEmprestimo,
  Cliente **listaClientesSaldoNegativo,
  int *qtdClientesSaldoNegativo,
  Cliente **listaClientesAptosCompraTitulos,
  int *qtdClientesAptosCompraTitulos
);

void efetuarSorteio(Cliente *listaClientes, int qtdClientes);

// void fecharArquivo(FILE *arquivo);
// FILE *abrirArquivo(char *caminho, char *modo);
// void lerArquivo(char *caminho);
// FILE *escreverArquivo(char *caminho, char *texto);

int validarTiposConta(Cliente cliente);
int validarTipoConta(TipoConta tipo);
int podeFazerEmprestimo(Cliente cliente);
int podeComprarTituloCapitalizacao(Cliente cliente);

float calcularLimiteEmprestimo(Cliente cliente);
float calcularSaldoBanco(Cliente *listaClientes, int qtdClientes);
float buscaSaldoClientePorTipo(Cliente cliente, TipoConta tipo);

void cadastrarCliente(Cliente *listaClientes, int *qtdClientes, int indiceAtual);
void ordernarOrdemDecrescenteSaldoContaSalario(Cliente *listaClientes, int qtdParticipantes, int *listaIndicesParticipantes);

int fibonacci(int);
int somaFibonacci(int);
int ehPrimo(int);


char converterMinuscula(char letra);

void limparBuffer();
void limparTela();
void pausar();

FILE *abrirArquivo(const char *caminho, const char *modo);
void fecharArquivo(FILE *arquivo);
void lerArquivo(const char *caminho);
int gravarArquivo(const char *caminho, const char *texto, const char *modo);

#endif
