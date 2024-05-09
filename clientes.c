#include <stdio.h>
#include "./lib/clientes.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h> //libreria para el Sleep

#include "./lib/VentasF.h" //Para obtener fecha


//Sirve para registrar a un nuevo cliente
int nuevoCliente(void){
    cliente nuevoCliente;
    char *newline;

    //Archivo donde se almacenan los datos de los clientes
    FILE *archClientes;
    if((archClientes = fopen("./bin/clientes.bin", "rb+"))==NULL){
        printf("\nError al abrir el archivo de clientes, abortando...");
        //Valor de retorno para un error
        return 1;
    }

    //Captura de datos

    //Se marca como activo al usuario
    nuevoCliente.estatus = 1;

    if(generarID(&nuevoCliente, archClientes)!=0){
        printf("Error al generar id.");
        return 1;
    }
    printf("\nIngrese nombre: ");
    fflush(stdin);
    fgets(nuevoCliente.nombre, sizeof(nuevoCliente.nombre), stdin);
    //Eliminar new line character
    newline = strstr(nuevoCliente.nombre, "\n");
    if(newline != NULL)
        strncpy(newline, "\0", 1);
    newline = NULL;
    printf("\nIngrese apellido Paterno: ");
    fflush(stdin);
    fgets(nuevoCliente.apellidoP, sizeof(nuevoCliente.apellidoP), stdin);
    newline = strstr(nuevoCliente.apellidoP, "\n");
    if(newline != NULL)
        strncpy(newline, "\0", 1);
    newline = NULL;
    printf("\nIngrese apellido Materno: ");
    fflush(stdin);
    fgets(nuevoCliente.apellidoM, sizeof(nuevoCliente.apellidoM), stdin);
    newline = strstr(nuevoCliente.apellidoM, "\n");
    if(newline != NULL)
        strncpy(newline, "\0", 1);
    newline = NULL;
    //RFC
    printf("Ingrese RFC: ");
    fflush(stdin);
    fgets(nuevoCliente.RFC, 11, stdin);
    newline = strstr(nuevoCliente.RFC, "\n");
    if(newline != NULL)
        strncpy(newline, "\0", 1);
    newline = NULL;
    compararRFC(&nuevoCliente, archClientes);

    //Correo electronico
    printf("\nIngrese su correo electronico: ");
    fflush(stdin);
    fgets(nuevoCliente.correo, sizeof(nuevoCliente.correo), stdin);
    newline = strstr(nuevoCliente.correo, "\n");
    if(newline != NULL)
        strncpy(newline, "\0", 1);
    newline = NULL;
    //numero de telefono
    printf("\nIngrese su numero de telefono: ");
    fflush(stdin);
    scanf("%d", &nuevoCliente.telefono);

    //Descuento
    printf("\nIngrese el descuento a aplicar [0.15-0.0]: ");
    fflush(stdin);
    scanf("%f", &nuevoCliente.descuento);
    while(nuevoCliente.descuento>(float)(0.15) || nuevoCliente.descuento<0){
        printf("Valor de descuento invalido");
        printf("\nIngrese un valor entre .15 y 0: ");
        scanf(" %f", &nuevoCliente.descuento);
    }

    //Direccion
    printf("\nIngrese calle: ");
    fflush(stdin);
    fgets(nuevoCliente.dir.calle, sizeof(nuevoCliente.dir.calle), stdin);
    newline = strstr(nuevoCliente.dir.calle, "\n");
    if(newline != NULL)
        strncpy(newline, "\0", 1);
    newline = NULL;

    printf("\nIngrese su colonia: ");
    fflush(stdin);
    fgets(nuevoCliente.dir.colonia, sizeof(nuevoCliente.dir.colonia), stdin);
    newline = strstr(nuevoCliente.dir.colonia, "\n");
    if(newline != NULL)
        strncpy(newline, "\0", 1);
    newline = NULL;

    printf("\nIngrese el numero de domicilio: ");
    fflush(stdin);
    scanf("%d", &nuevoCliente.dir.numero);

    printf("\nIngrese codigo postal: ");
    fflush(stdin);
    scanf("%d", &nuevoCliente.dir.cp);

    //Escribe la estructura al archivo clientes.bin
    if(fwrite(&nuevoCliente, sizeof(cliente), 1, archClientes) != 1){
        printf("Error al escribir al archivo");
    }
    if(fclose(archClientes)==EOF){
        printf("Error al cerrar el archivo");
    }
    return 0;
}

