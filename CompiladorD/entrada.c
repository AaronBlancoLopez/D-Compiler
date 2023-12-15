#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "definiciones.h"
#include "errores.h"

#define tamBloque 64
#define A 0
#define B 1

//estructura que contiene informacion sobre el puntero y su bloque
typedef struct {
    int puntero;
    int bloque;
} lector;

//variables de inicio
FILE* codigoFuente;
char bloqueA[tamBloque + 1], bloqueB[tamBloque + 1];
lector inicio, delantero;

//variables de control
int devolverExcepcion = 0;      //controla el caso en el que al devolver() se cambia de bloque
int comentarioExcepcion = 0;    //controla la deteccion de comentarios

//declaracion de funciones auxiliares
void _cargarBloque(int bloque);
char _leerBloque();
int _tamLexema();
void _avanzar();

//              FUNCIONES PRINCIPALES
//función que inicializa el fichero, los buffers y los punteros
void inicializarSE(char *ruta){
    codigoFuente = fopen(ruta, "r");
    if (codigoFuente == NULL) {
        imprimirError(1);
        exit(0);
    }
    bloqueA[tamBloque] = EOF;
    bloqueB[tamBloque] = EOF;
    inicio.puntero = 0;
    inicio.bloque = A;
    delantero.puntero = 0;
    delantero.bloque = A;
    _cargarBloque(A);
}

//función que retrocede la posición del puntero delantero
void devolver(){
    if (delantero.puntero == 0) {
        delantero.puntero = tamBloque;
        delantero.bloque = (delantero.bloque + 1) % 2;
        devolverExcepcion = 1;
    } else {
        delantero.puntero--;
    }
}

//función que lee el lexema comprendido entre los punteros delantero e inicio
void aceptarLexema(tipoelem *elem){
    char* aux;
    int tam = _tamLexema();
    elem->lexema = malloc(tam * sizeof(char ));
    //caso en el que los dos punteros están en el mismo bloque
    if (inicio.bloque == delantero.bloque) {
        //caso en el que el lexema se encuentra entero en el bloque A
        if (inicio.bloque == A) {
            strncpy(elem->lexema, &bloqueA[inicio.puntero], tam);
        //caso en el que el lexema se encuentra entero en el bloque B
        } else {
            strncpy(elem->lexema, &bloqueB[inicio.puntero], tam);
        }
    /*
        * Caso en el que el lexema esta en dos bloques distintos.
        *      1. Se hace una lectura desde 'inicio' hasta el final de su bloque.
        *      2. Se hace una lectura desde el principio del otro bloque hasta
        *              el puntero delantero.
        *      3. Se concatenan ambas lecturas.
     */
    } else {
        //caso en el que el lexema comienza en el bloque A
        if (inicio.bloque == A){
            aux = malloc(delantero.puntero * sizeof(char ));
            strncpy(elem->lexema, &bloqueA[inicio.puntero], tamBloque - inicio.puntero);
            strncpy(aux, &bloqueB[0], delantero.puntero);
            strcat(elem->lexema, aux);
            free(aux);
            inicio.bloque = B;
        //caso en el que el lexema comienza en el bloque B
        } else {
            aux = malloc((delantero.puntero + 1) * sizeof(char ));
            strncpy(elem->lexema, &bloqueB[inicio.puntero], tamBloque - inicio.puntero);
            strncpy(aux, &bloqueA[0], delantero.puntero);
            strcat(elem->lexema, aux);
            free(aux);
            inicio.bloque = A;
        }

    }
    inicio.puntero = delantero.puntero;
}

//funcion de aceptacion especial para gestionar caracteres que no queremos devolver como lexemas como comentarios o espacios
void aceptarEsp(){
    inicio.puntero = delantero.puntero;
    inicio.bloque = delantero.bloque;
}

//función que obtiene el siguiente caracter del código fuente
char siguienteCaracter(){
    char c;
    //si el puntero se encuentra al final del bloque, se avanza antes de leer
    if (delantero.puntero == tamBloque){
        _avanzar();
        c = _leerBloque();
        _avanzar();
        return c;
    }
    c = _leerBloque();
    _avanzar();
    return c;
}

//funcion especifica para detectar un comentario y evitar imprimir el error de lexema muy grande
void detectarComentario(){
    comentarioExcepcion = (comentarioExcepcion + 1) % 2;
}

//              FUNCIONES AUXILIARES
//funcion que lee del archivo y carga el contenido en uno de los bloques
void _cargarBloque(int bloque){
    int leidos;
    if (bloque == A) {
        /*
            * freads() devuelve los caracteres leidos, y ademas no lee los caracteres de fin de fichero.
            * Si lee menos de 64 (tamaño de bloque), se mete a mano el siguiente como fin de fichero.
        */
        leidos = fread(&bloqueA, sizeof(char), tamBloque, codigoFuente);
        if (leidos < 64 && leidos > 0) {
            bloqueA[leidos] = EOF;
        } else if (leidos <= 0){
            imprimirError(2);
        }
    } else if (bloque == B) {
        leidos = fread(&bloqueB, sizeof(char), tamBloque, codigoFuente);
        if (leidos < 64 && leidos > 0) {
            bloqueA[leidos] = EOF;
        } else if (leidos <= 0){
            imprimirError(2);
        }
    }
}

//funcion que modulariza la lectura de los bloques
char _leerBloque(){
    char c;
    if (delantero.bloque == A) {
        c = bloqueA[delantero.puntero];
    } else {
        c = bloqueB[delantero.puntero];
    }
    return c;
}

//funcion que calcula el tamaño del lexema aceptado
int _tamLexema(){
    //caso en el que los dos punteros están en el mismo bloque
    if (inicio.bloque == delantero.bloque) {
        return delantero.puntero - inicio.puntero;
        //caso en el que los punteros estan en dos bloques distintos
    } else {
        //caso en el que el lexema pasa por un caracter de fin de bloque o fichero
        return ((tamBloque - inicio.puntero) + (delantero.puntero + 1)) - 1;
    }
}

//función que avanza la posición del puntero delantero
void _avanzar(){
    //si el puntero delantero está al final de un bloque, pasa a apuntar al comienzo del otro bloque
    if (delantero.puntero == tamBloque){
        delantero.puntero = 0;
        delantero.bloque = (delantero.bloque + 1) % 2;
        if (devolverExcepcion){
            devolverExcepcion = 0;
        } else {
            _cargarBloque(delantero.bloque);
            //caso en el que delantero alcanza al bloque en el que esta el puntero de inicio
            if (inicio.bloque == delantero.bloque) {
                inicio.puntero = 0;
                inicio.bloque = (inicio.bloque + 1) % 2;
                //comprobacion de caso comentario en el que no se imprime el error
                if (comentarioExcepcion == 0) {
                    imprimirError(3);
                }
            }
        }
    } else {
        delantero.puntero++;
    }
}