#include "fila.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define avanca(idx, capacidade) (idx = ((idx + 1) % capacidade))

Fila* criarFila(int capacidade)
{
    if (capacidade <= 0) return NULL;

    Fila* F = (Fila*) malloc(sizeof(Fila));
    if (!F) return NULL;

    F->capacidade = capacidade;
    F->total = 0;
    F->inicio = 0;
    F->final = 0;

    F->dados = calloc(capacidade, sizeof(Aluno));
    if(!F->dados)
    {
        free(F);
        return NULL;
    }

    return F;
}

bool filaVazia(const Fila* F)
{
    return F->total == 0;
}

bool filaCheia(const Fila* F)
{
    return F->total == F->capacidade;
}

bool enfileirar(Fila* F, Aluno x)
{
    if (filaCheia(F)) return false;

    F->dados[F->final] = x;
    avanca(F->final, F->capacidade);
    F->total++;

    return true;
}

bool desinfileirar(Fila* F, Aluno* removido)
{
    if (filaVazia(F)) return false;

    *removido = F->dados[F->inicio];
    avanca(F->inicio, F->capacidade);
    F->total--;

    return true;
}

void destruirFila(Fila** F)
{
    if (!F || !*F) return;

    free((*F)->dados);
    free(*F);
    *F = NULL;
}

void exibirFila(const Fila* F)
{
    if (filaVazia(F))
    {
        printf("\nFila vazia!\n");
        return;
    }

    printf("\n==== FILA DE SOLICITAÇÕES (%d estudante(s)) ====\n", F->total);

    int idx = F->inicio;

    for (int i = 0 ; i < F->total ; i ++)
    {
        const Aluno* A = &F->dados[idx];

        printf("Posição %d: \n", i + 1);
        printf(" Matrícula: %d\n", A->matricula);
        printf(" CPF: %s\n", A->cpf);
        printf(" Nome: %s\n\n", A->nome);

        avanca(idx, F->capacidade);
    }
}