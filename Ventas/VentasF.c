#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>
#include <string.h>
#include "VentasF.h"
#include "../Clientes/clientes.h"
#include "../Productos/producto.h"

#include <sys/stat.h>
#include <sys/types.h>



void obtener_fecha(char *fecha_strF, size_t max_len) //Funcion para obtener una fecha en formato string
{//Si se quiere usar la funcion tienes que mandarle como argumentos la cadena donde se va a guardar(Longitud minima de 11) y un entero con valor minimo de  11
    //Obtenemos la fecha
    time_t t = time(NULL);
    struct tm *fecha_actual = localtime(&t);

    
    // Formatea la fecha en la cadena
    strftime(fecha_strF, max_len, "%Y-%m-%d", fecha_actual);
}



int menuF(void) //Esta funcion solo muestra el menu de ventas y retorna la opcion elegida
{
    system("cls");
    setlocale(LC_ALL, "spanish");//Cambiamos el idioma a español REVISAR SI FUNCIONA EN TODAS LAS MAQUINAS
    int op;
    //system("cls");
    do{
        printf("\n\n\t1.- Venta\n\t2.- Reporte\n\t\t- Dia\n\t\t-Cliente\n\t3.- Regresar al Menu Principal");
        printf("\nSeleccione una opcion: ");
        scanf("%d", &op);
    } while(op > 3 || op <1);
    return op;
    Sleep(2000);

}


int menuReporte(void)
{
    int op;
    system("cls");
    do{
        printf("\n\n\t1.- Dia\n\t2.- Cliente\nIngresa una opcion: ");
        scanf("%d", &op);
    } while(op != 1 && op != 2);
    return op;
    Sleep(2000);
}


int consultaClav2(cliente *resultado){

    FILE *archClientes;
    int idbusqueda;
    int flag = 0;

    archClientes = fopen("./bin/clientes.bin", "rb");
    if (archClientes == NULL){
        printf("\nError al abrir el archivo.");
        system("pause");
        return 0;
    }
	fflush(stdin);
    printf("\nIngrese el ID del cliente a buscar (Pulse 0 para regresar al submenu): ");
    scanf("%d", &idbusqueda);
    if(idbusqueda == 0)
    {
        return 0;
    }
    while(fread(resultado, sizeof(cliente), 1, archClientes)!= 0){
        if (idbusqueda == resultado->ID){
            flag = 1;
            break;
        }
    }
    fclose(archClientes);
    if (flag == 0){
        printf("\nNo se encontraron resultados.");
        return consultaClav2(resultado);
        
    }
    return flag;
}

int consultaClavP(Producto *ProductoVf){

    FILE *archProducto;
    int idbusqueda;
    int flag = 0;

    archProducto = fopen("./bin/productos.bin", "rb");
    if (archProducto == NULL){
        printf("\nError al abrir el archivo.");
        return 0;
    }

    printf("\nIngrese el ID del producto a comprar (Pulse 0 para regresar al submenu): ");
    fflush(stdin);
    scanf("%d", &idbusqueda);
    if(idbusqueda == 0)
    {
        return 0;
    }

    while(fread(ProductoVf, sizeof(Producto), 1, archProducto)){
        if (idbusqueda == ProductoVf->clave){
            flag = 1;
            break;
        }
    }
    fclose(archProducto);
    
    if (flag == 0){
        printf("\nNo se encontraron resultados.");
        return consultaClavP(ProductoVf);
        
    }
    return flag;
}


void procesoVenta(dataV *ventaF, infoI *InfoEF, char ***cadenas, int *tamanio)
{
    int cantidad, op;
    Producto ProductoV;
    Producto ProductoC;
	 
    
    if(consultaClavP(&ProductoV) !=0)
    {
        ventaF->ID_Producto = ProductoV.clave;
        ventaF->precio = ProductoV.precio;
        
        
        printf("\nInformacion de producto: \nDescripcion: %s \nExistencia: %d \nPrecio: %.2f", ProductoV.descripcion, ProductoV.cantidad, ProductoV.precio );
        do{
            printf("\nIngrese la cantidad a comprar: ");
            scanf("%d", &cantidad);
        }while(cantidad < 1 || cantidad > ProductoV.cantidad);
        ventaF->cantidadC = cantidad;
        InfoEF->subtotal+= ProductoV.precio*cantidad;
        
        //Proceso cambio de stock
        ProductoV.cantidad-= ventaF->cantidadC;
        FILE*archProd = fopen("./bin/productos.bin", "rb+");
        rewind(archProd);
        while(fread(&ProductoC, sizeof(Producto),1,archProd)!=0)
        {
        	if(ProductoC.clave == ProductoV.clave)
        	{
        		fseek(archProd, -(long)sizeof(Producto),SEEK_CUR);
        		fwrite(&ProductoV, sizeof(Producto),1,archProd);
        		break;
			}
		}
		fclose(archProd);
		
		//Esto es para poder almacenar los nombres de los productos
		(*tamanio)++;
		*cadenas = realloc(*cadenas, (*tamanio)*sizeof(char *));
		if (*cadenas == NULL) {
	        perror("Error al realocar memoria");
	        exit(EXIT_FAILURE);
	    }
	    (*cadenas)[*tamanio - 1] = malloc((strlen(ProductoV.nombre) + 1) * sizeof(char));
        if ((*cadenas)[*tamanio - 1] == NULL) {
            perror("Error al asignar memoria para la nueva cadena");
            exit(EXIT_FAILURE);
        }
        strcpy((*cadenas)[*tamanio - 1], ProductoV.nombre);
	
	
		
		

        FILE *archivoVentas = fopen("./bin/VentasG.bin", "ab");
        fwrite(ventaF, sizeof(dataV), 1, archivoVentas);
		fclose(archivoVentas);

        printf("\nDesea hacer otra compra (Si =1 / No =0 ): ");
        scanf("%d",&op);
        if(op == 1)
        {
            procesoVenta(ventaF, InfoEF, cadenas, tamanio);
        }else
        {
            return;
        }
    }else{
        return;
    }
}




