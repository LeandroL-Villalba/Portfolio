#ifndef TEST_H_INCLUDED
#define TEST_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lista_doble.h"

#define FILE_NAME_EMP_DAT "datos.bin"
#define FILE_NAME_SECT "sectores.txt"

#define ALL_OK 1
#define FILE_ERROR -2
#define NO_MEMORY -1
#define MAX_LINE 20
#define MAX_NOMBRE 30
#define SECTOR_NOT_FOUND 0

//#define CALC_POS(x, y) ( x * y ) //lista empieza en 0
#define CALC_POS(x, y) ( (x - 1) * y ) //lista empieza en 1

#define MENU_SECT_MAXSECT 3
#define MENU_SECT "///SECTORES///\n\n[1]GERENCIA\n[2]MONTAJE\n[3]EMPAQUETADO\n"
#define MENU_OPTS_PRCSO "\n[1]AUMENTO/BAJA DE SUELDO\n[2]INGRESAR EMPLEADO\n[3]DESVINCULAR EMPLEADO\n[4]ATRAS\n"

typedef struct
{
    unsigned id;
    char ape_nom[30];
    unsigned dni;
    unsigned antiguedad;
    float sueldo;
    char estado_emp[9];

}t_empleado;

////FUNCIONES VARIAS////
void mostrar_emp(void*, void*);
int comparar_emp_id(const void*, const void*);
void hacer_nada(void*, const void*, void*);

////CARGA ARCHIVO////
int obtener_rango(int*, int*);
int cargar_datos_core(t_lista*, int*, int*, unsigned, int (*comparar)(const void*, const void*), void (*accion)(void*, const void*, void*), void*);
int cargar_datos(t_lista*);

////MODIFICACION DATOS////
//Modificar Sueldo
void cambio_sueldo(void*, void*);
void modificar_sueldo(t_lista*);
//Modificar Empleado
void modificar_info_emp(void*, void*);
void obtener_id(void*, void*);
void completar_empleado(t_empleado*);
void autocompletar_empleado(t_empleado*);
void modificar_empleado(t_lista*, unsigned);

int procesar_seleccion(t_lista*);

////BUSQUEDA////
//Busqueda Manual
void busqueda_manual(t_lista*);
//Busqueda Automatica
int menu_busqueda_automatica(unsigned*, unsigned*result_op);
void busqueda_automatica(t_lista*);

////FUNCION PROCESAMIENTO PRINCIPAL////
void procesamiento(t_lista*);

////ACTUALIZACION ARCHIVO////
int calcular_pos_update(void*, unsigned);
int actualizar_cambios_core(t_lista*, unsigned);
int actualizar_cambios(t_lista*);

int test_c();

#endif // TEST_H_INCLUDED
