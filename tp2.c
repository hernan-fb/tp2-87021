#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "src/tp1.h"
#include "src/pokemon.h"
#include "src/pokemon_privado.h"
#include "src/lista.h"
#include "src/dibuja.h"
#include "src/menu.h"

#define MAX_HOSPITALES 10
#define MAX_NOMBRE_ARCHIVO 100

const char *imagenes[] = {"img/bulbasaur-solo.txt", "img/charmander-solo.txt", "img/squirtle-solo.txt", "img/pikatchu1.txt", NULL};
const char *imagen_bienvenida[] = {"img/_bienvenido_a_color_.txt"};
typedef enum {
    BULBASAUR,
    CHARMANDER,
    SQUIRTLE,
    PIKACHU
} pokebola_t;

typedef struct {
    int id;
    char nombreArchivo[MAX_NOMBRE_ARCHIVO];
    hospital_t* hospital;
} hospital_info_t;

hospital_info_t hospitales[MAX_HOSPITALES];
int hospitales_cargados = 0;
int hospital_activo = -1;

int obtener_numero_hospital_activo() {
    if (hospital_activo == -1) {
        printf("No hay hospital activo.\n");
        return -1;
    }
    return hospital_activo;
}

void comando_salir() {
    // Liberar la memoria de los hospitales cargados
    for (int i = 0; i < hospitales_cargados; i++) {
        hospital_destruir(hospitales[i].hospital);
    }
    exit(0);
}

void comando_cargar() {
    char nombreArchivo[MAX_NOMBRE_ARCHIVO];
    printf("Ingrese el nombre del archivo: ");
    scanf("%s", nombreArchivo);

    if (hospitales_cargados >= MAX_HOSPITALES) {
        printf("No se pueden cargar más hospitales. Límite alcanzado.\n");
        return;
    }

    hospital_t* hospital = hospital_crear_desde_archivo(nombreArchivo);
    if (hospital == NULL) {
        printf("No se pudo cargar el hospital desde el archivo.\n");
        return;
    }

    hospitales[hospitales_cargados].id = hospitales_cargados + 1;
    strcpy(hospitales[hospitales_cargados].nombreArchivo, nombreArchivo);
    hospitales[hospitales_cargados].hospital = hospital;
    hospitales_cargados++;

    printf("Hospital cargado exitosamente.\n");
}

void comando_estado() {
    printf("Hospitales cargados:\n");
    for (int i = 0; i < hospitales_cargados; i++) {
        printf("%d. %s\n", hospitales[i].id, hospitales[i].nombreArchivo);
    }

    int numero_hospital_activo = obtener_numero_hospital_activo();
    if (numero_hospital_activo != -1) {
        printf("Hospital activo: %d. %s\n", hospitales[numero_hospital_activo].id, hospitales[numero_hospital_activo].nombreArchivo);
    }
}

void comando_activar() {
    int id;
    printf("Ingrese el número de identificación del hospital a activar: ");
    scanf("%d", &id);

    if (id < 1 || id > hospitales_cargados) {
        printf("Número de hospital inválido.\n");
        return;
    }

    hospital_activo = id - 1;
    printf("Hospital activado: %d. %s\n", hospitales[hospital_activo].id, hospitales[hospital_activo].nombreArchivo);
}

bool wrapper_imprimir_pokemon(void* poke, void* contexto) {
	printf("Este es el pokemon \tid: \t\t%zu,\n\t\t\tNombre: \t%s\n\t\t\tEntrenador:\t%s\n\t\t\tSalud:\t\t%zu\n", 
	pokemon_id((pokemon_t*)poke), 
	pokemon_nombre((pokemon_t*)poke),
	pokemon_entrenador((pokemon_t*)poke),
	pokemon_salud((pokemon_t*)poke));
	return true;
}
bool wrapper_imprimir_nombre_pokemon(void* poke, void* contexto) {
	printf("Este es el pokemon \tNombre: \t%s\n", pokemon_nombre((pokemon_t*)poke));
	return true;
}

