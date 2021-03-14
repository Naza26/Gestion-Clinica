
#ifndef ZYXCBA
#define ZYXCBA


#include <stdio.h>
#include <string.h>
#include "strutil.h"
#include "mensajes.h"
#include "funciones_tp2.h"

#define COMANDO_PEDIR_TURNO "PEDIR_TURNO"
#define COMANDO_ATENDER "ATENDER_SIGUIENTE"
#define COMANDO_INFORME "INFORME"

void procesar_comando(const char* comando, char** parametros, clinica_t* clinica, size_t contador_params);
void eliminar_fin_linea(char* linea);
void procesar_entrada(clinica_t* clinica);

#endif
