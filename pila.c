#include "pila.h"
#include <stdlib.h>
#define CAPACIDAD 10
#define CANTIDAD 0

/* Definición del struct pila proporcionado por la cátedra.
 */
struct pila
{
    void **datos;
    size_t cantidad;  // Cantidad de elementos almacenados.
    size_t capacidad; // Capacidad del arreglo 'datos'.
};

/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/

pila_t *pila_crear(void)
{

    pila_t *pila = malloc(sizeof(pila_t));

    if (pila == NULL){
        free(pila);
        return NULL;
    }

    pila->capacidad = CAPACIDAD;
    pila->cantidad = CANTIDAD;
    pila->datos = malloc((pila->capacidad) * sizeof(void *));

    return pila;
}

void pila_destruir(pila_t *pila){

    free(pila->datos);
    free(pila);
}

bool pila_esta_vacia(const pila_t *pila){

    return pila->cantidad == 0;
}

bool pila_apilar(pila_t *pila, void *valor){

    void *nuevos_datos = NULL;

    if (pila->cantidad == pila->capacidad - 1){
        pila->capacidad = pila->capacidad * 2;
        nuevos_datos = realloc(pila->datos, pila->capacidad * sizeof(void *));
        pila->datos = nuevos_datos;
    }

    pila->datos[pila->cantidad] = valor;
    pila->cantidad++;
    return true;
}

void *pila_ver_tope(const pila_t *pila){

    if (pila->cantidad == 0){
        return NULL;
    }

    return pila->datos[pila->cantidad - 1];
}

void *pila_desapilar(pila_t *pila)
{

    void *nuevos_datos = NULL;

    if (pila_esta_vacia(pila)){
        return NULL;
    }

    if (pila->cantidad * 4 <= pila->capacidad){
        pila->capacidad = pila->capacidad / 2;
        nuevos_datos = realloc(pila->datos, pila->capacidad * sizeof(void *));
        pila->datos = nuevos_datos;
    }

    pila->cantidad--;
    return pila->datos[pila->cantidad];
}
