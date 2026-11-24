/*Funções*/
#include "meuBanco.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void abrirBanco(Cliente **listaClientes, int *qtdClientes)
{
  char opcaoBanco = 's';
  do
  {
    printf("\nc - Cadastrar Cliente\n");
    printf("s - Voltar para menu principal\n");
    printf("Digite a opcao desejada: ");
    scanf(" %c", &opcaoBanco);
    switch (opcaoBanco)
    {
    case 'c':
    {
      int qtdNovosClientes = -1;
      int verificacaoInput = -1;
      while (qtdNovosClientes <= 0 || verificacaoInput == 0)
      {
        printf("Digite a quantidade de clientes que deseja cadastrar: ");
        verificacaoInput = scanf("%d", &qtdNovosClientes);
        if (qtdNovosClientes <= 0 || verificacaoInput == 0)
        {
          printf("\n[ERRO]: Entrada invalido, digite um numero inteiro maior do que zero!\n");
        }
      }
      int qtdAnterior = *qtdClientes;
      int qtdTotalClientes = qtdAnterior + qtdNovosClientes;

      if (qtdAnterior == 0)
      {
        Cliente *temp = (Cliente *)malloc(sizeof(Cliente) * qtdTotalClientes);

        if (temp == NULL)
        {
          printf("Erro ao alocar memoria na lista de clientes\n");
          break;
        }
        *listaClientes = temp;
      }
      else
      {
        Cliente *temp = (Cliente *)realloc(*listaClientes, sizeof(Cliente) * qtdTotalClientes);
        if (temp == NULL)
        {
          printf("Erro ao alocar memoria na lista de clientes\n");
          break;
        }

        *listaClientes = temp;
      }

      *qtdClientes = qtdTotalClientes;
      cadastrarCliente(*listaClientes, qtdClientes, qtdAnterior);
      break;
    }
    case 's':
      printf("\nVoltando ao menu principal...\n");
      break;
    default:
      printf("\nOpcao invalida!\n");
      break;
    };
  } while (opcaoBanco != 's');
}

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
)
{
  printf("\n== Saldo Total do banco ==\n");
  printf("%d | R$ %.2f\n", qtdClientes, saldoBanco);

  printf("\n== Clientes Aptos a Emprestimos ==\n");

  for (int i = 0; i < qtdClientesAptosEmprestimo; i++)
  {
    printf(
        "%s | %d-0%d | R$ %.2f\n",
        listaClientesAptosEmprestimo[i].nome,
        listaClientesAptosEmprestimo[i].numeroConta,
        CONTA_POUPANCA,
        calcularLimiteEmprestimo(listaClientesAptosEmprestimo[i]));
  }

  printf("\n== Clientes com Saldo Negativo na Conta Corrente ==\n");
  for (int i = 0; i < qtdClientesNegativos; i++)
  {
    printf(
      "%s | %d-0%d | R$ %.2f\n",
      listaClientesSaldoNegativo[i].nome,
      listaClientesSaldoNegativo[i].numeroConta,
      CONTA_CORRENTE,
      buscaSaldoClientePorTipo(listaClientesSaldoNegativo[i], CONTA_CORRENTE)
    );
  }

  printf("\n== Clientes Aptos a Compra de Titulos ==\n");

  for (int i = 0; i < qtdClientesAptosCompraTitulos; i++)
  {
    printf(
        "%s | %d-0%d | R$ %.2f\n",
        listaClientesAptosCompraTitulos[i].nome,
        listaClientesAptosCompraTitulos[i].numeroConta,
        CONTA_SALARIO,
        buscaSaldoClientePorTipo(listaClientesAptosCompraTitulos[i], CONTA_SALARIO));
  }

  printf("\n== Cliente Contemplado no Sorteio ==\n");
  if(realizouSorteio == 1)
  {
    int temGanhador = 0;
    int temParticipante = 0;
    for (int i = 0; i < qtdClientes; i++)
    {
      if (listaClientes[i].comprouTitulo == 1) temParticipante = 1;
      if (listaClientes[i].premioSorteio > 0)
      {
        printf(
          "Contemplado: %s | %d-0%d | Premio R$ %.2f\n",
          listaClientes[i].nome,
          listaClientes[i].numeroConta,
          CONTA_SALARIO,
          listaClientes[i].premioSorteio
        );
        temGanhador = 1;
        temParticipante = 1;
      }
    }

    if(!temParticipante)
    {
      printf("Nao houve participantes para o sorteio!\n");
    }

    if (temParticipante == 1 && !temGanhador)
    {
      printf("Nenhum contemplado neste sorteio\n");
    }
  }
  else
  {
    printf("Sorteio Nao Realizado\n");
  }
}

