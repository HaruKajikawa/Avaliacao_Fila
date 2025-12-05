#include "estudante.h"
#include "fila.h"

#include <string.h>
#include <stdio.h>

BancoEstudantes banco = {.total = 0};

bool duplicidadeMatricula(int matricula)
{
    for (int i = 0; i < banco.total; i++)
    {
        if (banco.dados[i].matricula == matricula) return true;
    }
    return false;
}

bool duplicidadeCPF(const char* cpf)
{
    for (int i = 0; i < banco.total; i++)
    {
        if (strcmp(banco.dados[i].cpf, cpf) == 0) return true;
    }
    return false;
}

bool cadastrarEstudante(const Aluno* a)
{
    if (banco.total >= MAX_CADASTRO) return false;
    if (duplicidadeMatricula(a->matricula) || duplicidadeCPF(a->cpf)) return false;

    banco.dados[banco.total] = *a;
    banco.dados[banco.total].atendido = false;
    banco.total++;
    return true;
}

bool buscarMatricula(int matricula, Aluno* resultado)
{
    for (int i = 0; i < banco.total; i++)
    {
        if (banco.dados[i].matricula == matricula)
        {
            *resultado = banco.dados[i];
            return true;
        }
    }
    return false;
}

void listarEstudantes()
{
    if (banco.total == 0)
    {
        printf("\nNenhum estudante cadastrado\n");
        return;
    }

    printf("\n==== ESTUDANTES CADASTRADOS ====\n");

    for (int i = 0; i < banco.total; i++)
    {
        printf("------------------------------\n");
        printf("Matrícula: %d\n", banco.dados[i].matricula);
        printf("Nome: %s\n", banco.dados[i].nome);
        printf("CPF: %s\n", banco.dados[i].cpf);
        printf("Atendido: %s\n", banco.dados[i].atendido ? "Sim" : "Não");
        printf("------------------------------\n");
    }
}

void limparBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

bool alunoNaFila(const Fila* F, int matricula)
{
    int idx = F->inicio;
    for (int i = 0; i < F->total; i++)
    {
        if (F->dados[idx].matricula == matricula) return true;
        idx = (idx + 1) % F->capacidade;
    }
    return false;
}