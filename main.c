#include <stdio.h>
#include "Server.h"
#include <string.h>
int main() {
    printf("Begining\n");
    Initializegame();
    PrintSomething(6,0);
    PrintSomething(5,0);
    Hitbloque(6,0);
    PrintSomething(6,0);
    Hitbloque(5,0);
    Hitbloque(5,0);
    LostBall();
    PrintSomething(6,0);
    PrintSomething(5,0);
}