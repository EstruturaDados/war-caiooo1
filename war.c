#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// --- ESTRUTURAS E CONSTANTES ---
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

// Banco de missões pré-definidas (Vetor de Strings Constantes)
const char* LISTA_MISSOES[] = {
    "DOMINACAO TOTAL: Conquistar todos os territorios.",
    "SUPREMACIA MILITAR: Ter mais de 20 tropas no total.",
    "ANIQUILACAO: Eliminar totalmente o exercito Vermelho."
};
const int TOTAL_MISSOES = 3;

// --- PROTÓTIPOS (MODULARIZAÇÃO) ---
void atribuirMissao(char** missaoDoJogador);
void cadastrarTerritorios(Territorio* mapa, int qtd);
void exibirMapa(const Territorio* mapa, int qtd);
void atacar(Territorio* atacante, Territorio* defensor);
int verificarMissao(char* missao, Territorio* mapa, int qtd, char* corJogador);
void liberarMemoria(Territorio* mapa, char* missao);

// --- FUNÇÃO PRINCIPAL ---
int main() {
    Territorio* mapa = NULL;
    char* missaoJogador = NULL; // Ponteiro para a string da missão
    int qtdTerritorios;
    int rodando = 1;
    char corJogador[10] = "Azul"; // Vamos assumir que o jogador é o "Azul"

    srand(time(NULL));

    printf("=== WAR: NIVEL MESTRE (FINAL) ===\n");
    printf("Quantos territorios no mapa? ");
    scanf("%d", &qtdTerritorios);

    // 1. ALOCAÇÃO DINÂMICA DO MAPA (calloc limpa a memória)
    mapa = (Territorio*) calloc(qtdTerritorios, sizeof(Territorio));
    
    // 2. ALOCAÇÃO E SORTEIO DA MISSÃO
    // Passamos o endereço do ponteiro (&missaoJogador) para alocar dentro da função
    atribuirMissao(&missaoJogador);

    if (!mapa || !missaoJogador) {
        printf("Erro fatal de memoria.\n");
        return 1;
    }

    // 3. CONFIGURAÇÃO INICIAL
    cadastrarTerritorios(mapa, qtdTerritorios);
    
    printf("\n>>> SUA MISSAO SECRETA: %s <<<\n", missaoJogador);
    printf("Pressione ENTER para comecar...");
    getchar(); getchar();

    // 4. GAME LOOP
    while (rodando) {
        int opcao, atq, def;
        
        exibirMapa(mapa, qtdTerritorios);
        
        printf("\n1. Atacar\n2. Ver Missao\n0. Sair\nEscolha: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1: // Fase de Ataque
                printf("ID do Atacante (1-%d): ", qtdTerritorios);
                scanf("%d", &atq);
                printf("ID do Defensor (1-%d): ", qtdTerritorios);
                scanf("%d", &def);
                
                if(atq > 0 && atq <= qtdTerritorios && def > 0 && def <= qtdTerritorios)
                    atacar(&mapa[atq-1], &mapa[def-1]);
                else
                    printf("Indices invalidos.\n");
                
                // Verificação de Vitória Pós-Ataque
                if (verificarMissao(missaoJogador, mapa, qtdTerritorios, corJogador)) {
                    printf("\n******************************************\n");
                    printf("PARABENS! VOCE CUMPRIU SUA MISSAO:\n%s\n", missaoJogador);
                    printf("******************************************\n");
                    rodando = 0; // Encerra o jogo
                }
                break;

            case 2: // Lembrar Missão
                printf("\n>>> OBJETIVO: %s\n", missaoJogador);
                break;

            case 0:
                rodando = 0;
                break;
        }
    }

    // 5. LIMPEZA DE MEMÓRIA
    liberarMemoria(mapa, missaoJogador);

    return 0;
}

// --- IMPLEMENTAÇÃO DAS FUNÇÕES ---

