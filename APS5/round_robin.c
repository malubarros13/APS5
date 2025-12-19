#include <stdio.h>
#include <stdlib.h>

#include "round_robin.h"
#include "auxiliar.h"

void escalonamento_round_robin(
    processo *processos,
    int n,
    int quantum,
    int t_troca,
    FILE *saida
) {
    int tempo_atual = 0;
    int trocas_contexto = 0;
    int tempo_gasto_trocas = 0;

    inicializar_processos(processos, n);

    fprintf(saida, "=== Escalonamento Round Robin ===\n");

    int *fila = (int*) malloc(n * sizeof(int));
    int ini = 0, fim = 0; 
    int em_execucao = -1; 
    int concluidos = 0;

    while (concluidos < n) {
        for (int i = 0; i < n; i++) {
            if (processos[i].t_chegada == tempo_atual) {
                fila[fim++] = i;
            }
        }

        if (em_execucao == -1 && ini < fim) {
            em_execucao = fila[ini++];
            
            fprintf(saida, "%d-%d: Escalonador\n", tempo_atual, tempo_atual + t_troca - 1);
            
            for (int t = 1; t <= t_troca; t++) {
                tempo_atual++;
                for (int i = 0; i < n; i++) {
                    if (processos[i].t_chegada == tempo_atual) {
                        fila[fim++] = i;
                    }
                }
            }
            trocas_contexto++;
            tempo_gasto_trocas += t_troca;
        }

        if (em_execucao != -1) {
            int p = em_execucao;
            int tempo_exec = (processos[p].t_restante < quantum) ? processos[p].t_restante : quantum;

            fprintf(saida, "%d-%d: P%d\n", tempo_atual, tempo_atual + tempo_exec - 1, processos[p].id);

            for (int t = 1; t <= tempo_exec; t++) {
                tempo_atual++;
                for (int i = 0; i < n; i++) {
                    if (processos[i].t_chegada == tempo_atual) {
                        fila[fim++] = i;
                    }
                }
                processos[p].t_restante--;
            }

            if (processos[p].t_restante == 0) {
                processos[p].t_fim = tempo_atual;
                concluidos++;
                em_execucao = -1;
            } else {
                fila[fim++] = p;
                em_execucao = -1;
            }
        } 
        else{
            //CPU ociosa
            tempo_atual++;
        }
    }

    imprimir_resultados(processos, n, tempo_atual, trocas_contexto, tempo_gasto_trocas, saida);
}