int leerArchivo(void){
    cliente clienteImprimir;
    FILE *archCliente;
    //Esta parte de la funcion puede cambiar para hacerlo compatible con linux
    if ((archCliente = fopen("./bin/clientes.bin", "rb"))== NULL){
        printf("\nError al abrir el archivo, abortando...");
        return 1;
    }
    while(fread(&clienteImprimir, sizeof(cliente), 1, archCliente)!=0){
        if(clienteImprimir.estatus!=0){
            printf("\n");
            printf("ID: %d\n", clienteImprimir.ID);
            printf("Nombre: %s\n", clienteImprimir.nombre);
            printf("Apellido Paterno: %s\n", clienteImprimir.apellidoP);
            printf("Apellido Materno: %s\n", clienteImprimir.apellidoM);
            printf("RFC: %s\n", clienteImprimir.RFC);
            printf("Correo electronico: %s\n", clienteImprimir.correo);
            printf("Telefono: %d\n", clienteImprimir.telefono);
            printf("Descuento: %f\n", clienteImprimir.descuento);
            printf("Calle: %s\n", clienteImprimir.dir.calle);
            printf("Colonia: %s\n", clienteImprimir.dir.colonia);
            printf("Numero de domicilio: %d\n", clienteImprimir.dir.numero);
            printf("Codigo Postal: %d\n", clienteImprimir.dir.cp);
            printf("Estatus: %d\n", clienteImprimir.estatus); //Impresion para probar funcion eliminar clientes
        }
    }
    if (fclose(archCliente) == EOF){
        printf("Error al cerrar archivo");
    }
    return 0;
}


//Funcion para comparar que el RFC no se repita
int compararRFC(cliente *nuevoCliente, FILE *archivo){
    cliente clienteComp;
    rewind(archivo);
    while(fread(&clienteComp, sizeof(cliente), 1, archivo)!=0){
        while (strcmp(nuevoCliente->RFC, clienteComp.RFC)==0){
            printf("El RFC esta repetido.");
            printf("Ingrese otro RFC: ");
            fflush(stdin);
            fgets(nuevoCliente->RFC, sizeof(nuevoCliente->RFC), stdin);
            rewind(archivo);
        }
    }
    return 0;
}

int generarID(cliente *nuevoCliente, FILE *archivoClientes){
    long idviejo;
    cliente clienteViejo;
    if(fgetc(archivoClientes)==EOF){
        nuevoCliente->ID = 1;
        return 0;
    }
    rewind(archivoClientes);
    fseek(archivoClientes, -(long)(sizeof(cliente)), SEEK_END);
    if(fread(&clienteViejo, sizeof(cliente), 1, archivoClientes)!=1){
        printf("Error al leer");
        return -1;
    }
    nuevoCliente->ID = ++clienteViejo.ID;
    return 0;
}

int limpiarArchivo(void){
    FILE *archivo;
    if((archivo = fopen("./bin/clientes.bin", "wb"))==NULL){
        printf("Error al abrir el archivo.");
        return 1;
    }
    fclose(archivo);
    return 1;
}

