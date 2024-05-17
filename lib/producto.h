#ifndef PRODUCTO_H
#define PRODUCTO_H

#include <stdio.h>
#include <stdlib.h>


// Estructura para los productos
typedef struct {
    int clave;
    char nombre[50];
    char descripcion[100];
    int tipo_producto;
    int cantidad;
    int stock;
    float precio;
    int estatus;
} Producto;

// Enum para tipos de producto
typedef enum {
    ESCRITORIOS = 1,
    ARCHIVEROS,
    SILLAS,
    MESAS_CENTRO,
    MESAS_EJECUTIVAS,
    SOFAS
} TipoProducto;

// Declaraciones de funciones
void guardar_producto(Producto *producto);
int leer_productos(Producto *productos, int max_productos);
int verificar_clave(int clave);
void obtener_fecha2(char *fecha_strF, size_t max_len);


void mostrar_menu();

void nuevo_producto();
void editar_producto();
void consulta_tipo();
void consulta_clave(int );

#endif
