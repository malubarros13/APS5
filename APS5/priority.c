#include <stdio.h>
#include <stdlib.h>

#include "priority.h"
#include "auxiliar.h"

void escalonamento_prioridade(
    processo *processos,
    int n,
    int t_troca,
    FILE *saida
) {
    int tempo_atual = 0;
    int trocas_contexto = 0;
    int tempo_gasto_trocas = 0;

    inicializar_processos(processos, n);

    fprintf(saida, "=== Escalonamento por Prioridade ===\n");

    int em_execucao = -1;

    while (existem_processos_ativos(processos, n)) {

        int melhor = -1;

        for (int i = 0; i < n; i++) {
            if (processos[i].t_chegada <= tempo_atual &&
                processos[i].t_restante > 0) {

                if (melhor == -1 || processos[i].prioridade < processos[melhor].prioridade ||
                    (processos[i].prioridade == processos[melhor].prioridade && processos[i].id < processos[melhor].id)) {
                        
                    melhor = i;
                }
            }
        }   

        if (melhor == -1) {
            tempo_atual++;
            continue;
        }

        if (melhor != em_execucao) {
            fprintf(saida, "%d-%d: Escalonador\n",
                tempo_atual, 
                tempo_atual + t_troca - 1
            );

            tempo_atual += t_troca;
            trocas_contexto++;
            tempo_gasto_trocas += t_troca;
        }

        em_execucao = melhor;

        fprintf(saida, "%d-%d: P%d\n",
            tempo_atual, tempo_atual,
            processos[em_execucao].id);

        processos[em_execucao].t_restante--;
        tempo_atual++;

        if (processos[em_execucao].t_restante == 0) {
            processos[em_execucao].t_fim = tempo_atual;
            em_execucao = -1;
        }
}


    imprimir_resultados(
        processos,
        n,
        tempo_atual,
        trocas_contexto,
        tempo_gasto_trocas,
        saida
    );

    fprintf(saida, "\n");
}