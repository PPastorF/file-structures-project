// Projeto pratico (parte 2) da disciplina SCC0215012018 - Organizacao de Arquivos
// Pedro Pastorello Fernandes - NUSP 10262502

#ifndef _BUFFER_H_
#define _BUFFER_H_

// Definicoes de um buffer de paginas da Arvore B

/* Como o buffer sempre armazenara a pagina raiz, o tamanho do vetor de armazenamento devera ser
   menor que o tamanho total do buffer em 1 unidade */
#define TAMANHOBUFFER 5
#define TAMANHOVETOR TAMANHOBUFFER-1

/* O buffer funciona como uma fila, de acordo com a politica de organizacao FIFO.
 * Esta fila esta organizada em um vetor estatico e a mesma rotaciona por seus indices,
 * para economizar espaco de memoria e nao necessitar de realocacao de seus valores.
 *
 * A definicao TAMANHOBUFFER refere-se aos indices do vetor que serao gerenciados pela politica FIFO,
 * sendo que o vetor real possui um indice a mais (TAMANHOBUFFER), que e reservado para o no raiz
 */

typedef struct bufferpaginas {

	paginaArvore raiz;
	paginaArvore fila[TAMANHOVETOR];
	int primeiro;
	int ultimo;
	int qtd;

} bufferPaginas;

// Metodos
void 			inicializaBuffer(bufferPaginas*, FILE*, int);
int 			encontraChaveBuffer(bufferPaginas*, int);
void 			insereBuffer(bufferPaginas*, FILE*, int);
void 			inicializaFila(bufferPaginas*);
void			insereFila(bufferPaginas*, paginaArvore*); // Funcao de insercao de uma pagina no buffer (retorna 0 caso a insercao ocorre e 1 caso ela falhe)
paginaArvore*		retiraFila(bufferPaginas*);	// Retira uma pagina do buffer
void 			appendBufferInfo(int, int);
#endif
