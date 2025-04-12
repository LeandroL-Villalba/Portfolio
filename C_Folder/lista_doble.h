#ifndef LISTA_DOBLE_H_INCLUDED
#define LISTA_DOBLE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALL_OK 1
#define NO_MEMORY -1
#define NOT_FOUND 0
#define EMPTY 0

#define MINIMO(x, y) ( (x < y)? x:y )

typedef struct s_nodo
{
    unsigned tam;
    void*dato;
    struct s_nodo *ant, *sig;

}t_nodo;

typedef t_nodo* t_lista;

//FUNC. BASICAS
void lista_crear(t_lista*);
int lista_vacia(const t_lista*);

//FUNC. MOVIMIENTO
int lista_check_ant_sig(const t_lista*, unsigned);
void lista_mover_actual(t_lista*, unsigned);

//FUNC. BUSQUEDA
t_nodo* lista_search_nodo(t_lista*, int (*comparar)(const void*, const void*), void*);

//FUNC. OPERAR SOBRE LISTA
int lista_insertar(t_lista*, const void*, unsigned, int (*comparar)(const void*, const void*), void (*accion)(void*, const void*, void*), void*);
void lista_recorrer(t_lista*, int, void (*accion)(void*, void*), void*);
void lista_procesar_actual(t_lista*, void (*accion)(void*, void*), void*);
int lista_extraer_actual(t_lista*, void*, unsigned);

//FUNC. VACIAR
void lista_vaciar(t_lista*);

#endif // LISTA_DOBLE_H_INCLUDED
