#include "ASlib.h"
#include "Hijo.h"
#include "Nieto.h"

#define N_HIJOS 2

// Funciones para procesar los comandos ingresados
void LineInit();
char* LineGet();
void LineFree();
void Manejador(int senal);
void ManejadorPaquetePadre(int senal);
  
static char* linea;
static size_t tamano;

int main(int argc, char* argv[]) {
  umask(0);
  mkfifo(TUBO, 0666);

  LineInit();          
  pid_t hijos[N_HIJOS]; // Array para recoger el pid de los hijos

  TInit(0, getpid());

  // Se identifica
  TPrint("Soy Padre");

  // Redirige SIGUSR1 a un manejador
  signal(SIGUSR1, Manejador);
  signal(SIGUSR2, ManejadorPaquetePadre);

  // Crea sus hijos
  for (size_t i = 0; i < N_HIJOS; i++) {
    hijos[i] = Fork(ProcesoHijo);
    Pause(1000, 3000);
  }

  // Lee los comandos ingresados
  linea = LineGet();
  while (strcmp(linea, FIN) != 0) {
    Paquete paquete;
    paquete.destino = 0;

    RellenarPaquete(&paquete, linea);

    if (paquete.destino) {
      TPrintInt("Envio mensaje a Hijo ", hijos[paquete.destino / 10 - 1]);
      kill(hijos[paquete.destino / 10 - 1], SIGUSR2);
      Enviar(&paquete);
    }

    linea = LineGet();
  }

  // Si el comando es FIN ejecuta la finalizacion ordenada
  LineFree();

  // Envio SIGUSR1 a sus hijos para finalizar sus procesos
  for (size_t i = 0; i < N_HIJOS; i++) {
    TPrintInt("Envio SIGUSR1 a ", hijos[i]);
    kill(hijos[i], SIGUSR1);
    wait(&hijos[i]);
  }

  // Termino proceso Padre
  unlink(TUBO);
  raise(SIGUSR1);
  return 0;
}

// Definiciones
void LineInit()
{
  linea = NULL;
  tamano = 0;
}

char *LineGet()
{
  ssize_t n = getline(&linea, &tamano, stdin);

  if (n == -1)
    ERROR("Entrada erronea");

  linea[strlen(linea) - 1] = '\0';
  return (linea);
}

void LineFree()
{
  free(linea);
}

void Manejador(int senal)
{
  TPrint("Termino");
  exit(EXIT_SUCCESS);
}

void ManejadorPaquetePadre(int senal)
{
  Paquete paquete;

  // Recibe el mensaje
  Recibir(&paquete);

  // Imprime el mensaje
  TPrint("Mensaje recibido");
  Imprimir(&paquete);
}