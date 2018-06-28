// Projeto pratico (parte 2) da disciplina SCC0215012018 - Organizacao de Arquivos
// Pedro Pastorello Fernandes - NUSP 10262502

#ifndef _INDICE_H_
#define _INDICE_H_

/* Definicoes relacionadas a arquivos de indice e suas operacoes
 * Utilizando a estrutura de dados de Arvore B. */

#define ORDEMARVORE 10
#define TAMANHOREGISTROINDICE 116

// Representacao de um registro de arquivo de indices em memoria principal
typedef struct registroindice {

	int	chave;
	int	RRN;

} registroIndice;

	
// Representacao de ma pagina de Arvore B em memoria principal
typedef struct paginaarvore {

	int 			nChaves; 				  	// Numero n de chaves da pagina
	registroIndice  chaves[ORDEMARVORE-1]; 	 	// Vetor com as chaves armazenadas pela pagina
	int				filhas[ORDEMARVORE];	 	// Vetor com os RRNs das paginas filhas (implementacao em memoria secundaria)

} paginaArvore;

// Metodos:


/* Definicoes de um buffer de paginas da Arvore B
 *
 * O buffer funciona como uma fila, de acordo com a politica de organizacao FIFO.
 * Esta fila esta organizada em um vetor estatico e a mesma rotaciona por seus indices,
 * para economizar espaco de memoria e nao necessitar de realocacao de seus valores.
 */

typedef struct bufferpaginas {

	/* Vetor com os RRNs das paginas armazenadas no buffer.
	 * As posicoes de 1 a 4 se referem a fila, enquanto a posicao
	 * 5 e especial e reservada para o RRN da pagina raiz */
	int paginasArmazenadas[5];
	
	// Localizacao especial para a pagina raiz
	paginaArvore raiz;

	// Fila de paginas armazenadas no buffer
	paginaArvore fila[4];
	int primeiro;
	int ultimo;
	int qtd;

	int pageFault;
	int pageHit;

} buffer;

// Metodos

// Indice:
int 			getMae(FILE*, buffer*, int, int, paginaArvore*);
void 			criaArquivoIndice(char*);
int			numeroRegistrosIndice(FILE*);
void 			escrevePagina(FILE*, paginaArvore*, int);
void 			esvaziaPagina(paginaArvore*);
int 			posicaoInsercao(FILE*, buffer*, paginaArvore*, int);
void 			inserePaginaNaoCheia(FILE*, buffer*, int, registroIndice, int);
void 			split(FILE*, buffer*, int, registroIndice, int);
void 			insereChavePagina(FILE*, buffer*, int, registroIndice);
void			insereChaveIndice(FILE*, buffer*, int, int);
int 			encontraChavePagina(paginaArvore*, int);
int 			leCabecalhoArquivoIndice(FILE*, char*);
void			escreveCabecalhoArquivoIndice(FILE*, char*, int);
FILE* 			abreArquivoIndice();
void 			fechaArquivoIndice(FILE*);
paginaArvore* 		lePagina(FILE*, buffer*, int);
void 			copiaPagina(paginaArvore*, paginaArvore*);
int 			paginaCheia(paginaArvore*);
int 			paginaFolha(paginaArvore*);
int 			buscaIndice(FILE*, buffer*, int, int);


// Buffer:
void 	inicializaFila(buffer*);
buffer*	criaBuffer();
void 	limpaBuffer(buffer*);
int 	paginaEstaNoBuffer(buffer*, int, int*);
void 	insereBuffer(buffer*, FILE*, int);
int 	insereFila(buffer*, paginaArvore*);
void 	retiraFila(buffer*);
void 	bufferLogOutput(buffer*);

#endif
