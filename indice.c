// Projeto pratico (parte 2) da disciplina SCC0215012018 - Organizacao de Arquivos
// Pedro Pastorello Fernandes - NUSP 10262502

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "manipulacaoArquivos.h"
#include "indice.h"

// Funcao que retorna o RRN da pagina mae de uma pagina de rrn rrnPag
int getMae(FILE* I, buffer* B, int rrnRaiz, int rrnPag, paginaArvore* P) {

	/* A funcao funciona da seguinte maneira: 
		rrnRaiz se refere ao rrn da pagina atual de execucao da funcao
		(comeca pela raiz) e rrnPag nao muda e se refere ao rrn da pagina
		a ser buscada. P e uma referencia a pagina buscada em RAM

		Toda */

	// Caso a funca tenha entrado em uma subarvore nula (significa que algum parametro passado e invalido)
	if (rrnRaiz == -1)
		return -1;

	paginaArvore* raiz = lePagina(I, B, rrnRaiz);

	// Busca no vetor de filhas da pagina atual pelo RRN da pagina buscada
	int i;
	for (i = 0; i < ORDEMARVORE; i++) {
		if (raiz->filhas[i] == rrnPag)
			return rrnRaiz;
	}

	/* Caso nao tenha encontrado na pagina atual, analisa pelo primeiro indice da 
	pagina buscada qual deve ser a proxima pagina a ser buscar */

	int indiceRecursao = posicaoInsercao(I, B, P, P->chaves[0].chave);

	return getMae(I, B, raiz->filhas[indiceRecursao], rrnPag, P);
}

// Funcao de criacao do arquivo de indices. Retorna uma referencia para o mesmo
void criaArquivoIndice(char* nomeArquivo) {

	// Cria o arquivo
	FILE* I = fopen(nomeArquivo, "wb");
	char status = 0;
	fwrite(&status, 1, 1, I);

	// Escreve o cabecalho do arquivo vazio
	escreveCabecalhoArquivoIndice(I, "noRaiz", -1);
	escreveCabecalhoArquivoIndice(I, "altura", -1);
	escreveCabecalhoArquivoIndice(I, "ultimoRRN", -1);

	// Procedimento de fechamento do arquivo
	fechaArquivoIndice(I);
}

// Descobre a quantidade de registros em um arquivo de indices F,
// Depois, volta para o a posicao do arquivo indicada pelo argumento RRN
int	numeroRegistrosIndice(FILE* I) {

	fseek(I, 0L, SEEK_END);
	int tamanhoArquivo = ftell(I);
	int qtdRegistros = ( (tamanhoArquivo-13) / TAMANHOREGISTROINDICE );
	
	return qtdRegistros;
}

// Funcao de escrita de uma pagina
void escrevePagina(FILE* I, paginaArvore* P, int rrnPag) {
	
	// Escreve a pagina do arquivo
	fwrite( &(P->nChaves), 4, 1, I);
	int i;
	for (i = 0; i < ORDEMARVORE-1; i++) {
		fwrite( &(P->filhas[i]), 4, 1, I);
		fwrite( &(P->chaves[i].chave), 4, 1, I);
		fwrite( &(P->chaves[i].RRN), 4, 1, I);
	}
	fwrite( &(P->chaves[ORDEMARVORE-1]), 4, 1, I);

	escreveCabecalhoArquivoIndice(I, "ultimoRRN", rrnPag);

}

// Funcao auxiliar que 'esvazia' uma pagina P armazenada em RAM
void esvaziaPagina(paginaArvore* P) {

	P->nChaves = 0;
	int i;
	for (i = 0; i < ORDEMARVORE-1; i++) {
		P->chaves[i].chave = 0;
		P->chaves[i].RRN = -1;
		P->filhas[i] = -1;
	}
	P->filhas[ORDEMARVORE-1] = -1;
}

// Funcao que retorna a posicao em que deve ser inserida uma chave em uma pagina
int posicaoInsercao(FILE* I, buffer* B, paginaArvore* P, int chaveInsert) {

	/*	Caso o laco estoure, a posicao continua valida, pois isso
		significa que a chave e maior que todos as outras chaves da pagina.

		Mesmo que a pagina esteja cheia,  a posicao ainda continua valida pois, 
		nesse caso, ela esta sendo chamada para travessia da arvore.
		Seu retorno representara o indice do vetor de paginas filhas a ser acessado e
		sempre ha 1 posicao nele a mais que no vetor de chaves */
	
	int i;
	for (i = 0; i < P->nChaves; i++) {
	
		if (chaveInsert <= P->chaves[i].chave) {
			break;
		}
	}

	return i;
}

