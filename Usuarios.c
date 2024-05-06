#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>


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
