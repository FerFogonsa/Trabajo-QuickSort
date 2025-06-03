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

    for (int i=0 ; i<TAM ; i++){  // Recorremos el arreglo
        arr[i] = rand() % 1000000;    // Asignamos un numero aleatorio entre 0 y 999999
    }

    clock_t inicio = clock();       // Capturamos el tiempo actual antes de comenzar a ordenar

    int pi = partition(arr, 0, TAM - 1); // Particionamos el arreglo una vez en el main y obtenemos el índice del pivote

    pthread_t thread1, thread2;     // Declaramos dos threads

    QuickSortArgs* args1 = malloc(sizeof(QuickSortArgs));    // Reservamos memoria para los argumentos del primer thread
    args1->arr = arr;        // Asignamos el arreglo
    args1->low = 0;          // Índice inicial para la primera mitad
    args1->high = pi - 1;    // Índice final de la primera mitad

    QuickSortArgs* args2 = malloc(sizeof(QuickSortArgs)); // Reservamos memoria para los argumentos del segundo hilo
    args2->arr = arr;        // Asignamos el arreglo
    args2->low = pi + 1;     // Índice inicial de la segunda mitad
    args2->high = TAM - 1;   // Índice final del arreglo

    
    pthread_create(&thread1, NULL, quickSortThread, (void*)args1); // Iniciamos el primer hilo para ordenar la primera mitad
    pthread_create(&thread2, NULL, quickSortThread, (void*)args2); // Iniciamos el segundo hilo para ordenar la segunda mitad

    
    pthread_join(thread1, NULL);    // Esperamos que el primer hilo finalice
    pthread_join(thread2, NULL);    // Esperamos que el segundo hilo finalice

    clock_t fin = clock();          // Capturamos el tiempo tras finalizar la ejecución

    double tiempo = (double)(fin - inicio) / CLOCKS_PER_SEC; // Calculamos el tiempo en segundos

    
    printf("Tiempo de ejecución (concurrente): %f segundos\n", tiempo);    // Mostramos el tiempo de ejecución

    // (Opcional) Mostrar el arreglo ordenado
    /*
    for (int i = 0; i < TAM; i++) {
        printf("%d ", arr[i]);    //Imprimimos cada elemento ordenado
    }
    */

    return 0;    //Fin del programa
}
