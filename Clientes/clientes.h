#ifndef _CLIENTES_H
#define _CLIENTES_H
typedef struct{
    char calle[50];
    char colonia[50];
    int numero;
    int cp;
}direccion;
typedef struct {
    long ID;
    char nombre[100];
    char apellidoP[50];
    char apellidoM[50];
    char RFC[11];
    char correo[50];
    unsigned long telefono;
    float descuento;
    direccion dir;
    int estatus;
}cliente;

int nuevoCliente(void);

int leerArchivo(void);

int compararRFC(cliente *, FILE *);

int generarID(cliente*, FILE*);

int limpiarArchivo(void);

int eliminarCliente(void);

void *consultaNom(void *);

void consultaClav(cliente *);

void menuClientes(void);

void submenuClientes(void);

void clienteThread(void);

#endif