// Funcao que insere uma chave em uma pagina que garantidamente nao esta cheia
void inserePaginaNaoCheia(FILE* I, buffer* B, int rrnPag, registroIndice insert, int filhaDirInsert) {

	// Le a pagina a ser trabalhada nesta execucao da funcao
	paginaArvore* P = lePagina(I, B, rrnPag);

	// Descobre a posicao da pagina na qual deve ser inserida a chave
	int posicaoInsere = posicaoInsercao(I, B, P, insert.chave);

	// Remaneja a filha a direita da ultima posicao
	P->filhas[P->nChaves+1] = P->filhas[P->nChaves];

	// Faz o remanejamento de todas as posicoes
	int i;
	for (i = P->nChaves; i > posicaoInsere; i--) {
		P->chaves[i] = P->chaves[i-1];
		P->filhas[i] = P->filhas[i-1];
	}
	// Insere a chave no indice correto e incrementa o numero de chaves da pagina
	P->chaves[posicaoInsere] = insert;
	P->nChaves++;
	
	/*	Se a pagina na qual estamos inserindo for uma pagina folha, filhaDirInsert sera -1,
		e todos os valores de paginas filhas serao -1.
	 
		Porem, existe o caso em que esta funcao esta sendo chamada apos o split e ha a promocao
		de uma chave. Neste caso, a pagina que contem as chaves menores que a chave inserida
		estara uma posicao a frente no vetor de filhas. 

		Entao, a pagina filha da posicao a frente da posicao inserida deve ser a pagina filha da
		posicao de insercao e a nova pagina filha da posicao a frente deve ser a pagina criada
		durante o split (a direita da chave inserida), que esta armazenada em filhaDirInsert. */

	P->filhas[posicaoInsere]   = P->filhas[posicaoInsere+1];
	P->filhas[posicaoInsere+1] = filhaDirInsert;

	// Escreve a pagina P atualizada no arquivo de indices e a atualiza no buffer
	fseek(I, (rrnPag*TAMANHOREGISTROINDICE)+13, SEEK_SET);
	escrevePagina(I, P, rrnPag);

	insereBuffer(B, I, rrnPag);
	
	free(P);
}


