#ifndef _INVENTARIOJ_H_
#define _INVENTARIOJ_H_

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

// Definicion de la estructura de fecha
typedef struct {
    int dia;
    int mes;
    int anio;
} Fecha;
// Definición de la estructura para los productos
typedef struct {
    int clave;
    char descripcion[100];
    int tipo;
    int cantidadExistencia;
    int nivelStock;
    float precio;
    int estatus;
} Producto;
// Definicion de la estructura de folios de Reabastecimiento
typedef struct {
    int claveProducto;
    char producto[100];
    Fecha fecha;
    int folioPedido;
    int cantidadPedida;
    int nivelStock;
} Reabastecimient;


// Declaracion de funciones
int menuInventario(void);
void reabastecerProducto();

void reporteInventarioCompleto();

void reporteCompleto();

void reportePorTipoProducto();

void reporteNivelStock();

char* obtenerCategoria(int numero);

void Reabastecimiento();

void pedirFecha(Fecha *fecha);

int compararFechas(Fecha fecha1, Fecha fecha2);