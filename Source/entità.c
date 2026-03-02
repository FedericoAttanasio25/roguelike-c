#include "entità.h"
#include "genera_livello.h"
#include "pathfinding.h"
#include <stdlib.h>
#include <math.h>


void inizializza_player (giocatore* p, char map [][LARGHEZZA_MAPPA])
{
    int riga_casuale, colonna_casuale;
    do
    {
        riga_casuale = rand () % ALTEZZA_MAPPA;
        colonna_casuale = rand () % LARGHEZZA_MAPPA;
    }while(map[riga_casuale][colonna_casuale] != '.');

    p->y = riga_casuale;
    p->x = colonna_casuale;
    p->icon = '@';
    p->tasca = 0;
    p->arma = 0;

}



void inizializza_nemici (giocatore* p, nemico lista_nemici[], char map [][LARGHEZZA_MAPPA], int numero_da_spawnare)
{
    //prima li "uccido" tutti
    for (int i = 0; i < MAX_NEMICI; i++)
    {
        lista_nemici[i].x_e = -1;
        lista_nemici[i].y_e = -1;
    }

    //non spawnare più nemici di quanto l'array possa contenere
    if (numero_da_spawnare > MAX_NEMICI) numero_da_spawnare = MAX_NEMICI;

    //poi ne attivo solo quanti richiesti dalla difficoltà
    int riga_casuale_n, colonna_casuale_n;
    int distanza_sicurezza_y, distanza_sicurezza_x;

    //posizionamento nemici
    for(size_t i = 0; i < numero_da_spawnare; i++)
    {
        do
        {
            riga_casuale_n = rand () % ALTEZZA_MAPPA;
            colonna_casuale_n = rand () % LARGHEZZA_MAPPA;

            //calcolo distanza assoluta
            distanza_sicurezza_y = abs(riga_casuale_n - p->y);
            distanza_sicurezza_x = abs(colonna_casuale_n - p->x);
        } while (map[riga_casuale_n][colonna_casuale_n] != '.' || (distanza_sicurezza_y < 7 && distanza_sicurezza_x < 15));

        lista_nemici[i].y_e = riga_casuale_n;
        lista_nemici[i].x_e = colonna_casuale_n;
        lista_nemici[i].icon_e = 'E';
    }
}


void muovi_nemici(char map [][LARGHEZZA_MAPPA], giocatore p, nemico n[], int difficolta, int numero_nemici_attivi)
{
    for(int i = 0; i < numero_nemici_attivi; i++)
    {
        //se il nemico è morto salta il ciclo
        if (n[i].x_e == -1)
        {
            continue;
        }

        int new_y_e = n[i].y_e;
        int new_x_e = n[i].x_e;

        //difficoltà semplice e il giocatore disarmato
        if (difficolta == 1 && p.arma == 0)
        {
            if(p.y > n[i].y_e) new_y_e++;
            else if(p.y < n[i].y_e) new_y_e--;

            else if(p.x > n[i].x_e) new_x_e++;
            else if(p.x < n[i].x_e) new_x_e--;
        }

        //difficoltà 2 e 3
        else {
            //calcolo possibilità di errore
            int soglia_errore = 0;
            if (difficolta == 2) soglia_errore = 2; //20%
            if (difficolta == 3) soglia_errore = 0;


            int dado = rand () % 10;
            if (difficolta == 2 && dado < soglia_errore)
            {
                //mossa casuale del nemico
                int mossa_casuale = rand () % 4;
                switch (mossa_casuale)
                {
                    case 0:
                        new_y_e--;
                        break;

                    case 1:
                        new_y_e++;
                        break;

                    case 2:
                        new_x_e--;
                        break;

                    case 3:
                        new_x_e++;
                        break;

                    default:
                        break;
                }
            }

            else {
                //se il giocatore ha l'arma i nemici inseguono sempre con margine d'errore
                if(p.arma == 0)
                {
                    //A* calcola il percorso migliore grazie al passaggio per riferimento delle nuove coordinate
                    int trovato = calcolo_mossa_a_star(map, n[i].y_e, n[i].x_e, p.y, p.x, &new_y_e, &new_x_e);

                    //se a* non trova strada proviamo un movimento base verso il player
                    if (!trovato)
                    {
                        if (p.y > n[i].y_e) new_y_e++;
                        else if (p.y < n[i].y_e) new_y_e--;

                        // Se muovendo in Y trovo muro, provo X
                        if (map[new_y_e][n[i].x_e] == '#')
                        {
                            new_y_e = n[i].y_e; // Annullo Y

                            //non posso muovermi su y mi muovo su x
                            if (p.x > n[i].x_e) new_x_e++;
                            else if (p.x < n[i].x_e) new_x_e--;
                        }
                    }
                }

                else
                {
                    //logica dello scivolamento
                    //prova ad allontanarti, se c'è un muro prova l'altra direzione
                    int escape_y = n[i].y_e;
                    int escape_x = n[i].x_e;

                    //il nemico scappa lungo l'asse dove il player è più vicino
                    if(p.y > n[i].y_e)
                    {
                        escape_y--; //scappa su
                    }

                    else if (p.y < n[i].y_e)
                    {
                        escape_y++; //scappa giù
                    }


                    if(p.x > n[i].x_e)
                    {
                        escape_x--; //scappa a sinistra
                    }

                    else if (p.x < n[i].x_e)
                    {
                        escape_x++; //scappa a destra
                    }

                    //controllo se y e libera e nel caso mi muovo
                    if (escape_y != n[i].y_e && map[escape_y][n[i].x_e] != '#')
                    {
                        new_y_e = escape_y;
                    }

                    // Se non posso in Y (muro), provo in X (Orizzontale)
                    else if (escape_x != n[i].x_e && map[n[i].y_e][escape_x] != '#')
                    {
                        new_x_e = escape_x;
                    }
                }
            }
        }

        //collisione tra nemici
        int occupato = 0;
        for (int j = 0; j < numero_nemici_attivi; j++)
        {
            if (i == j) continue;
            if (n[j].x_e != -1 && n[j].y_e == new_y_e && n[j].x_e == new_x_e)
            {
                occupato = 1;
                break;
            }
        }

        //mi muovo solo se non c'è muro e se la casella non è occupata
        if (map[new_y_e][new_x_e] != '#' && !occupato)
        {
            n[i].y_e = new_y_e;
            n[i].x_e = new_x_e;
        }
    }

}

//restituisce 1 se si toccano, 0 altrimenti
int check_collisioni (giocatore* p, nemico* n)
{
    if (n->x_e == -1) return 0;

    return (p->y == n->y_e && p->x == n->x_e);
}

void uccidi_nemico (nemico* n)
{
    n->x_e = -1;
    n->y_e = -1;
}

int is_nemico_morto(nemico* n)
{
    return (n->x_e == -1);
}