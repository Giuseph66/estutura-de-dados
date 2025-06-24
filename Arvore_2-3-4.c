/****************************************************************
 * Trabalho T2 – Estrutura de Dados
 * Tema: Árvore 2-3-4 
 *
 * Curso/Disciplina : Estrutura de Dados
 * Professor(a)     : Ramilton Costa Gomes Junior
 * Academico        : Giuseph
 *
 * Requisitos do trabalho:
 *   • Pesquisa teórica sobre o tema;
 *   • Implementação em C da Árvore 2-3-4;
 *
 * Este arquivo contém a implementação completa solicitada, com:
 *   – Inserção, busca, remoção;
 *   – Exibição em formato lateral (ramo direito para cima);
 *   – Opção interativa de exemplo automático.
 ****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

typedef struct No {
    int qtdChaves;          
    bool folha;             
    int *chaves;            
    struct No **filhos;     
} No;

typedef struct {
    int t;        
    No *raiz;     
} ArvoreB;

static No   *criarNo(bool folha);
static ArvoreB *criarArvore(void);
static void dividirFilho(No *x, int i);
static void inserirNaoCheio(No *x, int chave);
static void inserirChave(ArvoreB *arv, int chave);
static No   *buscar(No *x, int chave);
static void removerChave(ArvoreB *arv, int chave);
static void removerDeNo(No *no, int chave);
static void removerDeFolha(No *no, int idx);
static void removerDeNaoFolha(No *no, int idx);
static int  obterPredecessor(No *no);
static int  obterSucessor(No *no);
static void preencher(No *no, int idx);
static void pegarDoAnterior(No *no, int idx);
static void pegarDoProximo(No *no, int idx);
static void fundirNos(No *no, int idx);
static void imprimirLateral(No *no, int ident);
static void liberarNo(No *no);

static No *criarNo(bool folha) {
    No *no = malloc(sizeof(No));
    if (!no) { perror("Falha ao alocar nó"); exit(EXIT_FAILURE); }
    no->folha     = folha;
    no->qtdChaves = 0;
    no->chaves  = calloc(3,       sizeof(int));  
    no->filhos  = calloc(4, sizeof(No *));       
    if (!no->chaves || !no->filhos) {
        perror("Falha ao alocar estruturas internas do nó");
        exit(EXIT_FAILURE);
    }
    return no;
}

static ArvoreB *criarArvore(void) {
    ArvoreB *arv = malloc(sizeof(ArvoreB));
    if (!arv) { perror("Falha ao alocar árvore"); exit(EXIT_FAILURE); }
    arv->t   = 2;
    arv->raiz = criarNo(true);
    return arv;
}

static void dividirFilho(No *x, int i) {
    No *y = x->filhos[i];
    No *z = criarNo(y->folha);
    
    z->qtdChaves = 1;
    z->chaves[0] = y->chaves[2];
    
    if (!y->folha) {
        z->filhos[0] = y->filhos[2];
        z->filhos[1] = y->filhos[3];
    }

    y->qtdChaves = 1; 
    
    for (int j = x->qtdChaves; j >= i + 1; j--)
        x->filhos[j + 1] = x->filhos[j];
    x->filhos[i + 1] = z;
    
    for (int j = x->qtdChaves - 1; j >= i; j--)
        x->chaves[j + 1] = x->chaves[j];
    x->chaves[i] = y->chaves[1];

    x->qtdChaves++;
}

static void inserirNaoCheio(No *x, int chave) {
    int i = x->qtdChaves - 1;

    if (x->folha) {
        while (i >= 0 && chave < x->chaves[i]) {
            x->chaves[i + 1] = x->chaves[i];
            i--;
        }
        x->chaves[i + 1] = chave;
        x->qtdChaves++;
    } else {
        while (i >= 0 && chave < x->chaves[i]) i--;
        i++;
        if (x->filhos[i]->qtdChaves == 3) {
            dividirFilho(x, i);
            if (chave > x->chaves[i]) i++;
        }
        inserirNaoCheio(x->filhos[i], chave);
    }
}

static void inserirChave(ArvoreB *arv, int chave) {
    if (buscar(arv->raiz, chave)) {
        printf("Chave %d já existe. Inserção ignorada.\n", chave);
        return;
    }

    No *r = arv->raiz;
    if (r->qtdChaves == 3) {
        No *s = criarNo(false);
        arv->raiz = s;
        s->filhos[0] = r;
        dividirFilho(s, 0);
        inserirNaoCheio(s, chave);
    } else {
        inserirNaoCheio(r, chave);
    }
    printf("Chave %d inserida com sucesso.\n", chave);
}

static No *buscar(No *x, int chave) {
    int i = 0;
    while (i < x->qtdChaves && chave > x->chaves[i]) i++;

    if (i < x->qtdChaves && chave == x->chaves[i]) return x;
    if (x->folha) return NULL;
    return buscar(x->filhos[i], chave);
}

static void removerDeFolha(No *no, int idx) {
    for (int i = idx + 1; i < no->qtdChaves; i++)
        no->chaves[i - 1] = no->chaves[i];
    no->qtdChaves--;
}

static int obterPredecessor(No *no) {
    while (!no->folha) no = no->filhos[no->qtdChaves];
    return no->chaves[no->qtdChaves - 1];
}

static int obterSucessor(No *no) {
    while (!no->folha) no = no->filhos[0];
    return no->chaves[0];
}

static void pegarDoAnterior(No *no, int idx) {
    No *filho   = no->filhos[idx];
    No *irmao   = no->filhos[idx - 1];

    for (int i = filho->qtdChaves - 1; i >= 0; i--)
        filho->chaves[i + 1] = filho->chaves[i];
    if (!filho->folha) {
        for (int i = filho->qtdChaves; i >= 0; i--)
            filho->filhos[i + 1] = filho->filhos[i];
    }
    filho->chaves[0] = no->chaves[idx - 1];
    if (!filho->folha)
        filho->filhos[0] = irmao->filhos[irmao->qtdChaves];
    no->chaves[idx - 1] = irmao->chaves[irmao->qtdChaves - 1];

    filho->qtdChaves++;
    irmao->qtdChaves--;
}

static void pegarDoProximo(No *no, int idx) {
    No *filho = no->filhos[idx];
    No *irmao = no->filhos[idx + 1];

    filho->chaves[filho->qtdChaves] = no->chaves[idx];
    if (!filho->folha)
        filho->filhos[filho->qtdChaves + 1] = irmao->filhos[0];
    no->chaves[idx] = irmao->chaves[0];

    for (int i = 1; i < irmao->qtdChaves; i++)
        irmao->chaves[i - 1] = irmao->chaves[i];
    if (!irmao->folha) {
        for (int i = 1; i <= irmao->qtdChaves; i++)
            irmao->filhos[i - 1] = irmao->filhos[i];
    }
    filho->qtdChaves++;
    irmao->qtdChaves--;
}

static void fundirNos(No *no, int idx) {
    No *filho  = no->filhos[idx];
    No *irmao  = no->filhos[idx + 1];

    filho->chaves[1] = no->chaves[idx];           
    filho->chaves[2] = irmao->chaves[0];          
    if (!filho->folha) {
        filho->filhos[2] = irmao->filhos[0];
        filho->filhos[3] = irmao->filhos[1];
    }

    for (int i = idx + 1; i < no->qtdChaves; i++)
        no->chaves[i - 1] = no->chaves[i];
    for (int i = idx + 2; i <= no->qtdChaves; i++)
        no->filhos[i - 1] = no->filhos[i];

    filho->qtdChaves += 1 + irmao->qtdChaves;
    no->qtdChaves--;

    free(irmao->chaves); free(irmao->filhos); free(irmao);
}

static void preencher(No *no, int idx) {
    if (idx != 0 && no->filhos[idx - 1]->qtdChaves >= 2)
        pegarDoAnterior(no, idx);
    else if (idx != no->qtdChaves && no->filhos[idx + 1]->qtdChaves >= 2)
        pegarDoProximo(no, idx);
    else {
        if (idx != no->qtdChaves)
            fundirNos(no, idx);
        else
            fundirNos(no, idx - 1);
    }
}

static void removerDeNaoFolha(No *no, int idx) {
    int chave = no->chaves[idx];

    if (no->filhos[idx]->qtdChaves >= 2) {
        int pred = obterPredecessor(no->filhos[idx]);
        no->chaves[idx] = pred;
        removerDeNo(no->filhos[idx], pred);
    } else if (no->filhos[idx + 1]->qtdChaves >= 2) {
        int suc = obterSucessor(no->filhos[idx + 1]);
        no->chaves[idx] = suc;
        removerDeNo(no->filhos[idx + 1], suc);
    } else {
        fundirNos(no, idx);
        removerDeNo(no->filhos[idx], chave);
    }
}

static void removerDeNo(No *no, int chave) {
    int idx = 0;
    while (idx < no->qtdChaves && no->chaves[idx] < chave) idx++;

    if (idx < no->qtdChaves && no->chaves[idx] == chave) {
        if (no->folha)
            removerDeFolha(no, idx);
        else
            removerDeNaoFolha(no, idx);
    } else {
        if (no->folha) {
            printf("Chave %d não existe.\n", chave);
            return;
        }
        bool flag = (idx == no->qtdChaves);
        if (no->filhos[idx]->qtdChaves < 2)
            preencher(no, idx);
        if (flag && idx > no->qtdChaves)
            removerDeNo(no->filhos[idx - 1], chave);
        else
            removerDeNo(no->filhos[idx], chave);
    }
}

static void removerChave(ArvoreB *arv, int chave) {
    if (!arv->raiz) { printf("Árvore vazia.\n"); return; }

    removerDeNo(arv->raiz, chave);

    if (arv->raiz->qtdChaves == 0) {
        No *tmp = arv->raiz;
        arv->raiz = arv->raiz->folha ? NULL : arv->raiz->filhos[0];
        free(tmp->chaves); free(tmp->filhos); free(tmp);
    }
    printf("Chave %d removida (se existia).\n", chave);
}

static void imprimirLateral(No *no, int ident) {
    if (!no) return;
    const int ESP = 6;

    imprimirLateral(no->filhos[no->qtdChaves], ident + ESP);

    for (int i = 0; i < ident; i++) putchar(' ');
    printf("[");
    for (int i = 0; i < no->qtdChaves; i++) {
        printf("%d", no->chaves[i]);
        if (i < no->qtdChaves - 1) printf(" ");
    }
    printf("]\n");

    for (int i = no->qtdChaves - 1; i >= 0; i--)
        imprimirLateral(no->filhos[i], ident + ESP);
}

static void liberarNo(No *no) {
    if (!no) return;
    if (!no->folha)
        for (int i = 0; i <= no->qtdChaves; i++)
            liberarNo(no->filhos[i]);
    free(no->chaves); free(no->filhos); free(no);
}

int main(void) {
    ArvoreB *arv = criarArvore();
    srand((unsigned)time(NULL));
    int op, chave;

    while (1) {
        printf("\n--- Árvore 2-3-4 ---\n");
        printf("1. Inserir chave\n");
        printf("2. Buscar chave\n");
        printf("3. Remover chave\n");
        printf("4. Exibir árvore\n");
        printf("5. Exemplo (10 valores aleatórios de 0 a 999)\n");
        printf("0. Sair\n");
        printf("Escolha: ");
        if (scanf("%d", &op) != 1) break;

        switch (op) {
            case 1:
                printf("Valor inteiro para inserir: ");
                if (scanf("%d", &chave) != 1) continue;
                inserirChave(arv, chave);
                printf("\nÁrvore após inserção:\n");
                imprimirLateral(arv->raiz, 0);
                break;
            case 2:
                printf("Valor inteiro para buscar: ");
                if (scanf("%d", &chave) != 1) continue;
                if (buscar(arv->raiz, chave))
                    printf("Chave %d encontrada.\n", chave);
                else
                    printf("Chave %d não encontrada.\n", chave);
                break;
            case 3:
                printf("Valor inteiro para remover: ");
                if (scanf("%d", &chave) != 1) continue;
                removerChave(arv, chave);
                break;
            case 4:
                printf("\nEstrutura da árvore:\n");
                imprimirLateral(arv->raiz, 0);
                break;
            case 5:
                printf("\nInserindo 10 valores aleatórios de 0 a 999:\n");
                for (int i = 0; i < 10; i++) {
                    int valor = rand() % 1000;
                    printf("\nValor gerado: %d\n", valor);
                    inserirChave(arv, valor);
                    printf("Árvore após inserção de %d:\n", valor);
                    imprimirLateral(arv->raiz, 0);
                }
                break;
            case 0:
                liberarNo(arv->raiz); free(arv);
                return 0;
            default:
                printf("Opção inválida!\n");
        }
    }
    liberarNo(arv->raiz); free(arv);
    return 0;
} 