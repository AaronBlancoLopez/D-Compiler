#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include "definiciones.h"
#include "entrada.h"
#include "tablaSimbolos.h"

//variable de ultimo caracter leido
char c;

//declaracion de funciones auxiliares
void _puntoFlotante(tipoelem* elem);
void _puntoFlotanteExponente(tipoelem* elem);
void _binario(tipoelem* elem);

//              FUNCIONES PRINCIPALES
//automata de cadenas alfanuméricas
void alfanumerico(tipoelem* elem){
    int aceptar = 0;
    while (aceptar != 1){
        if (!isalnum(c) && c != '_'){
            aceptar = 1;
            //retrocede el puntero final del sistema de entrada
            devolver();
            //obtiene el lexema aceptado
            aceptarLexema(elem);
            //caso en el que la cadena es "sizeof", la cual es un operador en D
            if (strcmp(elem->lexema, "sizeof") == 0){
                elem->codigo = OPERATOR;
                return;
            }
            //busca en la TS la palabra
            buscarTS(elem);
        } else {
            c = siguienteCaracter();
        }
    }
}

//automata de números
void numerico(tipoelem* elem){
    char sig = siguienteCaracter();
    /*
         * Caso especial en el que el numero empieza por 0.
         * Puede suponer un numero en binario o hexadecimal:
         *      0b0101010101
         *      0x3A34F10D
         * Puede suponer el numero 0 con o sin los sufijos:
         *      0
         *      0u
         *      0Lu
    */
    if (c == '0') {
        //numero en binario
        if (sig == 'b' || sig == 'B') {
            _binario(elem);
        //numero 0
        } else if (sig == '_'){
            while (sig == '_') {
                sig = siguienteCaracter();
            }
            if (sig == 'u' || sig == 'U') {
                sig = siguienteCaracter();
                if (sig == 'L') {
                    elem->codigo = INTEGER;
                    aceptarLexema(elem);
                }
            } else if (sig == 'L') {
                sig = siguienteCaracter();
                if (sig == 'u' || sig == 'U') {
                    elem->codigo = INTEGER;
                    aceptarLexema(elem);
                }
            } else {
                devolver();
                aceptarLexema(elem);
            }
        } else {
            devolver();
            aceptarLexema(elem);
        }
    } else {
        while (isnumber(sig) || sig == '_') {
            sig = siguienteCaracter();
        }
        /*
            * Numeros en punto flotante en los que aparece primero el punto.
            * Ejemplos:
            *      5.3
            *      40.4e+1
        */
        if (sig == '.') {
            _puntoFlotante(elem);
        /*
            * Numeros en punto flotante en los que aparece primero el exponente.
            * Ejemplos:
            *      53e+1
            *      40E-1
        */
        } else if (sig == 'e' || sig == 'E') {
            _puntoFlotanteExponente(elem);
        } else {
            devolver();
            elem->codigo = INTEGER;
            aceptarLexema(elem);
        }
    }
}

//automata de strings
void cadenas(tipoelem* elem){
    char sig = siguienteCaracter();
    while (sig != EOF) {
        // comprobacion de '\'
        if (sig == 92) {
            siguienteCaracter();
        } else if (sig == '"') {
            elem->codigo = STRING;
            aceptarLexema(elem);
            return;
        }
        sig = siguienteCaracter();
    }
}

