#ifndef ILMIOGIOCO_DISEGNA_MAPPA_H
#define ILMIOGIOCO_DISEGNA_MAPPA_H

#include "genera_livello.h"
#include "entità.h"


/*Importante è l'utilizzo di 'char[][80]' compatibile con il puntatore globale*/
void disegna_mappa(char map [][LARGHEZZA_MAPPA], giocatore p, char *msg, nemico n[], int numero_nemici);

#endif //ILMIOGIOCO_DISEGNA_MAPPA_H