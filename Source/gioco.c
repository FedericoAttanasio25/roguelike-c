#include "gioco.h"
#include "genera_livello.h"
#include "entità.h"
#include "disegna_mappa.h"
#include "pathfinding.h"
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <time.h>
#include <math.h>

struct Gioco {
    char mappa[ALTEZZA_MAPPA][LARGHEZZA_MAPPA]; //Livello 1
    char mappa_s[ALTEZZA_MAPPA][LARGHEZZA_MAPPA]; //Segrete

    //Puntatore a matrice che permette di scambiare il livello 1 con le segrete
    char (*p_mappa)[LARGHEZZA_MAPPA];

    giocatore player;
    nemico nemici_terra[MAX_NEMICI];
    nemico nemici_segrete[MAX_NEMICI];
    nemico* p_nemici;

    int difficolta;
    int num_nemici_attivi;

    int x_uscita;
    int y_uscita;

    int flag;

    char messaggio_status[100];
};

static void piazza_oggetto(char map[][LARGHEZZA_MAPPA], char obj);
static void ricolloca_nemici_vivi (giocatore* p, nemico* lista, char map[][LARGHEZZA_MAPPA], int num_nemici);
static void calcola_movimento (int input, int* dy, int* dx);
static int gestisci_interazione_oggetti(Gioco* g, int new_y, int new_x, char destinazione);

static void piazza_oggetto(char map[][LARGHEZZA_MAPPA], char obj)
{
    int r, c;
    do {
        r = rand() % ALTEZZA_MAPPA;
        c = rand() % LARGHEZZA_MAPPA;
    } while (map[r][c] != '.');
    map[r][c] = obj;
}

static void ricolloca_nemici_vivi (giocatore* p, nemico* lista, char map[][LARGHEZZA_MAPPA], int num_nemici)
{
    int riga_casuale_n, colonna_casuale_n;
    int distanza_sicurezza_y, distanza_sicurezza_x;

    //posizionamento nemici
    for(size_t i = 0; i < num_nemici; i++)
    {
        if (lista[i].x_e == -1) continue; //se il nemico è morto, salta il giro e lascialo morto

        do
        {
            riga_casuale_n = rand () % ALTEZZA_MAPPA;
            colonna_casuale_n = rand () % LARGHEZZA_MAPPA;

            //calcolo distanza assoluta
            distanza_sicurezza_y = abs(riga_casuale_n - p->y);
            distanza_sicurezza_x = abs(colonna_casuale_n - p->x);
        } while (map[riga_casuale_n][colonna_casuale_n] != '.' || (distanza_sicurezza_y < 5 && distanza_sicurezza_x < 10));

        lista[i].y_e = riga_casuale_n;
        lista[i].x_e = colonna_casuale_n;
    }
}

static void calcola_movimento (int input, int* dy, int* dx)
{
    *dy = 0;
    *dx = 0;

    switch (input)
    {
        case KEY_UP:
            *dy = -1;
            break;

        case KEY_DOWN:
            *dy = 1;
            break;

        case KEY_LEFT:
            *dx = -1;
            break;

        case KEY_RIGHT:
            *dx = 1;
            break;

        default:
            break;
    }
}

static int gestisci_interazione_oggetti(Gioco* g, int new_y, int new_x, char destinazione)
{
    if (destinazione == 'k')
    {
        g->player.tasca = 1;
        g->p_mappa[new_y][new_x] = '.';
        g->player.x = new_x;
        g->player.y = new_y;
        strcpy(g->messaggio_status, "Hai preso la chiave!");
    }

    else if (destinazione == 'W')
    {
        g->player.arma = 1;
        g->p_mappa[new_y][new_x] = '.';
        g->player.x = new_x;
        g->player.y = new_y;
        g->player.durata = 51;
        strcpy(g->messaggio_status, "Hai preso un martello! Quest'arma dura 50 turni ");
    }

    else if (destinazione == 'P')
    {
        g->player.arma = 2;
        g->p_mappa[new_y][new_x] = '.';
        g->player.x = new_x;
        g->player.y = new_y;
        g->player.durata = 26;
        strcpy(g->messaggio_status, "Hai preso un pugnale! Quest'arma dura 25 turni");
    }

    else if (destinazione == 'R')
    {
        g->player.arma = 3;
        g->p_mappa[new_y][new_x] = '.';
        g->player.x = new_x;
        g->player.y = new_y;
        g->player.durata = 16;
        strcpy(g->messaggio_status, "Hai preso un sasso! Quest'arma dura 15 turni");
    }

    else if (destinazione == '!')
    {
        g->player.arma = 4;
        g->p_mappa[new_y][new_x] = '.';
        g->player.x = new_x;
        g->player.y = new_y;
        g->player.durata = 36;
        strcpy(g->messaggio_status, "Hai preso una spada! Quest'arma dura 35 turni");
    }

    else if (destinazione == 'd')
    {
        if (g->player.tasca == 1)
        {
            g->p_mappa[new_y][new_x] = '.';
            g->player.x = new_x;
            g->player.y = new_y;
            strcpy(g->messaggio_status, "Porta aperta!");
        }

        else
        {
            strcpy(g->messaggio_status, "Serve la chiave!");
        }
    }

    else if (destinazione == 'x' || destinazione == 'X')
    {
        clear();
        attron(COLOR_PAIR(2));
        mvprintw(ALTEZZA_MAPPA / 2, (LARGHEZZA_MAPPA / 2) - 10, "HAI VINTO! Premi un tasto per uscire...");
        attroff(COLOR_PAIR(2));
        refresh();
        getch();
        return 0;
    }

    return 1;
}


