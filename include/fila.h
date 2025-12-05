#ifndef FILA_H
#define FILA_H

#include <stdbool.h>
#define MAX_ESTUDANTE 100

typedef struct Aluno
{
    int matricula;
    char cpf[12];
    char nome[61];
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
bool desinfileirar(Fila* F, Aluno* removido);
void destruirFila(Fila** F);
void exibirFila(const Fila* F);
#endif