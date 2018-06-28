// Projeto pratico (parte 2) da disciplina SCC0215012018 - Organizacao de Arquivos
// Pedro Pastorello Fernandes - NUSP 10262502

#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "manipulacaoArquivos.h"
#include "indice.h"

// Inicializa um novo buffer ou esvazia um existente 
void inicializaFila(buffer* B) {
	B->primeiro = 0;
	B->ultimo = 0;
	B->qtd = 0;
}

// Inicializa um buffer de paginas em memoria principal
buffer* criaBuffer() {

	// Aloca a memoria para o buffer
	buffer* B = malloc( sizeof(buffer) );

	// Inicializa o vetor de RRN das paginas armazenadas pelo buffer
	int i;
	for (i = 0; i < 5; i++) {
		B->paginasArmazenadas[i] = -1;
	}

	// Inicializa a fila de paginas
	inicializaFila(B);

	// Inicializa os contadores de pagehit e pagefault
	B->pageFault = 0;
	B->pageHit   = 0;

	return B;
}

// Esvazia e limpa a memoria ocupada por um buffer
void limpaBuffer(buffer* B) {

	inicializaFila(B);
	free(B);

}

/* Funcao que retorna vedadeira se uma pagina de RRN rrnPag esta armazenada
 * no buffer e salva seu indice */
int paginaEstaNoBuffer(buffer* B, int rrnPag, int* indice) {
	
	int i;

	for (i = 0; i < 5; i++) {
		// Caso tenha encontrado
		if (rrnPag == B->paginasArmazenadas[i]) {			
			*indice = i;
			return 1;	
		}	
	}

	// Caso nao tenha encontrado
	*indice = -1;
	return 0;
}

// Funcao de insercao no buffer. Caso esteja cheio, remove como uma fila
void insereBuffer(buffer* B, FILE* I, int rrnPag) {
	
	paginaArvore* P = malloc(sizeof(paginaArvore) );
	int indiceInserido, indiceSeEstiverNoBuffer;

	// Le a pagina do arquivo
	fseek(I, (rrnPag*TAMANHOREGISTROINDICE)+13, SEEK_SET);
	
	fread( &(P->nChaves), 4, 1, I);
	int i;
	for (i = 0; i < ORDEMARVORE-1; i++) {
		fread( &(P->filhas[i]), 4, 1, I);
		fread( &(P->chaves[i].chave), 4, 1, I);
		fread( &(P->chaves[i].RRN), 4, 1, I);
	}
	fread( &(P->filhas[ORDEMARVORE-1]), 4, 1, I);

	// Caso a pagina lida seja a raiz, a insere em sua posicao especial
	int rrnRaiz = leCabecalhoArquivoIndice(I, "noRaiz");
	if (rrnPag == rrnRaiz) {

		B->paginasArmazenadas[4] = rrnPag;
		copiaPagina(&B->raiz, P);
	}
	else if ( paginaEstaNoBuffer(B, rrnPag, &indiceSeEstiverNoBuffer) ) {
		// Caso a pagina lida ja esteja no buffer, apenas a atualiza
		copiaPagina(&B->fila[indiceSeEstiverNoBuffer], P);
	}
	else {
		// Caso nao esteja, a insere no buffer
		indiceInserido = insereFila(B, P);
		B->paginasArmazenadas[indiceInserido] = rrnPag;
	}

}

// Insere uma pagina no buffer,
int insereFila(buffer* B, paginaArvore* P) {

	// Caso a fila esteja cheia, remove a ultima pagina
	if (B->qtd == 4) {
		retiraFila(B);
	}

	// Realiza a insercao
	B->qtd++;	
	copiaPagina(&B->fila[B->ultimo], P);
	int indiceInserido = B->ultimo;

	// Caso a fila precise 'dar a volta' no vetor
	if (B->ultimo == 3) {
		B->ultimo = 0;
	}
	else {
		B->ultimo++;
	}

	return indiceInserido;
}

// Retira uma pagina da fila de paginas do buffer
void retiraFila(buffer* B) {

	// Caso a fila esteja vazia
	if (B->qtd == 0) {
		return;
	}

	// Caso nao esteja vazia:
	B->qtd--;
	
	// Remove o RRN do vetor de paginas armazenadas
	B->paginasArmazenadas[B->primeiro] = -1;

	// Caso a fila precise 'dar a volta' no fila
	if (B->primeiro == 3)
		B->primeiro = 0;
	else
		B->primeiro++;

}

// Funcao que imprime as informacoes sobre a execucao do programa, referentes
// ao buffer-pool, no arquivo especificado
void bufferLogOutput(buffer* B) {

	// Abre o arquivo no modo append (ou o cria, caso necessario)
	FILE* arq = fopen("buffer-info.txt", "a+");
	
	// Escreve as informacoes sobre a execucao do programa
	fprintf(arq, "Page fault: %d; Page hit: %d.\n", B->pageFault, B->pageHit);
	
	fclose(arq);
}
