#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>


//Estructuras que se van a utilizar
struct datosDireccion{
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


int menuUsuarios(void)
{
	setlocale(LC_ALL, "spanish");
	int op;
	system("cls");
	do
	{
		
		printf("\n\t1.- Nuevo Usuario\n\t2.- Eliminar Usuario\n\t3.- Activar Usuario\n\t4.- Regresar Menú Principal");
		printf("\nSeleccione una opción: ");
		scanf("%d", &op);
	}while(op <=0 || op >4);
	return op;
	
}
