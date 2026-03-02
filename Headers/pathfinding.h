
#ifndef ILMIOGIOCO_PATHFINDING_H
#define ILMIOGIOCO_PATHFINDING_H

#include "genera_livello.h"

//Input:mappa, coordinate di partenza (nemico), coordinate di arrivo (player)
//Riempie next_y e next_x con le coordinate del prossimo passo da fare
//Restituisce 1 se ha trovato un percorso, 0 se non c'è strada
int calcolo_mossa_a_star(char map[][LARGHEZZA_MAPPA], int start_y, int start_x, int target_y, int target_x, int* next_y, int* next_x);

#endif //ILMIOGIOCO_PATHFINDING_H