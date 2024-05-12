#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>
#include <string.h>
#include "./lib/VentasF.h"
#include "./lib/clientes.h"
#include "producto.h"

#include <sys/stat.h>
#include <sys/types.h>



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


int consultaClav2(cliente *resultado){

    FILE *archClientes;
    int idbusqueda;
    int flag = 0;

    archClientes = fopen("./bin/clientes.bin", "rb");
    if (archClientes == NULL){
        printf("\nError al abrir el archivo. :C");
        return 0;
    }

    printf("\nIngrese el ID del cliente a buscar (Pulse 0 para regresar al submenu): ");
    fflush(stdin);
    scanf("%d", &idbusqueda);
    if(idbusqueda == 0)
    {
        return 0;
    }

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

int consultaClavP(Producto *ProductoVf){

    FILE *archProducto;
    int idbusqueda;
    int flag = 0;

    archProducto = fopen("./bin/productos.bin", "rb");
    if (archProducto == NULL){
        printf("\nError al abrir el archivo.");
        return 0;
    }

    printf("\nIngrese el ID del producto a comprar (Pulse 0 para regresar al submenu): ");
    fflush(stdin);
    scanf("%d", &idbusqueda);
    if(idbusqueda == 0)
    {
        return 0;
    }

    while(fread(ProductoVf, sizeof(Producto), 1, archProducto)){
        if (idbusqueda == ProductoVf->clave){
            flag = 1;
            break;
        }
    }
    fclose(archProducto);
    
    if (flag == 0){
        printf("\nNo se encontraron resultados.");
        return consultaClavP(ProductoVf);
        
    }
    return flag;
}


void procesoVenta(dataV *ventaF, infoI *InfoEF)
{
    int cantidad, op;
    Producto ProductoV;
    if(consultaClavP(&ProductoV) !=0)
    {
        ventaF->ID_Producto = ProductoV.clave;
        ventaF->precio = ProductoV.precio;
        printf("\nInformacion de producto: \nDescripcion: %s \nExistencia: %d \nPrecio: %.2f", ProductoV.descripcion, ProductoV.cantidad, ProductoV.precio );
        do{
            printf("\nIngrese la cantidad a comprar: ");
            scanf("%d", &cantidad);
        }while(cantidad < 1 || cantidad > ProductoV.cantidad);
        ventaF->cantidadC = cantidad;
        InfoEF->subtotal+= ProductoV.precio;


        FILE *archivoVentas = fopen("bin/VentasG.bin", "ab");
        fwrite(ventaF, sizeof(dataV), 1, archivoVentas);


        printf("\nDesea hacer otra compra (Si =1 / No =0 )");
        scanf("%d",op);
        if(op == 1)
        {
            procesoVenta(ventaF, InfoEF);
        }else
        {
            return;
        }
    }else{
        return;
    }
}




void procesoTicket()
{
    dataV venta;
    dataV ventaL;
    infoI infoE;
    cliente clienteV;    
    if(consultaClav2(&clienteV) == 1)
    {
        venta.ID_Cliente=clienteV.ID;
        struct stat st;
        const char *nombreArchivo = "bin/ventasG.bin";
        if(stat(nombreArchivo, &st)== 0)
        {
            FILE*archivoVentas = fopen(nombreArchivo, "rb+");
            while(fread(&ventaL, sizeof(dataV),1, archivoVentas) !=1)
            {}
            venta.folio = ventaL.folio+1;
        }
        else
        {
            FILE *archivo = fopen(nombreArchivo,"wb+");
            fclose(archivo);
            venta.folio=1;
        }


        obtener_fecha(venta.fecha, 11);
        strcpy(infoE.fecha,venta.fecha);
        infoE.folio = venta.folio;
        strcpy(infoE.cliente, clienteV.nombre);
        infoE.subtotal =0.0;

        procesoVenta(&venta, &infoE);

        infoE.descuento = infoE.subtotal*clienteV.descuento;
        infoE.iva = infoE.subtotal+infoE.subtotal*.16;
        infoE.total = infoE.subtotal+infoE.iva-infoE.descuento;

        FILE *archiInfoE = fopen("bin/InfoE.bin", "ab");
        fread(&infoE, sizeof(infoI), 1, archiInfoE);
        fclose(archiInfoE);

    }
}


void ventasDiaActual()
{
    infoI datosExtras;
    char fecha[11];
    obtener_fecha(fecha, sizeof(fecha));
    printf("\nReporte de ventas del dia actual:\n\n ");
    printf("\n\t\t\t\tComercializadora Fuentes");
    printf("\n\t\t\t   Reporte de Ventas al dia %s\n", fecha);
    printf("%-8s%-20s%-10s%-12s%-9s%-9s\n", "Folio", "Cliente", "Subtotal", "Descuento", "IVA", "Total");
    FILE *archDaEx= fopen("bin/datosExtras.bin", "rb");
    while (fread(&datosExtras, sizeof(infoI),1, archDaEx)!=0)
    {
        if(strcmp(datosExtras.fecha, fecha) == 0)
        {
            printf("%-8s%-20s%-10s%-12s%-9s%-9s\n", datosExtras.folio, datosExtras.cliente,datosExtras.subtotal,datosExtras.descuento,datosExtras.iva, datosExtras.total );
        }
    }
    
}  

