#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Obter o tamanho da página do sistema
long getPageSize() {
    return sysconf(_SC_PAGESIZE);
}

// Imprimir as informações da memória do processo
void printMemoryInfo(pid_t pid) {
    char path[40];
    sprintf(path, "/proc/%d/statm", pid);

    // Checa se o arquivo existe antes de tentar abri-lo
    if (access(path, F_OK) != 0) {
        perror("Falha ao acessar o arquivo");
        return;
    }

    FILE *fp = fopen(path, "r");
    if (fp == NULL) {
        perror("Falha ao abrir o arquivo");
        return;
    }

    unsigned long size, resident;
    if (fscanf(fp, "%lu %lu", &size, &resident) != 2) {
        perror("Falha ao ler informações de memória");
        fclose(fp);
        return;
    }
    fclose(fp);

    long pageSize = getPageSize();
    printf("Tamanho da memória virtual do processo: %lu KB\n", size * pageSize / 1024);
    printf("Páginas na memória real (RAM): %lu\n", resident);
    printf("Tamanho da página do sistema: %ld bytes\n", pageSize);
}

int main() {
    pid_t pid;

    printf("Digite o PID do processo a ser monitorado: ");
    scanf("%d", &pid);

    while (1) {
        system("clear");
        printMemoryInfo(pid);
        sleep(1); // Espera 1 segundo para a próxima atualização
    }

    return 0;
}

