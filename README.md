# Gestion-Clinica

# Entradas

El programa recibe como argumentos el nombre de dos archivos en formato CSV:
  1. Archivo CSV con la lista de doctores. Tiene dos columnas: el nombre y la especialidad.
  ´´´
  nombre_doctor1,nombre_especialidad_A
  nombre_doctor2,nombre_especialidad_B
  nombre_doctor3,nombre_especialidad_A
  ...
  ´´´
  2. Archivo CSV con la lista de pacientes. Tiene también dos columnas: el nombre y el año de su inscripción.
  ´´´
  nombre_paciente1,año_inscripcion
  nombre_paciente2,año_inscripcion
  ´´´
  
# Pruebas

Para ejecutar las pruebas correr:
  $ ./pruebas.sh PATH-A-EJECUTABLE-TP2

# Interacción

Una instrucción se compone de un comando y sus parámetros. El formato siempre es: nombre del comando, carácter dos puntos ':' y parámetros del comando.

 ´´´
    CMD1:ARG_A
    CMD2:ARG_B,ARG_C
 ´´´
 
 Tanto el resultado como los mensajes de error se imprimen siempre por salida estándar.
 
 
 # Comandos
 
 1. Pedir Turno
 
  Se recibe un nombre de paciente y el nombre de una especialidad, y el sistema le añade a la lista de espera de la especialidad correspondiente.
  Formato:
  ´´´
      PEDIR_TURNO:NOMBRE_PACIENTE,NOMBRE_ESPECIALIDAD,URGENCIA
  ´´´
  Los valores válidos para URGENCIA son: URGENTE o REGULAR.
  
 Salida:
 ´´´
    Paciente NOMBRE_PACIENTE encolado
    N paciente(s) en espera para NOMBRE_ESPECIALIDAD
 ´´´
 2. Atender Siguiente Paciente
 
 Se recibe el nombre del doctor que quedó libre, y este atiende al siguiente paciente urgente (por orden de llegada). Si no hubiera ningún paciente urgente, atiende al siguiente paciente con mayor antigüedad como paciente en la clínica.
 
 Formato:
 ´´´
     ATENDER_SIGUIENTE:NOMBRE_DOCTOR
 ´´´
 Salida si se atendió a un paciente:
 ´´´
     Se atiende a NOMBRE_PACIENTE
    N paciente(s) en espera para NOMBRE_ESPECIALIDAD
 ´´´
 Salida si no había previamente pacientes en la lista de espera:
 
 ´´´
     No hay pacientes en espera
 ´´´
 
 # Informe Doctores
 
 El sistema imprime la lista de doctores en orden alfabético, junto con su especialidad y el número de pacientes que atendieron desde que arrancó el sistema. 
 Opcionalmente, se puede especificar el rango (alfabético) de doctores sobre los que se desean informes.
 
 Formato:
 
 ´´´
     INFORME:[INICIO],[FIN]
 ´´´
 En caso de no quererse indicar un inicio o fin, simplemente se deja vacío, por ejemplo:
 ´´´
    INFORME:Gonzalez,Gutierrez
    INFORME:,Gutierrez
    INFORME:Gonzalez,
    INFORME:,
 ´´´
 Salida (N + 1líneas, donde N es el número de doctores en el sistema que se encuentran en dicho rango):
 ´´´
    N doctor(es) en el sistema
    1: NOMBRE, especialidad ESPECIALIDAD, X paciente(s) atendido(s)
    2: NOMBRE, especialidad ESPECIALIDAD, Y paciente(s) atendido(s)
    ...
    N: NOMBRE, especialidad ESPECIALIDAD, Z paciente(s) atendido(s)
 ´´´
 

 
 
 
 
 
