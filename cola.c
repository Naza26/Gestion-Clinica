#include "cola.h"
#include <stdlib.h>

typedef struct nodo {
    void* dato;
    struct nodo* sig;
} nodo_t;

struct cola {
    nodo_t* prim;
    nodo_t* ult;
    size_t cantidad;
};

cola_t *cola_crear(void){

    cola_t *cola = malloc(sizeof(cola_t));

    if(cola == NULL){
        return NULL;
    }

    cola->prim = NULL;
    cola->ult = NULL;
    cola->cantidad = 0;
    return cola;

}

void cola_destruir(cola_t *cola, void (*destruir_dato)(void *)){

    while(!cola_esta_vacia(cola)){

        void *dato = cola_desencolar(cola);

        if(destruir_dato != NULL){
            destruir_dato(dato);
        }
    }
    free(cola);
}

bool cola_esta_vacia(const cola_t *cola){
    return cola->prim == NULL;
}

bool cola_encolar(cola_t *cola, void *valor){

    nodo_t *nuevo_nodo = malloc(sizeof(nodo_t));

    if(nuevo_nodo == NULL){
        return false;
    }

    nuevo_nodo->dato = valor;
    nuevo_nodo->sig = NULL;

    if(!cola_esta_vacia(cola)){
        cola->ult->sig = nuevo_nodo;
    } else {
        cola->prim = nuevo_nodo;   
    }
    cola->ult = nuevo_nodo;
    cola->cantidad++;
    return true;
}

void *cola_ver_primero(const cola_t *cola){
    return cola_esta_vacia(cola) ? NULL : cola->prim->dato;
}

void *cola_desencolar(cola_t *cola){

    if(cola_esta_vacia(cola)){
        return NULL;
    }

    if(cola->prim == NULL){
        cola->ult = NULL;
    }

    nodo_t *nodo_a_desenc = cola->prim;
    void *dato = nodo_a_desenc->dato;
    cola->prim = cola->prim->sig;
    cola->cantidad--;
    free(nodo_a_desenc);
    return dato;

}

size_t cola_cantidad(cola_t *cola){
    return cola->cantidad;
}