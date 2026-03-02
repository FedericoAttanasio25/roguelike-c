#include "genera_livello.h"
#include "entità.h"
#include "gioco.h"
#include <ncurses.h>

void disegna_mappa(char map [][LARGHEZZA_MAPPA], giocatore p, char *msg, nemico n[], int numero_nemici)
{
    clear();

    //disegno mappa statica
    for(int y = 0; y < ALTEZZA_MAPPA; y++)
    {
        for (int x = 0; x < LARGHEZZA_MAPPA; x++)
        {
            char carattere = map[y][x];
            switch(carattere)
            {
                case '#':
                    attron(COLOR_PAIR(3));
                    break;

                case 'x':
                    attron(COLOR_PAIR(1));
                    break;

                case '.':
                    attron(COLOR_PAIR(2));
                    break;

                case 'd':
                    attron(COLOR_PAIR(5));
                    break;

                case 'W':
                    attron(COLOR_PAIR(6));
                    break;

                case 'P':
                    attron(COLOR_PAIR(6));
                    break;

                case 'R':
                    attron(COLOR_PAIR(6));
                    break;

                case '!':
                    attron(COLOR_PAIR(6));
                    break;

                case 's':
                    attron(COLOR_PAIR(1));
                    break;

                case 'k':
                    attron(COLOR_PAIR(5));
                    break;

                default:
                    break;
            }
            mvaddch(y, x, carattere);
            attrset(A_NORMAL);
        }
    }

    //ciclo per stampare i nemici
    attron(COLOR_PAIR(1));
    for(int i = 0; i < numero_nemici; i++)
    {
        if(n[i].x_e != -1)
        {
            mvaddch(n[i].y_e, n[i].x_e, n[i].icon_e);
        }
    }
    attroff(COLOR_PAIR(1));

    //disegno giocatore
    attron(COLOR_PAIR(4));
    mvaddch(p.y, p.x, p.icon);
    attroff(COLOR_PAIR(4));

    mvprintw(ALTEZZA_MAPPA + 1, 0, "Status: %s", msg);
    mvprintw(ALTEZZA_MAPPA + 2, 0, "Tasca: %s", p.tasca ? "Piena (Chiave)" : "Vuota");
    mvprintw(ALTEZZA_MAPPA + 3, 0, "Arma: %s (Durata: %d)", p.arma ? "SI" : "No", p.durata);
    refresh();
}