#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>
#include "./lib/VentasF.h"
#include "./lib/clientes.h"

#include <sys/stat.h>
#include <sys/types.h>


typedef struct ventasData{
    int folio;
    long ID_Cliente;
    int ID_Producto;
    int cantidadC;
    float precio;
    char fecha[11];
} dataV;





void obtener_fecha(char *fecha_strF, size_t max_len) //Funcion para obtener una fecha en formato string
{//Si se quiere usar la funcion tienes que mandarle como argumentos la cadena donde se va a guardar(Longitud minima de 11) y un entero con valor minimo de  11
    //Obtenemos la fecha
    time_t t = time(NULL);
    struct tm *fecha_actual = localtime(&t);

    
    // Formatea la fecha en la cadena
    strftime(fecha_strF, max_len, "%Y-%m-%d", fecha_actual);
}



int menuF(void) //Esta funcion solo muestra el menu de ventas y retorna la opcion elegida
{
    system("cls");
    setlocale(LC_ALL, "spanish");//Cambiamos el idioma a español REVISAR SI FUNCIONA EN TODAS LAS MAQUINAS
    int op;
    system("cls");
    do{

        printf("\n\n\t1.- Venta\n\t2.- Reporte\n\t\t- Día\n\t\tCliente\n\t3.- Regresar al Menú Principal");
        printf("\nSeleccione una opción: ");
        scanf("%d", &op);
    } while(op > 3 || op <1);
    return op;

}


int menuReporte(void)
{
    int op;
    system("cls");
    do{
        printf("\n\n\t1.- Día\n\t2.- Cliente\nIngresa una opción: ");
        scanf("%d", &op);
    } while(op != 1 && op != 2);
    return op;
}

//________________Agregado del 9 de mayo______________________

int consultaClav2(cliente *resultado){

    FILE *archClientes;
    int idbusqueda;
    char fecha[11];
    int flag = 0;

    archClientes = fopen("./bin/clientes.bin", "rb");
    if (archClientes == NULL){
        printf("\nError al abrir el archivo.");
        return 0;
    }

    printf("\nIngrese el ID del cliente a buscar (Pulse 0 para regresar al submenu): ");
    fflush(stdin);
    scanf("%d", &idbusqueda);
    if(idbusqueda == 0)
    {
        return 0;
    }

    obtener_fecha(fecha, sizeof(fecha));

    while(fread(resultado, sizeof(cliente), 1, archClientes)!= 0){
        if (idbusqueda == resultado->ID){
            flag = 1;
            break;
        }
    }
    fclose(archClientes);
    
    if (flag == 0){
        printf("\nNo se encontraron resultados.");
        return consultaClav2(resultado);
        
    }
    return flag;
}


void procesoVenta(dataV *ventaF)
{
    int cantidad, op;
    printf("\nIngresa la cantidad que desea comprar");
    scanf("%d", &cantidad);


    printf("\nDesea algo mas (Si = 1 / No =0) ? ");
}


void procesoTicket()
{
    dataV venta;
    cliente clienteV;    
    if(consultaClav2(&clienteV) == 1)
    {
        struct stat st;
  
        const char *nombreArchivo = "bin/ventasG.bin";
        if(stat(nombreArchivo, &st)== 0)
        {

        }
        else
        {
            FILE *archivo = fopen(nombreArchivo,"rb");
            fclose(archivo);
            venta.folio=1;
        }
    }
}