int validarTipoConta(TipoConta tipo)
{
  return (
      tipo == CONTA_CORRENTE ||
      tipo == CONTA_POUPANCA ||
      tipo == CONTA_SALARIO);
}

int validarTiposConta(Cliente cliente)
{
  if (cliente.qtdContas <= 0 || cliente.qtdContas > 2)
  {
    printf("\n [ERRO]: Numero de contas invalido. Numero de contas permitidos: 1 ou 2 contas!\n");
    return 0;
  }

  if (
      cliente.qtdContas == 2 &&
      (!validarTipoConta(cliente.contas[0].tipo) ||
      !validarTipoConta(cliente.contas[1].tipo)))
  {
    printf("\n [ERRO]: Uma ou duas opcoes de conta esta(ao) invalida(s)!\n");
    return 0;
  }

  if (cliente.qtdContas == 2 && (cliente.contas[0].tipo == cliente.contas[1].tipo))
  {
    printf("\n [ERRO]: As Contas nao podem ser do mesmo tipo!\n");
    return 0;
  }

  if (cliente.qtdContas == 2 && (cliente.contas[0].tipo != cliente.contas[1].tipo))
  {
    if (
        (cliente.contas[0].tipo == CONTA_CORRENTE && cliente.contas[1].tipo == CONTA_SALARIO) ||
        (cliente.contas[0].tipo == CONTA_SALARIO && cliente.contas[1].tipo == CONTA_CORRENTE))
    {
      printf("\n [ERRO]: Nao eh possivel criar conta corrente e conta salario juntas!\n");
      return 0;
    }
    else
    {
      return 1;
    }
  }

  if (
      !validarTipoConta(cliente.contas[0].tipo))
  {
    printf("\n [ERRO]: Tipo de conta desconhecido, utilize uma das opcoes validas! \n");
    return 0;
  }

  return 1;
}

void cadastrarCliente(Cliente *listaClientes, int *qtdClientes, int indiceAtual)
{
  for (int i = indiceAtual; i < (*qtdClientes); i++)
  {
    Cliente novoCliente = {0};
    int numeroConta = 1000 + i;
    printf("Criando cliente %d:\n", i + 1);
    printf("Nome: ");
    limparBuffer();
    fgets(novoCliente.nome, sizeof(novoCliente.nome), stdin);
    novoCliente.nome[strcspn(novoCliente.nome, "\n")] = '\0';

    printf("Numero da Conta do cliente: %d", numeroConta);
    novoCliente.numeroConta = numeroConta;
    novoCliente.qtdContas = 1;

    do
    {
      int tipoDeConta1 = 0;
      printf("===SELECIONANDO TIPO DE CONTA===\n");
      printf("1 - CONTA CORRENTE\n");
      printf("2 - CONTA SALARIO\n");
      printf("Selecione o tipo da conta: ");
      scanf("%d", &tipoDeConta1);

      novoCliente.contas[0].tipo = tipoDeConta1;
      novoCliente.contas[0].saldo = 0;
      
      int desejaContaPoupanca = 0;
      
      printf("\nDeseja criar uma CONTA POUPANCA?\n");
      printf("1 - SIM\n");
      printf("2 - NAO\n");
      printf("Opcao: ");
      scanf("%d", &desejaContaPoupanca);
      
      if (desejaContaPoupanca == 1)
      {
        novoCliente.qtdContas = 2;
        novoCliente.contas[1].tipo = CONTA_POUPANCA;
        novoCliente.contas[1].saldo = 0;
      }
    } while (!validarTiposConta(novoCliente));

    listaClientes[i] = novoCliente;
    printf("Cliente Cadastrado com sucesso!\n");
  }
}

