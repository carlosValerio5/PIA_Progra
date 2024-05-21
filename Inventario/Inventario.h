#ifndef _INVENTARIO_H
#define _INVENTARIO_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Definicion de la estructura de producto
struct Producto {
    int clave;
    char nombre[50];
    char descripcion[100];
    int tipo_producto;
    int cantidad;
    int stock;
    float precio;
    int estatus;
};
// Definicion de la estructura de fecha
typedef struct {
    int dia;
    int mes;
    int anio;
} Fecha;
// Definicion de la estructura de reporte de reabastecimiento
struct Reabastecimiento {
    int folio;
    int clave;
    Fecha fecha;
    int cantidadPedida;
};


// Declaracion de funciones
// Funcion principal
void InventarioMenu();
// Subfunciones
void Reabastecimiento();
void reporteInventarioCompleto(char *archivo);
void reporteCompleto(char *archivo);
void reportePorTipoProducto(char *archivo);
void reporteNivelStock(char *archivo);
void reporteReabastecimiento();
// Herramientas
char* obtenerCategoria(int numero);
Fecha obtenerFechaActual();
void actualizarFolio(int nuevoFolio);
int obtenerFolioActual();
int compararFechas(Fecha fecha1, Fecha fecha2);

#endif