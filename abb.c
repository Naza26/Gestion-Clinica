#ifndef ABB_STRUCT
#define ABB_STRUCT
#define _POSIX_C_SOURCE 200809L
#include "abb.h"
#include <stdlib.h>
#include <string.h>
#include "pila.h"
#include <stdio.h>

typedef struct abb_nodo {
    struct abb_nodo *izq;
    struct abb_nodo *der;
    char *clave;
    void *dato;
} abb_nodo_t;

struct abb {
    abb_nodo_t *raiz;
    abb_comparar_clave_t cmp;
    abb_destruir_dato_t destruir_dato;
    size_t cant;
    size_t contador_rangos; 
};

struct abb_iter {
    pila_t *pila;
};

abb_nodo_t *crear_nodo(const char *clave, void *dato){
    abb_nodo_t *nuevo_nodo = malloc(sizeof(abb_nodo_t));
    if(!nuevo_nodo){
        return NULL;
    }
    char *clave_aux = strdup(clave);
    nuevo_nodo->clave = clave_aux;
    nuevo_nodo->dato = dato;
    nuevo_nodo->izq = NULL;
    nuevo_nodo->der = NULL;
    return nuevo_nodo;
}
abb_t *abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){
    abb_t *arbol = malloc(sizeof(abb_t));
    if(!arbol){
        return NULL;
    }
    arbol->contador_rangos = 0;
    arbol->cant = 0;
    arbol->raiz = NULL;
    arbol->cmp = cmp;
    arbol->destruir_dato = destruir_dato;
    return arbol;
}

abb_nodo_t *busqueda_nodo_insercion(const abb_t *arbol, abb_nodo_t *actual, const char* clave, void *dato, bool *es_hijo_izq){
    //Busca nodo anterior (padre) de otro nodo, haya sido ingresado todavía o no.
    if(!arbol){
        return NULL;
    }
    // 0 claves iguales, > 1 actual es mayor a la clave, < 1 actual menor a la clave
    int comparativo = arbol->cmp(actual->clave, clave); 
    if(comparativo == 0){
        return NULL;
    }
    if(comparativo < 0){
        if(!actual->der||arbol->cmp(clave, actual->der->clave) == 0){ 
            *es_hijo_izq = false;
            return actual;
        }
        return busqueda_nodo_insercion(arbol, actual->der, clave, dato, es_hijo_izq);
    }
    if(comparativo > 0){
        if(!actual->izq||arbol->cmp(clave, actual->izq->clave) == 0){
            *es_hijo_izq = true;
            return actual;
        }
        return busqueda_nodo_insercion(arbol, actual->izq, clave, dato, es_hijo_izq);
    }
    return NULL;
}

abb_nodo_t *buscar_nodo(const abb_t *arbol, abb_nodo_t *actual, const char* clave){
     if(!actual || arbol->cant == 0){
        return NULL;
    }
    if(arbol->cmp(actual->clave, clave) == 0){
        return actual;
    }
    if(arbol->cmp(actual->clave, clave) < 0){
        return buscar_nodo(arbol, actual->der, clave);
    }
    if(arbol->cmp(actual->clave, clave) > 0){
        return buscar_nodo(arbol, actual->izq, clave);
    }
    return NULL;
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato){
    if(abb_pertenece(arbol, clave)){
        abb_nodo_t *nodo = buscar_nodo(arbol, arbol->raiz, clave);
        void *dato_viejo = nodo->dato;
        if(arbol->destruir_dato){
            arbol->destruir_dato(dato_viejo);
        }
        nodo->dato = dato;
        return true;
    }
    if(!arbol->raiz){
        abb_nodo_t *nodo_nuevo = crear_nodo(clave, dato);
        if(!nodo_nuevo){
            return false;
        }
        arbol->raiz = nodo_nuevo;
        arbol->cant++;
        return true;
    }
    bool es_hijo_izq = false;
    abb_nodo_t *padre = busqueda_nodo_insercion(arbol, arbol->raiz, clave, dato, &es_hijo_izq);
    if(!padre){
        return false;
    }
    abb_nodo_t *nodo_nuevo = crear_nodo(clave, dato);
    if(!nodo_nuevo){
        return false;
    }
    if(!es_hijo_izq){
        padre->der = nodo_nuevo;
    } else {
        padre->izq = nodo_nuevo;
    }
    arbol->cant++;
    return true;
}

