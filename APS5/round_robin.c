#include <stdio.h>
#include <stdlib.h>

#include "round_robin.h"
#include "geral_process.h"

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

    // Inicializa os processos
    inicializar_processos(processos, n);

    fprintf(saida, "=== Escalonamento Round Robin ===\n");

    /*
     * AQUI VAI A LÓGICA DO ROUND ROBIN:
     *
     * - controlar fila de prontos
     * - inserir processos quando t_chegada == tempo_atual
     * - escolher próximo processo
     * - executar até quantum ou término
     * - tratar preempção
     * - registrar linha do tempo
     * - contabilizar trocas de contexto
     */
    int fila[100];           // fila de prontos (IDs armazenados como índice)
    int ini = 0, fim = 0;    // controle da fila
    int em_execucao = -1;    // índice do processo atual
    int concluidos = 0;

    while (concluidos < n) {
        // 1. Verificar quem chegou e colocar na fila (Critério ID se chegarem juntos)
        for (int i = 0; i < n; i++) {
            if (processos[i].t_chegada == tempo_atual) {
                fila[fim++] = i;
            }
        }

        // 2. Se a CPU está livre e há alguém na fila
        if (em_execucao == -1 && ini < fim) {
            em_execucao = fila[ini++];
            
            // Registra a troca de contexto conforme o PDF [cite: 38]
            fprintf(saida, "%d-%d: Escalonador\n", tempo_atual, tempo_atual + t_troca - 1);
            
            // Durante o tempo de troca de contexto, novos processos podem chegar!
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

            // Simula o tempo passando para checar novas chegadas durante a execução
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
                // Se não terminou, volta para o fim da fila
                fila[fim++] = p;
                em_execucao = -1;
            }
        } 
        else{
            // CPU ociosa
            tempo_atual++;
        }
    }

    imprimir_resultados(processos, n, tempo_atual, trocas_contexto, tempo_gasto_trocas, saida);
}