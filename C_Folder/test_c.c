#include "test_c.h"

////FUNCIONES VARIAS////
void mostrar_emp(void*emp, void*aux)
{
    printf("%3u | %-30s | %8u | %10u | %8.2f | %8s\n",
            ((t_empleado*)emp)->id,
            ((t_empleado*)emp)->ape_nom,
            ((t_empleado*)emp)->dni,
            ((t_empleado*)emp)->antiguedad,
            ((t_empleado*)emp)->sueldo,
            ((t_empleado*)emp)->estado_emp);
}

int comparar_emp_id(const void*a, const void*b)
{
    if( (((t_empleado*)a)->id) > (((t_empleado*)b)->id) )
        return -1;
    else if( (((t_empleado*)a)->id) < (((t_empleado*)b)->id) )
        return 1;

    return 0;
}

void hacer_nada(void*dato_lista, const void*dato_ext, void*aux)
{};

////CARGA ARCHIVO////
int obtener_rango(int*lim_sup, int*lim_inf)
{
    FILE*arch;
    int i = 0, opc, estado = ALL_OK;
    char linea[MAX_LINE], *p;

    arch = fopen(FILE_NAME_SECT, "rt");
    if(arch)
    {
        puts(MENU_SECT);

        do
        {
            printf("-Ingrese el sector a cargar: ");
            scanf("%d", &opc);

            if( (opc < 1) || (MENU_SECT_MAXSECT < opc) )
                puts("\n---ERROR: sector inexistente.---\n");

        }while( (opc < 1) || (MENU_SECT_MAXSECT < opc) );


        while( (i < opc) && fgets(linea, MAX_LINE, arch) )
        {i++;};

        if(i == opc)
        {
            p = strchr(linea, ':');
            if(p)
                sscanf((p + 1), "%d-%d", lim_sup, lim_inf);
            else
                estado = FILE_ERROR;
        }
        else
            estado = SECTOR_NOT_FOUND;
    }
    else
        estado = FILE_ERROR;

    fclose(arch);

    return estado;
}

int cargar_datos_core(t_lista*emps_sector, int*lim_sup, int*lim_inf, unsigned data_tam, int (*comparar)(const void*, const void*), void (*accion)(void*, const void*, void*), void*aux)
{
    FILE*arch;
    void*data = NULL;
    int i = 0, cant, estado = ALL_OK;

    arch = fopen(FILE_NAME_EMP_DAT, "rb");
    if(arch)
    {
        data = malloc(data_tam);
        if(data)
        {
            fseek(arch, CALC_POS( (*lim_sup), data_tam ), SEEK_CUR);

            cant = (*lim_inf) - (*lim_sup) + 1;

            fread(data, data_tam, 1, arch);
            while( (!feof(arch)) && (i++ < cant) )
            {
                lista_insertar(emps_sector, data, data_tam, comparar, accion, NULL);

                fread(data, data_tam, 1, arch);
            }
        }
        else
            estado = NO_MEMORY;
    }
    else
        estado = FILE_ERROR - 1;

    fclose(arch);
    free(data);

    return estado;
}

int cargar_datos(t_lista*emps_sector)
{
    int lim_sup, lim_inf;

    if( FILE_ERROR == obtener_rango(&lim_sup, &lim_inf) )
        return FILE_ERROR;

    return cargar_datos_core(emps_sector, &lim_sup, &lim_inf, sizeof(t_empleado), comparar_emp_id, hacer_nada, NULL);
}

////MODIFICACION DATOS////
//Modificar Sueldo
void cambio_sueldo(void*emp, void*cambio)
{
    ((t_empleado*)emp)->sueldo += (*(float*)cambio);
}

void modificar_sueldo(t_lista*emps_sector)
{
    float cambio;
    int opc;

    system("cls");

    puts("//AUMENTO/BAJA DE SUELDO//\n");

    do
    {
        printf("-Ingrese la cantidad a cambiar: ");
        scanf("%f", &cambio);

        if(cambio < 0)
            puts("\n---ERROR: cantidad negativa.---");

    }while(cambio < 0);

    do
    {
        printf("-Ingrese la operacion a realizar ( Reduccion(0)/Aumento(1) ): ");
        scanf("%d", &opc);

        if( (opc < 0) || (1 < opc) )
            puts("\n---ERROR: operacion inexistente.---");
        else
        {
            if(!opc)
            {
                cambio = cambio * (-1);
                lista_procesar_actual(emps_sector, cambio_sueldo, &cambio);
            }
            else
                lista_procesar_actual(emps_sector, cambio_sueldo, &cambio);
        }

    }while( (opc < 0) || (1 < opc) );
}

