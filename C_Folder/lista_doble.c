#include "lista_doble.h"

//FUNC. BASICAS
void lista_crear(t_lista*lista)
{
    *lista = NULL;
}

int lista_vacia(const t_lista*lista)
{
    return (NULL == *lista);
}

//FUNC. MOVIMIENTO
int lista_check_ant_sig(const t_lista*lista, unsigned sentido)
{
    if(*lista)
    {
        if(sentido)
        {
            if( (*lista)->sig )
                return 1;
        }
        else
        {
            if( (*lista)->ant )
                return 1;
        }
    }

    return EMPTY;
}

void lista_mover_actual(t_lista*lista, unsigned sentido)
{
    if(*lista)
    {
        if(sentido)
        {
            if( (*lista)->sig )
                *lista = (*lista)->sig;
        }
        else
        {
            if( (*lista)->ant )
                *lista = (*lista)->ant;
        }
    }
}

//FUNC. BUSQUEDA
t_nodo* lista_search_nodo(t_lista*lista, int (*comparar)(const void*, const void*), void*clave)
{
    t_nodo*actual;

    actual = *lista;

    if(actual)
    {
        while( (actual->sig) && (comparar(actual->dato, clave) > 0) )
            actual = actual->sig;

        while( (actual->ant) && (comparar(actual->dato, clave) < 0) )
            actual = actual->ant;

        if( !comparar(actual->dato, clave) )
        {
            *lista = actual;

            return actual;
        }
    }

    return NULL;
}

//FUNC. OPERAR SOBRE LISTA
int lista_insertar(t_lista*lista, const void*dato, unsigned tam_dato, int (*comparar)(const void*, const void*), void (*accion)(void*, const void*, void*), void*aux)
{
    t_nodo *nodo_nue, *actual, *aux_ant, *aux_sig;
    int cmp;

    actual = *lista;

    if(!actual)
        aux_ant = aux_sig = NULL;
    else
    {
        while( (actual->ant) && (comparar(actual->dato, dato) < 0) )
            actual = actual->ant;

        while( (actual->sig) && (comparar(actual->dato, dato) > 0) )
            actual = actual->sig;

        cmp = comparar(actual->dato, dato);

        if(!cmp)
        {
            accion(actual->dato, dato, aux);
            return 0;
        }
        else if(cmp < 0)
        {
            aux_ant = actual->ant;
            aux_sig = actual;
        }
        else
        {
            aux_ant = actual;
            aux_sig = actual->sig;
        }
    }

    nodo_nue = (t_nodo*)malloc(sizeof(t_nodo));
    if(!nodo_nue)
        return NO_MEMORY;

    nodo_nue->dato = malloc(tam_dato);
    if(!(nodo_nue->dato))
    {
        free(nodo_nue);
        return NO_MEMORY;
    }

    memcpy(nodo_nue->dato, dato, tam_dato);
    nodo_nue->tam = tam_dato;

    nodo_nue->sig = aux_sig;
    nodo_nue->ant = aux_ant;

    if(aux_sig)
        aux_sig->ant = nodo_nue;

    if(aux_ant)
        aux_ant->sig = nodo_nue;

    *lista  = nodo_nue;

    return ALL_OK;
}

void lista_recorrer(t_lista*lista, int orden, void (*accion)(void*, void*), void*aux)
{
    t_nodo*actual;

    actual = *lista;

    if(actual)
    {
        if(!orden)
            while(actual->ant)
                actual = actual->ant;
        else
            while(actual->sig)
                actual = actual->sig;

        while(actual)
        {
            accion(actual->dato, aux);

            if(!(actual->ant) || !(actual->sig))
                *lista = actual;

            if(!orden)
                actual = actual->sig;
            else
                actual = actual->ant;
        }
    }
}

void lista_procesar_actual(t_lista*lista, void (*accion)(void*, void*), void*aux)
{
    if(*lista)
        accion( (*lista)->dato, aux );
}

int lista_extraer_actual(t_lista*lista, void*dato_ext, unsigned tam_dato_user)
{
    t_nodo *actual, *aux_ant, *aux_sig;

    actual = *lista;

    if(!actual)
        return EMPTY;

    aux_ant = actual->ant;
    aux_sig = actual->sig;

    memcpy(dato_ext, actual->dato, MINIMO(actual->tam, tam_dato_user));

    free(actual->dato);
    free(actual);

    if(aux_sig)
    {
        aux_sig->ant = aux_ant;

        *lista = aux_sig;
    }

    if(aux_ant)
    {
        aux_ant->sig = aux_sig;

        if(!aux_sig)
            *lista = aux_ant;
    }

    if( !aux_sig && !aux_ant )
        *lista = NULL;

    return ALL_OK;
}

//FUNC. VACIAR
void lista_vaciar(t_lista*lista)
{
    t_nodo *nodo_elim, *aux_ant, *aux_sig;

    nodo_elim = *lista;

    if(nodo_elim)
    {
        aux_ant = nodo_elim->ant;
        aux_sig = nodo_elim->sig;

        while(nodo_elim)
        {
            free(nodo_elim->dato);
            free(nodo_elim);

            if(aux_sig)
            {
                nodo_elim = aux_sig;
                aux_sig = nodo_elim->sig;
            }
            else if(aux_ant)
            {
                nodo_elim = aux_ant;
                aux_ant = nodo_elim->ant;
            }
            else
                nodo_elim = NULL;
        }

        *lista = NULL;
    }
}
