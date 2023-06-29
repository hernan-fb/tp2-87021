#include "menu.h"
#include "utils.h"
#define MAXIMO_INGRESO_ELECCION_USUARIO 100
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "hash.h"
#include "dibuja.h"
#include <time.h>
// TDA Lista y TDA Hash (incluidos mediante #include)

void mensaje_error_simple(menu_t* self, void* aux){
    char* ingreso = (char*) aux;
    printf("\t\tingreso inválido %s\n", ingreso);
}

// Crear un nuevo menú
menu_t* menu_crear(bool mostrar_comandos_previos, const char **rutas_de_imagenes, size_t cuantas_imagenes, void (*accion)(menu_t*, void*)) {
    menu_t* menu = malloc(sizeof(menu_t));
    if (menu == NULL) {
        return NULL;
    }

    /*menu->opciones = lista_crear();
    if (menu->opciones == NULL) {
        free(menu);
        return NULL;
    }*/
    if (accion == NULL) {
        accion = mensaje_error_simple;
    }
    menu->imagenes = rutas_de_imagenes;
    menu->cantidad_imagenes = cuantas_imagenes;
    menu->accion_en_caso_de_mal_ingreso = accion;
    menu->muestra_comandos_previos = mostrar_comandos_previos;
    menu->hash_nombre_opcion = hash_crear(13);
    if (menu->hash_nombre_opcion == NULL) {
        //lista_destruir(menu->opciones, NULL);
        free(menu);
        return NULL;
    }

    menu->hash_letra_nombre = hash_crear(13);
    if (menu->hash_letra_nombre == NULL) {
        //lista_destruir(menu->opciones, NULL);
        hash_destruir(menu->hash_nombre_opcion);
        free(menu);
        return NULL;
    }

    return menu;
}
/*
bool wrapper_free(void* dato, void* contexto) {
    free(dato);
    return true;
}
*/
void opcion_destruir(void* opcion) {
    free(((opcion_t*)opcion)->nombre_alias);
    free(((opcion_t*)opcion)->descripcion);
    free(opcion);
    return;
}

// Destruir el menú
bool menu_destruir(menu_t* menu) {
    //lista_destruir(menu->opciones, NULL);
    //hash_con_cada_clave(menu,wrapper_free,NULL);
    hash_destruir(menu->hash_letra_nombre);
    hash_destruir_todo(menu->hash_nombre_opcion,opcion_destruir);
    free(menu);
    return true;
}

// Agregar una opción al menú
bool menu_agregar_opcion(menu_t* menu, char* nombre, char* texto_descripcion, char letra, void (*accion)()) {  //menu_t* nuevo_menu, void* aux
    if (menu == NULL || nombre == NULL || accion == NULL) {
        return false;
    }
    char* descripcion_copia = strdup(texto_descripcion);
    if (descripcion_copia == NULL) {
        return false;
    }
    char* nombre_copia = strdup(nombre);
    if (nombre_copia == NULL) {
        return false;
    }
    convertirMinusculas(descripcion_copia);
    convertirMinusculas(nombre_copia);
    char* validacion_opcion = strstr(descripcion_copia, nombre);
    if (validacion_opcion == NULL || strlen(nombre) == 0) {
        printf("se intentó agregar una opción inválida, las opciones deben tener en su descripción aunque sea una ocurrencia del campo nombre\n");
        return false;
    }
    opcion_t* opcion = malloc(sizeof(opcion_t));
    if (opcion == NULL) {
        return false;
    }
    opcion->nombre_alias = NULL;
    opcion->nombre_alias = malloc(strlen(nombre) + 1);  // +1 para el carácter nulo
    if (opcion->nombre_alias == NULL) {
        printf("Error al asignar memoria.\n");
        free(opcion);
        return false;
    }
    opcion->descripcion = NULL;
    opcion->descripcion = malloc(strlen(texto_descripcion) + 1);  // +1 para el carácter nulo
    if (opcion->descripcion == NULL) {
        printf("Error al asignar memoria.\n");
        free(opcion->nombre_alias);
        free(opcion);
        return false;
    }

    strcpy(opcion->nombre_alias, nombre_copia);
    strcpy(opcion->descripcion, descripcion_copia);
    subrayarPalabra(&(opcion->descripcion),nombre_copia);
    convertirCapital(opcion->descripcion);
    opcion->accion = accion;
    opcion->letra_alias = letra;

    free(nombre_copia);
    free(descripcion_copia);
    /*if (!lista_insertar(menu->opciones, opcion)) {
        free(opcion);
        return false;
    }*/
    printf("insertando: nombre %s, opcion\n", nombre);
    if (!hash_insertar(menu->hash_nombre_opcion, nombre, opcion, NULL)) {
        //lista_borrar_primero(menu->opciones);
        free(opcion->nombre_alias);
        free(opcion->descripcion);
        free(opcion);
        return false;
    }
    printf("insertando: letra: %c, nombre: %s\n", opcion->letra_alias, nombre);
    if (!hash_insertar(menu->hash_letra_nombre, &(opcion->letra_alias), nombre, NULL)) {
        //lista_borrar_primero(menu->opciones);
        free(opcion->nombre_alias);
        free(opcion->descripcion);
        free(opcion);
        return false;
    }

    return true;
}

