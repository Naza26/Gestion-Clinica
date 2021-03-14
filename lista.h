#ifndef LISTA_H
#define LISTA_H
#include <stdlib.h>
#include <stdbool.h>

/*Primitivas de la lista*/

typedef struct lista lista_t;

/* Crea una lista.
Post: Devuelve una nueva lista vacia. */
lista_t *lista_crear(void);

/* Devuelve true si la lista esta vacia, false en caso contrario
Pre: La lista fue creada. */
bool lista_esta_vacia(const lista_t *lista);

/* Agrega un nuevo elemento al comienzo de la lista. Devuelve false en caso de error.
Pre: La lista fue creada.
Post: Se agrego un nuevo elemento al comienzo de la lista con su respectivo valor. */
bool lista_insertar_primero(lista_t *lista, void *dato);

/* Agrega un nuevo elemento al final de la lista. Devuelve false en caso de error.
Pre: La lista fue creada.
Post: Se agrego un nuevo elemento al final de la lista con su respectivo valor. */
bool lista_insertar_ultimo(lista_t *lista, void *dato);

/* Elimina el primer elemento de la lista. Devuelve false en caso de error.
Pre: La lista fue creada.
Post: Se elimino el primer elemento de la lista. */
void *lista_borrar_primero(lista_t *lista);

/* Devuelve el primer elemento de la lista o NULL si la lista esta vacia. */
void *lista_ver_primero(const lista_t *lista);

/* Devuelve el ultimo elemento de la lista o NULL si la lista esta vacia. */
void *lista_ver_ultimo(const lista_t* lista);

/* Devuelve el largo de la lista o NULL si la lista esta vacia */
size_t lista_largo(const lista_t *lista);

/* Destruye la lista. Si se recibe la funcion destruir_dato por parametro,
para cada uno de los elementos de la lista, llama a destruir_dato.
Pre: La lista fue creada. Destruir_dato es una funcion capaz de destruir los datos de la lista, o NULL
en caso de que no se la utilice.
Post: Se eleminaron todos los elementos de la lista. */
void lista_destruir(lista_t *lista, void (*destruir_dato)(void *));

/*Primitiva del iterador interno*/

/*Itera la lista y su condicion de corte se basa en lo que devuleva visitar*/
void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra);

/*Primitivas del iterador externo*/

typedef struct lista_iter lista_iter_t;

/*Crea un iterador de lista
Post: Devuelve un iterador vacio*/
lista_iter_t *lista_iter_crear(lista_t *lista);

/*Avanza una posicion el iterador
Pre: El iterador ya fue creado
Post: Avanza el iterador y devuelve true si lo pudo avanzar correctamente, false, si no pudo o la lista estabava vacia*/
bool lista_iter_avanzar(lista_iter_t *iter);

/*Devuelve el actual del iterador o nulo si el iterador no existe*/
void *lista_iter_ver_actual(const lista_iter_t *iter);

/*Devuelve true si el iterador esta al final, false en caso contrario*/
bool lista_iter_al_final(const lista_iter_t *iter);

/*Destruye el iterador
Pre: El iterador fue creado
Post: El iterador fue destruido*/
void lista_iter_destruir(lista_iter_t *iter);

/*Inserta el elemento en el actual. Devuelve true si lo pude insertar correctamente, false en caso contrario*/
bool lista_iter_insertar(lista_iter_t *iter, void *dato);

/*Elimina el actual y devuelve el dato eliminado en tanto y en cuanto, la lista no este vacia, de ser ese el caso devuelvo NULL*/
void *lista_iter_borrar(lista_iter_t *iter);

/*Mis pruebas*/
void pruebas_lista_estudiante(void);

#endif  // LISTA_H