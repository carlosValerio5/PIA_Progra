#include <stdio.h>
#include "clientes.h"
#include <ctype.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>


//Sirve para registrar a un nuevo cliente
int nuevoCliente(void){
    cliente nuevoCliente;

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
    printf("\nIngrese apellido Paterno: ");
    fflush(stdin);
    fgets(nuevoCliente.apellidoP, sizeof(nuevoCliente.apellidoP), stdin);
    printf("\nIngrese apellido Materno: ");
    fflush(stdin);
    fgets(nuevoCliente.apellidoM, sizeof(nuevoCliente.apellidoM), stdin);

    //RFC
    printf("Ingrese RFC: ");
    fflush(stdin);
    fgets(nuevoCliente.RFC, 11, stdin);
    compararRFC(&nuevoCliente, archClientes);

    //Correo electronico
    printf("\nIngrese su correo electronico: ");
    fflush(stdin);
    fgets(nuevoCliente.correo, sizeof(nuevoCliente.correo), stdin);

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

    printf("\nIngrese su colonia: ");
    fflush(stdin);
    fgets(nuevoCliente.dir.colonia, sizeof(nuevoCliente.dir.colonia), stdin);

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
    return 1;
}