float calcularSaldoBanco(Cliente *listaClientes, int qtdClientes)
{
  float total = 0;
  for (int i = 0; i < qtdClientes; i++)
  {
    for (int j = 0; j < listaClientes[i].qtdContas; j++)
    {
      total += listaClientes[i].contas[j].saldo;
    }
  }
  return total;
}

float buscaSaldoClientePorTipo(Cliente cliente, TipoConta tipo)
{
  float total = 0;
  for (int i = 0; i < cliente.qtdContas; i++)
  {
    if (cliente.contas[i].tipo == tipo)
    {
      total += cliente.contas[i].saldo;
    }
  }
  return total;
}

int podeFazerEmprestimo(Cliente cliente)
{
  if (cliente.fezEmprestimo == 1)
  {
    return 0;
  }

  if (
      cliente.qtdContas == 2)
  {
    if (
        (cliente.contas[0].tipo == CONTA_CORRENTE && cliente.contas[1].tipo == CONTA_POUPANCA) ||
        (cliente.contas[0].tipo == CONTA_POUPANCA && cliente.contas[1].tipo == CONTA_CORRENTE))
    {
      if (
          (cliente.contas[0].tipo == CONTA_POUPANCA && cliente.contas[0].saldo > 0) ||
          (cliente.contas[1].tipo == CONTA_POUPANCA && cliente.contas[1].saldo > 0))
      {
        return 1;
      }
      return 0;
    }
    return 0;
  }
  return 0;
}

int podeComprarTituloCapitalizacao(Cliente cliente)
{
  int resultado = 0;
  for (int i = 0; i < cliente.qtdContas; i++)
  {
    if (
      cliente.contas[i].tipo == CONTA_SALARIO &&
      cliente.contas[i].saldo >= 1000 && 
      cliente.contas[i].saldo <= 1500
    )
    {
      resultado = 1;
    }
  }

  return resultado;
}

float calcularLimiteEmprestimo(Cliente cliente)
{
  if (podeFazerEmprestimo(cliente))
  {
    return 0.25 * 10 * (cliente.contas[0].saldo + cliente.contas[1].saldo); // 25% x 10 vezes x saldo total contas 
  }
  return 0;
}

