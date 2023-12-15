#ifndef ENTRADA_H
#define ENTRADA_H

//función que inicializa el fichero, los buffers y los punteros
void inicializarSE(char *ruta);

//función que retrocede la posición del puntero delantero
void devolver();

//función que lee el lexema comprendido entre los punteros delantero e inicio
void aceptarLexema(tipoelem *elem);

//funcion de aceptacion especial para gestionar comentarios. Unicamente iguala los punteros.
void aceptarEsp();

//función que obtiene el siguiente caracter del código fuente
char siguienteCaracter();

//funcion especifica para detectar un comentario y evitar imprimir el error de lexema muy grande
void detectarComentario();

#endif