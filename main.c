#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_ITENS 10

// --- Estruturas e Enums ---

typedef struct {
    char nome[30];
    char tipo[20]; // ex: Cura, Municao, Arma
    int quantidade;
    int prioridade; // 1 (baixa) a 5 (alta - essencial para fuga)
} Item;

typedef enum {
    POR_NOME,
    POR_TIPO,
    POR_PRIORIDADE
} CriterioOrdenacao;

// --- Variáveis Globais ---
Item mochila[MAX_ITENS];
int numItens = 0;
int comparacoes = 0; // Para análise de desempenho
bool ordenadaPorNome = false; // Controle para a busca binária

// --- Protótipos ---
void limparTela();
void exibirMenu();
void inserirItem();
void removerItem();
void listarItens();
void menuDeOrdenacao();
void insertionSort(CriterioOrdenacao criterio);
void buscaBinariaPorNome();

// --- Função Principal ---
int main() {
    int opcao;
    
    do {
        exibirMenu();
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar(); // Limpar buffer do teclado

        switch (opcao) {
            case 1: inserirItem(); break;
            case 2: removerItem(); break;
            case 3: listarItens(); break;
            case 4: menuDeOrdenacao(); break;
            case 5: buscaBinariaPorNome(); break;
            case 0: printf("Saindo da ilha...\n"); break;
            default: printf("Opcao invalida!\n");
        }
        
        if (opcao != 0) {
            printf("\nPressione Enter para continuar...");
            getchar();
        }
        
    } while (opcao != 0);

    return 0;
}

// --- Implementações ---

void limparTela() {
    // Simula limpeza imprimindo várias linhas
    for(int i=0; i<50; i++) printf("\n");
}

void exibirMenu() {
    limparTela();
    printf("=== MOCHILA DE SOBREVIVENCIA (FREE FIRE ED.) ===\n");
    printf("Itens na mochila: %d/%d\n", numItens, MAX_ITENS);
    printf("Status da ordenacao: %s\n", ordenadaPorNome ? "ORDENADO POR NOME (Busca Binaria Ativa)" : "NAO ORDENADO");
    printf("------------------------------------------------\n");
    printf("1. Adicionar Item (Loot)\n");
    printf("2. Remover Item (Drop)\n");
    printf("3. Listar Itens\n");
    printf("4. Ordenar Mochila (Insertion Sort)\n");
    printf("5. Buscar Item (Busca Binaria)\n");
    printf("0. Sair\n");
    printf("------------------------------------------------\n");
}

void inserirItem() {
    if (numItens >= MAX_ITENS) {
        printf("ERRO: Mochila cheia! Voce precisa dropar algo antes.\n");
        return;
    }

    Item novo;
    printf("Nome do item: ");
    scanf(" %[^\n]", novo.nome); // Lê string com espaços
    printf("Tipo (ex: Municao, Cura): ");
    scanf(" %[^\n]", novo.tipo);
    printf("Quantidade: ");
    scanf("%d", &novo.quantidade);
    printf("Prioridade (1-5): ");
    scanf("%d", &novo.prioridade);

    mochila[numItens] = novo;
    numItens++;
    
    // Ao inserir novo item, a ordem é quebrada
    ordenadaPorNome = false; 
    printf("Item adicionado com sucesso!\n");
}

void removerItem() {
    if (numItens == 0) {
        printf("Mochila vazia.\n");
        return;
    }

    char nomeBusca[30];
    printf("Nome do item para remover: ");
    scanf(" %[^\n]", nomeBusca);

    int encontrado = -1;
    for (int i = 0; i < numItens; i++) {
        if (strcasecmp(mochila[i].nome, nomeBusca) == 0) { // strcasecmp ignora maiuscula/minuscula
            encontrado = i;
            break;
        }
    }

    if (encontrado != -1) {
        // Desloca os itens para preencher o buraco (Shift Left)
        for (int i = encontrado; i < numItens - 1; i++) {
            mochila[i] = mochila[i + 1];
        }
        numItens--;
        printf("Item removido!\n");
    } else {
        printf("Item nao encontrado.\n");
    }
}

