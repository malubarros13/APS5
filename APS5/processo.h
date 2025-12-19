#ifndef PROCESSO_H
#define PROCESSO_H

typedef struct {
    int id;        
    int t_chegada; //tempo de chegada
    int prioridade;
    int t_cpu; //tempo TOTAL de CPU necessário
    int t_restante;//de CPU
    int t_fim; //quando o processo termina
} processo;


#endif
