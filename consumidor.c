#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/msg.h>
#include musica.h



int id_mensagem;
boolean rodando;

void *criaEspacoCompartilhamento(){
    id_mensagem = msgget((key_t)1234, 0666 | IPC_CREAT);

    if (id_mensagem == -1) {
        fprintf(stderr, "Erro na criacao de caixa postal: %d\n", errno);
        exit(EXIT_FAILURE);
    }
}
void *consome(){
    rodando = true;
    struct Musica my_msg_st some_data;
    long int msg_to_receive = 0;

    while(rodando){
        if (msgrcv(id_mensagem, (void *)&some_data, BUFSIZ,
                   msg_to_receive, 0) == -1) {
            fprintf(stderr, "msgrcv failed with error: %d\n", errno);
            exit(EXIT_FAILURE);
        }
        printf("O programa consumiu: %s", some_data.some_text);
        if (strncmp(some_data.some_text, "end", 3) == 0) {
            rodando = false;
        }
     }
}

void main(){
    criaEspacoCompartilhamento;
    consome;

    exit(EXIT_SUCCESS);

}