# 🗡️ Roguelike C-Engine (Terminal Game)

Un dungeon crawler procedurale scritto in **C**, giocabile da terminale tramite la libreria grafica `ncurses`. Il progetto è strutturato con un'architettura modulare (Game Engine, Entities, Level Generation, Pathfinding) tipica dello sviluppo videoludico.

## 🧠 Core Features & Game Design
* **Generazione Procedurale (Random Walker):** I livelli vengono scavati dinamicamente ad ogni avvio. Il sistema include un tool di validazione in background: se il motore genera una mappa matematicamente impossibile da finire (la porta di uscita è irraggiungibile), la scarta e ne rigenera una nuova in modo invisibile al giocatore.
* **A* Pathfinding (A-Star):** I nemici inseguono il giocatore calcolando il percorso più breve evitando i muri. L'A* è stato implementato da zero utilizzando la distanza di Manhattan e una coda di priorità custom per i nodi.
* **Comportamento Adattivo (Game AI):** I nemici cambiano pattern in base allo stato del giocatore. Se disarmato, lo inseguono (tramite A* o line-of-sight); se il giocatore raccoglie un'arma, l'IA inverte la logica e tenta di sfuggire scivolando lungo gli ostacoli.
* **Architettura Modulare e Game Loop:** Codice suddiviso rigorosamente (Headers e Source files) con un classico pattern *Update & Draw*, assicurando scalabilità e aggiornamenti a schermo fluidi.

## 🚀 Come avviare il gioco
Il progetto richiede la libreria `ncurses` per il rendering dell'interfaccia a riga di comando.

**Compilazione (tramite GCC):**
`gcc *.c -lncurses -lm -o roguelike`
*(Nota: `-lm` è incluso per linkare le funzioni matematiche standard se necessarie sui sistemi Linux, e `-lncurses` per il motore grafico).*

**Esecuzione:**
`./roguelike`

**Controlli di Gioco:**
* Usa le **Frecce Direzionali** per muoverti.
* **q** per uscire dal gioco.
