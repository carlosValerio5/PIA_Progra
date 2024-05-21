#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Inventario.h"

// Funcion principal de Inventario
void InventarioMenu() {
    char *archivo = "./bin/productos.bin";
    int opcion;
    do {
        printf("\n===== MENU INVENTARIO =====\n");
        printf("1. Reabastecer Producto\n");
        printf("2. Reportes Inventario\n");
        printf("3. Salir\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);
        // Segun la opcion seleccionada, llamar a la funcion correspondiente
        switch(opcion) {
            case 1:
                Reabastecimiento();
                system("pause");
                break;
            case 2:
                reporteInventarioCompleto(archivo);
                system("pause");
                break;
            case 3:
                printf("Saliendo del programa...\n");
                break;
            default:
                printf("Opcion no valida. Por favor, seleccione una opcion valida.\n");
        }
    } while(opcion != 3);
    printf("¡Hasta luego!\n");
    return;
}
// Funcion para mostrar menus de reportes y llamar a las funciones
void reporteInventarioCompleto(char *archivo) {
    int opcion;
    do {
        printf("\n===== MENU DE REPORTES INVENTARIO =====\n");
        printf("1. Reporte Completo\n");
        printf("2. Reporte Por Tipo de Producto\n");
        printf("3. Reporte Nivel de Stock\n");
        printf("4. Reabastecimiento\n");
        printf("5. Regresar al Menu Principal\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);
        // Segun la opcion seleccionada, llamar a la funcion correspondiente
        switch(opcion) {
            case 1:
                reporteCompleto(archivo);
                system("pause");
                break;
            case 2:
                reportePorTipoProducto(archivo);
                system("pause");
                break;
            case 3:
                reporteNivelStock(archivo);
                system("pause");
                break;
            case 4:
                reporteReabastecimiento();
                system("pause");
                break;
            case 5:
                printf("Regresando al Menu Principal...\n");
                break;
            default:
                printf("Opcion no valida. Por favor, seleccione una opcion valida.\n");
        }
    } while(opcion != 5);
}

//Funcion para Reabastecer productos
void Reabastecimiento() {
    int categoria;
    printf("Ingrese el numero de la categoria que desea reabastecer:\n");
    printf("1. Escritorios\n2. Archiveros\n3. Sillas\n4. Mesas de Centro\n5. Mesas Ejecutivas\n6. Sofas\n");
    scanf("%d", &categoria);

    char *categoriaNombre = obtenerCategoria(categoria);
    if (strcmp(categoriaNombre, "Categoria no encontrada") == 0) {
        printf("Categoria no valida.\n");
        return;
    }

    printf("Productos en la categoria: %s\n", categoriaNombre);

    FILE *file = fopen("productos.bin", "rb+");
    if (file == NULL) {
        perror("Error al abrir el archivo productos.bin");
        return;
    }

    struct Producto producto;
    int encontrado = 0;

    // Mostrar todos los productos de la categoria seleccionada
    while (fread(&producto, sizeof(struct Producto), 1, file) == 1) {
        if (producto.estatus == 1 && producto.tipo_producto == categoria) {
            printf("Clave: %d, Nombre: %s, Cantidad: %d, Stock: %d\n", producto.clave, producto.nombre, producto.cantidad, producto.stock);
            encontrado = 1;
        }
    }

    if (!encontrado) {
        printf("No se encontraron productos en la categoria seleccionada.\n");
        fclose(file);
        return;
    }

    int clave, cantidadAReabastecer;
    char opcion;

    do {
        encontrado = 0;
        printf("Ingrese la clave del producto a reabastecer: ");
        scanf("%d", &clave);

        // Volver al inicio del archivo para buscar y mostrar el producto
        rewind(file);
        while (fread(&producto, sizeof(struct Producto), 1, file) == 1) {
            if (producto.estatus == 1 && producto.clave == clave) {
                printf("Producto seleccionado: Clave: %d, Nombre: %s, Cantidad actual: %d, Stock: %d\n", producto.clave, producto.nombre, producto.cantidad, producto.stock);
                encontrado = 1;
                break;
            }
        }

        if (!encontrado) {
            printf("Clave de producto no valida.\n");
            printf("¿Desea realizar otra consulta? (s/n): ");
            scanf(" %c", &opcion);
            if (opcion != 's' && opcion != 'S') {
                fclose(file);
                return;
            }
            continue;
        }

        printf("Ingrese la cantidad a reabastecer: ");
        scanf("%d", &cantidadAReabastecer);

        // Volver al inicio del archivo para buscar y actualizar el producto
        rewind(file);
        while (fread(&producto, sizeof(struct Producto), 1, file) == 1) {
            if (producto.estatus == 1 && producto.clave == clave) {
                producto.cantidad += cantidadAReabastecer;  // Sumar la cantidad a reabastecer a la existente

                // Posicionar el puntero del archivo para reescribir el producto actualizado
                fseek(file, -(long)sizeof(struct Producto), SEEK_CUR);
                fwrite(&producto, sizeof(struct Producto), 1, file);

                printf("Producto actualizado: Clave: %d, Nombre: %s, Nueva Cantidad: %d\n", producto.clave, producto.nombre, producto.cantidad);

                // Registrar el reabastecimiento
                FILE *reabFile = fopen("reabastecimientos.bin", "ab");
                if (reabFile == NULL) {
                    perror("Error al abrir el archivo reabastecimientos.bin");
                    fclose(file);
                    return;
                }

                int folioCounter = obtenerFolioActual();  // Obtener el folio actual
                struct Reabastecimiento reab;
                reab.folio = folioCounter;
                reab.clave = producto.clave;
                reab.fecha =  obtenerFechaActual();
                reab.cantidadPedida = cantidadAReabastecer;

                fwrite(&reab, sizeof(struct Reabastecimiento), 1, reabFile);
                fclose(reabFile);

                actualizarFolio(folioCounter + 1);  // Incrementar y guardar el nuevo folio

                break;
            }
        }

        printf("¿Desea reabastecer otro producto? (s/n): ");
        scanf(" %c", &opcion);

    } while (opcion == 's' || opcion == 'S');

    fclose(file);
}

// Funcion para generar el reporte completo del inventario
void reporteCompleto(char *archivo) {

    printf("\t\t\t********************\n");
    printf("\t\t\tComercialiadora Fuentes\n");
    printf("\t\t\t\t Inventario\n");
    Fecha fechaActual = obtenerFechaActual();
    printf("\t\t\t\t%02d/%02d/%04d\n", fechaActual.dia, fechaActual.mes, fechaActual.anio);
    printf("\t\t\t********************\n");
    // Abrir el archivo en modo de lectura binaria
    FILE *file = fopen(archivo,"rb");
    if (file == NULL) {
        perror("Error al abrir el archivo productos.bin");
        return;
    }

    // Leer y mostrar los binos de los productos
    struct Producto producto;
    printf("Producto%26sClave%3sCantidad en existencia\n"," "," ");
    while (fread(&producto, sizeof(struct Producto), 1, file) == 1) {
        if (producto.estatus == 1) {
            printf("%-35s%03d%15d\n", producto.descripcion, producto.clave, producto.cantidad);
            printf("\n");
        }
    }

    // Cerrar el archivo
    fclose(file);
    return;
}

// Funcion para generar el reporte de historial de reabastecimientos entre fechas
void reporteReabastecimiento() {
    // Abrir el archivo en modo de lectura binaria
    FILE *file = fopen("./bin/reabastecimientos.bin", "rb");
    if (file == NULL) {
        perror("Error al abrir el archivo reabastecimientos.bin");
        return;
    }

    Fecha fechaInicio, fechaFin;
    printf("Ingrese la fecha de inicio (dd mm aaaa): ");
    scanf("%d %d %d", &fechaInicio.dia, &fechaInicio.mes, &fechaInicio.anio);
    printf("Ingrese la fecha de fin (dd mm aaaa): ");
    scanf("%d %d %d", &fechaFin.dia, &fechaFin.mes, &fechaFin.anio);

    struct Reabastecimiento reab;
    int hayReabastecimientos = 0;
    printf("\t\t\t********************\n");
    printf("\t\t\tComercialiadora Fuentes\n");
    printf("\t\t\t  Reabastecimientos\n");
    Fecha fechaActual = obtenerFechaActual();
    printf("\t\t\t\t%02d/%02d/%04d\n", fechaActual.dia, fechaActual.mes, fechaActual.anio);
    printf("\t\t\t********************\n");
    printf("Reporte de Reabastecimientos del %02d/%02d/%04d al %02d/%02d/%04d:\n",
           fechaInicio.dia, fechaInicio.mes, fechaInicio.anio,
           fechaFin.dia, fechaFin.mes, fechaFin.anio);
    printf("Folio\tClave del Producto\tFecha\t\tCantidad Reabastecida\n");
    printf("----------------------------------------------------------------\n");
    // Leer y mostrar los binos de los reabastecimientos
    while (fread(&reab, sizeof(struct Reabastecimiento), 1, file) == 1) {
        if (compararFechas(reab.fecha, fechaInicio) >= 0 && compararFechas(reab.fecha, fechaFin) <= 0) {
            printf("%d\t%d\t\t\t%02d/%02d/%04d\t%d\n", 
                   reab.folio, 
                   reab.clave, 
                   reab.fecha.dia, 
                   reab.fecha.mes, 
                   reab.fecha.anio, 
                   reab.cantidadPedida);
            hayReabastecimientos = 1;
        }
    }

    if (!hayReabastecimientos) {
        printf("No hay reabastecimientos durante el periodo especificado.\n");
    }

    fclose(file);
}


// Funcion para generar el reporte del inventario por tipo de producto
void reportePorTipoProducto(char *archivo) {
    printf("\t\t\t********************\n");
    printf("\t\t\tComercializadora Fuentes\n");
    printf("\t\t\t  Inventario por Tipo\n");
    Fecha fechaActual = obtenerFechaActual();
    printf("\t\t\t\t%02d/%02d/%04d\n", fechaActual.dia, fechaActual.mes, fechaActual.anio);
    printf("\t\t\t********************\n");

    // Pedir al usuario que ingrese el tipo de producto
    int tipo;
    printf("Ingrese el tipo de producto (1-6) o 0 para todos los tipos: ");
    scanf("%d", &tipo);

    // Validar la entrada del usuario
    if (tipo < 0 || tipo > 6) {
        printf("Tipo de producto no valido.\n");
        return;
    }

    // Abrir el archivo en modo de lectura binaria
    FILE *file = fopen(archivo, "rb");
    if (file == NULL) {
        perror("Error al abrir el archivo productos.bin");
        return;
    }

    struct Producto producto;

    if (tipo == 0) {
        // Mostrar todos los tipos de productos
        for (int i = 1; i <= 6; i++) {
            printf("\nTIPO DE PRODUCTO %d\n", i);
            rewind(file);
            printf("Producto%26sClave%3sCantidad en existencia\n", " ", " ");
            while (fread(&producto, sizeof(struct Producto), 1, file) == 1) {
                if (producto.estatus == 1 && producto.tipo_producto == i) {
                    printf("%-35s%03d%15d\n", producto.descripcion, producto.clave, producto.cantidad);
                }
            }
        }
    } else {
        // Mostrar productos del tipo seleccionado
        printf("\nTIPO DE PRODUCTO %d\n", tipo);
        printf("Producto%26sClave%3sCantidad en existencia\n", " ", " ");
        while (fread(&producto, sizeof(struct Producto), 1, file) == 1) {
            if (producto.estatus == 1 && producto.tipo_producto == tipo) {
                printf("%-35s%03d%15d\n", producto.descripcion, producto.clave, producto.cantidad);
            }
        }
    }

    fclose(file);
}


// Funcion para generar el reporte del nivel de stock
void reporteNivelStock(char *archivo) {
    printf("\t\t\t********************\n");
    printf("\t\t\tComercializadora Fuentes\n");
    printf("\t\t\t   Nivel de Stock\n");
    Fecha fechaActual = obtenerFechaActual();
    printf("\t\t\t\t%02d/%02d/%04d\n", fechaActual.dia, fechaActual.mes, fechaActual.anio);
    printf("\t\t\t********************\n");

    float res;
    int dif;

    // Abrir el archivo en modo de lectura binaria
    FILE *file = fopen(archivo, "rb");
    if (file == NULL) {
        perror("Error al abrir el archivo productos.bin");
        return;
    }

    printf("Clave\tProducto%8sCant en existencia   Cant de Stock    Dif    %%Faltante de Stock\n", " ", " ");

    struct Producto producto;

    // Leer y mostrar los productos con faltante de stock
    while (fread(&producto, sizeof(struct Producto), 1, file) == 1) {
        if (producto.estatus == 1 && producto.cantidad < producto.stock) {
            dif = producto.stock - producto.cantidad;
            res = ((float)dif / producto.stock) * 100;
            printf("%03d       %-15s   %10d   %17d   %12d         %.2f%%\n", producto.clave, producto.nombre, producto.cantidad, producto.stock, dif, res);
        }
    }

    fclose(file);
}



// Funcion para obtener la fecha actual local
Fecha obtenerFechaActual() {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    Fecha fecha;
    fecha.dia = tm.tm_mday;
    fecha.mes = tm.tm_mon + 1;  // tm_mon es meses desde Enero [0-11]
    fecha.anio = tm.tm_year + 1900;  // tm_year es años desde 1900
    return fecha;
}
// Funcion que actualiza el archivo centinela del numero de folio
void actualizarFolio(int nuevoFolio) {
    FILE *folioFile = fopen("./bin/folio.bin", "wb");
    if (folioFile == NULL) {
        perror("Error al abrir el archivo folio.bin");
        return;
    }
    fwrite(&nuevoFolio, sizeof(int), 1, folioFile);
    fclose(folioFile);
}
// Funcion que obtiene el numero de folio del archivo centinela
int obtenerFolioActual() {
    FILE *folioFile = fopen("./bin/folio.bin", "rb");
    if (folioFile == NULL) {
        return 1; // Si no existe el archivo, iniciamos con el folio 1
    }
    int folio;
    fread(&folio, sizeof(int), 1, folioFile);
    fclose(folioFile);
    return folio;
}
// Funcion para comparar fechas
int compararFechas(Fecha fecha1, Fecha fecha2) {
    if (fecha1.anio != fecha2.anio)
        return fecha1.anio - fecha2.anio;
    if (fecha1.mes != fecha2.mes)
        return fecha1.mes - fecha2.mes;
    return fecha1.dia - fecha2.dia;
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
