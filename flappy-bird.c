#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>

#define MAX_TUBERIAS 3

const int hueco = 6;
const int altura = 20;
const int ancho = 60;

struct Pajaro
{
    int x;
    int y;
    bool vivo;
};

struct Tuberia
{
    int x;
    int inicio_hueco;
    int fin_hueco;
};



void generarTuberia(struct Tuberia *t, int atraso)
{
    t->x = ancho + atraso - 1;

    int max_inicio = altura - hueco - 2;

    t->inicio_hueco = 1 + (rand() % max_inicio);

    t->fin_hueco = t->inicio_hueco + hueco;
}

void moverTuberia(struct Tuberia *t)
{
    t->x--;

    if (t->x < 0)
    {
        generarTuberia(t, 0);
    }
}

void subirPajaro(struct Pajaro *p)
{
    p->y -= 2;
    if (p->y >= altura - 1 || p->y <= 0)
    {
        p->vivo = false;
    }
}

void bajarPajaro(struct Pajaro *p)
{
    p->y++;

    if (p->y >= altura - 1 || p->y <= 0)
    {
        p->vivo = false;
    }
}

void moverPajaro(struct Pajaro *p)
{
    char tecla;

    if (read(STDIN_FILENO, &tecla, 1) > 0)
    {
        subirPajaro(p);
    }
    else
    {
        bajarPajaro(p);
    }
}

void detectarColisionPajaro(struct Pajaro *p, struct Tuberia *t)
{
    if (p->x == t->x)
    {
        if (p->y <= t->inicio_hueco || p->y >= t->fin_hueco)
        {
            p->vivo = false;
        }
    }
}

void screen(struct Pajaro *p, struct Tuberia tuberias[])
{
    printf("\033[H\033[J");

    for (int i = 0; i < altura; i++)
    {
        for (int j = 0; j < ancho; j++)
        {
            // límites pantalla
            if (i == 0 || i == altura - 1)
            {
                printf("-");
            }

            // pintar pájaro
            else if (j == p->x && i == p->y)
            {
                printf("@");
            }
            else 
            {
                // Dibujamos tuberias
                bool hayTuberia = false;
                for (int t = 0; t < MAX_TUBERIAS; t++) 
                {
                    // pintar tubería
                    if (j == tuberias[t].x)
                    {
                        hayTuberia = true;
                        if (i <= tuberias[t].inicio_hueco || i >= tuberias[t].fin_hueco)
                        {
                            printf("#");
                        }
                        else 
                        {
                            printf(" ");
                        }

                        break;
                    }
                }

                if (!hayTuberia)
                    printf(" ");

            }
        }
        printf("\n");
    }
    fflush(stdout);
}

void flappyBird(struct Pajaro *p, struct Tuberia tuberias[])
{
    screen(p, tuberias);
    moverPajaro(p);
    
    for (int t = 0; t < MAX_TUBERIAS; t++)
    {
        moverTuberia(&tuberias[t]);
        detectarColisionPajaro(p, &tuberias[t]);
    }

    usleep(80000);
}

int main(void)
{
    struct Pajaro pajaro = {.x = 5, .y = 10, .vivo = true};
    struct Tuberia tuberias[MAX_TUBERIAS];

    for (int i = 0; i < MAX_TUBERIAS; i++)
        generarTuberia(&tuberias[i], i * (ancho/MAX_TUBERIAS));

    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);

    while (pajaro.vivo == true)
    {
        flappyBird(&pajaro, tuberias);
    }

    return EXIT_SUCCESS;
}