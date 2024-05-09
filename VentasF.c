#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>
#include "./lib/VentasF.h"


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
  system("cls");
  do{

    printf("\n\n\t1.- Venta\n\t2.- Reporte\n\t\t- Día\n\t\tCliente\n\t3.- Regresar al Menú Principal");
    printf("\nSeleccione una opción: ");
    scanf("%d", &op);
  }while(op > 3 || op <1);
  return op;

}


int menuReporte(void)
{
  int op;
  system("cls");
  do{
    printf("\n\n\t1.- Día\n\t2.- Cliente\nIngresa una opción: ");
    scanf("%d", &op);
  }while(op != 1 && op !=2);
  return op;
}
