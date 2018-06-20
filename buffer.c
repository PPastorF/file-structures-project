// Projeto pratico (parte 2) da disciplina SCC0215012018 - Organizacao de Arquivos
// Pedro Pastorello Fernandes - NUSP 10262502

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "manipulacaoArquivos2.h"
#include "indice.h"
#include "buffer.h"

// Inicializa um buffer de paginas em memoria principal
void inicializaBuffer(bufferPaginas* B, FILE* arquivoIndice, int rrnRaiz) {

	inicializaFila(B);

}

/* Funcao que percorre as paginas armazenadas no buffer por uma chave C.
 * Caso encontre, retorna o RRN referente a ela. Caso nao encontre,
 * retorna -1 */
int encontraChaveBuffer(bufferPaginas* B, int C) {

	// Procura na pagina raiz
	int rrn = encontraChavePagina(&(B->raiz), C);
	if (rrn != -1)
		return rrn;
	else {
		// Procura na fila de paginas
		int i;
		for (i = 0; i < TAMANHOVETOR; i++) {

			rrn = encontraChavePagina(&(B->fila[i]), C);	
			if (rrn != -1)
				return rrn;
		}
		
		return -1;
	}

}

// Funcao de insercao no buffer. Caso esteja cheio, remove como uma fila
void insereBuffer(bufferPaginas* B, FILE* I, int RRN) {

	// Le a pagina P referente ao RRN especificado do arquivo de indice
	paginaArvore* P = lePagina(I, RRN);

	// Le o RRN do no raiz, no cabecalho do arquivo de indice
	int rrnRaiz = leRrnRaiz(I);

	// Caso o RRN seja referente a pagina raiz, o insere na posicao especifica para a pagina raiz
	if (RRN == rrnRaiz) {
		copiaPagina(B->raiz, P);
	}
	else {
		// Insere a pagina P na fila do buffer
		insereFila(B, P);
	}
}

// Inicializa um novo buffer ou esvazia um existente 
void inicializaFila(bufferPaginas* B) {
	B->primeiro = -1;
	B->ultimo = -1;
	B->qtd = 0;
}

// Insere uma pagina no buffer,
void insereFila(bufferPaginas* B, paginaArvore* P) {

	// Caso a fila esteja cheia
	if (B->qtd == TAMANHOVETOR) {
		retiraFila(B);
	}

	// Caso esteja vazia
	if (B->primeiro == -1) {
		B->primeiro = 0;
		B->ultimo = 0;
	}

	// Caso a fila precise 'dar a volta' no vetor
	if (B->ultimo == TAMANHOVETOR-1) {
		B->ultimo = 0;
	}
	else {
		B->ultimo++;
	}

	B->qtd++;
	copiaPagina(&B->fila[B->ultimo], P);

}

paginaArvore* retiraFila(bufferPaginas* B) {

	paginaArvore* aux;

	// Caso a fila esteja vazia
	if (B->qtd == 0) {
		return NULL;
	}
	// Caso nao esteja vazia
	else {
		// Caso a fila tenha apenas 1 indice (deve ser esvaziada)
		if (B->primeiro == B->ultimo) {
			aux = &(B->fila[B->primeiro]);
			inicializaFila(B);
		}
		else {
			// Caso a fila precise 'dar a volta' no fila
			if (B->primeiro == TAMANHOVETOR-1) {
				aux = &(B->fila[B->primeiro]);
				B->primeiro = 0;
			}
			else {
				aux = &(B->fila[B->primeiro]);
				B->primeiro++;
			}
		}

		return aux;
	}

}

/* Funcao que imprime as informacoes sobre a execucao do programa, referentes
 * ao buffer-pool, no arquivo especificado */
void appendBufferInfo(int pageFault, int pageHit) {

	// Abre o arquivo no modo append
	FILE* arq = fopen("buffer-info.txt", "a");
	
	// Escreve as informacoes sobre a execucao do programa
	fprintf(arq, "Page fault: %d; Page hit: %d.\n", pageFault, pageHit);

	fclose(arq);
}