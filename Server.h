//
// Created by Marcelo Truque Montero on 15/06/2022.
//

#ifndef LENGUAJESTAREA3_SERVER_H
#define LENGUAJESTAREA3_SERVER_H
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>
#include <netinet/in.h>
#include <strings.h>
#include <string.h>

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
int ajustevelocidad;
int ingame;
int sockfd, clisockfd, portno;
socklen_t clilen;
char buffer[256];
char contentBuffer[255];
struct sockaddr_in serv_addr, cli_addr;

void Initializegrid();

void Initializegame(int ajuste);

void PrintSomething(int i,int j);

void Barchange(int change);

void SpeedChange(int Change);

void Handleendgame(int Wincondition);

void LostBall();

void HandleBlockdestruction(struct bloque *Block);

void Hitbloque(int rows,int columns);

int Beginconnection();

#endif //LENGUAJESTAREA3_SERVER_H
