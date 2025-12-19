#ifndef ROUND_ROBIN_H
#define ROUND_ROBIN_H

#include <stdio.h>
#include "processo.h"

void escalonamento_round_robin(
    processo *processos, //array de processoss
    int n_process,
    int quantum, //tempo máximo que o processo vai ficar
    int t_troca, //tempo de troca de contexti
    FILE *saida
);

#endif