//automata de operadores
void operadores(tipoelem* elem) {
    char primero = c, sig = siguienteCaracter();
    // + , ++ , +=
    if (primero == '+') {
        if (sig == '+' || sig == '=') {
            elem->codigo = OPERATOR;
            aceptarLexema(elem);
        } else {
            devolver();
            elem->codigo = primero;
            aceptarLexema(elem);
        }
    // - , -- , -= , ->
    } else if (primero == '-') {
        if (sig == '-' || sig == '=' || sig == '>') {
            elem->codigo = OPERATOR;
            aceptarLexema(elem);
        } else {
            devolver();
            elem->codigo = primero;
            aceptarLexema(elem);
        }
    // & , && , &=
    } else if (primero == '&') {
        if (sig == '&' || sig == '=') {
            elem->codigo = OPERATOR;
            aceptarLexema(elem);
        } else {
            devolver();
            elem->codigo = primero;
            aceptarLexema(elem);
        }
    // | , || , |=
    } else if (primero == '|') {
        if (sig == '|' || sig == '=') {
            elem->codigo = OPERATOR;
            aceptarLexema(elem);
        } else {
            devolver();
            elem->codigo = primero;
            aceptarLexema(elem);
        }
    /*
        * operadores que solo pueden tener un caracter, y en caso de tener dos el segundo es '='
        * Ejemplos:
        *      *=
        *      /=
        *      %=
        *      !=
        *      ==
        *      y sus formas de un coracter
    */
    } else if (primero == '*' || primero == '/' || primero == '%' || primero == '!' || primero == '=' || primero == '^' || primero == '~') {
        if (sig == '=') {
            elem->codigo = OPERATOR;
            aceptarLexema(elem);
        } else {
            devolver();
            elem->codigo = primero;
            aceptarLexema(elem);
        }
    /*
        * Los operadores que comienzan por > o < pueden llegar a tener hasta 4 caracteres.
        * Dado que realizan la misma operaci
        * Ejemplos:
        *      Right Shift: > , >> , >>> , >= , >>= , >>>=
        *      Left Shift: < , << , <= , <<=
    */
    // >
    } else if (primero == '>') {
        // >=
        if (sig == '=') {
            elem->codigo = OPERATOR;
            aceptarLexema(elem);
        // >>
        } else if (sig == '>') {
            sig = siguienteCaracter();
            // >>=
            if (sig == '=') {
                elem->codigo = OPERATOR;
                aceptarLexema(elem);
            // >>>
            } else if (sig == '>') {
                sig = siguienteCaracter();
                // >>>=
                if (sig == '=') {
                    elem->codigo = OPERATOR;
                    aceptarLexema(elem);
                } else {
                    devolver();
                    elem->codigo = OPERATOR;
                    aceptarLexema(elem);
                }
            } else {
                devolver();
                elem->codigo = OPERATOR;
                aceptarLexema(elem);
            }
        } else {
            devolver();
            elem->codigo = primero;
            aceptarLexema(elem);
        }
    // <
    } else if (primero == '<') {
        // <=
        if (sig == '=') {
            elem->codigo = OPERATOR;
            aceptarLexema(elem);
        // <<
        } else if (sig == '<') {
            sig = siguienteCaracter();
            // <<=
            if (sig == '=') {
                elem->codigo = OPERATOR;
                aceptarLexema(elem);
            } else {
                devolver();
                elem->codigo = OPERATOR;
                aceptarLexema(elem);
            }
        } else {
            devolver();
            elem->codigo = primero;
            aceptarLexema(elem);
        }
    // ?:
    } else if (primero == '?') {
        if (sig == ':') {
            elem->codigo = OPERATOR;
            aceptarLexema(elem);
        } else {
            devolver();
            elem->codigo = primero;
            aceptarLexema(elem);
        }
    }
}

//autómata de comentarios
void comentarios(){
    int cont = 0, aceptar = 0;
    char tipo = siguienteCaracter();
    char caracter, cierre;
    //comentario de una linea
    if(tipo == '/'){
        do {
            tipo = siguienteCaracter();
        } while (tipo != '\n');
        aceptarEsp();
    //comentario multilinea
    } else if (tipo == '*'){
        do {
            caracter = siguienteCaracter();
            if (caracter == '*') {
                cierre = siguienteCaracter();
                if (cierre == '/'){
                    aceptar = 1;
                    aceptarEsp();
                }
            }
        } while (aceptar != 1);
    //comentario anidado
    } else if (tipo == '+'){
        cont++;
        do {
            caracter = siguienteCaracter();
            if (caracter == '/'){
                if (siguienteCaracter() == '+')
                    cont++;
            } else if (caracter == '+'){
                if (siguienteCaracter() == '/'){
                    cont--;
                }
            }
        } while (cont > 0);
        aceptarEsp();
    }
}

