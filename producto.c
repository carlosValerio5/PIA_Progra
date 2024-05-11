#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // Para sleep
#include "producto.h"

//funcion para guardar productos
void guardar_producto(Producto *producto) {
    FILE *file = fopen("./bin/productos.bin", "ab");//modo para añadir
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
//funcion para actualizar productos
void actualizar_producto(Producto *productos, int total) {
    FILE *file = fopen("./bin/productos.bin", "wb"); //modo para sobreescribir
    if (file != NULL) {
        fwrite(productos, sizeof(Producto), total, file);
        fclose(file);
    } else {
        printf("Error al abrir el archivo para actualizar.\n");
    }
}

//funcion para verificar que la clave sea unica
int verificar_clave(int clave) {
    Producto productos[100];
    int total = leer_productos(productos, 100); //lee productos del archiv
    int i;
    for (i = 0; i < total; i++) {
        if (productos[i].clave == clave) {
            return 0;
        }
    }
    return 1;
}
//funcion para nuevo producto: debe incluir clave unica, descripcion, tipo de producto, cantidad en existencia, nivel de stock(cantidad minima que debe tener en almacen), precio y estatus
void nuevo_producto(){
    Producto producto;
    int tipo, cantidad, stock;
    float precio;

    while(1){
        printf("Ingrese clave del producto (unica): ");
        scanf("%d", &producto.clave);
        fflush(stdin);

        if(verificar_clave(producto.clave)){
            break;
        } else {
            printf("Clave en uso. Intente de nuevo");
        }
    }

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
    printf("Producto agregado con exito\n");
    sleep(2);

    mostrar_menu();
}
//funcion para editar producto: capturar clave unica de productgo,
void editar_producto() {
    Producto productos[100]; // Máximo 100 productos
    int total = leer_productos(productos, 100);
    int clave, producto_encontrado = -1;
    int opcion;
    float precio, nuevo_precio;

    if (total == 0) {
        printf("No hay productos en el archivo\n");
        sleep(2);
        return;
    }

    while (producto_encontrado == -1) { //bucle para intentar hasta que se encuentre el producto o el usuario quiera salir
        printf("Ingrese clave del producto a editar: ");
        scanf("%d", &clave);

        for (int i = 0; i < total; i++) {
            Producto *producto = &productos[i];
            if (producto->clave == clave) {
                producto_encontrado = i;
                break;
            }
        }

        if (producto_encontrado == -1) {
            printf("Producto no encontrado\n");
            printf("¿Desea intentar de nuevo? (s/n): ");
            char respuesta;
            fflush(stdin);
            scanf("%c", &respuesta);

            if (respuesta != 's' && respuesta != 'S') {
                printf("Regresando al menu principal.\n");
                return;
            }
        }
    }


    Producto *producto = &productos[producto_encontrado];

    printf("Producto encontrado: %s\n", producto->nombre);
    printf("Seleccione una opcion para editar:\n");
    printf("1. Cambiar precio\n");
    printf("2. Cambiar nombre del producto\n");

    scanf("%d", &opcion);
    switch (opcion){
        case 1:
            printf("Ingrese nuevo precio: ");
            scanf("%f", &precio);
            producto->precio = nuevo_precio;
            printf("Precio actualizado a: %.2f\n", precio);
            sleep(2);
            mostrar_menu();
            break;
        case 2:
            fflush(stdin);
            printf("Ingrese nuevo nombre del producto: ");
            fgets(producto->nombre, 50, stdin);
            size_t len = strlen(producto->nombre);
            if (producto->nombre[len - 1] == '\n') {
                producto->nombre[len - 1] == '\0';
            }
            printf("Nombre actualizado a: %s\n", producto->nombre);
            sleep(2);
            mostrar_menu();
            break;
        default:
            printf("Opcion invalida\n");
            break;
    }
    actualizar_producto(productos, total);
    printf("Producto actualizado con exito\n");
    sleep(3);
}



//funcion para consulta por tipo
void consulta_tipo() {
    Producto productos[100];
    int total = leer_productos(productos, 100);

    if (total == 0) {
        printf("No hay productos en el archivo\n");
        sleep(2);
        return;
    }

    int tipo;
    printf("Ingrese tipo de producto \n1: Escritorios\n2: Archiveros\n3: Sillas\n4: Mesas de centro\n5: Mesas ejecutivas\n6: Sofas\n");
    scanf("%d", &tipo);

    system("cls");
    printf("Productos del tipo %d:\n", tipo);
    for (int i = 0; i < total; i++) {
        Producto *producto = &productos[i]; // Usando ->
        if (producto->tipo_producto == tipo && producto->estatus == 1) {
            printf("Clave: %d, Nombre: %s, Descripcion: %s, Cantidad: %d, Precio: %.2f\n", producto->clave, producto->nombre, producto->descripcion, producto->cantidad, producto->precio);
        }
    }
    sleep(2);
    mostrar_menu();
}

//funcion para consulta por clavep
void consulta_clave() {
    Producto productos[100];
    int total = leer_productos(productos, 100);
    int clave;

    if (total == 0) {
        printf("No hay productos en el archivo.\n");
        sleep(2);
        return;
    }

    printf("Ingrese clave del producto: ");
    scanf("%d", &clave);

    system("cls");
    int producto_encontrado = -1;
    for (int i = 0; i < total; i++) {
        Producto *producto = &productos[i];
        if (producto->clave == clave && producto->estatus == 1) {
            producto_encontrado = i;
            break;
        }
    }

    if (producto_encontrado == -1) {
        printf("Producto no encontrado.\n");
        sleep(2);
        return;
    }

    Producto *producto = &productos[producto_encontrado];
    printf("Clave: %d, Nombre: %s, Descripcion: %s, Cantidad: %d, Precio: %.2f\n", producto->clave, producto->nombre, producto->descripcion, producto->cantidad, producto->precio);
    sleep(2);
    mostrar_menu();
}

void mostrar_menu() {
    system("cls");
    printf("Menu Producto\n");
    printf("1. Nuevo Producto\n");
    printf("2. Editar Producto\n");
    printf("3. Consultar producto\n");
    printf("4. Regresar al menu principal\n");

    int opcion;
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
                    consulta_tipo(); // Consulta por tipo de producto
                    break;
                case 2:
                    consulta_clave(); // Consulta por clave de producto
                    break;
                case 3:
                    break; //regrese a los
                default:
                    printf("Opcion invalida.\n");
                    mostrar_menu();
                    break;
            }
            break;
        case 4:
            printf("Regresando al menu principal...\n");
            return;

        default:
            printf("Opcion invalida.\n");
            mostrar_menu();
            break;
    }
}

int main() {
    mostrar_menu(); // Llama al menú principal al inicio del programa
    return 0; // Devuelve 0 para indicar éxito
}
