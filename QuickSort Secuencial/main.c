#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM 500000  // Tamaño grande para evaluar rendimiento

// Prototipo de la función swap que se usará para intercambiar dos valores
void swap(int* a, int* b);

// Función que realiza la partición del arreglo
int partition(int arr[], int low, int high){

    // Elegimos el último elemento como pivote
    int pivot = arr[high];

    // Inicializamos el índice del menor elemento
    // i indica la posición final del pivote en esta pasada
    int i = low-1;

    // Recorremos el subarreglo desde low hasta high - 1
    for(int j = low; j <= high - 1; j++){
        // Si el elemento actual es menor que el pivote
        if(arr[j] < pivot){
            i++;  // Avanzamos el índice del menor elemento
            swap(&arr[i], &arr[j]);  // Intercambiamos el elemento actual con el que está en la posición i
        }
    }

    // Al final, colocamos el pivote en su posición correcta (después de los menores)
    swap(&arr[i+1], &arr[high]);

    // Devolvemos la posición final del pivote
    return i+1;
}

// Función principal de QuickSort (recursiva)
void quickSort(int arr[], int low, int high){

    // Si aún hay más de un elemento para ordenar
    if(low < high){

        // Obtenemos el índice de partición del pivote
        int pi = partition(arr, low, high);

        // Ordenamos recursivamente los elementos antes y después del pivote
        quickSort(arr, low, pi - 1);    // Lado izquierdo del pivote
        quickSort(arr, pi + 1, high);   // Lado derecho del pivote
    }
}

// Función que intercambia dos valores enteros utilizando punteros
void swap(int* a, int* b){
    int t = *a;  // Guardamos temporalmente el valor de a
    *a = *b;     // Asignamos a el valor de b
    *b = t;      // Asignamos a b el valor guardado originalmente en a
}

int main(){
    // Declaramos el arreglo grande
    int arr[TAM];

    // Inicializamos la semilla para números aleatorios
    srand(time(NULL));

    // Llenamos el arreglo con números aleatorios de 0 a 999999
    for(int i=0; i<TAM; i++){
        arr[i] = rand() % 1000000;
    }

    // Medimos el tiempo de inicio
    clock_t inicio = clock();

    // Llamamos a la función QuickSort sobre todo el arreglo
    quickSort(arr, 0, TAM-1);

    // Medimos el tiempo de finalización
    clock_t fin = clock();
    double tiempo = (double)(fin-inicio) / CLOCKS_PER_SEC;

    // Mostramos el tiempo de ejecución
    printf("Tiempo de ejecución: %f segundos\n", tiempo);

    // Mostrar arreglo ordenado
    /*
    for (int i = 0; i < TAM; i++){
        printf("%d ", arr[i]);
    }
    */

    // Fin del programa
    return 0;
}