bool imprime_opcion_de_menu(const char *clave, void *valor, void *aux){
    opcion_t* opcion = valor;
    printf("\t%zu. %s. (%c)\n", ++(*(size_t*)aux), opcion->descripcion, opcion->letra_alias);
    return true;
}

bool wrapper_print(const char *clave, void *valor, void *aux){
    printf("\t%zu. clave: %s. valor: %s\n", ++(*(size_t*)aux), (char*)clave, (char*)valor);
    return true;
}
/*
typedef struct {
    size_t valor1;
    size_t valor2;
    char valor3;
} MiEstructura;

MiEstructura* aux = malloc(sizeof(MiEstructura));

bool busca_opcion_nro_de_menu(const char *clave, void *valor, void *aux){
    size_t opcion_nro = (size_t*) aux;
    if (*clave)
}
*/

// Mostrar el menú y solicitar una opción al usuario
void menu_mostrar(menu_t* menu) {
    if (!menu->muestra_comandos_previos) {
    //    system("clear");
    }
    srand(time(NULL));
    int numero_aleatorio = rand() % (menu->cantidad_imagenes + 1);
    dibuja_imagen(menu->imagenes[numero_aleatorio]);
    size_t contador_auxiliar = 0;
    size_t cantidad = hash_cantidad(menu->hash_nombre_opcion);
    printf("Comandos disponibles:\n");
    hash_con_cada_clave(menu->hash_nombre_opcion,imprime_opcion_de_menu, &contador_auxiliar);
}

void menu_get_eleccion_usuario(menu_t* menu) {
    printf("Ingrese la opción deseada: ");
    char texto_ingresado[MAXIMO_INGRESO_ELECCION_USUARIO] = "";
    fgets(texto_ingresado, MAXIMO_INGRESO_ELECCION_USUARIO -1, stdin);
    size_t longitud_texto_ingresado = strcspn(texto_ingresado, "\n");
    texto_ingresado[longitud_texto_ingresado] = '\0';

    convertirMinusculas(texto_ingresado);

    opcion_t * opcion_elegida = NULL;
    char* alias_elegido = NULL;
    printf("texto_ingresado: %s\n", texto_ingresado);
    if (longitud_texto_ingresado == 1) {
        char c = texto_ingresado[0];
        printf("caracter ingresado: %c", c);
        alias_elegido = hash_obtener(menu->hash_letra_nombre, &c);
        size_t contador_temporal = 0;
        hash_con_cada_clave(menu->hash_letra_nombre, wrapper_print, &contador_temporal);
        if (alias_elegido == NULL) {
            menu->accion_en_caso_de_mal_ingreso(menu, texto_ingresado);
        } else {
            printf("alias: %s", alias_elegido);
            opcion_elegida = hash_obtener(menu->hash_nombre_opcion, alias_elegido);
            if (opcion_elegida == NULL) {
                printf("hubo un error\n");
            } else {
                opcion_elegida->accion();
            }
        }
    } else {
        opcion_elegida = hash_obtener(menu->hash_nombre_opcion, texto_ingresado);
        if (opcion_elegida == NULL) {
            menu->accion_en_caso_de_mal_ingreso(menu, texto_ingresado);
        } else {
            opcion_elegida->accion();
        }
    }

    printf("\n presione una tecla para volver al menú anterior...\n");
    getchar();
}