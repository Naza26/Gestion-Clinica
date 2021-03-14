#define _POSIX_C_SOURCE 200809L

#include "funciones_tp2.h"

struct clinica {
    abb_t *doctores;
    hash_t *pacientes_inscriptos;
    hash_t *pacientes_urgentes;
    hash_t *pacientes_regulares; 
    size_t contador_informe; //BORRARLO
};

struct doctor {
    char *nombre;
    char *especialidad;
    size_t cantidad_atendidos;
};

struct paciente {
    char *nombre;
    char* antiguedad;
};

void *crear_doctores(char **parametros, void *dato){
    doctor_t *doctor = malloc(sizeof(doctor_t));

    doctor->nombre = strdup(parametros[0]);
    doctor->especialidad = strdup(parametros[1]);
    doctor->cantidad_atendidos = 0; 
    return doctor;
}

void *crear_pacientes(char **parametros, void *dato){
    paciente_t *paciente = malloc(sizeof(paciente_t));
    paciente->nombre = strdup(parametros[0]);
    paciente->antiguedad = strdup(parametros[1]);
    return paciente;
}

clinica_t *clinica_crear(char** argv){
    clinica_t *clinica = malloc(sizeof(clinica_t));
    clinica->contador_informe = 0;

    abb_t *doctores = abb_crear(strcmp, destruir_doctor);
    if(!doctores) return NULL;
    clinica->doctores = doctores;

    hash_t *pacientes_inscriptos = hash_crear(destruir_paciente);
    if(!pacientes_inscriptos) return NULL;
    clinica->pacientes_inscriptos = pacientes_inscriptos;

    hash_t *espera_urgentes = hash_crear(NULL);
    if(!espera_urgentes) return NULL;
    clinica->pacientes_urgentes = espera_urgentes;

    hash_t *espera_regulares = hash_crear(NULL);
    if(!espera_regulares) return NULL;
    clinica->pacientes_regulares = espera_regulares;

    lista_t *lista_doctores = csv_crear_estructura(argv[1], crear_doctores, NULL);

    lista_t *lista_pacientes = csv_crear_estructura(argv[2], crear_pacientes, NULL);

    procesar_lista_doctores(clinica, lista_doctores);

    procesar_lista_pacientes(clinica, lista_pacientes);

    return clinica;
}


char* obtener_antiguedad(paciente_t* paciente){
    return paciente->antiguedad;
    }
int comparar_antiguedad(const void *paciente1, const void *paciente2){
    char* antiguedad_1 = obtener_antiguedad((paciente_t*)paciente1);
    char* antiguedad_2 = obtener_antiguedad((paciente_t*)paciente2);
    if(strcmp(antiguedad_1, antiguedad_2)  < 0){
        return 1;
    }
    if(strcmp(antiguedad_2, antiguedad_1)  < 0){    
        return -1;
    }
    return 0;
}


void asignar_turnos_urgentes(clinica_t *clinica, char *nombre_paciente, char *especialidad){
    paciente_t* paciente_urgente = hash_obtener(clinica->pacientes_inscriptos, nombre_paciente);
    cola_encolar(hash_obtener(clinica->pacientes_urgentes, especialidad), paciente_urgente);
    fprintf(stdout, PACIENTE_ENCOLADO, nombre_paciente);
    size_t cantidad_total = cola_cantidad(hash_obtener(clinica->pacientes_urgentes, especialidad)) + heap_cantidad(hash_obtener(clinica->pacientes_regulares, especialidad));
    fprintf(stdout, CANT_PACIENTES_ENCOLADOS, cantidad_total, especialidad);
}

void asignar_turnos_regulares(clinica_t *clinica, void *nombre_paciente, void *especialiadad){
    paciente_t *paciente_regular = hash_obtener(clinica->pacientes_inscriptos, nombre_paciente);
    heap_encolar(hash_obtener(clinica->pacientes_regulares, especialiadad), paciente_regular);
    fprintf(stdout, PACIENTE_ENCOLADO, (char*)nombre_paciente);
    size_t cantidad_total = cola_cantidad(hash_obtener(clinica->pacientes_urgentes, especialiadad)) + heap_cantidad(hash_obtener(clinica->pacientes_regulares, especialiadad));
    fprintf(stdout, CANT_PACIENTES_ENCOLADOS, cantidad_total, (char*)especialiadad);
}

