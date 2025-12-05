#include "estudante.h"

#include <stdio.h>
#include <string.h>

#define MAX_CADASTRO 100

static Aluno banco[MAX_CADASTRO];
static int totalAlunos = 0;

bool duplicidadeMatricula(int matricula)
{
    for (int i = 0; i < totalAlunos; i++)
    {
        if (banco[i].matricula == matricula) return true;
    }
    return false;
}

bool duplicidadeCPF(const char* cpf)
{
    for (int i = 0; i < totalAlunos; i++)
    {
        if (strcmp(banco[i].cpf, cpf) == 0) return true;
    }
    return false;
}

bool cadastrarEstudante(const Aluno* a)
{
    if (totalAlunos >= MAX_CADASTRO) return false;
    if (duplicidadeMatricula(a->matricula) || duplicidadeCPF(a->cpf)) return false;

    banco[totalAlunos++] = *a;
    return true;
}

bool buscarMatricula(int matricula, Aluno* resultado)
{
    for (int i = 0; i < totalAlunos; i++)
    {
        if (banco[i].matricula == matricula)
        {
            *resultado = banco[i];
            return true;
        }
    }
    return false;
}

void listarEstudantes()
{
    if (totalAlunos == 0)
    {
        printf("\nNenhum estudante cadastrado\n");
        return;
    }

    printf("\n==== ESTUDANTES CADASTRADOS ====\n");

    for (int i = 0; i < totalAlunos; i++)
    {
        printf("Matrícula: %d\n", banco[i].matricula);
        printf("Nome: %s\n", banco[i].nome);
        printf("CPF: %s\n\n", banco[i].cpf);
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
        avanca(idx, F->capacidade);
    }
    return false;
}