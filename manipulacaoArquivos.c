// Projeto pratico da disciplina SCC0215012018 - Organizacao de Arquivos
// Pedro Pastorello Fernandes - NUSP 10262502

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "manipulacaoArquivos.h"

// Funcao para leitura de strings baseada no formato .csv
char* leString(FILE* F) {
	
	char *string = malloc(sizeof(char));
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

	for(i = 0; i < strlen(S); i++) {
		S[i] = (char)toupper(S[i]) ;
	}

	return S;
}

void printaRegistro(registro* R) {

	printf("%d ", R->codEscola);
	printf(" ");                
	if(R->dataInicio == 0000000000)
	    printf("           ");
	else
	    printf("%s ", R->dataInicio);
	printf(" ");
	if(R->dataFinal == 0000000000)
	    printf("           ");
	else
	    printf("%s ", R->dataFinal);
	printf(" ");
	printf("%.2d ", R->nomeEscola.tamanho);
	printf("%s ", R->nomeEscola.valor);
	printf("%.2d ", R->municipio.tamanho);
	printf("%s ", R->municipio.valor);
	printf("%.2d ", R->endereco.tamanho);
	printf("%s " , R->endereco.valor);
	printf("\n");
}

// Funcao que retorna o proximo caracter de um arquivo a ser lido, sem altera-lo
int proxChar(FILE* F) {

	int c;

	c = fgetc(F);
	ungetc(c, F);

	return c;
}

registro* leRegistroCsv(FILE* F) {

	// Variavel auxiliar
	registro* aux = malloc( sizeof(registro) );
	
	// Variavel 'lixo', para os caracteres ";"
	char lixo;

	// Le o codigo da escola (int)
	// Para evitar bugs, uma condicao para verificar se o arquivo ainda nao foi completamente lido
	if(fscanf(F, "%d", &aux->codEscola) == EOF) {
		free(aux);
		return NULL;
	}
	fread(&lixo, 1, 1, F);			  // Limpa um caracter ";"

	// Le a string nomeEscola e seta seu tamanho:
	// Checa se o campo esta vazio, ou seja, se o proximo caracter eh ";"
	if(proxChar(F) == ';') {
		// Caso esteja vazia, seta o tamanho da string como tendo tamanho 0 e valor nulo
		aux->nomeEscola.tamanho = 0;
		aux->nomeEscola.valor = "\0";
		fread(&lixo, 1, 1, F);	// Limpa um caracter ";"
	}
	else {
		// Caso nao esteja vazio, le a string e seta seu tamanho (nao e preciso limpar o caracter ";" pois a funcao leString ja o faz)
		aux->nomeEscola.valor = leString(F);
		aux->nomeEscola.tamanho = strlen(aux->nomeEscola.valor);
	}

	// Le a string municipio e seta seu tamanho:
	// Checa se o campo esta vazio, ou seja, se o proximo caracter eh ";"
	if(proxChar(F) == ';') {
		// Caso esteja vazia, seta o tamanho da string como tendo tamanho 0 e valor nulo
		aux->municipio.tamanho = 0;
		aux->municipio.valor = "\0";
		fread(&lixo, 1, 1, F);	// Limpa um caracter ";"
	}
	else {
		// Caso nao esteja vazio, le a string e seta seu tamanho (nao e preciso limpar o caracter ";" pois a funcao leString ja o faz)
		aux->municipio.valor = leString(F);
		aux->municipio.tamanho = strlen(aux->municipio.valor);
	}

	// Le a string endereco e seta seu tamanho:
	// Checa se o campo esta vazio, ou seja, se o proximo caracter eh ";"
	if(proxChar(F) == ';') {
		// Caso esteja vazia, seta o tamanho da string como tendo tamanho 0 e valor nulo
		aux->endereco.tamanho = 0;
		aux->endereco.valor = "\0";
		fread(&lixo, 1, 1, F);	// Limpa um caracter ";"
	}
	else {
		// Caso nao esteja vazio, le a string e seta seu tamanho (nao e preciso limpar o caracter ";" pois a funcao leString ja o faz)
		aux->endereco.valor = leString(F);
		aux->endereco.tamanho = strlen(aux->endereco.valor);
	}

	// Le a string dataInicio (tamanho fixo de 10 caracteres):
	// Checa se o campo esta vazio, ou seja, se o proximo caracter eh ";"
	if(proxChar(F) == ';') {
		// Caso esteja vazio, seta a data como 'nula'
		char nada[11] = {'0','0','0','0','0','0','0','0','0','0','\0'};
		strcpy(aux->dataInicio, nada);
		fread(&lixo, 1, 1, F);				// Limpa um caracter ";"
	}
	else {
		// Caso nao esteja vazio, le a data normalmente

		fread(aux->dataInicio, 10, 1, F);	// Le dataInicio (string de 10 elementos) 
		aux->dataInicio[10] = '\0';			// Para evitar problemas
		fread(&lixo, 1, 1, F);				// Limpa um caracter ";"
	}

	// Le a string dataFinal (tamanho fixo de 10 caracteres):
	// Checa se o campo esta vazio, ou seja, se o proximo caracter eh "\n" (final do registro)
	if(proxChar(F) == '\n') {
		// Caso esteja vazio, seta a data como 'nula'
		char nada[11] = {'0','0','0','0','0','0','0','0','0','0','\0'};
		strcpy(aux->dataFinal, nada);
		// Nao e preciso limpar um caracter ";", pois chega-se ao fina do registro
	}
	else {
		// Caso nao esteja v/azio, le a data normalmente
		fread(aux->dataFinal, 10, 1, F);	// Le dataFinal (string de 10 elementos) 
		aux->dataFinal[10] = '\0';			// Para evitar problemas
		// Nao e preciso limpar um caracter ";", pois chega-se ao fina do registro
	}

	return aux;
}