int atualizarHistoricos(
    Cliente *listaClientes,
    int qtdClientes,
    Cliente **listaClientesAptosEmprestimo,
    int *qtdClientesAptosEmprestimo,
    Cliente **listaClientesSaldoNegativo,
    int *qtdClientesSaldoNegativo,
    Cliente **listaClientesAptosCompraTitulos,
    int *qtdClientesAptosCompraTitulos)
{
  int indiceAtualEmprestimo = 0;
  int indiceAtualSaldoNegativo = 0;
  int indiceAtualAptosCompraTitulos = 0;
  *qtdClientesAptosEmprestimo = 0;
  *qtdClientesSaldoNegativo = 0;
  *qtdClientesAptosCompraTitulos = 0;

  free(*listaClientesAptosEmprestimo);
  free(*listaClientesSaldoNegativo);
  free(*listaClientesAptosCompraTitulos);

  *listaClientesAptosEmprestimo = NULL;
  *listaClientesSaldoNegativo = NULL;
  *listaClientesAptosCompraTitulos = NULL;

  if (qtdClientes <= 0)
  {
    return 0;
  }

  for (int i = 0; i < qtdClientes; i++)
  {

    if (podeFazerEmprestimo(listaClientes[i]))
    {
      (*qtdClientesAptosEmprestimo)++;
    }

    for (int j = 0; j < listaClientes[i].qtdContas; j++)
    {
      if (
          listaClientes[i].contas[j].tipo == CONTA_CORRENTE &&
          listaClientes[i].contas[j].saldo < 0)
      {
        (*qtdClientesSaldoNegativo)++;
      }
      if (
          listaClientes[i].contas[j].tipo == CONTA_SALARIO &&
          (listaClientes[i].contas[j].saldo >= 1000 && listaClientes[i].contas[j].saldo <= 1500))
      {
        (*qtdClientesAptosCompraTitulos)++;
      }
    }
  }

  if (*qtdClientesAptosEmprestimo > 0)
  {
    *listaClientesAptosEmprestimo = (Cliente *)malloc(sizeof(Cliente) * (*qtdClientesAptosEmprestimo));
    if (*listaClientesAptosEmprestimo == NULL)
    {
      printf("[ERRO]: locacao memoria aptos emprestimo\n");
      return 0;
    }
  }

  if (*qtdClientesSaldoNegativo > 0)
  {
    *listaClientesSaldoNegativo = (Cliente *)malloc(sizeof(Cliente) * (*qtdClientesSaldoNegativo));
    if (*listaClientesSaldoNegativo == NULL)
    {
      printf("[ERRO]: locacao memoria lista saldo negativo conta corrente\n");
      return 0;
    }
  }

  if (*qtdClientesAptosCompraTitulos > 0)
  {
    *listaClientesAptosCompraTitulos = (Cliente *)malloc(sizeof(Cliente) * (*qtdClientesAptosCompraTitulos));
    if (*listaClientesAptosCompraTitulos == NULL)
    {
      printf("[ERRO]: locacao memoria lista conta salario faixa saldo\n");
      return 0;
    }
  }

  for (int i = 0; i < qtdClientes; i++)
  {

    if (podeFazerEmprestimo(listaClientes[i]))
    {
      (*listaClientesAptosEmprestimo)[indiceAtualEmprestimo] = listaClientes[i];
      indiceAtualEmprestimo++;
    }

    for (int j = 0; j < listaClientes[i].qtdContas; j++)
    {
      if (
        listaClientes[i].contas[j].tipo == CONTA_CORRENTE &&
        listaClientes[i].contas[j].saldo < 0
      )
      {
        (*listaClientesSaldoNegativo)[indiceAtualSaldoNegativo] = listaClientes[i];
        indiceAtualSaldoNegativo++;
      }
      if (
        listaClientes[i].contas[j].tipo == CONTA_SALARIO &&
        listaClientes[i].contas[j].saldo >= 1000 &&
        listaClientes[i].contas[j].saldo <= 1500
      )
      {
        (*listaClientesAptosCompraTitulos)[indiceAtualAptosCompraTitulos] = listaClientes[i];
        indiceAtualAptosCompraTitulos++;
      }
    }
  }
  return 1;
}

