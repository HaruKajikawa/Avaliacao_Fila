#include "menu.h"
#include "estudante.h"
#include "fila.h"

#include <stdio.h>
#include <string.h>

static int atendidos = 0;

static void mostrarMenu()
{
    printf("\n==== DISTRIBUIÇÃO DE KITS DE LABORATÓRIO ====\n");
    printf("1 - Cadastrar estudante\n");
    printf("2 - Enfileirar solicitação\n");
    printf("3 - Atender solicitação\n");
    printf("4 - Exibir fila\n");
    printf("5 - Exibir relatório\n");
    printf("6 - Sair\n");
    printf("Escolha: ");
}

static void processarOpcao(int opcao, Fila* F)
{
    switch (opcao)
    {
        case 1:
        {
            Aluno novo;

            printf("\nMatrícula: ");
            scanf("%d", &novo.matricula);

            if (duplicidadeMatricula(novo.matricula))
            {
                printf("Matrícula já cadastrada\n");
                break;
            }

            printf("CPF (11 dígitos): ");
            scanf("%s", novo.cpf);

            if (strlen(novo.cpf) != 11 || duplicidadeCPF(novo.cpf))
            {
                printf("CPF inválido ou já cadastrado\n");
                break;
            }

            printf("Nome completo: ");
            scanf(" %60[^\n]", novo.nome);

            if (cadastrarEstudante(&novo)) printf("Estudante cadastrado com sucesso\n");
            else printf("Limite de cadastro atingido\n");

            break;
        }

        case 2:
        {
            listarEstudantes();

            int matricula;
            printf("Digite a matrícula para enfileirar: ");
            scanf("%d", &matricula);

            Aluno a;

            if(!buscarMatricula(matricula, &a)) printf("Estudante não encontrado\n");
            else if (filaCheia(F)) printf("Fila cheia. Não é possível enfileirar\n");
            else
            {
                enfileirar(F, a);
                printf("Solicitação enfileirada\n");
            }
            break;
        }

        case 3:
        {
            if (filaVazia(F)) printf("Nenhuma solicitação para atender\n");
            else
            {
                Aluno removido;
                desinfileirar(F, &removido);

                printf("Atendendo: %s (Matrícula: %d)\n", removido.nome, removido.matricula);

                atendidos++;
            }
            break;    
        }

        case 4:
        {
            exibirFila(F);
            break;
        }

        case 5:
        {
            printf("\n==== RELATÓRIO ====\n");
            printf("Estudantes aguardando: %d\n", F->total);
            printf("Estudantes atendidos: %d\n", atendidos);
            break;
        }

        case 6:
        {
            printf("Saindo...\n");
            break;
        }

        default: printf("Opção inválida\n"); break;
    }
}

void menuPrincipal()
{
    int capacidade;
    printf("Digite a capacidade máxima da fila: ");
    scanf("%d", &capacidade);

    Fila* F = criarFila(capacidade);

    if (!F)
    {
        printf("Erro ao criar fila\n");
        return;
    }

    int opcao;
    do
    {
        mostrarMenu();
        scanf("%d", &opcao);
        processarOpcao(opcao, F);
    } while (opcao != 6);

    destruirFila(&F);
}