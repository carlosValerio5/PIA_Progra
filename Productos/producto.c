#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // Para sleep
#include <time.h>
#include "producto.h"

//funcion para guardar productos
void guardar_producto(Producto *producto) {
    FILE *file = fopen("./bin/productos.bin", "ab");//modo para a�adir
    if (file != NULL) {
        fwrite(producto, sizeof(Producto), 1, file);//guarda un producto2
        fclose(file);
    } else {
        printf("Error al abrir el archivo\n");
    }
}

//funcion para leer los productos desde archivo
int leer_productos(Producto *productos, int max_productos) {
    FILE *file = fopen("./bin/productos.bin", "rb"); //modo de lectura
    if (file != NULL) {
        int total = fread(productos, sizeof(Producto), max_productos, file);
        fclose(file);
        return total;
    }
    return 0;
}


//funcion para verificar que la clave sea unica
int verificar_clave(int clave) {
    FILE *archivo_productos = fopen("./bin/productos.bin", "rb");
    if (archivo_productos == NULL) {
        return -1;
    }
    Producto producto;
    while (fread(&producto, sizeof(Producto), 1, archivo_productos) != 0) {
        if (producto.clave == clave) {
            fclose(archivo_productos);
            return 0;
        }
    }
    fclose(archivo_productos);
    return 1;
}

//para obtener fecha
void obtener_fecha2(char *fecha_strF, size_t max_len) {
    // Obtenemos la fecha actual
    time_t t = time(NULL);
    struct tm *fecha_actual = localtime(&t);

    // Formatea la fecha en la cadena
    strftime(fecha_strF, max_len, "%d/%m/%y", fecha_actual);
}

//funcion para nuevo producto: debe incluir clave unica, descripcion, tipo de producto, cantidad en existencia, nivel de stock(cantidad minima que debe tener en almacen), precio y estatus

void nuevo_producto(){
    Producto producto;
    int tipo, cantidad, stock;
    float precio;
    char fecha_str[11];


     while (1) {
        printf("Ingrese clave del producto (unica): ");
        fflush(stdin);
        scanf("%d", &producto.clave);

        if (verificar_clave(producto.clave)) {
            break;
        } else {
            printf("Clave en uso. Intente de nuevo\n");
        }
    }

    while (getchar() != '\n');

    //NOMBRE
    printf("Ingrese nombre del producto: ");
    fgets(producto.nombre, 50, stdin);
    size_t len = strlen(producto.nombre);
    if (producto.nombre[len - 1] == '\n') {
        producto.nombre[len - 1] == '\0';
    }

    //DESCRIPCION
    printf("Ingrese descripcion del producto: ");
    fgets(producto.descripcion, 100, stdin);
    len = strlen(producto.descripcion);
    if (producto.descripcion[len - 1] == '\n') {
        producto.descripcion[len - 1] == '\0';
    }

    //TIPO DE PRODUCTO
    printf("Ingrese tipo de producto (1: Escritorios, 2: Archiveros, 3: Sillas, 4: Mesas de centro, 5: Mesas ejecutivas, 6: Sofas): ");
    scanf("%d", &tipo);
    producto.tipo_producto = tipo;

    //CANTIDAD EN EXISTENCIA
    printf("Ingrese cantidad en existencia: ");
    scanf("%d", &cantidad);
    producto.cantidad = cantidad;

    //STOCK MINIMO
    printf("Ingrese nivel minimo de stock: ");
    scanf("%d", &stock);
    producto.stock = stock;

    //PRECIO
    printf("Ingrese precio del producto: ");
    scanf("%f", &precio);
    producto.precio = precio;

    producto.estatus = 1;

    guardar_producto(&producto);
    obtener_fecha(fecha_str, sizeof(fecha_str));
    printf("Producto agregado con exito el %s\n", fecha_str);
    sleep(2);
    return;
}

//funcion para editar producto: capturar clave unica de productgo,
void editar_producto() {
    int clave, producto_encontrado = -1;
    int opcion;
    float precio, nuevo_precio;
    char nombre_nuevo[50];
    char fecha_str[11];

    FILE *archivo_productos = fopen("./bin/productos.bin", "rb+");
    if (archivo_productos == NULL) {
        printf("Error al abrir el archivo de productos\n");
        return;
    }

    printf("Ingrese clave del producto a editar: ");
    scanf("%d", &clave);

    Producto producto;

    while (fread(&producto, sizeof(Producto), 1, archivo_productos) != 0) {
        if (producto.clave == clave) {
            producto_encontrado = 1;
            break;
        }
    }

    if (producto_encontrado == -1) {
        printf("Producto no encontrado\n");
        fclose(archivo_productos);
        return;
    }

    printf("Producto encontrado: %s\n", producto.nombre);
    printf("Seleccione una opcion para editar:\n");
    printf("1. Cambiar precio\n");
    printf("2. Cambiar nombre del producto\n");

    scanf("%d", &opcion);
    switch (opcion){
        case 1:
            printf("Ingrese nuevo precio: ");
            scanf("%f", &nuevo_precio);
            producto.precio = nuevo_precio;
            break;
        case 2:
            fflush(stdin);
            printf("Ingrese nuevo nombre del producto: ");
            fgets(nombre_nuevo, 50, stdin);
            size_t len = strlen(nombre_nuevo);
            if (nombre_nuevo[len - 1] == '\n') {
                nombre_nuevo[len - 1] == '\0';
            }
            strcpy(producto.nombre, nombre_nuevo);
            break;
        default:
            printf("Opcion invalida\n");
            fclose(archivo_productos);
            return;
    }

    fseek(archivo_productos, -(long)sizeof(Producto), SEEK_CUR);
    fwrite(&producto, sizeof(Producto), 1, archivo_productos);
    fclose(archivo_productos);

    obtener_fecha(fecha_str, sizeof(fecha_str));
    printf("Producto actualizado con exito el %s\n", fecha_str);
    sleep(3);

}



