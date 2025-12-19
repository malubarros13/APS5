#ifndef AUXILIAR_H
#define AUXILIAR_H

#include <stdio.h>
#include "processo.h"

//autoexplicativo
int existem_processos_ativos(processo *processos, int n);

//autoexplicativo
void inicializar_processos(processo *processos, int n); 

void imprimir_resultados(
    processo *processos,
    int n,
    int tempo_total,
    int num_trocas,
    int tempo_trocas,
    FILE *saida
);

#endif