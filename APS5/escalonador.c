#include <stdio.h>
#include <stdlib.h>
#include "escalonador.h"

/* -------------------------------------------------
   Funções auxiliares (protótipos internos)
   ------------------------------------------------- */

// Verifica se ainda existe algum processo não finalizado
static int existem_processos_ativos(processo *processos, int n);

// Inicializa campos auxiliares dos processos
static void inicializar_processos(processo *processos, int n);

// Calcula e imprime métricas finais
static void imprimir_resultados(
    processo *processos,
    int n,
    int tempo_total,
    int num_trocas,
    int tempo_trocas,
    FILE *saida
);

/* -------------------------------------------------
   Escalonamento Round Robin (preemptivo)
   ------------------------------------------------- */
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
        } else {
            // CPU ociosa
            tempo_atual++;
        }
    }

    imprimir_resultados(processos, n, tempo_atual, trocas_contexto, tempo_gasto_trocas, saida);
}


/* -------------------------------------------------
   Escalonamento por Prioridade (preemptivo)
   ------------------------------------------------- */
void escalonamento_prioridade(
    processo *processos,
    int n,
    int t_troca,
    FILE *saida
) {
    int tempo_atual = 0;
    int trocas_contexto = 0;
    int tempo_gasto_trocas = 0;

    // Inicializa os processos
    inicializar_processos(processos, n);

    fprintf(saida, "=== Escalonamento por Prioridade ===\n");

    /*
     * AQUI VAI A LÓGICA DO ESCALONAMENTO POR PRIORIDADE:
     *
     * - escolher processo de menor prioridade numérica
     * - tratar chegada de novos processos
     * - realizar preempção se chegar processo mais prioritário
     * - registrar linha do tempo
     * - contabilizar trocas de contexto
     */

    int em_execucao = -1;

while (existem_processos_ativos(processos, n)) {

    int melhor = -1;

    /* Escolher processo pronto de maior prioridade */
    for (int i = 0; i < n; i++) {
        if (processos[i].t_chegada <= tempo_atual &&
            processos[i].t_restante > 0) {

            if (melhor == -1 ||
                processos[i].prioridade < processos[melhor].prioridade ||
               (processos[i].prioridade == processos[melhor].prioridade &&
                processos[i].id < processos[melhor].id)) {
                melhor = i;
            }
        }
    }

    /* Nenhum processo pronto */
    if (melhor == -1) {
        tempo_atual++;
        continue;
    }

    /* Troca de contexto */
    if (melhor != em_execucao && em_execucao != -1) {
        fprintf(saida, "%d-%d: Escalonador\n",
                tempo_atual, tempo_atual + t_troca - 1);

        tempo_atual += t_troca;
        trocas_contexto++;
        tempo_gasto_trocas += t_troca;
    }

    em_execucao = melhor;

    /* Executa 1 unidade de tempo */
    fprintf(saida, "%d-%d: P%d\n",
            tempo_atual, tempo_atual,
            processos[em_execucao].id);

    processos[em_execucao].t_restante--;
    tempo_atual++;

    /* Processo terminou */
    if (processos[em_execucao].t_restante == 0) {
        processos[em_execucao].t_fim = tempo_atual;
        em_execucao = -1;
    }
}


    // Impressão dos resultados finais
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

/* -------------------------------------------------
   Implementação das funções auxiliares
   ------------------------------------------------- */

static void inicializar_processos(processo *processos, int n) {
    for (int i = 0; i < n; i++) {
        processos[i].t_restante = processos[i].t_cpu;
        processos[i].t_fim = -1;
    }
}

static int existem_processos_ativos(processo *processos, int n) {
    for (int i = 0; i < n; i++) {
        if (processos[i].t_restante > 0) {
            return 1;
        }
    }
    return 0;
}

static void imprimir_resultados(
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
}
