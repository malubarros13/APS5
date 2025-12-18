#ifndef PROCESSO_H
#define PROCESSO_H

// Estrutura que representa um processo do sistema
typedef struct {
    int id;             // Identificador do processo
    int t_chegada;      // Tempo de chegada/criação
    int prioridade;     // Prioridade (menor valor = maior prioridade)
    int t_cpu;          // Tempo TOTAL de CPU necessário
    int t_restante;     // Tempo de CPU que ainda falta executar
    int t_fim;          // Tempo em que o processo termina
} processo;


#endif
