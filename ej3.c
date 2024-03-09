#include <stdio.h>

#define P 5 // número de procesos
#define R 3 // número de recursos


int allocate[P][R];
int maximum[P][R];
int need[P][R];
int available[R];

// Funciones
void initialize();
int request_resources(int process_id, int request[]);
int release_resources(int process_id, int release[]);
int is_safe();
void print_status();

void initialize() {
    // Inicializar allocate, maximum, need y available
    for(int i = 0; i < P; i++) {
        for(int j = 0; j < R; j++) {
            allocate[i][j] = 0;
            maximum[i][j] = 3; // Supongamos que cada proceso puede solicitar hasta 3 unidades de cada recurso
            need[i][j] = maximum[i][j];
        }
    }

    for(int i = 0; i < R; i++) {
        available[i] = 10; // Supongamos que hay 10 unidades de cada recurso disponibles inicialmente
    }
}

int request_resources(int process_id, int request[]) {
    // Verificar y asignar recursos
    for(int i = 0; i < R; i++) {
        if(request[i] > need[process_id][i]) {
            printf("Proceso %d: la solicitud excede la necesidad máxima.\n", process_id);
            return -1;
        }

        if(request[i] > available[i]) {
            printf("Proceso %d: no hay suficientes recursos disponibles.\n", process_id);
            return -1;
        }
    }

    // Asignar recursos y verificar si el sistema está en un estado seguro
    for(int i = 0; i < R; i++) {
        allocate[process_id][i] += request[i];
        available[i] -= request[i];
        need[process_id][i] -= request[i];
    }

    if(!is_safe()) {
        // Si el sistema no está en un estado seguro, revertir la asignación de recursos
        for(int i = 0; i < R; i++) {
            allocate[process_id][i] -= request[i];
            available[i] += request[i];
            need[process_id][i] += request[i];
        }
        printf("Proceso %d: la solicitud ha sido revertida para mantener el sistema en un estado seguro.\n", process_id);
        return -1;
    }

    return 0;
}

int release_resources(int process_id, int release[]) {
    // Liberar recursos
    for(int i = 0; i < R; i++) {
        allocate[process_id][i] -= release[i];
        available[i] += release[i];
        need[process_id][i] += release[i];
    }

    return 0;
}

int is_safe() {
    int work[R];
    int finish[P] = {0};

    for (int i = 0; i < R; i++) {
        work[i] = available[i];
    }

    while (1) {
        int found = 0;
        for (int i = 0; i < P; i++) {
            if (!finish[i]) {
                int j;
                for (j = 0; j < R; j++)
                    if (need[i][j] > work[j])
                        break;
                if (j == R) {
                    for (int k = 0; k < R; k++)
                        work[k] += allocate[i][k];
                    finish[i] = 1;
                    found = 1;
                }
            }
        }
        if (!found) break;
    }

    for (int i = 0; i < P; i++)
        if (!finish[i])
            return 0; // El sistema no está en un estado seguro
    return 1; // El sistema está en un estado seguro
}

void print_status() {
    printf("Proceso\t\tAsignado\tMáximo\t\tNecesario\tDisponible\n");
    for(int i = 0; i < P; i++) {
        printf("P%d\t\t", i);
        for(int j = 0; j < R; j++) {
            printf("%d ", allocate[i][j]);
        }
        printf("\t\t");
        for(int j = 0; j < R; j++) {
            printf("%d ", maximum[i][j]);
        }
        printf("\t\t");
        for(int j = 0; j < R; j++) {
            printf("%d ", need[i][j]);
        }
        if(i == 0) {
            printf("\t\t");
            for(int j = 0; j < R; j++) {
                printf("%d ", available[j]);
            }
        }
        printf("\n");
    }
}





