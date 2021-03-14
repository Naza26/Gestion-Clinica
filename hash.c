#define _POSIX_C_SOURCE 200809L
#include "hash.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
enum estado
{
    VACIO,
    OCUPADO,
    BORRADO
};

typedef struct campo
{
    char *clave;
    void *valor;
    enum estado estado;
} campo_t;

struct hash
{
    size_t capacidad;
    size_t cantidad_claves;
    hash_destruir_dato_t funcion_destruccion;
    campo_t *tabla;
};

struct hash_iter
{
    const hash_t *hash;
    campo_t campo_actual;
    size_t posicion;
};
    static unsigned long f_hash(const char *str)
    {
        unsigned long hash = 0;
        int c;

        while ((c = *str++))
            hash = c + (hash << 6) + (hash << 16) - hash;

        return hash;
    }

hash_t *hash_crear(hash_destruir_dato_t destruir_dato)
{
    hash_t *hash = malloc(sizeof(hash_t));
    if (!hash)
        return NULL;
    hash->cantidad_claves = 0;
    hash->capacidad = 10;
    campo_t *tabla = malloc(sizeof(campo_t) * hash->capacidad);
    if (!tabla)
    {
        free(hash);
        return NULL;
    }
    hash->tabla = tabla;
    for(size_t i = 0; i < hash->capacidad;i++){
		hash->tabla[i].estado = VACIO;
	}
    hash->funcion_destruccion = destruir_dato;
    return hash;
}

bool redimensionar_hash(hash_t *hash)
{
    size_t capacidad_anterior = hash->capacidad;
    hash->capacidad = hash->capacidad * 7;
    campo_t *tabla_anterior = hash->tabla;
    campo_t *nueva_tabla = malloc(sizeof(campo_t) * hash->capacidad);
    if (!nueva_tabla)
        return false;
    hash->tabla = nueva_tabla;
    hash->cantidad_claves = 0;
    for(size_t i = 0; i < hash->capacidad; i++){
		hash->tabla[i].estado = VACIO;
	}
    for (size_t i = 0; i < capacidad_anterior; i++)
    {
        if (tabla_anterior[i].estado == OCUPADO)
        {
            char *clave = tabla_anterior[i].clave;
            void *valor = tabla_anterior[i].valor;
            hash_guardar(hash, clave, valor);
            free(clave);
        }
    }
    free(tabla_anterior);
    return true;
}
bool hash_guardar(hash_t *hash, const char *clave, void *dato)
{
    //redimensionar cuando el factor de carga sea 0,7. factor de carga = cantidad de claves / tamanio tabla

    if ((((hash->cantidad_claves + 1) * 100 / (hash->capacidad)) >= 70))
    {
        bool red = redimensionar_hash(hash);
        if (!red)
            return false;
    }
    char *clave_aux = strdup(clave);
    size_t clave_hasheada = f_hash(clave) % hash->capacidad;
    for (size_t i = 0; i < hash->capacidad; i++)
    {
        if(clave_hasheada > hash->capacidad)
        {
            clave_hasheada = 0;
        }
        if (hash->tabla[clave_hasheada].estado == VACIO)
        {
            hash->tabla[clave_hasheada].clave = clave_aux;
            hash->tabla[clave_hasheada].valor = dato;
            hash->tabla[clave_hasheada].estado = OCUPADO;
            hash->cantidad_claves++;
            return true;
        }
        if (hash->tabla[clave_hasheada].estado == OCUPADO)
        {
            if (strcmp(hash->tabla[clave_hasheada].clave, clave) == 0)
            {
				if(hash->funcion_destruccion != NULL){
					void *dato_aux = hash->tabla[clave_hasheada].valor;
					hash->funcion_destruccion(dato_aux);
				}
				hash->tabla[clave_hasheada].valor = dato;
				free(clave_aux);
				return true;
            }
        }
        clave_hasheada++;
    }
    return false;
}

