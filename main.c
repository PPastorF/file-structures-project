// Projeto pratico da disciplina SCC0215012018 - Organizacao de Arquivos
// Pedro Pastorello Fernandes - NUSP 10262502

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "manipulacaoArquivos.h"

int main(int argc, char *argv[]) {

    // Referencias
    registro* reg = malloc(sizeof(registro) );
    FILE* fileDados;
    FILE* fileOut;

    // Definicao de uma string nula, de tamanho 10 para ser utilizada futuramente nos campos de tamanho fixo
    char nada[11] = {'0','0','0','0','0','0','0','0','0','0','\0'};

    // Caso o usuario nao entre com parametros
    if(argc == 1){
        printf("ERRO: Parametros ausentes!\n", argc);
        return -1;
    }
    
    switch( atoi(argv[1]) ) {

        case 1: // Leitura de registros ( ./programa 1 'arquivo.csv' )

            // Caso o usuario entre com o numero errado de parametros
            if(argc != 3){
                printf("ERRO: Numero de parametros invalido!\n", argc);
                return -1;
            }

            // Carregamento do arquivo .csv para a memoria principal
            FILE* fileIn;
            fileIn = fopen(argv[2], "r");
            if(fileIn == NULL) {
                printf("ERRO: O arquivo '%s' nao foi encontrado.\n", argv[2]);
                return -1;
            }

            // Vetor que armazena os registros carregados do arquivo fileIn
            registroV* dadosIn = malloc(sizeof(registroV) );
            dadosIn->tam = 0;
            dadosIn->vet = malloc(sizeof(registro) );

            // Le os registros do arquivo e armazena-os no vetor de dados. Depois, fecha-o
            leArquivoCsv(fileIn, dadosIn);
            
            fclose(fileIn);

            // Escreve os registros do vetor de dados no arquivo de saida
            escreveArquivo(fileOut, dadosIn);

            printf("Arquivo carregado.\n");

            break;

        case 2: // Recuperacao de dados ( ./programaTrab1 2 )

            // Caso o usuario entre com o numero errado de parametros
            if(argc != 2){
                printf("ERRO: Numero de parametros invalido!\n", argc);
                return -1;
            }

            buscaRegistro(fileDados, 0, "");

            break;

        case 3: // Busca de registros ( ./programaTrab1 3 'NomeDoCampo' valor )

            // Caso o usuario entre com o numero errado de parametros
            if(argc != 4){
                printf("ERRO: Numero de parametros invalido!\n", argc);
                return -1;
            }

            // String de argumento para a busca
            char* argBusca = malloc(27);
            strncpy(argBusca, argv[3], 26);
            argBusca[26] = '\0';

            if(strcmp(argv[2], "codEscola") == 0) {
                buscaRegistro(fileDados, 1, argBusca);
                break;
            }

            if(strcmp(argv[2], "dataInicio") == 0) {
                buscaRegistro(fileDados, 2, argBusca);
                break;                
            }

            if(strcmp(argv[2], "dataFinal") == 0) {
                buscaRegistro(fileDados, 3, argBusca);
                break;                
            }

            if(strcmp(argv[2], "nomeEscola") == 0) {
                buscaRegistro(fileDados, 4, argBusca);
                break;
            }

            if(strcmp(argv[2], "municipio") == 0) {
                buscaRegistro(fileDados, 5, argBusca);
                break;
            }
            
            if(strcmp(argv[2], "endereco") == 0) {
                buscaRegistro(fileDados, 6, argBusca);
                break;                
            }

            printf("ERRO: Campo invalido!\n");
            return -1;
            
            break;

        case 4: // Busca por RRN ( ./programaTrab1 4 RRN )

            // Caso o usuario entre com o numero errado de parametros
            if(argc != 3){
                printf("ERRO: Numero de parametros invalido!\n", argc);
                return -1;
            }

            buscaRegistro(fileDados, 7, argv[2]);

            break;

        case 5: // Remocao logica de registros ( ./programaTrab1 5 RRN )

            // Caso o usuario entre com o numero errado de parametros
            if(argc != 3){
                printf("ERRO: Numero de parametros invalido!\n", argc);
                return -1;
            }

            removeRegistro(fileDados, argv[2]);

            break;

        case 6: // Insercao dinamica ( ./programaTrab1 6 valorCampo1 valorCampo2 valorCampo3 valorCampo4 valorCampo5 valorCampo6 )

            // Caso o usuario entre com o numero errado de parametros
            if(argc != 8){
                printf("ERRO: Numero de parametros invalido!\n", argc);
                return -1;
            }

            // Atualiza o registro auxiliar com as informações entradas
            reg->codEscola = atoi(argv[2]);

            // Caso a entrada seja 0, guarda uma string nula
            if(strcmp(argv[3], "0") == 0)
                strcpy(reg->dataInicio, nada);
            else
                strcpy(reg->dataInicio, argv[3]);

            if(strcmp(argv[4], "0") == 0)
                strcpy(reg->dataFinal, nada);
            else
                strcpy(reg->dataInicio, argv[4]);

            if(strlen(argv[5]) == 0) {
                reg->endereco.tamanho = 1;
                reg->municipio.valor   = malloc(1);
                strcpy(reg->nomeEscola.valor, "\0");
            }
            else {
                reg->nomeEscola.tamanho = strlen(argv[5]);
                reg->nomeEscola.valor   = malloc(reg->nomeEscola.tamanho+1);
                strcpy(reg->nomeEscola.valor, argv[5]);
            }
            if(strlen(argv[6]) == 0) {
                reg->municipio.tamanho = 1;
                reg->municipio.valor   = malloc(1);
                strcpy(reg->municipio.valor, "\0");
            }
            else {
                reg->municipio.tamanho = strlen(argv[6]);
                reg->municipio.valor   = malloc(reg->municipio.tamanho+1);
                strcpy(reg->municipio.valor, argv[6]);
            }
            
            if(strlen(argv[7]) == 0) {
                reg->endereco.tamanho = 1;
                reg->endereco.valor   = malloc(1);
                strcpy(reg->endereco.valor, "\0");
            }
            else {
                reg->endereco.tamanho = strlen(argv[7]);
                reg->endereco.valor   = malloc(reg->endereco.tamanho+1);
                strcpy(reg->endereco.valor, argv[7]);
            }

            insercaoDinamica(fileDados, reg);
            printf("Registro alterado com sucesso.\n");

            break;

        case 7: // Atualizacao de registro por RRN ( ./programaTrab1 7 RRN valorCampo1 valorCampo2 valorCampo3 valorCampo4 valorCampo5 valorCampo6 )

            // Caso o usuario entre com o numero errado de parametros
            if(argc != 9) {
                printf("ERRO: Numero de parametros invalido!\n", argc);
                return -1;
            }
            // RRN do registro a ser alterado
            int argRrn = atoi(argv[2]);

            // Descobre o numero de registros no arquivo de dado
            fseek(fileDados, 0L, SEEK_END);
            int tamanhoArquivo = ftell(fileDados);
            int qtdRegistros = ( (tamanhoArquivo-5) / TAMANHO_REGISTRO);
            fseek(fileDados, ( (argRrn-1)*(TAMANHO_REGISTRO) )+5, SEEK_SET);

            // Verificacao da existencia do registro no arquivo de dados
            if( (proxChar(fileDados) == '*') || (argRrn < 1) || (argRrn > qtdRegistros) ) {
                printf("ERRO: Registro inexistente.\n");
                return -1;
            }
            // Atualiza o registro auxiliar com as informações entradas
            reg->codEscola = atoi(argv[3]);

            // Caso a entrada seja 0, guarda uma string nula
            if(strcmp(argv[4], "0") == 0)
                strcpy(reg->dataInicio, nada);
            else
                strcpy(reg->dataInicio, argv[4]);

            if(strcmp(argv[5], "0") == 0)
                strcpy(reg->dataFinal, nada);
            else
                strcpy(reg->dataInicio, argv[5]);

            if(strlen(argv[6]) == 0) {
                reg->endereco.tamanho = 1;
                reg->municipio.valor   = malloc(1);
                strcpy(reg->nomeEscola.valor, "\0");
            }
            else {
                reg->nomeEscola.tamanho = strlen(argv[6]);
                reg->nomeEscola.valor   = malloc(reg->nomeEscola.tamanho+1);
                strcpy(reg->nomeEscola.valor, argv[6]);
            }
            if(strlen(argv[7]) == 0) {
                reg->municipio.tamanho = 1;
                reg->municipio.valor   = malloc(1);
                strcpy(reg->municipio.valor, "\0");
            }
            else {
                reg->municipio.tamanho = strlen(argv[7]);
                reg->municipio.valor   = malloc(reg->municipio.tamanho+1);
                strcpy(reg->municipio.valor, argv[7]);
            }
            
            if(strlen(argv[8]) == 0) {
                reg->endereco.tamanho = 1;
                reg->endereco.valor   = malloc(1);
                strcpy(reg->endereco.valor, "\0");
            }
            else {
                reg->endereco.tamanho = strlen(argv[8]);
                reg->endereco.valor   = malloc(reg->endereco.tamanho+1);
                strcpy(reg->endereco.valor, argv[8]);
            }
            
            // Variavel status para o cabecalho
            byte status = 0;

            // Carregamento do arquivo binario para a memoria principal
            fileDados = fopen("fileOut", "r+");
            if(fileDados == NULL) {
                printf("ERRO: Arquivo de dados nao encontrado.");
                return -1;
            }
            else {
                // Caso exista, seta seu status (na cabecalho) como 0
                fseek(fileDados, 0, SEEK_SET);
                fwrite(&status , 1, 1, fileDados);
            }

            // Econtra a posicao para o registro a ser escrito, baseado no RRN passado ao programa como argumento
            fseek(fileDados, ( (argRrn-1)*(TAMANHO_REGISTRO) )+5, SEEK_SET);

            // Escreve o registro
            escreveRegistro(fileDados, reg);

            // Procedimento de fechamento do arquivo
            status = 1;
            fseek(fileDados, 0, SEEK_SET);
            fwrite(&status , 1, 1, fileDados);
            fclose(fileDados);

            printf("Registro alterado com sucesso.\n");
            break;

        case 8: // Desfragmentacao ( ./programaTrab1 8 )

            // Caso o usuario entre com o numero errado de parametros
            if(argc != 2){
                printf("ERRO: Numero de parametros invalido!\n", argc);
                return -1;
            }

            // Carregamento do arquivo de dados para a memoria principal
            fileDados = fopen("fileOut", "rb");
            if(fileDados == NULL) {
                printf("ERRO: Arquivo de dados nao encontrado.\n");
                return -1;
            }

            // Vetor que armazena os registros carregados do arquivo fileIn
            registroV* dadosBin = malloc(sizeof(registroV) );
            dadosBin->tam = 0;
            dadosBin->vet = malloc(sizeof(registro) );

            // Le os registros do arquivo e armazena-os no vetor de dados. Depois, fecha-o
            leArquivoBin(fileDados, dadosBin);
            fclose(fileDados);

            // Escreve os registros do vetor de dados no arquivo de saida
            escreveArquivo(fileOut, dadosBin);

            printf("Arquivo de dados compactado com sucesso.\n");

    		break;

    	case 9:	// Printar pilha de RRN vazios ( ./programaTrab1 9 )

    		// Caso o usuario entre com o numero errado de parametros
    		if(argc != 2){
				printf("ERRO: Numero de parametros invalido!\n", argc);
				return -1;
    		}

            // Carregamento do arquivo binario para a memoria principal
            fileDados = fopen("fileOut", "rb");
            if(fileDados == NULL) {
                printf("ERRO: Arquivo de dados nao encontrado.");
                return -1;
            }

            // Le o topo da pilha
            int topoPilha;
            fseek(fileDados, 1, SEEK_SET);
            fread(&topoPilha, sizeof(int), 1, fileDados);
            if(topoPilha == -1) {
                printf("Pilha vazia.\n");
            }
            else {
                // Le os valores da pilha ate encontrar o valor -1 (pilha vazia)
                while (topoPilha != -1) {
                    printf("%d ", topoPilha);
                    
                    // Encontra o proximo valor de topoPilha, baseado no valor atual
                    fseek(fileDados, ( (topoPilha-1)*TAMANHO_REGISTRO )+6, SEEK_SET);
                    fread(&topoPilha, sizeof(int), 1, fileDados);
                }
                printf("\n");
            }

            // Fecha o arquivo
            fclose(fileDados);
    		break;
    }
    
    return 0;
}