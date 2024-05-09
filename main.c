#include <stdio.h>
#include <string.h>
#include "clientes.c"
#include "VentasF.c"
#include "Usuarios.c"
#include "login.c"

int main(){
    //Apartado para crear la carpera de usuarios y el archivo con los datos respectivos del usuario1____________________________________________________________________
    //Comprobacion y creacion de la carpeta "usuarios"
	const char *carpeta = "usuarios";
	struct stat st;
	if(stat(carpeta, &st) == -1)
	{
		mkdir(carpeta);
	}
	
	
	//Comprobacion y creacion del archivo "UsuariosData"
	const char *nombreArchivo = "usuarios/usuariosData.bin";

	if (stat(nombreArchivo, &st) == 0) {
        // El archivo ya existe
    } else {
        // El archivo no existe, crearlo
        FILE *archivo = fopen(nombreArchivo, "wb");
        
        //Poner al usuario Principal
        dataU usP;
        usP.id= 1;
		strcpy(usP.nombre,"Guillermo");
		strcpy(usP.apellidoP,"Bautista");
		strcpy(usP.apellidoM,"Hernandez");
		strcpy(usP.correo, "guille@gmail.com");
		strcpy(usP.nombreUsuario,"GuillermoBta");
		strcpy(usP.contra,"Guille123");
		strcpy(usP.direccion.calle,"Calle 1");
		strcpy(usP.direccion.colonia,"Colonia 1");
		usP.direccion.numero = 1083;
		usP.direccion.cp = 64280;
		usP.status = 1;
		usP.tipoUsuario = 0;

		fwrite(&usP,sizeof(dataU), 1, archivo);
		fclose(archivo);
    }
//___________________________________________________________________________________________________________________________________
    if (!loginUsuario()){
    // Apartado de ventas (Esto va dentro de un switch op 3)______________________________________________________________
        /*int op;
        op = menuVentas(); // Tomara la opcion que eligio el usuario del Primer menu de ventas
        switch (op)
        {
            case 1:
            break;
            case 2:
                op = menuReporte();
                if(op == 1)
                {
                }
                else
                {
                }
            break;
            case 3:
            break;
            //____________________________________________________________________________________________________________
        }
        */
    }    
    return 0;
}
