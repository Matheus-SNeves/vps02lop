#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_MAQUINAS 4
#define MAX_DIAS 100

typedef struct {
    char data[11];
    int producao[NUM_MAQUINAS];
} ProducaoDiaria;

ProducaoDiaria producaoDiaria[MAX_DIAS];
int totalDias = 0, i;

int encontrarDia(char *data) {
    for ( i = 0; i < totalDias; i++) {
        if (strcmp(producaoDiaria[i].data, data) == 0) {
            return i;
        }
    }
    return -1;
}

int main() {
    FILE *arquivo, *relatorio;
    char linha[100];

    arquivo = fopen("producao.csv", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    fgets(linha, sizeof(linha), arquivo);

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        char data[11];
        char hora[6];
        int numeroMaquina;

        sscanf(linha, "%10[^;];%5[^;];%d", data, hora, &numeroMaquina);

        int indiceDia = encontrarDia(data);
        if (indiceDia == -1) {
            strcpy(producaoDiaria[totalDias].data, data);
            for ( i = 0; i < NUM_MAQUINAS; i++) {
                producaoDiaria[totalDias].producao[i] = 0;
            }
            indiceDia = totalDias;
            totalDias++;
        }

        if (numeroMaquina >= 1 && numeroMaquina <= NUM_MAQUINAS) {
            producaoDiaria[indiceDia].producao[numeroMaquina - 1]++;
        }
    }

    fclose(arquivo);

    relatorio = fopen("explosivos.md", "w");
    if (relatorio == NULL) {
        printf("Erro ao criar o relatório.\n");
        return 1;
    }

    fprintf(relatorio, "# Relatório de Produção de Explosivos\n\n");
    fprintf(relatorio, "| Data       | Máquina 1 | Máquina 2 | Máquina 3 | Máquina 4 |\n");
    fprintf(relatorio, "|------------|-----------|-----------|-----------|-----------|\n");

    for ( i = 0; i < totalDias; i++) {
        fprintf(relatorio, "| %s | %9d | %9d | %9d | %9d |\n",
                producaoDiaria[i].data,
                producaoDiaria[i].producao[0],
                producaoDiaria[i].producao[1],
                producaoDiaria[i].producao[2],
                producaoDiaria[i].producao[3]);
    }

    fclose(relatorio);

    printf("Relatório salvo em 'explosivos.md'.\n");

    return 0;
}

