// Projeto pratico (parte 2) da disciplina SCC0215012018 - Organizacao de Arquivos
// Pedro Pastorello Fernandes - NUSP 10262502

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "manipulacaoArquivos.h"
#include "indice.h"

int main(int argc, char* argv[]) {

    // Referencias
    FILE* fileDados;
    registro* reg;
    buffer* bufferPaginas;

    if (argc <= 1) {
        printf("ERRO: Parametros ausentes!\n");
        exit(1);
    }

    switch ( atoi(argv[1]) ) {

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

            /* Cria o buffer de paginas que sera utilizado pelas funcoes que
             * trabalham com o arquivo de indices */
            bufferPaginas = criaBuffer();

            // Cria o arquivo de indices para o arquivo de dados
            criaArquivoIndice("fileIndex");

            // Escreve os registros do vetor de dados no arquivo de saida
            escreveArquivo(dadosIn, "fileOut", bufferPaginas);


            // Finalizacao da execucao
            bufferLogOutput(bufferPaginas);
            limpaBuffer(bufferPaginas); 
            printf("Arquivo carregado.\n");
            break;

        case 2: // Recuperacao de dados ( ./programaTrab1 2 )

            // Checa se o numero de parametros esta de acordo com o esperado
            checkParametros(argc, 2);

            buscaRegistro(0, "");

            break;

        case 3: // Busca de registros ( ./programaTrab1 3 'NomeDoCampo' valor )

            // Checa se o numero de parametros esta de acordo com o esperado
            checkParametros(argc, 4);

            // String de argumento para a busca
            char* argBusca = malloc(27);
            strncpy(argBusca, argv[3], 26);
            argBusca[26] = '\0';

            if(strcmp(argv[2], "codEscola") == 0) {
                buscaRegistro(1, argBusca);
                break;
            }

            if(strcmp(argv[2], "dataInicio") == 0) {
                buscaRegistro(2, argBusca);
                break;                
            }

            if(strcmp(argv[2], "dataFinal") == 0) {
                buscaRegistro(3, argBusca);
                break;                
            }

            if(strcmp(argv[2], "nomeEscola") == 0) {
                buscaRegistro(4, argBusca);
                break;
            }

            if(strcmp(argv[2], "municipio") == 0) {
                buscaRegistro(5, argBusca);
                break;
            }
            
            if(strcmp(argv[2], "endereco") == 0) {
                buscaRegistro(6, argBusca);
                break;                
            }

            printf("ERRO: Campo invalido!\n");
            exit(0);
            
            break;

        case 4: // Busca por RRN ( ./programaTrab1 4 RRN )

            // Checa se o numero de parametros esta de acordo com o esperado
            checkParametros(argc, 3);

            buscaRegistro(7, argv[2]);

            break;

        case 5: // Remocao logica de registros ( ./programaTrab1 5 RRN )

            // Checa se o numero de parametros esta de acordo com o esperado
            checkParametros(argc, 3);

            removeRegistro(argv[2]);
            
            break;

        case 6: // Insercao dinamica ( ./programaTrab1 6 valorCampo1 valorCampo2 valorCampo3 valorCampo4 valorCampo5 valorCampo6 )

            // Checa se o numero de parametros esta de acordo com o esperado
            checkParametros(argc, 8);

            // Registro auxiliar com as informações entradas
            reg = registroEntrada(argv);

            /* Cria o buffer de paginas que sera utilizado pelas funcoes que
             * trabalham com o arquivo de indices */
            bufferPaginas = criaBuffer();

            /* Insere dinamicamente o registro no arquivo de dados e seu elemento correspondente
             * no arquivo de indices */
            insercaoDinamica(reg, bufferPaginas);

            // Finalizacao da execucao
            bufferLogOutput(bufferPaginas);
            limpaBuffer(bufferPaginas); 
            printf("Registro alterado com sucesso.\n");
            break;

        case 7: // Atualizacao de registro por RRN ( ./programaTrab1 7 RRN valorCampo1 valorCampo2 valorCampo3 valorCampo4 valorCampo5 valorCampo6 )
    
            fileDados = fopen("fileDados","rb+");
            if (fileDados == NULL) {
                printf("ERRO: Arquivo de dados nao encontrado.\n");
                exit(0);
            }

            // Checa se o numero de parametros esta de acordo com o esperado
            checkParametros(argc, 9);

            // RRN do registro a ser alterado
            int argRrn = atoi(argv[2]);

            // Descobre o numero de registros no arquivo de dados
            int qtdRegistros = numeroRegistros(fileDados);

            fseek(fileDados, argRrn*TAMANHO_REGISTRO +5, SEEK_SET);
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
            fileDados = fopen("fileOut", "rb+");
            if(fileDados == NULL) {
                printf("ERRO: Arquivo de dados nao encontrado.");
                exit(0);
            }
            else {
                // Caso exista, seta seu status (no cabecalho) como 0
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
            
            /* Cria o buffer de paginas que sera utilizado pelas funcoes que
             * trabalham com o arquivo de indices */
            bufferPaginas = criaBuffer();

            // Escreve os dados no novo arquivo
            escreveArquivo(dadosBin, "tempData", bufferPaginas);

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
            
            // Escreve as informacoes referentes ao buffer nessa execucao do programa
            bufferLogOutput(bufferPaginas);

            // Limpa o buffer de paginas
            limpaBuffer(bufferPaginas); 

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

            // Checa se o numero de parametros esta de acordo com o esperado
            checkParametros(argc, 3);

            // Trata o parametro de entrada (de string para inteiro)
            int chaveBusca = atoi(argv[2]);

            // Abre o arquivo de dados para leitura
            FILE* I = fopen("fileIndex", "rb");

            /* Cria o buffer de paginas que sera utilizado pelas funcoes que
             * trabalham com o arquivo de indices */
            bufferPaginas = criaBuffer();

            // Le o RRN da pagina raiz do cabecalho do arquivo de indices (necessario para inicar a busca)
            int rrnRaiz = leCabecalhoArquivoIndice(I, "noRaiz");

            // Realiza a busca pelo RRN no arquivo de indices arvore b
            int rrnArquivoDados = buscaIndice(I, bufferPaginas, rrnRaiz, chaveBusca);

            // Truncamento de dados pois a pesquisa usa string
            char busca[12];
            sprintf(busca, "%d", rrnArquivoDados);

            // Realiza busca (e impressao) do registro no arquivo de dados utilizando o RRN
            buscaRegistro(7, busca);

            // Finalizacao da execucao
            bufferLogOutput(bufferPaginas);
            limpaBuffer(bufferPaginas); 
            fclose(I);
            break;

        default:
            break;
    }
    
    return 0;
}