void leArquivoCsv(FILE* F, registroV* R) {

	// Aloca o vetor de registros que sera utilizado para armazena-los
	R->vet = NULL;

	// Variavel auxiliar
	registro* registroAux = malloc(sizeof(registro) );

	// Loop de leitura do arquivo F
	do {
		// Le um registro do arquivo F
		registroAux = leRegistroCsv(F);
		
		// Acrescenta o registro lido ao vetor de registros,
		// incrementando seu tamanho, na ultima posicao.
		if( registroAux != NULL) {
			
			// Incrementa o tamanho do vetor e aloca a memoria de seus ponteiros
			R->tam++;
			R->vet = realloc(R->vet, R->tam*sizeof(registro) );
			
			R->vet[R->tam-1].nomeEscola.valor = malloc(registroAux->nomeEscola.tamanho * sizeof(char));
			R->vet[R->tam-1].municipio.valor = malloc(registroAux->municipio.tamanho * sizeof(char));
			R->vet[R->tam-1].endereco.valor = malloc(registroAux->endereco.tamanho * sizeof(char));

			// Copia os dados da variavel auxiliar para o ultimo endereco do vetor
			R->vet[R->tam-1].codEscola = registroAux->codEscola;
			
			R->vet[R->tam-1].nomeEscola.tamanho = registroAux->nomeEscola.tamanho;
			strncpy(R->vet[R->tam-1].nomeEscola.valor, registroAux->nomeEscola.valor, registroAux->nomeEscola.tamanho);
			
			R->vet[R->tam-1].municipio.tamanho = registroAux->municipio.tamanho;
			strncpy(R->vet[R->tam-1].municipio.valor, registroAux->municipio.valor, registroAux->municipio.tamanho);
			
			R->vet[R->tam-1].endereco.tamanho = registroAux->endereco.tamanho;
			strncpy(R->vet[R->tam-1].endereco.valor, registroAux->endereco.valor, registroAux->endereco.tamanho);
			
			strncpy(R->vet[R->tam-1].dataInicio, registroAux->dataInicio, 11);
			strncpy(R->vet[R->tam-1].dataFinal, registroAux->dataFinal, 11);
		}

	} while(!feof(F));
}

