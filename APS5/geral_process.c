#include <stdio.h>
#include <stdlib.h>
#include "geral_process.h"

/* -------------------------------------------------
   Funções auxiliares (protótipos internos)
   ------------------------------------------------- */

/* -------------------------------------------------
   Escalonamento Round Robin (preemptivo)
   ------------------------------------------------- */

/* -------------------------------------------------
   Escalonamento por Prioridade (preemptivo)
   ------------------------------------------------- */

/* -------------------------------------------------
   Implementação das funções auxiliares
   ------------------------------------------------- */

void inicializar_processos(processo *processos, int n) {
    for (int i = 0; i < n; i++) {
        processos[i].t_restante = processos[i].t_cpu;
        processos[i].t_fim = -1;
    }
}

int existem_processos_ativos(processo *processos, int n) {
    for (int i = 0; i < n; i++) {
        if (processos[i].t_restante > 0) {
            return 1;
        }
    }
    return 0;
}

void imprimir_resultados(
    processo *processos,
    int n,
    int tempo_total,
    int num_trocas,
    int tempo_trocas,
    FILE *saida
) {
    double tempo_medio_retorno = 0.0;

    for (int i = 0; i < n; i++) {
        tempo_medio_retorno += (processos[i].t_fim - processos[i].t_chegada);
    }

    tempo_medio_retorno /= n;

    fprintf(saida, "\n--- Resultados ---\n");
    fprintf(saida, "Tempo medio de retorno: %.2f\n", tempo_medio_retorno);
    fprintf(saida, "Numero de trocas de contexto: %d\n", num_trocas);

    if (tempo_total > 0) {
        fprintf(saida, "Overhead de troca de contexto: %.2f\n",
                (double) tempo_trocas / tempo_total);
    } else {
        fprintf(saida, "Overhead de troca de contexto: 0.00\n");
    }

    fprintf(saida, "Tempo total da simulacao: %d\n", tempo_total);

    fprintf(saida, "\n");
    fprintf(saida, "\n");
    fprintf(saida, "\n");
}
