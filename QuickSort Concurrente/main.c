#include <stdio.h>      // Para printf
#include <stdlib.h>     // Para malloc, rand, srand
#include <pthread.h>    // Para usar threads
#include <time.h>       // Para medir el tiempo con clock()

#define TAM 500000    // Tamaño del arreglo grande

// Estructura para pasar los argumentos a cada thread
typedef struct {
    int* arr;   // Puntero al arreglo a ordenar
    int low;    // Índice inferior del subarreglo
    int high;   // Índice superior del subarreglo
} QuickSortArgs;

// Prototipos de funciones
void* quickSortThread(void* args);     // Función que ejecuta el QuickSort en un thread
void quickSort(int* arr, int low, int high); // QuickSort recursivo (secuencial)
int partition(int arr[], int low, int high); // Función de partición
void swap(int* a, int* b);             // Intercambio de elementos

// Intercambia dos valores enteros usando punteros
void swap(int* a, int* b){
    int t = *a;
    *a = *b;
    *b = t;
}

// Función de partición (igual que en la versión secuencial)
int partition(int arr[], int low, int high){
    int pivot = arr[high];     // Elegimos el último como pivote
    int i = low-1;           // Índice para elementos menores

    for (int j=low ; j<=high-1 ; j++){
        if (arr[j] < pivot){  // Si es menor que el pivote
            i++;               // Avanzamos el índice de menores
            swap(&arr[i], &arr[j]); // Intercambiamos
        }
    }

    swap(&arr[i + 1], &arr[high]); // Ponemos el pivote en su lugar
    return i + 1;                  // Retornamos índice del pivote
}

// QuickSort recursivo secuencial
void quickSort(int* arr, int low, int high){
    if (low < high){
        int pi = partition(arr, low, high);  // Particionamos

        quickSort(arr, low, pi - 1);         // Ordenamos parte izquierda
        quickSort(arr, pi + 1, high);        // Ordenamos parte derecha
    }
}

// Función que corre en un thread (envuelve al quickSort secuencial)
void* quickSortThread(void* args){
    QuickSortArgs* qsArgs = (QuickSortArgs*)args;           // Cast del argumento a nuestra estructura
    quickSort(qsArgs->arr, qsArgs->low, qsArgs->high);      // Llamamos quickSort normal
    free(qsArgs);                                            // Liberamos la memoria del struct
    return NULL;
}

int main() {

    int arr[TAM];                    // Declaramos un arreglo de TAM elementos

    srand(time(NULL));              // Inicializamos la semilla para rand()

    for (int i=0 ; i<TAM ; i++){ // Llenamos el arreglo con números aleatorios
        arr[i] = rand() % 1000000;
    }

    clock_t inicio = clock();       // Medimos el tiempo de inicio

    int pi = partition(arr, 0, TAM - 1); // Particionamos el arreglo una vez en el main

    pthread_t thread1, thread2;     // Declaramos dos threads

    // Creamos los argumentos para el primer thread
    QuickSortArgs* args1 = malloc(sizeof(QuickSortArgs));
    args1->arr = arr;
    args1->low = 0;
    args1->high = pi - 1;

    // Creamos los argumentos para el segundo thread
    QuickSortArgs* args2 = malloc(sizeof(QuickSortArgs));
    args2->arr = arr;
    args2->low = pi + 1;
    args2->high = TAM - 1;

    // Creamos los threads para ordenar ambas mitades del arreglo
    pthread_create(&thread1, NULL, quickSortThread, (void*)args1);
    pthread_create(&thread2, NULL, quickSortThread, (void*)args2);

    // Esperamos a que ambos threads terminen su ejecución
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    clock_t fin = clock();          // Tiempo de fin

    double tiempo = (double)(fin - inicio) / CLOCKS_PER_SEC; // Calculamos el tiempo en segundos

    // Mostramos el tiempo de ejecución
    printf("Tiempo de ejecución (concurrente): %f segundos\n", tiempo);

    // (Opcional) Mostrar el arreglo ordenado
    /*
    for (int i = 0; i < TAM; i++) {
        printf("%d ", arr[i]);
    }
    */

    return 0;
}