void escreveRegistro(FILE* F, registro* R) {

	// Campos de tamanho fixo
	fprintf(F, "%d", R->codEscola);		// Escreve codEscola (int / 4 bytes)
	fwrite(R->dataInicio, 10, 1, F);	// Escreve dataInicio (string de 10 bytes)
	fwrite(R->dataFinal, 10, 1, F);		// Escreve dataFinal  (string de 10 bytes)

	// Campos de tamanho variavel (truncados para 26)
	if(R->nomeEscola.tamanho > 26)
		R->nomeEscola.tamanho = 26;
	fprintf(F, "%.2d", R->nomeEscola.tamanho);	// Escreve o indicador de tamanho para nomeEscola (int / 4bytes)
	
	if(R->nomeEscola.tamanho <= 26)		// Escreve nomeEscola (string de tamanho variavel, ate o maximo de 26 caracteres)
		fwrite(R->nomeEscola.valor, R->nomeEscola.tamanho, 1, F);
	else
		fwrite(R->nomeEscola.valor, 26, 1, F);
	
	char nulo = 0;
	int i;
	
	// Realiza o padding da string ate 26, para que o registro final tenha 112 bytes
	if(R->nomeEscola.tamanho < 26) {	
		i = R->nomeEscola.tamanho;
		while(i < 26) {
			fwrite(&nulo, 1, 1, F);
			i++;
		}
	}

	if(R->municipio.tamanho > 26)
		R->municipio.tamanho = 26;
	fprintf(F, "%.2d", R->municipio.tamanho);	// Escreve o indicador de tamanho para municipio (int / 4bytes)
	if(R->municipio.tamanho <= 26)				// Escreve municipio (string de tamanho variavel, ate o maximo de 26 caracteres)
		fwrite(R->municipio.valor, R->municipio.tamanho, 1, F);
	else
		fwrite(R->municipio.valor, 26, 1, F);
		
	// Padding
	if(R->municipio.tamanho < 26) {	
		i = R->municipio.tamanho;
		while(i < 26) {
			fwrite(&nulo, 1, 1, F);
			i++;
		}
	}

	if(R->endereco.tamanho > 26)
		R->endereco.tamanho = 26;
	fprintf(F, "%.2d", R->endereco.tamanho);				// Escreve o indicador de tamanho para endereco (int / 4bytes)
	
	if(R->endereco.tamanho <= 26)								// Escreve endereco (string de tamanho variavel, ate o maixo de 26 caracteres)
		fwrite(R->endereco.valor, R->endereco.tamanho, 1, F);
	else
		fwrite(R->endereco.valor, 26, 1, F);
		
	// Padding
	if(R->endereco.tamanho < 26) {
		i = R->endereco.tamanho;
		while(i < 26) {
			fwrite(&nulo, 1, 1, F);
			i++;
		}
	}
}

void escreveArquivo(FILE* F, registroV* R) {

    // Criacao do arquivo de saida
    F = fopen("fileOut", "wb");

	// Escreve o cabecalho do arquivo
	byte status = 0;
	fwrite(&status, 1, 1, F);

	int topoPilha = -1;
	fwrite(&topoPilha, sizeof(int), 1, F);


	// Escreve o corpo do arquivo
	int i;
	for(i = 0; i < R->tam; i++)
		escreveRegistro(F, &(R->vet[i]));

    // Procedimento de fechamento do arquivo
    status = 1;
    fseek(F, 0, SEEK_SET);
    fwrite(&status , 1, 1, F);
    fclose(F);

}

void limpaRegistro(registro* R) {

	free(R->nomeEscola.valor);
	free(R->municipio.valor);
	free(R->endereco.valor);
	free(R);
}

