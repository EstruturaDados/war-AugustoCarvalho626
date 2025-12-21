/*======================================================================
    Construção de Territótio: Desafio Novato

    Nome: Augusto Bento Carvalho

    Objetivo Desta Parte:
    Implementando um sistema de cadastro de territórios com uso de structs.
======================================================================*/


#include <stdio.h>
#include <string.h>

// --- Constantes Globais ---
#define quant_tropa 5

// Definição da Estrutura
struct Territorio {
    char nome[50];
    char cor[20];
    int tropas;
};

int main() {
    struct Territorio tropa[quant_tropa]; // Array para armazenar quantidade de territórios
    int i;
    
    printf("========== Bem-Vindo ao jogo War! ==============\n");
    printf("------------ Cadastre 5 Territórios  -------------\n\n");

    // Loop para cadastro
    for(i = 0; i < quant_tropa; i++) {
        printf("Cadastro do %dº territorio:\n", i + 1);
        
        printf("Nome: ");
        scanf(" %[^\n]s", tropa[i].nome); // Lê inclusive espaços
        
        printf("Cor: ");
        scanf(" %[^\n]s", tropa[i].cor);
        
        printf("Numero de tropas: ");
        scanf("%d", &tropa[i].tropas);
        
        printf("---------------------------\n");
    }

    // Exibição dos dados (Opcional, mas bom para o trabalho)
    printf("=======================================\n");
    printf("Relatório de Territórios Cadastrados:\n");
    printf("=======================================\n");
    for(i = 0; i < quant_tropa; i++) {
        printf("TERRITÓRIO %d:\n", i+1);
        printf("Nome: %s\n", tropa[i].nome);
        printf("Dominado por: Exercito %s\n", tropa[i].cor);
        printf("Tropas: %d\n", tropa[i].tropas);
        printf("----------------------------\n");
    }

    return 0;
}