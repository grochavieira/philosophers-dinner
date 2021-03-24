#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <pthread.h>

using namespace std;

/* Apenas n - 1 filósofos são utilizados, pois um "filósofo" (thread) é responsável
 por observar todos os outros e imprimir na tela a situação deles */
#define Filosofos 6

int Filosofo[Filosofos]; //numero do filosofo
pthread_cond_t condicao[Filosofos];
char estadoFilosofos[Filosofos][1];
bool garfos[5] = {false, false, false, false, false};
int ciclos = 0;

void garfoDireito(int filosofo);
void garfoEsquerdo(int filosofo);
void devolveGarfos(int filosofo);
void pensar(int filosofo);
void comer(int filosofo);
void *threadFilosofo(void *arg);

int main(int argc, char *argv[])
{
    pthread_t ph_thread[Filosofos];

    for (int i = 0; i < Filosofos; i++)
    {
        pthread_cond_init(&condicao[i], NULL);
        Filosofo[i] = i;
    }

    for (int i = 0; i < Filosofos; i++)
    {
        printf("%4d - Filosofo #%d sentou na mesa.\n", i, i + 1);
        pthread_create(&ph_thread[i], NULL, threadFilosofo, &Filosofo[i]);
        sleep(0);
    }
    for (int i = 0; i < Filosofos; i++)
    {
        pthread_join(ph_thread[i], NULL);
    }

    for (int i = 0; i < Filosofos; i++)
    {
        pthread_cond_destroy(&condicao[i]);
    }
    return (0);
}

void *threadFilosofo(void *arg)
{
    int filosofo = *(int *)arg;
    while (1)
    {
        if (filosofo == Filosofos - 1)
        {
            usleep(100 * 1000);
            ciclos++;
            printf("Ciclo %d ->", ciclos);
            for (int i = 0; i < (Filosofos - 1); i++)
            {
                printf(" F%d: %c ", (i + 1), estadoFilosofos[i][0]);
            }

            printf("\n");
        }
        else
        {
            pensar(filosofo);
            usleep(100 * 1000);
            // usleep((rand() % 1000) * 1000);

            garfoEsquerdo(filosofo);
            garfoDireito(filosofo);

            comer(filosofo);
            usleep(100 * 1000);
            // usleep((rand() % 1000) * 1000);

            devolveGarfos(filosofo);
        }
    }
}

void pensar(int filosofo)
{
    estadoFilosofos[filosofo][0] = 'P';
}

void garfoEsquerdo(int filosofo)
{
    estadoFilosofos[filosofo][0] = 'E';

    if (!garfos[filosofo])
    {
        garfos[filosofo] = true;
    }
    else
    {
        while (garfos[filosofo])
        {
            // usleep((rand() % 1000) * 1000);
            // usleep(10 * 1000);
        }
        garfos[filosofo] = true;
    }
}

void garfoDireito(int filosofo)
{
    estadoFilosofos[filosofo][0] = 'E';

    if (!garfos[(filosofo + 1) % (Filosofos - 1)])
    {
        garfos[(filosofo + 1) % (Filosofos - 1)] = true;
    }
    else
    {
        while (garfos[(filosofo + 1) % (Filosofos - 1)])
        {
            // usleep((rand() % 1000) * 1000);
            // usleep(10 * 1000);
        }
        garfos[(filosofo + 1) % (Filosofos - 1)] = true;
    }
}

void comer(int filosofo)
{
    estadoFilosofos[filosofo][0] = 'C';
}

void devolveGarfos(int filosofo)
{
    estadoFilosofos[filosofo][0] = 'P';
    garfos[filosofo] = false;
    garfos[(filosofo + 1) % (Filosofos - 1)] = false;
}