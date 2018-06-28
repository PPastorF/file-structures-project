// Projeto pratico (parte 2) da disciplina SCC0215012018 - Organizacao de Arquivos
// Pedro Pastorello Fernandes - NUSP 10262502

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "indice.h"
#include "manipulacaoArquivos2.h"

/* Etc: */

/* Funcao chamada em main para checar se o numero n de argumentos do programa esta de acordo
 * com o numero e esperado */
void checkParametros(int n, int e) {
    
    if (n == 1) {
        printf("ERRO: Parametros ausentes!\n");
        exit(1);
    }
    else
   		if (n != e) {
            printf("ERRO: Numero de parametros invalido!\n");
            exit(1);
        }
}

/* Realiza o tratamento de um registro entrado como argumento do programa
 * e retorna um ponteiro para o mesmo */
registro* registroEntrada(char* args[]) {

    // Definicao de uma string nula, de tamanho 10 para ser utilizada futuramente nos campos de tamanho fixo
    char nada[11] = {'0','0','0','0','0','0','0','0','0','0','\0'};

	registro* R = malloc(sizeof(registro));

	// codEscola
    R->codEscola = atoi(args[2]);

    // Campos de tamanho fixo
    if (strcmp(args[3], "0") == 0)
        strcpy(R->dataInicio, nada);
    else
        strcpy(R->dataInicio, args[3]);

    if (strcmp(args[4], "0") == 0)
        strcpy(R->dataFinal, nada);
    else
        strcpy(R->dataInicio, args[4]);

    // Campos de tamanho variavel: tratamento de seus tamanhos

    if (strlen(args[5]) == 0) {
        R->endereco.tamanho = 1;
        R->municipio.valor   = malloc(1);
        strcpy(R->nomeEscola.valor, "\0");
    }
    else {
        R->nomeEscola.tamanho = strlen(args[5]);
        R->nomeEscola.valor   = malloc(R->nomeEscola.tamanho+1);
        strcpy(R->nomeEscola.valor, args[5]);
    }
    if (strlen(args[6]) == 0) {
        R->municipio.tamanho = 1;
        R->municipio.valor   = malloc(1);
        strcpy(R->municipio.valor, "\0");
    }
    else {
        R->municipio.tamanho = strlen(args[6]);
        R->municipio.valor   = malloc(R->municipio.tamanho+1);
        strcpy(R->municipio.valor, args[6]);
    }
    
    if (strlen(args[7]) == 0) {
        R->endereco.tamanho = 1;
        R->endereco.valor   = malloc(1);
        strcpy(R->endereco.valor, "\0");
    }
    else {
        R->endereco.tamanho = strlen(args[7]);
        R->endereco.valor   = malloc(R->endereco.tamanho+1);
        strcpy(R->endereco.valor, args[7]);
    }

    return R;
}

// Funcao para leitura de strings baseada no formato .csv
char* leString(FILE* F) {
	
	char* string = malloc(sizeof(char));
	int pos = 0;
	
	do {
		fread(string + pos++, sizeof(char), 1, F);
		string = realloc(string, sizeof(char) * (pos  + 1));
	} while (string[pos - 1] != ';');

	string[pos - 1] = '\0';
	return string;
}

// Funcao que converte uma string para seu equivalente maisculo
char* maiusculoStr(char *S) {

	int i;

	for (i = 0; i < strlen(S); i++) {
		S[i] = (char)toupper(S[i]) ;
	}

	return S;
}

// Funcao que retorna o proximo caracter de um arquivo a ser lido, sem altera-lo
int proxChar(FILE* F) {

	int c;

	c = fgetc(F);
	ungetc(c, F);

	return c;
}

// Copia os campos do registro src para o registro dest
void copiaRegistro(registro* dest, registro* src) {

	dest->codEscola = src->codEscola;
	
	dest->nomeEscola.tamanho = src->nomeEscola.tamanho;
	strncpy(dest->nomeEscola.valor, src->nomeEscola.valor, src->nomeEscola.tamanho);
	
	dest->municipio.tamanho = src->municipio.tamanho;
	strncpy(dest->municipio.valor, src->municipio.valor, src->municipio.tamanho);
	
	dest->endereco.tamanho = src->endereco.tamanho;
	strncpy(dest->endereco.valor, src->endereco.valor, src->endereco.tamanho);
	
	strncpy(dest->dataInicio, src->dataInicio, 11);
	strncpy(dest->dataFinal, src->dataFinal, 11);

}

