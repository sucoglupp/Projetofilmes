#include "funcs.h"
#pragma once
#pragma warning (disable : 4996)

int cria_chaveprim(FILME filme) {
	int i = 0;
	int soma_chave = filme.ano;
	char aux_filme[81];
	strcpy(aux_filme, filme.nome_filme);
	while (aux_filme[i] != '\0') {
		soma_chave += (int)(aux_filme[i]);
		i++;
	}
	//printf("%d", soma_chave);
	return soma_chave;
}

TARVB* TARVB_Insere(TARVB* T, int k, int t, FILME f) {
	if (TARVB_Busca(T, k)) return T;
	if (!T) {
		T = TARVB_Cria(t);
		T->chave_primaria[0] = k;
		T->filme[0] = f;
		T->nchaves = 1;
		return T;
	}
	if (T->nchaves == (2 * t) - 1) {
		TARVB* S = TARVB_Cria(t);
		S->nchaves = 0;
		S->folha = 0;
		S->filho[0] = T;
		S = Divisao(S, 1, T, t);
		S = Insere_Nao_Completo(S, k, t, f);
		return S;
	}
	T = Insere_Nao_Completo(T, k, t, f);
	return T;
}

TARVB* Insere_Nao_Completo(TARVB* x, int k, int t, FILME filme) {
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
	x->filho[i] = Insere_Nao_Completo(x->filho[i], k, t, filme);
	return x;
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

void ins_filme_arv(int t, char* nome, TARVB* T) {
	FILE* fp = fopen(nome, "rt");
	if (!fp) {
		printf("Arquivo não encontrado.");
		exit(1);
	}

	char aux[170];

	char* token;
	int i = 0;
	while (fgets(aux, 130, fp)) {
		FILME aux_filme;
		const char* s = "/";
		token = strtok(aux, s);
		strcpy(aux_filme.nome_filme, token);
		token = strtok(NULL, s);
		aux_filme.ano = atoi(token);
		token = strtok(NULL, s);
		strcpy(aux_filme.nome_diretor, token);
		token = strtok(NULL, s);
		strcpy(aux_filme.genero, token);
		token = strtok(NULL, s);
		aux_filme.duracao = atoi(token);

		int ch = cria_chaveprim(aux_filme);
		TARVB_Insere(T, ch, t, aux_filme);
	}
	fclose(fp);
}

TARVB* TARVB_Libera(TARVB* a) {
	if (a) {
		if (!a->folha) {
			int i;
			for (i = 0; i <= a->nchaves; i++) TARVB_Libera(a->filho[i]);
		}
		free(a->chave_primaria);
		free(a->filho);
		free(a);
		return NULL;
	}
}


void imp_rec(TARVB* a, int andar) {
	if (a) {
		int i, j;
		for (i = 0; i <= a->nchaves - 1; i++) {
			imp_rec(a->filho[i], andar + 1);
			for (j = 0; j <= andar; j++) printf("\t");
			printf("%d\n", a->chave_primaria[i]);
		}
		imp_rec(a->filho[i], andar + 1);
	}
}


void TARVB_Imprime(TARVB* a) {
	imp_rec(a, 0);
}

TARVB* TARVB_Cria(int t) {
	TARVB* novo = (TARVB*)malloc(sizeof(TARVB));
	novo->nchaves = 0;
	novo->chave_primaria = (int*)malloc(sizeof(int*) * ((t * 2) - 1));
	novo->folha = 1;
	novo->filho = (TARVB**)malloc(sizeof(TARVB*) * t * 2);
	novo->filme = (FILME*)malloc(sizeof(FILME));
	int i;
	for (i = 0; i < (t * 2); i++) novo->filho[i] = NULL;
	return novo;
}

TARVB* TARVB_Inicializa() {
	return NULL;
}

TARVB* TARVB_Busca(TARVB* x, int ch) {
	if (!x) return NULL;
	int i = 0;
	while (i < x->nchaves && ch > x->chave_primaria[i]) i++;
	if (i < x->nchaves && ch == x->chave_primaria[i]) return x;
	if (x->folha) return NULL;
	return TARVB_Busca(x->filho[i], ch);
}

void Busca_infosub(TARVB* a, int ch) {
	TARVB* no = TARVB_Busca(a, ch);
	FILME* f = no->filme;
	printf("nome do diretor: %s, genero: %s, duracao: %d", f->nome_diretor, f->genero, f->duracao);
}

int main(void) {
	TARVB* arvore = TARVB_Inicializa();
	int op;
	int t;
	char nome[100];
	int cmp;
	do {
		printf("1-Carregue o arquivo\n2-INSERE \n3-REMOVE\n4-BUSCA\n5-Display\n>>>>> ");
		scanf_s("%d", &op);
		switch (op) {
		case 1://criando a arvore
			printf("Digite o nome do arq> (com .txt)");
			scanf("%s", nome);
			cmp = strcmp(nome, "filmes.txt");
			if (cmp == 1) {
				printf("Arquivo nao encontrado!\n");
				break;
			}
			printf("Digite o tm t> ");
			scanf_s("%d", &t);
			ins_filme_arv(t, nome, arvore);
			break;
		case 2:
			if (!arvore) {
				printf("Digite outro valor!!!");
				break;
			}
			int elem;
			printf("Digite o num para remover: ");
			scanf_s("%d", &elem);
			break;
		case 3:
			if (!arvore) {
				printf("Digite outro valor!!!");
				break;
			}
			break;
		case 4:
			if (!arvore) {
				printf("Digite outro valor!!!");
				break;
			}
			/*FILME aux_busca;
			int ano;
			char nome_filme[81];
			printf("Digite os dados da chave primária:(ano/nome_filme");
			scanf_s("%d", &ano);
			scanf_s("%s", nome_filme);
			aux_busca.ano = ano;
			strcpy(aux_busca.nome_filme, nome_filme);
			int chave = cria_chaveprim(aux_busca);
			Busca_infosub(arvore, chave);
			*/break;
		case 5:
			if (!arvore) {
				printf("Digite outro valor!!!");
				break;
			}
			TARVB_Imprime(arvore);
		}

	} while (op != 6);
	return 0;
}
//valgrind --tool=memcheck --leak-check=yes ./<nome_executavel>