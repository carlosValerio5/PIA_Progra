#include <stdio.h>
#include <string.h>
#include "clientes.c"
#include "VentasF.c"

int main(){
    
    printf("Hola jeje");
    // Apartado de ventas (Esto va dentro de un switch op 3)
    int op;
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
    }


 
    return 0;
}
