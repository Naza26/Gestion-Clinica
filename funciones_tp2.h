#ifndef FUNCIONES_TP2
#define FUNCIONES_TP2


#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include "hash.h"
#include "cola.h"
#include "heap.h"
#include "abb.h"
#include "mensajes.h"
#include "csv.h"

typedef struct clinica clinica_t;

typedef struct doctor doctor_t;

typedef struct paciente paciente_t;

clinica_t *clinica_crear(char** argv);

/*Esta funcion ordena los pacientes de mayor a menor antiguedad con respecto al ingreso a la clinica*/

int comparar_antiguedad(const void *p1, const void *p2);

/*Esta funcion encola los pacientes con una mayor prioridad por sobre los pacientes regulares*/
void asignar_turnos_urgentes(clinica_t *clinica, char *nombre_paciente, char *especialidad);

/*Esta funcion encola a los pacientes en base al orden de llegada sin nigun tipo de prioridad*/
void asignar_turnos_regulares(clinica_t *clinica, void *nombre_paciente, void *especialiadad);

/*Esta funcion contabiliza la cantidad de pacientes atendidos por doctor*/
hash_t *contabilizar_p_atendidos(hash_t *cantidad_p_atendidos, char*doctor);

/*Si hay un paciente para ser atendido en base a la especialidad necesitada, se lo desencola*/
void atender_paciente(clinica_t *clinica, char* nombre_doctor);

/*Guardamos a los doctores en un abb*/
void procesar_lista_doctores(clinica_t *clinica, lista_t *lista_de_doctores);

/*Guardamos los pacientes en su respectivo hash*/
void procesar_lista_pacientes(clinica_t *clinica, lista_t *lista_de_pacientes);

/*Asociamos cada doctor a su determinada especialidad*/
void cargar_especialidades(clinica_t *clinica);

/*Obtenemos el informe de doctores en el rango pedido o de principio a fin si no se especifica un rango*/
void obtener_informe_doctores(clinica_t *clinica, char*inicio, char*fin);

/*Procesamos el csv*/
void procesar_csv();

/*Destruimso la clinica y todas sus estructuras auxiliares*/
void clinica_destruir(clinica_t *clinica);
/*Captura errores*/
bool capturar_errores(clinica_t *clinica, const char*comando, char**parametros, size_t contador);

void destruir_paciente(void *paciente);

void destruir_doctor(void *doctor);


#endif




