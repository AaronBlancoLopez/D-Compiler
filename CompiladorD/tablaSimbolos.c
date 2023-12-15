#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "abb.h"

//tabla de símbolos
abb TS;

//declaracion de funciones auxiliares
void _insertarTS(char *identificador, int valor);
void _imprimir(abb tablaSimbolos);


//              FUNCIONES PRINCIPALES
//función de inicialización de la tabla de símbolos
void inicializarTS() {
    //palabras clave iniciales
    char* palabra[7];
    palabra[0] = "import";
    palabra[1] = "double";
    palabra[2] = "int";
    palabra[3] = "while";
    palabra[4] = "foreach";
    palabra[5] = "void";
    palabra[6] = "return";
    //valores de las palabras clave
    int valor[7];
    valor[0] = IMPORT;
    valor[1] = DOUBLE;
    valor[2] = INT;
    valor[3] = WHILE;
    valor[4] = FOREACH;
    valor[5] = VOID;
    valor[6] = RETURN;
    //creación del árbol binario
    crear(&TS);
    //inserción de datos iniciales
    for(int i = 0; i < 7; i++){
        _insertarTS(palabra[i], valor[i]);
    }
}

//función de impresión de la tabla de símbolos
void imprimirTS(){
    printf("\nTabla de símbolos:\n");
    _imprimir(TS);
}

//función de busqueda en la TS de un lexema. Devuelve su codigo.
void buscarTS(tipoelem* elem){
    tipoelem aux;
    if(es_miembro(TS, *elem)){
        buscar_nodo(TS, elem->lexema, &aux);
        elem->codigo = aux.codigo;
        return;
    }
    elem->codigo = ID;
    _insertarTS(elem->lexema, elem->codigo);
}

//destruye la TS liberado toda la memoria
void destruirTS(){
    destruir(&TS);
}


//              FUNCIONES AUXILIARES
//función auxiliar para la impresión inorden del arbol binario
void _imprimir(abb tablaSimbolos){
    tipoelem elem;
    if(!es_vacio(tablaSimbolos)){
        _imprimir(izq(tablaSimbolos));
        leer(tablaSimbolos, &elem);
        printf("%-15s %-15d\n", elem.lexema, elem.codigo);
        _imprimir(der(tablaSimbolos));
    }
}

//función auxiliar de inserción de datos en la TS
void _insertarTS(char *identificador, int valor){
    tipoelem elem;
    elem.lexema = (char*)(malloc(sizeof(strlen(identificador) + 1)));
    strcpy(elem.lexema, identificador);
    elem.codigo = valor;
    insertar(&TS, elem);
}