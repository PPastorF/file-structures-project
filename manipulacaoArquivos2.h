// Projeto pratico (parte 2) da disciplina SCC0215012018 - Organizacao de Arquivos
// Pedro Pastorello Fernandes - NUSP 10262502

#ifndef _MANIPULACAOARQUIVOS_H_
#define _MANIPULACAOARQUIVOS_H_

#include "indice.h"

#define TAMANHO_REGISTRO 112

typedef unsigned char byte;

// Definicao de um campo de tamanho variavel (string)
typedef struct campostring {

	int   tamanho;
	char* valor;

} campoString;

// Definicao de um registro
typedef struct Registro {

	int 		codEscola;
	char 		dataInicio[11];
	char  		dataFinal[11];
	campoString nomeEscola;
	campoString municipio;
	campoString endereco;

} registro;

// Definicao de um vetor de registros que armazena seu tamanho
typedef struct registrov {

	int 	  tam;
	registro* vet;

} registroV;

// Metodos

// Etc
registro*	registroEntrada(char**);
char*		leString(FILE*);
char*		maiusculoStr(char*);
void		checkParametros(int, int);
void 		copiaRegistro(registro*, registro*);
void 		printaRegistro(registro*);
void  		limpaRegistro(registro*);
void 		appendRegistroV(registroV*, registro*);
void		fechaArquivo(FILE*);

// Manipulacao de arquivo .csv
registro*	leRegistroCsv(FILE*);
void		leTamanhoVariavelCsv(FILE*, campoString*);
void		leTamanhoFixoCsv(FILE*, char*);
void  		leArquivoCsv(FILE*, registroV*);


// Manipulacaoo do arquivo de dados (arquivo binario)
registro*	leRegistroBin(FILE*);
void		printaRegistro(registro*);
void  		escreveRegistro(FILE*, registro*);
void  		escreveArquivo(registroV*, char*, buffer*);
void		leArquivoBin(FILE*, registroV*);
void		buscaRegistro(int, char*);
void		removeRegistro( char*);
void		insercaoDinamica(registro*, buffer*);
void		leTamanhoFixoBin(FILE*, char*);
void		leTamanhoVariavelBin(FILE*, campoString*);
int			numeroRegistros(FILE*);
int			proxChar(FILE*);

#endif