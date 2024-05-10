#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>
#include <string.h>
#include "./lib/InventarioJ.h"

#include <sys/stat.h>
#include <sys/types.h>

int menuInventario(void) {
    int opcion;
    do {

        printf("\n===== MENU INVENTARIO =====\n");
        printf("1. Reabastecer Producto\n");
        printf("2. Reportes Iventario\n");
        printf("3. Salir\n");
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);

        // Según la opción seleccionada, llamar a la función correspondiente
        switch(opcion) {
            case 1:
                reabastecerProducto();
                break;
            case 2:
                reporteInventarioCompleto();
                break;
            case 3:
                printf("Saliendo del programa...\n");
                break;
            default:
                printf("Opcion no valida. Por favor, seleccione una opcion valida.\n");
        }
    } while(opcion != 3);
    printf("¡Hasta luego!\n");
    return 0;
}

void reabastecerProducto() {
    FILE *archivoProductos = fopen("productos.txt", "r+");
    if (archivoProductos == NULL) {
        printf("Error al abrir el archivo de productos.\n");
        return;
    }
    int tipoProducto;
    int claveProducto;
    int cantidadReabastecer;
    char continuar;

    do {
        printf("\nTipos de Productos Disponibles:\n");
        printf("1. Escritorios\n");
        printf("2. Archiveros\n");
        printf("3. Sillas\n");
        printf("4. Mesas de Centro\n");
        printf("5. Mesas Ejecutivas\n");
        printf("6. Sofas\n");
        printf("Seleccione el tipo de producto: ");
        scanf(" %d", &tipoProducto);

        printf("\nProductos Existentes del Tipo %d:\n", tipoProducto);

        Producto producto;
        int encontrado = 0;

        // Buscar y mostrar productos del tipo seleccionado
        rewind(archivoProductos);
        while (fscanf(archivoProductos, "%d,%[^,],%d,%d,%d,%f,%d\n", &producto.clave, producto.descripcion, &producto.tipo, &producto.cantidadExistencia, &producto.nivelStock, &producto.precio, &producto.estatus) == 7) {
            if (producto.tipo == tipoProducto && producto.estatus == 1) {
                printf("Clave: %d\n", producto.clave);
                printf("Descripción: %s\n", producto.descripcion);
                printf("Cantidad en Existencia: %d\n", producto.cantidadExistencia);
                printf("Nivel de Stock: %d\n", producto.nivelStock);
                printf("Precio: %.2f\n\n", producto.precio);
                encontrado = 1;
            }
        }

        if (!encontrado) {
            printf("No hay productos activos del tipo seleccionado.\n");
        } else {
            printf("Ingrese la clave del producto a reabastecer: ");
            scanf("%d", &claveProducto);

            // Se busca el producto por clave y abre un archivo temporal
            rewind(archivoProductos);
            FILE *temporal = fopen("temporal.txt", "w");
            while (fscanf(archivoProductos, "%d,%[^,],%d,%d,%d,%f,%d\n", &producto.clave, producto.descripcion, &producto.tipo, &producto.cantidadExistencia, &producto.nivelStock, &producto.precio, &producto.estatus) == 7) {
                if (producto.clave == claveProducto && producto.estatus == 1) {
                    printf("\nProducto Seleccionado:\n");
                    printf("Clave: %d\n", producto.clave);
                    printf("Descripción: %s\n", producto.descripcion);
                    printf("Cantidad en Existencia: %d\n", producto.cantidadExistencia);
                    printf("Nivel de Stock: %d\n", producto.nivelStock);
                    printf("Precio: %.2f\n", producto.precio);

                    printf("Ingrese la cantidad a reabastecer: ");
                    scanf("%d", &cantidadReabastecer);

                    producto.cantidadExistencia += cantidadReabastecer;
                    encontrado = 1;
                }
                fprintf(temporal, "%d,%s,%d,%d,%d,%.2f,%d\n", producto.clave, producto.descripcion, producto.tipo, producto.cantidadExistencia, producto.nivelStock, producto.precio, producto.estatus);
            }
            fclose(temporal);

            // Se elimina el archivo original y renombra el temporal
            remove("productos.txt");
            rename("temporal.txt", "productos.txt");

            if (!encontrado) {
                printf("El producto con la clave ingresada no existe o no está activo.\n");
            } else {
                printf("Reabastecimiento exitoso.\n");
            }
        }

        printf("\n¿Desea reabastecer otro producto? (s/n): ");
        scanf(" %c", &continuar);
    } while (continuar == 's' || continuar == 'S');

    fclose(archivoProductos);
}