int eliminarCliente(void){
    int id;
    char opc;
    FILE *archClientes;
    cliente clseleccionado;

    //Archivo abierto con permisos de lectura y escritura
    if ((archClientes=fopen("./bin/clientes.bin", "rb+"))==NULL){
        printf("\nError al abrir el archivo.");
        return 1;
    }

    printf("Ingrese el ID del cliente a eliminar: ");
    fflush(stdin);
    scanf("%d", &id);

    //ciclo sigue hasta que la funcion fread lea el final del archivo
    while(fread(&clseleccionado, sizeof(cliente), 1, archClientes)>0){
        if(clseleccionado.ID == id){
            if(clseleccionado.estatus == 0){
                printf("\nCliente ya esta dado de baja.");
                break;
            }
            else{
                clseleccionado.estatus = 0;

                //Regresa el puntero del archivo un espacio atras para sobreescribir
                if(fseek(archClientes, -(long)(sizeof(cliente)), SEEK_CUR) != 0){
                    printf("Error al actualizar los cambios, intente de nuevo...");
                    Sleep(3);
                    return 1;
                }
                //reescribe la informacion del cliente pero con el estatus actualizado
                if ((fwrite(&clseleccionado, sizeof(cliente), 1, archClientes))!=1){
                    printf("Error al actualizar los cambios, intente de nuevo...");
                    Sleep(3);
                    return 1;
                }
                else{
                    printf("\nCliente eliminado exitosamente.");
                    break;
                }
            }
        }
        else{
            printf("\nID no encontrado. Desea hacer otra busqueda? [s/n]");
            fflush(stdin);
            scanf("%c", &opc);
            while (tolower(opc) != 's' && tolower(opc) != 'n'){
                printf("Opcion invalida intente denuevo...");
                printf("\nDesea hacer otra busqueda? [s/n]");
                fflush(stdin);
                scanf("%c", &opc);
            }
            opc = tolower(opc);
            if (opc == 's'){
                rewind(archClientes);
                printf("Ingrese el ID del cliente a eliminar: ");
                fflush(stdin);
                scanf("%d", &id);
            }
            else{
                break;
            }
        }
    }
    fclose(archClientes);
}

//Consultas:
//Consulta de cliente por nombre

int consultaNom(void){
    FILE *archClientes;
    cliente coincidencia;
    char nomjunto[200];
    char *newline;
    char nom[50];
    int flag = 0;
    char fecha[11];

    obtener_fecha(fecha, sizeof(fecha));

    archClientes = fopen("./bin/clientes.bin", "rb");
    if (archClientes == NULL){
        printf("Error al abrir el archivo de clientes...");
        Sleep(3);
        return 1;
    }

    printf("\nIngrese el nombre del cliente a consultar: ");
    fflush(stdin);
    fgets(nom, sizeof(nom), stdin);
    //Eliminar \n del nombre
    newline = strstr(nom, "\n");
    if(newline != NULL)
        strncpy(newline, "\0", 1);
    while(fread(&coincidencia, sizeof(cliente), 1, archClientes)>0){
        //Reiniciar la variable nomjunto
        strcpy(nomjunto, "");

        //Juntamos el nombre del cliente en una sola direccion
        strcat(nomjunto, coincidencia.nombre);
        strcat(nomjunto, " ");
        strcat(nomjunto, coincidencia.apellidoP);
        strcat(nomjunto, " ");
        strcat(nomjunto, coincidencia.apellidoM);

        if((strcmp(coincidencia.nombre, nom))==0){
            printf("\n\t\t\tComercializadora\n\n\t\t\tConsulta Por Cliente");
            printf("\n%s", fecha);
            printf("\n\n\t\t\tPor Nombre");
            printf("\nNombre\t\t\tClave\tDireccion\t\tTelefono\tCorreo Electronico\tEstatus");
            printf("\n%s %s %s", coincidencia.nombre, coincidencia.apellidoP, coincidencia.apellidoM);
            printf("\t%d", coincidencia.ID);
            printf("\t%s %s %d %d", coincidencia.dir.colonia, coincidencia.dir.calle, coincidencia.dir.numero, coincidencia.dir.cp);
            printf("\t%d", coincidencia.telefono);
            printf("\t%s", coincidencia.correo);
            printf("\t\t%s", (coincidencia.estatus)==1?"Activo\0":"Inactivo\0");
            flag = 1;
        }
        else if ((strcmp(coincidencia.apellidoP, nom))==0){
            printf("\n\t\t\tComercializadora\n\n\t\t\tConsulta Por Cliente");
            printf("\n%s", fecha);
            printf("\n\n\t\t\tPor Nombre");
            printf("\nNombre\t\t\tClave\tDireccion\t\tTelefono\tCorreo Electronico\tEstatus");
            printf("\n%s %s %s", coincidencia.nombre, coincidencia.apellidoP, coincidencia.apellidoM);
            printf("\t%d", coincidencia.ID);
            printf("\t%s %s %d %d", coincidencia.dir.colonia, coincidencia.dir.calle, coincidencia.dir.numero, coincidencia.dir.cp);
            printf("\t%d", coincidencia.telefono);
            printf("\t%s", coincidencia.correo);
            printf("\t\t%s", (coincidencia.estatus)==1?"Activo\0":"Inactivo\0");
            flag = 1;
        }
        else if ((strcmp(coincidencia.apellidoP, nom))==0){
            printf("\n\t\t\tComercializadora\n\n\t\t\tConsulta Por Cliente");
            printf("\n%s", fecha);
            printf("\n\n\t\t\tPor Nombre");
            printf("\nNombre\t\t\tClave\tDireccion\t\tTelefono\tCorreo Electronico\tEstatus");
            printf("\n%s %s %s", coincidencia.nombre, coincidencia.apellidoP, coincidencia.apellidoM);
            printf("\t%d", coincidencia.ID);
            printf("\t%s %s %d %d", coincidencia.dir.colonia, coincidencia.dir.calle, coincidencia.dir.numero, coincidencia.dir.cp);
            printf("\t%d", coincidencia.telefono);
            printf("\t%s", coincidencia.correo);
            printf("\t\t%s", (coincidencia.estatus)==1?"Activo\0":"Inactivo\0");
            flag = 1;
        }
        else if ((strcmp(nomjunto, nom))==0){
            printf("\n\t\t\tComercializadora\n\n\t\t\tConsulta Por Cliente");
            printf("\n%s", fecha);
            printf("\n\n\t\t\tPor Nombre");
            printf("\nNombre\t\t\tClave\tDireccion\t\tTelefono\tCorreo Electronico\tEstatus");
            printf("\n%s %s %s", coincidencia.nombre, coincidencia.apellidoP, coincidencia.apellidoM);
            printf("\t%d", coincidencia.ID);
            printf("\t%s %s %d %d", coincidencia.dir.colonia, coincidencia.dir.calle, coincidencia.dir.numero, coincidencia.dir.cp);
            printf("\t%d", coincidencia.telefono);
            printf("\t%s", coincidencia.correo);
            printf("\t\t%s", (coincidencia.estatus)==1?"Activo\0":"Inactivo\0");
            flag =1;
        }
    }
    if (flag == 0){
        printf("\nNo se encontraron coincidencias.");
    }
    fclose(archClientes);
    return 0;
}