void atribuirMissao(char** missaoDoJogador) {
    // Sorteia um índice
    int indice = rand() % TOTAL_MISSOES;
    
    // Aloca memória exata para a string da missão (+1 para o caractere nulo \0)
    *missaoDoJogador = (char*) malloc(strlen(LISTA_MISSOES[indice]) + 1);
    
    if (*missaoDoJogador != NULL) {
        strcpy(*missaoDoJogador, LISTA_MISSOES[indice]);
    }
}

void cadastrarTerritorios(Territorio* mapa, int qtd) {
    // Para facilitar o teste, vamos preencher automaticamente,
    // mas num cenário real usariamos scanf como nos níveis anteriores.
    for(int i=0; i<qtd; i++) {
        sprintf(mapa[i].nome, "Territorio_%d", i+1);
        if(i == 0) strcpy(mapa[i].cor, "Azul"); // Garante pelo menos um do jogador
        else strcpy(mapa[i].cor, (rand()%2 == 0) ? "Vermelho" : "Branco");
        mapa[i].tropas = 3 + (rand() % 5); // 3 a 7 tropas
    }
    printf("\nTerritorios gerados aleatoriamente para agilidade do teste.\n");
}

void atacar(Territorio* atacante, Territorio* defensor) {
    printf("\nBatalha: %s (%s) vs %s (%s)\n", atacante->nome, atacante->cor, defensor->nome, defensor->cor);

    // Validações
    if(strcmp(atacante->cor, defensor->cor) == 0) {
        printf("Fogo amigo nao permitido!\n"); return;
    }
    if(atacante->tropas <= 1) {
        printf("Tropas insuficientes para ataque.\n"); return;
    }

    int d_atk = (rand() % 6) + 1;
    int d_def = (rand() % 6) + 1;
    printf("Dados: Atk [%d] x Def [%d] -> ", d_atk, d_def);

    if (d_atk > d_def) {
        printf("VITORIA DO ATAQUE!\n");
        int transferencia = atacante->tropas / 2;
        strcpy(defensor->cor, atacante->cor);
        defensor->tropas = transferencia;
        atacante->tropas -= transferencia;
    } else {
        printf("DEFESA VENCEU!\n");
        atacante->tropas--;
    }
}

// Lógica inteligente para interpretar a string da missão
int verificarMissao(char* missao, Territorio* mapa, int qtd, char* corJogador) {
    int condicaoAtingida = 0;

    // Lógica 1: Dominação Total
    if (strstr(missao, "DOMINACAO TOTAL") != NULL) {
        int contagem = 0;
        for(int i=0; i<qtd; i++) {
            if(strcmp(mapa[i].cor, corJogador) == 0) contagem++;
        }
        if(contagem == qtd) condicaoAtingida = 1;
    }
    // Lógica 2: Supremacia Militar (Total de tropas)
    else if (strstr(missao, "SUPREMACIA MILITAR") != NULL) {
        int totalTropas = 0;
        for(int i=0; i<qtd; i++) {
            if(strcmp(mapa[i].cor, corJogador) == 0) totalTropas += mapa[i].tropas;
        }
        if(totalTropas > 20) condicaoAtingida = 1;
    }
    // Lógica 3: Aniquilação (Vermelho)
    else if (strstr(missao, "ANIQUILACAO") != NULL) {
        int vermelhosVivos = 0;
        for(int i=0; i<qtd; i++) {
            if(strcmp(mapa[i].cor, "Vermelho") == 0) vermelhosVivos++;
        }
        if(vermelhosVivos == 0) condicaoAtingida = 1;
    }

    return condicaoAtingida;
}

void exibirMapa(const Territorio* mapa, int qtd) {
    printf("\nSTATUS DO MUNDO:\n");
    for(int i=0; i<qtd; i++) {
        printf("[%d] %s \t| %s \t| %d tropas\n", i+1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
    printf("------------------------------------------\n");
}

void liberarMemoria(Territorio* mapa, char* missao) {
    if(mapa) free(mapa);
    if(missao) free(missao);
    printf("\nMemoria liberada. Fim de jogo.\n");
}