void reporteInventarioCompleto() {
    int opcion;

    do {
        printf("\n===== MENU DE REPORTES INVENTARIO =====\n");
        printf("1. Reporte Completo\n");
        printf("2. Reporte Por Tipo de Producto\n");
        printf("3. Reporte Nivel de Stock\n");
        printf("4. Reabastecimiento\n");
        printf("5. Regresar al Menú Principal\n");
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);

        switch(opcion) {
            case 1:
                reporteCompleto();
                break;
            case 2:
                reportePorTipoProducto();
                break;
            case 3:
                reporteNivelStock();
                break;
            case 4:
                Reabastecimiento();
                break;
            case 5:
                printf("Regresando al Menú Principal...\n");
                break;
            default:
                printf("Opcion no valida. Por favor, seleccione una opcion valida.\n");
        }
    } while(opcion != 5);
}

// Función para generar el reporte completo del inventario
void reporteCompleto() {
    FILE *archivoProductos = fopen("productos.txt", "r");
    if (archivoProductos == NULL) {
        printf("Error al abrir el archivo de productos.\n");
        return;
    }

    printf("\t\t\t********************\n");
    printf("\t\t\t\tInventario\n");
    printf("\t\t\t********************\n");
    printf("Producto\t\tClave\tCantidad en Existencia\n");

    Producto producto;

    while (fscanf(archivoProductos, "%d,%[^,],%d,%d,%d,%f,%d\n", &producto.clave, producto.descripcion, &producto.tipo, &producto.cantidadExistencia, &producto.nivelStock, &producto.precio, &producto.estatus) == 7) {
        if (producto.estatus == 1) {
            printf("%s\t\t%d\t\t%d\n", producto.descripcion, producto.clave, producto.cantidadExistencia);
        }
    }
    fclose(archivoProductos);
}


// Funcion para generar el reporte del inventario por tipo de producto
void reportePorTipoProducto() {
    int tipoactual = 1;
    int cen = 0;
    FILE *archivoProductos = fopen("productos.txt", "r");
    if (archivoProductos == NULL) {
        printf("Error al abrir el archivo de productos.\n");
        return;
    }
    printf("\n\n\t\t\t\tInventario por Tipo Producto\n\n");

    Producto producto;

    while (fscanf(archivoProductos, "%d,%[^,],%d,%d,%d,%f,%d\n", &producto.clave, producto.descripcion, &producto.tipo, &producto.cantidadExistencia, &producto.nivelStock, &producto.precio, &producto.estatus) == 7) {
        if (producto.estatus == 1) {
            if (cen == 0){
                char *categoria = obtenerCategoria(producto.tipo);
                printf("\n\t**%s**\n", categoria);
                tipoactual = producto.tipo;
                cen++;
            }
            if(producto.tipo != tipoactual){
                char *categoria = obtenerCategoria(producto.tipo);
                printf("\n\t**%s**\n", categoria);
            }

            printf("Producto\t\tClave\tCantidad en Existencia\n");
            tipoactual = producto.tipo;

            printf("%s\t\t%d\t%d\n", producto.descripcion, producto.clave, producto.cantidadExistencia);
        }
    }

    fclose(archivoProductos);
}

