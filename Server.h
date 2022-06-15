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

/**Estructura bloques
 * Contiene información pertinenete a cada bloque que se va a almacenar contiene la vida, los puntos y el poder
 * que un determinado bloque tendrá
*/
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
int ajustevelocidad;//Variable que ajusta la velocidad de forma que cada nivel comience conmayor rapidez
int ingame;//Variable para determinar si se está en juego
//Variables de la conexión Cliente Servidor
int sockfd, clisockfd, portno;
socklen_t clilen;
char buffer[256];//Contiene el mensaje proveniente del usuario
char contentBuffer[255];
struct sockaddr_in serv_addr, cli_addr;

/**
 * @author Marcelo Truque
 * Función que inicializa la matriz de bloques, le asigna a cada bloque su vida y puntaje según su nivel de profundidad
 * Además asigna según una probabilidad de 25% un poder a un bloque. La construcción de la matriz de bloques se guarda
 * en la variable grid
 */
void Initializegrid();

/**
 * @author Marcelo Truque
 * Función que inicializa las variables del juego. Pone la vida en su valor correspondiente, las
 * velocidades y el tamaño de la barra. Como entrada recibe el ajuste de velocidad que se hará según
 * corresponda por el escalón de nivel
 * @param ajuste en la velocidad de las pelotas
 */
void Initializegame(int ajuste);

/**
 * @author Marcelo Truque
 * Función para imprimir toda la información contenida de un bloque según las posiciones i,j de la matriz
 * @param i Fila del bloque por imprimir
 * @param j Columna del bloque por imprimir
 */
void PrintSomething(int i,int j);


/**
 * @author Marcelo Truque
 * Función para aumetar o disminuir el tamaño de la barra según se indique en la entrada, la función regula que el tamaño
 * máximo de la barra sea de 560 y el mínimo 35
 * @param change indicador de si se aumentará o disminuirá la barra; toma como valor 1 si es aumento
 * y 0 si es disminución. El aumento es duplicarla y la disminución dividirla a la mitad
 */
void Barchange(int change);

/**
 * @author Marcelo Truque
 * Función para aumetar o disminuir la velocidad de las pelotas según se indique en la entrada la función regula que la velocidad
 * máxima de las pelotas sea 9 y la mínima 1
 * @param change indicador de si se aumentará o disminuirá la velocidad; toma como valor 1 si es aumento
 * y 0 si es disminución. El aumento es sumará 2 y la disminución restara 2
 */
void SpeedChange(int Change);


/**
 * @author Marcelo Truque
 * Manejo del final del juego, la función fija el valor de ingame en 0 y notifica al usuario su condición
 * de finalización (Ganó o perdió) Si ganó inicializa el juego con un ajuste de velocidad mayor. De lo contrario
 * Nada más termina
 * @param Wincondition
 */
void Handleendgame(int Wincondition);

/**
 * @author Marcelo Truque
 * Manejo de la pérdida de una pelota, maneja que el usuario pierda una vida, y en caso de que no tenga más
 * vidas termina el juego, también puede suceder que el usuario no tenga más pelotas en cuyo caso también finaliza el juego
 */
void LostBall();


/**
 * @author Marcelo Truque
 * Maneja el caso que un bloque sea destruido, envía la cantidad de puntos que se deben de sumar al usuario
 * también en caso de que el bloque contenga un poder, manejala notificación al usuario. Por último también
 * valida que se hayan acabado los bloques en cuyo caso notifica al cliente que debe de pasar de ronda
 * @param Block Bloque que fue destruido
 */
void HandleBlockdestruction(struct bloque *Block);

/**
 * @author Marcelo Truque
 * Detecta un golpe en un determinado bloque, y en caso de que no tenga m´ás vida llama a la función que
 * administra la destrucción de un bloque
 * @param rows la fila del bloque golpeado
 * @param columns la columna del bloque golpeado
 */
void Hitbloque(int rows,int columns);


/**
 * @author Ashley
 * Inicializa  la conexión cliente servidor y maneja los mensajes que se reciben
 */
int Beginconnection();

#endif //LENGUAJESTAREA3_SERVER_H
