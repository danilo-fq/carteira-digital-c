#include <stdio.h>
#include <stdlib.h>
#include "meuBanco.h"

// remover testes manuais no final
Cliente clientesTeste[] =
    {
        {"Ana Souza",
         1001,
         0,
         2,
         3,
         {{CONTA_CORRENTE, 500.00f},
          {CONTA_POUPANCA, 1000.00f}},
         {{'D', 200.00f},
          {'S', 50.00f},
          {'D', 300.00f},
          {'\0', 0.0f},
          {'\0', 0.0f}}},

        {"Bruno Lima",
         1002,
         0,
         1,
         5,
         {{CONTA_CORRENTE, -200.00f}},
         {{'D', 500.00f},
          {'S', 700.00f},
          {'D', 100.00f},
          {'S', 50.00f},
          {'D', 80.00f}}},

        {"Carla Mendes",
         1003,
         0,
         1,
         5,
         {{CONTA_SALARIO, 1000.00f}},
         {{'D', 1000.00f},
          {'S', 200.00f},
          {'D', 300.00f},
          {'S', 100.00f},
          {'D', 50.00f}}}};

int qtdClientesTeste =
    sizeof(clientesTeste) / sizeof(clientesTeste[0]);

int main()
{
  Cliente *listaClientes = NULL;
  Cliente *listaClientesAptosEmprestimo = NULL;
  Cliente *listaClientesSaldoNegativo = NULL;
  Cliente *listaClientesAptosCompraTitulos = NULL;
  int qtdClientes = 0;
  int qtdClientesAptosEmprestimo = 0;
  int qtdClientesNegativos = 0;
  int qtdClientesContaSalarioFaixaSaldo = 0;
  char opcaoFuncionario = 's';
  char opcaoCliente = 's';

  // remover testes manuais no final
  qtdClientes = qtdClientesTeste;
  listaClientes = malloc(sizeof(Cliente) * qtdClientes);

  if (listaClientes == NULL)
  {
    printf("Erro ao alocar memoria para os clientes de teste\n");
    return 1;
  }

  // remover testes manuais no final
  for (int i = 0; i < qtdClientes; i++)
  {
    listaClientes[i] = clientesTeste[i];
  }

  printf("\n===ABRINDO APLICATIVO===\n");

  /*Adicionei um loop while para controle dos dois menus de cliente e funcionario , para que navegue entre os tipos de usuarios,
   *Já fiz a opçao do cliente acessar a conta e verificar se esta negativado, usando o listarClientesSaldoNegativo
   * Coloquei para limpar a memoria so quando sair do programa e nao entre os dois menus*/

  while (1)
  {
    int opcaoMenuPrincipal;
    printf("\n=== MENU PRINCIPAL ===\n");
    printf("1 - Funcionário\n");
    printf("2 - Cliente\n");
    printf("3 - Sair\n");
    printf("Escolha uma opcao: ");
    scanf("%d", &opcaoMenuPrincipal);

    if (opcaoMenuPrincipal == 1)
    {
      printf("\nBem-vindo, funcionario!\n");

      do
      {
        printf("\na - Abrir Banco\n");
        printf("c - Emitir Relatorio\n");
        printf("s - Efetuar Sorteio\n");
        printf("x - Sair\n");
        printf("\nEscolha uma opcao: ");
        scanf(" %c", &opcaoFuncionario);
        switch (opcaoFuncionario)
        {
        case 'a':
          printf("\nAbrindo banco...\n");
          abrirBanco(&listaClientes, &qtdClientes);
          break;
        case 'c':
        {
          int fezAtualizacao = atualizarHistoricos(
              listaClientes,
              qtdClientes,
              &listaClientesAptosEmprestimo,
              &qtdClientesAptosEmprestimo,
              &listaClientesSaldoNegativo,
              &qtdClientesNegativos,
              &listaClientesAptosCompraTitulos,
              &qtdClientesContaSalarioFaixaSaldo);

          if (fezAtualizacao)
          {
            float saldoBanco = calcularSaldoBanco(listaClientes, qtdClientes);
            emitirHistorico(
                qtdClientes,
                listaClientesAptosEmprestimo,
                qtdClientesAptosEmprestimo,
                listaClientesSaldoNegativo,
                qtdClientesNegativos,
                listaClientesAptosCompraTitulos,
                qtdClientesContaSalarioFaixaSaldo,
                saldoBanco);
          }
          else
          {
            printf("\n[ERRO]: Nao ha clientes cadastrados!\n");
          }
          printf("\n=== ENCERRANDO CONSULTA ===\n");
          printf("Voltando para o menu inicial...\n");
          break;
        }
        case 's':
        {
          printf("\n== REALIZANDO SORTEIO ==\n");
          break;
        }
        case 'x':
          printf("Fechando o banco\n");
          break;
        default:
          printf("Opcao invalida!\n");
          break;
        }
      } while (opcaoFuncionario != 's');
    }
    else if (opcaoMenuPrincipal == 2)
    {

      do
      {
        atualizarHistoricos(
            listaClientes,
            qtdClientes,
            &listaClientesAptosEmprestimo,
            &qtdClientesAptosEmprestimo,
            &listaClientesSaldoNegativo,
            &qtdClientesNegativos,
            &listaClientesAptosCompraTitulos,
            &qtdClientesContaSalarioFaixaSaldo);
        printf("\nBem-vindo, cliente!\n");
        printf("a - Acessar Conta\n");
        printf("x - Sair\n");
        printf("\nEscolha uma opcao: ");
        scanf(" %c", &opcaoCliente);
        switch (opcaoCliente)
        {
        case 'a':
        {
          printf("\nAcessando conta...\n");
          printf("Digite o numero da sua conta: ");
          int numeroConta;
          scanf("%d", &numeroConta);

          int achouNegativado = 0;

          for (int i = 0; i < qtdClientesNegativos; i++)
          {
            if (listaClientesSaldoNegativo[i].numeroConta == numeroConta)
            {
              achouNegativado = 1;

              for (int j = 0; j < listaClientesSaldoNegativo[i].qtdContas; j++)
              {
                if (
                    listaClientesSaldoNegativo[i].contas[j].tipo == CONTA_CORRENTE &&
                    listaClientesSaldoNegativo[i].contas[j].saldo < 0.0f)
                {
                  printf(
                      "\n[AVISO]: %s (Conta %d), sua Conta Corrente esta negativa em R$ %.2f!\n",
                      listaClientesSaldoNegativo[i].nome,
                      listaClientesSaldoNegativo[i].numeroConta,
                      listaClientesSaldoNegativo[i].contas[j].saldo);
                  break;
                }
              }
              break;
            }
          }

          if (!achouNegativado)
          {
            printf("\nConta %d nao encontrada na lista de negativados.\n", numeroConta);
          }

          break;
        }

        case 'x':
          printf("Saindo...\n");
          break;

        default:
          printf("Opcao invalida!\n");
          break;
        }

      } while (opcaoCliente != 's');
    }
    else if (opcaoMenuPrincipal == 3)
    {
      printf("Saindo...\n");
      free(listaClientesAptosEmprestimo);
      free(listaClientesSaldoNegativo);
      free(listaClientesAptosCompraTitulos);
      free(listaClientes);
      break;
    }
    else
    {
      printf("Opcao invalida! Voltando para o menu principal...\n");
      continue;
    }
  }
  return 0;
}
