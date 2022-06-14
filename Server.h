//
// Created by Lenovo on 6/6/2022.
//

#ifndef LENGUAJESTAREA3_SERVER_H
#define LENGUAJESTAREA3_SERVER_H
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

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

void Initializegrid();

void Initializegame();

void PrintSomething(int i,int j);

void Hitbloque(int rows,int columns);

void SpeedChange(int Change);

void LostBall();

void Barchange(int Change);

void HandleBlockdestruction(struct bloque *Block);

void Handleendgame(int Wincondition);

#endif //LENGUAJESTAREA3_SERVER_H
