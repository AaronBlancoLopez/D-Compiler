#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "definiciones.h"
#include "lexico.h"

//función que inicia el proceso de compilación del código fuente
void compilar(){
    tipoelem elem;
    elem.lexema = NULL;
    do {
        siguienteComponente(&elem);
        /*
            * Si se llega al EOF este no se imprime al no ser un caracter.
            * Se libera la memoria de la estructura y termina.
         */
        if (elem.codigo == EOFVAL){
            if (elem.lexema != NULL){
                free(elem.lexema);
                elem.lexema = NULL;
            }
            break;
        }
        printf("\n<\"%s\"> , <\"%d\">", elem.lexema, elem.codigo);
        //se libera la memoria reservada por el lexema
        if (elem.lexema != NULL){
            free(elem.lexema);
            elem.lexema = NULL;
        }
    } while(elem.codigo != EOFVAL);
}