void efetuarSorteio(Cliente *listaClientes, int qtdClientes)
{
  int qtdParticipantes = 0;
  int indiceVencedor = -1;
  int maiorSomaPrima = -1;
  
  for (int i = 0; i < qtdClientes; i++)
  {
    listaClientes[i].premioSorteio = 0.0f;
    if (listaClientes[i].comprouTitulo == 1)
    {
      qtdParticipantes++;
    }
  }
  
  if (!qtdParticipantes)
  {
    printf("Nao houve participantes para o sorteio!\n");
    return;
  }
  
  int *indiceParticipantesSorteio = (int *) malloc(sizeof(int) * qtdParticipantes);
  int indiceAtual = 0;
  
  if(indiceParticipantesSorteio == NULL)
  {
    printf("[ERRO]: erro ao alocar memoria dos participantes\n");
    return;
  }
  
  for(int i = 0; i < qtdClientes; i++)
  {
    if(listaClientes[i].comprouTitulo == 1)
    {
      indiceParticipantesSorteio[indiceAtual] = i;
      indiceAtual++;
    }
  }
  
  ordernarOrdemDecrescenteSaldoContaSalario(listaClientes, qtdParticipantes, indiceParticipantesSorteio);

  int qtdFinal = qtdParticipantes;

  if(qtdFinal > 5) qtdFinal = 5;

  for (int i = 0; i < qtdFinal; i++)
  {
    int aposta = rand() % 15 + 1;
    int soma = somaFibonacci(aposta);
    if (ehPrimo(soma))
    {
      if (soma > maiorSomaPrima)
      {
        maiorSomaPrima = soma;
        indiceVencedor = i;
      }
    }
  }

  if (indiceVencedor == -1)
  {
    printf("Nenhum contemplado neste sorteio\n");
  }
  else
  {
    int indiceReal = indiceParticipantesSorteio[indiceVencedor];
    float saldoSalario = buscaSaldoClientePorTipo(listaClientes[indiceReal], CONTA_SALARIO);
    float premio = 10 * saldoSalario;

    for (int i = 0; i < listaClientes[indiceReal].qtdContas; i++)
    {
      if (listaClientes[indiceReal].contas[i].tipo == CONTA_SALARIO)
      {
        listaClientes[indiceReal].contas[i].saldo += premio;
      }
    }

    listaClientes[indiceReal].premioSorteio = premio;

    printf(
      "Contemplado: %s | %d-0%d | Premio R$ %.2f\n",
      listaClientes[indiceReal].nome,
      listaClientes[indiceReal].numeroConta,
      CONTA_SALARIO,
      premio
    );
  }
  

  free(indiceParticipantesSorteio);
}

void ordernarOrdemDecrescenteSaldoContaSalario(Cliente *listaClientes, int qtdParticipantes, int *listaIndicesParticipantes)
{
  for (int i = 0; i < qtdParticipantes - 1; i++)
  {
    int trocou = 0;
    for(int j = 0; j < qtdParticipantes - 1 - i; j++)
    {
      float saldoClienteAtual = buscaSaldoClientePorTipo(listaClientes[listaIndicesParticipantes[j]], CONTA_SALARIO);
      float saldoProximoCliente = buscaSaldoClientePorTipo(listaClientes[listaIndicesParticipantes[j + 1]], CONTA_SALARIO);

      if (saldoClienteAtual < saldoProximoCliente)
      {
        int aux = listaIndicesParticipantes[j];
        listaIndicesParticipantes[j] = listaIndicesParticipantes[j + 1];
        listaIndicesParticipantes[j + 1] = aux;
        trocou = 1;
      }
    }
    if (!trocou) break;
  }
}

int fibonacci(int posicao)
{
  if (posicao == 1)
  {
    return 0;
  }
  if (posicao == 2)
  {
    return 1;
  }
  return fibonacci(posicao - 1) + fibonacci(posicao - 2);
}

int somaFibonacci(int valor)
{
  int resultado = 0;
  for (int i = 1; i <= valor; i++)
  {
    resultado += fibonacci(i);
  }
  return resultado;
}

int ehPrimo(int numero)
{
  if (numero <= 1)
  {
    return 0;
  }
  for (int i = 2; i < numero; i++)
  {
    if (numero % i == 0)
    {
      return 0;
    }
  }
  return 1;
}

char converterMinuscula(char letra)
{
  if (letra >= 'A' && letra <= 'Z')
  {
    return letra + 32;
  }
  return letra;
}

void limparBuffer()
{
  int c;

  while ((c = getchar()) != '\n' && c != EOF)
    ;
}