#ifndef ESTUDANTE_H
#define ESTUDANTE_H

#include <stdbool.h>
#include "fila.h"

#define MAX_CADASTRO 100

typedef struct BancoEstudantes
{
    Aluno dados[MAX_CADASTRO];
    int total;
} BancoEstudantes;

bool cadastrarEstudante(const Aluno* a);
bool buscarMatricula(int matricula, Aluno* resultado);
bool duplicidadeMatricula(int matricula);
bool duplicidadeCPF(const char* cpf);
void listarEstudantes();
void limparBuffer();
bool alunoNaFila(const Fila* F, int matricula);

extern BancoEstudantes banco;
#endif