//Modificar Empleado
void modificar_info_emp(void*dato_lista, void*data_nueva)
{
    memcpy(dato_lista, data_nueva, sizeof(t_empleado));
}

void obtener_id(void*emp, void*aux)
{
    ((t_empleado*)aux)->id = ((t_empleado*)emp)->id;
}

void completar_empleado(t_empleado*aux)
{
    char*p;

    system("cls");

    puts("///COMPLETAR DATOS EMPLEADO///\n");

    fflush(stdin);
    printf("-Ingresar nombre(Apellido Nombre): ");
    fgets( (aux->ape_nom), MAX_NOMBRE, stdin );

    p = strchr( (aux->ape_nom), '\n' );
    if(p)
        *p = '\0';

    do
    {
        printf("\n-Ingresar DNI: ");
        scanf("%u", &(aux->dni));

        if( ((aux->dni) < 10000000) || (99999999 < (aux->dni)) )
            puts("\n---ERROR: numero invalido.---");

    }while( 99999999 < (aux->dni) );

    aux->antiguedad = 0;

    do
    {
        printf("\n-Ingresar sueldo: ");
        scanf("%f", &(aux->sueldo));

        if( (aux->sueldo) < 0 )
            puts("\n---ERROR: numero invalido.---");

    }while( (aux->sueldo) < 0 );

    strcpy( (aux->estado_emp), "ACTIVO" );
}

void autocompletar_empleado(t_empleado*aux)
{
    strcpy( (aux->ape_nom), "-" );
    aux->dni = 0;
    aux->antiguedad = 0;
    aux->sueldo = 0;
    strcpy( (aux->estado_emp), "VACANTE" );
}

void modificar_empleado(t_lista*emps_sector, unsigned opc)
{
    t_empleado aux;

    lista_procesar_actual(emps_sector, obtener_id, &aux);

    if(opc)
        completar_empleado(&aux);
    else
        autocompletar_empleado(&aux);

    lista_procesar_actual(emps_sector, modificar_info_emp, &aux);
}

int procesar_seleccion(t_lista*emps_sector)
{
    int opc;

    system("cls");

    puts("///OPERACIONES///\n");

    printf("%3s | %-30s | %8s | %10s | %8s | %8s\n",
        "ID", "APELLIDO Y NOMBRE", "DNI", "ANTIGUEDAD", "SUELDO", "ESTADO");
    lista_procesar_actual(emps_sector, mostrar_emp, NULL);

    printf(MENU_OPTS_PRCSO);

    do
    {
        printf("\n-Ingrese la opcion deseada: ");
        scanf("%d", &opc);

        if( (opc < 1) || (4 < opc) )
            puts("\n---ERROR: opcion inexistente.---");

    }while( (opc < 1) || (4 < opc) );

    switch(opc)
    {
        case 1: modificar_sueldo(emps_sector);
        break;
        case 2: modificar_empleado(emps_sector, 1);
        break;
        case 3: modificar_empleado(emps_sector, 0);
    }

    return opc;
}

////BUSQUEDA////
//Busqueda Manual
void busqueda_manual(t_lista*emps_sector)
{
    int opc, caso, result_op = 4;

    do
    {
        system("cls");

        if(4 != result_op)
        {
            puts("---OPERACION REALIZADA CON EXITO.---\n");
            result_op = 4;
        }


        caso = 4;

        puts("///BUSQUEDA MANUAL///\n");

        puts("EMPLEADO:");
        printf("%3s | %-30s | %8s | %10s | %8s | %8s\n",
        "ID", "APELLIDO Y NOMBRE", "DNI", "ANTIGUEDAD", "SUELDO", "ESTADO");

        lista_procesar_actual(emps_sector, mostrar_emp, NULL);
        printf("\n");

        if( !lista_check_ant_sig(emps_sector, 1) )
            caso -= 3;
        else
            puts("[1]SIGUIENTE");

        if( !lista_check_ant_sig(emps_sector, 0) )
            caso -= 1;
        else
            puts("[2]ANTERIOR");

        puts("[3]SELECCIONAR\n[4]ATRAS\n");

        do
        {
            printf("-Ingrese la opcion deseada: ");
            scanf("%d", &opc);

            if( (opc < 1) || (4 < opc) )
                puts("---ERROR: opcion inexistente.---\n");

        }while( (opc < 1) || (4 < opc) );

        switch(opc)
        {
            case 1: if(2 < caso)
                        lista_mover_actual(emps_sector, 1);
            break;
            case 2: if( (0 < caso) && (3 != caso) )
                        lista_mover_actual(emps_sector, 0);
            break;
            case 3: result_op = procesar_seleccion(emps_sector);
        }

    }while(4 != opc);
}

