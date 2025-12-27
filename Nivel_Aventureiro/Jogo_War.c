/*
 * Nome: Augusto Bento Carvalho
 *
 * Jogo de War - Simulação Simplificada
 *
 * Este programa simula a mecânica básica de ataques do jogo War.
 * Funcionalidades:
 * - Alocação dinâmica de territórios.
 * - Simulação de batalhas com dados aleatórios.
 * - Transferência de posse de territórios.
 * - Gerenciamento de memória.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Definição da estrutura que representa um Território
typedef struct {
    int numero;         // Número do território
    char nome[50];      // Nome do território
    char cor[50];      // Nome do exército/jogador cor (ex: Azul, Vermelho)
    int tropas;         // Quantidade de exércitos no território
} Territorio;

/*
 * Função: cadastrarTerritorios
 * ----------------------------
 * Responsável por alocar a memória dinamicamente para o vetor de territórios
 * e solicitar ao usuário os dados iniciais de cada um.
 *
 * qtd: Quantidade de territórios a serem criados.
 *
 * Retorno: Ponteiro para o vetor de estruturas Territorio alocado.
 */
Territorio* cadastrarTerritorios(int qtd) {
    // Uso de calloc para alocar memória e inicializar com zero
    Territorio* mapa = (Territorio*) calloc(qtd, sizeof(Territorio));

    if (mapa == NULL) {
        printf("Erro crítico: Falha na alocação de memória.\n");
        exit(1);
    }

    printf("\n--- Cadastro de Territórios ---\n");
    for (int i = 0; i < qtd; i++) {
        mapa[i].numero = i + 1;
        printf("\nTerritório Número %d:\n", i + 1);
        
        printf("Nome do Território: ");
        scanf(" %[^\n]", mapa[i].nome); // Lê string com espaços
        
        printf("Cor do Exército: ");
        scanf(" %[^\n]", mapa[i].cor);
        
        printf("Quantidade de tropas iniciais: ");
        scanf("%d", &mapa[i].tropas);
    }
    
    return mapa;
}

/*
 * Função: exibirMapa
 * ------------------
 * Exibe o estado atual de todos os territórios (Dono e Tropas).
 *
 * mapa: Ponteiro para o vetor de territórios.
 * qtd: Tamanho do vetor.
 */
void exibirMapa(Territorio* mapa, int qtd) {
    printf("\n================ MAPA ATUAL ================\n");
    printf("%-6s | %-20s | %-15s | %s\n", "Número", "Nome", "Cor", "Tropas");
    printf("-----------------------------------------------------\n");
    for (int i = 0; i < qtd; i++) {
        // Acesso aos membros via ponteiro (aritmética de ponteiros mapa[i] é equivalente a *(mapa + i))
        printf("%-6d | %-20s | %-15s | %d\n", 
               mapa[i].numero, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
    printf("====================================================\n");
}

/*
 * Função: atacar
 * --------------
 * Simula um ataque entre dois territórios usando números aleatórios (dados).
 * Atualiza as tropas e, se o defensor perder todas, transfere a posse.
 *
 * atacante: Ponteiro para o território que está atacando.
 * defensor: Ponteiro para o território que está defendendo.
 */
void atacar(Territorio* atacante, Territorio* defensor) {
    printf("\n--- Iniciando Batalha ---\n");
    printf("%s (%s) ATACA %s (%s)\n", 
           atacante->nome, atacante->cor, defensor->nome, defensor->cor);

    // Validação básica: Atacante precisa de mais de 1 tropa para atacar
    if (atacante->tropas <= 1) {
        printf("Ataque cancelado: O território atacante precisa de mais de 1 tropa.\n");
        return;
    }

    // Simulação de dados (1 a 6)
    int dadoAtaque = (rand() % 6) + 1;
    int dadoDefesa = (rand() % 6) + 1;

    printf("Dado do Atacante: %d\n", dadoAtaque);
    printf("Dado do Defensor: %d\n", dadoDefesa);

    // Regra simplificada: Maior vence. Empate favorece a defesa.
    if (dadoAtaque > dadoDefesa) {
        printf("Resultado: Vitoria do ATACANTE!\n");
        defensor->tropas--; // Defensor perde uma tropa

        // Verifica se o defensor foi conquistado
        if (defensor->tropas <= 0) {
            printf(">>> TERRITÓRIO CONQUISTADO! <<<\n");
            printf("%s agora pertence ao exército %s.\n", defensor->nome, atacante->cor);
            
            // Troca de cor
            strcpy(defensor->cor, atacante->cor);
            
            // Move 1 tropa do atacante para ocupar o novo território
            defensor->tropas = 1;
            atacante->tropas--;
        }
    } else {
        printf("Resultado: Vitoria da DEFESA!\n");
        atacante->tropas--; // Atacante perde uma tropa
    }
    
    // Exibição pós-ataque imediata
    printf("\nStatus Pós-Batalha:\n");
    printf("Atacante (%s): %d tropas\n", atacante->nome, atacante->tropas);
    printf("Defensor (%s): %d tropas\n", defensor->nome, defensor->tropas);
}

/*
 * Função: liberarMemoria
 * ----------------------
 * Libera o espaço de memória alocado para o vetor de territórios.
 *
 * mapa: Ponteiro para o vetor a ser liberado.
 */
void liberarMemoria(Territorio* mapa) {
    if (mapa != NULL) {
        free(mapa);
        printf("\nMemória liberada com sucesso. Encerrando jogo.\n");
    }
}

int main() {
    // Inicializa o gerador de números aleatórios
    srand(time(NULL));

    int qtdTerritorios;
    Territorio* mapa = NULL;
    int opcao, idAtk, idDef;

    printf("Bem-vindo ao Jogo de War (Simulador)\n");
    printf("Quantos territórios deseja criar no mapa? ");
    scanf("%d", &qtdTerritorios);

    if (qtdTerritorios < 2) {
        printf("É necessário pelo menos 2 territórios para jogar.\n");
        return 1;
    }

    // 1. Alocação e Cadastro
    mapa = cadastrarTerritorios(qtdTerritorios);

    // Loop do Jogo
    do {
        printf("\n--- MENU ---\n");
        printf("1. Exibir Mapa\n");
        printf("2. Realizar Ataque\n");
        printf("3. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                // 2. Exibição
                exibirMapa(mapa, qtdTerritorios);
                break;

            case 2:
                // Interface amigável para selecionar territórios
                exibirMapa(mapa, qtdTerritorios);
                printf("\nDigite o Número do território ATACANTE: ");
                scanf("%d", &idAtk);
                printf("Digite o Número do território DEFENSOR: ");
                scanf("%d", &idDef);

                // Validações de input
                if (idAtk < 1 || idAtk > qtdTerritorios || idDef < 1 || idDef > qtdTerritorios) {
                    printf("Números inválidos!\n");
                } else if (idAtk == idDef) {
                    printf("Um território não pode atacar a si mesmo.\n");
                } else if (strcmp(mapa[idAtk - 1].cor, mapa[idDef - 1].cor) == 0) {
                    printf("Você não pode atacar seu próprio aliado!\n");
                } else {
                    // 3. Simulação de Ataque (Passagem por ponteiro)
                    // &mapa[id] pega o endereço do elemento específico do vetor
                    atacar(&mapa[idAtk - 1], &mapa[idDef - 1]);
                }
                break;

            case 3:
                printf("Saindo...\n");
                break;

            default:
                printf("Opção inválida.\n");
        }

    } while (opcao != 3);

    // 4. Liberação de Memória
    liberarMemoria(mapa);
    mapa = NULL; // Boa prática: anular o ponteiro após o free

    return 0;
}