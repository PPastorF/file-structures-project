// Projeto pratico (parte 2) da disciplina SCC0215012018 - Organizacao de Arquivos
// Pedro Pastorello Fernandes - NUSP 10262502

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "manipulacaoArquivos2.h"
#include "indice.h"
#include "buffer.h"

int main(int argc, char* argv[]) {

    // Referencias
    FILE* fileDados;
    FILE* fileOut;
    registro* reg;

    // Definicao de uma string nula, de tamanho 10 para ser utilizada futuramente nos campos de tamanho fixo
    char nada[11] = {'0','0','0','0','0','0','0','0','0','0','\0'};

    if(argc <= 1) {
        printf("ERRO: Parametros ausentes!\n");
        exit(1);
    }

    switch( atoi(argv[1]) ) {

        case 1: // Leitura de registros ( ./programa 1 'arquivo.csv' )

            // Checa se o numero de parametros esta de acordo com o esperado
            checkParametros(argc, 3);

            // Carregamento do arquivo .csv para a memoria principal
            FILE* fileIn;
            fileIn = fopen(argv[2], "r");
            if(fileIn == NULL) {
                printf("ERRO: O arquivo '%s' nao foi encontrado.\n", argv[2]);
                exit(0);
            }

            // Vetor que armazena os registros carregados do arquivo fileIn
            registroV* dadosIn = malloc(sizeof(registroV) );
            dadosIn->tam = 0;
            dadosIn->vet = malloc(sizeof(registro) );

            // Le os registros do arquivo e armazena-os no vetor de dados. Depois, fecha-o
            leArquivoCsv(fileIn, dadosIn);
            
            fclose(fileIn);

            // Escreve os registros do vetor de dados no arquivo de saida
            escreveArquivo(fileOut, dadosIn, "fileOut");   

            printf("Arquivo carregado.\n");

            break;

        case 2: // Recuperacao de dados ( ./programaTrab1 2 )

            // Checa se o numero de parametros esta de acordo com o esperado
            checkParametros(argc, 2);

            buscaRegistro(fileDados, 0, "");

            break;

        case 3: // Busca de registros ( ./programaTrab1 3 'NomeDoCampo' valor )

            // Checa se o numero de parametros esta de acordo com o esperado
            checkParametros(argc, 4);

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
            exit(0);
            
            break;

        case 4: // Busca por RRN ( ./programaTrab1 4 RRN )

            // Checa se o numero de parametros esta de acordo com o esperado
            checkParametros(argc, 3);

            buscaRegistro(fileDados, 7, argv[2]);

            break;

        case 5: // Remocao logica de registros ( ./programaTrab1 5 RRN )

            // Checa se o numero de parametros esta de acordo com o esperado
            checkParametros(argc, 3);

            removeRegistro(fileDados, argv[2]);
            
            break;

        case 6: // Insercao dinamica ( ./programaTrab1 6 valorCampo1 valorCampo2 valorCampo3 valorCampo4 valorCampo5 valorCampo6 )

            // Checa se o numero de parametros esta de acordo com o esperado
            checkParametros(argc, 8);

            // Registro auxiliar com as informações entradas
            reg = registroEntrada(argv);

            insercaoDinamica(fileDados, reg);

            printf("Registro alterado com sucesso.\n");
            break;

        case 7: // Atualizacao de registro por RRN ( ./programaTrab1 7 RRN valorCampo1 valorCampo2 valorCampo3 valorCampo4 valorCampo5 valorCampo6 )

            // Checa se o numero de parametros esta de acordo com o esperado
            checkParametros(argc, 9);

            // RRN do registro a ser alterado
            int argRrn = atoi(argv[2]);

            // Descobre o numero de registros no arquivo de dado
            int qtdRegistros = numeroRegistros(fileDados, argRrn);

            // Verificacao da existencia do registro no arquivo de dados
            if( (proxChar(fileDados) == '*') || (argRrn < 1) || (argRrn > qtdRegistros) ) {
                printf("ERRO: Registro inexistente.\n");
                exit(0);
            }

            // Registro auxiliar com as informações entradas
            reg = registroEntrada(argv);

            // Variavel status para o cabecalho
            byte status = 0;

            // Carregamento do arquivo binario para a memoria principal
            fileDados = fopen("fileOut", "r+");
            if(fileDados == NULL) {
                printf("ERRO: Arquivo de dados nao encontrado.");
                exit(0);
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

            fechaArquivo(fileDados);
            printf("Registro alterado com sucesso.\n");
            break;

        case 8: // Desfragmentacao ( ./programaTrab1 8 )

            // Checa se o numero de parametros esta de acordo com o esperado
            checkParametros(argc, 2);

            // Carregamento do arquivo de dados para a memoria principal
            fileDados = fopen("fileOut", "rb");
            if (fileDados == NULL) {
                printf("ERRO: Arquivo de dados nao encontrado.\n");
                exit(0);
            }

            // Vetor que armazena os registros carregados do arquivo fileIn
            registroV* dadosBin = malloc(sizeof(registroV) );
            dadosBin->tam = 0;
            dadosBin->vet = malloc(sizeof(registro) );

            // Le os registros do arquivo e armazena-os no vetor de dados. Depois, fecha-o
            leArquivoBin(fileDados, dadosBin);
            fclose(fileDados);

            // Cria um arquivo de dados temporario
            FILE* tempFile;

            // Escreve os dados no novo arquivo
            escreveArquivo(tempFile, dadosBin, "tempData");

            // Remove o antigo arquivo de dados
            if (remove("fileOut") == -1 ) {
                printf("ERRO: Falha na compactacao do arquivo de dados.\n");
                exit(1);
            }

            // Renomea o arquivo temporario para se tornar o novo arquivo de dados 
            if (rename("tempData", "fileOut") == -1 ) {
                printf("ERRO: Falha na compactacao do arquivo de dados.\n");
                exit(1);
            }
            
            printf("Arquivo de dados compactado com sucesso.\n");
    		break;

    	case 9:	// Printar pilha de RRN vazios ( ./programaTrab1 9 )

            // Checa se o numero de parametros esta de acordo com o esperado
            checkParametros(argc, 2);

            // Carregamento do arquivo binario para a memoria principal
            fileDados = fopen("fileOut", "rb");
            if(fileDados == NULL) {
                printf("ERRO: Arquivo de dados nao encontrado.");
                exit(0);
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
    
        case 12:

            break;

        case 13:

            break;

        case 14:

            break;

    }
    
    return 0;
}