void procesoTicket()
{
	system("cls");
    dataV venta;
    dataV ventaL;
    infoI infoE;
    cliente clienteV;   
	
	
	char **NombreProds = NULL;
	int cantProd = 0, i;
	
	 
    if(consultaClav2(&clienteV) == 1)
    {
        venta.ID_Cliente=clienteV.ID;
        struct stat st;
        const char *nombreArchivo = "./bin/ventasG.bin";
        if(stat(nombreArchivo, &st)== 0)
        {
            FILE*archivoVentas = fopen(nombreArchivo, "rb+");
            while(fread(&ventaL, sizeof(dataV),1, archivoVentas) ==1)
            {}
            fclose(archivoVentas);
            venta.folio = ventaL.folio+1;
        }
        else
        {
            FILE *archivo = fopen(nombreArchivo,"wb+");
            fclose(archivo);
            venta.folio=1;
        }

        obtener_fecha(venta.fecha, 11);
        strcpy(infoE.fecha,venta.fecha);
        infoE.folio = venta.folio;
        strcpy(infoE.cliente, clienteV.nombre);
        infoE.subtotal =0.0;

        procesoVenta(&venta, &infoE, &NombreProds,&cantProd);
        if(infoE.subtotal ==0)
        {
        	return;
		}
		printf("\n\n\n%f, %f",clienteV.descuento, infoE.descuento );
        infoE.descuento = infoE.subtotal*clienteV.descuento;
        infoE.iva = infoE.subtotal*.16;
        infoE.total = infoE.subtotal+infoE.iva-infoE.descuento;

        FILE *archiInfoE = fopen("./bin/InfoE.bin", "ab");
        fwrite(&infoE, sizeof(infoI), 1, archiInfoE);
        fclose(archiInfoE);

        system("cls");
        printf("\n\n\t\tTicket de compra:");
        printf("\nCliente: %s\n", infoE.cliente);
        printf("Fecha: %s\n", infoE.fecha);
        printf("Folio: %d\n", infoE.folio);
        printf("Productos comprados:\n");
        
        for (i = 0; i < cantProd; i++) {
            printf("%d. %s\n", i + 1, *(NombreProds+i));
        }
        printf("\nSubtotal: %.2f\n", infoE.subtotal);
        printf("Descuento: %.2f\n", infoE.descuento);
        printf("IVA: %.2f\n", infoE.iva);
        printf("Total: %.2f\n", infoE.total);
        
        //Generacion del nombre del ticket
        char nombreTicket[100];
        strcpy(nombreTicket, "Facturas/Fact");
        char numeroStr[10];
        sprintf(numeroStr, "%d", venta.folio);
        strcat(nombreTicket, numeroStr);
        strcat(nombreTicket, ".txt");
        //GUARDAMOS EL TICKET
        FILE *archivoTicket = fopen(nombreTicket,"w");
        if (archivoTicket == NULL) {
        	perror("Error al abrir el archivo");
        	return;
    	}
        fprintf(archivoTicket, "\n\n\t\tTicket de compra:");
	    fprintf(archivoTicket, "\nCliente: %s\n", infoE.cliente);
	    fprintf(archivoTicket, "Fecha: %s\n", infoE.fecha);
	    fprintf(archivoTicket, "Folio: %d\n", infoE.folio);
	    fprintf(archivoTicket, "Productos comprados:\n");
	
	    for (i = 0; i < cantProd; i++) {
	        fprintf(archivoTicket, "%d. %s\n", i + 1, *(NombreProds+i));
	    }
	    
	    fprintf(archivoTicket, "\nSubtotal: %.2f\n", infoE.subtotal);
	    fprintf(archivoTicket, "Descuento: %.2f\n",  infoE.descuento);
	    fprintf(archivoTicket, "IVA: %.2f\n", infoE.iva);
	    fprintf(archivoTicket, "Total: %.2f\n", infoE.total);
	
	    fclose(archivoTicket);
        
        
        
        
        
        
        for (i = 0; i < cantProd; i++) {
            free(NombreProds[i]);
        }
        free(NombreProds);
        system("pause");
    }

}




