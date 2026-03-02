/*Caratteristiche tecniche principali:
1. Generazione procedurale con algoritmo RANDOM WALKER
2. Uso di puntatori a matrice per il cambio del livello
3. IA dei nemici e pathfinding con algoritmo A*
4. Gestione stati, risorse, condizioni di vittoria/sconfitta e difficoltà variabile
*/

#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

#include "gioco.h"

int main (void)
{
    // setup ncurses
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);


    srand((unsigned)time(NULL));

    //definizione colori
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_CYAN, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(6, COLOR_YELLOW, COLOR_BLACK); // Colore per messaggi/armi

    // logica di generazione sicura
    Gioco* partita = NULL;
    int max_tentativi = 100;
    int tentativo_corrente = 0;
    int successo = 0; //flag

    for (tentativo_corrente = 0; tentativo_corrente < max_tentativi; tentativo_corrente++)
    {
        partita = gioco_init();

        // se malloc fallisce (memoria finita), usciamo subito
        if (!partita)
        {
            endwin();
            fprintf(stderr, "Errore critico: Memoria insufficiente per allocare il gioco.\n");
            return -1;
        }

        // Se la mappa è valida (get_flag == 1), usciamo dal ciclo: ABBIAMO VINTO!
        if (get_flag(partita) == 1)
        {
            successo = 1;
            break;
        }

        // se get_flag == 0 mappa non valida
        gioco_free(partita);
        partita = NULL; // Per sicurezza


        if (tentativo_corrente < max_tentativi - 1)
        {
            clear();
            mvprintw(LINES/2, (COLS/2)-15, "Rigenerazione mappa %d/%d...", tentativo_corrente + 1, max_tentativi);
            refresh();
        }
    }

    //controllo successo, se siamo usciti dal ciclo senza successo
    if (!successo)
    {
        endwin();
        fprintf(stderr, "Errore: Impossibile generare una mappa valida dopo %d tentativi.\n", max_tentativi);
        return -1;
    }

    // game loop
    gioco_draw(partita);

    int tasto;
    while ((tasto = getch()) != 'q')
    {
        int continua = gioco_update(partita, tasto);

        if (!continua) break;

        gioco_draw(partita);
    }

    gioco_free(partita);
    endwin();
    return 0;
}