void listarItens() {
    if (numItens == 0) {
        printf("Mochila vazia.\n");
        return;
    }

    printf("\n%-20s | %-15s | %-4s | %-4s\n", "NOME", "TIPO", "QTD", "PRIO");
    printf("----------------------------------------------------------\n");
    for (int i = 0; i < numItens; i++) {
        printf("%-20s | %-15s | %-4d | %-4d\n", 
            mochila[i].nome, mochila[i].tipo, mochila[i].quantidade, mochila[i].prioridade);
    }
}

void insertionSort(CriterioOrdenacao criterio) {
    int i, j;
    Item chave;
    comparacoes = 0;

    for (i = 1; i < numItens; i++) {
        chave = mochila[i];
        j = i - 1;

        bool precisaTrocar = false;

        // Loop de comparação do Insertion Sort
        while (j >= 0) {
            comparacoes++;
            
            if (criterio == POR_NOME) {
                // strcmp > 0 significa que mochila[j] vem DEPOIS da chave alfabeticamente
                if (strcasecmp(mochila[j].nome, chave.nome) > 0) precisaTrocar = true;
                else precisaTrocar = false;
            } 
            else if (criterio == POR_TIPO) {
                if (strcasecmp(mochila[j].tipo, chave.tipo) > 0) precisaTrocar = true;
                else precisaTrocar = false;
            }
            else if (criterio == POR_PRIORIDADE) {
                // Prioridade decrescente (Maior para Menor)
                if (mochila[j].prioridade < chave.prioridade) precisaTrocar = true;
                else precisaTrocar = false;
            }

            if (precisaTrocar) {
                mochila[j + 1] = mochila[j];
                j = j - 1;
            } else {
                break; // Encontrou a posição correta
            }
        }
        mochila[j + 1] = chave;
    }

    // Atualiza flag global
    if (criterio == POR_NOME) {
        ordenadaPorNome = true;
    } else {
        ordenadaPorNome = false;
    }
    
    printf("\nOrdenacao concluida! Comparacoes realizadas: %d\n", comparacoes);
}

void menuDeOrdenacao() {
    if (numItens < 2) {
        printf("Nao ha itens suficientes para ordenar.\n");
        return;
    }

    int op;
    printf("\n--- CRITERIO DE ORDENACAO ---\n");
    printf("1. Por Nome (A-Z)\n");
    printf("2. Por Tipo (A-Z)\n");
    printf("3. Por Prioridade (Maior para Menor)\n");
    printf("Opcao: ");
    scanf("%d", &op);

    switch(op) {
        case 1: insertionSort(POR_NOME); break;
        case 2: insertionSort(POR_TIPO); break;
        case 3: insertionSort(POR_PRIORIDADE); break;
        default: printf("Opcao invalida.\n");
    }
}

void buscaBinariaPorNome() {
    if (!ordenadaPorNome) {
        printf("ERRO: A mochila precisa estar ordenada por NOME para usar a Busca Binaria.\n");
        printf("Vá na opcao 4 e ordene por Nome primeiro.\n");
        return;
    }

    char alvo[30];
    printf("Digite o nome do item para buscar: ");
    scanf(" %[^\n]", alvo);

    int inicio = 0;
    int fim = numItens - 1;
    int meio;
    int encontrado = 0;
    int iteracoes = 0;

    printf("\nIniciando Busca Binaria...\n");

    while (inicio <= fim) {
        iteracoes++;
        meio = (inicio + fim) / 2;
        int comparacao = strcasecmp(alvo, mochila[meio].nome);

        if (comparacao == 0) {
            printf("\n--- ITEM ENCONTRADO! ---\n");
            printf("Nome: %s\n", mochila[meio].nome);
            printf("Tipo: %s\n", mochila[meio].tipo);
            printf("Quantidade: %d\n", mochila[meio].quantidade);
            printf("Prioridade: %d\n", mochila[meio].prioridade);
            printf("Posicao na mochila: %d\n", meio + 1);
            printf("Iteracoes da busca: %d\n", iteracoes);
            encontrado = 1;
            break;
        } 
        else if (comparacao < 0) {
            // Alvo é "menor" (vem antes), busca na esquerda
            fim = meio - 1;
        } 
        else {
            // Alvo é "maior" (vem depois), busca na direita
            inicio = meio + 1;
        }
    }

    if (!encontrado) {
        printf("Item \"%s\" nao encontrado na mochila.\n", alvo);
    }
}
