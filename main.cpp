/*
 * Main.cpp
 *
 *  Created on: 01/04/2015
 *      Author: eliton_perin
 */

#include<stdio.h>
#include<stdlib.h>

#define MAX_N 100
#define MIN_N 0
#define MAX_M MAX_N*(MAX_N-1)/2
#define MIN_M 1
#define MAX_XYZ 100
#define MIN_XYZ 1

typedef struct arestas{
	int nome1;
	int nome2;
	int dist;
	int conj;
	struct arestas *prox;
} aresta;


typedef struct verte{
	int nome;
	int conj;
	struct verte *prox;
} vertice;


/* 	Essa Função adiciona um vertice a uma lista dada.
*	Ela garante a não repetição de vertices. Recebe também
*	um inteiro para associar o vertice a um conjunto.
*	Se tentarem adicionar um vertice já existente na lista ele
*	retorna 0, caso tenha adicionado um novo retorna 1.
*	Esse retorno foi aproveitado na associação dos conjuntos aos vertices.
*/
int addVertice(vertice *raiz, int str, int conj){
	vertice *p, *q;
	p = raiz;
	q = raiz->prox;
	while(q != NULL && q->nome != str){
		p = p->prox;
		q = q->prox;
	}
	if(q == NULL)
	{
		q = (vertice*)malloc(sizeof(vertice));
		q->nome = str;
		q->conj = conj;
		p->prox = q;
		q->prox = NULL;
		return 1;
	}
	return 0;
}

/*  Essa função adiciona uma aresta a uma lista de arestas dada.
*	Como na entrada não consideramos a entrada de uma mesma aresta com dois pesos,
*	essa função aceita que duas arestas com mesmos elementos seja adicionada.
*/
void addAresta(aresta *raiz, int str, int str2, float dist){
	aresta *p, *q;
	q = raiz;
	p = raiz->prox;
	while(p != NULL){
		q = q->prox;
		p = p->prox;
	}
	p = (aresta*)malloc(sizeof(aresta));
	p->nome1 = str;
	p->nome2 = str2;
	p->dist = dist;
	q->prox = p;
	p->prox = NULL;
}


/*	Essa função busca um vértice em uma lista de vértices dada.
*	Retorna o endereço se ela for encontrada, caso contrário retorna NULL.
*/
vertice *buscaVertice(vertice *cabeca, int str)
{
  vertice *resultado = NULL;
  vertice *aux = cabeca->prox;

  while(aux != NULL)
  {
    if(aux->nome == str)
      resultado = aux;
    aux = aux->prox;
  }

  return resultado;
}


/* 	Essa função troca o conjunto antigo atribuído a um conjunto de vértices de uma
*	lista de vértices para um novo conjunto dado.
*/
void mudaConj( vertice *raiz, int antconj, int novoconj ){
	vertice *p;
	p = raiz;
	while(p != NULL){
		if(p->conj == antconj)
			p->conj = novoconj;
		p = p->prox;
	}
}

/* 	Essa função recebe uma lista de vértices e deleta todos seus elementos.
*	A função faz isso de forma recursiva.
*/
void deletaVertices( vertice *raiz ){
	vertice *p = raiz;

	if(p->prox == NULL)
		free(p);
	else
		deletaVertices( p->prox );
}

/* 	Essa função recebe uma lista de arestas e deleta todos seus elementos.
*	A função faz isso de forma recursiva.
*/
void deletaArestas( aresta *raiz ){
	aresta *p = raiz;

	if(p->prox == NULL)
		free(p);
	else
		deletaArestas(p->prox);
}


/*	Algoritmo de Kruskal, que recebe o conjunto de arestas já ordenadas e
*	também o conjunto de vértices com seus respectivos conjuntos.
*	Cria uma lista para a árvore geradora mínima. Retorna o "valor" dessa
*	árvore mínima gerada.
*/
aresta *arvoreGeradoraMinima( aresta *cabeca, vertice *raiz){
	float Soma = 0.0;
	aresta *aux;
	aresta *AGM = (aresta*)malloc(sizeof(aresta));
	vertice *temp1, *temp2;

	AGM->prox = NULL;
	for(aux = cabeca->prox; aux != NULL; aux = aux->prox){

		temp1 = buscaVertice(raiz, aux->nome1);
		temp2 = buscaVertice(raiz, aux->nome2);
		if( temp1->conj != temp2->conj ){
			addAresta(AGM, aux->nome1, aux->nome2, aux->dist);
			mudaConj(raiz, temp2->conj, temp1->conj);
		}
	}
	for(aux = AGM->prox; aux != NULL; aux = aux->prox)
	{
		Soma = Soma + aux->dist;
	}
	// deletaArestas( AGM );
	return AGM;

}