//funcion que obtiene el siguiente componente lexico a traves de los automatas
void siguienteComponente(tipoelem *elem){
    int aceptar = 0;
    while (aceptar != 1) {
        c = siguienteCaracter();
        //lexemas que solo pueden ser formados por un caracter
        if (c == '(' || c == ')' || c == '[' || c == ']' || c == '{' || c == '}' || c == ';' || c == '.' || c == ',') {
            elem->codigo = c;
            aceptarLexema(elem);
            aceptar = 1;
        } else if (c == ' ' || c == '\n') {
            aceptarEsp();
        //automata de cadenas alfanumericas
        } else if (isalpha(c)) {
            alfanumerico(elem);
            aceptar = 1;
        /*
            * Hay que gestionar el caso en el que las variables comienzan por un '_'.
            * En ese caso, el primer caracter de la variable distinto de '_' puede ser
            * un numero o una letra. Tambien pueden ser unicamente caracteres '_'.
            * Ejemplo:
            *      int _variable = 0;
            *      int ___0variable_ = 0;
            *      int _______ = 0;
         */
        } else if (c == '_') {
            while (c == '_') {
                c = siguienteCaracter();
            }
            if (isalnum(c)) {
                alfanumerico(elem);
                aceptar = 1;
            } else {
                devolver();
                elem->codigo = ID;
                aceptarLexema(elem);
            }
        //automata de numeros
        } else if (isdigit(c)) {
            numerico(elem);
            aceptar = 1;
        } else if (c == '"') {
            cadenas(elem);
            aceptar = 1;
        /*
            * Hay que distinguir las posibilidades del caracter '/'
            * Puede ser un comentario:
            *      //Esto es un comentario
            * O un operador:
            *        a / b
         */
        } else if (c == '/'){
            char sig = siguienteCaracter();
            if (sig == '/' || sig == '*' || sig == '+'){
                //se activa la deteccion de un comentario
                detectarComentario();
                devolver();
                //automata de comentarios
                comentarios();
                //el flag de deteccion de comentario vuelve a cero
                detectarComentario();
            } else {
                devolver();
                operadores(elem);
                aceptar = 1;
            }
        //automata de operadores
        } else if (c == '<' || c == '>' || c == '+' || c == '-' || c == '*' || c == '%' || c == '&' || c == '|' || c == '^' || c == '?' || c == '=') {
            operadores(elem);
            aceptar = 1;
        } else if (c == EOF) {
            elem->codigo = EOFVAL;
            aceptarEsp();
            aceptar = 1;
        }
    }
}

//              FUNCIONES AUXILIARES
//automata para numeros en punto flotante en los que primero aparece el punto
void _puntoFlotante(tipoelem* elem){
    char sig = siguienteCaracter();
    //el caracter despues del punto siempre tiene que ser un numero
    if (!isnumber(sig)){
        devolver();
        aceptarLexema(elem);
    } else {
        //se tienen en cuenta todas las posibles combinaciones de '_' y digitos
        while (sig == '_' || isnumber(sig)) {
            sig = siguienteCaracter();
        }
        /*
            * Puede aparecer un exponente.
            * Ejemplos:
            *   40.4e+1
            *   5.3E-3
        */
        if (sig == 'e' || sig == 'E') {
            sig = siguienteCaracter();
            if (sig == '+' || sig == '-') {
                sig = siguienteCaracter();
                while (isnumber(sig) || sig == '_') {
                    sig = siguienteCaracter();
                }
                devolver();
                elem->codigo = FLOAT;
                aceptarLexema(elem);
            } else {
                devolver();
                elem->codigo = FLOAT;
                aceptarLexema(elem);
            }
            /*
                * Los numeros en punto flotante pueden tambien contener sufijos.
                * Ejemplos:
                *      40.4e+1f
                *      5.3E-3F
                *      12.1L
             */
        } else if (sig == 'f' || sig == 'F' || sig == 'L') {
            elem->codigo = FLOAT;
            aceptarLexema(elem);
        } else {
            devolver();
            elem->codigo = FLOAT;
            aceptarLexema(elem);
        }
    }
}

//automata par numeros en punto flotante en los que primero aparece el exponente
void _puntoFlotanteExponente(tipoelem* elem) {
    char sig = siguienteCaracter();
    if (sig == '+' || sig == '-') {
        sig = siguienteCaracter();
        while (isnumber(sig) || sig == '_') {
            sig = siguienteCaracter();
        }
        //comprobacion de sufijos
        if (sig == 'f' || sig == 'F' || sig == 'L') {
            elem->codigo = FLOAT;
            aceptarLexema(elem);
        } else {
            devolver();
            elem->codigo = FLOAT;
            aceptarLexema(elem);
        }
    } else {
        devolver();
        elem->codigo = FLOAT;
        aceptarLexema(elem);
    }
}

//automata de numeros en formato binario
void _binario(tipoelem* elem) {
    char sig = siguienteCaracter();
    /*
        * Los enteros en binario pueden contener '_' y sufijos.
        * Ejemplos:
        *      0b__100_1
        *      0B_101uL
     */
    while (sig == '0' || sig == '1' || sig == '_') {
        sig = siguienteCaracter();
    }
    if (sig == 'u' || sig == 'U') {
        sig = siguienteCaracter();
        if (sig == 'L') {
            elem->codigo = INTEGER;
            aceptarLexema(elem);
        }
    } else if (sig == 'L') {
        sig = siguienteCaracter();
        if (sig == 'u' || sig == 'U') {
            elem->codigo = INTEGER;
            aceptarLexema(elem);
        }
    } else {
        devolver();
        aceptarLexema(elem);
    }
}