abb_nodo_t* busqueda_de_reemplazo(abb_nodo_t* nodo){
    while(nodo->izq){
        nodo = nodo->izq;
    }
    return nodo;
}

void *abb_borrar(abb_t *arbol, const char *clave){
    if(!arbol||!abb_pertenece(arbol, clave)){
        return NULL;
    }
    if(arbol->cant == 1) {
        void* aux = arbol->raiz->dato;
        free(arbol->raiz->clave);
        free(arbol->raiz);
        arbol->raiz = NULL;
        arbol->cant--;
        return aux;
    }
    abb_nodo_t *nodo = buscar_nodo(arbol, arbol->raiz, clave); //obtengo al que será eliminado
    if(!nodo) return NULL;
    bool es_hijo_izq;
    abb_nodo_t *padre = busqueda_nodo_insercion(arbol, arbol->raiz, clave, nodo->dato, &es_hijo_izq); //obtengo al padre del que quiero borrar
    if(!nodo->izq && !nodo->der){
        /* Caso sin hijos */
        if(!padre){
            arbol->raiz = NULL;
        }
        else if(es_hijo_izq){  
            padre->izq = NULL;
        }
        else{ 
            padre->der = NULL;
        }
        free(nodo->clave);
        void* aux = nodo->dato;
        free(nodo);
        arbol->cant--;
        return aux; 
    }
    if((!nodo->izq && nodo->der) || (nodo->izq && !nodo->der)){
        /* Caso un hijo */
        if(nodo->izq){
            if(!padre){
                arbol->raiz = nodo->izq;
            }
            else if(!es_hijo_izq){ 
                padre->der = nodo->izq;
            }
            else{ 
                padre->izq = nodo->izq;
            }
        }
        else if(nodo->der){
            if(!padre){
                arbol->raiz = nodo->der;
            }
            else if(!es_hijo_izq){  
                padre->der = nodo->der;
            }
            else if(es_hijo_izq){ 
                padre->izq = nodo->der;
            }
        }
        free(nodo->clave);
        void* aux = nodo->dato;
        free(nodo);
        arbol->cant--;
        return aux; 
    }
    if(nodo->der && nodo->izq){
        abb_nodo_t* reemplazante = busqueda_de_reemplazo(nodo->der);
        char* clave_reemplazante = strdup(reemplazante->clave);
        void* dato_reemplazante = reemplazante->dato;
        abb_borrar(arbol, reemplazante->clave);
        char* clave_a_liberar = nodo->clave;
        void* dato_a_devolver = nodo->dato;
        nodo->clave = clave_reemplazante;
        nodo->dato = dato_reemplazante;
        free(clave_a_liberar);
        return dato_a_devolver;
    }
    return NULL;
}


void *abb_obtener(const abb_t *arbol, const char *clave){
    abb_nodo_t *nodo = buscar_nodo(arbol, arbol->raiz, clave);
    if(!nodo){
        return NULL;
    }
    if(arbol->cmp(arbol->raiz->clave, clave) == 0){
        return arbol->raiz->dato;
    }
    return nodo->dato;
}

bool abb_pertenece(const abb_t *arbol, const char *clave){
    abb_nodo_t *nodo = buscar_nodo(arbol, arbol->raiz, clave);
    if(!nodo){
        return false;
    }
    return true;
}

size_t abb_cantidad(abb_t *arbol){
    return arbol->cant;
}