void atender_paciente(clinica_t *clinica, char* nombre_doctor){
    doctor_t *doctor_obtenido = abb_obtener(clinica->doctores, nombre_doctor); //log(d)
    if(!doctor_obtenido){
        fprintf(stdout, ENOENT_DOCTOR, nombre_doctor);
        return;
    }
    size_t cantidad_encolados = 0;
    if(cola_cantidad(hash_obtener(clinica->pacientes_urgentes, doctor_obtenido->especialidad)) == 0){
        // atiende regular
        heap_t *pacientes_especialidad = hash_obtener(clinica->pacientes_regulares, doctor_obtenido->especialidad);
        cantidad_encolados += heap_cantidad(pacientes_especialidad);
        if(cantidad_encolados == 0){
            fprintf(stdout, SIN_PACIENTES);
            return;
        }
        paciente_t *datos_pacientes = heap_desencolar(pacientes_especialidad);
        fprintf(stdout, PACIENTE_ATENDIDO, datos_pacientes->nombre);
        
    } else {
        // atiende urgente
        cola_t *pacientes_especialidad = hash_obtener(clinica->pacientes_urgentes, doctor_obtenido->especialidad);
        cantidad_encolados += cola_cantidad(pacientes_especialidad);
        cantidad_encolados += heap_cantidad(hash_obtener(clinica->pacientes_regulares, doctor_obtenido->especialidad));
        paciente_t *paciente_atendido = cola_desencolar(pacientes_especialidad); //O(log d + log e)
        fprintf(stdout, PACIENTE_ATENDIDO, (char*)paciente_atendido->nombre);

    }
    doctor_obtenido->cantidad_atendidos++;
    cantidad_encolados--;
    fprintf(stdout, "%zu paciente(s) en espera para %s\n", cantidad_encolados, doctor_obtenido->especialidad);
}



void procesar_lista_doctores(clinica_t *clinica, lista_t *lista_de_doctores){
    lista_iter_t *iterador_lista_doctores = lista_iter_crear(lista_de_doctores);

    if(!lista_de_doctores || !iterador_lista_doctores){
        return;
    }

    while(!lista_iter_al_final(iterador_lista_doctores)){
        doctor_t *linea_doctor = lista_iter_ver_actual(iterador_lista_doctores);
        bool ok = abb_guardar(clinica->doctores, linea_doctor->nombre, linea_doctor);
        if(!ok) return;
        if(!hash_pertenece(clinica->pacientes_urgentes, linea_doctor->especialidad)){
            hash_guardar(clinica->pacientes_urgentes, linea_doctor->especialidad, cola_crear());
            hash_guardar(clinica->pacientes_regulares, linea_doctor->especialidad, heap_crear(comparar_antiguedad));
        }
        lista_iter_avanzar(iterador_lista_doctores);
    }

    lista_iter_destruir(iterador_lista_doctores);
    lista_destruir(lista_de_doctores, NULL);
}

void procesar_lista_pacientes(clinica_t *clinica, lista_t *lista_de_pacientes){
    lista_iter_t *iterador_lista_pacientes = lista_iter_crear(lista_de_pacientes);
    if(!lista_de_pacientes || !iterador_lista_pacientes){
        return;
    }

    while(!lista_iter_al_final(iterador_lista_pacientes)){
        paciente_t *linea_paciente = lista_iter_ver_actual(iterador_lista_pacientes);
        hash_guardar(clinica->pacientes_inscriptos, linea_paciente->nombre, linea_paciente);
        lista_iter_avanzar(iterador_lista_pacientes);
    }

    lista_iter_destruir(iterador_lista_pacientes);
    lista_destruir(lista_de_pacientes, NULL);
}

