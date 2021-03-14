#define _POSIX_C_SOURCE 200809L

#include "zyxcba.h"

//#include "funciones_tp2.h"
//ult version
void procesar_comando(const char* comando, char** parametros, clinica_t *clinica, size_t contador_params) {
	if(strcmp(comando, COMANDO_PEDIR_TURNO) != 0 && strcmp(comando, COMANDO_ATENDER) != 0 && strcmp(comando, COMANDO_INFORME)!= 0){
		printf(ENOENT_CMD, comando);
		return;
	}
	if (strcmp(comando, COMANDO_PEDIR_TURNO) == 0) {
		if(contador_params != 3){
			fprintf(stdout, ENOENT_PARAMS, comando);
			return;
		}
		bool hay_errores = capturar_errores(clinica, comando, parametros, contador_params);
		if (hay_errores) return;
		if(strcmp(parametros[2], "URGENTE") == 0){
			asignar_turnos_urgentes(clinica, parametros[0], parametros[1]);
		} else {
			asignar_turnos_regulares(clinica, parametros[0], parametros[1]);
		}
	} else if (strcmp(comando, COMANDO_ATENDER) == 0) {
		if(contador_params != 1){
			fprintf(stdout, ENOENT_PARAMS, comando);
			return;
		}
		atender_paciente(clinica, parametros[0]);
	} else if (strcmp(comando, COMANDO_INFORME) == 0) {
		if(contador_params != 2){
			fprintf(stdout, ENOENT_PARAMS, comando);
			return;
		}
		obtener_informe_doctores(clinica, parametros[0], parametros[1]);
	} else {
		fprintf(stdout, ENOENT_CMD, comando);
	}
}

void eliminar_fin_linea(char* linea) {
	size_t len = strlen(linea);
	if (linea[len - 1] == '\n') {
		linea[len - 1] = '\0';
	}
}

void procesar_entrada(clinica_t *clinica) {
	char* linea = NULL;
	size_t c = 0;
	while (getline(&linea, &c, stdin) > 0) {
		eliminar_fin_linea(linea);
		char** campos = split(linea, ':');
		if (campos[1] == NULL) {
			printf(ENOENT_FORMATO, linea);
			free_strv(campos);
			continue;	
		}
		char** parametros = split(campos[1], ',');
		size_t cantidad_parametros = 0;
		for (size_t i = 0; parametros[i] != NULL; i++){
			cantidad_parametros++;
		}
		procesar_comando(campos[0], parametros, clinica, cantidad_parametros);
		free_strv(parametros);
		free_strv(campos);
	}
	free(linea);
}

int main(int argc, char** argv) {
	if(argc != 3){
		printf(ENOENT_CANT_PARAMS); 
	}
	clinica_t *clinica = clinica_crear(argv);
	procesar_entrada(clinica);
	clinica_destruir(clinica);
	return 0;

}


