#include "lista.h"
#include <stdlib.h>

typedef struct nodo{
	void* dato;
	struct nodo* siguiente;
}nodo_t;

struct lista{
	size_t largo;
	nodo_t* primero;
	nodo_t* ultimo;
};

struct lista_iter{
	lista_t* lista;
	nodo_t* actual;
	nodo_t* anterior;
};

///

lista_t *lista_crear(void){

	lista_t* lista = malloc(sizeof(lista_t));
	if(!lista) return NULL;
	lista->largo = 0;
	lista->primero = NULL;
	lista->ultimo = NULL;

	return lista;
}

bool lista_esta_vacia(const lista_t *lista){
	return (lista->largo == 0);
}

bool lista_insertar_primero(lista_t *lista, void *dato){

	nodo_t* nodo = malloc(sizeof(nodo_t));
	if(!nodo) return false;
	nodo->dato = dato;
	nodo->siguiente = lista->primero; //el que antes estaba primero es el nuevo 'segundo'
	lista->primero = nodo; //el nuevo es el nuevo primero
	if(lista->primero->siguiente == NULL){
		lista->ultimo = nodo;
	}
	lista->largo+=1;
	return true;
}

bool lista_insertar_ultimo(lista_t *lista, void *dato){

	nodo_t* nodo = malloc(sizeof(nodo_t));
	if(!nodo) return false;
	nodo->dato = dato;
	nodo->siguiente = NULL; //es el ultimo
	if(lista->largo == 0){
		lista->primero = nodo;
		lista->ultimo = nodo;
	}else{
		lista->ultimo->siguiente = nodo;
		lista->ultimo = nodo;
	}
	lista->largo+=1;

	return true;
}

void *lista_borrar_primero(lista_t *lista){

	if(lista->largo == 0) return NULL;
	nodo_t* aux = lista->primero;
	void* aux_dato = lista->primero->dato; 
	lista->primero = lista->primero->siguiente;
	lista->largo-=1;
	free(aux);

	return aux_dato;
}

void *lista_ver_primero(const lista_t *lista){

	if(lista->largo == 0) return NULL;

	return lista->primero->dato;
}

void *lista_ver_ultimo(const lista_t* lista){

	if(lista->largo == 0) return NULL;

	return lista->ultimo->dato;
}

size_t lista_largo(const lista_t *lista){
	return lista->largo;
}

void lista_destruir(lista_t *lista, void (*destruir_dato)(void *)){
	while(lista->largo != 0){
		if(destruir_dato){
			void* dato = lista_borrar_primero(lista);
			destruir_dato(dato);
		}else{
			lista_borrar_primero(lista);
		}
	}
	free(lista);
}

void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra){
	if(visitar == NULL){
		return;
	}
	if(lista->largo == 0){
		return;
	}
	nodo_t* actual = lista->primero;
	bool respuesta = true;
	while(actual != NULL && respuesta==true){
		respuesta = visitar(actual->dato, extra);
		actual = actual->siguiente;

	}
}

lista_iter_t *lista_iter_crear(lista_t *lista){

	lista_iter_t* iterador = malloc(sizeof(lista_iter_t));
	if(!iterador) return NULL;
	iterador->anterior = NULL;
	iterador->actual = lista->primero;
	iterador->lista = lista;

	return iterador;
}

bool lista_iter_avanzar(lista_iter_t *iter){

	if(iter->actual == NULL){
		return false;
	}
	nodo_t* actual = iter->actual;
	iter->anterior = actual;
	iter->actual = actual->siguiente;

	return true;
}

void *lista_iter_ver_actual(const lista_iter_t *iter){
	if(iter->actual == NULL){
		return NULL;
	}
	return iter->actual->dato;
}

bool lista_iter_al_final(const lista_iter_t *iter){

	if(iter->actual== NULL){
		return true;
	}
	return false;
}

void lista_iter_destruir(lista_iter_t *iter){
	free(iter);
}

bool lista_iter_insertar(lista_iter_t *iter, void *dato){
	if(iter->actual == iter->lista->primero){
		//Si quiero insertar en la primera posición;
		nodo_t* auxx = iter->actual;
		bool ok = lista_insertar_primero(iter->lista, dato);
		if(!ok) return false;
		iter->actual = iter->lista->primero;
		iter->anterior = auxx;
	}else if(lista_iter_al_final(iter)){
		bool ok1 = lista_insertar_ultimo(iter->lista, dato);
		if(!ok1) return false;
		iter->actual = iter->lista->ultimo;
		iter->anterior->siguiente = iter->actual;
	}else{
		nodo_t* nodo = malloc(sizeof(nodo_t));
		if(!nodo) return false;
		nodo->dato = dato;
		nodo_t* aux2 = iter->actual;
		iter->anterior->siguiente = nodo;
		nodo->siguiente = aux2;
		iter->actual = nodo;
		iter->lista->largo++;

	}
	return true;
	}

void *lista_iter_borrar(lista_iter_t *iter){
	if(iter->actual == NULL){
		return NULL;
	}
	nodo_t* eliminado = iter->actual;
	void* eliminado_dato = iter->actual->dato;
	if(eliminado == iter->lista->primero){
		//Si quiero elminar el primer elemento
		iter->actual = iter->actual->siguiente;
		iter->lista->primero = eliminado->siguiente;
		//El anterior se mantiene igual
		iter->lista->largo--;
		free(eliminado);

	}
	else if(eliminado == iter->lista->ultimo){
		//Si quiero eliminar el último elemento
		iter->actual = iter->actual->siguiente;
		iter->anterior->siguiente = iter->actual;
		iter->lista->ultimo = iter->anterior;
		iter->lista->largo--;
		free(eliminado);
	}else{
		//En alguna otra posición entre la primera y la última
		iter->actual = iter->actual->siguiente;
		iter->anterior->siguiente = iter->actual; 
		iter->lista->largo--;
		free(eliminado);

	}
	return eliminado_dato;

}