// Funcao de impressao formatada de um registro
void printaRegistro(registro* R) {

	if (R != NULL) {
		printf("%d ", R->codEscola);
		printf(" ");                
		if ( strcmp(R->dataInicio,"0000000000") == 0 )
		    printf("           ");
		else
		    printf("%s ", R->dataInicio);
		printf(" ");
		if ( strcmp(R->dataFinal,"0000000000") == 0 )
		    printf("           ");
		else
		    printf("%s ", R->dataFinal);
		printf(" ");
		printf("%.2d ", R->nomeEscola.tamanho);
		if (R->nomeEscola.tamanho)
			printf("%s ", R->nomeEscola.valor);

		printf("%.2d ", R->municipio.tamanho);
		if (R->municipio.tamanho)
			printf("%s ", R->municipio.valor);
		
		printf("%.2d ", R->endereco.tamanho);
		if (R->endereco.tamanho)
			printf("%s " , R->endereco.valor);
		printf("\n");
	}
}

// Libera a memoria dinamica ocupada por um registro
void limpaRegistro(registro* R) {

	free(R->nomeEscola.valor);
	free(R->municipio.valor);
	free(R->endereco.valor);
	free(R);
}

// 	Incrementa o tamanho de um vetor de registros, adicionando o registro reg na ultima posicao
void appendRegistroV(registroV* R, registro* reg) {
				
	// Incrementa o tamanho do vetor e aloca a memoria de seus ponteiros
	R->tam++;
	R->vet = realloc(R->vet, R->tam*sizeof(registro) );
	
	R->vet[R->tam-1].nomeEscola.valor = malloc(reg->nomeEscola.tamanho * sizeof(char));
	R->vet[R->tam-1].municipio.valor  = malloc(reg->municipio.tamanho * sizeof(char));
	R->vet[R->tam-1].endereco.valor   = malloc(reg->endereco.tamanho * sizeof(char));

	// Copia os dados da variavel auxiliar para o ultimo endereco do vetor
	copiaRegistro( &(R->vet[R->tam-1]), reg);
}

// Procedimento de fechamento do arquivo de dados
void fechaArquivo(FILE* F) {

	byte status = 1;
	fseek(F, 0, SEEK_SET);
	fwrite(&status , 1, 1, F);
	fclose(F);
}

/* Manipulacao de arquivo .csv: */

// Le um campo de tamanho variavel de um arquivo .csv
void leTamanhoVariavelCsv(FILE* F, campoString* C) {

	// Variavel 'lixo', para os caracteres ";"
	char lixo;

	// Checa se o campo esta vazio, ou seja, se o proximo caracter eh ";"
	if (proxChar(F) == ';') {

		// Caso esteja vazia, seta o tamanho da string como tendo tamanho 0 e valor nulo
		C->tamanho = 0;
		C->valor = "\0";

		// Limpa um caracter ";"
		fread(&lixo, 1, 1, F);
	}
	else {
		// Caso nao esteja vazio, le a string e seta seu tamanho (nao e preciso limpar o caracter ";" pois a funcao leString ja o faz)
		C->valor = leString(F);
		C->tamanho = strlen(C->valor);
	}

}

// Le um campo de tamanho fixo do arquivo .csv
void leTamanhoFixoCsv(FILE* F, char* C) {

	/* Le uma string de tamanho (tamanho fixo de 10 caracteres)::
	 * Checa se o campo esta vazio, ou seja, se o proximo caracter eh ';',
	 * no caso de dataInicio, ou '\n' no caso de dataFinal. Caso seja o ultimo
	 * registro do arquivo, o proximo char é EOF (fim do arquivo)
	*/
	if ( (proxChar(F) == ';') || (proxChar(F) == '\n') || (proxChar(F) == EOF) ) {

		// Caso esteja vazio, seta a data como 'nula'
		char nada[11] = {'0','0','0','0','0','0','0','0','0','0','\0'};
		strcpy(C, nada);
	}
	else {
		// Caso nao esteja vazio, le a data normalmente

		fread(C, 10, 1, F);	// Le dataInicio (string de 10 elementos) 
		C[10] = '\0';		// Manipulacao da sring		
	}

}

