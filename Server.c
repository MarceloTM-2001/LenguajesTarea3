//
// Created by Ashley Vásquez Concepción on 1/6/22.
//

#include "Server.h"


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
                int r2=rand() % 6;
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
                }else if(r2==5){
                    ptr->power=6;//Nueva vida
                }
            }
            (grid[i][j])=*ptr;
        }
    }
}

void Initializegame(int ajuste){
    int *ptrvidas;
    int *ptrbarra;
    int *ptrcantidadBloques;
    int *ptrcantidadBolas;
    int *ptrvelocidadBolas;
    int *ptringame;
    int *ptrajustevelocidad;



    ptrvidas=&vidas;
    ptrbarra=&barra;
    ptrvelocidadBolas=&velocidadBolas;
    ptrcantidadBloques=&cantidadBloques;
    ptrcantidadBolas=&cantidadBolas;
    ptringame=&ingame;
    ptrajustevelocidad=&ajustevelocidad;

    *ptringame=1;
    *ptrvidas=3;
    *ptrbarra=140;
    *ptrcantidadBolas=1;
    *ptrcantidadBloques=8*14;
    *ptrajustevelocidad=ajuste;
    *ptrvelocidadBolas=5+ajustevelocidad;

    Initializegrid();
}

void PrintSomething(int i,int j){
    struct bloque Block=grid[i][j];
    printf("%d,%d\n",i,j);
    printf("Vida=%d\n",Block.vida);
    printf("Power=%d\n",Block.power);
    printf("Puntos=%d\n",Block.puntos);
}

void Barchange(int change){
    if(change==1 && barra!=560){
        barra*=2;
        printf("Se aumentó la barra a %d\n",barra);
        send(clisockfd, "+B", 13, 0);
    }else if(change==0 && barra!=35){
        barra/=2;
        printf("Se disminuyó la barra a %d\n",barra);
        send(clisockfd, "-B", 13, 0);
    }else{
        printf("Hubo cambio de barra pero no se aplicó ya que está en el límite  B=%d\n",barra);
    }
}

void SpeedChange(int Change){
    if(Change==1 && velocidadBolas!=9){
        velocidadBolas+=2;
        printf("Se aumentó la velocidad a %d\n",velocidadBolas);
        send(clisockfd, "+V", 13, 0);
    }else if(Change==0 && velocidadBolas!=1){
        velocidadBolas-=2;
        printf("Se disminuyó la velocidad a %d\n",velocidadBolas);
        send(clisockfd, "-V", 13, 0);
    }else{
        printf("Hubo cambio de velocidad pero no se aplicó ya que la velocidad está en el límite V= %d\n",velocidadBolas);
    }
}

void Handleendgame(int Wincondition){
    printf("El juego terminó\n");
    if(Wincondition==0){
        ingame=0;
        printf("Has Perdido\n");
        send(clisockfd, "LG", 13, 0);
    }else{
        int ajuste;
        if(ajustevelocidad!=4){
            ajuste= ajustevelocidad+2;
        } else{
            ajuste=4;
        }
        Initializegame(ajuste);
        printf("Siguiente ronda\n");
        send(clisockfd, "WG", 13, 0);
    }
}

void LostBall(){
    cantidadBolas--;
    vidas--;
    send(clisockfd, "LL", 13, 0);
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
    char message[2];
    sprintf(message,"%d",Block->puntos);
    printf("%s",message);
    send(clisockfd, message, 13, 0);
    if(cantidadBloques==0){
        Handleendgame(1);
    }if(Block->power==0){
        printf("No hay poder\n");
    }else if(Block->power==1){
        Barchange(1);
    }else if(Block->power==2){
        Barchange(0);
    }else if(Block->power==3){
        SpeedChange(1);
    }else if(Block->power==4){
        SpeedChange(0);
    }else if(Block->power==5){
        printf("Añadir pelota\n");
        cantidadBolas++;
        send(clisockfd, "NB", 13, 0);
    }else if(Block->power==6){
        if(vidas!=3){
            vidas++;
            printf("Añadir Vida\n");
            send(clisockfd, "NL", 13, 0);
            printf("Se debía sumar una vida pero está en el límite %d\n",vidas);
        }
    }else{
        printf("This shouldn't happen, power problem\n");
    }
}

void Hitbloque(int rows,int columns){
    struct bloque *Block;
    Block=&(grid[rows][columns]);
    Block->vida--;
    if (Block->vida==0){
        char message[6];

        sprintf(message,"K:%d,%d",rows+1,columns+1);
        printf("%s",message);
        //send(clisockfd, message, 13, 0);
        HandleBlockdestruction(Block);
    }

}



 int Beginconnection()
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("ERROR opening socket");
        return(1);
    }

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
    srand(time(NULL));
    Initializegame(0);
    char lostballstr[]="LB";

    while (ingame) {
        bzero(buffer, 256);
        bzero(contentBuffer, 255);
        printf("[+]Client connected.\n");

        send(clisockfd, "Hola\n", 13, 0);
        (recv(clisockfd, buffer, 100, 0));

        if(strcmp(buffer,lostballstr)==0){
            LostBall();
        }else{
            int i,j,coord;
            coord=atoi(buffer);
            if(coord==0){
                continue;
            }else if(coord/100==0){
                j=coord%10;
                i=coord/10;
            }else{
                j=(coord%10)+(((coord/10)%10)*10);
                i=coord/100;
            }
            printf("Hay hit en i=%d j=%d\n",i,j);
            Hitbloque(i,j);
        }
    }
}
