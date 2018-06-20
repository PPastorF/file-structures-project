// Projeto pratico (parte 2) da disciplina SCC0215012018 - Organizacao de Arquivos
// Pedro Pastorello Fernandes - NUSP 10262502

#ifndef _MANIPULACAOARQUIVOS_H_
#define _MANIPULACAOARQUIVOS_H_

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
void		checkParametros(int, int);
char*		leString(FILE*);
char*		maiusculoStr(char*);
void 		copiaRegistro(registro*, registro*);
void 		printaRegistro(registro*);
void  		limpaRegistro(registro*);
void 		appendRegistroV(registroV*, registro*);
void		fechaArquivo(FILE*);

// Manipulacao de arquivo .csv
void		leTamanhoVariavelCsv(FILE*, campoString*);
void		leTamanhoFixoCsv(FILE*, char*);
registro*	leRegistroCsv(FILE*);
void  		leArquivoCsv(FILE*, registroV*);


// Manipulacaoo do arquivo de dados (arquivo binario)
void		printaRegistro(registro*);
void  		escreveRegistro(FILE*, registro*);
void  		escreveArquivo(FILE*, registroV*, char*);
registro*	leRegistroBin(FILE*);
void		leArquivoBin(FILE*, registroV*);
void		buscaRegistro(FILE*, int, char*);
void		removeRegistro(FILE*, char*);
void		insercaoDinamica(FILE*, registro*);
int			numeroRegistros(FILE*, int);
int			proxChar(FILE*);
void		leTamanhoFixoBin(FILE*, char*);
void		leTamanhoVariavelBin(FILE*, campoString*);

#endif