void listar_pokemones(hospital_t* hospital, bool wrapper(void*, void*)) {
    lista_con_cada_elemento(hospital->pokemones, wrapper, NULL);
	return;
}

void comando_listar_nombres() {
    int numero_hospital_activo = obtener_numero_hospital_activo();
    if (numero_hospital_activo != -1) {
        hospital_t* hospital = hospitales[numero_hospital_activo].hospital;
		listar_pokemones(hospital, wrapper_imprimir_nombre_pokemon);
    }
	return;
}

void comando_listar_en_detalle() {
    int numero_hospital_activo = obtener_numero_hospital_activo();
    if (numero_hospital_activo != -1) {
        hospital_t* hospital = hospitales[numero_hospital_activo].hospital;
        listar_pokemones(hospital, wrapper_imprimir_pokemon);
    }
}

void comando_destruir() {
    int numero_hospital_activo = obtener_numero_hospital_activo();
    if (numero_hospital_activo != -1) {
        hospital_destruir(hospitales[numero_hospital_activo].hospital);
        printf("Hospital destruido.\n");
        // Mover los hospitales restantes hacia la izquierda en el arreglo hospitales
        for (int i = numero_hospital_activo; i < hospitales_cargados - 1; i++) {
            hospitales[i] = hospitales[i + 1];
        }
        hospitales_cargados--;
        hospital_activo = -1;
    }
}

void comando_mostrar_creditos() {
    printf("\n\nEste es un programa de licencia libre desarrollado por el alumno Hernán Fernández Brando\n");
    printf("\tEn la cátedra \"Mendez\" de Algoritmos y programación 2,\n");
    printf("\ten el primer ciclo lectivo del año MMXXIII.\n");
    printf("\t\tCopyright 2023 © - todos los derechos están reservados.\n");
}

menu_t* crear_menu_principal() {
    menu_t* menu_principal = menu_crear(false, imagenes, 4, NULL);
    menu_agregar_opcion(menu_principal, "salir", "Salir del programa.", 's', comando_salir);
    menu_agregar_opcion(menu_principal, "ayuda", "Muestra este menú de ayuda.", 'h', comando_salir);
    menu_agregar_opcion(menu_principal, "cargar", "Desde un archivo cargar un hospital.", 'c', comando_cargar);
    menu_agregar_opcion(menu_principal, "mostrar", "Mostrar todos los hospitales: los cargados y el activo.", 'e', comando_estado);
    menu_agregar_opcion(menu_principal, "activar", "Activar un hospital por su número de identificación.", 'a', comando_activar);
    menu_agregar_opcion(menu_principal, "listar nombres", "Listar nombres de los pokemones del hospital activo.", 'm', comando_listar_nombres);
    menu_agregar_opcion(menu_principal, "listar en detalle", "Listar en detalle los pokemones del hospital activo.", 'l', comando_listar_en_detalle);
    menu_agregar_opcion(menu_principal, "destruir", "Destruir el hospital activo.", 'd', comando_destruir);
    return menu_principal;
}

void comando_menu_principal() {
    menu_t* menu_principal = crear_menu_principal();
    while(1) {
        menu_mostrar(menu_principal);
        menu_get_eleccion_usuario(menu_principal);
    }
}

menu_t* crear_menu_bienvenida() {
    menu_t* menu_bienvenida = menu_crear(false, imagen_bienvenida, 1, NULL);
    menu_agregar_opcion(menu_bienvenida, "salir", "Salir del programa.", 's', comando_salir);
    menu_agregar_opcion(menu_bienvenida, "jugar", "Empezar a jugar con los pokes.", 'j', comando_menu_principal);
    menu_agregar_opcion(menu_bienvenida, "creditos", "Creditos con copyright HernánF.B.", 'c', comando_mostrar_creditos);
    return menu_bienvenida;
}

int main() {
    char opcion;

    menu_t* menu_bienvenida = crear_menu_bienvenida();

    while(1) {
        menu_mostrar(menu_bienvenida);
        menu_get_eleccion_usuario(menu_bienvenida);
    }
    return 0;
}