void *hash_borrar(hash_t *hash, const char *clave)
{   
    if(hash->cantidad_claves == 0 || !hash_pertenece(hash, clave)){
        return NULL;
    }
    size_t clave_hasheada = f_hash(clave) % hash->capacidad;
    for (size_t i = 0; i < hash->capacidad; i++)
    {
		
		if(clave_hasheada > hash->capacidad)
        {
            clave_hasheada = 0;
        }
        if (hash->tabla[clave_hasheada].estado == OCUPADO)
        {
			
            if (strcmp(hash->tabla[clave_hasheada].clave, clave) == 0)
            {
                hash->tabla[clave_hasheada].estado = BORRADO;
                hash->cantidad_claves--;
                free(hash->tabla[clave_hasheada].clave);
                return hash->tabla[clave_hasheada].valor;
            }
        }
        clave_hasheada++;
    }
    return NULL;
}

void *hash_obtener(const hash_t *hash, const char *clave)
{   
    if(hash->cantidad_claves == 0 || !hash_pertenece(hash, clave)){
        return NULL;
    }
    size_t clave_hasheada = f_hash(clave) % hash->capacidad;
    for (size_t i = 0; i < hash->capacidad; i++)
    {
        if(clave_hasheada > hash->capacidad){
            clave_hasheada = 0;
        }
        if (hash->tabla[clave_hasheada].estado == OCUPADO)
        {
            if(strcmp(clave, hash->tabla[clave_hasheada].clave) == 0)
            {
                return hash->tabla[clave_hasheada].valor;
            }
        }
        clave_hasheada++;
    }
    return NULL;
}

bool hash_pertenece(const hash_t *hash, const char *clave)
{
    if(hash->cantidad_claves == 0){
        return NULL;
    }
    size_t clave_hasheada = f_hash(clave) % hash->capacidad;
    for(size_t i = 0; i < hash->capacidad; i++)
    {
        if(clave_hasheada > hash->capacidad){
            clave_hasheada = 0;
        }
        if (hash->tabla[clave_hasheada].estado == OCUPADO)
        {
            if (strcmp(hash->tabla[clave_hasheada].clave, clave) == 0)
            {
                return true;
            }
        }
        else if (hash->tabla[clave_hasheada].estado == VACIO || hash->tabla[i].estado == BORRADO)
        {
            continue;
        }
        clave_hasheada++;
    }
    return false;
}

size_t hash_cantidad(const hash_t *hash)
{
    return hash->cantidad_claves;
}

void hash_destruir(hash_t *hash)
{
	if(hash->cantidad_claves != 0){
		for (size_t i = 0; i < hash->capacidad; i++){
            if(hash->tabla[i].estado == OCUPADO){
                void* dato = hash_obtener(hash, hash->tabla[i].clave);
                hash_borrar(hash, hash->tabla[i].clave);
                if(hash->funcion_destruccion != NULL){
                    hash->funcion_destruccion(dato);
                }
            }   
        }
	}

    free(hash->tabla);
    free(hash);
}

hash_iter_t *hash_iter_crear(const hash_t *hash)
{
    hash_iter_t *iter = malloc(sizeof(hash_iter_t));

    if(!iter){
        return NULL;
    }
    iter->hash = hash;
    iter->posicion = 0;
    for(size_t i = 0; i < iter->hash->capacidad; i++){
		if(iter->hash->tabla[i].estado == OCUPADO){
			iter->posicion = i;
			break;
		}
	}
    iter->campo_actual = iter->hash->tabla[iter->posicion];
    return iter;
}

bool hash_iter_avanzar(hash_iter_t *iter)
{   
	if(iter->hash->cantidad_claves == 0){
		return false;
	}
    iter->posicion++;
	while(!hash_iter_al_final(iter)){
		if(iter->hash->tabla[iter->posicion].estado == VACIO || iter->hash->tabla[iter->posicion].estado == BORRADO){
			iter->posicion++;
			continue;
		}
		return true;
	}
	return false;
}

const char *hash_iter_ver_actual(const hash_iter_t *iter)
{
    if(iter->hash->cantidad_claves == 0 || hash_iter_al_final(iter)){
        return NULL;
    }
    return iter->hash->tabla[iter->posicion].clave;
}

bool hash_iter_al_final(const hash_iter_t *iter)
{   
    if(iter->posicion >= iter->hash->capacidad || iter->hash->cantidad_claves == 0){
        return true;
    }
    return false;
}

void hash_iter_destruir(hash_iter_t *iter)
{
    free(iter);
}
