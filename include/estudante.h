#ifndef ESTUDANTE_H
#define ESTUDANTE_H

#include <stdbool.h>
#include "fila.h"

bool cadastrarEstudante(const Aluno* a);
bool buscarMatricula(int matricula, Aluno* resultado);
bool duplicidadeMatricula(int matricula);
bool duplicidadeCPF(const char* cpf);
void listarEstudantes();
#endif