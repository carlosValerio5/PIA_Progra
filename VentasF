#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

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