// Funcao de split de uma pagina cheia, ao ser inserida a chave insert
void split(FILE* I, buffer* B, int rrnPag, registroIndice insert, int filhaDirInsert) {

	if (rrnPag == -1)
		return;

	// Le a pagina a ser trabalhada nesta execucao da funcao
	paginaArvore* P = lePagina(I, B, rrnPag);

	// Caso a pagina nao esteja cheia, apenas insere a chave na pagina
	if ( !paginaCheia(P) ) {

		inserePaginaNaoCheia(I, B, rrnPag, insert, filhaDirInsert);
		free(P);
		return; 
	}

	/* Caso a pagina esteja cheia, realiza o split da mesma: */

	/* Armazena todas as chaves (+ a chave a ser inserida) da pagina em um vetor,
	 * que estara ordenado: */
	
	registroIndice chavesSplit[ORDEMARVORE];
	int filhasSplit[ORDEMARVORE+1];

	// Passa todas as chaves e ponteiros da pagina para seus espectivos vetores
	int i;
	for (i = 0; i < ORDEMARVORE-1; i++) {
		chavesSplit[i] = P->chaves[i];
		filhasSplit[i] = P->filhas[i];
	}
	filhasSplit[ORDEMARVORE-1] = P->filhas[ORDEMARVORE-1];

	// Descobre a posicao no vetor em que deve ficar a nova chave
	int posicaoInsere = posicaoInsercao(I, B, P, insert.chave);

	// Insere a chave e seus ponteios no vetor, remanejando as chaves e ponteiros a direita
	for (i = ORDEMARVORE-1; i > posicaoInsere; i--)
		chavesSplit[i] = chavesSplit[i-1];
	
	for (i = ORDEMARVORE; i > posicaoInsere; i--)
		filhasSplit[i] = filhasSplit[i-1];

	filhasSplit[posicaoInsere]   = filhasSplit[posicaoInsere-1];
	chavesSplit[posicaoInsere]   = insert;
	filhasSplit[posicaoInsere+1] = filhaDirInsert;

	// Armazena a chave do meio do vetor, que sera promovida para a pagina mae de P
	int meioVetor = ORDEMARVORE/2;
	registroIndice chavePromovida = chavesSplit[meioVetor];

	// 'Limpa' a pagina P e reinsere na mesma as chaves ate o meio do vetor
	esvaziaPagina(P);
	for (i = 0; i < meioVetor; i++) {
		P->chaves[i] = chavesSplit[i];
		P->filhas[i] = filhasSplit[i];
	}
	P->filhas[meioVetor] = filhasSplit[meioVetor];
	P->nChaves = meioVetor;

	// Cria uma nova pagina, a direita da pagina P
	paginaArvore* novaPag = malloc(sizeof(paginaArvore) );
	esvaziaPagina(novaPag);

	// Insere nela as chaves que estao apos o meio do vetor
	for (i = meioVetor+1; i < ORDEMARVORE ; i++) {
		novaPag->chaves[i-meioVetor-1] = chavesSplit[i];
		novaPag->filhas[i-meioVetor-1] = filhasSplit[i];
	}
	novaPag->filhas[4] = filhasSplit[ORDEMARVORE];
	novaPag->nChaves = meioVetor-1;

	// Descobre o RRN da nova pagina criada
	int rrnNovaPag = numeroRegistrosIndice(I);

	// Escreve a pagina P atualizada e a nova pagina no arquivo de indices e insere seus valores no buffer
	fseek(I, (rrnPag*TAMANHOREGISTROINDICE)+13, SEEK_SET);
	escrevePagina(I, P, rrnPag);
	insereBuffer(B, I, rrnPag);

	fseek(I, 0L, SEEK_END);
	escrevePagina(I, novaPag, rrnNovaPag);
	insereBuffer(B, I, rrnNovaPag);

	// Caso a pagina atual seja a pagina raiz, cria a nova pagina raiz da arvore
	if (rrnPag == leCabecalhoArquivoIndice(I, "noRaiz") ) {
		
		// Criacao da nova raiz
		paginaArvore* novaRaiz = malloc(sizeof(paginaArvore) );
		esvaziaPagina(novaRaiz);
		novaRaiz->filhas[0] = rrnPag;
		novaRaiz->chaves[0] = chavePromovida; 
		novaRaiz->filhas[1] = rrnNovaPag;
		novaRaiz->nChaves = 1;

		// Descobre o RRN da nova raiz
		int rrnNovaRaiz = numeroRegistrosIndice(I);

		// Escreve a nova raiz
		fseek(I, 0L, SEEK_END);
		escrevePagina(I, novaRaiz, rrnNovaRaiz);

		// A arvore cresce em altura
		int novaAltura = leCabecalhoArquivoIndice(I, "altura") + 1;

		// Atualiza os valores do cabecalho
		escreveCabecalhoArquivoIndice(I, "noRaiz", rrnNovaRaiz);
		escreveCabecalhoArquivoIndice(I, "altura", novaAltura);

		insereBuffer(B, I, rrnNovaRaiz);
		free(novaRaiz);
	}
	else {
		// Caso a pagina nao seja a raiz, continua a cadeia de splits para cima
		split(I, B, getMae(I, B, leCabecalhoArquivoIndice(I, "noRaiz"), rrnPag, P), chavePromovida, rrnNovaPag);
	}

	free(P);
	free(novaPag);
}

// Insere a chave insert numa pagina armazenada no rrn
void insereChavePagina(FILE* I, buffer* B, int rrnPag, registroIndice insert) {
			
	// Le a pagina na qual a chave esta sendo inserida para RAM
	paginaArvore* P = lePagina(I, B, rrnPag);

	// Caso a pagina seja uma folha:
	if ( paginaFolha(P) ) {

		// Caso a pagina esteja cheia, comeca a cadeia de splits
		if ( paginaCheia(P) ) {
		
			/* 	A funcao de splits 'sobe' a arvore recursivamente, dividindo paginas
				ate encontrar uma pagina que nao esteja cheia ou ate criar uma nova raiz.
				O ultimo parametro da funcao so e usado a partir do segundo split da cadeia */
			split(I, B, rrnPag, insert, -1);

		}
		else {
			/* 	Caso a pagina nao esteja cheia, apenas insere a chave.
				O ultimo parametro da funcao se refere ao RRN da pagina filha a direita
				da chave inserida (quando houve promocao da mesma).
				Como a pagina atual e uma folha, este valor e -1 */
			inserePaginaNaoCheia(I, B, rrnPag, insert, -1);
		}

	}
	else {
		// Caso nao seja uma folha, continua a busca pela pagina folha correta para realizar a insercao
		int rrnFilha = P->filhas[posicaoInsercao(I, B, P, insert.chave)];
		insereChavePagina(I, B, rrnFilha, insert);
	}

	free(P);
}

