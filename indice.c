// Projeto pratico (parte 2) da disciplina SCC0215012018 - Organizacao de Arquivos
// Pedro Pastorello Fernandes - NUSP 10262502

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "manipulacaoArquivos2.h"
#include "indice.h"
#include "buffer.h"


/* Funcao que percorre uma pagina de indice por uma chave C.
 * Caso encontre, retorna o RRN referente a ela. Caso nao encontre,
 * retorna -1 */
int encontraChavePagina(paginaArvore* P, int C) {

	// Percorre todos os indices de 'registros[]'
	int i;
	for (i = 0; i < P->nChaves; i++) {
		// Retorna o rrn referente a chave, caso encontre
		if ( P->registros[i].chave == C )
			return P->registros[i].RRN
	}

	// Caso nao encontre, retorna -1
	return -1;
}

/* Metodos auxiliares */

// Procedimento de fechamento do arquivo de indice
void fechaArquivoIndice(FILE* I) {

	// Tratamento do cabecalho
	char status = 1;
	fseek(I, 0, SEEK_SET);
	fwrite(&status, 1, 1, I);

	// Fecha o arquivo
	fclose(I);
}

// Funcao que le o rrn da pagina raiz do arquivo de indice, a partir de seu cabecalho
int leRrnRaiz(FILE* I) {

	I = fopen("fileIndex", "rb");
	fseek(I, 1, SEEK_SET);
	int rrnRaiz;
	fread(&rrnRaiz, 4, 1, I);

	return rrnRaiz;	
}

// Funcao que le e retorna uma pagina de arvore de um arquivo de indice I, a partir de seu RRN
paginaArvore* lePagina(FILE* I, int rrn) {

	// Abre o arquivo de indice I
	I = fopen("fileIndex", "rb");
	
	// Encontra a posicao da pagina a ser inserida no buffer
	fseek(I, (rrn*TAMANHOREGISTROINDICE)+13, SEEK_SET);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 

	// Pagina que sera retornada
	paginaArvore* aux = malloc(sizeof(paginaArvore));

	// Le a pagina do arquivo
	fread( &(aux.nChaves), 4, 1, I);
	int i;
	for (i = 0; i < ORDEMARVORE-1; i++) {
		fread( &(aux.nChaves), 4, 1, I);
		fread( &(aux.registros[i]), 4, 1, I);
		fread( &(aux.filhas[i]), 4, 1, I);
	}
	fread( &(aux.registros[9]), 4, 1, I);

	// Termino da execucao da funcao
	fclose(I);
	return aux;
}

// Funcao que copia os conteudos da pagina src para a pagina dest
void copiaPagina(paginaArvore* dest, paginaArvore* src) {

	// Copia o numero de chaves
	dest->nChaves = src->nChaves;

	int i;
	// Copia os valores de registros
	for (i = 0; i < ORDEMARVORE-1; i++) {
		dest->registros[i].chave = src->registros[i].chave;
		dest->registros[i].RRN   = src->registros[i].RRN;
	}

	// Copia os ponteiros para filhas
	for (i = 0; i < ORDEMARVORE; i++) {
		dest->filhas[i] = src->filhas[i];
	}
}