/*	Essa função ordena uma lista de arestas pelo seu peso,
*	no caso a distância, de forma crescente.
*	Usa o método selectionSort de ordenação.
*/
void selectionSortListaOrdem(aresta *cabeca){
	aresta *aux, *menor, *aux1, *temp = NULL;

	for( aux = cabeca; aux->prox != NULL; aux = aux->prox)
	{
		menor = aux->prox;
		for( aux1 = aux->prox; aux1->prox != NULL; aux1 = aux1->prox)
		{
			if(aux1->prox->nome1 < menor->nome1){
				temp = aux1;
				menor = aux1->prox;
			}
			else if(aux1->prox->nome1 == menor->nome1){
				if(aux1->prox->nome2 < menor->nome2){
					temp = aux1;
					menor = aux1->prox;
				}
			}
		}
		if(menor != aux->prox)
		{
			temp->prox = menor->prox;
			temp = aux;
			menor->prox = temp->prox;
			temp->prox = menor;
		}
	}
}

/*	Essa função ordena uma lista de arestas pelo seu peso,
*	no caso a distância, de forma crescente.
*	Usa o método selectionSort de ordenação.
*/
void selectionSortListaDist(aresta *cabeca){
	aresta *aux, *menor, *aux1, *temp = NULL;

	for( aux = cabeca; aux->prox != NULL; aux = aux->prox)
	{
		menor = aux->prox;
		for( aux1 = aux->prox; aux1->prox != NULL; aux1 = aux1->prox)
		{
			if(aux1->prox->dist < menor->dist)
			{
				temp = aux1;
				menor = aux1->prox;
			}else if(aux1->prox->dist == menor->dist){
				if(aux1->prox->nome1 < menor->nome1){
					temp = aux1;
					menor = aux1->prox;
				}
				else if(aux1->prox->nome1 == menor->nome1){
					if(aux1->prox->nome2 < menor->nome2){
						temp = aux1;
						menor = aux1->prox;
					}
				}
			}
		}
		if(menor != aux->prox)
		{
			temp->prox = menor->prox;
			temp = aux;
			menor->prox = temp->prox;
			temp->prox = menor;
		}
	}
}

void printAresta(aresta *cabeca, bool dist){
	aresta *aux;
	if(dist)
		for( aux = cabeca->prox; aux != NULL; aux = aux->prox)
		{
			printf("%d %d %d\n", aux->nome1, aux->nome2, aux->dist);
		}
	else
		for( aux = cabeca->prox; aux != NULL; aux = aux->prox)
		{
			printf("%d %d\n", aux->nome1, aux->nome2);
		}
}


int main(){

	aresta *at;
	vertice *vt;
	int a, n, cj, m;
	int dist, nome1, nome2;
	int teste = 0;

	do{
		teste++;
		at = (aresta*)malloc(sizeof(aresta));
		at->prox = NULL;
		vt = (vertice*)malloc(sizeof(vertice));
		vt->prox = NULL;
		scanf("%d %d", &n, &m);
		if(n < 1)
			return 0;
		a = m;
		cj = 1;
		while(a > 0 && n <= 100)
		{
			scanf("%d %d %d", &nome1, &nome2, &dist);
	// 		   Adicionando uma aresta por vez na lista de arestas.
			if(nome1 < nome2)
				addAresta( at, nome1, nome2, dist);
			else
				addAresta( at, nome2, nome1, dist);
	// 		   Adicionando dois vertices por vez na lista de vertices
	//		   dentro da lista, na função ele evita que se repita vertices.
	//		   Esse cj é o conjunto ao qual ele vai pertencer, e em cada ida a função
	//		   ele retorna se o vertice já existe na lista com 0 e 1 se foi adicionado.
	//		   Aqui estou inicializando cada vertice com um conjunto distinto.
	//		   Portante ele manda um conjunto válido pra próxima função, se não for
	//		   adicionado para o primeiro que retorna zero ele manda para o próximo
	//		   vértice que não estiver adicionado.
			cj = cj + addVertice( vt, nome1, cj);
			cj = cj + addVertice( vt, nome2, cj);
			a--;
		}
	//		   Ordenamos as arestas em ordem crescente antes de passar para a função
	//		   árvore geradora mínima.

		selectionSortListaDist( at );

		//printAresta(at);
		printf("Teste %d\n", teste);
		at = arvoreGeradoraMinima(at, vt);
		selectionSortListaOrdem(at);
		printAresta(at, false);
		printf("\n");

		deletaArestas( at );
		deletaVertices( vt );
	}while(1);
/*
	int N, M;
	int matriz[MAX_N][MAX_N];
	int new_matriz[MAX_N][MAX_N];
	int testes = 0;
	Aresta ares[MAX_M];

	do{
		scanf("%d %d", &N, &M);

		if(N == 0)
			return 0;

		int X, Y, Z;

		for(int i = 0; i < N; i++)
			for(int j = 0; j < N; j++){
				matriz[i][j] = 0;
				new_matriz[i][j] = 0;
			}

		for(int i = 0; i < M; i++){
			scanf("%d %d %d", &X, &Y, &Z);
			ares[i].conj = i+1;
			ares[i].nome1 = X;
			ares[i].nome2 = Y;
			ares[i].dist = Z;
			matriz[X-1][Y-1] = Z;
			matriz[Y-1][X-1] = Z;
		}

		for(int i = 0; i < N; i++){
			for(int j = 0; j < N; j++)
				printf("%d ", matriz[i][j]);
			printf("\n");
		}

		printf("Teste %d\n", testes);

	}while(1);
	*/


	return 0;
}

