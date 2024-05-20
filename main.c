#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "clientes.c"
#include "VentasF.c"
#include "Usuarios.c"
#include "login.c"
#include "producto.c"
#include "Inventario.c"

void menuPrincipaladmin(){
	printf("\nComercializadora Fuentes");
	printf("\n1. Clientes");
	printf("\n2. Productos");
	printf("\n3. Ventas");
	printf("\n4. Inventario");
	printf("\n5. Usuarios");
	printf("\n6. Salir");
	printf("\nSeleccione una opcion: ");
    return;
}

void menuPrincipalventas(){
	printf("\nComcercializadora Fuentes");
	printf("\n1. Clientes");
	printf("\n2. Ventas");
	printf("\n3. Salir");
	printf("\nSeleccione una opcion: ");
	return;
}

int main(){
    //Apartado para crear la carpera de usuarios y el archivo con los datos respectivos del usuario1
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
		usP.tipoUsuario = 1;

		fwrite(&usP,sizeof(dataU), 1, archivo);
		fclose(archivo);
    }
//___________________________________________________________________________________________________________________________________
//Menu Principal
    dataU comparar;
    if (!loginUsuario(&comparar)){
        int opcionPrincipal = 0;
		if (comparar.tipoUsuario == 1){
	        while(opcionPrincipal!= 6){
		    	system("cls");
	            menuPrincipaladmin();
                scanf("%d", &opcionPrincipal);
	        	while(opcionPrincipal<0||opcionPrincipal>6){
	        		printf("\nOpcion Invalida, ingrese un numero entre 1 y 6");
	        		Sleep(2000);
	        		system("cls");
	        		menuPrincipaladmin();
	        		scanf("%d", &opcionPrincipal);
	        	}
		    	system("cls");
	        	if(opcionPrincipal == 1){
	        		//Clientes
		    		int opcClientes = 0, opcsubClientes = 0;
		    		while (opcClientes != 4){
		    			menuClientes();
		    			printf("\nIngrese una opcion: ");
		    			scanf("%d", &opcClientes);
		    			while(opcClientes>4 || opcClientes <0){
		    				printf("\nOpcion Invalida, ingrese un numero entre 1 y 4");
		    				Sleep(2000);
		    				system("cls");
		    				menuClientes();
		    				printf("\nIngrese una opcion: ");
		    				scanf("%d", &opcClientes);
		    			}
		    			if (opcClientes==1){
		    				if (nuevoCliente()!= 0){
		    					printf("Error al crear nuevo cliente");
		    				}
		    			}
		    			else if (opcClientes== 2){
		    				if (eliminarCliente()!= 0){
		    					printf("Error al eliminar cliente");
		    				}
		    			}
		    			else if (opcClientes == 3){
		    				submenuClientes();
		    				printf("\nIngrese una opcion: ");
		    				scanf("%d", &opcsubClientes);
		    				while (opcsubClientes <0 || opcsubClientes >2)
		    				{
		    					printf("\nOpcion Invalida, ingrese un numero entre 1 y 2");
		    				    Sleep(2000);
		    				    system("cls");
		    				    submenuClientes();
		    					printf("\nIngrese una opcion: ");
		    				    scanf("%d", &opcsubClientes);
		    				}
		    				if (opcsubClientes == 1){
		    					clienteThread();
		    					printf("\nPresione enter para continuar...");
		    					fflush(stdin);
		    					getchar();
		    				}
		    				else if(opcsubClientes == 2){
		    					cliente vacio;
		    					consultaClav(&vacio);
		    					printf("\nPresione enter para continuar...");
		    					fflush(stdin);
		    					getchar();
		    				}
		    			}
		    		}
	        	}
	        	else if (opcionPrincipal == 2){
	        		//Producto
		    		mostrar_menu();
	        	}
	        	else if (opcionPrincipal == 3){
        
                    // Apartado de ventas (Esto va dentro de un switch op 3)______________________________________________________________
                    int opVentas;
					int op;
                    do{
	                    opVentas = menuF(); // Tomara la opcion que eligio el usuario del Primer menu de ventas
	                    switch (opVentas)
	                    {
	                        case 1:
	                        	//nuevoCliente();
	                    		//nuevo_producto();
	                            procesoTicket();
	                        break;
	                        case 2:
	                            op = menuReporte();
	                            if(op == 1)
	                            {
	                            	mostrarDatosDeHoy();
	                            }
	                            else
	                            {
	                            	mostrarVentasPorCliente();
	                            }
	                        break;
	                        case 3:
	                        break;
	                        //____________________________________________________________________________________________________________
	                    }
            	    }while(opVentas!=3);
	        	}
		    	else if (opcionPrincipal == 4){
		    		//Inventario
		    	}
		    	else if(opcionPrincipal == 5){
		    		//Usuarios
		    		int opcUsuarios;
		    		do {
		    		    opcUsuarios = menuUsuarios();
		    		    if (opcUsuarios == 1){
		    		    	nuevoU();
		    		    }
		    		    else if(opcUsuarios == 2){
		    		    	EliminarUsuario();
		    		    }
		    		    else if(opcUsuarios == 3){
		    		    	reactivarUsuario();
		    		    }
		    		}while(opcUsuarios!=4);
		    	}
	        }
		}
		else if(comparar.tipoUsuario == 0){
			while(opcionPrincipal != 3){
                system("cls");
	            menuPrincipalventas();
                scanf("%d", &opcionPrincipal);
	        	while(opcionPrincipal<0||opcionPrincipal>6){
	        		printf("\nOpcion Invalida, ingrese un numero entre 1 y 6");
	        		Sleep(2000);
	        		system("cls");
	        		menuPrincipalventas();
	        		scanf("%d", &opcionPrincipal);
	        	}
		    	system("cls");
                if(opcionPrincipal == 1){
	        		//Clientes
		    		int opcClientes = 0, opcsubClientes = 0;
		    		while (opcClientes != 4){
		    			menuClientes();
		    			printf("\nIngrese una opcion: ");
		    			scanf("%d", &opcClientes);
		    			while(opcClientes>4 || opcClientes <0){
		    				printf("\nOpcion Invalida, ingrese un numero entre 1 y 4");
		    				Sleep(2000);
		    				system("cls");
		    				menuClientes();
		    				printf("\nIngrese una opcion: ");
		    				scanf("%d", &opcClientes);
		    			}
		    			if (opcClientes==1){
		    				if (nuevoCliente()!= 0){
		    					printf("Error al crear nuevo cliente");
		    				}
		    			}
		    			else if (opcClientes== 2){
		    				if (eliminarCliente()!= 0){
		    					printf("Error al eliminar cliente");
		    				}
		    			}
		    			else if (opcClientes == 3){
		    				submenuClientes();
		    				printf("\nIngrese una opcion: ");
		    				scanf("%d", &opcsubClientes);
		    				while (opcsubClientes <0 || opcsubClientes >2)
		    				{
		    					printf("\nOpcion Invalida, ingrese un numero entre 1 y 2");
		    				    Sleep(2000);
		    				    system("cls");
		    				    submenuClientes();
		    					printf("\nIngrese una opcion: ");
		    				    scanf("%d", &opcsubClientes);
		    				}
		    				if (opcsubClientes == 1){
		    					clienteThread();
		    					printf("\nPresione enter para continuar...");
		    					fflush(stdin);
		    					getchar();
		    				}
		    				else if(opcsubClientes == 2){
		    					cliente vacio;
		    					consultaClav(&vacio);
		    					printf("\nPresione enter para continuar...");
		    					fflush(stdin);
		    					getchar();
		    				}
		    			}
		    		}
	        	}
	        	else if (opcionPrincipal == 2){
        
                    // Apartado de ventas (Esto va dentro de un switch op 3)______________________________________________________________
                    int opVentas, op;
                    do{
	                    opVentas = menuF(); // Tomara la opcion que eligio el usuario del Primer menu de ventas
	                    switch (opVentas)
	                    {
	                        case 1:
	                        	//nuevoCliente();
	                    		//nuevo_producto();
	                            procesoTicket();
	                        break;
	                        case 2:
	                            op = menuReporte();
	                            if(op == 1)
	                            {
	                            	mostrarDatosDeHoy();
	                            }
	                            else
	                            {
	                            	mostrarVentasPorCliente();
	                            }
	                        break;
	                        case 3:
	                        break;
	                        //____________________________________________________________________________________________________________
	                    }
            	    }while(opVentas!=3);
	        	}
			}
		}
    } 

    return 0;
}