void mostrarDatosDeHoy() {
	system("cls");
    // Obtener la fecha actual en formato de cadena (dd/mm/aaaa)
    char fecha_actual[11];
    obtener_fecha(fecha_actual, 11);

    // Abrir el archivo "InfoE.dat" en modo lectura
    FILE *archivo = fopen("./bin/InfoE.bin", "rb");
    if (archivo == NULL) {
        printf("No se pudo abrir el archivo.\n");
        return;
    }
    float total_subtotal = 0.0;
    float total_descuento = 0.0;
    float total_iva = 0.0;
    float total_total = 0.0;

    // Leer los datos del archivo y mostrar los que coincidan con la fecha actual
    infoI info;
    int encontrados = 0;
    printf("\n\t\t\t\tComercializadora Fuentes\n\t\t\tReporte de Ventas al d�a %s\nFolio   Cliente   Subtotal   Descuento   IVA     Total\n\n", fecha_actual);
    while (fread(&info, sizeof(infoI), 1, archivo) ==1) {
        if (strcmp(info.fecha, fecha_actual) == 0) {
            printf("%-8d%-10s%-12.2f%-10.2f%-8.2f%-8.2f\n", info.folio, info.cliente, info.subtotal, info.descuento, info.iva, info.total);
            encontrados = 1;
            total_subtotal += info.subtotal;
            total_descuento += info.descuento;
            total_iva += info.iva;
            total_total += info.total;
        }
    }

    if (encontrados==0) {
        printf("\nNo se encontraron datos para la fecha de hoy.\n");
    }else{
    	printf("%-8s%-10s%-12.2f%-10.2f%-8.2f%-8.2f\n", "", "", total_subtotal, total_descuento, total_iva, total_total);
	}

    fclose(archivo);
    system("pause");
}

void mostrarVentasPorCliente() {
	system("cls");
    cliente clienteV;
    infoI infoExtra;
    dataV infoV;
    Producto infoProd;
    int idCliente;
    
    

    FILE *archInfoE = fopen("./bin/InfoE.bin", "rb");
    FILE *archVentas = fopen("./bin/ventasG.bin", "rb");
    FILE *archProd = fopen("./bin/productos.bin", "rb");
    if(consultaClav2(&clienteV) == 1)
    {
    	printf("                                         Comercializadora Fuentes\n");
		printf("                                      Reporte de Ventas por Cliente\n");
		printf("Clave:  %ld\n", clienteV.ID);
		printf("Nombre :  %s %s %s\n", clienteV.nombre, clienteV.apellidoP, clienteV.apellidoM);
		printf("Direcci�n:  %d %s %s %d\n", clienteV.dir.cp, clienteV.dir.colonia, clienteV.dir.calle, clienteV.dir.numero);
		printf("Tel�fono : %ld\t\t\t\t\t\t\tCorreo Electr�nico: %s\n\n", clienteV.telefono, clienteV.correo);
    	while(fread(&infoExtra, sizeof(infoI), 1, archInfoE) == 1)
    	{
    		if(strcmp(clienteV.nombre, infoExtra.cliente) == 0)
    		{
				//printf("\n\n\n\nCantidad de ventas que hace");
    			while(fread(&infoV, sizeof(dataV), 1, archVentas) == 1)
    			{
    				
    				if(infoV.folio == infoExtra.folio)
    				{
    					//printf("\n\n\n\nCantidad de productos en el folio");
    					while(fread(&infoProd, sizeof(Producto), 1,archProd)==1 )
    					{
    						if(infoV.ID_Producto == infoProd.clave)
    						{
    							//printf("\n\n\n\nCantidad de productos con la clave");
							    int longitud = strlen(infoProd.descripcion);
							    if (infoProd.descripcion[longitud - 1] == '\n') {
							        infoProd.descripcion[longitud - 1] = '\0'; // Reemplaza el car�cter de salto de l�nea con el terminador de cadena
							    }
							    printf("%-8s%-15s%-50s%-10s%-8s%-8s\n", "Folio", "Fecha", "Descripcion", "Cantidad", "Precio", "Total");
								printf("%-8d%-15s%-50s%-10d%-8.2f%-8.2f\n", infoExtra.folio, infoExtra.fecha, infoProd.descripcion, infoV.cantidadC, infoProd.precio, (infoProd.precio*(float)infoV.cantidadC));
							}
						}
						rewind(archProd);
					}
				}
				printf("%110s: %.2f\n%110s: %.2f\n%110s: %.2f\n%110s: %.2f\n\n\n", "Subtotal", infoExtra.subtotal, "Descuento", infoExtra.descuento, "IVA", infoExtra.iva, "Total", infoExtra.total);
				rewind(archVentas);
			}
		}
	}
	fclose(archInfoE);
	fclose(archVentas);	
	fclose(archProd);
	system("pause");
}
    	
    



