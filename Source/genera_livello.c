#include"genera_livello.h"
#include <stdlib.h>

void genera_livello(char map [][LARGHEZZA_MAPPA])
{
    //ALGORITMO RANDOM WALKER
    for(int y = 0; y < ALTEZZA_MAPPA; y++)
    {
        for (int x = 0; x < LARGHEZZA_MAPPA; x++)
        {
            map[y][x] = '#'; //riempie tutto con muri
        }
    }

    //coordinate del minatore casuali
    int miner_x = 2 + rand () % 78;
    int miner_y = 2 + rand () % 39;
    map[miner_y][miner_x] = '.';

    //scavo casuale per 8000 passi
    for (int i = 0; i < 8000; i++)
    {
        int direzione = rand () % 4;
        int temp_miner_x = miner_x;
        int temp_miner_y = miner_y;

        switch(direzione)
        {
            case 0:
                temp_miner_y--;
                break;

            case 1:
                temp_miner_y++;
                break;

            case 2:
                temp_miner_x--;
                break;

            case 3:
                temp_miner_x++;
                break;

            default:
                break;
        }

        //resto nei bordi lasciando una cornice di 2
        if(temp_miner_x > 1 && temp_miner_y > 1 && temp_miner_x < LARGHEZZA_MAPPA - 2 && temp_miner_y < ALTEZZA_MAPPA - 2)
        {
            miner_x = temp_miner_x;
            miner_y = temp_miner_y;
            map[miner_y][miner_x] = '.';
        }

    }
}