//funcion para consulta por tipo
void consulta_tipo() {
Producto *productos = NULL;
    int total = 0;
    char fecha_str[11];
    int tipo;
    int found = 0;
    FILE *archivo_productos;

    printf("Ingrese tipo de producto:\n1: Escritorios\n2: Archiveros\n3: Sillas\n4: Mesas de centro\n5: Mesas ejecutivas\n6: Sofas\n");
    scanf("%d", &tipo);

    archivo_productos = fopen("./bin/productos.bin", "rb");
    if (archivo_productos == NULL) {
        printf("Error al abrir el archivo de productos\n");
        sleep(2);
        return;
    }

    productos = malloc(sizeof(Producto)); // Reservamos espacio para un solo producto inicialmente
    system("cls");
    printf("\n");

    printf("Productos del tipo %d:\n\n", tipo);


    printf("                       Comercializadora Fuentes\n\n");
    printf("                    Consulta por Tipo de Producto %d\n\n", tipo);
     while (fread(&productos[total], sizeof(Producto), 1, archivo_productos) != 0) {
        if (productos[total].tipo_producto == tipo && productos[total].estatus == 1) {
            obtener_fecha(fecha_str, sizeof(fecha_str));
            printf("Fecha de consulta: %s", fecha_str);
            printf("%*s\n\n", (80 + strlen("")) / 2, "");
            printf("Clave: %d", productos[total].clave);
            printf("\nNombre: %s", productos[total].nombre);
            printf("\nDescripcion: %s", productos[total].descripcion);
            printf("\nCantidad en existencia: %d", productos[total].cantidad);
            printf("\nPrecio: %.2f", productos[total].precio);
            printf("\n\n");
            found = 1;
            total++;
            productos = realloc(productos, (total + 1) * sizeof(Producto)); // A�adimos espacio para otro producto
        }
    }

    if (!found) {
        printf("No hay productos de este tipo\n");
    }

    fclose(archivo_productos);
    free(productos);
    system("pause");
}


//funcion para consulta por clavep
void consulta_clave(int clave) {
    FILE *archivo_productos;
    Producto producto;
    int found = 0;
    char fecha_str[11];


    archivo_productos = fopen("./bin/productos.bin", "rb");
    if (archivo_productos == NULL) {
        printf("Error al abrir el archivo de productos\n");
        return;
    }
    system("cls");
    printf("                       Comercializadora Fuentes\n\n");
    printf("                         Consulta por Producto");
    printf("%*s\n\n", (80 + strlen("")) / 2, "");



    while (fread(&producto, sizeof(Producto), 1, archivo_productos) != 0) {
        if (producto.clave == clave) {
            printf("Clave: %d\n", producto.clave);
            printf("Nombre: %s", producto.nombre);
            printf("Descripcion: %s", producto.descripcion);
            printf("Cantidad en existencia: %d\n", producto.cantidad);
            printf("Precio: %.2f\n", producto.precio);
            printf("Estatus: %s\n", producto.estatus ? "Activo" : "Inactivo");
            found = 1;
            break;
        }
    }


    if (!found) {
        printf("Producto no encontrado\n");
    }
    system("pause");
    fclose(archivo_productos);
}


void mostrar_menu() {
    int opcion, clave, tipo;
    do{
    system("cls");
    printf("Menu Producto\n");
    printf("1. Nuevo Producto\n");
    printf("2. Editar Producto\n");
    printf("3. Consultar producto\n");
    printf("4. Regresar al menu principal\n");

    printf("Seleccione una opcion: ");
    scanf("%d", &opcion);

    switch (opcion) {
        case 1:
            nuevo_producto();
                    break;
        case 2:
         editar_producto();
                    break;
        case 3:
            system("cls");
            printf("1. Consulta por Tipo\n");
            printf("2. Consulta por Clave\n");
            printf("3. Regresar\n");

            int subopcion_cons;
            printf("Seleccione una opcion: ");
            scanf("%d", &subopcion_cons);

            switch (subopcion_cons) {
                case 1:
                    consulta_tipo();
                    break;
                case 2:
                    printf("Ingrese la clave del producto: ");
                    scanf("%d", &clave);
                    consulta_clave(clave);
                    break;
                case 3:
                    break;
                default:
                    printf("Opcion invalida\n");
                    break;
            }
            break;
        case 4:
            printf("Regresando al menu principal...\n");
            return;

        default:
            printf("Opcion invalida\n");
            break;
    }
    } while (opcion != 4);
}