registro* leRegistroBin(FILE* F) {

	// Variavel auxiliar
	registro* aux = malloc( sizeof(registro) );
	
	// Le o codigo da escola (int)
	// Para evitar bugs, uma condicao para verificar se o arquivo ainda nao foi completamente lido
	
	// Condicao para caso o registro tenha sido removido
	if( proxChar(F) == '*' ) {
		return NULL;
	}
	else {
		// Le os primeiros 8 bytes do registro e salva em codEscola
		char* ajuda = malloc(9); // Vetor de 9 bytes (8 + '\0') para auxiliar na leitura de inteiros 
		int read;
		if( fread(ajuda, 8, 1, F) != 1) {
			free(aux);
			return NULL;
		}
		ajuda[8] = '\0';
		aux->codEscola = atoi(ajuda);	

		// Le a string dataInicio (tamanho fixo de 10 caracteres):
		fread(aux->dataInicio, 1, 10, F);	// Le dataInicio (string de 10 elementos) 
		aux->dataInicio[10] = '\0';			// Para evitar problemas


		// Le a string dataFinal (tamanho fixo de 10 caracteres):
		fread(aux->dataFinal, 1, 10, F);	// Le dataFinal (string de 10 elementos) 
		aux->dataFinal[10] = '\0';			// Para evitar problemas

		// Le a string nomeEscola e seta seu tamanho:
		fread(ajuda, 2, 1, F);
		ajuda[2] = '\0';
		read = atoi(ajuda);	
		if(read == 0) {
			// Caso esteja vazia, seta o tamanho da string como tendo tamanho 0 e valor nulo
			aux->nomeEscola.tamanho = 0;
			aux->nomeEscola.valor = malloc(1);
			aux->nomeEscola.valor = "\0";
		}
		else {
			// Caso nao esteja vazio, le a string e seta seu tamanho
			aux->nomeEscola.valor = malloc(read+1);
			fread(aux->nomeEscola.valor, 1, read, F);
			aux->nomeEscola.valor[read] = '\0';
			aux->nomeEscola.tamanho = strlen(aux->nomeEscola.valor);

			// Caso a string seja maior que 26 bytes
			if(read >= 26) {
				aux->nomeEscola.valor[25] = '\0';
				aux->nomeEscola.tamanho = 26;
			}
		}	

		// Encontra a proxima string
		char lixo;
		while( (proxChar(F) == 0) || (proxChar(F) == ' ') ) {
			fread(&lixo,1,1,F);
		}

		// Le a string municipio e seta seu tamanho:
		fread(ajuda, 2, 1, F);
		ajuda[2] = '\0';
		read = atoi(ajuda);	
		if(read == 0) {
			// Caso esteja vazia, seta o tamanho da string como tendo tamanho 0 e valor nulo
			aux->municipio.tamanho = 0;
			aux->municipio.valor = malloc(1);		
			aux->municipio.valor = "\0";
		}
		else {
			aux->municipio.valor = malloc(read+1);
			fread(aux->municipio.valor, 1, read, F);
			aux->municipio.valor[read] = '\0';
			aux->municipio.tamanho = strlen(aux->municipio.valor);
		
			// Caso a string seja maior que 26 bytes
			if(read >= 26) {
				aux->municipio.valor[25] = '\0';
				aux->municipio.tamanho = 26;
			}

		}

		// Encontra o a proxima string
		while( (proxChar(F) == 0) || (proxChar(F) == ' ') ) {
			fread(&lixo,1,1,F);
		}

		// Le a string endereco e seta seu tamanho:
		fread(ajuda, 2, 1, F);
		ajuda[2] = '\0';
		read = atoi(ajuda);	
		if(read == 0) {
			// Caso esteja vazia, seta o tamanho da string como tendo tamanho 0 e valor nulo
			aux->endereco.tamanho = 0;
			aux->endereco.valor = malloc(1);		
			aux->endereco.valor = "\0";
		}
		else {
			aux->endereco.valor = malloc(read+1);
			fread(aux->endereco.valor, 1, read, F);
			aux->endereco.valor[read] = '\0';
			aux->endereco.tamanho = strlen(aux->endereco.valor);
		
			// Caso a string seja maior que 26 bytes
			if(read >= 26) {
				aux->endereco.valor[25] = '\0';
				aux->endereco.tamanho = 26;
			}

		}

		return aux;
	}
}   

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

		if( registroAux != NULL) {
		
			// Acrescenta o registro lido ao vetor de registros,
			// incrementando seu tamanho, na ultima posicao.
				
			// Incrementa o tamanho do vetor e aloca a memoria de seus ponteiros
			R->tam++;
			R->vet = realloc(R->vet, R->tam*sizeof(registro) );
			
			// Copia os dados da variavel auxiliar para o ultimo endereco do vetor
			R->vet[R->tam-1].codEscola = registroAux->codEscola;
			
			R->vet[R->tam-1].nomeEscola.tamanho = registroAux->nomeEscola.tamanho;
			R->vet[R->tam-1].nomeEscola.valor = malloc(registroAux->nomeEscola.tamanho * sizeof(char));
			strncpy(R->vet[R->tam-1].nomeEscola.valor, registroAux->nomeEscola.valor, registroAux->nomeEscola.tamanho);
			
			R->vet[R->tam-1].municipio.tamanho = registroAux->municipio.tamanho;
			R->vet[R->tam-1].municipio.valor = malloc(registroAux->municipio.tamanho * sizeof(char));
			strncpy(R->vet[R->tam-1].municipio.valor, registroAux->municipio.valor, registroAux->municipio.tamanho);
			
			R->vet[R->tam-1].endereco.tamanho = registroAux->endereco.tamanho;
			R->vet[R->tam-1].endereco.valor = malloc(registroAux->endereco.tamanho * sizeof(char));
			strncpy(R->vet[R->tam-1].endereco.valor, registroAux->endereco.valor, registroAux->endereco.tamanho);
			
			strncpy(R->vet[R->tam-1].dataInicio, registroAux->dataInicio, 11);
			strncpy(R->vet[R->tam-1].dataFinal, registroAux->dataFinal, 11);
		}

	} while(!feof(F));

}