// Le um registro de um arquivo .csv
registro* leRegistroCsv(FILE* F) {

	// Variavel auxiliar
	registro* aux = malloc( sizeof(registro) );
	
	// Variavel 'lixo', para os caracteres ";"
	char lixo;

	int status = 0;
	// Caso exista, seta seu status (no cabecalho) como 0
	if (fscanf(F, "%d", &aux->codEscola) == EOF) {
    	fseek(F, 0, SEEK_SET);
		fwrite(&status , 1, 1, F);
		free(aux);
		return NULL;
	}

	// Limpa um caracter ";"
	fread(&lixo, 1, 1, F);

	// Le os campos de tamanho variavel
	leTamanhoVariavelCsv(F, &(aux->nomeEscola) );
	leTamanhoVariavelCsv(F, &(aux->municipio) );
	leTamanhoVariavelCsv(F, &(aux->endereco) );

	// Le os campos de tamanho fixo
	leTamanhoFixoCsv(F, aux->dataInicio);
	fread(&lixo, 1, 1, F);
	leTamanhoFixoCsv(F, aux->dataFinal);

	return aux;
}

// Le um arquivo .csv
void leArquivoCsv(FILE* F, registroV* R) {

	// Aloca o vetor de registros que sera utilizado para armazena-los
	R->vet = NULL;

	// Variavel auxiliar
	registro* registroAux = NULL;
	// Loop de leitura do arquivo F
	do {
		// Le um registro do arquivo F
		registroAux = leRegistroCsv(F);
		
		// Incrementa o tamanho do vetor de registros e coloca o registro lido nesta nova posicao
		if ( registroAux != NULL) {		
			appendRegistroV(R, registroAux);
		}

	} while(!feof(F));
}

/* Manipulacaoo do arquivo de dados (arquivo binario) */

// Escreve um registro no arquivo de dados
void escreveRegistro(FILE* F, registro* R) {

	// codEscola (4 bytes)
	fwrite( &(R->codEscola), 4, 1, F);		// Escreve codEscola (int / 4 bytes)
	// Campos de tamanho fixo (10 + 10 bytes = 20 bytes)	
	fwrite(R->dataInicio, 10, 1, F);	// Escreve dataInicio (string de 10 bytes)
	fwrite(R->dataFinal, 10, 1, F);		// Escreve dataFinal  (string de 10 bytes)

	// 4 + 20 = 24 bytes

	/* Campos de tamanho variavel: devem se truncados para que o registro possua 112 bytes
	 * 26 + 25 + 25 bytes = 76 bytes.
	 * 
	 * 76 + 12 (3x4, dos indicadores de tamanho) = 88
	 * 
	 * 88 + 24 = 112 bytes
	 */

	// nomeEscola: truncado para 26 bytes
	if (R->nomeEscola.tamanho > 26)
		R->nomeEscola.tamanho = 26;
	// Escreve o indicador de tamanho para nomeEscola (int / 4bytes)
	fwrite(&(R->nomeEscola.tamanho), 4, 1,F);
	
	if (R->nomeEscola.tamanho <= 26)		// Escreve nomeEscola (string de tamanho variavel, ate o maximo de 26 caracteres)
		fwrite(R->nomeEscola.valor, R->nomeEscola.tamanho, 1, F);
	else
		fwrite(R->nomeEscola.valor, 26, 1, F);
	
	char nulo = 0;
	int i;
	
	// Realiza o padding da string ate 26, para que o registro final tenha 112 bytes
	if (R->nomeEscola.tamanho < 26) {	
		i = R->nomeEscola.tamanho;
		while(i < 26) {
			fwrite(&nulo, 1, 1, F);
			i++;
		}
	}

	// municipio: truncado para 25 bytes
	if (R->municipio.tamanho > 25)
		R->municipio.tamanho = 25;
	fwrite(&(R->municipio.tamanho), 4, 1,F);
	// Escreve municipio (string de tamanho variavel, ate o maximo de 26 caracteres)
	if (R->municipio.tamanho <= 25)
		fwrite(R->municipio.valor, R->municipio.tamanho, 1, F);
	else
		fwrite(R->municipio.valor, 25, 1, F);
		
	// Padding
	if (R->municipio.tamanho < 25) {	
		i = R->municipio.tamanho;
		while(i < 25) {
			fwrite(&nulo, 1, 1, F);
			i++;
		}
	}

	// endereco: truncado para 25 bytes
	if (R->endereco.tamanho > 25)
		R->endereco.tamanho = 25;
	fwrite(&(R->endereco.tamanho), 4, 1,F);
	
	// Escreve endereco (string de tamanho variavel, ate o maixo de 26 caracteres)
	if (R->endereco.tamanho <= 25)
		fwrite(R->endereco.valor, R->endereco.tamanho, 1, F);
	else
		fwrite(R->endereco.valor, 25, 1, F);
		
	// Padding
	if (R->endereco.tamanho < 25) {
		i = R->endereco.tamanho;
		while(i < 25) {
			fwrite(&nulo, 1, 1, F);
			i++;
		}
	}

}