void destruccion_total(abb_t *arbol, abb_nodo_t *nodo){
    if(!nodo){
        return;
    }
    destruccion_total(arbol, nodo->izq);
    destruccion_total(arbol, nodo->der);
    free(nodo->clave);
    if(arbol->destruir_dato != NULL){
        arbol->destruir_dato(nodo->dato);
    }
    free(nodo);
}

void abb_destruir(abb_t *arbol){
    destruccion_total(arbol, arbol->raiz);
    free(arbol);
}

bool iterar(abb_nodo_t *actual, bool visitar(const char *, void *, void *), void *extra, bool *continuar){
    if(!actual||(*continuar == false)) return false;
    iterar(actual->izq, visitar, extra, continuar);
    if(*continuar == true){
        if(!visitar(actual->clave, actual->dato, extra)){
            *continuar = false;
            return false;
        }
    }
    return iterar(actual->der, visitar, extra, continuar);
}

size_t abb_cantidad_informe(abb_t *arbol){
    return arbol->contador_rangos;
    }
void abb_in_order(abb_t *arbol,bool visitar(const char *, void *, void *),void *extra){
    abb_nodo_t *actual = arbol->raiz;
    bool continuar = true;
    iterar(actual, visitar, extra, &continuar);
}

void _arbol_iterar_rango(abb_t *arbol, abb_nodo_t *actual, void visitar(void *clave, void *dato, abb_t *arbol), char *inicio, char *fin){

    if(!actual){
        return;
    }
    bool iterar_izquierda = false;
    if ((strcmp(actual->clave, inicio) >= 0) || (inicio[0] == '\0')){
        iterar_izquierda = true;
    }
    bool iterar_derecha = false;
    if ((strcmp(actual->clave, fin) <= 0) || (fin[0]== '\0')){//'\0'
        iterar_derecha = true;
    } 
    if (iterar_izquierda){
        _arbol_iterar_rango(arbol, actual->izq, visitar, inicio, fin);
    }
    if (iterar_izquierda && iterar_derecha){
        arbol->contador_rangos++;
        if(visitar){
            visitar(actual->clave, actual->dato, arbol);
        } 
    }
    if (iterar_derecha){
        _arbol_iterar_rango(arbol, actual->der, visitar, inicio, fin);
    }
}




void arbol_iterar_rango(abb_t *arbol, void visitar(void *clave, void *dato, abb_t *arbol), char *inicio, char *fin){
    if(!arbol){
        return;
    }
    arbol->contador_rangos = 0;
    _arbol_iterar_rango(arbol, arbol->raiz, visitar, inicio, fin);
}



 abb_iter_t *abb_iter_in_crear(const abb_t *arbol){
    abb_iter_t *iter = malloc(sizeof(abb_iter_t));
    if(!iter){
        return NULL;
    }
    iter->pila = pila_crear();
    if(!iter->pila){
        free(iter);
        return NULL;
    }
    abb_nodo_t* actual = arbol->raiz;
    while(actual){
        pila_apilar(iter->pila, actual);
        actual = actual->izq;
    }
    return iter;
 }

 bool abb_iter_in_avanzar(abb_iter_t *iter){
    if(abb_iter_in_al_final(iter)) return false;
    abb_nodo_t* actual = pila_ver_tope(iter->pila);
    if(!actual){
        return false;
    }
    actual = actual->der;
    pila_desapilar(iter->pila);
    while (actual){
        pila_apilar(iter->pila, actual);
        actual = actual->izq;
    }

    return true;
 }

 const char *abb_iter_in_ver_actual(const abb_iter_t *iter){
    if(abb_iter_in_al_final(iter)) return NULL;
    abb_nodo_t* tope = pila_ver_tope(iter->pila);
    return tope->clave;
 }

 bool abb_iter_in_al_final(const abb_iter_t *iter){
    return pila_esta_vacia(iter->pila);
 }

 void abb_iter_in_destruir(abb_iter_t *iter){
    pila_destruir(iter->pila);
    free(iter);
}

#endif

