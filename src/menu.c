#include "menu.h"
#include "estudante.h"
#include "fila.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static int atendidos = 0;
static const char* ARQ_ESTUDANTES = "estudantes.txt";
static const char* ARQ_FILA = "fila.txt";

static bool lerInt(const char* msg, int* destino)
{
    char buf[64];
    if (msg) printf("%s", msg);
    if (!fgets(buf, sizeof(buf), stdin)) return false;
    return sscanf(buf, "%d", destino) == 1;
}

static void salvarEstudantesArquivo(void)
{
    FILE* fp = fopen(ARQ_ESTUDANTES, "w");
    if (!fp) { perror("salvarEstudantes"); return; }

    for (int i = 0; i < banco.total; i++)
    {
        const Aluno* a = &banco.dados[i];
        fprintf(fp, "%d|%s|%d|%s\n", a->matricula, a->cpf, a->atendido ? 1 : 0, a->nome);
    }

    fclose(fp);
}

static void carregarEstudantesArquivo(void)
{
    FILE* fp = fopen(ARQ_ESTUDANTES, "r");
    if (!fp) return;

    banco.total = 0;
    char linha[512];
    while (fgets(linha, sizeof(linha), fp))
    {
        linha[strcspn(linha, "\n")] = 0;
        char* p = linha;
        char* tok;

        tok = strtok(p, "|");
        if (!tok) continue;
        int matricula = atoi(tok);

        tok = strtok(NULL, "|");
        if (!tok) continue;
        char cpf[MAX_CPF];
        strncpy(cpf, tok, MAX_CPF-1); cpf[MAX_CPF-1] = '\0';

        tok = strtok(NULL, "|");
        if (!tok) continue;
        int atend = atoi(tok);

        tok = strtok(NULL, "|");
        char nome[MAX_NOME] = "";
        if (tok) { strncpy(nome, tok, MAX_NOME-1); nome[MAX_NOME-1] = '\0'; }

        Aluno a;
        a.matricula = matricula;
        strncpy(a.cpf, cpf, MAX_CPF-1); a.cpf[MAX_CPF-1] = '\0';
        strncpy(a.nome, nome, MAX_NOME-1); a.nome[MAX_NOME-1] = '\0';
        a.atendido = (atend != 0);

        if (banco.total < MAX_CADASTRO)
            banco.dados[banco.total++] = a;
    }
    fclose(fp);
}

static void salvarFilaArquivo(const Fila* F)
{
    FILE* fp = fopen(ARQ_FILA, "w");
    if (!fp) { perror("salvarFila"); return; }

    fprintf(fp, "%d %d\n", F->total, atendidos);
    int idx = F->inicio;
    for (int i = 0; i < F->total; i++)
    {
        const Aluno* a = &F->dados[idx];
        fprintf(fp, "%d\n", a->matricula);
        idx = (idx + 1) % F->capacidade;
    }
    fclose(fp);
}


static void carregarFilaArquivo(Fila* F)
{
    FILE* fp = fopen(ARQ_FILA, "r");
    if (!fp) return;

    int total = 0;
    if (fscanf(fp, "%d %d\n", &total, &atendidos) != 2) { fclose(fp); return; }

    for (int i = 0; i < total; i++)
    {
        int mat;
        if (fscanf(fp, "%d\n", &mat) != 1) break;
        Aluno a;
        if (buscarMatricula(mat, &a))
        {
            if (!filaCheia(F))
                enfileirar(F, a);
        }
    }
    fclose(fp);
}

static void salvarDados(const Fila* F)
{
    salvarEstudantesArquivo();
    salvarFilaArquivo(F);
}

static void carregarDados(Fila* F)
{
    carregarEstudantesArquivo();
    carregarFilaArquivo(F);
}

