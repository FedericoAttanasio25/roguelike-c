#ifndef ILMIOGIOCO_GIOCO_H
#define ILMIOGIOCO_GIOCO_H

typedef struct Gioco Gioco;

Gioco* gioco_init();

int gioco_update (Gioco* g, int input_tasto);

int get_flag(Gioco* g);

void gioco_draw (Gioco *g);

void gioco_free (Gioco* g);

#endif //ILMIOGIOCO_GIOCO_H