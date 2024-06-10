#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <locale.h>

typedef struct {
    int numero_de_serie;
    int resistencia;
    int durabilidade;
} Produto;

int main() {
    FILE *arquivo_entrada, *arquivo_saida;
    char linha[100];
    int resistencia_total = 0, durabilidade_total = 0;
    int maior_resistencia = INT_MIN, menor_resistencia = INT_MAX;
    int maior_durabilidade = INT_MIN, menor_durabilidade = INT_MAX;
    int num_produtos = 0;
    int num_produto_maior_resistencia = -1, num_produto_menor_resistencia = -1;
    int num_produto_maior_durabilidade = -1, num_produto_menor_durabilidade = -1;

    setlocale(LC_ALL, "Portuguese");

    arquivo_entrada = fopen("parametros.csv", "r");
    if (arquivo_entrada == NULL) {
        printf("Erro ao abrir o arquivo de entrada.\n");
        return 1;
    }

    arquivo_saida = fopen("qualidade.md", "w");
    if (arquivo_saida == NULL) {
        printf("Erro ao criar o arquivo de sa�da.\n");
        fclose(arquivo_entrada);
        return 1;
    }

    fgets(linha, sizeof(linha), arquivo_entrada);

    while (fgets(linha, sizeof(linha), arquivo_entrada) != NULL) {
        Produto produto;
        sscanf(linha, "%d;%d;%d", &produto.numero_de_serie, &produto.resistencia, &produto.durabilidade);

        resistencia_total += produto.resistencia;
        durabilidade_total += produto.durabilidade;

        if (produto.resistencia > maior_resistencia) {
            maior_resistencia = produto.resistencia;
            num_produto_maior_resistencia = produto.numero_de_serie;
        }
        if (produto.resistencia < menor_resistencia) {
            menor_resistencia = produto.resistencia;
            num_produto_menor_resistencia = produto.numero_de_serie;
        }
        if (produto.durabilidade > maior_durabilidade) {
            maior_durabilidade = produto.durabilidade;
            num_produto_maior_durabilidade = produto.numero_de_serie;
        }
        if (produto.durabilidade < menor_durabilidade) {
            menor_durabilidade = produto.durabilidade;
            num_produto_menor_durabilidade = produto.numero_de_serie;
        }

        num_produtos++;
    }

    fclose(arquivo_entrada);

    float media_resistencia = (float)resistencia_total / num_produtos;
    float media_durabilidade = (float)durabilidade_total / num_produtos;

    fprintf(arquivo_saida, "# An�lise de Qualidade\n\n");
    fprintf(arquivo_saida, "## M�dias\n");
    fprintf(arquivo_saida, "- M�dia de resist�ncia: %.2f\n", media_resistencia);
    fprintf(arquivo_saida, "- M�dia de durabilidade: %.2f\n\n", media_durabilidade);

    fprintf(arquivo_saida, "## Produtos Destacados\n");
    fprintf(arquivo_saida, "- Maior resist�ncia (N�mero de s�rie: %d): %d\n", num_produto_maior_resistencia, maior_resistencia);
    fprintf(arquivo_saida, "- Menor resist�ncia (N�mero de s�rie: %d): %d\n", num_produto_menor_resistencia, menor_resistencia);
    fprintf(arquivo_saida, "- Maior durabilidade (N�mero de s�rie: %d): %d\n", num_produto_maior_durabilidade, maior_durabilidade);
    fprintf(arquivo_saida, "- Menor durabilidade (N�mero de s�rie: %d): %d\n\n", num_produto_menor_durabilidade, menor_durabilidade);

    if (maior_resistencia > media_resistencia)
        fprintf(arquivo_saida, "H� mais produtos com resist�ncia acima da m�dia.\n");
    else if (maior_resistencia < media_resistencia)
        fprintf(arquivo_saida, "H� mais produtos com resist�ncia abaixo da m�dia.\n");
    else
        fprintf(arquivo_saida, "N�o h� diferen�a na resist�ncia em rela��o � m�dia.\n");

    if (maior_durabilidade > media_durabilidade)
        fprintf(arquivo_saida, "H� mais produtos com durabilidade acima da m�dia.\n");
    else if (maior_durabilidade < media_durabilidade)
        fprintf(arquivo_saida, "H� mais produtos com durabilidade abaixo da m�dia.\n");
    else
        fprintf(arquivo_saida, "N�o h� diferen�a na durabilidade em rela��o � m�dia.\n");

    fclose(arquivo_saida);

    printf("An�lise conclu�da. Resultados salvos no arquivo 'qualidade.md'.\n");

    return 0;
}

