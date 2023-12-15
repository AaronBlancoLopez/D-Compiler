#include <stdio.h>
#include "errores.h"

//funcion de gestion de errores
void imprimirError(int codigo) {
    switch (codigo) {
        case 0:
            printf("\nEl numero de argumentos es incorrecto. Numero de argumentos necesarios: 1 (ruta al archivo fuente)");
            break;
        case 1:
            printf("\nError al leer el archivo. Comprobar que el archivo existe y que la ruta es correcta.");
            break;
        case 2:
            printf("\nError al cargar el bloque");
            break;
        case 3:
            printf("\n\nEl tamaño del lexema es demasiado grande. Solo se imprime la parte disponible del lexema.");
            break;
    }
}