// Escreve o arquivo de dados (binario)
void escreveArquivo(registroV* R, char* nomeArquivo, buffer* B) {

    // Criacao do arquivo de saida
    FILE* F = fopen(nomeArquivo, "wb");
    if (F == NULL) {
        printf("ERRO: Falha na criacao de arquivo de dados\n");
        exit(0);
    }

	// Escreve o cabecalho do arquivo
	byte status = 0;
	fwrite(&status, 1, 1, F);

	int topoPilha = -1;
	fwrite(&topoPilha, sizeof(int), 1, F);

	// Abre o arquivo de indices
	FILE* I = abreArquivoIndice();

	// Escreve o corpo do arquivo
	int i;
	for (i = 0; i < R->tam; i++) {

		// Escreve um registro no arquivo de dado 
		escreveRegistro(F, &(R->vet[i]));

		// Insere o elemento correspondente ao registro no arquivo de indice
		insereChaveIndice(I, B, R->vet[i].codEscola, i);
	}

	fechaArquivoIndice(I);
	fechaArquivo(F);
}

// Le um campo de tamanho fixo do arquivo de dados
void leTamanhoFixoBin(FILE* F, char* C) {

	fread(C, 1, 10, F);
	C[10] = '\0';
}

// Le um campo de tamanho variavel de 26 bytes do arquivo de dados
void leTamanhoVariavel26Bin(FILE* F, campoString* C) {

	int tam; // Variavel auxiliar
	fread(&tam, 4, 1, F);
	
	// Caso esteja vazia, seta o tamanho da string como tendo tamanho 0 e valor nulo
	if (tam == 0) {
		C->tamanho = 0;
		C->valor = malloc(1);
		C->valor = "\0";
	}
	else {
		// Caso nao esteja vazio, le a string e seta seu tamanho
		C->valor = malloc(tam+1);
		fread(C->valor, 1, tam, F);
		C->valor[tam] = '\0';
		C->tamanho = strlen(C->valor);

		// Caso a string seja maior que 26 bytes
		if (tam >= 26) {
			C->valor[25] = '\0';
			C->tamanho = 26;
		}
	}

	// Anda o offset restante para o proximo campo
	char lixo;
	int i;
	for (i = tam; i < 26; i++) {
		fread(&lixo, 1, 1, F);
	}

}

// Le um campo de tamanho variavel de 25 bytes do arquivo de dados
void leTamanhoVariavel25Bin(FILE* F, campoString* C) {

	int tam; // Variavel auxiliar
	fread(&tam, 4, 1, F);

	// Caso esteja vazia, seta o tamanho da string como tendo tamanho 0 e valor nulo
	if (tam == 0) {
		C->tamanho = 0;
		C->valor = malloc(1);
		C->valor = "\0";
	}
	else {
		// Caso nao esteja vazio, le a string e seta seu tamanho
		C->valor = malloc(tam+1);
		fread(C->valor, 1, tam, F);
		C->valor[tam] = '\0';
		C->tamanho = strlen(C->valor);

		// Caso a string seja maior que 26 bytes
		if (tam >= 25) {
			C->valor[24] = '\0';
			C->tamanho = 25;
		}
	}

	// Anda o offset restante para o proximo campo
	char lixo;
	int i;
	for (i = tam; i < 26; i++) {
		fread(&lixo, 1, 1, F);
	}
}

