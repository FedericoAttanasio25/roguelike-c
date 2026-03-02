#include "pathfinding.h"
#include <stdlib.h>
#include <math.h>
#include <limits.h>

typedef struct node
{
    int y, x;
    int g_cost; // Distanza dallo start
    int h_cost; // Distanza stimata dal target
    int f_cost; // G + H
    int parent_y, parent_x; //precedenti di y e x
    int open; //caselle scoperte da analizzare
    int close; //caselle già visitate o inaccessibili
}nodo;

//y1 e x1 sono le coordinate del nemico y2 e x2 sono le coordinate del player
//utilizzo formula della distanza di manhattan
static int calcola_euristica (int y1, int y2, int x1, int x2)
{
    return abs(y1 - y2) + abs (x1 - x2);
}

int calcolo_mossa_a_star(char map[][LARGHEZZA_MAPPA], int start_y, int start_x, int target_y, int target_x, int* next_y, int* next_x)
{
    //se siamo già arrivati siamo fermi
    if (start_y == target_y && start_x == target_x)
    {
        *next_y = start_y;
        *next_x = start_x;
        return 1;
    }

    //creo una griglia di nodi grande come la mappa
    static nodo griglia[ALTEZZA_MAPPA][LARGHEZZA_MAPPA];

    //inizializzazione della griglia
    //non inizializzo ora h o f perchè è inutile farlo all'inizio
    for (int y = 0; y < ALTEZZA_MAPPA; y++)
    {
        for (int x = 0; x < LARGHEZZA_MAPPA; x++)
        {
            griglia[y][x].y = y;
            griglia[y][x].x = x;
            griglia[y][x].g_cost = INT_MAX;
            griglia[y][x].f_cost = INT_MAX;
            griglia[y][x].parent_y = -1;
            griglia[y][x].parent_x = -1;
            griglia[y][x].open = 0;
            griglia[y][x].close = 0;
        }
    }

    //setup nodo iniziale
    //non inizializzo x, y i parent e close perchè l'abbiamo già fatto prima
    nodo* nodo_iniziale = &griglia[start_y][start_x];
    nodo_iniziale->g_cost = 0;
    nodo_iniziale->h_cost = calcola_euristica(start_y, target_y, start_x, target_x);
    nodo_iniziale->f_cost = nodo_iniziale->h_cost;
    nodo_iniziale->open = 1;



    while (1)
    {
        //cerco il nodo con f_cost minore
        nodo* current = NULL;
        int min_f = INT_MAX;

        //coda di priorità
        //cuore decisionale dell'algoritmo, fa capire quale tra tutte le strade che può prendere è quella più promettente
        for (int y = 0; y < ALTEZZA_MAPPA; y++)
        {
            for (int x = 0; x < LARGHEZZA_MAPPA; x++)
            {
                //in questo momento cerco la mia priorità
                if (griglia[y][x].open && griglia [y][x].f_cost < min_f)
                {
                    min_f = griglia [y][x].f_cost;
                    current = &griglia[y][x];
                }
            }
        }

        //non c'è percorso
        if (current == NULL) return 0;

        //sono arrivato al player (non fisicamente ma ho trovato il percorso migliore)
        //voglio conoscere dove fare il primo passo fisico
        if (current->y == target_y && current->x == target_x)
        {
            //RICOSTRUZIONE PERCORSO A RITROSO
            nodo* temp = current;

            while (temp->parent_y != -1 && (temp->parent_y != start_y || temp->parent_x != start_x))
            {
                temp = &griglia[temp->parent_y][temp->parent_x];
            }

            *next_y = temp->y;
            *next_x = temp->x;
            return 1;
        }

        //questa casella l'ho analizzata
        current->open = 0;
        current->close = 1;

        //definisco i cambiamenti per le 4 direzioni
        int dy[4] = {-1, 1, 0, 0};
        int dx[4] = {0, 0, 1, -1};

        for (int i = 0; i < 4; i++)
        {
            int ny = current->y + dy[i];
            int nx = current->x + dx[i];

            if (ny < 0 || ny >= ALTEZZA_MAPPA || nx < 0 || nx >= LARGHEZZA_MAPPA)
            {
                continue;
            }

            if (map[ny][nx] == '#')
            {
                continue;
            }

            if (griglia[ny][nx].close)
            {
                continue;
            }

            //provvisorio perchè non so se è la strada migliore
            int g_provvisorio = current->g_cost + 1;

            //se la strada provvisoria è meglio di quella che ho già fatto oppure non l'ho ancora analizzata
            if (!griglia[ny][nx].open || g_provvisorio < griglia[ny][nx].g_cost)
            {
                //aggiorno i genitori con le coordinate della strada più vantaggiosa
                griglia[ny][nx].parent_y = current->y;
                griglia[ny][nx].parent_x = current->x;

                griglia[ny][nx].g_cost = g_provvisorio;
                griglia[ny][nx].h_cost = calcola_euristica(ny, target_y, nx, target_x);
                griglia[ny][nx].f_cost = griglia[ny][nx].g_cost + griglia[ny][nx].h_cost;

                griglia[ny][nx].open = 1;
            }
        }
    }
}