Gioco *gioco_init()
{
    Gioco *g = (Gioco *) malloc(sizeof(Gioco));
    if (!g)
    {
        return NULL;
    }

    clear();mvprintw(5, 10, "BENVENUTO NEL ROGUELIKE");
    mvprintw(7, 10, "Seleziona la difficolta':");
    mvprintw(9, 10, "1 - FACILE (Pochi nemici, IA standard)");
    mvprintw(10, 10, "2 - MEDIA  (Nemici standard, IA intelligente)");
    mvprintw(11, 10, "3 - DIFFICILE (Tanti nemici, IA infallibile)");
    refresh();

    int scelta = 0;
    while (scelta < '1' || scelta > '3')
    {
        scelta = getch();
    }

    g->difficolta = scelta - '0';

    if (g->difficolta == 1) g->num_nemici_attivi = 2;
    else if (g-> difficolta == 2) g-> num_nemici_attivi = 3;
    else if (g-> difficolta == 3) g-> num_nemici_attivi = 5;

    //setup puntatori
    g->p_mappa = g->mappa;
    g->p_nemici = g->nemici_terra;

    //Generazione mappe
    genera_livello(g->mappa);
    genera_livello(g->mappa_s);

    //setup oggetti fissi
    piazza_oggetto(g->mappa_s, 's');
    piazza_oggetto(g->mappa, 's');

    int dove_chiave = rand () % 2;
    if (dove_chiave == 0)
    {
        piazza_oggetto(g->mappa, 'k');
    }

    else
    {
        piazza_oggetto(g->mappa_s, 'k');
    }



    char armi [] = {'W', 'P', 'R', '!'};
    int numero_armi = 4;

    for (int i = 0; i < numero_armi; i++)
    {
        //0 = terra, 1 = segrete
        int dove_arma = rand () % 2;

        if (dove_arma == 0)
        {
            piazza_oggetto(g->mappa, armi[i]);
        }

        else
        {
            piazza_oggetto(g->mappa_s, armi[i]);
        }
    }

    //generazione scatola della x
    int r_centro, c_centro;
    do {
        r_centro = 3 + rand() % (ALTEZZA_MAPPA - 6); //utilizzo 3 come margine di sicurezza
        c_centro = 3 + rand() % (LARGHEZZA_MAPPA - 6);
    } while (g->mappa[r_centro][c_centro] != '.');

    //costruzione mura 3x3
    for (int y = r_centro - 1; y <= r_centro + 1; y++)
    {
        for (int x = c_centro - 1; x <= c_centro + 1; x++)
        {
            g->mappa[y][x] = '#';
        }
    }
    g->mappa[r_centro][c_centro] = 'x';
    g->x_uscita = c_centro;
    g->y_uscita = r_centro;

    //porta d su un lato casuale e nella casella davanti metto pavimento
    int lato = rand() % 4;
    switch (lato)
    {
        case 0:
            g->mappa[r_centro - 1][c_centro] = 'd';
            g->mappa[r_centro - 2][c_centro] = '.';
            break;

        case 1:
            g->mappa[r_centro + 1][c_centro] = 'd';
            g->mappa[r_centro + 2][c_centro] = '.';
            break;

        case 2:
            g->mappa[r_centro][c_centro - 1] = 'd';
            g->mappa[r_centro][c_centro - 2] = '.';
            break;

        case 3:
            g->mappa[r_centro][c_centro + 1] = 'd';
            g->mappa[r_centro][c_centro + 2] = '.';
            break;

        default:
            break;
    }
    //fine generazione scatola per la x


    int x_temp = 0;
    int y_temp = 0;

    inizializza_player(&g->player, g->mappa);
    //flag per capire se posso arrivare alla porta o no
    g->flag = calcolo_mossa_a_star(g->mappa, g->player.y, g->player.x, g->y_uscita, g->x_uscita, &x_temp, &y_temp );

    inizializza_nemici(&g->player, g->nemici_terra, g->mappa, g->num_nemici_attivi);
    inizializza_nemici(&g->player, g->nemici_segrete, g->mappa_s, g->num_nemici_attivi);

    strcpy(g->messaggio_status, "Trova la chiave per uscire!");

    return g;
}

