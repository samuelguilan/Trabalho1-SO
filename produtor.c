#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/msg.h>

#define QTD_MUSICAS 12
#define TAM_PALAVRA 512

pthread_t trabalhador1, trabalhador2, trabalhador3, trabalhador4;
pthread_mutex_t trava;
int contador = 0;
int id_mensagem;
bool rodando = true;
struct Musica aSerEnviada;

typedef struct Musica{
    long int tipo_mensagem;
    char nome[TAM_PALAVRA];
    char autor[TAM_PALAVRA];
    char genero[TAM_PALAVRA];
    char duracao[TAM_PALAVRA];
} Musica;

struct Musica bliblioteca[] = {
    {.nome = "Get Loud For Me", .autor = "Gizzle", .genero = "Rock", .duracao = "3:01", .tipo_mensagem = 1 },
    {.nome = "Icky Thump", .autor = "The White Stripes", .genero = "Rock", .duracao = "4:15", .tipo_mensagem = 1},
    {.nome = "Thunderstruck", .autor = "ACDC", .genero = "Rock", .duracao = "4:53", .tipo_mensagem = 1 },
    {.nome = "The Less I Know The Better", .autor = "Tame Impala", .genero = "Indie", .duracao = "3:36", .tipo_mensagem = 1 },
    {.nome = "Gangsta's Paradise", .autor = "Coolio", .genero = "Rap", .duracao = "4:01", .tipo_mensagem = 1 },
    {.nome = "Till I Collapse", .autor = "Eminem", .genero = "Rap", .duracao = "4:58", .tipo_mensagem = 1 },
    {.nome = "Mount Everest", .autor = "Labrinth", .genero = "Outro", .duracao = "2:37", .tipo_mensagem = 1 },
    {.nome = "Take You Down", .autor = "Daniel Pemberton", .genero = "Rock", .duracao = "3:25", .tipo_mensagem = 1 },
    {.nome = "Last Breath", .autor = "Future", .genero = "Rap", .duracao = "4:03", .tipo_mensagem = 1 },
    {.nome = "Pensando Em Você", .autor = "Edy Lemond", .genero = "EletroFunk", .duracao = "3:27", .tipo_mensagem = 1 },
    {.nome = "X Gon' Give It to Ya", .autor = "DMX", .genero = "Rap", .duracao = "3:40", .tipo_mensagem = 1 },
    {.nome = "Immigrant Song", .autor = "Led Zeppelin", .genero = "Rock", .duracao = "2:27", .tipo_mensagem = 1 },
};

void *producao(){
    while(rodando){

        pthread_mutex_lock(&trava);
        aSerEnviada = bliblioteca[contador%QTD_MUSICAS];
        contador ++;
        //ENVIA MENSAGEM
        printf("Musica enviada: %s \n", aSerEnviada.nome);
        if (msgsnd(id_mensagem, (void *)&aSerEnviada, 2056, 0) == -1) {
            fprintf(stderr, "Envio de mensagem falhou\n");
            exit(EXIT_FAILURE);
        }
       
        pthread_mutex_unlock(&trava);

        sleep(7);

    }

}

int main(){
    pthread_mutex_init(&trava, NULL);
    
    pthread_create(&trabalhador1, NULL, producao, NULL) ;
    pthread_create(&trabalhador2, NULL, producao, NULL) ;
    pthread_create(&trabalhador3, NULL, producao, NULL) ;
    pthread_create(&trabalhador4, NULL, producao, NULL) ; 
    
    
    
    pthread_join(trabalhador1,NULL);  
    pthread_join(trabalhador2,NULL); 
    pthread_join(trabalhador3,NULL); 
    pthread_join(trabalhador4,NULL); 

    pthread_mutex_destroy(&trava);
    exit(EXIT_SUCCESS);
}