//Busqueda Automatica
int menu_busqueda_automatica(unsigned*estado, unsigned*result_op)
{
    int opc;

    system("cls");

    if( !(*estado) )
        puts("---CLAVE NO ENCONTRADA.---\n");

    if( 4 != (*result_op) )
        puts("---OPERACION REALIZADA CON EXITO.---\n");

    puts("///BUSQUEDA AUTOMATICA///\n\n[1]Buscar otro ID.\n[2]Atras.\n");

    do
    {
        printf("-Ingrese la opcion deseada: ");
        scanf("%d", &opc);

        if( (opc < 1) || (2 < opc) )
            puts("---ERROR: opcion inexistente.---\n");

    }while( (opc < 1) || (2 < opc) );

    return (opc - 1);
}

void busqueda_automatica(t_lista*emps_sector)
{
    unsigned clave, estado, result_opc;

    do
    {
        system("cls");

        result_opc = 4;
        estado = 1;

        printf("///BUSQUEDA AUTOMATICA///\n\n-Ingrese el numero de ID a buscar: ");
        scanf("%u", &clave);

        if( lista_search_nodo(emps_sector, comparar_emp_id, &clave) )
            result_opc = procesar_seleccion(emps_sector);
        else
            estado = 0;

    }while( !menu_busqueda_automatica(&estado, &result_opc) );
}

////FUNCION PROCESAMIENTO PRINCIPAL////
void procesamiento(t_lista*emps_sector)
{
    int opc = 0;

    do
    {
        system("cls");

        if(!opc)
            puts("---CARGA CORRECTA---\n");

        puts("///TIPO DE BUSQUEDA///\n\n[1]BUSQUEDA MANUAL\n[2]BUSQUEDA AUTOMATICA\n[3]SALIR\n");

        do
        {
            printf("-Ingrese la opcion deseada: ");
            scanf("%d", &opc);

            if( (opc < 1) || (3 < opc) )
                puts("\n---ERROR: opcion inexistente.---\n");

        }while( (opc < 1) || (3 < opc) );

        if(1 == opc)
            busqueda_manual(emps_sector);
        else if(2 == opc)
            busqueda_automatica(emps_sector);

    }while(3 != opc);
}

////ACTUALIZACION ARCHIVO////
int calcular_pos_update(void*data, unsigned tam_data)
{
    return CALC_POS( ((t_empleado*)data)->id, tam_data );
}

int actualizar_cambios_core(t_lista*lista, unsigned tam_data)
{
    FILE*arch;
    void*data = NULL;
    int estado = ALL_OK;

    arch = fopen(FILE_NAME_EMP_DAT, "r+b");
    if(arch)
    {
        data = malloc(tam_data);
        if(data)
        {
            while( !lista_vacia(lista) )
            {
                lista_extraer_actual(lista, data, tam_data);

                fseek(arch, calcular_pos_update(data, tam_data), SEEK_SET);

                fwrite(data, tam_data, 1, arch);
            }

            free(data);
        }
        else
            estado = NO_MEMORY;
    }
    else
        estado = FILE_ERROR;

    fclose(arch);

    return estado;
}

int actualizar_cambios(t_lista*lista)
{
    return actualizar_cambios_core(lista, sizeof(t_empleado));
}

int test_c()
{
    t_lista empleados;
    int result_carga;

    lista_crear(&empleados);

    result_carga = cargar_datos(&empleados);

    if(1 == result_carga)
    {
        procesamiento(&empleados);
        actualizar_cambios(&empleados);

        system("cls");
    }
    else if( (-1) == result_carga )
        puts("\n---ERROR: NO_MEMORY---");
    else if( (-2) == result_carga )
        puts("\n---ERROR: FILE_ERROR(sectores.txt)---");
    else if( (-3) == result_carga )
        puts("\n---ERROR: FILE_ERROR(datos.bin)---");

    return result_carga;
}
