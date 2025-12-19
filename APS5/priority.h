#ifndef PRIORITY_H
#define PRIORITY_H

#include <stdio.h>
#include "processo.h"

void escalonamento_prioridade(
    processo *processos, //array de processos
    int n_process,
    int t_troca, //tempo de troca de contexto
    FILE *saida
);

#endif