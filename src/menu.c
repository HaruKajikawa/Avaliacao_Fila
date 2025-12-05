#include "menu.h"
#include "estudante.h"
#include "fila.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

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

static void lerString(const char* prompt, char* destino, size_t maxlen)
{
    char buf[512];
    if (prompt) printf("%s", prompt);
    if (!fgets(buf, sizeof(buf), stdin)) { destino[0] = '\0'; return; }
    buf[strcspn(buf, "\n")] = 0;
    /* copia com segurança */
    strncpy(destino, buf, maxlen - 1);
    destino[maxlen - 1] = '\0';
}

static void trim(char *s) 
{
    size_t ln = strlen(s);
    while (ln > 0 && (s[ln-1] == '\n' || s[ln-1] == '\r' || s[ln-1] == ' ' || s[ln-1] == '\t')) {
        s[--ln] = '\0';
    }

    char *p = s;
    while (*p && isspace((unsigned char)*p)) p++;
    if (p != s) memmove(s, p, strlen(p) + 1);
}

static void salvarEstudantesFormatado()
{
    FILE* fp = fopen(ARQ_ESTUDANTES, "w");
    if (!fp) { perror("salvarEstudantes"); return; }

    for (int i = 0; i < banco.total; i++)
    {
        const Aluno* a = &banco.dados[i];
        fprintf(fp, "=== ESTUDANTE ===\n");
        fprintf(fp, "Matricula: %d\n", a->matricula);
        fprintf(fp, "CPF: %s\n", a->cpf);
        fprintf(fp, "Nome: %s\n", a->nome);
        fprintf(fp, "Atendido: %s\n", a->atendido ? "SIM" : "NAO");
        fprintf(fp, "\n");
    }

    fclose(fp);
}

static void salvarFilaFormatado(const Fila* F)
{
    FILE* fp = fopen(ARQ_FILA, "w");
    if (!fp) { perror("salvarFila"); return; }

    fprintf(fp, "=== FILA ===\n");
    fprintf(fp, "Total: %d\n", F->total);
    fprintf(fp, "Atendidos: %d\n\n", atendidos);

    int idx = F->inicio;
    for (int i = 0; i < F->total; i++)
    {
        fprintf(fp, "Matricula: %d\n", F->dados[idx].matricula);
        idx = (idx + 1) % F->capacidade;
    }

    fclose(fp);
}

static void carregarEstudantesFormatado()
{
    FILE* fp = fopen(ARQ_ESTUDANTES, "r");
    if (!fp) return;

    banco.total = 0;
    char linha[512];

    while (fgets(linha, sizeof(linha), fp))
    {
        trim(linha);
        if (strlen(linha) == 0) continue;

        if (strcmp(linha, "=== ESTUDANTE ===") == 0)
        {
            Aluno a;
            a.nome[0] = a.cpf[0] = '\0';
            a.matricula = 0;
            a.atendido = false;

            if (!fgets(linha, sizeof(linha), fp)) break;
            trim(linha);
            if (sscanf(linha, "Matricula: %d", &a.matricula) != 1) continue;

            if (!fgets(linha, sizeof(linha), fp)) break;
            trim(linha);
            if (sscanf(linha, "CPF: %15s", a.cpf) != 1) a.cpf[0] = '\0';

            if (!fgets(linha, sizeof(linha), fp)) break;
            trim(linha);

            if (strncmp(linha, "Nome:", 5) == 0) 
            {
                const char* p = linha + 5;
                while (*p == ' ') p++;
                strncpy(a.nome, p, MAX_NOME-1);
                a.nome[MAX_NOME-1] = '\0';
            }

            if (!fgets(linha, sizeof(linha), fp)) break;
            trim(linha);
            if (strncmp(linha, "Atendido:", 8) == 0) 
            {
                const char* p = linha + 8;
                while (*p == ' ') p++;
                if (strcasecmp(p, "SIM") == 0 || strcasecmp(p, "S") == 0) a.atendido = true;
                else a.atendido = false;
            }

            if (a.matricula != 0 && !duplicidadeMatricula(a.matricula) && !duplicidadeCPF(a.cpf))
            {
                if (banco.total < MAX_CADASTRO)
                {
                    banco.dados[banco.total++] = a;
                }
            }
        }
    }

    fclose(fp);
}

static void carregarFilaFormatado(Fila* F)
{
    FILE* fp = fopen(ARQ_FILA, "r");
    if (!fp) return;

    char linha[256];
    int total = 0;
    int l_atendidos = 0;

    while (fgets(linha, sizeof(linha), fp))
    {
        trim(linha);
        if (strcmp(linha, "=== FILA ===") == 0) break;
    }

    if (!fgets(linha, sizeof(linha), fp)) { fclose(fp); return; }
    trim(linha);
    sscanf(linha, "Total: %d", &total);

    if (!fgets(linha, sizeof(linha), fp)) { fclose(fp); return; }
    trim(linha);
    sscanf(linha, "Atendidos: %d", &l_atendidos);
    atendidos = l_atendidos;

    fgets(linha, sizeof(linha), fp);

    while (fgets(linha, sizeof(linha), fp))
    {
        trim(linha);
        if (strlen(linha) == 0) continue;

        int mat = 0;
        if (sscanf(linha, "Matricula: %d", &mat) == 1)
        {
            Aluno a;
            if (buscarMatricula(mat, &a))
            {
                if (!filaCheia(F))
                    enfileirar(F, a);
            }
        }
    }

    fclose(fp);
}

static void salvarEstudantesArquivo() { salvarEstudantesFormatado(); }
static void carregarEstudantesArquivo() { carregarEstudantesFormatado(); }
static void salvarFilaArquivo(const Fila* F) { salvarFilaFormatado(F); }
static void carregarFilaArquivo(Fila* F) { carregarFilaFormatado(F); }

static void mostrarMenu()
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
            salvarEstudantesArquivo();
            salvarFilaArquivo(F);
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

void menuPrincipal()
{
    int capacidade;
    if (!lerInt("Capacidade máxima da fila: ", &capacidade) || capacidade <= 0)
    {
        printf("Capacidade inválida\n");
        return;
    }

    Fila* F = criarFila(capacidade);
    if (!F) { printf("Erro ao criar fila\n"); return; }

    carregarEstudantesArquivo();
    carregarFilaArquivo(F);

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

    salvarEstudantesArquivo();
    salvarFilaArquivo(F);
    destruirFila(&F);
}