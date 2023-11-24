#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#pragma warning (disable : 4996)

#define TAM_VET 130

typedef struct f {
	char nome_filme[81];
	int ano;
	char nome_diretor[51];
	char genero[31];
	int duracao;
}FILME;

typedef struct ArvB {
	int nchaves, folha;
	struct ArvB** filho;
	int* chave_primaria;
	FILME* filme;
}TARVB;

typedef struct nomedir {
	char nome_dir[51];
	struct nomedir* prox;

}ND;
typedef ND* THD[TAM_VET];


int conta_char(FILE* aux);//ajeitar


TARVB* TARVB_Inicializa();
TARVB* TARVB_Cria(int t);
TARVB* TARVB_Libera(TARVB* a);
TARVB* TARVB_Busca(TARVB* x, int ch);
TARVB* TARVB_Insere(TARVB* T, int k, int t);
TARVB* TARVB_Retira(TARVB* arv, int k, int t);
TARVB* Divisao(TARVB* x, int i, TARVB* y, int t);
TARVB* Insere_Nao_Completo(TARVB* x, int k, int t);
void Busca(TARVB* a, int ch);
void TARVB_Imprime(TARVB* a);