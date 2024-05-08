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


void nuevoU()//Funcion para agregar un nuevo usuario (Probablemente se puede modificar para que no reciba nada )
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
    	usuarioF.apellidoP[strcspn(usuarioF.apellidoP, "\n")] = '\0'; // Eliminar el salto de línea

    	printf("\nApellido Materno: ");
    	fflush(stdin);
    	fgets(usuarioF.apellidoM, 20, stdin);
    	usuarioF.apellidoM[strcspn(usuarioF.apellidoM, "\n")] = '\0'; // Eliminar el salto de línea

    	printf("\nCorreo: ");
    	fflush(stdin);
    	fgets(usuarioF.correo, 60, stdin);
    	usuarioF.correo[strcspn(usuarioF.correo, "\n")] = '\0'; // Eliminar el salto de línea

    	printf("\nNombre de Usuario: ");
    	fflush(stdin);
    	fgets(usuarioF.nombreUsuario, 30, stdin);
    	usuarioF.nombreUsuario[strcspn(usuarioF.nombreUsuario, "\n")] = '\0'; // Eliminar el salto de línea

    	printf("\nContraseña: ");
    	fflush(stdin);
    	fgets(usuarioF.contra, 20, stdin);
    	usuarioF.contra[strcspn(usuarioF.contra, "\n")] = '\0'; // Eliminar el salto de línea

    	printf("\nCalle: ");
    	fflush(stdin);
    	fgets(usuarioF.direccion.calle, 20, stdin);
    	usuarioF.direccion.calle[strcspn(usuarioF.direccion.calle, "\n")] = '\0'; // Eliminar el salto de línea

    	printf("\nColonia: ");
    	fflush(stdin);
    	fgets(usuarioF.direccion.colonia, 20, stdin);
    	usuarioF.direccion.colonia[strcspn(usuarioF.direccion.colonia, "\n")] = '\0'; // Eliminar el salto de línea

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
    	FILE *archivo = fopen("usuarios/usuariosData.bin", "rb");
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
}


void EliminarUsuario(void) //Funcion para eliminar el usuario
{
	system("cls");
	fflush(stdin);
	
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
		//fread(&usuario, sizeof(dataU), 1, archivo);
		printf("\n%s \n%s", nombre, usuario.nombre);
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
	}else{
		FILE *archivos = fopen("usuarios/usuariosData.bin","rb+");
		rewind(archivos);
		while(!feof(archivo))
		{
			fread(&usuario, sizeof(dataU), 1, archivo);
			//Cambio de status
			if(strcmp(nombre,usuario.nombre) ==0 )
			{
				usuario.status = 0;//Cambiamos el status
				fseek(archivos, -(long)sizeof(dataU),SEEK_CUR);//Nos movemos a la posicion de la estructura a modificar
				fwrite(&usuario, sizeof(dataU), 1, archivos);
				break;
				
			}
		}
		fclose(archivos);
	}
}




