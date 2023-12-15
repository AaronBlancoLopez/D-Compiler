#include <printf.h>
#include "tablaSimbolos.h"
#include "sintactico.h"
#include "entrada.h"
#include "errores.h"

int main(int argc, char **argv) {
    if (argc == 2){
        //inicialización del sistema de entrada
        inicializarSE(argv[1]);
        //inicialización de la tabla de simbolos
        inicializarTS();
        //impresión de la TS inicial
        printf("\n==============TABLA DE SIMBOLOS INICIAL==============\n");
        imprimirTS();
        //se inicia la compilación
        printf("\n==============RESULTADO DE LA COMPILACION==============\n");
        compilar();
        //libera la memoria de la tabla de simbolos
        printf("\n\n==============TABLA DE SIMBOLOS FINAL==============\n");
        imprimirTS();
        destruirTS();
    } else {
        imprimirError(0);
    }

}
