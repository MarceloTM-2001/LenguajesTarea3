//
// Created by Lenovo on 6/6/2022.
//

#include "Server.h"

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
