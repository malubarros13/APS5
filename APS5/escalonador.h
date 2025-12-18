#ifndef ESCALONADOR_H
#define ESCALONADOR_H

#include <stdio.h>
#include "processo.h"

/* 
 * Simula o algoritmo Round Robin (preemptivo)
 * processos: vetor de processos
 * n: número de processos
 * quantum: fatia de tempo
 * t_troca: tempo de troca de contexto
 * saida: arquivo de saída
 */
void escalonamento_round_robin(
    processo *processos,
    int n,
    int quantum,
    int t_troca,
    FILE *saida
);

/*
 * Simula o escalonamento por Prioridade (preemptivo)
 * processos: vetor de processos
 * n: número de processos
 * t_troca: tempo de troca de contexto
 * saida: arquivo de saída
 */
void escalonamento_prioridade(
    processo *processos,
    int n,
    int t_troca,
    FILE *saida
);

#endif
