#ifndef ABB_H
#define ABB_H
#include <stdbool.h>
#include <stddef.h>
#include "pila.h"
#include "hash.h"

/* Tipos de función para comparar claves y destruir datos. */
typedef int (*abb_comparar_clave_t)(const char *, const char *);
typedef void (*abb_destruir_dato_t)(void *);

typedef struct abb abb_t;

struct abb_iter;
typedef struct abb_iter abb_iter_t;

/* Primitivas del mapa. */

/* Crea el abb */
abb_t *abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato);

/* Guarda un elemento en el arbol, si la clave ya se encuentra en la
 * estructura, la reemplaza. De no poder guardarlo devuelve false.
 * Pre: La estructura abb fue inicializada
 * Post: Se almacenó el par (clave, dato)
 */
bool abb_guardar(abb_t *arbol, const char *clave, void *dato);

/* Borra un elemento del arbol y devuelve el dato asociado.  Devuelve
 * NULL si el dato no estaba.
 * Pre: La estructura arbol fue inicializada
 * Post: El elemento fue borrado de la estructura y se lo devolvió,
 * en el caso de que estuviera guardado.
 */
void *abb_borrar(abb_t *arbol, const char *clave);

/* Obtiene el valor de un elemento del arbol, si la clave no se encuentra
 * devuelve NULL.
 * Pre: La estructura arbol fue inicializada
 */
void *abb_obtener(const abb_t *arbol, const char *clave);

/* Determina si clave pertenece o no al arbol.
 * Pre: La estructura hash fue inicializada
 */
bool abb_pertenece(const abb_t *arbol, const char *clave);

/* Devuelve la cantidad de elementos del arbol.
 * Pre: La estructura arbol fue inicializada
 */
size_t abb_cantidad(abb_t *arbol);

/* Destruye la estructura liberando la memoria pedida y llamando a la función
 * destruir para cada par (clave, dato).
 * Pre: La estructura arbol fue inicializada
 * Post: La estructura arbol fue destruida
 */ 
void abb_destruir(abb_t *arbol);

/* Implementa el iterador interno.
   "visitar" es una función de callback que recibe la clave, el valor y un
   puntero extra, y devuelve true si se debe seguir iterando, false en caso
   contrario).
 */
void abb_in_order(abb_t *arbol,bool visitar(const char *, void *, void *),void *extra);

void arbol_iterar_rango(abb_t *arbol, void visitar(void *clave, void *dato, abb_t *arbol), char *inicio, char *fin);

/* Primitivas del iterador externo. */
abb_iter_t *abb_iter_in_crear(const abb_t *arbol);

// Avanza iterador
bool abb_iter_in_avanzar(abb_iter_t *iter);

// Devuelve clave actual, esa clave no se puede modificar ni liberar.
const char *abb_iter_in_ver_actual(const abb_iter_t *iter);

size_t abb_cantidad_informe(abb_t *arbol);
// Destruye iterador
bool abb_iter_in_al_final(const abb_iter_t *iter);

/* Iterador interno del arbol */
void abb_iter_in_destruir(abb_iter_t *iter);

#endif  // ABB_H
