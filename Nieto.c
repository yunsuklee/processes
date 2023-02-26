// Nieto.c
// Biblioteca de funciones para los Procesos Nieto

#include "Nieto.h"

void ProcesoNieto()
{
  // Redirijo la senal SIGUSR1 a un manejador para el hijo
  signal(SIGUSR1, ManejadorNieto);

  // Redirijo la senal SIGUSR2 a un manejador para el intercambio de mensajes
  signal(SIGUSR2, ManejadorPaqueteNieto);

  // Creo a proceso Nieto
  TInit(2, getpid());

  // Se identifica a si mismo y a su Padre
  TPrintInt("Soy Nieto y mi Padre es ", getppid());

  // El Nieto espera infinitamente
  while(1)
    Pause(1000, 1000);
}

void ManejadorNieto()
{ 
  // Indico la recepcion de la senal
  TPrint("Recibi la senal SIGUSR1");

  // Termino proceso Nieto
  TPrint("Termino");
  exit(EXIT_SUCCESS);
}

void ManejadorPaqueteNieto()
{
  Paquete paquete;

  // Recibe el mensaje
  Recibir(&paquete);

  // Imprime mensaje recibido
  TPrint("Mensaje recibido ");
  Imprimir(&paquete);

  // Agrega su pid
  paquete.nieto = getpid();

  // Imprime mensaje enviado
  TPrint("Mensaje enviado ");
  Imprimir(&paquete);

  // Devuelve la informacion al hijo  
  TPrint("Envio mensaje a mi Padre");
  kill(getppid(), SIGUSR2);
  Enviar(&paquete);
}