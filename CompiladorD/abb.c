#include "abb.h"
#include <stdlib.h>
#include <string.h>

struct celda {
    tipoelem info;
    struct celda *izq, *der;
};

/*Extraer la clave de una celda */
tipoclave _clave_elem(tipoelem *E) {
    return E->lexema;
}

/* Esta funcion puente nos permite modificar el tipo de
 * de datos del TAD sin tener que cambiar todas las
 * comparaciones del resto de la biblioteca y en su lugar
 * cambiando solo esta. */
int _comparar_claves(tipoclave cl1, tipoclave cl2) {
    if(strcmp(cl1,cl2) == 0){
        return 0;
    }else if(strcmp(cl1,cl2) > 0){
        return 1;
    }else{
        return -1;
    }
}

//OPERACIONES DE CREACIÓN Y DESTRUCCIÓN
void crear(abb *A) {
    *A = NULL;
}

void destruir(abb *A) {
    if (*A != NULL) {
        destruir(&(*A)->izq);
        destruir(&(*A)->der);
        free((*A)->info.lexema);
        free(*A);
        *A = NULL;
    }
}

//OPERACIONES DE INFORMACIÓN

unsigned es_vacio(abb A) {
    return A == NULL;
}

abb izq(abb A) {
    return A->izq;
}

abb der(abb A) {
    return A->der;
}


void leer(abb A, tipoelem *E) {
    *E = A->info;
}

// Función privada para comparar las claves

int _comparar_clave_elem(tipoclave cl, tipoelem E) {
    return _comparar_claves(cl, _clave_elem(&E));
}
//Función privada para informar si una clave está en el árbol


unsigned _es_miembro_clave(abb A, tipoclave cl) {
    if (es_vacio(A)) {
        return 0;
    }
    int comp = _comparar_clave_elem(cl, A->info);

    if (comp == 0) { //cl == A->info
        return 1;
    }
    if (comp > 0) { //cl > A->info
        return _es_miembro_clave(der(A), cl);
    }
    //cl < A->info
    return _es_miembro_clave(izq(A), cl);
}

//Funciones públicas

unsigned es_miembro(abb A, tipoelem E) {
    return _es_miembro_clave(A, _clave_elem(&E));
}


void buscar_nodo(abb A, tipoclave cl, tipoelem *nodo) {
    if (es_vacio(A)) {
        return;
    }
    int comp = _comparar_clave_elem(cl, A->info);

    if (comp == 0) { // cl == A->info
        *nodo = A->info;
    } else if (comp < 0) { // cl < A->info
        buscar_nodo(A->izq, cl, nodo);
    } else { // cl > A->info
        buscar_nodo(A->der, cl, nodo);
    }
}
//OPERACIONES DE MODIFICACIÓN


/* Funcion recursiva para insertar un nuevo nodo
   en el arbol. Se presupone que no existe un nodo
   con la misma clave en el arbol. */
void insertar(abb *A, tipoelem E) {
    if (es_vacio(*A)) {
        *A = (abb) malloc(sizeof (struct celda));
        (*A)->info = E;
        (*A)->izq = NULL;
        (*A)->der = NULL;
        return;
    }
    tipoclave cl = _clave_elem(&E);
    int comp = _comparar_clave_elem(cl, (*A)->info);
    if (comp > 0) {
        insertar(&(*A)->der, E);
    } else {
        insertar(&(*A)->izq, E);
    }
}

/* Funcion privada que devuelve mínimo de subárbol dcho */
tipoelem _suprimir_min(abb * A) {//Se devuelve el elemento más a la izquierda
    abb aux;
    tipoelem ele;
    if (es_vacio((*A)->izq)) {//Si izquierda vacía, se devuelve codigo nodo actual A
        ele = (*A)->info;
        aux = *A;
        *A = (*A)->der;
        free(aux);
        return ele;
    } else {
        return _suprimir_min(&(*A)->izq); //se vuelve a buscar mínimo rama izquierda
    }
}

/* Funcion que permite eliminar un nodo del arbol */
void suprimir(abb *A, tipoelem E) {
    abb aux;
    if (es_vacio(*A)) {
        return;
    }

    tipoclave cl = _clave_elem(&E);
    int comp = _comparar_clave_elem(cl, (*A)->info);
    if (comp < 0) {
        suprimir(&(*A)->izq, E);
    } else if (comp > 0) { //(E > (*A)->info) {
        suprimir(&(*A)->der, E);
    } else if (es_vacio((*A)->izq) && es_vacio((*A)->der)) {
        free(*A);
        *A = NULL;
    } else if (es_vacio((*A)->izq)) { // pero no es vacio derecha
        aux = *A;
        *A = (*A)->der;
        free(aux);
    } else if (es_vacio((*A)->der)) { //pero no es vacio izquierda
        aux = *A;
        *A = (*A)->izq;
        free(aux);
    } else {
        (*A)->info = _suprimir_min(&(*A)->der);
    }
}

/* Funcion privada para pasar la clave y no tener que
   extraerla del nodo en las llamadas recursivas.*/
void _modificar(abb A, tipoclave cl, tipoelem nodo) {
    if (es_vacio(A)) {
        return;
    }
    int comp = _comparar_clave_elem(cl, A->info);
    if (comp == 0) {
        A->info = nodo;
    } else if (comp < 0) {
        _modificar(A->izq, cl, nodo);
    } else {
        _modificar(A->der, cl, nodo);
    }
}