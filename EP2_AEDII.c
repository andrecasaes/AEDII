//Andre Biondi Casaes - Numero USP 10391184
//Gabriel André Melo de Oliveira Silva - Numero USP 10724312

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX 6
#define branco 0
#define cinza 1
#define preto 2

typedef struct vert{
	int indice;
	struct vert *prox;
}VERTICE;

typedef struct NO{
	int nome;
	struct NO *prox;
	struct NO *ant;
}NO;

typedef struct {	
	VERTICE *adj[MAX+1];
	int cor[MAX+1];
	int pai[MAX+1];
	int ordem[MAX+1];
	int minor[MAX+1];
	int stNum[MAX+1];
	char sinal[MAX+1];
	int ordemGeral;
}GRAFO;

void inicializarGrafo(GRAFO *G){
	printf("Inicializando Grafo\n");
   for(int i = 1; i <= MAX; i++){
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
	for (int i = 1; i <= MAX; i++){
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
	for (int i = 1; i <= MAX; i++){
		printf("Vertice %i:\n Tem ordem %i e minor %i\n ",i, G->ordem[i], G->minor[i]);
		if (i != 1 && (G->ordem[i] == G->minor[i])){
			printf("\n****\nA Aresta %i - %i e critica! Ela desconecta o Grafo!\n****\n",G->pai[i],i);
			biconexo = false;
		}
		printf("------\n");
	}
	return biconexo;
}

NO* insereAntes(NO* inicio, int filho, int pai){
	if (!inicio){
		inicio = (NO*) malloc(sizeof(NO));
		inicio->nome= filho;
		inicio->prox = NULL;
		inicio->ant = NULL;
		return inicio;
	}
	NO* p = inicio;
	while(p){
		if(p->nome == pai){
			NO* novo = (NO*) malloc(sizeof(NO));
			novo->nome = filho;
			novo->ant = p->ant;
			if(p->ant) p->ant->prox = novo;
			p->ant = novo;
			novo->prox = p;
			if(p == inicio) inicio = novo; 
		}
		p = p->prox;
	}
	return inicio;
}

NO* insereDepois(NO* inicio, int filho, int pai){
	if (!inicio){
		inicio = (NO*) malloc(sizeof(NO));
		inicio->nome= filho;
		inicio->prox = NULL;
		inicio->ant = NULL;
		return inicio;
	}
	NO* p = inicio;
	while(p){
		if(p->nome == pai){
			NO* novo = (NO*) malloc(sizeof(NO));
			novo->nome = filho;
			novo->prox = p->prox;
			if(p->prox) p->prox->ant = novo;
			novo->ant = p;
			p->prox = novo;
		}
		p = p->prox;
	}
	return inicio;
}

void stnum(GRAFO* G){
	NO* inicio = NULL;
	if (!Busca(G)){
		printf("O grafo não é biconexo logo ele não tem st Numeração \n");
	}else{
		for(int i = 1; i <= MAX; i++){
			if(G->ordem[i]==1) G->sinal[i]='-';
			if(G->sinal[G->minor[i]]=='+'){
				inicio = insereAntes(inicio,i,G->pai[i]);
				G->sinal[G->pai[i]] = '-';
			}else if (G->sinal[G->minor[i]]=='-'){
				inicio = insereDepois(inicio,i,G->pai[i]);
				G->sinal[G->pai[i]] = '+';
			}
		}
		int i = 1;           
		for(int j = 1; j <= MAX; j++){
			NO* p = inicio;
			while(p){
				if (p->nome==G->ordem[j]){
					G->stNum[j] = i;
					printf("O vertice %i stNum %i\n",p->nome,i);
					break;
				}
				i=i+1;
				p = p->prox;
			}
			i=1;
		}
	}
}


int main() {
   GRAFO *G = (GRAFO*) malloc(sizeof(GRAFO)); 
   inicializarGrafo(G);
   //Vertice 1
   inserir(G,1,2);
   inserir(G,1,6);
   //Vertice 2
   inserir(G,2,1);
   inserir(G,2,3);
   inserir(G,2,6);
   //Vertice 3
   inserir(G,3,2);
   inserir(G,3,4);
   inserir(G,3,5);
   inserir(G,3,6);
   //Vertice 4
   inserir(G,4,3);
   inserir(G,4,5);
   //Vertice 5
   inserir(G,5,3);
   inserir(G,5,4);
   inserir(G,5,6);
   //Vertice 6
   inserir(G,6,1);
   inserir(G,6,2);
   inserir(G,6,3);
   inserir(G,6,5);
	stnum(G);
   return 0;
}