// Le um registro do arquivo de dados
registro* leRegistroBin(FILE* F) {

	// Variavel auxiliar
	registro* aux = malloc( sizeof(registro) );
	
	// Condicao para caso o registro tenha sido removido
	if ( proxChar(F) == '*' ) {
		return NULL;
	}
	else {
		// Le os primeiros 4 bytes do registro e salva em codEscola
		int ajuda;
		if ( fread(&ajuda, 4, 1, F) != 1 ) {
			free(aux);
			return NULL;
		}
		aux->codEscola = ajuda;
		// Le as strings de tamanho fixo
		leTamanhoFixoBin(F, aux->dataInicio);
		leTamanhoFixoBin(F, aux->dataFinal);

		// Le as strings de tamanho variavel
		leTamanhoVariavel26Bin(F, &(aux->nomeEscola) );
		leTamanhoVariavel25Bin(F, &(aux->municipio) );
		leTamanhoVariavel25Bin(F, &(aux->endereco) );

		return aux;
	}
}   

// Le o arquivo de dados (binario)
void leArquivoBin(FILE* F, registroV* R) {
	
	// Aloca o vetor de registros que sera utilizado para armazena-los
	R->vet = NULL;

	// Variavel auxiliar
	registro* registroAux = malloc(sizeof(registro) );

	int i = 0;
	fseek(F, 5, SEEK_SET);
	// Loop de leitura do arquivo F
	do {
		// Le um registro do arquivo F
		fseek(F, (i++*TAMANHO_REGISTRO)+5, SEEK_SET);
		registroAux = leRegistroBin(F);

		// Incrementa o tamanho do vetor de registros e coloca o registro lido nesta nova posicao
		if ( registroAux != NULL) {		
			appendRegistroV(R, registroAux);
		}

	} while(!feof(F));

}


// Descobre a quantidade de registros em um arquivo de dados F,
// Depois, volta para o a posicao do arquivo indicada pelo argumento RRN
int	numeroRegistros(FILE* F) {

	fseek(F, 0L, SEEK_END);
	int tamanhoArquivo = ftell(F);
	int qtdRegistros = ( (tamanhoArquivo-5) / TAMANHO_REGISTRO );

	return qtdRegistros;

}

