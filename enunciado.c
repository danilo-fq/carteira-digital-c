/*
  EXERCICIO EM SALA 11/11/2026
  Construa um algoritmo que simula uma carteira digital. 
  A carteira contém os seguintes campos obrigatórios:
    - cliente; (array de char)
    - saldo; (float)
    - tipo { c - corrente, s - salario, p - poupança }; array de inteiros (incompatibilidade: corrente e salario) pode ter até 2 contas.
    - *historico; (as ultimas 5 transações) [valor, tipo] (deposito, saque, emprestimo, titulo de Capitalização)
    - numero começando em 1000;
  O banco pode ter 'n' clientes. O cliente pode solicitar emprestimo caso tenha saldo na poupança e sua conta seja corrente, limitando-se a apenas um empréstimo. O limite de emprestimo equivale a 25% de 10 vezes o saldo da poupança + conta.

  Saída:
    - saldo total do banco;
    - clientes que podem fazer emprestimo:
    - Clientes com saldo negativo na conta corrente;
    - Clientes com saldo entre 1000 e 1500 na conta salário
  
  OBS: o algoritmo só deve finalizar quando o usuário digitar 's'

  VER SOBRE INT* TIME()
  SYTEM("CLS")
*/