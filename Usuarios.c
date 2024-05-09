#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include<windows.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "usuarios.h"

//Borre las estructuras al principio de este archivo porque ya estan definidas dentro de el archivo usuarios.h

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
		Sleep(1);
	}while(op <=0 || op >4);
	return op;
	
}


void nuevoU()
{
    	setlocale(LC_ALL, "spanish");
    	system("cls");
	dataU usuarioF;
    // Limpiar el búfer de entrada antes de la primera lectura
	fflush(stdin);
	
	printf("\nIngrese los datos solicitados \nNombre: ");
    	fgets(usuarioF.nombre, 20, stdin);
    	usuarioF.nombre[strcspn(usuarioF.nombre, "\n")] = '\0'; // Eliminar el salto de línea

    	printf("\nApellido Paterno: ");
    	fflush(stdin);
    	fgets(usuarioF.apellidoP, 20, stdin);
    	usuarioF.apellidoP[strcspn(usuarioF.apellidoP, "\n")] = '\0';

    	printf("\nApellido Materno: ");
    	fflush(stdin);
    	fgets(usuarioF.apellidoM, 20, stdin);
    	usuarioF.apellidoM[strcspn(usuarioF.apellidoM, "\n")] = '\0'; 

    	printf("\nCorreo: ");
    	fflush(stdin);
    	fgets(usuarioF.correo, 60, stdin);
    	usuarioF.correo[strcspn(usuarioF.correo, "\n")] = '\0';

    	printf("\nNombre de Usuario: ");
    	fflush(stdin);
    	fgets(usuarioF.nombreUsuario, 30, stdin);
    	usuarioF.nombreUsuario[strcspn(usuarioF.nombreUsuario, "\n")] = '\0'; 

    	printf("\nContraseña: ");
    	fflush(stdin);
    	fgets(usuarioF.contra, 20, stdin);
    	usuarioF.contra[strcspn(usuarioF.contra, "\n")] = '\0'; 

    	printf("\nCalle: ");
    	fflush(stdin);
    	fgets(usuarioF.direccion.calle, 20, stdin);
    	usuarioF.direccion.calle[strcspn(usuarioF.direccion.calle, "\n")] = '\0';

    	printf("\nColonia: ");
    	fflush(stdin);
    	fgets(usuarioF.direccion.colonia, 20, stdin);
    	usuarioF.direccion.colonia[strcspn(usuarioF.direccion.colonia, "\n")] = '\0';

    	printf("\nNúmero: ");
    	fflush(stdin);
    	scanf("%d", &(usuarioF.direccion.numero));
    
    	printf("\nCodigo Postal: ");
    	fflush(stdin);
    	scanf("%d", &(usuarioF.direccion.cp));

    	printf("Ingresa el status (1 = Activo / 0 = Inactivo): ");
    	fflush(stdin);
    	scanf("%d", &(usuarioF.status));

    	printf("Ingresa el tipo de usuario (1 = Administrador / 0 = Ventas): ");
    	fflush(stdin);
    	scanf("%d", &(usuarioF.tipoUsuario));

    //Para sacar el ID tendremos que leer el archivo y ver cual ID fue el ultimo en ser agregado
    	FILE *archivo = fopen("./usuarios/usuariosData.bin", "rb");
    // Leer la estructura del archivo
    	dataU us;
    	while(!feof(archivo))
    	{
	        fread(&us, sizeof(dataU), 1, archivo);
	        if (feof(archivo)) 
		    {
            		break;
        	}
    	}
    	usuarioF.id = us.id + 1;
    	fclose(archivo);

    	//Ahora pondremos los datos en el archivo
    	FILE *archivos = fopen("usuarios/usuariosData.bin", "ab");
    	fwrite(&usuarioF, sizeof(dataU), 1, archivos);
    	fclose(archivos);
	printf("\nUsuario agregado");
	Sleep(2);
}


