#ifndef ILMIOGIOCO_ENTITÀ_H
#define ILMIOGIOCO_ENTITÀ_H

#include "genera_livello.h"

#define MAX_NEMICI 10 //define per array sovradimensionale
typedef struct
{
    int y;
    int x;
    int tasca;  //tasca funzionerà se è piena (1) o vuota (0), se il personaggio ha la chiave o no.
    int arma;   //0 = vuota 1 = Martello 2 = pugnale 3 = sasso 4 = spada
    int durata; //durata delle armi sempre diversa per ogni arma
    char icon;
}giocatore;

typedef struct
{
    int y_e;
    int x_e;
    char icon_e;
}nemico;

void muovi_nemici(char map [][LARGHEZZA_MAPPA], giocatore p, nemico n[], int difficolta, int numero_nemici_attivi);
void inizializza_player (giocatore* p, char map [][LARGHEZZA_MAPPA]);
void inizializza_nemici (giocatore* p, nemico lista_nemici[], char map [][LARGHEZZA_MAPPA], int numero_da_spawnare);

int check_collisioni (giocatore* p, nemico* n);
void uccidi_nemico (nemico* n);
int is_nemico_morto(nemico* n);

#endif //ILMIOGIOCO_ENTITÀ_H