#ifndef DEFINICIONES_H
#define DEFINICIONES_H

//declaracion de valores para cada componente léxico
//palabras clave
#define IMPORT 300
#define DOUBLE 301
#define INT 302
#define WHILE 303
#define FOREACH 304
#define RETURN 305
#define VOID 306
//resto de posibles valores
#define ID 307
#define STRING 308
#define INTEGER 309
#define FLOAT 310
#define OPERATOR 311
#define EOFVAL -999

//estructura de almacenamiento de cada componente léxico que inclue el componente y su codigo
typedef struct  {
    char *lexema;
    int codigo;
}tipoelem;

#endif