// Funcao de busca e impressao de registros
void buscaRegistro(int op, char* arg) {

	/* Passa por todos os registros de um arquivo de dados (acessando-o cada vez) e o printa caso encontre um correspondente a busca 
	a variavel op indica qual o campo que esta sendo buscado:

		0 = nenhum (printa todos os registros)
		1 = codEscola
		2 = dataInicio
		3 = dataFinal
		4 = nomeEscola
		5 = municipio
		6 = endereco
		7 = RRN
	*/

	// Variaveis
	registro* reg = malloc(sizeof(registro) );
	int i = 0;
	int qtdEncontrados = 0;
	FILE* fileDados;
	switch(op) {

		case 0:	// Busca 'nula', printa todos

			// Passa por todos os registros e, caso encontre um correspondente a busca, o printa
			do {
			    // Carregamento do arquivo binario para a memoria principal
			    fileDados = fopen("fileOut", "rb");
			    if (fileDados == NULL) {
			        printf("ERRO: Arquivo de dados nao encontrado.");
			        return;
			    }

			    // Econtra a posicao para o registro atual
			    fseek(fileDados, ( (i++)*(TAMANHO_REGISTRO) ) +5, SEEK_SET);

			    // Condicao para caso o registro tenha sido removido
			    if ( proxChar(fileDados) == '*' ) {
			    	fclose(fileDados);
			    	continue;
			    }

			    // Le um registro do arquivo"file
			    reg = leRegistroBin(fileDados);
			    
			    // Printa o registro
			    if (reg != NULL) { // Caso tenha-se chego ao final do arquivo, a funcao leRegistroBin() retorna NULL
			        printaRegistro(reg);
					qtdEncontrados++;
			    }
			    else
			        break;

			    // Fecha o arquivo
			    fclose(fileDados);

			} while(reg != NULL);
			break;

		case 1:	// Busca por codEscola

			// Passa por todos os registros e, caso encontre um correspondente a busca, o printa
			do {
			    // Carregamento do arquivo binario para a memoria principal
			    fileDados = fopen("fileOut", "rb");
			    if (fileDados == NULL) {
			        printf("ERRO: Arquivo de dados nao encontrado.");
			        return;
			    }

			    // Econtra a posicao para o registro atual
			    fseek(fileDados, ( (i++)*(TAMANHO_REGISTRO) )+5, SEEK_SET);
			    
			    // Condicao para caso o arquivo tenha sido removido
			    if ( proxChar(fileDados) == '*' ) {
			    	fclose(fileDados);
			    	continue;
			    }

			    // Le um registro do arquivo
			    reg = leRegistroBin(fileDados);
			    
			    // Printa o registro
			    if (reg != NULL) { // Caso tenha-se chego ao final do arquivo, a funcao leRegistroBin() retorna NULL
			  
			  		if ( reg->codEscola == atoi(arg) ) {
			  	    	printaRegistro(reg);
						qtdEncontrados++;
			  		}
			    }
			    else
			        break;

			    // Fecha o arquivo
			    fclose(fileDados);

			} while(reg != NULL);
			break;

		case 2:	// Busca por dataInicio

			// Passa por todos os registros e, caso encontre um correspondente a busca, o printa
			do {
			    // Carregamento do arquivo binario para a memoria principal
			    fileDados = fopen("fileOut", "rb");
			    if (fileDados == NULL) {
			        printf("ERRO: Arquivo de dados nao encontrado.");
			        return;
			    }

			    // Econtra a posicao para o registro atual
			    fseek(fileDados, ( (i++)*(TAMANHO_REGISTRO) )+5, SEEK_SET);

			    // Condicao para caso o arquivo tenha sido removido
			    if ( proxChar(fileDados) == '*' ) {
			    	fclose(fileDados);
			    	continue;
			    }
			    
			    // Le um registro do arquivo
			    reg = leRegistroBin(fileDados);
			    
			    // Printa o registro
			    if (reg != NULL) { // Caso tenha-se chego ao final do arquivo, a funcao leRegistroBin() retorna NULL
			  
			  		if ( strstr(reg->dataInicio, arg) !=  NULL ) {
			  	    	printaRegistro(reg);
			  	    	qtdEncontrados++;
			  		}
			    }
			    else
			        break;

			    // Fecha o arquivo
			    fclose(fileDados);

			} while(reg != NULL);
			break;

		case 3:	// Busca por dataFinal

			// Passa por todos os registros e, caso encontre um correspondente a busca, o printa
			do {
			    // Carregamento do arquivo binario para a memoria principal
			    fileDados = fopen("fileOut", "rb");
			    if (fileDados == NULL) {
			        printf("ERRO: Arquivo de dados nao encontrado.");
			        return;
			    }

			    // Econtra a posicao para o registro atual
			    fseek(fileDados, ( (i++)*(TAMANHO_REGISTRO) )+5, SEEK_SET);
			    
			    // Condicao para caso o arquivo tenha sido removido
			    if ( proxChar(fileDados) == '*' ) {
			    	fclose(fileDados);
			    	continue;
			    }

			    // Le um registro do arquivo
			    reg = leRegistroBin(fileDados);
			    
			    // Printa o registro
			    if (reg != NULL) { // Caso tenha-se chego ao final do arquivo, a funcao leRegistroBin() retorna NULL
			  
			  		if ( strstr(reg->dataFinal, arg) != NULL ) {
			  	    	printaRegistro(reg);
			  	    	qtdEncontrados++;
			  		}
			    }
			    else
			        break;

			    // Fecha o arquivo
			    fclose(fileDados);

			} while(reg != NULL);
			break;

		case 4:	// Busca por nomeEscola

			// Passa por todos os registros e, caso encontre um correspondente a busca, o printa
			do {
			    // Carregamento do arquivo binario para a memoria principal
			    fileDados = fopen("fileOut", "rb");
			    if (fileDados == NULL) {
			        printf("ERRO: Arquivo de dados nao encontrado.");
			        return;
			    }

			    // Econtra a posicao para o registro atual
			    fseek(fileDados, ( (i++)*(TAMANHO_REGISTRO) )+5, SEEK_SET);
			    
			    // Condicao para caso o arquivo tenha sido removido
			    if ( proxChar(fileDados) == '*' ) {
			    	fclose(fileDados);
			    	continue;
			    }
			    
			    // Le um registro do arquivo
			    reg = leRegistroBin(fileDados);
			    
			    // Printa o registro
			    if (reg != NULL) { // Caso tenha-se chego ao final do arquivo, a funcao leRegistroBin() retorna NULL
			  
			  		if ( strstr(reg->nomeEscola.valor, maiusculoStr(arg)) != 0 ) {
			  	    	printaRegistro(reg);
			  	    	qtdEncontrados++;
			  		}
			    }
			    else
			        break;

			    // Fecha o arquivo
			    fclose(fileDados);

			} while(reg != NULL);
			break;

		case 5:	// Busca por municipio

			// Passa por todos os registros e, caso encontre um correspondente a busca, o printa
			do {
			    // Carregamento do arquivo binario para a memoria principal
			    fileDados = fopen("fileOut", "rb");
			    if (fileDados == NULL) {
			        printf("ERRO: Arquivo de dados nao encontrado.");
			        return;
			    }

			    // Econtra a posicao para o registro atual
			    fseek(fileDados, ( (i++)*(TAMANHO_REGISTRO) ) +5, SEEK_SET);

			    // Condicao para caso o arquivo tenha sido removido
			    if ( proxChar(fileDados) == '*' ) {
			    	fclose(fileDados);
			    	continue;
			    }
			    
			    // Le um registro do arquivo
			    reg = leRegistroBin(fileDados);
			    
			    // Printa o registro
			    if (reg != NULL) { // Caso tenha-se chego ao final do arquivo, a funcao leRegistroBin() retorna NULL
			  
			  		if ( strstr(reg->municipio.valor, maiusculoStr(arg)) != NULL ) {
			  	    	printaRegistro(reg);
			  	    	qtdEncontrados++;
			  		}
			    }
			    else
			        break;

			    // Fecha o arquivo
			    fclose(fileDados);

			} while(reg != NULL);
			break;

		case 6:	// Busca por endereco

			// Passa por todos os registros e, caso encontre um correspondente a busca, o printa
			do {
			    // Carregamento do arquivo binario para a memoria principal
			    fileDados = fopen("fileOut", "rb");
			    if (fileDados == NULL) {
			        printf("ERRO: Arquivo de dados nao encontrado.");
			        return;
			    }

			    // Econtra a posicao para o registro atual
			    fseek(fileDados, ( (i++)*(TAMANHO_REGISTRO) )+5, SEEK_SET);
			    
			    // Condicao para caso o arquivo tenha sido removido
			    if ( proxChar(fileDados) == '*' ) {
			    	fclose(fileDados);
			    	continue;
			    }
			    
			    // Le um registro do arquivo
			    reg = leRegistroBin(fileDados);
			    
			    // Printa o registro
			    if (reg != NULL) { // Caso tenha-se chego ao final do arquivo, a funcao leRegistroBin() retorna NULL
			  
			  		if ( strstr(reg->endereco.valor, maiusculoStr(arg)) != NULL ) {
			  	    	printaRegistro(reg);
			  	    	qtdEncontrados++;
			  		}
			    }
			    else
			        break;

			    // Fecha o arquivo
			    fclose(fileDados);

			} while(reg != NULL);
			break;

	case 7:	// Busca por RRN

			// Printa apenas um registro, de acordo com o byte offset provido pelo RRN

		    // Carregamento do arquivo binario para a memoria principal
		    fileDados = fopen("fileOut", "rb");
		    if (fileDados == NULL) {
		        printf("ERRO: Arquivo de dados nao encontrado.");
		        return;
		    }

		    int rrn = atoi(arg);

			// Descobre o numero de registros no arquivo de dados
		    int qtdRegistros = numeroRegistros(fileDados);

		    // Econtra a posicao para o registro atual
		    fseek(fileDados, ( (rrn)*TAMANHO_REGISTRO ) +5, SEEK_SET);

			// Verificacao da existencia do registro no arquivo de dados
			if ( (proxChar(fileDados) == '*') || (rrn < 0) || (rrn > qtdRegistros-1) ) {
		    	printf("ERRO: Registro inexistente.\n");
		    	return;
		    }
		    else {
			    // Econtra a posicao para o registro atual
			    fseek(fileDados, ( (rrn)*TAMANHO_REGISTRO )+5, SEEK_SET);

			    // Condicao para caso o arquivo tenha sido removido
			    if ( proxChar(fileDados) == '*' ) {
			    	fclose(fileDados);
			    	break;
			    }
				    
			    // Le um registro do arquivo
			    reg = leRegistroBin(fileDados);
			    
			    // Printa o registro
			    if (reg != NULL) { // Caso tenha-se chego ao final do arquivo, a funcao leRegistroBin() retorna NULL
			        printaRegistro(reg);
					qtdEncontrados++;
			    }
			    else
			        break;
		    }

		    // Fecha o arquivo
		    fclose(fileDados);
		    break;
	}

	// Caso nao tenha sido encontrado nenhum registro que corresponde a busca
	if (qtdEncontrados == 0) 
		printf("Registro inexistente.\n");

}

