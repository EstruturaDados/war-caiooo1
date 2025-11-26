/*
 * NÍVEL NOVATO - Cadastro Estático
 * Objetivo: Structs, Vetores e Input/Output básico.
 */
#include <stdio.h>
#include <string.h>

// 1. Definição da Struct
struct Territorio {
    char nome[30];
    char cor[15]; // Ex: "Azul", "Vermelho"
    int tropas;
};

int main() {
    // 2. Vetor estático (memória alocada automaticamente na compilação)
    struct Territorio mapa[5]; 
    int i;

    printf("=== SETUP DO MAPA (NÍVEL NOVATO) ===\n");

    // 3. Entrada de Dados
    for(i = 0; i < 5; i++) {
        printf("\nTerritorio %d:\n", i + 1);
        
        // Leitura do Nome com fgets (seguro para strings com espaços)
        printf("Nome: ");
        fgets(mapa[i].nome, 30, stdin);
        // Remove o \n que o fgets captura
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = 0;

        printf("Cor do Exercito: ");
        fgets(mapa[i].cor, 15, stdin);
        mapa[i].cor[strcspn(mapa[i].cor, "\n")] = 0;

        printf("Numero de Tropas: ");
        scanf("%d", &mapa[i].tropas);
        getchar(); // Limpa o buffer do teclado após o scanf
    }

    // 4. Exibição
    printf("\n=== ESTADO ATUAL DO MUNDO ===\n");
    for(i = 0; i < 5; i++) {
        printf("[%d] %-20s | Cor: %-10s | Tropas: %d\n", 
               i+1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }

    return 0;
}