void buscaRegistro(FILE* fileDados, int op, char* arg) {

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

	switch(op) {

		case 0:	// Busca 'nula', printa todos

			// Passa por todos os registros e, caso encontre um correspondente a busca, o printa
			do {
			    // Carregamento do arquivo binario para a memoria principal
			    fileDados = fopen("fileOut", "rb");
			    if(fileDados == NULL) {
			        printf("ERRO: Arquivo de dados nao encontrado.");
			        return;
			    }

			    // Econtra a posicao para o registro atual
			    fseek(fileDados, ( (i++)*(TAMANHO_REGISTRO) ) +5, SEEK_SET);

			    // Condicao para caso o arquivo tenha sido removido
			    if( proxChar(fileDados) == '*' ) {
			    	fclose(fileDados);
			    	continue;
			    }

			    // Le um registro do arquivo
			    reg = leRegistroBin(fileDados);
			    
			    // Printa o registro
			    if(reg != NULL) { // Caso tenha-se chego ao final do arquivo, a funcao leRegistroBin() retorna NULL
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
			    if(fileDados == NULL) {
			        printf("ERRO: Arquivo de dados nao encontrado.");
			        return;
			    }

			    // Econtra a posicao para o registro atual
			    fseek(fileDados, ( (i++)*(TAMANHO_REGISTRO) )+5, SEEK_SET);
			    
			    // Condicao para caso o arquivo tenha sido removido
			    if( proxChar(fileDados) == '*' ) {
			    	fclose(fileDados);
			    	continue;
			    }

			    // Le um registro do arquivo
			    reg = leRegistroBin(fileDados);
			    
			    // Printa o registro
			    if(reg != NULL) { // Caso tenha-se chego ao final do arquivo, a funcao leRegistroBin() retorna NULL
			  
			  		if( reg->codEscola == atoi(arg) ) {
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
			    if(fileDados == NULL) {
			        printf("ERRO: Arquivo de dados nao encontrado.");
			        return;
			    }

			    // Econtra a posicao para o registro atual
			    fseek(fileDados, ( (i++)*(TAMANHO_REGISTRO) )+5, SEEK_SET);

			    // Condicao para caso o arquivo tenha sido removido
			    if( proxChar(fileDados) == '*' ) {
			    	fclose(fileDados);
			    	continue;
			    }
			    
			    // Le um registro do arquivo
			    reg = leRegistroBin(fileDados);
			    
			    // Printa o registro
			    if(reg != NULL) { // Caso tenha-se chego ao final do arquivo, a funcao leRegistroBin() retorna NULL
			  
			  		if( strstr(reg->dataInicio, arg) !=  NULL ) {
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
			    if(fileDados == NULL) {
			        printf("ERRO: Arquivo de dados nao encontrado.");
			        return;
			    }

			    // Econtra a posicao para o registro atual
			    fseek(fileDados, ( (i++)*(TAMANHO_REGISTRO) )+5, SEEK_SET);
			    
			    // Condicao para caso o arquivo tenha sido removido
			    if( proxChar(fileDados) == '*' ) {
			    	fclose(fileDados);
			    	continue;
			    }

			    // Le um registro do arquivo
			    reg = leRegistroBin(fileDados);
			    
			    // Printa o registro
			    if(reg != NULL) { // Caso tenha-se chego ao final do arquivo, a funcao leRegistroBin() retorna NULL
			  
			  		if( strstr(reg->dataFinal, arg) != NULL ) {
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
			    if(fileDados == NULL) {
			        printf("ERRO: Arquivo de dados nao encontrado.");
			        return;
			    }

			    // Econtra a posicao para o registro atual
			    fseek(fileDados, ( (i++)*(TAMANHO_REGISTRO) )+5, SEEK_SET);
			    
			    // Condicao para caso o arquivo tenha sido removido
			    if( proxChar(fileDados) == '*' ) {
			    	fclose(fileDados);
			    	continue;
			    }
			    
			    // Le um registro do arquivo
			    reg = leRegistroBin(fileDados);
			    
			    // Printa o registro
			    if(reg != NULL) { // Caso tenha-se chego ao final do arquivo, a funcao leRegistroBin() retorna NULL
			  
			  		if( strstr(reg->nomeEscola.valor, maiusculoStr(arg)) != 0 ) {
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
			    if(fileDados == NULL) {
			        printf("ERRO: Arquivo de dados nao encontrado.");
			        return;
			    }

			    // Econtra a posicao para o registro atual
			    fseek(fileDados, ( (i++)*(TAMANHO_REGISTRO) ) +5, SEEK_SET);

			    // Condicao para caso o arquivo tenha sido removido
			    if( proxChar(fileDados) == '*' ) {
			    	fclose(fileDados);
			    	continue;
			    }
			    
			    // Le um registro do arquivo
			    reg = leRegistroBin(fileDados);
			    
			    // Printa o registro
			    if(reg != NULL) { // Caso tenha-se chego ao final do arquivo, a funcao leRegistroBin() retorna NULL
			  
			  		if( strstr(reg->municipio.valor, maiusculoStr(arg)) != NULL ) {
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
			    if(fileDados == NULL) {
			        printf("ERRO: Arquivo de dados nao encontrado.");
			        return;
			    }

			    // Econtra a posicao para o registro atual
			    fseek(fileDados, ( (i++)*(TAMANHO_REGISTRO) )+5, SEEK_SET);
			    
			    // Condicao para caso o arquivo tenha sido removido
			    if( proxChar(fileDados) == '*' ) {
			    	fclose(fileDados);
			    	continue;
			    }
			    
			    // Le um registro do arquivo
			    reg = leRegistroBin(fileDados);
			    
			    // Printa o registro
			    if(reg != NULL) { // Caso tenha-se chego ao final do arquivo, a funcao leRegistroBin() retorna NULL
			  
			  		if( strstr(reg->endereco.valor, maiusculoStr(arg)) != NULL ) {
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
		    if(fileDados == NULL) {
		        printf("ERRO: Arquivo de dados nao encontrado.");
		        return;
		    }

		    int rrn = atoi(arg);

			// Descobre o numero de registros no arquivo de dado
			fseek(fileDados, 0L, SEEK_END);
			int tamanhoArquivo = ftell(fileDados);
			int qtdRegistros = ( (tamanhoArquivo-5) / TAMANHO_REGISTRO);
			fseek(fileDados, ( (rrn-1)*(TAMANHO_REGISTRO) )+ 5, SEEK_SET);

			// Verificacao da existencia do registro no arquivo de dados
			if( (proxChar(fileDados) == '*') || (rrn < 1) || (rrn > qtdRegistros) ) {
		    	printf("ERRO: Registro inexistente.\n");
		    	return;
		    }
		    else {
			    // Econtra a posicao para o registro atual
			    fseek(fileDados, ( (rrn-1)*TAMANHO_REGISTRO )+5, SEEK_SET);

			    // Condicao para caso o arquivo tenha sido removido
			    if( proxChar(fileDados) == '*' ) {
			    	fclose(fileDados);
			    	break;
			    }
				    
			    // Le um registro do arquivo
			    reg = leRegistroBin(fileDados);
			    
			    // Printa o registro
			    if(reg != NULL) { // Caso tenha-se chego ao final do arquivo, a funcao leRegistroBin() retorna NULL
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
	if(qtdEncontrados == 0) 
		printf("Registro inexistente.\n");

}

void removeRegistro(FILE* F, char* argRrn) {

	// Abre o arquivo de dados
	byte status = 0;

    F = fopen("fileOut", "r+");
    if(F == NULL) {
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
	fseek(F, 0L, SEEK_END);
	int tamanhoArquivo = ftell(F);
	int qtdRegistros = ( (tamanhoArquivo-5) / TAMANHO_REGISTRO);
	fseek(F, ( (rrn-1)*(TAMANHO_REGISTRO) )+ 5, SEEK_SET);

	// Verificacao da existencia do registro no arquivo de dados
	if( (proxChar(F) == '*') || (rrn < 1) || (rrn > qtdRegistros) ) {
		
		printf("ERRO: Registro inexistente\n");
	}
	else {

		// Le o topo da pilha do cabecalho
		int topoPilha;
		fseek(F, 1, SEEK_SET);
		fread(&topoPilha, sizeof(int), 1, F);

		// Encontra o byte offset do registro a ser removido
		fseek(F, ( (rrn-1)*(TAMANHO_REGISTRO) )+ 5, SEEK_SET);
		fprintf(F, "%c", '*');
		fwrite(&topoPilha, sizeof(int), 1, F);
		
		// Seta o topo da pilha no cabecalho (rrn do registro removido nesta instancia da funcao)
		fseek(F, 1, SEEK_SET);
		fwrite(&rrn , sizeof(int), 1, F);
		printf("Registro removido com sucesso.\n");
	}

	// Procedimento de fechamento do arquivo
	status = 1;
	fseek(F, 0, SEEK_SET);
	fwrite(&status , 1, 1, F);
	fclose(F);
}

void insercaoDinamica(FILE* F, registro* R) {

	// Abre o arquivo de dados
	byte status = 0;

    F = fopen("fileOut", "r+");
    if(F == NULL) {
    	// Caso o arquivo nao exista
        printf("ERRO: Arquivo de dados nao encontrado.");
        return;
    }
    else {
    	// Caso exista, seta seu status (na cabecalho) como 0
    	fseek(F, 0, SEEK_SET);
		fwrite(&status , 1, 1, F);
    }

    // Le topo da pilha do cabecalho
    int topoPilha;
    fseek(F, 1, SEEK_SET);
    fread(&topoPilha, sizeof(int), 1, F);

    // Caso a pilha esteja vazia, escreve o registro no final do arquivo
    if(topoPilha == -1) {
    	fseek(F, 0, SEEK_END);
    	escreveRegistro(F, R);
    }
    else {	// Caso a pilha nao esteja vazia:
   		
    	int rrn = topoPilha;

    	// Le o proximo topoPilha, do RRN correto
    	fseek(F, ( (rrn-1)*TAMANHO_REGISTRO )+6, SEEK_SET);
	    fread(&topoPilha, sizeof(int), 1, F);
   		
   		// Escreve o novo topoPilha, lido anteriormente, no cabecalho
   		fseek(F, 1, SEEK_SET);
	    fwrite(&topoPilha, sizeof(int), 1, F);
    
    	// Escreve o novo registro no RRN lido do topo da pilha
		fseek(F, ( (rrn-1)*TAMANHO_REGISTRO )+5, SEEK_SET);
		escreveRegistro(F, R);
    }

	// Procedimento de fechamento do arquivo
	status = 1;
	fseek(F, 0, SEEK_SET);
	fwrite(&status , 1, 1, F);
	fclose(F);
}