// Insere uma chave insert num arquivo de indices P
void insereChaveIndice(FILE* I, buffer* B, int chave, int rrn) {

	// Novo registro a ser inserido
	registroIndice insert;
	insert.chave = chave;
	insert.RRN = rrn;

	int pagRaiz = leCabecalhoArquivoIndice(I, "noRaiz");
	
	// Caso a pagina a ser inserida seja a primeira
	if (pagRaiz == -1) {

		// Cria a pagina raiz e insere a chave nela
		paginaArvore* primeiraRaiz = malloc( sizeof(paginaArvore) );
		esvaziaPagina(primeiraRaiz);
		primeiraRaiz->chaves[0] = insert;
		primeiraRaiz->nChaves = 1;

		// Escreve a nova raiz
		fseek(I, 0L, SEEK_END);
		escrevePagina(I, primeiraRaiz, 0);

		// A altura da arvore se torna 0 (a raiz tambem e uma folha e altura de folhas = 0)

		// Atualiza os valores do cabecalho
		escreveCabecalhoArquivoIndice(I, "noRaiz", 0);
		escreveCabecalhoArquivoIndice(I, "altura", 0);

		// Insere a pagiana no buffer		
		insereBuffer(B, I, 0);

		free(primeiraRaiz);

	}
	else {
		/* Insere a chave na pagina raiz.
		 * A propria funcao encontra a posicao da chave e trata todas as operacoes necessarias
		 * para a manutencao correta da arvore e do arquivo de indices */
		insereChavePagina(I, B, pagRaiz, insert);
	}
}

/* Funcao que percorre uma pagina de indice por uma chave C.
 * Caso encontre, retorna o rrn referente a ela. Caso nao encontre,
 * retorna -1 */
int encontraChavePagina(paginaArvore* P, int C) {

	// Percorre todos os indices de chave da pagina P
	int i;
	for (i = 0; i < P->nChaves; i++) {
		// Retorna o rrn referente a chave, caso encontre
		if ( P->chaves[i].chave == C )
			return P->chaves[i].RRN;
	}

	// Caso nao encontre, retorna -1
	return -1;
}

// Le um campo do caPalho de um arquivo de indice
int leCabecalhoArquivoIndice(FILE* I, char* campo) {

	int aux;
	// Descobre o campo que sera lido e o le
	if (strcmp(campo, "noRaiz") == 0) {
	
		fseek(I, 1, SEEK_SET);
		fread(&aux, 4, 1, I);
	}
	else if (strcmp(campo, "altura") == 0) {
		
		fseek(I, 5, SEEK_SET);
		fread(&aux, 4, 1, I);
	}
	else if (strcmp(campo, "ultimoRRN") == 0) {
		
		fseek(I, 9, SEEK_SET);
		fread(&aux, 4, 1, I);
	}

	return aux;
}

// Atualiza um campo especifico do cabecalho do arquivo de indice.
// Considera que o mesmo ja esta aberto em RAM e sera fechado depois.
void escreveCabecalhoArquivoIndice(FILE* I, char* campo, int arg) {

	// Descobre o campo que sera atualizado e o atualiza
	if (strcmp(campo, "noRaiz") == 0) {
		
		fseek(I, 1, SEEK_SET);
		fwrite(&arg, 4, 1, I);
	}
	else if (strcmp(campo, "altura") == 0) {
		
		fseek(I, 5, SEEK_SET);
		fwrite(&arg, 4, 1, I);
	}
	else if (strcmp(campo, "ultimoRRN") == 0) {
		
		fseek(I, 9, SEEK_SET);
		fwrite(&arg, 4, 1, I);
	}
}