// Remove um registro do arquivo de dados
void removeRegistro(char* argRrn) {

	// Abre o arquivo de dados
	byte status = 0;

    FILE* F = fopen("fileOut", "rb+");
    if (F == NULL) {
    	// Caso o arquivo nao exista
        printf("ERRO: Arquivo de dados nao encontrado.");
        return;
    }
    else {
    	// Caso exista, seta seu status (na cabecalho) como 0
    	fseek(F, 0, SEEK_SET);
		fwrite(&status , 1, 1, F);
    }

	// Truncamento do argumento de RRN para um inteiro
	int rrn = atoi(argRrn);

	// Descobre o numero de registros no arquivo de dado
	int qtdRegistros = numeroRegistros(F);

	// Encontra o byte offset do registro a ser removido
	fseek(F, ( (rrn)*(TAMANHO_REGISTRO) )+ 5, SEEK_SET);

	// Verificacao da existencia do registro no arquivo de dados
	if ( (proxChar(F) == '*') || (rrn < 0) || (rrn > qtdRegistros-1) ) {
		
		printf("ERRO: Registro inexistente\n");
	}
	else {

		// Le o topo da pilha do cabecalho
		int topoPilha;
		fseek(F, 1, SEEK_SET);
		fread(&topoPilha, sizeof(int), 1, F);

		// Encontra o byte offset do registro a ser removido
		fseek(F, ( (rrn)*(TAMANHO_REGISTRO) )+ 5, SEEK_SET);
		fprintf(F, "%c", '*');
		fwrite(&topoPilha, sizeof(int), 1, F);
		
		// Seta o topo da pilha no cabecalho (rrn do registro removido nesta instancia da funcao)
		fseek(F, 1, SEEK_SET);
		fwrite(&rrn , sizeof(int), 1, F);
		printf("Registro removido com sucesso.\n");
	}

	fechaArquivo(F);

}