void EliminarUsuario(void) //Funcion para eliminar el usuario
{
	system("cls");
	fflush(stdin);
	int op;
	char nombre[20];
	printf("\nIngresa el nombre del usuario que desea eliminar: ");
	fgets(nombre, 20, stdin);
	nombre[strcspn(nombre, "\n")] = '\0';
	
	//buscamos si el nombre existe
	FILE *archivo = fopen("usuarios/usuariosData.bin","rb");
	dataU usuario;
	int i=0;
	
	while(fread(&usuario, sizeof(dataU), 1, archivo) ==1)
	{
		if(strcmp(nombre,usuario.nombre) ==0 )
		{
			i= 1;
		}
	}
	fclose(archivo);
	
	//Proceso cambio de status
	if(i==0)
	{
		printf("\nNo se encontro un usuario con ese nombre");
		printf("\nDesea realizar una nueva busqueda (Si = 0 / No = 1)? ");
		scanf("%d", &op);
		Sleep(1);
		if(op == 1){
			return;
		}else{
			EliminarUsuario();//LLamamos a la misma funcion para que vuelva hacer el proceseso
			return;
		}
	}else{
		FILE *archivos = fopen("usuarios/usuariosData.bin","rb+");
		rewind(archivos);
		while(!feof(archivo))
		{
			fread(&usuario, sizeof(dataU), 1, archivo);
			//Cambio de status
			if(strcmp(nombre,usuario.nombre) ==0  && usuario.status !=0)
			{
				printf("\nDesea dar de bajar al usuario (Si = 0 / No = 1)? ");
				scanf("%d", &op);
				if(op == 0){
					usuario.status = 0;//Cambiamos el status
					fseek(archivos, -(long)sizeof(dataU),SEEK_CUR);//Nos movemos a la posicion de la estructura a modificar
					fwrite(&usuario, sizeof(dataU), 1, archivos);
					printf("\nUsuario eliminado con exito");
					Sleep(1);
					break;
				}else{
					break;
				}
				
			}else if(strcmp(nombre,usuario.nombre) ==0  && usuario.status ==0){
				printf("\nEl usuario ya esta dado de baja");
				Sleep(1);
				break;
			}
		}
		fclose(archivos);
	}
}



void reactivarUsuario(void) //Funcion para activar el usuario (SIMILAR A ELIMINAR)
{
	system("cls");
	fflush(stdin);
	int op;
	char nombre[20];
	printf("\nIngresa el nombre del usuario que desea actualizar: ");
	fgets(nombre, 20, stdin);
	nombre[strcspn(nombre, "\n")] = '\0';
	
	//buscamos si el nombre existe
	FILE *archivo = fopen("usuarios/usuariosData.bin","rb");
	dataU usuario;
	int i=0;
	
	while(fread(&usuario, sizeof(dataU), 1, archivo) ==1)
	{

		if(strcmp(nombre,usuario.nombre) ==0 )
		{
			i= 1;
		}
	}
	fclose(archivo);
	
	//Proceso cambio de status
	if(i==0)
	{
		printf("\nNo se encontro un usuario con ese nombre");
		printf("\nDesea realizar una nueva busqueda (Si = 0 / No = 1)? ");
		scanf("%d", &op);
		Sleep(1);
		if(op == 1){
			return;
		}else{
			reactivarUsuario();
			return;
		}
	}else{
		FILE *archivos = fopen("usuarios/usuariosData.bin","rb+");
		rewind(archivos);
		while(!feof(archivo))
		{
			fread(&usuario, sizeof(dataU), 1, archivo);
			//Cambio de status
			if(strcmp(nombre,usuario.nombre) ==0  && usuario.status !=1)
			{
				printf("\nDesea dar de activar el usuario (Si = 0 / No = 1)? ");
				scanf("%d", &op);
				if(op == 0){
					usuario.status = 1;//Cambiamos el status
					fseek(archivos, -(long)sizeof(dataU),SEEK_CUR);//Nos movemos a la posicion de la estructura a modificar
					fwrite(&usuario, sizeof(dataU), 1, archivos);
					printf("\nUsuario actualizado con exito");
					Sleep(1);
					break;
				}else{
					break;
				}
				
			}else if(strcmp(nombre,usuario.nombre) ==0  && usuario.status ==1){
				printf("\nEl usuario no esta dado de baja");
				Sleep(1);
				break;
			}
		}
		fclose(archivos);
	}
}


