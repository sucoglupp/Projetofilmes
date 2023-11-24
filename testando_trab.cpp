#include "funcs.h"
#pragma once
#pragma warning (disable : 4996)


int cria_chaveprim(FILME *filme){
	int x  = filme->ano;
	int aux;
	sscanf(filme->nome_filme, "%d", &aux);
	x += aux;
	printf("%d\n", x);
	return x;
}


TARVB* TARVB_Insere(TARVB* T, int k, int t,FILME *filme) {
	if (TARVB_Busca(T, k)) return T;
	if (!T) {
		T = TARVB_Cria(t);
		T->chave_primaria[0] = k;
		T->nchaves = 1;
		return T;
	}
	if (T->nchaves == (2 * t) - 1) {
		TARVB* S = TARVB_Cria(t);
		S->nchaves = 0;
		S->folha = 0;
		S->filho[0] = T;
		S = Divisao(S, 1, T, t);
		S = Insere_Nao_Completo(S, k, t);
		return S;
	}
	T = Insere_Nao_Completo(T, k, t);
	return T;
}

TARVB* Insere_Nao_Completo(TARVB* x, int k, int t) {
	int i = x->nchaves - 1;
	if (x->folha) {
		while ((i >= 0) && (k < x->chave_primaria[i])) {
			x->chave_primaria[i + 1] = x->chave_primaria[i];
			i--;
		}
		x->chave_primaria[i + 1] = k;
		x->nchaves++;
		return x;
	}
	while ((i >= 0) && (k < x->chave_primaria[i])) i--;
	i++;
	if (x->filho[i]->nchaves == ((2 * t) - 1)) {
		x = Divisao(x, (i + 1), x->filho[i], t);
		if (k > x->chave_primaria[i]) i++;
	}
	x->filho[i] = Insere_Nao_Completo(x->filho[i], k, t);
	return x;
}



void ins_filme_arv(int t,char *nome,TARVB *T) {
	FILE* fp = fopen(nome, "rt");
	if (!fp) exit(1);
	char aux[130];//AJEITAR MALLOC
	char* token;
	int i = 0;
	while (fgets(aux, 130, fp)) {
		FILME *aux_filme;
		const char* s = "/";
		token = strtok(aux, s);
		strcpy(aux_filme->nome_filme, token);
		token = strtok(NULL, s);
		aux_filme->ano = atoi(token);
		token = strtok(NULL, s);
		strcpy(aux_filme->nome_diretor, token);
		token = strtok(NULL, s);
		strcpy(aux_filme->genero, token);
		token = strtok(NULL, s);
		aux_filme->duracao = atoi(token);

		int ch = cria_chaveprim(aux_filme);
		TARVB_Insere(T,ch, t,aux_filme);
	}
	fclose(fp);
}



TARVB* Divisao(TARVB* x, int i, TARVB* y, int t) {
	TARVB* z = TARVB_Cria(t);
	z->nchaves = t - 1;
	z->folha = y->folha;
	int j;
	for (j = 0; j < t - 1; j++) z->chave_primaria[j] = y->chave_primaria[j + t];
	if (!y->folha) {
		for (j = 0; j < t; j++) {
			z->filho[j] = y->filho[j + t];
			y->filho[j + t] = NULL;
		}
	}
	y->nchaves = t - 1;
	for (j = x->nchaves; j >= i; j--) x->filho[j + 1] = x->filho[j];
	x->filho[i] = z;
	for (j = x->nchaves; j >= i; j--) x->chave_primaria[j] = x->chave_primaria[j - 1];
	x->chave_primaria[i - 1] = y->chave_primaria[t - 1];
	x->nchaves++;
	return x;
}




TARVB* TARVBF_Cria(int t) {
	TARVB* novo = (TARVB*)malloc(sizeof(TARVB));
	novo->nchaves = 0;
	novo->chave_primaria = (int*)malloc(sizeof(int*) * ((t * 2) - 1));
	novo->folha = 1;
	novo->filho = (TARVB**)malloc(sizeof(TARVB*) * t * 2);
	int i;
	for (i = 0; i < (t * 2); i++) novo->filho[i] = NULL;
	return novo;
}


TARVB* TARVB_Busca(TARVB* x, int ch) {
	if (!x) return NULL;
	int i = 0;
	while (i < x->nchaves && ch > x->chave_primaria[i]) i++;
	if (i < x->nchaves && ch == x->chave_primaria[i]) return x;
	if (x->folha) return NULL;
	return TARVB_Busca(x->filho[i], ch);
}

void Busca(TARVB* a, int ch) {
	TARVB* no = TARVB_Busca(a, ch);
	FILME* f = no->filme;
	printf("nome do diretor: %s, genero: %s, duracao: %d", f->nome_diretor, f->genero, f->duracao);
}



int main() {
	//char nome[100];
	//printf("Digite o nome do arq> ");
	//scanf_s("%d", nome);
	// int t;
	//printf("Digite o tm t> ");
	//scanf_s("%d", t);
	//ins_filme_arv(t,nome,arv)
	

	return 0;
}



//valgrind --tool=memcheck --leak-check=yes ./<nome_executavel>