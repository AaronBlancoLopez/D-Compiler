#ifndef TABLASIMBOLOS_H
#define TABLASIMBOLOS_H
#include "definiciones.h"

//función auxiliar de inserción de datos en la TS
void inicializarTS();

//función de impresión de la tabla de símbolos
void imprimirTS();

//función de busqueda en la TS de un lexema. Devuelve su codigo.
void buscarTS(tipoelem* elem);

//destruye la TS liberado toda la memoria
void destruirTS();

#endif