//Consulta Clave
//Debe proporcionarse el puntero a una estructura de tipo cliente
//esto debido a que es necesario para almacenar los resultados de la busqueda
void consultaClav(cliente *resultado){
    FILE *archClientes;
    int idbusqueda;
    char fecha[11];
    int flag = 0;

    archClientes = fopen("./bin/clientes.bin", "rb");
    if (archClientes == NULL){
        printf("\nError al abrir el archivo.");
        return;
    }

    printf("\nIngrese el ID del cliente a buscar: ");
    fflush(stdin);
    scanf("%d", &idbusqueda);

    obtener_fecha(fecha, sizeof(fecha));

    while(fread(resultado, sizeof(cliente), 1, archClientes)!= 0){
        if (idbusqueda == resultado->ID){
            printf("\n\t\t\tComercializadora\n\n\t\t\tConsulta Por Cliente");
            printf("\n%s", fecha);
            printf("\n\n\t\t\tPor Clave");
            printf("\nNombre\t\t\tClave\tDireccion\t\tTelefono\tCorreo Electronico\tEstatus");
            printf("\n%s %s %s", resultado->nombre, resultado->apellidoP, resultado->apellidoM);
            printf("\t%d", resultado->ID);
            printf("\t%s %s %d %d", resultado->dir.colonia, resultado->dir.calle, resultado->dir.numero, resultado->dir.cp);
            printf("\t%d", resultado->telefono);
            printf("\t%s", resultado->correo);
            printf("\t\t%s", (resultado->estatus)==1?"Activo\0":"Inactivo\0");
            flag = 1;
            break;
        }
    }
    if (flag == 0){
        printf("\nNo se encontraron resultados.");
    }
    fclose(archClientes);
    return;
}