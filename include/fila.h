#ifndef FILA_H
#define FILA_H

#include <stdbool.h>

#define avanca(idx, capacidade) (idx = ((idx + 1) % (capacidade)))
#define MAX_NOME 61
#define MAX_CPF 12

typedef struct Aluno
{
    int matricula;
    char cpf[MAX_CPF];
    char nome[MAX_NOME];
    bool atendido;
} Aluno;

typedef struct Fila
{
    int capacidade;
    int total;
    int inicio;
    int final;
    Aluno* dados;
} Fila;

Fila* criarFila(int capacidade);
bool filaVazia(const Fila* F);
bool filaCheia(const Fila* F);
bool enfileirar(Fila* F, Aluno x);
bool desenfileirar(Fila* F, Aluno* removido);
void destruirFila(Fila** F);
void exibirFila(const Fila* F);
#endif