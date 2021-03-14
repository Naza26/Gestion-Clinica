#ifndef HEAP_STRUCT
#define HEAP_STRUCT
#include "heap.h"
#include <stdlib.h>
#include <stdio.h>

struct heap {
    void **datos;
    size_t cant;
    size_t tam;
    cmp_func_t cmp;
};

void swap(void** arreglo, size_t x, size_t y) {
    int* aux = arreglo[x];
    arreglo[x] = arreglo[y]; 
    arreglo[y] = aux;
}

size_t padre_desde_hijo(size_t i) {
    return (i - 1) / 2;
}

size_t hijo_izq_desde_padre(size_t i) {
    return (2 * i) + 1;
}

size_t hijo_der_desde_padre(size_t i) {
    return (2 * i) + 2;
}

void upheap(void **arreglo, size_t largo, size_t hijo, cmp_func_t cmp, size_t contador_largo){
    if(contador_largo < 1){
        return;
    }
    size_t padre = padre_desde_hijo(hijo);
    if(cmp(arreglo[padre], arreglo[hijo]) < 0){ //si el padre es mas chico q su hijo 
        swap(arreglo, padre, hijo);
        contador_largo -= 1;
        upheap(arreglo, largo, padre, cmp, padre);
    }
}

void downheap(void **arreglo, size_t largo, size_t posicion_padre, cmp_func_t cmp, size_t contador)
{
    if (contador >= largo){
        return;
    }
    size_t h_izq = hijo_izq_desde_padre(posicion_padre);
    size_t h_der = hijo_der_desde_padre(posicion_padre);
    size_t max = 0;
    if (h_izq >= largo && h_der >= largo){
        return;
    }
    if (h_der >= largo && h_izq < largo){
        max = h_izq;
        if (cmp(arreglo[max], arreglo[posicion_padre]) < 0){
            return;
        }
    }
    if (h_izq < largo && h_der < largo){
        if (cmp(arreglo[h_der], arreglo[posicion_padre]) > 0){
            if (cmp(arreglo[h_izq], arreglo[h_der]) > 0){
                max = h_izq;
            }
            else{
                max = h_der;
            }
        }
        else{
            if (cmp(arreglo[h_izq], arreglo[posicion_padre]) > 0){
                max = h_izq;
            }
            else{
                return;
            }
        }
    }
    swap(arreglo, posicion_padre, max); // Cambio al padre con el elemento de mayor prioridad
    contador += 1;
    downheap(arreglo, largo, max, cmp, contador);
}

heap_t *heap_crear(cmp_func_t cmp){
    heap_t *heap = malloc(sizeof(heap_t));
    if(!heap){
        return NULL;
    }
    heap->cant = 0;
    heap->tam = 10;
    heap->cmp = cmp;
    heap->datos = malloc((heap->tam) * sizeof(void**));
    if(!heap->datos){
        return NULL;
    }
    return heap;
}

size_t heap_cantidad(const heap_t *heap){
    return heap->cant;
}

bool heap_esta_vacio(const heap_t *heap){
    if(!heap->datos || heap->cant == 0){
        return true;
    }
    return false;
}

void *heap_ver_max(const heap_t *heap){
    if(heap_esta_vacio(heap)){
        return NULL;
    }
    return heap->datos[0];
}

bool heap_redimensionar(heap_t *heap, bool aumentar){
    if(aumentar){
        heap->tam = heap->tam * 2;
    } else {
        heap->tam = heap->tam / 2;
    }
    void **nuevo_tamanio = realloc(heap->datos, heap->tam * sizeof(void**));
    if(!nuevo_tamanio) return false;
    heap->datos = nuevo_tamanio;
    return true;
}

bool heap_encolar(heap_t *heap, void *arr){
    if(heap->cant == heap->tam){ 
        if(!heap_redimensionar(heap, true)) return false;
    }
    heap->datos[heap->cant] = arr;
    upheap(heap->datos, heap->cant, heap->cant, heap->cmp, heap->cant);
    heap->cant++;
    return true;
}

void *heap_desencolar(heap_t *heap){
    if(heap->cant == 0){
        return NULL;
    }
    void **desencolado = heap->datos[0];
    if(heap->cant == 1){
        heap->cant--;
        return desencolado;
    }
    swap(heap->datos, 0, heap->cant-1);
    heap->cant--;
    downheap(heap->datos, heap->cant, 0, heap->cmp, 0);
    if(heap->cant * 4 <= heap->cant){
        if(!heap_redimensionar(heap, false)){
            return heap->datos[0];
        }
    }
    return desencolado;
}

void heap_destruir(heap_t *heap, void (*destruir_elemento)(void *e)){
    if(destruir_elemento != NULL){
        size_t largo = heap->cant;
        for (size_t i = 0; i < largo; i++)
        {
            void *elemento = heap->datos[i];
            destruir_elemento(elemento);
        }
    }
    free(heap->datos);
    free(heap);
}

void heapify(void **elementos, size_t largo, cmp_func_t cmp){
    size_t contador = largo / 2;
    while(contador > 0){
        downheap(elementos, largo, contador-1, cmp, 0);
        contador--;
    }
}

heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp){

    // Hacemos hepaify sobre el heap ya creado.
    heap_t *heap = malloc(sizeof(heap_t));
    if(!heap){
        return NULL;
    }
    heap->tam = n;
    heap->cant = n;
    heap->cmp = cmp;
    heap->datos = malloc((n) * sizeof(void**));
    if(!heap->datos){
        free(heap);
        return NULL;
    }
    for (size_t i = 0; i < n; i++){
       heap->datos[i] = arreglo[i];
    }
    heapify(heap->datos,n,cmp);
    return heap;
}

void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp){
    if(cant == 0) return;
    heapify(elementos, cant, cmp);
    size_t ult_relativo_pos = cant-1;
    while(ult_relativo_pos > 0){
        swap(elementos, 0, ult_relativo_pos);
        ult_relativo_pos--;
        downheap(elementos, ult_relativo_pos+1, 0, cmp, 0);
        //ult_relativo_pos--;
    }
}

void print_heap(heap_t *heap){
    for (size_t i = 0; i < heap->cant; i++){
        printf("%i -> ", *(int*)heap->datos[i]);
    }
    printf("\n");
}

#endif