// Procedimento de abertura do arquivo de indice para escrita e leitura
FILE* abreArquivoIndice() {

	FILE* I = fopen("fileIndex", "rb+");

	// Tratamento do cabecalho
	char status = 0;
	fseek(I, 0, SEEK_SET);
	fwrite(&status, 1, 1, I);

	return I;
}

// Procedimento de fechamento do arquivo de indice
void fechaArquivoIndice(FILE* I) {

	// Tratamento do cabecalho
	char status = 1;
	fseek(I, 0, SEEK_SET);
	fwrite(&status, 1, 1, I);

	// Fecha o arquivo
	fclose(I);
}

// Funcao que le e retorna uma pagina de arvore de um arquivo de indice I, a partir de seu rrn
paginaArvore* lePagina(FILE* I, buffer* B, int rrn) {

	// Pagina que sera retornada
	paginaArvore* P = malloc(sizeof(paginaArvore));

	// Variavel utilizada para resgatar a pagina do buffer
	int indiceNoBuffer;
	int i;
	
	// Checa se a pagina a ser lida ja esta armazenada no buffer
	if ( paginaEstaNoBuffer(B, rrn, &indiceNoBuffer) ) {
		
		B->pageHit++;

		// Caso o indice seja 4, significa que a pagina e a raiz.
		// O quinto indice e especial e reservado apenas para o RRN da pagina raiz.
		// Ele nunca e acessado por operacoes de insercao que nao se referem a raiz.
		if (indiceNoBuffer == 4) {
			// Neste caso, le a pagina raiz de sua posicao especial
			copiaPagina(P, &(B->raiz) );
		}
		else {
			// Le a pagina do buffer
			copiaPagina(P, &(B->fila[indiceNoBuffer]) );
		}
	}
	else {
		// Caso nao esteja, a insere no buffer
		B->pageFault++;

		// Encontra a posicao da pagina a ser lida
		fseek(I, (rrn*TAMANHOREGISTROINDICE)+13, SEEK_SET);
		
		// Le a pagina do arquivo
		fread( &(P->nChaves), 4, 1, I);
		for (i = 0; i < ORDEMARVORE-1; i++) {
			fread( &(P->filhas[i]), 4, 1, I);
			fread( &(P->chaves[i].chave), 4, 1, I);
			fread( &(P->chaves[i].RRN), 4, 1, I);
		}
		fread( &(P->filhas[ORDEMARVORE-1]), 4, 1, I);

		// Insere a pagina no buffer
		insereBuffer(B, I, rrn);
	}

	/* Retorna a pagina lida, seja ela proveniente do buffer ou lida
	 * lida do arquivo de indice */
	return P;
}

// Funcao que copia os conteudos da pagina src para a pagina dest
void copiaPagina(paginaArvore* dest, paginaArvore* src) {

	// Copia o numero de chaves
	dest->nChaves = src->nChaves;

	int i;
	// Copia os valores de chaves e ponteiros para filhas
	for (i = 0; i < ORDEMARVORE-1; i++) {
		dest->chaves[i] = src->chaves[i];
		dest->filhas[i] = src->filhas[i];
	}
	dest->filhas[ORDEMARVORE-1] = src->filhas[ORDEMARVORE-1];
}

// Funcao que identifica se uma pagina P esta cheia
int paginaCheia(paginaArvore* P) {

	return (P->nChaves == ORDEMARVORE-1);
}

// Funcao que identifica se uma pagina P e uma folha
int paginaFolha(paginaArvore* P) {

	return (P->filhas[0] == -1);
}

// Funcao de busca no arquivo de indices arvore B por uma chave
int buscaIndice(FILE* I, buffer* B, int rrnPag, int chaveBusca) {

	if (rrnPag == -1)
		return -1;

	// Le a pagina a ser trabalhada nessa execucao da funcao
	paginaArvore* P = malloc( sizeof(paginaArvore) );
	P = lePagina(I, B, rrnPag);

	// Tenta encontrar a chave na pagina atual
	int encontraPagina = encontraChavePagina(P, chaveBusca);

	// Caso seja o valor invalido (-1), continua buscando em uma pagina filha de P 
	if (encontraPagina == -1) {
		int filhaBusca = posicaoInsercao(I, B, P, chaveBusca);
		return buscaIndice(I, B, P->filhas[filhaBusca], chaveBusca );
	}
	else {
		return encontraPagina;
	}

}
