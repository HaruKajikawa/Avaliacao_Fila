#include "fila.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Fila* criarFila(int capacidade)
{
    if (capacidade <= 0) return NULL;

    Fila* F = malloc(sizeof(Fila));
    if (!F) return NULL;

    F->capacidade = capacidade;
    F->total = 0;
    F->inicio = 0;
    F->final = 0;

    F->dados = calloc((size_t)capacidade, sizeof(Aluno));
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

bool enfileirar(Fila* F,const Aluno* x)
{
    if (!F || !x) return false;
    if (filaCheia(F)) return false;

    F->dados[F->final].matricula = x->matricula;
    strncpy(F->dados[F->final].cpf, x->cpf, MAX_CPF - 1);
    F->dados[F->final].cpf[MAX_CPF - 1] = '\0';
    strncpy(F->dados[F->final].nome, x->nome,  MAX_NOME - 1);
    F->dados[F->final].nome[MAX_NOME - 1] = '\0';

    avanca_idx(&F->final, F->capacidade);
    F->total++;

    return true;
}

bool desenfileirar(Fila* F, Aluno* removido)
{
    if (!F || !removido) return false;
    if (filaVazia(F)) return false;

    removido->matricula = F->dados[F->inicio].matricula;
    strncpy(removido->cpf, F->dados[F->inicio].cpf, MAX_CPF);
    removido->cpf[MAX_CPF - 1] = '\0';
    strncpy(removido->nome, F->dados[F->inicio].nome, MAX_NOME);
    removido->nome[MAX_NOME - 1] = '\0';

    F->dados[F->inicio].matricula = 0;
    F->dados[F->inicio].cpf[0] = '\0';
    F->dados[F->inicio].nome[0] = '\0';

    avanca_idx(&F->inicio, F->capacidade);
    F->total--;

    return true;
}

void destruirFila(Fila** F)
{
    if (!F || !*F) return;

    free((*F)->dados);
    (*F)->dados = NULL;
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

        printf("Posição %d (índice %d): \n", i + 1, idx);
        printf(" Matrícula: %d\n", A->matricula);
        printf(" CPF: %s\n", A->cpf);
        printf(" Nome: %s\n\n", A->nome);
        printf(" Atendido: %s\n\n", A->atendido ? "Sim" : "Não");

        avanca(idx, F->capacidade);
    }
}