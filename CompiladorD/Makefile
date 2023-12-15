CC=gcc -Wall
HEADER_FILES_DIR = .
INCLUDES = -I $(HEADER_FILES_DIR)
OUTPUT = compilar
LIB_HEADERS = $(HEADER_FILES_DIR)/entrada.h $(HEADER_FILES_DIR)/lexico.h $(HEADER_FILES_DIR)/sintactico.h $(HEADER_FILES_DIR)/abb.h $(HEADER_FILES_DIR)/tablaSimbolos.h $(HEADER_FILES_DIR)/definiciones.h $(HEADER_FILES_DIR)/errores.h
SRCS = main.c abb.c entrada.c lexico.c sintactico.c tablaSimbolos.c errores.c
$(OUTPUT): $(SRCS) $(LIB_HEADERS)
	$(CC) -o $(OUTPUT) $(SRCS) $(INCLUDES)
