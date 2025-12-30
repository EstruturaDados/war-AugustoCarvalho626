/*
 * Nome: Augusto Bento Carvalho
 *
 * Jogo End War
 *
 * Descrição:
 * Este programa simula um jogo de estratégia simplificado baseado em turnos e conquista de territórios.
 * Utiliza alocação dinâmica de memória, ponteiros e estruturação modular.
 *
 * Funcionalidades:
 * - Cadastro de territórios.
 * - Sorteio de missões dinâmicas.
 * - Sistema de ataque com simulação de dados (rand).
 * - Verificação de condições de vitória.
 * - Gerenciamento de memória (malloc/calloc/free).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Definição da estrutura que representa um Território
typedef struct {
    int numero_territorio;  // Número único do território (1 a 5)
    char nome_exercito[50]; // Nome do exército que domina o território
    char cor[20];           // Cor do exército
    int tropas;             // Quantidade de tropas no território
} Territorio;

// --- Protótipos das Funções ---

// Função para preencher os dados iniciais dos territórios
void cadastrarTerritorios(Territorio* mapa, int qtd);

// Função para sortear e atribuir uma missão ao jogador (alocação dinâmica)
void atribuirmissao(char** missao_ptr, int* id_origem, int* id_alvo, int qtd_territorios);

// Função para exibir o estado atual de todos os territórios
void exibirmapa(Territorio* mapa, int qtd);

// Função para exibir a missão atual
void exibirmissao(char* missao);

// Função para realizar um ataque entre dois territórios
void atacar(Territorio* atacante, Territorio* defensor);

// Função para verificar se a missão foi cumprida
int verificarmissao(Territorio* mapa, int id_origem, int id_alvo, int ultimo_atacante);

// Função para liberar toda a memória alocada dinamicamente
void liberarmemoria(Territorio* mapa, char* missao);

// --- Função Principal (Main) ---

int main() {
    // Inicializa o gerador de números aleatórios
    srand(time(NULL));

    int qtd_territorios = 5;
    int id_origem_missao = -1; // Armazenará o ID do território que deve conquistar
    int id_alvo_missao = -1; // Armazenará o ID do território que precisa ser conquistado
    char* missao_jogador = NULL; // Ponteiro para a string da missão (será alocado dinamicamente)
    int ultimo_atacante = 0; // Armazena o ID do último território que atacou

    printf("=== BEM-VINDO AO JOGO END WAR ===\n");
    printf("Iniciando configuração do mapa...\n\n");

    // ALOCAÇÃO DE MEMÓRIA: Usa calloc para criar o vetor de territórios (inicia zerado)
    Territorio* mapa = (Territorio*) calloc(qtd_territorios, sizeof(Territorio));
    if (mapa == NULL) {
        printf("Erro crítico: Falha na alocação de memória para o mapa.\n");
        return 1;
    }

    // Cadastro inicial dos territórios
    cadastrarTerritorios(mapa, qtd_territorios);

    // Atribuição da missão
    atribuirmissao(&missao_jogador, &id_origem_missao, &id_alvo_missao, qtd_territorios);
    exibirmissao(missao_jogador); // Exibe a missão apenas uma vez no início

    int opcao = 0;
    int jogo_ativo = 1;

    // Loop principal do jogo
    while (jogo_ativo) {
        printf("\n--- MENU PRINCIPAL ---\n");
        printf("1. Exibir Mapa (Status dos Territórios)\n");
        printf("2. Atacar\n");
        printf("3. Sair do Jogo\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                exibirmapa(mapa, qtd_territorios);
                break;
            case 2: {
                int id_atk, id_def;
                printf("\n--- FASE DE ATAQUE ---\n");
                printf("Digite o Número do território atacante (1-%d): ", qtd_territorios);
                scanf("%d", &id_atk);
                printf("Digite o Número do território alvo (1-%d): ", qtd_territorios);
                scanf("%d", &id_def);

                // Validação dos índices
                if (id_atk < 1 || id_atk > qtd_territorios || id_def < 1 || id_def > qtd_territorios) {
                    printf("Erro: IDs de território inválidos.\n");
                } else if (id_atk == id_def) {
                    printf("Erro: Um território não pode atacar a si mesmo.\n");
                } else {
                    // Passagem por referência (ponteiros) para a função atacar
                    // Subtraímos 1 porque o vetor começa em 0, mas os IDs começam em 1
                    ultimo_atacante = id_atk;
                    atacar(&mapa[id_atk - 1], &mapa[id_def - 1]);
                }
                break;
            }
            case 3:
                printf("Saindo do jogo...\n");
                jogo_ativo = 0;
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
        }

        // Verificação de vitória ao final de cada ação
        // Se o alvo ou a origem forem destruídos, o jogo acaba
        if (mapa[id_alvo_missao - 1].tropas <= 0 || mapa[id_origem_missao - 1].tropas <= 0) {
            jogo_ativo = 0;
        }
    }

    printf("\n=== FIM DE JOGO ===\n");
    exibirmapa(mapa, qtd_territorios); // Mostra a tabela final para conferência

    if (verificarmissao(mapa, id_origem_missao, id_alvo_missao, ultimo_atacante)) {
        printf("\n**************************************************\n");
        printf("Missão Cumprida!\n");
        printf("**************************************************\n");
    } else if (mapa[id_origem_missao - 1].tropas == 0) {
        printf("\nMissão não cumprida: Território %d não dominou Território %d (Origem destruída).\n", id_origem_missao, id_alvo_missao);
    } else {
        printf("\nMissão não foi cumprida ou fim de jogo: Território %d não dominou Território %d.\n", id_origem_missao, id_alvo_missao);
    }

    // Liberação de memória antes de fechar o programa
    liberarmemoria(mapa, missao_jogador);

    return 0;
}

// --- Implementação das Funções ---

void cadastrarTerritorios(Territorio* mapa, int qtd) {
    for (int i = 0; i < qtd; i++) {
        mapa[i].numero_territorio = i + 1; // Define Número de 1 a 5
        printf("\n--- Cadastro do Território %d ---\n", mapa[i].numero_territorio);
        
        printf("Nome do Exército: ");
        scanf(" %[^\n]s", mapa[i].nome_exercito); // Lê string com espaços
        
        printf("Cor do Exército: ");
        scanf(" %[^\n]s", mapa[i].cor);
        
        printf("Quantidade de Tropas: ");
        scanf("%d", &mapa[i].tropas);
    }
    printf("\nTodos os territórios foram cadastrados com sucesso!\n");
}

void atribuirmissao(char** missao_ptr, int* id_origem, int* id_alvo, int qtd_territorios) {
    // Sorteia um território de origem e um alvo diferente
    *id_origem = (rand() % qtd_territorios) + 1;
    do {
        *id_alvo = (rand() % qtd_territorios) + 1;
    } while (*id_alvo == *id_origem);

    // Cria a string da missão
    char buffer[100];
    sprintf(buffer, "Território %d deve destruir as tropas do Território %d", *id_origem, *id_alvo);

    // ALOCAÇÃO DE MEMÓRIA: malloc para a string da missão
    *missao_ptr = (char*) malloc((strlen(buffer) + 1) * sizeof(char));
    
    if (*missao_ptr != NULL) {
        strcpy(*missao_ptr, buffer); // Copia a string para a memória alocada
        printf("\n[SISTEMA] Missão sorteada e atribuída!\n");
    } else {
        printf("\n[ERRO] Falha ao alocar memória para a missão.\n");
    }
}

void exibirmapa(Territorio* mapa, int qtd) {
    printf("\n--- MAPA ATUAL ---\n");
    for (int i = 0; i < qtd; i++) {
        printf("Num: %d | Exército: %-15s | Cor: %-10s | Tropas: %d\n", 
               mapa[i].numero_territorio, mapa[i].nome_exercito, mapa[i].cor, mapa[i].tropas);
    }
    printf("------------------\n");
}

void exibirmissao(char* missao) {
    if (missao != NULL) {
        printf("\n>>> SUA MISSÃO: %s <<<\n", missao);
    } else {
        printf("\nNenhuma missão atribuída.\n");
    }
}

void atacar(Territorio* atacante, Territorio* defensor) {
    if (atacante->tropas <= 1 || defensor->tropas == 0) {
        printf("\n[ATAQUE FALHOU] Condições inválidas (Atacante <= 1 tropa ou Defensor com 0).\n");
        return;
    }

    printf("\nIniciando combate: %s (Num %d) VS %s (Num %d)\n", 
           atacante->nome_exercito, atacante->numero_territorio, defensor->nome_exercito, defensor->numero_territorio);

    // Simulação de rolagem de dados (1 a 6)
    int dado_atk = (rand() % 6) + 1;
    int dado_def = (rand() % 6) + 1;

    printf("Rolagem de dados -> Atacante: %d | Defensor: %d\n", dado_atk, dado_def);

    if (dado_atk > dado_def) {
        printf("VITORIA DO ATACANTE!\n");
        // Atacante vence: Defensor perde 10 tropas
        defensor->tropas -= 10;
        if (defensor->tropas < 0) defensor->tropas = 0;
        printf("O defensor sofreu dano massivo! Perdeu 10 tropas. Restantes: %d\n", defensor->tropas);
    } else {
        printf("DERROTA DO ATACANTE!\n");
        // Atacante perde: Atacante perde 5, Defensor perde 0
        atacante->tropas -= 5;
        
        if (atacante->tropas < 0) atacante->tropas = 0;
        
        printf("O atacante perdeu 5 tropas. Tropas restantes: %d\n", atacante->tropas);
        printf("O defensor defendeu com sucesso e nao perdeu tropas. Tropas restantes: %d\n", defensor->tropas);
    }
}

int verificarmissao(Territorio* mapa, int id_origem, int id_alvo, int ultimo_atacante) {
    // A missão é cumprida quando as tropas do alvo chegam a zero E o atacante foi a origem
    return (mapa[id_alvo - 1].tropas <= 0 && ultimo_atacante == id_origem);
}

void liberarmemoria(Territorio* mapa, char* missao) {
    if (missao != NULL) free(missao); // Libera a string da missão
    if (mapa != NULL) free(mapa);     // Libera o vetor de territórios
    printf("\nMemória liberada com sucesso. Encerrando.\n");
}