void incrementar_contador(clinica_t* clinica){
    clinica->contador_informe++;
    }

size_t obtener_cantidad(doctor_t* doctor){
    return doctor->cantidad_atendidos;
   }

char* obtener_especialidad(doctor_t* doctor){
    return doctor->especialidad;
   }
    
void funcion_visitar(void *clave, void *estructura_doctor, abb_t *arbol_doctores){
    fprintf(stdout, INFORME_DOCTOR, abb_cantidad_informe(arbol_doctores), (char*)clave, obtener_especialidad(estructura_doctor), obtener_cantidad(estructura_doctor));
}
void obtener_informe_doctores(clinica_t *clinica, char*inicio, char*fin){
    arbol_iterar_rango(clinica->doctores, NULL, inicio, fin);
    size_t cantidad = abb_cantidad_informe(clinica->doctores);
    fprintf(stdout, DOCTORES_SISTEMA, cantidad);
    arbol_iterar_rango(clinica->doctores, funcion_visitar, inicio, fin);

}

bool capturar_errores(clinica_t *clinica, const char*comando, char**parametros, size_t contador_params){
    bool hay_errores = false;
    if (!hash_pertenece(clinica->pacientes_inscriptos, parametros[0])){
        hay_errores = true;
        fprintf(stdout, ENOENT_PACIENTE, parametros[0]);
        if(contador_params == 1){
            return true;
        }
    }
    if (!hash_pertenece(clinica->pacientes_urgentes, (char*)parametros[1])){
        hay_errores = true;
        fprintf(stdout, ENOENT_ESPECIALIDAD, parametros[1]);
        if(contador_params == 2){
            return true;
        }
    }
    if (strcmp(parametros[2], "URGENTE") != 0 && strcmp(parametros[2], "REGULAR") != 0){
        hay_errores = true;
        fprintf(stdout, ENOENT_URGENCIA, parametros[2]);
    }
    if (hay_errores){
        return true;
    }
    return false;
}
char* obtener_nombre_paciente(paciente_t* paciente_dato){
    return paciente_dato->nombre;
}

char* obtener_antiguedad_paciente(paciente_t* paciente_dato){
    return paciente_dato->antiguedad;
}
    
void destruir_paciente(void *paciente_dato){
	free(obtener_nombre_paciente(paciente_dato));
	free(obtener_antiguedad_paciente(paciente_dato));
	free(paciente_dato);
}

char *obtener_nombre_doctor(doctor_t *doctor_dato){
	return doctor_dato->nombre;
}

void destruir_doctor(void *doctor_dato){
	free(obtener_nombre_doctor(doctor_dato));
	free(obtener_especialidad(doctor_dato));
	free(doctor_dato);
}

void destruir_hash_regulares(hash_t *p_regulares){
	hash_iter_t *iter = hash_iter_crear(p_regulares);
	while(!hash_iter_al_final(iter)){
		heap_destruir(hash_obtener(p_regulares, hash_iter_ver_actual(iter)), NULL);
		hash_iter_avanzar(iter);
	}
	hash_iter_destruir(iter);
	hash_destruir(p_regulares);
}

void destruir_hash_urgentes(hash_t *p_urgentes){
	hash_iter_t *iter = hash_iter_crear(p_urgentes);
	while(!hash_iter_al_final(iter)){
		cola_destruir(hash_obtener(p_urgentes, hash_iter_ver_actual(iter)), NULL);
		hash_iter_avanzar(iter);
	}
	hash_iter_destruir(iter);
	hash_destruir(p_urgentes);
}


void clinica_destruir(clinica_t *clinica_estructura){
	destruir_hash_urgentes(clinica_estructura->pacientes_urgentes);
	destruir_hash_regulares(clinica_estructura->pacientes_regulares);
    abb_destruir(clinica_estructura->doctores);
    hash_destruir(clinica_estructura->pacientes_inscriptos);
    free(clinica_estructura);
}


