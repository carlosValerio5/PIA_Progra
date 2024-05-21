#ifndef USUARIOS_H
#define USUARIOS_H

#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <sys/stat.h>
#include <sys/types.h>

struct datosDireccion {
    char calle[40];
    char colonia[40];
    int numero;
    int cp;
};

typedef struct datos_Usuarios{
    int id;
    char nombre[20];
    char apellidoP[20];
    char apellidoM[20];
    char correo[60];
    char nombreUsuario[30];
    char contra[20];
    struct datosDireccion direccion;
    int status;
    int tipoUsuario;
}dataU;


int menuUsuarios(void);
void nuevoU(void);
void EliminarUsuario(void);
void reactivarUsuario(void);

#endif