static void mostrarMenu(void)
{
    printf("\n==== DISTRIBUIÇÃO DE KITS DE LABORATÓRIO ====\n");
    printf("1 - Cadastrar estudante\n");
    printf("2 - Enfileirar solicitação\n");
    printf("3 - Atender solicitação\n");
    printf("4 - Exibir fila\n");
    printf("5 - Exibir relatório\n");
    printf("6 - Salvar dados\n");
    printf("7 - Sair\n");
}

static void lerString(const char* prompt, char* destino, size_t maxlen)
{
    char buf[256];
    if (prompt) printf("%s", prompt);
    if (!fgets(buf, sizeof(buf), stdin)) { destino[0] = '\0'; return; }
    buf[strcspn(buf, "\n")] = 0;
    strncpy(destino, buf, maxlen - 1);
    destino[maxlen - 1] = '\0';
}

static void processarOpcao(int opcao, Fila* F)
{
    switch (opcao)
    {
        case 1:
        {
            Aluno novo;
            if (!lerInt("Matrícula: ", &novo.matricula))
            {
                printf("Matrícula inválida\n");
                break;
            }

            if (duplicidadeMatricula(novo.matricula))
            {
                printf("Matrícula já cadastrada\n");
                break;
            }

            lerString("CPF (11 dígitos): ", novo.cpf, MAX_CPF);
            if (strlen(novo.cpf) != 11 || duplicidadeCPF(novo.cpf))
            {
                printf("CPF inválido ou já cadastrado\n");
                break;
            }

            lerString("Nome completo: ", novo.nome, MAX_NOME);
            novo.atendido = false;

            if (cadastrarEstudante(&novo)) printf("Estudante cadastrado\n");
            else printf("Limite de cadastro atingido\n");
            break;
        }

        case 2:
        {
            listarEstudantes();
            int matricula;
            if (!lerInt("Digite a matrícula para enfileirar: ", &matricula))
            {
                printf("Entrada inválida\n");
                break;
            }

            Aluno a;
            if (!buscarMatricula(matricula, &a)) { printf("Estudante não encontrado\n"); break; }
            if (a.atendido) { printf("Estudante já foi atendido\n"); break; }
            if (alunoNaFila(F, matricula)) { printf("Estudante já está na fila\n"); break; }
            if (filaCheia(F)) { printf("Fila cheia\n"); break; }

            enfileirar(F, a);
            printf("Solicitação enfileirada\n");
            break;
        }

        case 3:
        {
            if (filaVazia(F)) { printf("Fila vazia. Nenhuma solicitação\n"); break; }
            Aluno removido;
            if (desenfileirar(F, &removido))
            {
                printf("Atendendo: %s (Matrícula: %d)\n", removido.nome, removido.matricula);
                atendidos++;
                for (int i = 0; i < banco.total; i++)
                {
                    if (banco.dados[i].matricula == removido.matricula)
                    {
                        banco.dados[i].atendido = true;
                        break;
                    }
                }
            }
            else printf("Erro ao atender\n");
            break;
        }

        case 4:
            exibirFila(F);
            break;

        case 5:
            printf("\n==== RELATÓRIO ====\n");
            printf("Estudantes aguardando: %d\n", F->total);
            printf("Estudantes atendidos: %d\n", atendidos);
            break;

        case 6:
            salvarDados(F);
            printf("Dados salvos com sucesso\n");
            break;

        case 7:
            printf("Saindo...\n");
            break;

        default:
            printf("Opção inválida\n");
            break;
    }
}

void menuPrincipal(void)
{
    int capacidade;
    if (!lerInt("Capacidade máxima da fila: ", &capacidade) || capacidade <= 0)
    {
        printf("Capacidade inválida\n");
        return;
    }

    Fila* F = criarFila(capacidade);
    if (!F) { printf("Erro ao criar fila\n"); return; }

    carregarDados(F);

    int opcao;
    do
    {
        mostrarMenu();
        if (!lerInt("Escolha: ", &opcao))
        {
            printf("Entrada inválida\n");
            continue;
        }
        processarOpcao(opcao, F);
    } while (opcao != 7);

    salvarDados(F);
    destruirFila(&F);
}