/* Insere um registro no arquivo de dados, de acordo com o reaproveitamento
 * dinamico de espaco de armazenamento */
void insercaoDinamica(registro* R, buffer* B) {

	// Abre o arquivo de dados
	byte status = 0;

    FILE* F = fopen("fileOut", "rb+");
    if (F == NULL) {
    	// Caso o arquivo nao exista
        printf("ERRO: Arquivo de dados nao encontrado.");
        return;
    }
    else {
    	// Caso exista, seta seu status (na cabecalho) como 0
    	fseek(F, 0, SEEK_SET);
		fwrite(&status , 1, 1, F);
    }

    // Abre o arquivo de indices
	FILE* I = abreArquivoIndice();

    // Le topo da pilha do cabecalho do arquivo de indices
    int topoPilha;
    fseek(F, 1, SEEK_SET);
    fread(&topoPilha, sizeof(int), 1, F);

    // RRN de insercao do registro no arquivo de dados
    int rrn;

    // Caso a pilha esteja vazia, escreve o registro no final do arquivo
    if (topoPilha == -1) {
    	rrn = numeroRegistros(F) -1;
    	fseek(F, 0, SEEK_END);
    	escreveRegistro(F, R);

    }
    else {	// Caso a pilha nao esteja vazia:
   		
    	rrn = topoPilha;

    	// Le o proximo topoPilha, do RRN correto
    	fseek(F, ( (rrn)*TAMANHO_REGISTRO )+6, SEEK_SET);
	    fread(&topoPilha, sizeof(int), 1, F);
   		
   		// Escreve o novo topoPilha, lido anteriormente, no cabecalho
   		fseek(F, 1, SEEK_SET);
	    fwrite(&topoPilha, sizeof(int), 1, F);
    
    	// Escreve o novo registro no RRN lido do topo da pilha
		fseek(F, ( (rrn)*TAMANHO_REGISTRO )+5, SEEK_SET);
		escreveRegistro(F, R);

    }
    // Insere a chave correspondente no arquivo de indices
    insereChaveIndice(I, B, R->codEscola, rrn);

    // Fecha ambos os arquivos
	fechaArquivo(F);
	fechaArquivoIndice(I);
}
