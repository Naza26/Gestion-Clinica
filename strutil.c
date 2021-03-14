#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "strutil.h"

char* substr(const char* str, size_t n){

    char* nueva_cadena = calloc(n + 1, sizeof(char));
    for(size_t i = 0; i<n && str[i] != '\0'; i++){
        nueva_cadena[i] = str[i];
    }

    return nueva_cadena;
}
char** split(const char* str, char separador){

    size_t cantidad_separadores = 0;
    for(size_t t = 0; str[t] != '\0'; t++){
        if(str[t] == separador){
            cantidad_separadores++;
        }
    }
    char **arreglo = calloc(cantidad_separadores+2, sizeof(char*));
    if(cantidad_separadores==0){
        char* cadena = strdup(str);
        arreglo[0] = cadena;
        arreglo[1] = NULL;
        return arreglo;
    }
    size_t inicio = 0;
    size_t posicion_en_arreglo = 0;
    size_t longitud = strlen(str);
    size_t longitud_subgrupo = 0;
    for(size_t i = 0; i<=longitud+1; i++){
        longitud_subgrupo +=1;
        if(str[i] == separador || str[i] == '\0'){
            char* subcadena = strndup(str + inicio, i-inicio);
            arreglo[posicion_en_arreglo] = subcadena;
            if(str[i] == '\0'){
                break;
            }
            inicio += longitud_subgrupo;
            posicion_en_arreglo++;
            longitud_subgrupo = 0;
            if((i==longitud && str[i] == separador)){
              //varios casos borde  
            break;
            }           
        }
    }
    return arreglo;
}
char* join(char** strv, char sep){
    if(!strv) return "";
    size_t elementos_arreglo = 0;
    size_t longitud_total = 0;
    for(size_t a = 0; strv[a] != NULL; a++){
        elementos_arreglo += 1;
        longitud_total += strlen(strv[a]);
    }
    if(elementos_arreglo == 0){
        char* resultado = calloc(1, sizeof(char));
        return resultado;
    }
    if(sep == '\0'){
      elementos_arreglo = 1; //caso borde.
    }
    char* resultado = calloc(elementos_arreglo + longitud_total, sizeof(char));     
    size_t posicion_en_cadena = strlen(strv[0]);
    char* puntero = stpcpy(resultado, strv[0]);
    puntero += 1;
    for(size_t m = 1; strv[m] != NULL; m++){
        size_t len = strlen(strv[m]);
        if(len == 0){
            resultado[posicion_en_cadena] = sep;
            posicion_en_cadena += len+1;
            puntero += 1;
        }else{
            if(sep == '\0'){
              puntero = stpcpy(puntero-1, strv[m]);
              puntero+=1;
            }else{
              puntero = stpcpy(puntero, strv[m]);
              resultado[posicion_en_cadena]= sep;
              posicion_en_cadena += 1;
              puntero+=1;
            }
            posicion_en_cadena += len;
        } 
}   
    return resultado;
}

void free_strv(char *arreglo[]){
    for(size_t i = 0; arreglo[i] != NULL; i++){
        free(arreglo[i]);
    }
    free(arreglo);
}



