#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/msg.h>
#include <stdbool.h>

#define TAM_PALAVRA 512
#define TAM_MUSICA 2056

typedef struct Musica{
    long int tipo_mensagem;
    char nome[TAM_PALAVRA];
    char autor[TAM_PALAVRA];
    char genero[TAM_PALAVRA];
    char duracao[TAM_PALAVRA];
} Musica;


int id_mensagem;
bool rodando;

void criaEspacoCompartilhamento(){
    id_mensagem = msgget((key_t)1234, 0666 | IPC_CREAT);

    if (id_mensagem == -1) {
        fprintf(stderr, "Erro na criacao de espaco de troca de mensagens: %d\n", errno);
        exit(EXIT_FAILURE);
    }
}
void consome(){
    rodando = true;
    struct Musica musica;
    long int msg_to_receive = 0;

    while(rodando){
        printf("Aguardando musicas .... \n");
        if (msgrcv(id_mensagem, (void *)&musica, TAM_MUSICA,
                   msg_to_receive, 0) == -1) {
            fprintf(stderr, "Recebimento falhou com erro: %d\n", errno);
            exit(EXIT_FAILURE);
        }
        printf("O programa consumiu: \n Musica: %s \n Autor: %s \n Genero: %s \n Duracao: %s \n", musica.nome, musica.autor, musica.genero, musica.duracao);
        if (strncmp(musica.nome, "end", 3) == 0) {
            printf("Fim do loop\n");
            rodando = false;
        }
        sleep(3);
     }
}

void main(){
    criaEspacoCompartilhamento();
    consome();

    exit(EXIT_SUCCESS);

}