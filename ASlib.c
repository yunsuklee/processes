// ASlib.c
// Biblioteca de funciones para Arquitectura de Sistemas II
#include "ASlib.h"

#define N_TABS 80
static char tabs[N_TABS];

void TInit(int ntabs, int id)
{
	int i;
	if (id < 10000000 && ntabs < N_TABS - 10) {
		char* p = tabs;
		
		for (i=0; i<ntabs; i++) 
			*p++ = '\t';
		
		sprintf(p, "[%d]", id);
	} else {
			ERROR("Encolumnado erróneo");
	}
}

void TPrint(const char *texto) 
{ 
	printf("%s%s\n", tabs, texto); 
}

void TPrintInt(const char* texto, int valor) 
{ 
	printf("%s%s%d\n", tabs, texto, valor); 
}

void TPrintFloat(const char* str, float valor) 
{ 
	printf("%s%s%0.2f\n", tabs, str, valor); 
}

void TPrintString (const char* texto, const char* valor) 
{ 
	printf("%s%s%s\n", tabs, texto, valor); 
}

void Pause(int min, int max)
{
	int tiempo = (min < max)
		? (rand() % (max - min)) + min
		: min;

	usleep(tiempo * 1000);
}

pid_t Fork(void (*proceso)()) 
{
	pid_t pid_hijo = fork();

	if (pid_hijo == 0) {
		TPrintInt("He creado a: ", getpid());
		proceso();
	} else if (pid_hijo < 0) {
		ERROR("Error");
	}

	return pid_hijo;
}

void RellenarPaquete(Paquete* paquete, const char* linea)
{
	if (!strcmp(linea, "N11")) {
		paquete->destino = 11; 
	} else if (!strcmp(linea, "N12")) {
		paquete->destino = 12;
	} else if (!strcmp(linea, "N21")) {
		paquete->destino = 21;
	} else if (!strcmp(linea, "N22")) {
		paquete->destino = 22;
	} else {
		TPrint("Comando no identificado");
		return;
	}

	// Si es el nombre de un nieto
	paquete->padre = getpid();
	paquete->hijo = 0;
	paquete->nieto = 0;	
}

void Enviar(Paquete *paquete)
{
	FILE *f = fopen(TUBO, "w");
	if (!f)
		ERROR("Error al abrir el tubo");

	size_t l = fwrite(paquete, sizeof(Paquete), 1, f);
	if (l != 1) {
		printf("%ld, %ld\n", l, sizeof(Paquete));
		ERROR("Error al escribir en el tubo");
	}
	fclose(f);
}

void Recibir(Paquete *paquete)
{
	FILE *f = fopen(TUBO, "r");
	if (!f)
		ERROR("Error al abrir el tubo");

	size_t l = fread(paquete, sizeof(Paquete), 1, f);
	if (l != 1)
		ERROR("Error al leer en el tubo");

	fclose(f);
}

void Imprimir(Paquete *paquete)
{
	char string[100];
	sprintf(
		string,
		"Destino: %d, Padre: %d, Hijo: %d, Nieto: %d", 
		paquete->destino,
		paquete->padre,
		paquete->hijo,
		paquete->nieto
	);

	TPrint(string);
}
