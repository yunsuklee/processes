// Hijo.c
// Biblioteca de funciones para los Procesos Hijo

#include "Hijo.h"

#define N_NIETOS 2

pid_t nietos[N_NIETOS]; // Array para recoger el pid de los nietos

void ProcesoHijo() 
{ 
  // Redirijo la senal SIGUSR1 a un manejador para el hijo
  signal(SIGUSR1, ManejadorHijo);

  // Redirijo las senales SIGUSR2 y SIGUSR3 para el manejo de mensajes
  signal(SIGUSR2, ManejadorPaqueteHijo);

  // Creo a proceso Hijo
  TInit(1, getpid());

  // Se identifica a si mismo y a su Padre
  TPrintInt("Soy Hijo y mi Padre es ", getppid());

  // Creo a sus hijos, (nietos del padre)
  for (size_t i = 0; i < N_NIETOS; i++) {
    nietos[i] = Fork(ProcesoNieto);
    Pause(1000, 1500);
  }

  // El hijo espera infinitamente
  while(1)
    Pause(1000, 1000);
}

void ManejadorHijo(int senal)
{
  // Indico la recepcion de la senal
  TPrint("Recibi la senal SIGUSR1");

  // Envio la senal para finalizar a los nietos
  for (size_t i = 0; i < N_NIETOS; i++) {
    TPrintInt("Envio SIGUSR1 a ", nietos[i]);
    kill(nietos[i], SIGUSR1);
    wait(&nietos[i]);
  }

  // Termino proceso Hijo
  TPrint("Termino");  
  exit(EXIT_SUCCESS);
}

void ManejadorPaqueteHijo(int senal) 
{ 
  Paquete paquete;
  
  // Recibe el mensaje
  Recibir(&paquete);

  // Imprime mensaje recibido
  TPrint("Mensaje recibido ");
  Imprimir(&paquete);

  if (paquete.hijo == 0) {
    // Agrega su pid
    paquete.hijo = getpid();

    // Imprime mensaje enviado
    TPrint("Mensaje enviado ");
    Imprimir(&paquete);

    // Lo envia al Nieto correspondiente
    TPrintInt("Envio mensaje a Nieto ", nietos[paquete.destino % 10 - 1]);
    kill(nietos[paquete.destino % 10 - 1], SIGUSR2);
    Enviar(&paquete);
  } else {
    // Imprime mensaje enviado
    TPrint("Mensaje enviado ");
    Imprimir(&paquete);

    // Retransmite el mensaje al Padre
    TPrint("Envio mensaje a Padre");
    kill(getppid(), SIGUSR2);
    Enviar(&paquete);
  }
}