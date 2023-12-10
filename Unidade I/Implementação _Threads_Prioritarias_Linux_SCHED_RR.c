#include <stdio.h>       // Inclusão da biblioteca para funções de entrada e saída
#include <pthread.h>     // Inclusão da biblioteca para trabalhar com threads
#include <sched.h>       // Inclusão da biblioteca para definir políticas de escalonamento
#include <stdlib.h>      // Inclusão da biblioteca padrão
#include <unistd.h>      // Inclusão da biblioteca para chamadas de sistema do Unix
#include <time.h>        // Inclusão da biblioteca para funções de tempo

// Função para inicializar uma matriz com valores aleatórios
void initializeMatrix(int *matrix, int size)
{
    for(int i = 0; i < size; i++)
        for(int j = 0; j < size; j++)
            matrix[i * size + j] = rand() % (10 - 1) * 1;
}

// Estrutura para armazenar dados das threads
typedef struct {
    int size;        // Tamanho da matriz
    int priority;    // Prioridade da thread
} thread_data;

// Função que será executada pelas threads
void* thread_matriz(void* arg) 
{
    thread_data* data = (thread_data*) arg;
    int size = data->size;
    int priority = data->priority;

    int i, j, k;
    struct timespec start_time, end_time;  // Variáveis para calcular o tempo de execução
    double elapsed_time;

    // Alocação de memória para as matrizes A, B e C
    int  *A = (int *) malloc (sizeof(int)*size*size);
    int  *B = (int *) malloc (sizeof(int)*size*size);
    int  *C = (int *) malloc (sizeof(int)*size*size);
    
    initializeMatrix(A, size);  // Inicialização da matriz A
    initializeMatrix(B, size);  // Inicialização da matriz B
    
    // Captura o tempo inicial
    clock_gettime(CLOCK_MONOTONIC, &start_time);
    // Multiplicação das matrizes A e B
    for(i = 0; i < size; i++)
        for(j = 0; j < size; j++)
            for(k = 0; k < size; k++)
            {
                C[i * size + j] += A[i * size + k] * B[k * size + j];
            }
    // Captura o tempo final
    clock_gettime(CLOCK_MONOTONIC, &end_time);

    // Liberação da memória alocada para as matrizes
    free(A);
    free(B);
    free(C);
    
    // Cálculo do tempo total de execução
    elapsed_time = (end_time.tv_sec - start_time.tv_sec) + 1e-9*(end_time.tv_nsec - start_time.tv_nsec);   
    unsigned long thread_id = (unsigned long)pthread_self();
    // Impressão dos resultados
    printf("| ID Thread: %lu | Prioridade: %d | Duração: %.2f segundos |\n", thread_id, priority, elapsed_time);

    return NULL;
}

int main() 
{
    thread_data data[5];
    // Inicialização dos dados das threads
    for(int i = 0; i < 5; i++) {
        data[i].size = 1000;        // Tamanho da matriz
        data[i].priority = i + 1;  // Prioridade da thread
    }

    pthread_t threads[5];
    struct sched_param param;     // Estrutura para definir a prioridade das threads
    int policy = SCHED_RR;        // Política de escalonamento Round Robin
    
    printf("+---------------------------------------------------------------------+\n");
    // Criação das threads e definição de suas prioridades
    for (int i = 0; i < 5; i++) 
    {
        pthread_create(&threads[i], NULL, thread_matriz, &data[i]);
        param.sched_priority = data[i].priority;
        pthread_setschedparam(threads[i], policy, &param);
    }

    // Espera todas as threads terminarem
    for (int i = 0; i < 5; i++) 
    {
        pthread_join(threads[i], NULL);
    }
    printf("+---------------------------------------------------------------------+\n");

    return 0;
}
