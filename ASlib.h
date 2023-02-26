// ASlib.h
// Biblioteca de funciones para Arquitectura de Sistemas II

#ifndef TOOLS_H_
#define TOOLS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define FIN "FIN"
#define TUBO "TUBO"

typedef struct Paquete {
    int destino;
    pid_t padre, hijo, nieto;
} Paquete;

#define TRACE \
    printf("%s:%d\n", __FILE__, __LINE__)
    
#define ERROR(msj)\
    {\
        fprintf(stderr, "ERROR %s--> %s:%d\n", msj, __FILE__, __LINE__);\
        exit(EXIT_FAILURE);\
    }

void TInit(int ntabs, int id);
void TPrint(const char* texto);
void TPrintInt(const char* texto, int valor);
void TPrintFloat(const char* texto, float valor);
void TPrintString (const char* texto, const char* valor);

void Pause(int min, int max);

pid_t Fork(void (*proceso)());
void RellenarPaquete(Paquete* paquete, const char* linea);
void Enviar(Paquete* paquete);
void Recibir(Paquete* paquete);
void Imprimir(Paquete* paquete);

#endif
