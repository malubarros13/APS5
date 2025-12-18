#include <stdio.h>
#include <stdlib.h>
#include "processo.h"
#include "escalonador.h"

int main() {

    int n_Proc, quantum, t_Troca; //infos da primeira linha
    processo *array_process; //auto explicativo

    FILE *entrada_texto = NULL;
    FILE *saida_texto = NULL;

    entrada_texto = fopen("EntradaProcessos.txt", "r");
    saida_texto = fopen("SaidaProcessos.txt", "w");

    if(entrada_texto == NULL || saida_texto == NULL){
        printf("Erro ao abrir arquivo");
    } //isso foi indicado pela IA (gemini) para saber onde foi o erro caso ocorresse

    fscanf(entrada_texto, "%d,%d,%d", 
        &n_Proc,
        &quantum,
        &t_Troca
    ); //isso é a leitura da primeira linha

    //criando array para armazenar processos (sintaxe bem diferente de C++)
    array_process = malloc(n_Proc * sizeof(processo)); //análogo ao new processo(n_proc);
    
    //lendo linha por linha
    for(int i = 0; i < n_Proc; i++){
        fscanf(entrada_texto, "%d,%d,%d,%d", 
            &array_process[i].id,
            &array_process[i].t_chegada,
            &array_process[i].prioridade,
            &array_process[i].t_cpu
        );
    }

    //criar a FILA para 


    fclose(entrada_texto);
    fclose(saida_texto);

    free(array_process); //análogo ao delete [] array process;

    
    return 0;
}
