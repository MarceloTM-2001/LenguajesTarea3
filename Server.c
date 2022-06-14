//
// Created by Ashley Vásquez Concepción on 1/6/22.
//
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>


//Estructura bloques
struct bloque{
    int vida;
    int puntos;
    int power;
};

//---------------------Variables relevantes al juego
//Matriz bloque juegos
struct bloque grid [8][14];//Filas,Columnas
int cantidadBloques;
int barra;
int cantidadBolas;
int vidas;
int velocidadBolas;

void Initializegame(){
    int *ptrvidas;
    int *ptrbarra;
    int *ptrcantidadBloques;
    int *ptrcantidadBolas;
    int *ptrvelocidadBolas;

    ptrvidas=&vidas;
    ptrbarra=&barra;
    ptrvelocidadBolas=&velocidadBolas;
    ptrcantidadBloques=&cantidadBloques;
    ptrcantidadBolas=&cantidadBolas;

    *ptrvidas=3;
    *ptrbarra=140;
    *ptrcantidadBolas=1;
    *ptrcantidadBloques=8*14;
    *ptrvelocidadBolas=5;
    srand(time(NULL));
    Initializegrid();
}

void Initializegrid(){
    for(int i=0;i!=8;i++){
        for(int j=0;j!=14;j++){
            struct bloque Block;
            struct bloque *ptr;
            ptr=&Block;
            //Asignación de vida
            if(i<2){
                ptr->vida=4;
                ptr->puntos=4;
            }else if (i<4){
                ptr->vida=3;
                ptr->puntos=3;
            }else if (i<6){
                ptr->vida=2;
                ptr->puntos=2;
            }else if (i<8){
                ptr->vida=1;
                ptr->puntos=1;
            }
            //Asignación Poder (25% probabilidad de tener poder)
            int r = rand() % 4;
            if(r==0){
                ptr->power=0;//No contiene poder
            }else{
                int r2=rand() % 5;
                if(r2==0){
                    ptr->power=1;//Aumento barra
                } else if(r2==1){
                    ptr->power=2;//Disminución barra
                }else if(r2==2){
                    ptr->power=3;//Aumento velocidad
                }else if(r2==3){
                    ptr->power=4;//Disminución velocidad
                } else if(r2==4){
                    ptr->power=5;//Nueva pelota
                }
            }
            (grid[i][j])=*ptr;

        }
    }
}

void PrintSomething(int i,int j){
   struct bloque Block=grid[i][j];
   printf("%d,%d\n",i,j);
   printf("Vida=%d\n",Block.vida);
   printf("Power=%d\n",Block.power);
   printf("Puntos=%d\n",Block.puntos);
}

void Hitbloque(int rows,int columns){
    struct bloque *Block;
    Block=&(grid[rows][columns]);
    Block->vida--;
    if (Block->vida==0){
        HandleBlockdestruction(Block);
    }
}

void Barchange(int Change){
    if(Change==1){
        if(barra!=560){
            barra*=2;
        }
    }else{
        if(barra!=35){
            barra/=2;
        }
    }
}

void SpeedChange(int Change){

}

void LostBall(){
    cantidadBolas--;
    vidas--;
    if(vidas==0){
        Handleendgame(0);
    }
    if(cantidadBolas==0){
        Handleendgame(0);
    }
}

void HandleBlockdestruction(struct bloque *Block){
    cantidadBloques--;
    printf("Bloque destruido\n");
    if(cantidadBloques==0){
        Handleendgame(1);
    }if(Block->power==0){
        printf("No hay poder\n");
    }else if(Block->power==1){
        printf("Aumento Barra\n");
        Barchange(1);
    }else if(Block->power==2){
        printf("Disminuye Barra\n");
        Barchange(0);
    }else if(Block->power==3){
        printf("Aumento velocidad\n");
        SpeedChange(1);
    }else if(Block->power==4){
        printf("Disminución velocidad\n");
        SpeedChange(0);
    }else if(Block->power==5){
        printf("Añadir pelota\n");
        cantidadBolas++;
    }else{
        printf("This shouldn't happen, power problem\n");
    }
}

void Handleendgame(int Wincondition){
    printf("El juego terminó\n");
    Initializegrid();
    if(Wincondition==0){
        printf("Has Perdido\n");
    }else{
        printf("Siguiente ronda\n");
    }
}


int main( int argc, char *argv[] )
{
    int sockfd, clisockfd, portno;
    socklen_t clilen;
    char buffer[256];
    char contentBuffer[255];
    struct sockaddr_in serv_addr, cli_addr;


    /* First call to socket() function */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("ERROR opening socket");
        return(1);
    }

    /* Initialize socket structure */
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = 5566;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);


    if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
    {
        perror("ERROR on binding");
        return(1);
    }

    listen(sockfd,5);
    printf("listening...\n");
    clilen = (socklen_t) sizeof(cli_addr);

    clisockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);

    if (clisockfd < 0)
    {
        perror("ERROR on accept");
        return(1);
    }


    while (1) {
        bzero(buffer, 256);
        bzero(contentBuffer, 255);
        printf("[+]Client connected.\n");

        /* If connection is established then start communicating */

        send(clisockfd, "Hola\n", 13, 0);
        (recv(clisockfd, buffer, 100, 0));
        printf("Client: %s\n", buffer);
        break;
    }}