int gioco_update(Gioco *g, int input_tasto) {

    int dy, dx;
    calcola_movimento(input_tasto, &dy, &dx);

    //se il giocatore non si muove non aggiorniamo la pagina (risparmio CPU e logica)
    if (dy == 0 && dx == 0) return 1;

    int new_y = g->player.y + dy;
    int new_x = g->player.x + dx;

    //variabile che serve a capire cosa c'è nella casella di destinazione
    char destinazione = g->p_mappa[new_y][new_x];

    if (destinazione == 's')
    {
        if (g->p_mappa == g->mappa)
        {
            g->p_mappa = g->mappa_s;
            g->p_nemici = g->nemici_segrete;
            strcpy(g->messaggio_status, "Sei sceso nelle segrete");
        }

        else
        {
            g->p_mappa = g->mappa;
            g->p_nemici = g->nemici_terra;
            strcpy(g->messaggio_status, "Sei tornato in superficie");
        }

        int scale_trovate = 0;
        //cerco scale nella nuova mappa per metterci sopra il giocatore
        for (int y = 0; y < ALTEZZA_MAPPA; y++)
        {
            for (int x = 0; x < LARGHEZZA_MAPPA; x++)
            {
                if (g->p_mappa[y][x] == 's')
                {
                    g->player.y = y;
                    g->player.x = x;
                    scale_trovate = 1;
                    break;
                }
            }
            if (scale_trovate) break;
        }
        ricolloca_nemici_vivi(&g->player, g->p_nemici, g->p_mappa, g->num_nemici_attivi);
        return 1;
    }

    //se dalla funzione esce 0, ovvero siamo andati sulla x finisce il gioco
    if (gestisci_interazione_oggetti(g, new_y, new_x, destinazione) == 0)
    {
        return 0;
    }

    //se è pavimento (o era un oggetto preso) e non è muro
    if (destinazione == '.')
    {
        g->player.x = new_x;
        g->player.y = new_y;
        strcpy(g->messaggio_status, ""); //reset del messaggio di status
    }

    muovi_nemici(g->p_mappa, g->player, g->p_nemici, g->difficolta, g->num_nemici_attivi);


    if (g->player.arma > 0)
    {
        g->player.durata--;

        if (g->player.durata == 0)
        {
            g->player.arma = 0;
            g->player.durata = 0;
            strcpy(g->messaggio_status, "L'arma si è rotta");
        }
    }

    for (int i = 0; i < g->num_nemici_attivi; i++)
    {
        if (check_collisioni(&g->player, &g->p_nemici[i]))
        {
            if(g->player.arma > 0)
            {
                uccidi_nemico(&g->p_nemici[i]);
                strcpy(g->messaggio_status, "Nemico sconfitto!");
            }

            else
            {
                clear();
                attron(COLOR_PAIR(1));
                mvprintw(ALTEZZA_MAPPA / 2, (LARGHEZZA_MAPPA / 2) - 10, "SEI STATO PRESO! Premi un tasto per uscire...");
                attroff(COLOR_PAIR(1));
                refresh();
                getch();
                return 0;
            }
        }
    }

    return 1;
}

void gioco_draw (Gioco* g)
{
    disegna_mappa(g->p_mappa, g->player, g->messaggio_status, g->p_nemici, g->num_nemici_attivi);
}

void gioco_free (Gioco* g)
{
    if (g) free (g);
}

//uso a* per controllare se posso arrivare alla porta d altrimenti richiamo gioco_init
int get_flag (Gioco* g)
{
    return g->flag;
}