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

	int 			nChaves; 				  // Numero n de chaves da pagina
	registroIndice  registros[ORDEMARVORE-1]; // Vetor com as chaves armazenadas pela pagina
	int				filhas[ORDEMARVORE];	  // Vetor com os RRNs das paginas filhas (implementacao em memoria secundaria)

} paginaArvore;


// Representacao de uma Arvore B em memoria principal
typedef struct arvoreb {
	
	paginaArvore* raiz;
	int	altura;

} arvoreB;


// Metodos:

// TAD Indice/Arvore B
FILE*			criaIndice();	// Funcao de criacao de um arquivo de indices
int				buscaIndice(FILE*, char*);	// Funcao que retorna o RRN correspondente a uma chave, no arquivo de indices
void			insereChave(FILE*);	// Funcao de insercao de um registro em um arquivo de indice
void			removeChave(FILE*);	// Funcao de remocao de um registro em um arquivo de indice
void			atualizaIndice(FILE*, FILE*);	// Funcao que atualiza os registros de um arquivo de indices, baseado em um arquivo de dados
void			destroiIndice(FILE*);	// Funcao de destruicao de um arquivo de indices
FILE*			carregaIndice(char*); // Funao de carregamento de um arquivo de indices para memoria principal
void			reescreveIndice(FILE*, indiceRAM);
int 			encontraChavePagina(paginaArvore* P, int);

// Metodos Auxiliares
void 			fechaArquivoIndice(FILE*);
int 			leRrnRaiz(FILE*);
paginaArvore*	lePagina(FILE*, int);
void			copiaPagina(paginaArvore*, paginaArvore*); // Funcao auxiliar que copia os conteudos de uma pagina para outra

#endif