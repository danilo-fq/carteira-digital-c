#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "meuBanco.h"

// remover testes manuais no final
Cliente clientesTeste[] =
    {
        // 1) Elegivel - salario 1500 - comprou titulo COM registro 'T' no historico
        {"Ana Souza", 1001, 0, 1, 3, 1, 0.0f, {{CONTA_SALARIO, 1500.00f}}, {{'D', 200.00f}, {'T', 300.00f}, {'S', 50.00f}, {'\0', 0.0f}, {'\0', 0.0f}}},

        // 2) Elegivel - salario 1400 + poupanca - comprou titulo COM registro 'T'
        {"Bruno Lima", 1002, 0, 2, 5, 1, 0.0f, {{CONTA_SALARIO, 1400.00f}, {CONTA_POUPANCA, 800.00f}}, {{'D', 500.00f}, {'S', 700.00f}, {'T', 250.00f}, {'E', 1000.00f}, {'D', 80.00f}}},

        // 3) Elegivel - salario 1300 - comprou titulo COM registro 'T'
        {"Carla Mendes", 1003, 0, 1, 5, 1, 0.0f, {{CONTA_SALARIO, 1300.00f}}, {{'D', 1000.00f}, {'S', 200.00f}, {'T', 200.00f}, {'S', 100.00f}, {'D', 50.00f}}},

        // 4) Elegivel - salario 1200 - comprou titulo mas SEM registro 'T' no historico
        //    (prova que a elegibilidade vem do CAMPO comprouTitulo, nao do historico)
        {"Diego Alves", 1004, 0, 1, 4, 1, 0.0f, {{CONTA_SALARIO, 1200.00f}}, {{'D', 600.00f}, {'S', 150.00f}, {'D', 250.00f}, {'S', 75.00f}, {'\0', 0.0f}}},

        // 5) Elegivel - salario 1100 - comprou titulo COM registro 'T'
        {"Elisa Rocha", 1005, 0, 1, 3, 1, 0.0f, {{CONTA_SALARIO, 1100.00f}}, {{'D', 400.00f}, {'D', 700.00f}, {'T', 150.00f}, {'\0', 0.0f}, {'\0', 0.0f}}},

        // 6) Elegivel - salario FORA DA FAIXA (2000) mas comprouTitulo=1
        //    (prova que "ter adquirido basta, mesmo fora da faixa")
        {"Felipe Nunes", 1006, 0, 1, 3, 1, 0.0f, {{CONTA_SALARIO, 2000.00f}}, {{'D', 2000.00f}, {'T', 300.00f}, {'S', 300.00f}, {'\0', 0.0f}, {'\0', 0.0f}}},

        // 7) NAO elegivel - comprouTitulo = 0 (deve ser filtrado do sorteio)
        //    tem emprestimo 'E' no historico
        {"Gabriela Dias", 1007, 0, 2, 5, 0, 0.0f, {{CONTA_CORRENTE, -200.00f}, {CONTA_POUPANCA, 500.00f}}, {{'D', 500.00f}, {'S', 700.00f}, {'E', 1250.00f}, {'S', 50.00f}, {'D', 80.00f}}}};

int qtdClientesTeste =
    sizeof(clientesTeste) / sizeof(clientesTeste[0]);

int main()
{
  srand(time(NULL));
  Cliente *listaClientes = NULL;
  Cliente *listaClientesAptosEmprestimo = NULL;
  Cliente *listaClientesSaldoNegativo = NULL;
  Cliente *listaClientesAptosCompraTitulos = NULL;
  int qtdClientes = 0;
  int qtdClientesAptosEmprestimo = 0;
  int qtdClientesNegativos = 0;
  int qtdClientesAptosCompraTitulos = 0;
  char opcaoFuncionario = 's';
  char opcaoCliente = 's';
  int realizouSorteio = 0;

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
    printf("1 - Funcionario\n");
    printf("2 - Cliente\n");
    printf("3 - Sair\n");
    printf("\nEscolha uma opcao: ");
    scanf("%d", &opcaoMenuPrincipal);

    if (opcaoMenuPrincipal == 1)
    {
      printf("\nBem-vindo, funcionario!\n");

      do
      {
        printf("\na - Abrir Banco\n");
        printf("r - Emitir Relatorio\n");
        printf("s - Efetuar Sorteio\n");
        printf("x - Sair\n");
        printf("\nEscolha uma opcao: ");
        scanf(" %c", &opcaoFuncionario);
        opcaoFuncionario = converterMinuscula(opcaoFuncionario);
        switch (opcaoFuncionario)
        {
        case 'a':
          printf("\nAbrindo banco...\n");
          abrirBanco(&listaClientes, &qtdClientes);
          break;
        case 'r':
        {
          int fezAtualizacao = atualizarHistoricos(
              listaClientes,
              qtdClientes,
              &listaClientesAptosEmprestimo,
              &qtdClientesAptosEmprestimo,
              &listaClientesSaldoNegativo,
              &qtdClientesNegativos,
              &listaClientesAptosCompraTitulos,
              &qtdClientesAptosCompraTitulos);

          if (fezAtualizacao)
          {
            float saldoBanco = calcularSaldoBanco(listaClientes, qtdClientes);
            emitirHistorico(
                listaClientes,
                qtdClientes,
                listaClientesAptosEmprestimo,
                qtdClientesAptosEmprestimo,
                listaClientesSaldoNegativo,
                qtdClientesNegativos,
                listaClientesAptosCompraTitulos,
                qtdClientesAptosCompraTitulos,
                saldoBanco,
                realizouSorteio);
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
          efetuarSorteio(listaClientes, qtdClientes);
          realizouSorteio = 1;
          printf("\n=== ENCERRANDO SORTEIO ===\n");
          printf("Voltando para o menu inicial...\n");
          break;
        }
        case 'x':
          printf("Fechando o banco\n");
          break;
        default:
          printf("Opcao invalida!\n");
          break;
        }
      } while (opcaoFuncionario != 'x');
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
            &qtdClientesAptosCompraTitulos);
        printf("\nBem-vindo, cliente!\n");
        printf("\na - Acessar Conta\n");
        printf("x - Sair\n");
        printf("\nEscolha uma opcao: ");
        scanf(" %c", &opcaoCliente);
        opcaoCliente = converterMinuscula(opcaoCliente);
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

      } while (opcaoCliente != 'x');
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
