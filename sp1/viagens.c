#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define MAX_FUNCIONARIOS 50
#define MAX_VIAGENS 50

typedef struct
{
    int matricula;
    char funcionario[50];
    char data[12];
    char origem[50];
    char destino[50];
    int distancia;
    float preco;
    float total;
} Viagem;

typedef struct
{
    char nome[50];
    float total_reembolso;
} Funcionario;

Viagem viagens[MAX_VIAGENS];
Funcionario funcionarios[MAX_FUNCIONARIOS];
int totalViagens = 0;
int totalFuncionarios = 0;

void carregaDados()
{
    FILE *arq;
    int i;
    char linha[100];
    arq = fopen("viagens.csv", "r");
    if (arq == NULL)
    {
        printf("Erro ao abrir o arquivo\n");
    }
    else
    {
        fgets(linha, 100, arq); 
        while (fgets(linha, 100, arq) != NULL)
        {
            viagens[totalViagens].matricula = atoi(strtok(linha, ";"));
            strcpy(viagens[totalViagens].funcionario, strtok(NULL, ";"));
            strcpy(viagens[totalViagens].data, strtok(NULL, ";"));
            strcpy(viagens[totalViagens].origem, strtok(NULL, ";"));
            strcpy(viagens[totalViagens].destino, strtok(NULL, ";"));
            viagens[totalViagens].distancia = atoi(strtok(NULL, ";"));
            viagens[totalViagens].preco = atof(strtok(NULL, ";"));

            viagens[totalViagens].total = (viagens[totalViagens].preco * viagens[totalViagens].distancia) / 3;

            int existe = 0;
            for (i = 0; i < totalFuncionarios; i++)
            {
                if (strcmp(funcionarios[i].nome, viagens[totalViagens].funcionario) == 0)
                {
                    funcionarios[i].total_reembolso += viagens[totalViagens].total;
                    existe = 1;
                    break;
                }
            }

            if (!existe)
            {
                strcpy(funcionarios[totalFuncionarios].nome, viagens[totalViagens].funcionario);
                funcionarios[totalFuncionarios].total_reembolso = viagens[totalViagens].total;
                totalFuncionarios++;
            }

            totalViagens++;
        }
    }
    fclose(arq);
}

void gerarRelatorioMarkdown()
{
    FILE *arquivoMarkdown;
    int i;
    arquivoMarkdown = fopen("RelatorioViagens.md", "w");
    if (arquivoMarkdown == NULL)
    {
        printf("Erro ao criar o arquivo RelatorioViagens.md\n");
        return;
    }

    fprintf(arquivoMarkdown, "# Relatório de Viagens\n\n");

    fprintf(arquivoMarkdown, "## Viagens ACME\n");
    fprintf(arquivoMarkdown, "| Matrícula | Funcionário | Data | Origem | Destino | Distância | Preço | Total |\n");
    fprintf(arquivoMarkdown, "|-----------|-------------|------|--------|---------|-----------|-------|-------|\n");
    for (i = 0; i < totalViagens; i++)
    {
        fprintf(arquivoMarkdown, "| %d | %s | %s | %s | %s | %d | %.2f | %.2f |\n", viagens[i].matricula, viagens[i].funcionario, viagens[i].data, viagens[i].origem, viagens[i].destino, viagens[i].distancia, viagens[i].preco, viagens[i].total);
    }
    fprintf(arquivoMarkdown, "\n");

    fprintf(arquivoMarkdown, "## Reembolso por Funcionário\n");
    fprintf(arquivoMarkdown, "| Funcionário | Total de Reembolso |\n");
    fprintf(arquivoMarkdown, "|-------------|---------------------|\n");
    for (i = 0; i < totalFuncionarios; i++)
    {
        fprintf(arquivoMarkdown, "| %s | %.2f |\n", funcionarios[i].nome, funcionarios[i].total_reembolso);
    }
    fprintf(arquivoMarkdown, "\n");

    float totalGeral = 0;
    for (i = 0; i < totalViagens; i++)
    {
        totalGeral += viagens[i].total;
    }
    fprintf(arquivoMarkdown, "### Total Geral para Reembolso: %.2f\n", totalGeral);

    fclose(arquivoMarkdown);

    printf("Arquivo RelatorioViagens.md gerado com sucesso.\n");
}

int main()
{
	setlocale(LC_ALL, "");
    carregaDados();
    gerarRelatorioMarkdown();
    return 0;
}

