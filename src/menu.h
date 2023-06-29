#ifndef MENU_H_
#define MENU_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "hash.h"

typedef struct menu_t menu_t;

// TDA Opcion
typedef struct {
    char *nombre_alias;
    char letra_alias;
    char *descripcion;
    bool (*accion)(); //menu_t* nuevo_menu, void* aux
} opcion_t;

// TDA Menu
struct menu_t {
    const char **imagenes;
    size_t cantidad_imagenes;
    hash_t* hash_nombre_opcion;
    hash_t* hash_letra_nombre;
    bool muestra_comandos_previos;
    void (*accion_en_caso_de_mal_ingreso)(menu_t* self, void* aux);
};

// Crear un nuevo menú
menu_t* menu_crear(bool mostrar_comandos_previos, const char **rutas_de_imagenes, size_t cuantas_imagenes, void (*accion)(menu_t*, void*));

//destruye la opción (libera memoria utilizada por la opción)
void opcion_destruir(void* opcion);

// Destruir el menú
bool menu_destruir(menu_t* menu);

// Agregar una opción al menú
bool menu_agregar_opcion(menu_t* menu, char* nombre, char* texto_descripcion, char letra, bool (*accion)());  //menu_t* nuevo_menu, void* aux

bool imprime_opcion_de_menu(const char *clave, void *valor, void *aux);

// Mostrar el menú y solicitar una opción al usuario
void menu_mostrar(menu_t* menu);

bool menu_get_eleccion_usuario(menu_t* menu);

void limpiar_buffer_entrada();

#endif