#ifndef VENTASF_H
#define VENTASF_H

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "clientes.h"
#include "producto.h"




typedef struct ventasData{
    int folio;
    long ID_Cliente;
    int ID_Producto;
    int cantidadC;
    float precio;
    char fecha[11];
} dataV;

typedef struct infoparaImpresion{
    char fecha[11];
    int folio;
    char cliente[100];
    float subtotal;
    float descuento;
    float iva;
    float total;
}infoI;


void obtener_fecha(char *, size_t);

int menuF(void);

int menuReporte(void);

int consultaClav2(cliente *);

int consultaClavP(Producto *);

void procesoVenta(dataV *, infoI *);

void procesoTicket(void);

void ventasDiaActual(void);

#endif