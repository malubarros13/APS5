#ifndef GERAL_PROCESS_H
#define GERAL_PROCESS_H

#include <stdio.h>
#include "processo.h"

// Verifica se ainda existe algum processo não finalizado
int existem_processos_ativos(processo *processos, int n);

// Inicializa campos auxiliares dos processos
void inicializar_processos(processo *processos, int n);

// Calcula e imprime métricas finais
void imprimir_resultados(
    processo *processos,
    int n,
    int tempo_total,
    int num_trocas,
    int tempo_trocas,
    FILE *saida
);

#endif