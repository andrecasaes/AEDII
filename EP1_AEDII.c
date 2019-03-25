#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX 12
#define branco 0
#define cinza 1
#define preto 2

typedef struct vert{
	int indice;
	struct vert *prox;
}VERTICE;

typedef struct {	
	VERTICE *adj[MAX];
	int cor[MAX];
	int pai[MAX];
	int ordem[MAX];
	int minor[MAX];
	int ordemGeral;
}GRAFO;

void inicializar(GRAFO *G){
   for(int i = MAX; i > 0; i--){
      G->adj[i] = NULL;
      G->cor[i] = branco;
      G->pai[i] = 0;
      G->ordem[i] = -1;
      G->minor[i] = MAX;
   }
   G->ordemGeral = 0;
}

void inserir(GRAFO *G, int indice, int vertice){
	VERTICE* aux = G->adj[indice];
	while(aux && aux->prox) aux = aux->prox;
	VERTICE* novo = (VERTICE*) malloc(sizeof(VERTICE));
	novo->indice = vertice;
	novo->prox = NULL;
	if (aux) aux->prox = novo;
	else G->adj[indice] = novo;
}

void imprimir(GRAFO *G){
	VERTICE* aux;
	for (int i = 1; i <= MAX; ++i){
		aux = G->adj[i];
		printf("Adjacentes de %i: ",i);
		while(aux) {
			printf("%i ", aux->indice);
			aux = aux->prox;
		}
		printf("\n");
	}
} 

void DFS1(GRAFO *G, int y){
	VERTICE *u = G->adj[y];
	while(u){
		if (G->cor[u->indice] == branco){
			G->cor[u->indice] = cinza;
			G->ordemGeral++;
			G->ordem[u->indice] = G->ordemGeral;
			G->minor[u->indice] = G->ordemGeral;
			G->pai[u->indice] = y;
			DFS1(G,u->indice);
		}
		u = u->prox;
	}
}

void DFS2(GRAFO *G,int z){
	VERTICE *u = G->adj[z];
	while(u){
		if (G->pai[u->indice] == z) DFS2(G,u->indice);
		u = u->prox;
	}
	u = G->adj[z];
	while(u){
		if (u->indice != G->pai[z] && (G->minor[u->indice] < G->minor[z])){
			G->minor[z] = G->minor[u->indice];
		}
		u = u->prox;
	}
}

bool Busca(GRAFO *G){
	bool biconexo = false;

	G->cor[1] = cinza;
	G->ordemGeral++;
	G->ordem[1] = G->ordemGeral;
	G->minor[1] = G->ordemGeral;
	DFS1(G,1);
	
	if (G->ordemGeral == MAX){
		G->minor[1] = G->ordem[1];
		DFS2(G,1);
	}
	biconexo = true;
	for (int i = 1; i <= MAX; ++i){
		printf("Vertice %i:\n Tem ordem %i e minor %i\n ",i, G->ordem[i], G->minor[i]);
		if (i != 1 && (G->ordem[i] == G->minor[i])){
			printf("Esse vertice e critico \n");
			biconexo =false;
		}
		printf("------\n");
	}
	return biconexo;
}

int main() {
   GRAFO *G = (GRAFO*) malloc(sizeof(GRAFO)); 
   inicializar(G);
   //Vertice 1
   inserir(G,1,2);
   inserir(G,1,3);
   //Vertice 2
   inserir(G,2,1);
   inserir(G,2,4);
   inserir(G,2,5);
   //Vertice 3
   inserir(G,3,1);
   inserir(G,3,5);
   //Vertice 4
   inserir(G,4,2);
   inserir(G,4,6);
   inserir(G,4,7);
   //Vertice 5
   inserir(G,5,2);
   inserir(G,5,3);
   inserir(G,5,6);
   //Vertice 6
   inserir(G,6,4);
   inserir(G,6,5);
   //Vertice 7
   inserir(G,7,4);
   inserir(G,7,8);
   inserir(G,7,9);
   inserir(G,7,12);
   //Vertice 8
   inserir(G,8,7);
   inserir(G,8,9);
   //Vertice 9
   inserir(G,9,7);
   inserir(G,9,10);
   //Vertice 10
   inserir(G,10,9);
   inserir(G,10,11);
   inserir(G,10,12);
   //Vertice 11
   inserir(G,11,9);
   inserir(G,11,10);
   //Vertice 12
   inserir(G,12,7);
   inserir(G,12,10);
   // imprimir(G);

   if (Busca(G)) printf("**O Grafo e Biconexo**\n");
   else printf("**O grafo nao e Biconexo**\n");
   return 0;
}