// Funcion para generar el reporte del nivel de stock
void reporteNivelStock() {
    FILE *archivoProductos = fopen("productos.txt", "r");
    if (archivoProductos == NULL) {
        printf("Error al abrir el archivo de productos.\n");
        return;
    }

    printf("\n\n\t\t\t\tNivel de Stock\n\n");
    printf("Clave\tPRODUCTO\tCant En existencia\tCant de Stock\tDif\t%%Faltante de Stock\n");

    Producto producto;

    while (fscanf(archivoProductos, "%d,%[^,],%d,%d,%d,%f,%d\n", &producto.clave, producto.descripcion, &producto.tipo, &producto.cantidadExistencia, &producto.nivelStock, &producto.precio, &producto.estatus) == 7) {
        if (producto.cantidadExistencia < producto.nivelStock && producto.estatus == 1) {
            float diferencia = producto.nivelStock - producto.cantidadExistencia;
            float porcentajeFaltante = (diferencia / producto.nivelStock) * 100;

            printf("%03d\t%-20s\t%d\t\t\t%d\t\t\t%.0f\t%.1f%%\n", producto.clave, producto.descripcion, producto.cantidadExistencia, producto.nivelStock, diferencia, porcentajeFaltante);
        }
    }

    fclose(archivoProductos);
}

//Funcion para mostrar los abastecimientos durante fechas
void Reabastecimiento() {
    FILE *archivoReabastecimiento = fopen("reabastecimiento.txt", "r");
    if (archivoReabastecimiento == NULL) {
        printf("Error al abrir el archivo de historial de reabastecimiento.\n");
        return;
    }

    Fecha fechaInicial, fechaFinal;

    // Se solicita al usuario las fechas inicial y final
    printf("Ingrese la fecha inicial (dd/mm/aaaa): ");
    scanf("%d/%d/%d", &fechaInicial.dia, &fechaInicial.mes, &fechaInicial.anio);
    printf("Ingrese la fecha final (dd/mm/aaaa): ");
    scanf("%d/%d/%d", &fechaFinal.dia, &fechaFinal.mes, &fechaFinal.anio);

    printf("\n\n\t\t\t\tReabastecimiento de productos\n\n");
    printf("del %02d/%02d/%04d al %02d/%02d/%04d\n\n", fechaInicial.dia, fechaInicial.mes, fechaInicial.anio, fechaFinal.dia, fechaFinal.mes, fechaFinal.anio);
    printf("Clave PROD\tPRODUCTO\tFecha\tNo Pedido\tCantidad Pedida\tNivel Stock\n\n");

    Reabastecimient registro;

    while (fscanf(archivoReabastecimiento, "%d,%[^,],%d/%d/%d,%d,%d,%d\n", &registro.claveProducto, registro.producto, &registro.fecha.dia, &registro.fecha.mes, &registro.fecha.anio, &registro.folioPedido, &registro.cantidadPedida, &registro.nivelStock) == 8) {
        if (compararFechas(registro.fecha, fechaInicial) >= 0 && compararFechas(registro.fecha, fechaFinal) <= 0) {
            printf("%d\t\t%s\t%02d/%02d/%04d\t%d\t%d\t\t%d\n", registro.claveProducto, registro.producto, registro.fecha.dia, registro.fecha.mes, registro.fecha.anio, registro.folioPedido, registro.cantidadPedida, registro.nivelStock);
        }
    }

    fclose(archivoReabastecimiento);
}

// Funcion que comparar dos fechas (devuleve -1 si la primera es menor, 0 si son iguales, y 1 si la primera es mayor)
int compararFechas(Fecha fecha1, Fecha fecha2) {
    if (fecha1.anio != fecha2.anio) {
        return (fecha1.anio < fecha2.anio) ? -1 : 1;
    } else if (fecha1.mes != fecha2.mes) {
        return (fecha1.mes < fecha2.mes) ? -1 : 1;
    } else if (fecha1.dia != fecha2.dia) {
        return (fecha1.dia < fecha2.dia) ? -1 : 1;
    } else {
        return 0;
    }
}

// Funcion que da la cadena del tipo de producto
char* obtenerCategoria(int numero) {
    switch(numero) {
        case 1:
            return "Escritorios";
        case 2:
            return "Archiveros";
        case 3:
            return "Sillas";
        case 4:
            return "Mesas de Centro";
        case 5:
            return "Mesas Ejecutivas";
        case 6:
            return "Sofas";
        default:
            return "Categoria no encontrada";
    }
}